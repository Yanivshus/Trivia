#pragma once

#include "IRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "StructHelper.h"
#include "Helper.h"

class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	Room m_room;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

	/// <summary>
	/// this function implements all the close room by admin logic.
	/// </summary>
	/// <returns>resault which is serialized response.</returns>
	RequestResult closeRoom(RequestInfo info);

	/// <summary>
	/// this function implements all the start game by admin logic.
	/// </summary>
	/// <returns>resault which is serialized response.</returns>
	RequestResult startGame(RequestInfo info);

	/// <summary>
	/// this function implements all the get room states by admin logic.
	/// </summary>
	/// <returns>resault which is serialized response.</returns>
	RequestResult getRoomState(RequestInfo info);

	//needed to add this function for lobby functionality.
	RequestResult GetPlayersInRoom(RequestInfo info);
public:
	RoomAdminRequestHandler(RequestHandlerFactory& factory , Room room, const LoggedUser& user);
	virtual bool isRequestRelevant(RequestInfo info) override;
	virtual RequestResult handleRequest(RequestInfo info, SOCKET sock) override;
	
};