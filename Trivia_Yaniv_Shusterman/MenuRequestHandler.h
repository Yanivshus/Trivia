#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "Constants.h"

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

	RequestResult signout(RequestInfo);
};
