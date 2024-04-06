#pragma once
#include <iostream>

class LoggedUser {
private:
	std::string m_username;
public:
	LoggedUser(const LoggedUser& other);
	LoggedUser(const std::string& username);
	std::string getUserName();
};
