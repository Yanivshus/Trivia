#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <io.h>
#include <string>
#include <map>
#include <mutex>


#define DB_NAME "triviaDB.db"

class SqliteDataBase : public IDatabase {
public:
	virtual bool open() override;
	virtual bool close() override;

	/// <summary>
	/// if 1 returned it means a user exists. 0 means it doesn't exists.
	/// </summary>
	/// <param name="username:">username of the user.</param>
	/// <returns>1 or 0</returns>
	virtual int doesUserExist(const std::string& username) override;

	/// <summary>
	/// checks if password match the username and its password, if yes true(1) will return, else false(0) will return.
	/// </summary>
	/// <param name="username:">username to check</param>
	/// <param name="password:">password to check</param>
	/// <returns>1 or 0</returns>
	virtual int doesPasswordMatch(const std::string& username, const std::string& password) override;

	/// <summary>
	/// adds new user to db.
	/// </summary>
	/// <param name="username:">user username</param>
	/// <param name="password:">user password</param>
	/// <param name="email:">user email</param>
	/// <returns>true if worked.</returns>
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email, const std::string& addres, const std::string& phone, const std::string& date) override;

	/// <summary>
	/// returns all questions randomized, by requested amount.
	/// </summary>
	/// <param name="amount:">amount of questions to get.</param>
	/// <returns>list of questions.</returns>
	virtual std::list<Question> getQuestions(const int amount) override;

	/// <summary>
	/// get the requested player avarege answer time.
	/// </summary>
	/// <param name="username:">username of the user we want his avg time.</param>
	/// <returns>avg time.</returns>
	virtual double getPlayerAverageAnswerTime(const std::string& username) override;

	/// <summary>
	/// get the number of correct answers of user.
	/// </summary>
	/// <param name="username:">username of the user we want his number of correct answers.</param>
	/// <returns>number of correct.></returns>
	virtual int getNumOfCorrectAnswers(const std::string& username) override;

	/// <summary>
	/// get the number of total answers of user.
	/// </summary>
	/// <param name="username:">username of the user we want his number of answers</param>
	/// <returns>number of answers.</returns>
	virtual int getNumOfTotalAnswers(const std::string& username) override;

	/// <summary>
	/// get the number of games a player played.
	/// </summary>
	/// <param name="username:">username of the user we want his number of games</param>
	/// <returns>number of games</returns>
	virtual int getNumOfPlayerGames(const std::string& username) override;

	/// <summary>
	/// get player score.
	/// </summary>
	/// <param name="username:">user name of the user we want his score.</param>
	/// <param name="gameId:">score of a specific game.</param>
	/// <returns>player score.</returns>
	virtual int getPlayerScore(const std::string& username, int gameId) override;

	/// <summary>
	/// get top three players with the highest scores.
	/// </summary>
	/// <returns>vector of scores in the format , username=score</returns>
	virtual std::vector<std::string> getHighScores();

	/// <summary>
	/// submits scores to the db when game finished.
	/// </summary>
	virtual int submitGameStatistics(GameData data, int gameId, const std::string& uNmae) override;

	/// <summary>
	/// add a game to games table.
	/// </summary>
	/// <param name="gameId:">gamed id to add.</param>
	virtual int addGameToGames(const unsigned int gameId) override;

	/// <summary>
	/// add question to database.
	/// </summary>
	/// <param name="question:">the question</param>
	/// <param name="w_answer1:">first wrong answer</param>
	/// <param name="w_answer2:">second wrong answer</param>
	/// <param name="w_answer3:">third wrong answer</param>
	/// <param name="c_answer4:">correct answer</param>
	/// <returns></returns>
	virtual int addQuestionToDB(const std::string& question, const std::string& w_answer1, const std::string& w_answer2, const std::string& w_answer3, const std::string& c_answer4) override;

	static SqliteDataBase* getInstance() {
		if (_sqliteDb == nullptr) {
			_sqliteDb = new SqliteDataBase;
		}
		return _sqliteDb;
	}
private:
	// runs a query.
	bool runQuery(const std::string& query);
	std::map<std::string, std::string> userList;
	std::list<Question> questions;
	sqlite3* _db;
	std::mutex dbMutex;
	static SqliteDataBase* _sqliteDb;
};
