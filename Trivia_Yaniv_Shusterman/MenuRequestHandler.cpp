#include "MenuRequestHandler.h"


MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& m_handlerFactory, const LoggedUser& m_user) : m_handlerFactory(m_handlerFactory), m_user(m_user)
{
    
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo info)
{
    if (info.id == CREATE_ROOM_REQUEST ||
        info.id == GET_ROOMS_REQUEST ||
        info.id == GET_PLAYERS_IN_ROOM_REQUEST ||
        info.id == JOIN_ROOM_REQUEST ||
        info.id == HIGH_SCORE_REQUEST ||
        info.id == LOGOUT_REQUSET ||
        info.id == GET_PERSONAL_STATS_REQUEST ||
        info.id == DELETE_GAME_REQUEST) 
    {
        return true;
    }
    return false;

}

RequestResult MenuRequestHandler::handleRequest(RequestInfo info, SOCKET sock, std::map<SOCKET, IRequestHandler*>& m_clients)
{
    try
    {
        //genarate a resault and handle it.
        if (info.id == CREATE_ROOM_REQUEST) {
            return this->createRoom(info, m_clients);
        }
        if (info.id == GET_ROOMS_REQUEST) {
            return this->getRooms(info);
        }
        if (info.id == GET_PLAYERS_IN_ROOM_REQUEST) {
            return this->getPlayersInRoom(info);
        }
        if(info.id == JOIN_ROOM_REQUEST) {
            return this->joinRoom(info);
        }
        if (info.id == HIGH_SCORE_REQUEST) {
            return this->getHighScore(info);
        }
        if (info.id == LOGOUT_REQUSET) {
            return this->signout(info);
        }
        if (info.id == GET_PERSONAL_STATS_REQUEST) {
            return this->getPersonalStats(info);
        }
        if (info.id == DELETE_GAME_REQUEST) {
            return this->deleteGame(info);
        }
    }
    catch (const std::exception& e) // if eny thrown exceptions caught , return a error response.
    {
        std::cout << e.what() << std::endl;
        ErrorResponse res = { "FAILED" };
        return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
    }
    catch (const nlohmann::json::exception& e)
    {
        ErrorResponse res = { "ERROR : INVALID JSON " };
        return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
    }
}

RequestResult MenuRequestHandler::signout(RequestInfo info)
{
    try
    {
        this->m_handlerFactory.getLoginManager().logout(this->m_user.getUserName());// signing out.
        std::cout << "Successfull logout!" << std::endl;

        LogoutResponse outRes;
        outRes.status = LOGOUT_RESPONSE;
        return { JsonResponsePacketSerializer::serializeResponse(outRes), this->m_handlerFactory.createLoginRequestHandler() };
    }
    catch (const std::exception& e) {
        throw e;
    }

}

RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
    try
    {
        std::vector<RoomData> rooms = this->m_handlerFactory.getRoomManager().getRooms();
        //constract room response.
        GetRoomsResponse roomsRes;
        roomsRes.rooms = rooms;
        roomsRes.status = GET_ROOMS_RESPONSE;
        //return resault.
        return { JsonResponsePacketSerializer::serializeResponse(roomsRes), nullptr };
    }
    catch (const std::exception& e)
    {
        throw e;
    }
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
    try
    {
        GetPlayersInRoomRequest playersReq = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(info.buffer);// get the room id from buffer.
        std::vector<LoggedUser> players = this->m_handlerFactory.getRoomManager().getRoom(playersReq.roomId).getAllUsers(); // get all the user active in the room.

        //creating vector of strings.
        std::vector<std::string> playersStr;
        for (auto& player : players)
        {
            playersStr.push_back(player.getUserName());
        }

        GetPlayersInRoomResponse playersRes;
        playersRes.players = playersStr;
        // create the resault packet.
        return { JsonResponsePacketSerializer::serializeResponse(playersRes), nullptr };
    }
    catch (const std::exception& e)
    {
        throw e;
    }
    catch (const json::exception& e) {
        throw std::exception("json exception");
    }

}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
    try
    {
        // get from statistics manager the user stats.
        std::vector<std::string> stats = this->m_handlerFactory.getStatisticsManager().getUserStatistics(m_user.getUserName());

        GetPersonalStatsResponse statsRes;
        statsRes.statistics = stats;
        statsRes.status = GET_PERSONAL_STATS_REQUEST;

        return { JsonResponsePacketSerializer::serializeResponse(statsRes) , nullptr };
    }
    catch (const std::exception& e)
    {
        throw e;
    }
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
    try
    {
        // get top five best performers.
        std::vector<std::string> top = this->m_handlerFactory.getStatisticsManager().getHighScore();

        GetHighScoreResponse topRes;
        topRes.statistics = top;
        topRes.status = HIGH_SCORE_REQUEST;

        return { JsonResponsePacketSerializer::serializeResponse(topRes) , nullptr };
    }
    catch (const std::exception& e)
    {
        throw e;
    }
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
    try
    {
        JoinRoomRequest joinReq = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer); // desrilize to get room if.
        this->m_handlerFactory.getRoomManager().getRoom(joinReq.roomId).addUser(LoggedUser(m_user.getUserName(), m_user.getSock()));// add user to the room.

        JoinRoomResponse joinRes;
        joinRes.status = JOIN_ROOM_RESPONSE;
        //returns appropriate room member request handler.
        return { JsonResponsePacketSerializer::serializeResponse(joinRes) , (IRequestHandler*)this->m_handlerFactory.CreateRoomMemberRequestHandler(this->m_user, this->m_handlerFactory.getRoomManager().getRoom(joinReq.roomId)) };

    }
    catch (const std::exception& e)
    {
        throw e;
    }
}

RequestResult MenuRequestHandler::createRoom(RequestInfo info, std::map<SOCKET, IRequestHandler*>& m_clients)
{
    try
    {
        CreateRoomRequest createReq = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
        RoomData data;
        //set room data parameters.
        data.id = CreateRoomId(); // create random id for the room.
        data.isActive = 1;
        data.maxPlayers = createReq.maxUsers;
        data.name = createReq.roomName;
        data.numOfQuestionsInGame = createReq.questionCount;
        data.timePerQuestion = createReq.answerTimeout;

        this->m_handlerFactory.getRoomManager().createRoom(this->m_user, data);// create the room in the room manager.
        CreateRoomResponse createRes;
        createRes.id = data.id;
        return { JsonResponsePacketSerializer::serializeResponse(createRes), (IRequestHandler*)this->m_handlerFactory.CreateRoomAdminRequestHandler(this->m_user, this->m_handlerFactory.getRoomManager().getRoom(data.id), m_clients)};
    }
    catch (const std::exception& e)
    {
        throw e;
    }

}

RequestResult MenuRequestHandler::deleteGame(RequestInfo info)
{
    try {
        DeleteGameRequest deleteReq = JsonRequestPacketDeserializer::deserializeDeleteGameRequest(info.buffer);

        // delete both the room and the game.
        this->m_handlerFactory.getGameManager().deleteGame(deleteReq.roomId);
        this->m_handlerFactory.getRoomManager().deleteRoom(deleteReq.roomId);

        DeleteGameResponse res = { DELETE_GAME_RESPONSE };
        return { JsonResponsePacketSerializer::serializeResponse(res), nullptr };
    }
    catch (const std::exception& e)
    {
        throw e;
    }
}

int MenuRequestHandler::CreateRoomId()
{
    time_t time = std::time(0);
    // Create a random number generator engine
    std::random_device rd;
    std::mt19937 generator(rd()); // Use Mersenne Twister engine

    // Define a uniform distribution
    std::uniform_int_distribution<int> distribution1(100, 1000);
    std::uniform_int_distribution<int> distribution2(1, 10);

    int rand1 = distribution1(generator);
    int rand2 = distribution2(generator);
    
    // Use long long for the intermediate result to avoid overflow
    long long result = static_cast<long long>(time) * rand1 / rand2;

    // Ensure the result fits within the range of an int
    return static_cast<int>(result % INT_MAX);
}
