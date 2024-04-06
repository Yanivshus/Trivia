#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "Constants.h"
#include <random>

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler 
{
public:
	MenuRequestHandler(RequestHandlerFactory& m_handlerFactory, const LoggedUser& m_user);
	/// <summary>
	/// check if the request is relevent to the menu.
	/// </summary>
	/// <param name="info:">info of request.</param>
	/// <returns>true if relevent</returns>
	virtual bool isRequestRelevant(RequestInfo info) override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="info:">request info.</param>
	/// <returns>result of the hadling (sucess or error)</returns>
	virtual RequestResult handleRequest(RequestInfo info) override;

private:
	RequestHandlerFactory& m_handlerFactory;
	LoggedUser m_user;

	//need to add to some of the next correct next handler.
	RequestResult signout(RequestInfo info);
	RequestResult getRooms(RequestInfo info);
	RequestResult getPlayersInRoom(RequestInfo info);
	RequestResult getPersonalStats(RequestInfo info);
	RequestResult getHighScore(RequestInfo info);
	RequestResult joinRoom(RequestInfo info);
	RequestResult createRoom(RequestInfo info);

	/// <summary>
	/// genrate a id for a room (time * random number(100-1000)) / random number(1-10);
	/// </summary>
	/// <returns>gerated id number</returns>
	int CreateRoomId();
};
