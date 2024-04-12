#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <string>

#define BUFFER_SIZE 1024

class Helper {
public:

	/// <summary>
	/// send data bu the socket.
	/// </summary>
	/// <param name="soc:">socket of client.</param>
	/// <param name="Msg:">msg to send.</param>
	static void sendData(SOCKET soc, std::vector<unsigned char> Msg);

	/// <summary>
	/// get data from server.
	/// </summary>
	/// <param name="soc:">socket of client.</param>
	/// <returns>vector of bytes which is the data.</returns>
	static std::vector<unsigned char> getData(SOCKET soc);
};