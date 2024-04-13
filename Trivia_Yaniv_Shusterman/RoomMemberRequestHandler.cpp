#include "RoomMemberRequestHandler.h"

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo info)
{
    //create a leave room packet and send to the current user.
    LeaveRoomResponse res = { LEAVE_ROOM_RESPONSE };

    //remove user from the room
    this->m_handlerFactory.getRoomManager().getRoom(this->m_room.getRoomData().id).deleteUser(this->m_user);

    return { JsonResponsePacketSerializer::serializeResponse(res), (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomMemberRequestHandler::getPlayersInRoom(RequestInfo info)
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

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo info)
{
    GetRoomStateResponse res;
    // get users
    std::vector<LoggedUser> users = this->m_handlerFactory.getRoomManager().getRoom(this->m_room.getRoomData().id).getAllUsers();

    //creating vector of strings.
    std::vector<std::string> usersStr;
    for (auto& user : users)
    {
        usersStr.push_back(user.getUserName());
    }

    //create packet.
    res.players = usersStr;
    res.answerTimeOut = this->m_room.getRoomData().timePerQuestion;
    res.hasGameBegun = this->m_room.getRoomData().isActive;
    res.questionCount = this->m_room.getRoomData().numOfQuestionsInGame;
    res.status = GET_ROOM_STATE_RESPONSE;

    return { JsonResponsePacketSerializer::serializeResponse(res), nullptr };
}

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& factory, Room room, const LoggedUser& user) : m_handlerFactory(factory), m_room(room), m_user(user)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo info)
{
	if (info.id == GET_PLAYERS_IN_ROOM_REQUEST ||
		info.id == LEAVE_ROOM_REQUEST ||
		info.id == GET_ROOM_STATE_REQUEST) 
	{
		return true;
	}
	return false;
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo info, SOCKET sock)
{
    try
    {
        //genarate a resault and handle it.
        if (info.id == LEAVE_ROOM_REQUEST) {
            return this->leaveRoom(info);
        }
        if (info.id == GET_ROOM_STATE_REQUEST) {
           return this->getRoomState(info);
        }
        if (info.id == GET_PLAYERS_IN_ROOM_REQUEST) {
           return this->getPlayersInRoom(info);
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
