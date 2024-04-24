#include "GameManager.h"

void GameManager::setDB(IDatabase* m_database)
{
	this->m_database = m_database;
}

Game& GameManager::createGame(const Room& room)
{
	// get questions from the db.
	std::list<Question> questions_list = this->m_database->getQuestions(room.getRoomData().numOfQuestionsInGame);

	std::vector<Question> questions(questions_list.begin(), questions_list.end()); // turn them into a vector.


	Game* game = new Game(questions, room.getAllUsers(), room.getRoomData().id, room); // create the game with the right parameters.

	// update the statring time of the game so in the end of it i could calculate avg answer time.
	for (auto& player : game->getPlayersInGame())
	{
		player.second.averageAnswerTime = (double)time(0);
	}

	this->m_games.push_back(*game); // add to active games.
	this->m_database->addGameToGames(room.getRoomData().id);// add the id of the game to the db for later use.
	return *game;
}


void GameManager::deleteGame(const unsigned int gameId)
{
	//run on the games.
	for (auto i = this->m_games.begin(); i != this->m_games.end(); )
	{
		// if game found it will be deleted.
		if (i->getGameId() == gameId)
		{
			i = this->m_games.erase(i);
			return;
		}
		else {
			++i;
		}
	}
	throw std::exception("Couldn't delete");
}

Game& GameManager::getGame(const int gameId)
{
	for (auto& game : this->m_games)
	{
		//return the game if found.
		if (game.getGameId() == gameId) 
		{
			return game;
		}
	}
}
