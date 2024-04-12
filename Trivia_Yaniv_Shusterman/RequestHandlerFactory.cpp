#include "RequestHandlerFactory.h"


LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& adminUser)
{
	return new MenuRequestHandler(*this, adminUser);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}

RoomManger& RequestHandlerFactory::getRoomManager()
{
	return this->m_roomManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return this->m_StatisticsManager;
}

RoomAdminRequestHandler* RequestHandlerFactory::CreateRoomAdminRequestHandler(LoggedUser user, Room room)
{
	return new RoomAdminRequestHandler(*this, room, user);
}

void RequestHandlerFactory::setDB(IDatabase* m_database)
{
	m_loginManager.setDB(m_database);
	m_StatisticsManager.setDB(m_database);
}
