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

	Question getQuestionForUser(LoggedUser& user) const;
	void submitAnswer(const LoggedUser& user, const unsigned int ans);
	void removePlayer(const LoggedUser& user);
	bool checkIfAllPlayersFinishedPlaying() const;

	//getters.
	std::map<LoggedUser, GameData>& getPlayersInGame();
	int getGameId();

};