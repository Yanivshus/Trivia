#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <io.h>
#include <map>

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
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
private:
	bool runQuery(const std::string& query);
	std::map<std::string, std::string> userList;
	sqlite3* _db;
};
