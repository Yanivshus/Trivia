#include "Helper.h"

void Helper::sendData(SOCKET soc, std::vector<unsigned char> Msg)
{
	Msg.resize(1024, 0);
	const char* data = reinterpret_cast<const char*>(Msg.data());

	if (send(soc, data, Msg.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}


std::vector<unsigned char> Helper::getData(SOCKET soc)
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
		throw std::exception("User disconnected\n");
	}
	return buffer;
}
