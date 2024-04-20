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

	/// <summary>
	/// gets the currrent question for the user.
	/// </summary>
	RequestResult getQuestion(RequestInfo info);

	/// <summary>
	/// submit the current answer, update the wrong or correct answer by checkig if the answer is correct. then moves to the next question.
	/// </summary>
	RequestResult submitAnswer(RequestInfo info);

	/// <summary>
	/// returns the game results if everyone finished the game. ele returns empty results with appropriate code.
	/// </summary>
	RequestResult getGameResult(RequestInfo info);

	/// <summary>
	/// leaves the game but saves the player stats in the game in the db.
	/// </summary>
	RequestResult leaveGame(RequestInfo info);

	/// <summary>
	/// saves user results in the db.
	/// </summary>
	/// <param name="user:">user object.</param>
	/// <param name="data:">stats of the game of the user.</param>
	void saveInDB(const LoggedUser& user, const GameData& data);
};