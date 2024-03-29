#pragma once
#include <WinSock2.h>
#include <iostream>
#include <Windows.h>
#include <thread>
#include "Communicator.h"

class Server {
private:
	Communicator m_communicator;
public:
	void run();
};
