#pragma once
#include "LoggedUser.h"
#include "map"
#include "Question.h"
#include "GameData.h"
#include "Room.h"


class Game {
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	int m_gameId;
	Room m_room;
public:
	Game(std::vector<Question> questions, std::vector<LoggedUser> users, const int gameId, Room room);

	/// <summary>
	/// returns a question for user if the question arent over.
	/// </summary>
	/// <param name="user:">user who asked for another question.</param>
	Question getQuestionForUser(const LoggedUser& user) const;

	/// <summary>
	/// submits an answer, check if it corrrect and update the stats by that.
	/// </summary>
	/// <param name="user:">user who submited.</param>
	/// <param name="ans:">answer index.</param>
	void submitAnswer(const LoggedUser& user, const unsigned int ans);

	/// <summary>
	/// removes user from a game.
	/// </summary>
	/// <param name="user:">user to remove.</param>
	void removePlayer(const LoggedUser& user);

	/// <summary>
	/// returns true if all the users finished playing.
	/// </summary>
	bool checkIfAllPlayersFinishedPlaying() const;

	//getters.
	std::map<LoggedUser, GameData>& getPlayersInGame();
	int getGameId();

};