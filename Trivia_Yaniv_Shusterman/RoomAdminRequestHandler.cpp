#include "RoomAdminRequestHandler.h"


RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo info)
{
    return RequestResult();
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo info)
{
    return RequestResult();
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo info)
{
    return RequestResult();
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo info)
{
	if (info.id == CLOSE_ROOM_REQUEST ||
		info.id == START_GAME_REQUEST ||
		info.id == GET_ROOM_STATE_REQUEST) 
	{
		return true;
	}
	return false;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo info, SOCKET sock)
{
	try
	{
        //genarate a resault and handle it.
        if (info.id == CLOSE_ROOM_REQUEST) {
            return this->closeRoom(info);
        }
        if (info.id == START_GAME_REQUEST) {
            return this->startGame(info);
        }
        if (info.id == GET_ROOM_STATE_REQUEST) {
            return this->getRoomState(info);
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
