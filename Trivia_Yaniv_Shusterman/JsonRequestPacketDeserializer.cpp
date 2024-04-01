#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<unsigned char> buffer)
{
	//taking the buffer into a string.
	json jsData = json::from_bson(buffer);// parsing into json object
	LoginRequest req;
	req.password = jsData["password"]; //taking the password and username and email.
	req.username = jsData["username"];
	return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(std::vector<unsigned char> buffer)
{
	//taking the buffer into a string.
	json jsData = json::from_bson(buffer);// parsing into json object
	SignupRequest req;
	req.password = jsData["password"];//taking the password and username and email.
	req.username = jsData["username"];
	req.email = jsData["email"];
	return req;
}
