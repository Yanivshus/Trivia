#pragma once
#include <iostream>

class LoggedUser {
private:
	std::string m_username;
public:
	LoggedUser(const std::string& username);
	std::string getUserName();
};
