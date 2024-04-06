#include "MenuRequestHandler.h"


MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& m_handlerFactory, const LoggedUser& m_user) : m_handlerFactory(m_handlerFactory), m_user(m_user)
{
    
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo info)
{
    if (info.id == CREATE_ROOM_REQUEST ||
        info.id == GET_ROOMS_REQUEST ||
        info.id == GET_PLAYERS_IN_ROOM_REQUEST ||
        info.id == JOIN_ROOM_REQUEST ||
        info.id == HIGH_SCORE_REQUEST ||
        info.id == LOGOUT_REQUSET ||
        info.id == GET_PERSONAL_STATS_REQUEST) 
    {
        return true;
    }
    return false;

}

RequestResult MenuRequestHandler::handleRequest(RequestInfo info)
{
    try
    {
        
    }
    catch (const std::exception& e)
    {

    }
}

RequestResult MenuRequestHandler::signout(RequestInfo)
{
    try
    {
        this->m_handlerFactory.getLoginManager().logout(this->m_user.getUserName());// signing out.
        std::cout << "Successfull logout!" << std::endl;

        LogoutResponse outRes;
        outRes.status = LOGOUT_RESPONSE;
        return { JsonResponsePacketSerializer::serializeResponse(outRes), this->m_handlerFactory.createLoginRequestHandler() };
    }
    catch (const std::exception& e) {
        throw e;
    }

}
