#include "LoggedUser.h"

LoggedUser::LoggedUser(const LoggedUser& other)
{
	this->m_username = other.m_username;
	this->sock = other.sock;
}

LoggedUser::LoggedUser(const std::string& username, const SOCKET sock)
{
	this->m_username = username;
	this->sock = sock;
}

std::string LoggedUser::getUserName() const
{
	return this->m_username;
}

SOCKET LoggedUser::getSock() const 
{
	return this->sock;
}

bool LoggedUser::operator==(const LoggedUser& user) const
{
	if (this->m_username == user.m_username) {
		return true;
	}
	return false;
}

bool LoggedUser::operator<(const LoggedUser& user) const
{
	if (this->m_username.size() < user.m_username.size()) {
		return true;
	}
	return false;
}
