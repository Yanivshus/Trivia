#pragma once
#include <iostream>
#include "IDatabase.h"
#include "LoggedUser.h"
#include <vector>
#include <exception>
#include <regex>
#include <functional>

class LoginManager {
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
public:
	/// <summary>
	/// signup the user to the database.
	/// </summary>
	/// <param name="username:">user usernaem</param>
	/// <param name="password:">user password</param>
	/// <param name="email:">user email</param>
	void signup(const std::string& username, const std::string& password, const std::string& email, const std::string& addres, const std::string& phone, const std::string& date);

	/// <summary>
	/// login to a user if possiable.
	/// </summary>
	/// <param name="username:">username of user to log to.</param>
	/// <param name="password">password of user to log to.</param>
	void login(const std::string& username, const std::string& password, SOCKET sock);

	/// <summary>
	/// logout from user acount.
	/// </summary>
	/// <param name="username:">user the logout from.</param>
	void logout(const std::string& username);

	void setDB(IDatabase* m_database);
};