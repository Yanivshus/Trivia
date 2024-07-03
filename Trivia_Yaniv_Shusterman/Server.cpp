#include "Server.h"

Server* Server::_server = nullptr;
Server::Server() : m_handlerFactory(RequestHandlerFactory::getInstance())
{
	m_communicator = Communicator::getInstance(this->m_handlerFactory);
	this->m_database = SqliteDataBase::getInstance();
	if (!this->m_database->open()) {
		std::cout << "DATABASE PROBLEM" << std::endl;
	}
	m_handlerFactory.setDB(this->m_database);
}
Server::~Server()
{
	delete this->m_communicator;
	delete this->m_database;
}
void Server::run()
{
	std::cout << "Server is up and running!!!" << std::endl;
	std::thread connectorThread(&Communicator::startHandleRequests, this->m_communicator);
	connectorThread.detach();

	//main loop when admin enters exit the server will close.
	std::string exit = "";
	while (exit != "EXIT")
	{
		std::cin >> exit;
	}
	std::cout << "Server closed!" << std::endl;
	this->m_database->close();
}
