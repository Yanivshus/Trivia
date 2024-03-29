#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include "WSAInitializer.h"
#include <Windows.h>
#include "IRequestHandler.h"

#include <iostream>
#include <map>
#include <thread>
#include <exception>
#include <string>



#define PORT 9999

class Communicator {
private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler> m_clients;
	void bindAndListen();
	void handleNewClient(SOCKET client_sock);
	void sendData(SOCKET soc, const std::string& Msg);
	std::string getData(SOCKET soc, const int byteNum);
public:
	Communicator();
	~Communicator();
	void startHandleRequests();
	


};
