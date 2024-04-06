#pragma once
#include "StructHelper.h"
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

class JsonRequestPacketDeserializer {
public:
	static LoginRequest deserializeLoginRequest(std::vector<unsigned char> buffer);
	static SignupRequest deserializeSignupRequest(std::vector<unsigned char> buffer);

	static GetPlayersInRoomRequest deserializeGetPlayersRequest(std::vector<unsigned char> buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(std::vector<unsigned char> buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(std::vector<unsigned char> buffer);
};