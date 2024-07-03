#pragma once
#include <WinSock2.h>
#include <iostream>
#include <Windows.h>
#include <thread>
#include "Communicator.h"
#include "SqliteDataBase.h"
#include "RequestHandlerFactory.h"

class Server 
{
private:
	Communicator* m_communicator;
	RequestHandlerFactory& m_handlerFactory;
	IDatabase* m_database;
	static Server* _server;
public:
	Server();
	~Server();
	//main server entry point
	void run();

	static Server* getInstance() {
		if (_server == nullptr) {
			_server = new Server();
		}
		return _server;
	}
};
