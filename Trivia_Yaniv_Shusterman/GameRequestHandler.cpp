#include "GameRequestHandler.h"

bool GameRequestHandler::isRequestRelevant(RequestInfo info)
{
	if (info.id == GET_GAME_RESULTS_REQUEST ||
		info.id == SUBMIT_ANSWER_REQUEST ||
		info.id == GET_QUESTION_REQUEST ||
		info.id == LEAVE_GAME_REQUEST) 
	{
		return true;
	}
	return false;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo info, SOCKET sock, std::map<SOCKET, IRequestHandler*>& m_clients)
{
	return RequestResult();
}

GameRequestHandler::GameRequestHandler(const LoggedUser& m_user, RequestHandlerFactory& m_handlerFactory, Game& game) : m_user(m_user) , m_handlerFactory(m_handlerFactory) , m_game(m_game)
{
}
