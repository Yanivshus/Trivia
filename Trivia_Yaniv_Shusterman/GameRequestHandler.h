#pragma once;

#include "IRequestHandler.h"
#include "Game.h"
#include "RequestHandlerFactory.h"
#include "Constants.h"
#include "GameData.h"

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler {

public:
	virtual bool isRequestRelevant(RequestInfo info) override;
	virtual RequestResult handleRequest(RequestInfo info, SOCKET sock, std::map<SOCKET, IRequestHandler*>& m_clients) override;

	GameRequestHandler(const LoggedUser& m_user, RequestHandlerFactory& m_handlerFactory, Game& game); 


private:
	Game& m_game;
	const LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult getQuestion(RequestInfo info);
	RequestResult submitAnswer(RequestInfo info);
	RequestResult getGameResult(RequestInfo info);
	RequestResult leaveGame(RequestInfo info);

	void saveInDB(const LoggedUser& user, const GameData& data);
};