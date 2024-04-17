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

Question Game::getQuestionForUser(LoggedUser& user) const
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
