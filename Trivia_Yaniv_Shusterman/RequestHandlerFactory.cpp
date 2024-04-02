#include "RequestHandlerFactory.h"


LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler;
}

LoginManager RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}

void RequestHandlerFactory::setDB(IDatabase* m_database)
{
	this->m_database = m_database;
	m_loginManager.setDB(m_database);
}
