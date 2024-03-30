#pragma once
#include <iostream>

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


