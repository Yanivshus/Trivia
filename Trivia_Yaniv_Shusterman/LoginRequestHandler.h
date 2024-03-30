#pragma once
#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler {
public:
	/// <summary>
	/// check if the request is login or signup
	/// </summary>
	/// <param name="info:">info of request.</param>
	/// <returns>true if relevent</returns>
	virtual bool isRequestRelevant(RequestInfo info) override;

	/// <summary>
	/// handle the request, either signup or login, can return error result.
	/// </summary>
	/// <param name="info:">request info.</param>
	/// <returns>result of the hadling (sucess or error)</returns>
	virtual RequestResult handleRequest(RequestInfo info) override;
};
