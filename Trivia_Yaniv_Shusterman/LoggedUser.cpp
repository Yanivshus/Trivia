#include "LoggedUser.h"

LoggedUser::LoggedUser(const LoggedUser& other)
{
	this->m_username = other.m_username;
}

LoggedUser::LoggedUser(const std::string& username)
{
	this->m_username = username;
}

std::string LoggedUser::getUserName()
{
	return this->m_username;
}
