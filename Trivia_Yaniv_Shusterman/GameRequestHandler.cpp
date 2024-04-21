#include "GameRequestHandler.h"

bool GameRequestHandler::isRequestRelevant(RequestInfo info)
{
	if (info.id == GET_GAME_RESULTS_REQUEST ||
		info.id == SUBMIT_ANSWER_REQUEST ||
		info.id == GET_QUESTION_REQUEST ||
		info.id == LEAVE_GAME_REQUEST) 
	{
		return true;
	}
	return false;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo info, SOCKET sock, std::map<SOCKET, IRequestHandler*>& m_clients)
{
	try 
	{
		if (info.id == GET_QUESTION_REQUEST) {
			return this->getQuestion(info);
		}
		if (info.id == SUBMIT_ANSWER_REQUEST) {
			return this->submitAnswer(info);
		}
		if (info.id == GET_GAME_RESULTS_REQUEST) {
			return this->getGameResult(info);
		}
		if (info.id == LEAVE_GAME_REQUEST) {
			return this->leaveGame(info);
		}
		
	}
	catch (std::exception& e) // if eny thrown exceptions caught , return a error response.
	{
		std::cout << e.what() << std::endl;
		ErrorResponse res = { "FAILED" };
		return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
	}
	catch (nlohmann::json::exception& e)
	{
		ErrorResponse res = { "ERROR : INVALID JSON " };
		return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
	}
}

GameRequestHandler::GameRequestHandler(const LoggedUser& m_user, RequestHandlerFactory& m_handlerFactory, Game& game) : m_user(m_user) , m_handlerFactory(m_handlerFactory) , m_game(game)
{
}

RequestResult GameRequestHandler::getQuestion(RequestInfo info)
{
	try {
		// get the current active game.
		Game& game = this->m_handlerFactory.getGameManager().getGame(this->m_game.getGameId());
		Question curr = game.getQuestionForUser(this->m_user); // get user current question.
		std::map<unsigned int, std::string> answers;
		unsigned int i = 0;

		// run on the answers.
		for (auto& ans : curr.getPossiableAnswers())
		{
			answers[i] = ans;
			i++;
		}
		

		// build the res packet.
		GetQuestionResponse res = { GET_QUESTION_RESPONSE, curr.getQuestion(), answers };
		return { JsonResponsePacketSerializer::serializeResponse(res), nullptr };
	}
	catch (std::exception& e) // if eny thrown exceptions caught , return a error response.
	{
		throw e;
	}
	catch (nlohmann::json::exception& e)
	{
		throw e;
	}
}

RequestResult GameRequestHandler::submitAnswer(RequestInfo info)
{
	try {
		// get the object out of the bson.
		SubmitAnswerRequest req = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
		// submit the answer to the game.
		this->m_handlerFactory.getGameManager().getGame(this->m_game.getGameId()).submitAnswer(this->m_user, req.answerId);
		SubmitAnswerResponse res = { SUBMIT_ANSWER_RESPONSE, 3 }; // the correct answer alwayes will be in the 3 spot.
		return { JsonResponsePacketSerializer::serializeResponse(res), nullptr };
	}
	catch (std::exception& e) // if eny thrown exceptions caught , return a error response.
	{
		throw e;
	}
	catch (nlohmann::json::exception& e)
	{
		throw e;
	}


}

RequestResult GameRequestHandler::getGameResult(RequestInfo info)
{
	try {
		std::vector<PlayerResults> results;
		if (this->m_handlerFactory.getGameManager().getGame(this->m_game.getGameId()).checkIfAllPlayersFinishedPlaying() == true)
		{
			// run on all the players.
			for (auto& player : this->m_handlerFactory.getGameManager().getGame(this->m_game.getGameId()).getPlayersInGame())
			{
				unsigned int score = ((player.second.correctAnswerCount + player.second.wrongAnswerCount) * 100) / player.second.averageAnswerTime; // calculate the score by the formula.
				PlayerResults playerRes = { player.first.getUserName(), player.second.correctAnswerCount, player.second.wrongAnswerCount, player.second.averageAnswerTime, score }; // make a player result instance.
				
				// if the player hasn't left the game i will save his results.
				if (player.second.hasPlayersFinished != true) {
					this->saveInDB(player.first, player.second); // save the user stats to the db.
				}

				results.push_back(playerRes); // add to the vector of players results.
			}


			// delete the room and the game because all the user finished.
			this->m_handlerFactory.getRoomManager().deleteRoom(this->m_game.getGameId());
			this->m_handlerFactory.getGameManager().deleteGame(this->m_game.getGameId());

			// return to the user the result and create an menu instance because the game is finished.
			GetGameResultsResponse res = { 1, results };
			return { JsonResponsePacketSerializer::serializeResponse(res), (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
		}

		else {
			// return to the user an empty results because the players didnt finished yet.
			GetGameResultsResponse res = { 0, results };
			return { JsonResponsePacketSerializer::serializeResponse(res), nullptr };
		}
	}
	catch (std::exception& e) // if eny thrown exceptions caught , return a error response.
	{
		throw e;
	}
	catch (nlohmann::json::exception& e)
	{
		throw e;
	}
}

RequestResult GameRequestHandler::leaveGame(RequestInfo info)
{
	try 
	{
		for (auto& player : this->m_handlerFactory.getGameManager().getGame(this->m_game.getGameId()).getPlayersInGame())
		{
			if (player.first.getUserName() == this->m_user.getUserName())
			{
				player.second.currentQuestion = Question("over", std::vector<std::string>()); // set the game to finished.
				player.second.averageAnswerTime = (double)time(0) - player.second.averageAnswerTime; // find the time differnce between the start of the game and now which is the end.
				player.second.averageAnswerTime /= (player.second.correctAnswerCount + player.second.wrongAnswerCount); // divide the time difference by amount of question to find avg time per question.

				// save to db.
				this->saveInDB(player.first, player.second);
				player.second.hasPlayersFinished = true; // seet the player to be finished so i won't save his stats twice in the db when getting the results.
			}
		}



		//return menu instance to user.
		LeaveRoomResponse res = { LEAVE_GAME_RESPONSE };
		return { JsonResponsePacketSerializer::serializeResponse(res), (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
	}
	catch (std::exception& e) // if eny thrown exceptions caught , return a error response.
	{
		throw e;
	}
	catch (nlohmann::json::exception& e)
	{
		throw e;
	}
}

void GameRequestHandler::saveInDB(const LoggedUser& user, const GameData& data)
{
	int id = this->m_game.getGameId();
	std::string name = user.getUserName();
	this->m_handlerFactory.getDB()->submitGameStatistics(data, id, name); // save the results to the db.
}
