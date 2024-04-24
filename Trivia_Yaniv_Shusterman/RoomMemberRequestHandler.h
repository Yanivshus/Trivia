#pragma once
#include "IRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "StructHelper.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler {
private:
	Room m_room;
	const LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

	/// <summary>
	/// removes user from room and returns the menu handler.
	/// </summary>
	RequestResult leaveRoom(RequestInfo info);

	/// <summary>
	/// get active players in room.
	/// </summary>
	RequestResult getPlayersInRoom(RequestInfo info);

	/// <summary>
	/// returns the room state.
	/// </summary>
	RequestResult getRoomState(RequestInfo info);
public:
	RoomMemberRequestHandler(RequestHandlerFactory& factory, Room room, const LoggedUser& user);
	virtual bool isRequestRelevant(RequestInfo info) override;
	virtual RequestResult handleRequest(RequestInfo info, SOCKET sock, std::map<SOCKET, IRequestHandler*>& m_clients) override;
};