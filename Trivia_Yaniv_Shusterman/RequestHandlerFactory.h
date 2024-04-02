#pragma once
#include "IDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include <iostream>

class LoginRequestHandler;

class RequestHandlerFactory {
private:
	IDatabase* m_database;
	LoginManager m_loginManager;
public:
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();
	void setDB(IDatabase* m_database);
};
