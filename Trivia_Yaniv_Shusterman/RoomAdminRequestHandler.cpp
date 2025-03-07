#include "RoomAdminRequestHandler.h"


RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo info)
{
    // get all current active users.   
    std::vector<LoggedUser> users = this->m_handlerFactory.getRoomManager().getRoom(this->m_room.getRoomData().id).getAllUsers();
    std::lock_guard<std::mutex> lock(this->genMtx);
    this->m_handlerFactory.getRoomManager().getRoom(this->m_room.getRoomData().id).deleteUser(users[0]); // delete the admin from the room

    auto user = users.begin(); 
    user++; // start from the member users.

    // run on all of the users and remove them.
    for (; user != users.end(); user++)
    {
        this->m_handlerFactory.getRoomManager().getRoom(this->m_room.getRoomData().id).deleteUser(*user); // delete room meber.

        this->m_clients[user->getSock()] = (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(*user); // update the user handler


        //create a leave room packet and send to the current user.
        LeaveRoomResponse res = { LEAVE_ROOM_RESPONSE };
        std::vector<unsigned char> buffer = JsonResponsePacketSerializer::serializeResponse(res);
        Helper::sendData(user->getSock(), buffer);
    }

    this->m_handlerFactory.getRoomManager().deleteRoom(this->m_room.getRoomData().id); // delete the room.
    CloseRoomResponse resRoom = { CLOSE_ROOM_RESPONSE };
    //send the complete resault out of the function.
    return { JsonResponsePacketSerializer::serializeResponse(resRoom), (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}


RequestResult RoomAdminRequestHandler::startGame(RequestInfo info)
{
    std::vector<LoggedUser> users = this->m_handlerFactory.getRoomManager().getRoom(this->m_room.getRoomData().id).getAllUsers();

    auto user = users.begin();
    user++; // start from the member users.

    Game& new_game = this->m_handlerFactory.getGameManager().createGame(this->m_handlerFactory.getRoomManager().getRoom(this->m_room.getRoomData().id)); // create the game.

    // starts game for all the users.
    for (; user != users.end(); user++)
    {
        this->m_clients[user->getSock()] = (IRequestHandler*)this->m_handlerFactory.CreateGameRequestHandler(*user, new_game); // set the game to the user.

        //need to add later the actual starting of the game and the creation of the handler.
        StartGameResponse res = { START_GAME_RESPONSE };
        std::vector<unsigned char> buffer = JsonResponsePacketSerializer::serializeResponse(res);
        Helper::sendData(user->getSock(), buffer);
    }
    this->m_handlerFactory.getRoomManager().getRoom(this->m_room.getRoomData().id).setRoomActive(0);// set the room to be not active.
    StartGameResponse res = { START_GAME_RESPONSE };
    //send the complete resault out of the function.
    return { JsonResponsePacketSerializer::serializeResponse(res), (IRequestHandler*)this->m_handlerFactory.CreateGameRequestHandler(this->m_user, new_game) };
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo info)
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

RequestResult RoomAdminRequestHandler::GetPlayersInRoom(RequestInfo info)
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

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, Room room, const LoggedUser& user, std::map<SOCKET, IRequestHandler*>& m_clients) : m_handlerFactory(factory), m_room(room), m_user(user), m_clients(m_clients)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo info)
{
	if (info.id == CLOSE_ROOM_REQUEST ||
		info.id == START_GAME_REQUEST ||
		info.id == GET_ROOM_STATE_REQUEST ||
        info.id == GET_PLAYERS_IN_ROOM_REQUEST) 
	{
		return true;
	}
	return false;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo info, SOCKET sock, std::map<SOCKET, IRequestHandler*>& m_clients)
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
        if (info.id == GET_PLAYERS_IN_ROOM_REQUEST) {
            return this->GetPlayersInRoom(info);
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
