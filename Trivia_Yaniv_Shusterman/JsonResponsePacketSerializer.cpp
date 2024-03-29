#include "JsonResponsePacketSerializer.h"

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& err)
{
    json j = { { "message", err.massage} };
    Codes curr = ERROR_RESPONSE;
    return createPacket(curr, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse& err)
{
    json j = { { "message", err.status} };
    Codes curr = LOGIN_RESPONSE;
    return createPacket(curr, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignupResponse& err)
{
    json j = { { "message", err.status} };
    Codes curr = SIGNUP_RESPONSE;
    return createPacket(curr, j.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::createPacket(const int code, const std::string& data)
{
	std::vector<unsigned char> bytesVec;
	bytesVec.push_back(static_cast<unsigned char>(code));//PUSHING THE CODE OF THE RESPONSE FIRST / SIZE 1 BYTE.

    std::vector<unsigned char> bytesOfLenData = createDataLengthAsBytes(data.size());// take the length of data as bytes.
    std::copy(bytesOfLenData.begin(), bytesOfLenData.end(), std::back_inserter(bytesVec));// copy the length to the vector.
    std::copy(data.begin(), data.end(), std::back_inserter(bytesVec));//copy the data to the vector.

    // for debugging
   /* for (int i = 0; i < bytesVec.size(); i++)
    {
        std::cout << bytesVec[i] << std::endl;
    }*/

    return bytesVec;
}

std::vector<unsigned char> JsonResponsePacketSerializer::createDataLengthAsBytes(const int num)
{
    std::vector<unsigned char> bytes(CODE_SIZE);

    // Extract each byte from the integer
    for (int i = 0; i < CODE_SIZE; i++) {
        bytes[CODE_SIZE - 1 - i] = (num >> (i * 8)); // Extract each byte
    }
    // Return the vector of bytes
    return bytes;
}
