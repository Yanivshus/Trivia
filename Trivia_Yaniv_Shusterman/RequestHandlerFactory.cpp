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

GameManager& RequestHandlerFactory::getGameManager()
{
	return this->m_gameManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return this->m_StatisticsManager;
}

RoomAdminRequestHandler* RequestHandlerFactory::CreateRoomAdminRequestHandler(LoggedUser user, Room room, std::map<SOCKET, IRequestHandler*>& m_clients)
{
	return new RoomAdminRequestHandler(*this, room, user, m_clients);
}

RoomMemberRequestHandler* RequestHandlerFactory::CreateRoomMemberRequestHandler(LoggedUser user, Room room)
{
	return new RoomMemberRequestHandler(*this, room, user);
}

GameRequestHandler* RequestHandlerFactory::CreateGameRequestHandler(LoggedUser user, Game& game)
{
	return new GameRequestHandler(user, *this, game);
}

IDatabase* RequestHandlerFactory::getDB() const
{
	return this->m_database;
}

void RequestHandlerFactory::setDB(IDatabase* m_database)
{
	this->m_database = m_database;
	m_loginManager.setDB(m_database);
	m_StatisticsManager.setDB(m_database);
	m_gameManager.setDB(m_database);
}
