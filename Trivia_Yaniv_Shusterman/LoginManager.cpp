#include "LoginManager.h"


void LoginManager::signup(const std::string& username, const std::string& password, const std::string& email, const std::string& addres, const std::string& phone, const std::string& date)
{
	if (this->m_database->doesUserExist(username) == 1) // if the user already exists we cant add him.
	{
		throw std::exception("User already exists.");
	}
	else
	{
		std::regex passCheck("^(?=.*[A-Z])(?=.*[a-z])(?=.*[*&^%$#@!]).{8,}$");
		if(!std::regex_match(password, passCheck))
			throw std::exception("regex error.");
		
		std::regex mailCheck("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$");
		if (!std::regex_match(email, mailCheck))
			throw std::exception("regex error.");
		//std::string s = "n"
		std::regex addCheck("^[a-zA-Z]+, \\d+, [a-zA-Z ]+$");
		if (!std::regex_match(addres, addCheck))
			throw std::exception("regex error.");

		std::regex phoneCheck("^0\\d{9}$");
		if (!std::regex_match(phone, phoneCheck))
			throw std::exception("regex error.");

		std::regex dateCheck("^(0[1-9]|[12][0-9]|3[01])[./](0[1-9]|1[0-2])[./](\\d{4})$");
		if (!std::regex_match(date, dateCheck))
			throw std::exception("regex error.");

		std::hash<std::string> hasher;

		// if everything passed the regex we will add the user.
		this->m_database->addNewUser(username, std::to_string(hasher(password)), email, addres, phone, date);
	}
}

void LoginManager::login(const std::string& username, const std::string& password, SOCKET sock)
{
	std::hash<std::string> hasher;
	if (this->m_database->doesUserExist(username) == 1)// check if the user exists before we login.
	{
		if (this->m_database->doesPasswordMatch(username, std::to_string(hasher(password))))//if exists we try to login with username and password.
		{
			//check if the user already connected.
			for (auto& user : this->m_loggedUsers)
			{
				if (user.getUserName() == username) {
					throw std::exception("User already connected.");
				}
			}

			//add new logged user.
			LoggedUser newLoggedUser(username, sock);
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
