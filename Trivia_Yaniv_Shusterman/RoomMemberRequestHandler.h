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
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult leaveRoom(RequestInfo info);
	RequestResult getPlayersInRoom(RequestInfo info);
	RequestResult getRoomState(RequestInfo info);
public:
	RoomMemberRequestHandler(RequestHandlerFactory& factory, Room room, const LoggedUser& user);
	virtual bool isRequestRelevant(RequestInfo info) override;
	virtual RequestResult handleRequest(RequestInfo info, SOCKET sock) override;
};