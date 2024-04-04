#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& m_handlerFactory) : m_handlerFactory(m_handlerFactory)
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo info)
{
    return false;
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo info)
{
    return RequestResult();
}
