#include "SqliteDataBase.h"

int callBackGetQuestions(void* data, int argc, char** argv, char** azColName) 
{
	//turing the data to static so if there will be a couple of runs of the callback the data will remain.
	auto& questionsL = *static_cast<std::list<Question>*>(data);

	
	std::vector<std::string> Possiable;
	std::string correct = "";
	std::string quest = "";
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "question") {
			quest = argv[i];
		}
		if (std::string(azColName[i]) == "w_answer1") {
			Possiable.push_back(argv[i]);
		}
		if (std::string(azColName[i]) == "w_answer2") {
			Possiable.push_back(argv[i]);
		}
		if (std::string(azColName[i]) == "w_answer3") {
			Possiable.push_back(argv[i]);
		}
		if (std::string(azColName[i]) == "c_answer4") {
			Possiable.push_back(argv[i]);
		}
	}

	Question newQ(quest, Possiable);
	questionsL.push_back(newQ);
	return 0;
}




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

		query = "CREATE TABLE IF NOT EXISTS questions (ID INTEGER, question TEXT, w_answer1 TEXT, w_answer2 TEXT, w_answer3	TEXT,c_answer4	TEXT, PRIMARY KEY(ID AUTOINCREMENT));";

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

std::list<Question> SqliteDataBase::getQuestions(const int amount)
{
	std::string query = "SELECT question, w_answer1, w_answer2, w_answer3, c_answer4 FROM questions ORDER BY RANDOM() LIMIT "+std::to_string(amount)+";";
	this->questions.clear();

	char* errMsg = nullptr;
	//if qeury didnt worked we will print why.
	if (sqlite3_exec(this->_db, query.c_str(), callBackGetQuestions, &this->questions, &errMsg) != SQLITE_OK) {
		std::cout << "sql err" << std::endl;
		std::cout << "reason: " << errMsg << std::endl;
	}

	return this->questions;
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
