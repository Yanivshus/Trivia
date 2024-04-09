#include "Communicator.h"



Communicator::Communicator(RequestHandlerFactory& r) : m_handlerFactory(r)
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

		// Iterate through the map
		for (auto& pair : m_clients) {
			// Check if the map entry has at least two elements
			if (pair.second != nullptr) {
				// Delete the second element
				delete pair.second;
				pair.second = nullptr;  // Optional: Set to nullptr to avoid dangling pointer
			}
		}
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
		this->m_clients[client_socket] = this->m_handlerFactory.createLoginRequestHandler();
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
			std::vector<unsigned char> data(buffer.begin() + SIZE_OF_START, buffer.begin() + dataLength + SIZE_OF_START);// get the data itself.
			
			std::cout << data.data() << std::endl;

			//build the request.
			RequestInfo reqInfo;
			reqInfo.buffer = data;
			reqInfo.id = (Codes)code;
			std::cout << reqInfo.id << std::endl;
			reqInfo.time = std::time(0);

			if (this->m_clients.find(client_sock)->second == nullptr)
			{
				std::cout << "[ERROR] Couldn't find socket" << std::endl;
			}


			//checking if the request is relevent to the current user stage.
			if (this->m_clients.find(client_sock)->second != nullptr && this->m_clients[client_sock]->isRequestRelevant(reqInfo))
			{
				RequestResult res = this->m_clients[client_sock]->handleRequest(reqInfo);//handling the request.
				//if the returned type of the handler is nullptr the user status will not change.
				this->clientListMtx.lock();
				if (res.newHandler != nullptr) {
					this->m_clients[client_sock] = res.newHandler;// if its not nullptr it will change to the new handler.
				}
				this->clientListMtx.unlock();

				sendData(client_sock, res.buffer);
			}
			else {
				//if the request isnt relevnt i will update the client about it.
				ErrorResponse err;
				err.massage = "Request isn't relevent.";
				std::vector<unsigned char> errorBuff = JsonResponsePacketSerializer::serializeResponse(err);// if yes we will send to the client back an error massage.
				sendData(client_sock, errorBuff);
			}

		}
		catch (std::exception& e) 
		{
			std::cout << e.what() << std::endl;
			closesocket(client_sock);
			break;
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
		throw std::exception("User disconnected");
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

