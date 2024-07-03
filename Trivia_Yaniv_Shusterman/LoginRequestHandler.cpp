#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& m_handlerFactory) : m_handlerFactory(m_handlerFactory)
{
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo info)
{
    //checks if the request is either login or signup.
    if (info.id == LOGIN_REQUEST || info.id == SIGNUP_REQUEST) 
    {
        return true;
    }
    else 
    {
        return false;
    }
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo info, SOCKET sock, std::map<SOCKET, IRequestHandler*>& m_clients)
{

    try {
        if (info.id == LOGIN_REQUEST)
        {
            //for now.
            std::cout << "Logging in..." << std::endl;
            LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);// turning the buffer to a struct for easy use.

            std::cout << "Password: " << req.password << std::endl;
            std::cout << "Username: " << req.username << std::endl;

            LoginManager& log_manager = this->m_handlerFactory.getLoginManager();//getting the login manager
            log_manager.login(req.username, req.password, sock);// trying to log the user.
            
            //setting the next handler to be the menu.
            LoginResponse res = { LOGIN_RESPONSE };
            return { JsonResponsePacketSerializer::serializeResponse(res) , (IRequestHandler*)this->m_handlerFactory.createMenuRequestHandler(LoggedUser(req.username, sock))};
        }

        else if (info.id == SIGNUP_REQUEST)
        {
            //for now.
            std::cout << "Signing up..." << std::endl;
            SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);// turning the buffer to a struct for easy use.
            std::cout << "Password: " << req.password << std::endl;
            std::cout << "Username: " << req.username << std::endl;
            std::cout << "Email: " << req.email << std::endl;

            LoginManager& log_manager = this->m_handlerFactory.getLoginManager();//getting the login manager
            log_manager.signup(req.username, req.password, req.email, req.adrres, req.phone, req.date);// trying to sign up the user.

            //creating new respone, the next handler will be still nullptr, because after signing up you still need to log in.
            SignupResponse res = { SIGNUP_RESPONSE };
            return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
        }
    }
    catch (std::exception& e) // if eny thrown exceptions caught , return a error response.
    {
        std::cout << e.what() << std::endl;
        ErrorResponse res = { "FAILED" };
        return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
    }
    catch(nlohmann::json::exception& e)
    {
        ErrorResponse res = { "ERROR : INVALID JSON "};
        return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
    }
}
