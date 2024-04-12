#pragma once
#include "IDatabase.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "LoginRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RoomAdminRequestHandler.h"
#include <iostream>

class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;

class RequestHandlerFactory {
private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManger m_roomManager;
	StatisticsManager m_StatisticsManager;
public:
	// returns login request handler.
	LoginRequestHandler* createLoginRequestHandler();

	// returns a menu request handler and sets the admin user to the current user.
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& adminUser);//need to fix this after finished the menuRequestHandler.

	// gets the login manager referense.
	LoginManager& getLoginManager();

	// gets the room manager referense.
	RoomManger& getRoomManager();

	// gets the stats manager referense.
	StatisticsManager& getStatisticsManager();

	// sets the db to the factory.
	void setDB(IDatabase* m_database);
};
