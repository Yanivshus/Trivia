#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "Constants.h"
#include <random>
#include <iostream>
#include <limits>

#define INT_MAX 2147483647

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
	virtual RequestResult handleRequest(RequestInfo info, SOCKET sock, std::map<SOCKET, IRequestHandler*>& m_clients) override;

private:
	RequestHandlerFactory& m_handlerFactory;
	const LoggedUser m_user;

	/// <summary>
	/// sign outs of the current user
	/// </summary>
	RequestResult signout(RequestInfo info);

	/// <summary>
	/// get the active rooms.
	/// </summary>
	RequestResult getRooms(RequestInfo info);

	/// <summary>
	/// get players in a specific room
	/// </summary>
	RequestResult getPlayersInRoom(RequestInfo info);

	/// <summary>
	/// get personal stats of the current player
	/// </summary>
	RequestResult getPersonalStats(RequestInfo info);

	/// <summary>
	/// get the top 5 highest scores.
	/// </summary>
	RequestResult getHighScore(RequestInfo info);

	/// <summary>
	/// join a room by id.
	/// </summary>
	RequestResult joinRoom(RequestInfo info);

	/// <summary>
	/// cretes a room by user settings.
	/// </summary>
	RequestResult createRoom(RequestInfo info, std::map<SOCKET, IRequestHandler*>& m_clients);

	/// <summary>
	/// genrate a id for a room (time * random number(100-1000)) / random number(1-10);
	/// </summary>
	/// <returns>gerated id number</returns>
	int CreateRoomId();
};
