#include "SqliteDataBase.h"
int callBackGetUsers(void* data, int argc, char** argv, char** azColName)
{
	//turing the data to static so if there will be a couple of runs of the callback the data will remain.
	auto& usersMap = *static_cast<std::map<std::string,std::string>*>(data);
	
	std::string userName;
	std::string password;
	//creating an map entry.
	for (int i = 0; i < argc; i++)
	{

		if (std::string(azColName[i]) == "username")
		{
			userName = argv[i];
		}
		else if (std::string(azColName[i]) == "password")
		{
			password = argv[i];
		}
	}
	usersMap[userName] = password;
	return 0;
}

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
		std::string query = "CREATE TABLE IF NOT EXISTS users (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL);";

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
	std::string query = "SELECT username,password FROM USERS WHERE username='"+username+"';";
	this->userList.clear();

	char* errMsg = nullptr;
	//if qeury didnt worked we will print why.
	if (sqlite3_exec(this->_db, query.c_str(), callBackGetUsers, &this->userList, &errMsg) != SQLITE_OK) {
		std::cout << "sql err" << std::endl;
		std::cout << "reason: " << errMsg << std::endl;
	}

	//if user wasnt found we will return 0 for false.
	if (this->userList.empty() == true) {
		return 0;
	}
	return 1;//we will return 1 for true, user exists.
	
}

int SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	std::string query = "SELECT username,password FROM USERS WHERE username='"+username+"' AND password='"+password+"';";// query to select username and password from the db.
	this->userList.clear();

	char* errMsg = nullptr;
	//if qeury didnt worked we will print why.
	if (sqlite3_exec(this->_db, query.c_str(), callBackGetUsers, &this->userList, &errMsg) != SQLITE_OK) {
		std::cout << "sql err" << std::endl;	
		std::cout << "reason: " << errMsg << std::endl;
	}

	// if no matched user found, we will return false
	if (this->userList.empty() == true) {
		return 0;
	}
	return 1;// else if found a match true will be returned.
}

int SqliteDataBase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	std::string query = "INSERT INTO USERS (username, password, email) values('"+username+"', '"+password+"', '"+email+"');";
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
		return 0;
	}
	return 1;
}
