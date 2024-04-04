#pragma once
#include "IDatabase.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "LoginRequestHandler.h"
#include <iostream>

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory {
private:
	IDatabase* m_database;
	LoginManager m_loginManager;
public:
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();
	LoginManager& getLoginManager();
	void setDB(IDatabase* m_database);
};
