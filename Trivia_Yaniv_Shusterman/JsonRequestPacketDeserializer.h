#pragma once
#include "StructHelper.h"
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

class JsonRequestPacketDeserializer {
public:
	/// <summary>
	/// deserializing a buffer of byte into an approptiate login request struct.
	/// </summary>
	static LoginRequest deserializeLoginRequest(std::vector<unsigned char> buffer);

	/// <summary>
	/// deserializing a buffer of byte into an approptiate Signup Request struct.
	/// </summary>
	static SignupRequest deserializeSignupRequest(std::vector<unsigned char> buffer);

	/// <summary>
	/// deserializing a buffer of byte into an approptiate GetPlayersInRoom Request struct.
	/// </summary>
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(std::vector<unsigned char> buffer);
	/// <summary>
	/// deserializing a buffer of byte into an approptiate JoinRoom Request struct.
	/// </summary>
	static JoinRoomRequest deserializeJoinRoomRequest(std::vector<unsigned char> buffer);

	/// <summary>
	/// deserializing a buffer of byte into an approptiate CreateRoom Request struct.
	/// </summary>
	static CreateRoomRequest deserializeCreateRoomRequest(std::vector<unsigned char> buffer);


	/// <summary>
	/// deserialize the buffer to submit room request.
	/// </summary>
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(std::vector<unsigned char> buffer);


	/// <summary>
	/// deserilzes delete game request to struct.
	/// </summary>

	static DeleteGameRequest deserializeDeleteGameRequest(std::vector<unsigned char> buffer);

	/// <summary>
	/// deserialize add question to questions.
	/// </summary>
	static QuestionAddition deserializeAddQuestion(std::vector<unsigned char> buffer);

	
};