#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <io.h>
#include <list>

#define DB_NAME "triviaDB.db"

class SqliteDataBase : public IDatabase {
public:
	virtual bool open() override;
	virtual bool close() override;

	virtual int doesUserExist(const std::string& username) override;
	virtual int doesPasswordMatch(const std::string& username, const std::string& password) override;
	virtual int addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
private:
	bool runQuery(const std::string& query);
	std::list<std::string> userNames;
	sqlite3* _db;
};
