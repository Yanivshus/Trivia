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
	while (true)
	{
		try
		{
			std::vector<unsigned char> buffer = getData(client_sock);
			const int code = static_cast<int>(buffer[0]); // the code of the msg
			const int dataLength = getSizeOfData(std::vector<unsigned char>(buffer.begin() + 1, buffer.begin() + SIZE_OF_START));// get the length of the data by slicing the buffer.
			//std::cout << dataLength << std::endl;
			std::vector<unsigned char> data(buffer.begin() + SIZE_OF_START, buffer.begin() + dataLength + SIZE_OF_START);// get the data itself.
			std::cout << data.size() << std::endl;
			std::cout << data.data() << std::endl;

			//build the request.
			RequestInfo reqInfo;
			reqInfo.buffer = data;
			reqInfo.id = (Codes)code;
			reqInfo.time = std::time(0);

			//checking if the request is relevent to the current user stage.
			if (this->m_clients[client_sock]->isRequestRelevant(reqInfo))
			{
				RequestResult res = this->m_clients[client_sock]->handleRequest(reqInfo);//handling the request.
				sendData(client_sock, res.buffer);
			}
			else {
				throw std::exception("Request isn't relevent.");
			}

		}
		catch (std::exception& e) {
			//checking if the was relevent request error.
			if (e.what() == "Request isn't relevent.")
			{
				ErrorResponse err;
				err.massage = "error in your request.";
				std::vector<unsigned char> errorBuff = JsonResponsePacketSerializer::serializeResponse({ err });// if yes we will send to the client back an error massage.
				sendData(client_sock, errorBuff);
			}
			else // if not we will just print the error.
			{
				std::cout << e.what() << std::endl;
				break;//tempppppp
			}
		}
	}
}

void Communicator::sendData(SOCKET soc, std::vector<unsigned char> Msg)
{
	const char* data = reinterpret_cast<const char*>(Msg.data());

	if (send(soc, data, Msg.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}

std::vector<unsigned char> Communicator::getData(SOCKET soc)
{
	std::vector<unsigned char> buffer(BUFFER_SIZE);
	int res = recv(soc, reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE, 0);

	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(soc);
		throw std::exception(s.c_str());
	}
	else {
		buffer.resize(res);
	}
	if (buffer.size() == 0) {
		throw std::exception("User sent empty");
	}
	return buffer;
}

int Communicator::getSizeOfData(std::vector<unsigned char> buffer)
{
	int value = 0;

	// Combine each byte into the integer
	for (size_t i = 0; i < buffer.size(); ++i) {
		value |= static_cast<int>(buffer[i]) << (8 * (LENGTH_DATA - 1 - i)); // Combine the byte into the integer
	}

	return value;
}

