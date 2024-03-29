#include "Server.h"

void Server::run()
{
	std::cout << "Server is up and running!!!" << std::endl;
	std::thread connectorThread(&Communicator::startHandleRequests, &this->m_communicator);
	connectorThread.detach();

	//main loop when admin enters exit the server will close.
	std::string exit = "";
	while (exit != "EXIT") 
	{
		std::cin >> exit;
	}
	std::cout << "Server closed!" << std::endl;
}
