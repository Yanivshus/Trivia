#include "RequestHandlerFactory.h"


LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
	return new MenuRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}

void RequestHandlerFactory::setDB(IDatabase* m_database)
{
	m_loginManager.setDB(m_database);
}
