#pragma once
#include "IDatabase.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "LoginRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"
#include "GameManager.h"
#include <iostream>

// for circular include.
class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class GameRequestHandler;

class RequestHandlerFactory {
private:
	IDatabase* m_database;
	LoginManager& m_loginManager;
	RoomManger& m_roomManager;
	StatisticsManager& m_StatisticsManager;
	GameManager& m_gameManager;
public:
	RequestHandlerFactory();

	// returns login request handler.
	LoginRequestHandler* createLoginRequestHandler();

	// returns a menu request handler and sets the admin user to the current user.
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& adminUser);//need to fix this after finished the menuRequestHandler.

	// gets the login manager referense.
	LoginManager& getLoginManager();

	// gets the room manager referense.
	RoomManger& getRoomManager();

	// gets the game manager referense;
	GameManager& getGameManager();

	// gets the stats manager referense.
	StatisticsManager& getStatisticsManager();

	// create a admin request handler.
	RoomAdminRequestHandler* CreateRoomAdminRequestHandler(LoggedUser user, Room room, std::map<SOCKET, IRequestHandler*>& m_clients);

	// create room member handler.
	RoomMemberRequestHandler* CreateRoomMemberRequestHandler(LoggedUser user, Room room);

	// create a game request handler.
	GameRequestHandler* CreateGameRequestHandler(LoggedUser user, Game& game);

	IDatabase* getDB() const;

	static RequestHandlerFactory& getInstance() {
		static RequestHandlerFactory theFactory;
		return theFactory;
	}

	// sets the db to the factory.
	void setDB(IDatabase* m_database);
};
