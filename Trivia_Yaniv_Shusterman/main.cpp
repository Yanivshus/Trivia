#include "Server.h"
#include "JsonResponsePacketSerializer.h"
int main() {
	
	WSAInitializer wsaInit;
	Server server;
	server.run();
}