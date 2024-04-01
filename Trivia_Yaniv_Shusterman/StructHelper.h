#pragma once
#include <iostream>
#include "Constants.h"
#include <vector>
#include <string>

class IRequestHandler; // fix circular includes.

//responses
typedef struct {
	std::string massage;
} ErrorResponse;

typedef struct {
	int status;
} LoginResponse;

typedef struct {
	int status;
} SignupResponse;


//requests
typedef struct {
	std::string username;
	std::string password;
} LoginRequest;

typedef struct {
	std::string username;
	std::string password;
	std::string email;
} SignupRequest;

typedef struct {
	Codes id;
	time_t time;
	std::vector<unsigned char> buffer;
} RequestInfo;

typedef struct {
	std::vector<unsigned char> buffer;
	IRequestHandler* newHandler;
} RequestResult;




