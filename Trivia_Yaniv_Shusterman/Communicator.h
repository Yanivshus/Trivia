#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include "WSAInitializer.h"
#include <Windows.h>
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


#include <iostream>
#include <map>
#include <thread>
#include <exception>
#include <string>
#include <mutex>
#include <vector>
#include <ctime>

#include "Helper.h"



#define PORT 9999

#define LENGTH_DATA 4

class Communicator {
private:
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
	static Communicator* _communicator;

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
	/// turn the len of data back to int from vector of 4 bytes.
	/// </summary>
	/// <param name="buffer:">vector of bytes.</param>
	/// <returns>length of data.</returns>
	int getSizeOfData(std::vector<unsigned char> buffer);
public:
	static Communicator* getInstance(RequestHandlerFactory& r) {
		if (_communicator == nullptr) {
			_communicator = new Communicator(r);
		}
		return _communicator;
	}

	Communicator(RequestHandlerFactory& r);
	~Communicator();
	void startHandleRequests();

};
