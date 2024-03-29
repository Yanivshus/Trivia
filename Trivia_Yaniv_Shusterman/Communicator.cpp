#include "Communicator.h"


Communicator::Communicator()
{
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(this->m_serverSocket);
	}
	catch (...) {}
}

void Communicator::startHandleRequests()
{
	
	try 
	{
		bindAndListen();
	}
	catch (...) {
		std::cout << "there is a error in the bind or listen." << std::endl;
	}

	

}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"
	// Connects between the socket and the configuration (port and etc..)
	if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind.");

	// Start listening for incoming requests of clients
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen.");

	while (true)
	{

		// this accepts the client and create a specific socket from server to this client
		// the process will not continue until a client connects to the server
		SOCKET client_socket = accept(this->m_serverSocket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
			throw std::exception("accept problem.");

		std::cout << "Client accepted. Server and client can speak." << std::endl;
		this->clientListMtx.lock();
		this->m_clients[client_socket] = new LoginRequestHandler;
		this->clientListMtx.unlock();

		// the function that handle the conversation with the client
		std::thread clientThread(&Communicator::handleNewClient, this, client_socket);
		clientThread.detach();
	}
}

void Communicator::handleNewClient(SOCKET client_sock)
{
	try {
		//sending and receiving hello.
		sendData(client_sock, "Hello");
		std::cout << getData(client_sock, 5) << std::endl;
	}
	catch (...) {
		std::cout << "error handeling client" << std::endl;
	}
}

void Communicator::sendData(SOCKET soc, const std::string& Msg)
{
	const char* data = Msg.c_str();

	if (send(soc, data, Msg.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}

std::string Communicator::getData(SOCKET soc, const int byteNum)
{
	if (byteNum == 0)
	{
		return (char*)"";
	}

	char* data = new char[byteNum + 1];
	int res = recv(soc, data, byteNum, 0);

	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(soc);
		throw std::exception(s.c_str());
	}

	data[byteNum] = 0;
	std::string stringRes(data);
	return stringRes;
}

