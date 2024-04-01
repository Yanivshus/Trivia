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
    if (info.id == LOGIN_REQUEST) 
    {
        //for now.
        std::cout << "Logging in..." << std::endl;
        LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
        std::cout << "Password: " << req.password<< std::endl;
        std::cout << "Username: " << req.username<< std::endl;

        LoginResponse res = { 1 };
        return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
    }

    else if (info.id == SIGNUP_REQUEST) 
    {
        //for now.
        std::cout << "Signing up..." << std::endl;
        SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
        std::cout << "Password: " << req.password << std::endl;
        std::cout << "Username: " << req.username << std::endl;
        std::cout << "Email: " << req.email << std::endl;

        SignupResponse res = { 2 };
        return { JsonResponsePacketSerializer::serializeResponse(res) , nullptr };
    }
}
