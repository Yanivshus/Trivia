#include "Game.h"
#include "IDatabase.h"
#include "Question.h"
#include <vector>


class GameManager {
private:
	IDatabase* m_database;
	std::vector<Game> m_games;
public:
	void setDB(IDatabase* m_database);
	Game& createGame(const Room& room);
	void deleteGame(unsigned int gameId);
};