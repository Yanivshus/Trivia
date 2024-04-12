#include "LoggedUser.h"

LoggedUser::LoggedUser(const LoggedUser& other)
{
	this->m_username = other.m_username;
	this->sock = other.sock;
}

LoggedUser::LoggedUser(const std::string& username, SOCKET sock)
{
	this->m_username = username;
	this->sock = sock;
}

std::string LoggedUser::getUserName()
{
	return this->m_username;
}

SOCKET LoggedUser::getSock()
{
	return this->sock;
}
