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

	/// <summary>
	/// send data bu the socket.
	/// </summary>
	/// <param name="soc:">socket of client.</param>
	/// <param name="Msg:">msg to send.</param>
	void sendData(SOCKET soc, std::vector<unsigned char> Msg);

	/// <summary>
	/// get data from server.
	/// </summary>
	/// <param name="soc:">socket of client.</param>
	/// <returns>vector of bytes which is the data.</returns>
	std::vector<unsigned char> getData(SOCKET soc);

	/// <summary>
	/// turn the len of data back to int from vector of 4 bytes.
	/// </summary>
	/// <param name="buffer:">vector of bytes.</param>
	/// <returns>length of data.</returns>
	int getSizeOfData(std::vector<unsigned char> buffer);
public:
	Communicator();
	~Communicator();
	void startHandleRequests();
	


};
