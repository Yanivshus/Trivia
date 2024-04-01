#pragma once
//each value represnts a possiable code in the requests.

#define SIZE_OF_START 5
enum Codes {
	ERROR_RESPONSE = 12,
	LOGIN_RESPONSE = 13,
	SIGNUP_RESPONSE = 14,
	LOGIN_REQUEST = 100,
	SIGNUP_REQUEST =101
};