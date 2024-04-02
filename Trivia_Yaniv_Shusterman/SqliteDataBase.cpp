#include "SqliteDataBase.h"

bool SqliteDataBase::open()
{
	int fileExists = _access(DB_NAME, 0);//checking if file exists.
	int res = sqlite3_open(DB_NAME, &this->_db);
	if (res != SQLITE_OK)
	{
		this->_db = nullptr;
		std::cout << "Failes to open db" << std::endl;
	}

	if (fileExists != 0)
	{
		// creating the tables.
		std::string query = "CREATE TABLE IF NOT EXISTS users (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL)";

		if (!runQuery(query))
		{
			return false;
		}
	}
	return true;
}

bool SqliteDataBase::close()
{
	if (sqlite3_close(this->_db) == SQLITE_OK) 
	{
		delete this->_db;
		return true;
	}
	return false;
}

int SqliteDataBase::doesUserExist(const std::string& username)
{
	return 0;
}

int SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	return 0;
}

int SqliteDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	std::string query = "INSERT INTO USERS (username, password, email) values('"+username+"', '"+password+"', '"+email+"')";
	return runQuery(query);
}

bool SqliteDataBase::runQuery(const std::string& query)
{
	char* errMsg = nullptr;
	int res = sqlite3_exec(this->_db, query.c_str(), nullptr, nullptr, &errMsg);//execute the query
	//if qeury didnt worked we will print why.
	if (res != SQLITE_OK) {
		std::cout << "sql err" << std::endl;
		std::cout << "reason: " << errMsg << std::endl;
		return false;
	}
	return true;
}
