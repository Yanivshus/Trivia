#pragma once
#include <iostream>
#include <WinSock2.h>

class LoggedUser{
private:
	std::string m_username;
	SOCKET sock;
public:
	LoggedUser(const LoggedUser& other);
	LoggedUser(const std::string& username, SOCKET sock);
	std::string getUserName(); // getter
	SOCKET getSock();
};
