#include "Server.h"

int main() {
	WSAInitializer wsaInit;
	Server* server = Server::getInstance();
	server->run();
	delete server;
}