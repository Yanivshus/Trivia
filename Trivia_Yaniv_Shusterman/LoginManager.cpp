#include "LoginManager.h"



LoginManager::LoginManager()
{
}

void LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	if (this->m_database->doesUserExist(username) == 1) // if the user already exists we cant add him.
	{
		throw std::exception("User already exists.");
	}
	else
	{
		this->m_database->addNewUser(username, password, email);
	}
}

void LoginManager::login(const std::string& username, const std::string& password)
{
	if (this->m_database->doesUserExist(username) == 1)// check if the user exists before we login.
	{
		if (this->m_database->doesPasswordMatch(username, password))//if exists we try to login with username and password.
		{
			//add new logged user.
			LoggedUser newLoggedUser(username);
			this->m_loggedUsers.push_back(newLoggedUser);
		}
		else
		{
			throw std::exception("Password is wrong.");
		}
	}
	else {
		throw std::exception("User doesn't exists.");
	}
}

void LoginManager::logout(const std::string& username)
{
	// check if user exusts in the database.
	if (this->m_database->doesUserExist(username) == 1)
	{
		std::vector<LoggedUser>::iterator userToDelete;
		int found = 0;
		//try to find the user in the logged user vector.
		for (auto i = this->m_loggedUsers.begin(); i != this->m_loggedUsers.end() && found == 0; i++)
		{
			// if user was found i will save his iterator.
			if (i->getUserName() == username) 
			{
				userToDelete = i;
				found = 1;// set the found value to found.
			}
		}

		if (found == 0)
		{
			throw std::exception("User isn't logged");
		}
		else
		{
			this->m_loggedUsers.erase(userToDelete);
		}
	}
	else {
		throw std::exception("User doesn't exists.");
	}
}

void LoginManager::setDB(IDatabase* m_database)
{
	this->m_database = m_database;
}
