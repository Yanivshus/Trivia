#pragma once
#include "StructHelper.h"
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

class JsonRequestPacketDeserializer {
public:
	static LoginRequest deserializeLoginRequest(std::vector<unsigned char> buffer);
	static SignupRequest deserializeSignupRequest(std::vector<unsigned char> buffer);
};