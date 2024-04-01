#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include "WSAInitializer.h"
#include <Windows.h>
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"

#include <iostream>
#include <map>
#include <thread>
#include <exception>
#include <string>
#include <mutex>
#include <vector>
#include <ctime>



#define PORT 9999
#define BUFFER_SIZE 1024
#define LENGTH_DATA 4

class Communicator {
private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;

	/// <summary>
	/// bind to a socket and listen to new clients.
	/// </summary>
	void bindAndListen();

	/// <summary>
	/// handles to coneversation with the client accepted.
	/// </summary>
	/// <param name="client_sock:"> socket of the client.</param>
	void handleNewClient(SOCKET client_sock);

	std::mutex clientListMtx;//using with the clientsList.

	//temporary send data to clients and get data from clients.
	void sendData(SOCKET soc, std::vector<unsigned char> Msg);
	std::vector<unsigned char> getData(SOCKET soc);
	int getSizeOfData(std::vector<unsigned char> buffer);
public:
	Communicator();
	~Communicator();
	void startHandleRequests();
	


};
