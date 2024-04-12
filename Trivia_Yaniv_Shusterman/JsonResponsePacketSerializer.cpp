#include "JsonResponsePacketSerializer.h"

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& err)
{
    json j = { { "message", err.massage} };
    Codes curr = ERROR_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse& err)
{
    json j = { { "status", err.status} };
    Codes curr = LOGIN_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignupResponse& err)
{
    json j = { { "status", err.status} };
    Codes curr = SIGNUP_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& err)
{
    json j = { {"status", err.status} };
    Codes curr = LOGIN_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& err)
{
    std::string rooms = "";
    //concatane to string the rooms names.
    for (auto& room : err.rooms)
    {
        rooms += room.name;
        rooms += "=";
        rooms += std::to_string(room.id);
        rooms += ", ";
    }

    if (rooms.size() >= 1) {
        rooms.pop_back(); // removing trailing ','.
        rooms.pop_back(); // removing trailing ' '.
    }

    json j = { {"status", err.status}, {"Rooms", rooms} };
    
    Codes curr = GET_ROOMS_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& err)
{   
    std::string players = "";
    //concatane to string the players usernames.
    for (auto& player : err.players)
    {
        players += player;
        players += ", ";
    }

    if (players.size() >= 1) {
        players.pop_back(); // removing trailing ','.
    }

    json j = { {"Players", players} };
    Codes curr = GET_PLAYERS_IN_ROOM_RESPONSE;
    return createPacket(curr, j);

}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& err)
{
    std::string usersScores = "";
    //concatane to string the username=score.
    for (auto& score : err.statistics)
    {
        usersScores += score;
        usersScores += ", ";
    }

    if (usersScores.size() >= 1) {
        usersScores.pop_back(); // removing trailing ','.
    }

    json j = { {"HighScores", usersScores} };
    Codes curr = GET_HIGH_SCORE_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& err)
{
    json j = { {"AvgAnswerTime", err.statistics[0]},
        {"NumCorrectAnswers", err.statistics[1]},
        {"NumTotalAnswers", err.statistics[2]},
        {"NumPlayerGames", err.statistics[3]}};

    Codes curr = GET_PERSONAL_STATS_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& err)
{
    json j = { {"status", err.status} };
    Codes curr = JOIN_ROOM_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& err)
{
    json j = { {"roomId", err.id} };
    Codes curr = CREATE_ROOM_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& err)
{
    json j = { {"status", err.status} };
    Codes curr = CLOSE_ROOM_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& err)
{
    json j = { {"status", err.status} };
    Codes curr = START_GAME_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& err)
{
    std::string players = "";

    //create list of players.
    for (auto& player : err.players)
    {
        players += player;
        players += ", ";
    }

    //poop the trailing ", ".
    if (players.size() > 0) {
        players.pop_back();
      players.pop_back();
    }
    //create json data.
    json j{ {"status", err.status}, {"hasGameBegun", err.hasGameBegun}, {"players", players}, {"questionCount", err.questionCount}, {"answerTimeOut", err.answerTimeOut} };
    Codes curr = GET_ROOM_STATE_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& err)
{
    json j = { {"status", err.status} };
    Codes curr = LEAVE_ROOM_RESPONSE;
    return createPacket(curr, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::createPacket(const int code, json data)
{
    std::vector<unsigned char> bytesVec;
    bytesVec.push_back(static_cast<unsigned char>(code));//pushing the code of the response first / size 1 byte.

    //turning the json to char vector
    std::vector<unsigned char> dataVector = json::to_bson(data);

    std::vector<unsigned char> bytesOfLenData = createDataLengthAsBytes(dataVector.size());// take the length of data as bytes.
    std::copy(bytesOfLenData.begin(), bytesOfLenData.end(), std::back_inserter(bytesVec));// copy the length to the vector.

    std::copy(dataVector.begin(), dataVector.end(), std::back_inserter(bytesVec));//copy the data to the vector.

    return bytesVec;
}

std::vector<unsigned char> JsonResponsePacketSerializer::createDataLengthAsBytes(const int num)
{
    std::vector<unsigned char> bytes(CODE_SIZE);

    // Extract each byte from the integer
    for (int i = 0; i < CODE_SIZE; i++) {
        bytes[CODE_SIZE - 1 - i] = static_cast<unsigned char>((num >> (i * 8)) & 0xFF); // Extract each byte
    }
    // Return the vector of bytes
    return bytes;
}
