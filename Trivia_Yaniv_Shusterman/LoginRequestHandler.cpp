#include "LoginRequestHandler.h"

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

RequestResult LoginRequestHandler::handleRequest(RequestInfo info)
{
    if (info.id == LOGIN_REQUEST) {
        for (auto& i : info.buffer)
        {
            std::cout << i << std::endl;
        }
        std::cout << info.buffer.size() << std::endl;
        //for now.
        std::cout << "Logging in..." << std::endl;
        LoginRequest log= JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
        std::cout << "Password: " << log.password<< std::endl;
        std::cout << "Username: " << log.username<< std::endl;

        ErrorResponse res = { "login not working :)" };
        return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
    }

    else if (info.id == SIGNUP_REQUEST) {
        for (auto& i : info.buffer)
        {
            std::cout << i << std::endl;
        }
        std::cout << info.buffer.size() << std::endl;
        //for now.
        std::cout << "Signing up..." << std::endl;
        SignupRequest log = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
        std::cout << "Password: " << log.password << std::endl;
        std::cout << "Username: " << log.username << std::endl;
        std::cout << "Email: " << log.email << std::endl;

        ErrorResponse res = { "signup not working :)" };
        return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
    }
}
