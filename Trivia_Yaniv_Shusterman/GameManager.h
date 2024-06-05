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

	/// <summary>
	/// creates an instance of a game out of a room when admin starts game.
	/// </summary>
	Game& createGame(const Room& room);

	/// <summary>
	/// delete game from the active games vector.
	/// </summary>
	void deleteGame(const unsigned int gameId);

	/// <summary>
	/// returns instance of game.
	/// </summary>
	Game& getGame(const int gameId);

	////singleton by reference.
	static GameManager& getInstance() {
		static GameManager theGameManager;
		return theGameManager;
	}
};