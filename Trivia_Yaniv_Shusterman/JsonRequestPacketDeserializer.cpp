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
	json jsData = json::from_bson(buffer);
	// parsing into json object
	SignupRequest req;
	req.password = jsData["password"];//taking the password and username and email.
	req.username = jsData["username"];
	req.email = jsData["email"];
	req.phone = jsData["phone"];
	req.date = jsData["date"];
	req.adrres = jsData["address"];
	return req;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(std::vector<unsigned char> buffer)
{
	json jsData = json::from_bson(buffer);// parsing into json object
	GetPlayersInRoomRequest req;
	req.roomId = jsData["roomId"];
	return req;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(std::vector<unsigned char> buffer)
{
	json jsData = json::from_bson(buffer);// parsing into json object
	JoinRoomRequest req;
	req.roomId = jsData["roomId"];
	return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::vector<unsigned char> buffer)
{
	json jsData = json::from_bson(buffer);// parsing into json object
	CreateRoomRequest req;
	req.answerTimeout = jsData["answerTime"];
	req.maxUsers = jsData["maxUsers"];
	req.questionCount = jsData["questionCount"];
	req.roomName = jsData["roomName"];
	return req;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(std::vector<unsigned char> buffer)
{
	json jsData = json::from_bson(buffer);// parsing into json object
	SubmitAnswerRequest req;
	req.answerId = jsData["correctAnsId"];
	return req;
}

DeleteGameRequest JsonRequestPacketDeserializer::deserializeDeleteGameRequest(std::vector<unsigned char> buffer)
{
	json jsData = json::from_bson(buffer);// parsing into json object
	DeleteGameRequest req;
	req.roomId = jsData["roomId"];
	return req;
}

QuestionAddition JsonRequestPacketDeserializer::deserializeAddQuestion(std::vector<unsigned char> buffer)
{
	json jsData = json::from_bson(buffer);
	QuestionAddition req;
	req.question = jsData["question"];
	req.w_answer1 = jsData["w_answer1"];
	req.w_answer2 = jsData["w_answer2"];
	req.w_answer3 = jsData["w_answer3"];
	req.c_answer4 = jsData["c_answer4"];
	return req;
}
