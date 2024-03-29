#pragma once
#include "json.hpp"
#include "StructHelper.h"
#include <vector>
#include "Constants.h"

using json = nlohmann::json;

class JsonResponsePacketSerializer {
	static std::vector<unsigned char> serializeResponse(const ErrorResponse& err);
	static std::vector<unsigned char> serializeResponse(const LoginResponse& err);
	static std::vector<unsigned char> serializeResponse(const SignupResponse& err);
};