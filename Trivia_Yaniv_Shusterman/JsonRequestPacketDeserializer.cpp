#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<unsigned char> buffer)
{
	//taking the buffer into a string.
	std::string buff(buffer.begin(), buffer.end());

	LoginRequest req;

	//extracting only the json part.
	auto found = buff.find("{");
	if (std::string::npos != found) 
	{
		std::string data = buff.substr(found);
		json jsData = json::parse(data);// parsing into json object
		req.password = jsData["password"];//taking the password and username.
		req.username = jsData["username"];
		return req;
	}
	return req;//empty if wasnt found.
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(std::vector<unsigned char> buffer)
{
	//taking the buffer into a string.
	std::string buff(buffer.begin(), buffer.end());

	SignupRequest req;

	//extracting only the json part.
	auto found = buff.find("{");
	if (std::string::npos != found)
	{
		std::string data = buff.substr(found);
		json jsData = json::parse(data);// parsing into json object
		req.password = jsData["password"];//taking the password and username and email.
		req.username = jsData["username"];
		req.email = jsData["email"];
		return req;
	}
	return req;//empty if wasnt found.
}
