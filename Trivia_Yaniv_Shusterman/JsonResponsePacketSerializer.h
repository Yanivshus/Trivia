#pragma once
#include "json.hpp"
#include "StructHelper.h"
#include <vector>
#include "Constants.h"


using json = nlohmann::json;

#define CODE_SIZE 4

class JsonResponsePacketSerializer {
public:
	static std::vector<unsigned char> serializeResponse(const ErrorResponse& err);
	static std::vector<unsigned char> serializeResponse(const LoginResponse& err);
	static std::vector<unsigned char> serializeResponse(const SignupResponse& err);
private:
	/// <summary>
	/// creates a packet in this protocol:
	/// 1 byte for response code
	/// 4 bytes for data length
	/// rest for data.
	/// </summary>
	/// <param name="code:">reasponse code.</param>
	/// <param name="data:">data to pack.</param>
	/// <returns>return vector of unsigned char (bytes)</returns>
	static std::vector<unsigned char> createPacket(const int code, const std::string& data);

	/// <summary>
	/// turnes length of data to a unsigned char (byte)
	/// </summary>
	/// <param name="num:">length of data</param>
	/// <returns>vector of unsigned chars</returns>
	static std::vector<unsigned char> createDataLengthAsBytes(const int num);
	
};