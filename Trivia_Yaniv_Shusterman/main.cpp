#include "Server.h"

int main() {
	WSAInitializer wsaInit;
	Server server;
	server.run();
}