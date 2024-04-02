#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler {
private:
	RequestHandlerFactory& m_handlerFactory;
public:
	MenuRequestHandler(RequestHandlerFactory& m_handlerFactory);
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
};
