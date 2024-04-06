#pragma once
#include "IDatabase.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "LoginRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include <iostream>

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory {
private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManger m_roomManager;
	StatisticsManager m_StatisticsManager;
public:
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();//need to fix this after finished the menuRequestHandler.
	LoginManager& getLoginManager();
	RoomManger& getRoomManager();
	StatisticsManager& getStatisticsManager();
	void setDB(IDatabase* m_database);
};
