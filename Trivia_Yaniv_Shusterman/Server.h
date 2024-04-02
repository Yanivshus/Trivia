#pragma once
#include <WinSock2.h>
#include <iostream>
#include <Windows.h>
#include <thread>
#include "Communicator.h"
#include "SqliteDataBase.h"
#include "RequestHandlerFactory.h"

class Server {
private:
	Communicator m_communicator;
	RequestHandlerFactory m_handlerFactory;
	IDatabase* m_database;
public:
	Server();
	void run();
};
