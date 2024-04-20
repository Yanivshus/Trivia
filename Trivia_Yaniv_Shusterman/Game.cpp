#include "Game.h"

Game::Game(std::vector<Question> questions, std::vector<LoggedUser> users, const int gameId, Room room)
{
	this->m_gameId = gameId;
	this->m_room = room;

	//add all the questions.
	for (const auto& q : questions)
	{
		this->m_questions.push_back(q);
	}

	//add all the users to the game for stat control.
	for (auto& user : users)
	{
		GameData data = GameData(this->m_questions[0], 0, 0, 0);
		this->m_players[user] = data; // set every user to deafult stats and the first questions.
	}
}

Question Game::getQuestionForUser(const LoggedUser& user) const
{
	// run on the players and look for asked pplayer.
	for (auto& player : this->m_players)
	{
		// if user found we will return his question.
		if (user.getUserName() == player.first.getUserName()) 
		{
			return player.second.currentQuestion;
		}
	}

	return Question("", std::vector<std::string>());
}

void Game::submitAnswer(const LoggedUser& user, const unsigned int ans)
{

	GameData* data = nullptr;
	// run on the players and look for asked player.
	for (auto& player : this->m_players)
	{
		if (user.getUserName() == player.first.getUserName())
		{
			data = &player.second; // save the player game data.
		}
	}

	// check if the questions are over.
	if (data->currentQuestion.getQuestion() != "over")
	{
		// check if the answer was correct.
		if (ans == 3) {
			data->correctAnswerCount++; // if yes we will update the correct answer count.
		}
		else {
			data->wrongAnswerCount++; // if not we will update the wrong answer count.
		}

		// now i will set the next question index.
		int place = 0;
		int flag = 0;
		for (place = 0; place < this->m_questions.size() && flag == 0; place++)
		{
			// if the current question found i will save its index.
			if (this->m_questions[place].getQuestion() == data->currentQuestion.getQuestion()) {
				flag++;
			}
		}
		 
		// set the next question the user will have to answer by its index.
		if (place + 1 != this->m_questions.size()) {
			data->currentQuestion = this->m_questions[place + 1];
		}
		else {
			data->currentQuestion = Question("over", std::vector<std::string>());

			data->averageAnswerTime = (double)time(0) - data->averageAnswerTime; // find the time differnce between the start of the game and now which is the end.

			data->averageAnswerTime /= (data->correctAnswerCount + data ->wrongAnswerCount); // divide the time difference by amount of question to find avg time per question.
		}
	}

}

void Game::removePlayer(const LoggedUser& user)
{

	auto it = this->m_players.find(user);// look for the user.
	if (it != this->m_players.end()) // if found earse it.
	{
		this->m_players.erase(user);
	}
	else {
		throw std::exception("User not exists.");
	}
}

bool Game::checkIfAllPlayersFinishedPlaying() const
{
	for (auto& user : this->m_players)
	{
		// if there is a user who didnt finish we will return false.
		if(user.second.currentQuestion.getQuestion() != "over") 
		{
			return false;
		}
	}
	return true;// else true will be returned.
}

std::map<LoggedUser, GameData>& Game::getPlayersInGame()
{
	return this->m_players;
}

int Game::getGameId() const
{
	return this->m_gameId;
}
