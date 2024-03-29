#include "Server.h"

void Server::run()
{
	std::thread connectorThread(&Communicator::startHandleRequests, &this->m_communicator);
	connectorThread.detach();


	std::string exit = "";
	while (exit != "EXIT") 
	{
		std::cin >> exit;
	}
	std::cout << "Server closed!" << std::endl;
}
