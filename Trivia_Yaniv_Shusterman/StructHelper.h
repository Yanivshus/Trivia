#pragma once
#include <iostream>
#include "Constants.h"
#include "Question.h"
#include <vector>
#include <string>
#include <map>

#define ERROR_GAME_RES 0
#define _GAME_RES 1

class IRequestHandler; // fix circular includes.

typedef struct {
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
} RoomData;

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

typedef struct {
	unsigned int status;
} LogoutResponse;

typedef struct {
	unsigned int status;
	std::vector<RoomData> rooms;
} GetRoomsResponse;

typedef struct {
	std::vector<std::string> players;
} GetPlayersInRoomResponse;

typedef struct {
	unsigned int status;
	std::vector<std::string> statistics;
} GetHighScoreResponse;

typedef struct {
	unsigned int status;
	std::vector<std::string> statistics;
} GetPersonalStatsResponse;

typedef struct {
	unsigned int status;
} JoinRoomResponse;

typedef struct {
	unsigned int id;
} CreateRoomResponse;

//v3
typedef struct {
	unsigned int status;
} CloseRoomResponse;


typedef struct {
	unsigned int status;
} StartGameResponse;

typedef struct 
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeOut;
} GetRoomStateResponse;

typedef struct {
	unsigned int status;
} LeaveRoomResponse;

//v4
typedef struct {
	unsigned int status;
} LeaveGameResponse;

typedef struct {
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
} GetQuestionResponse;

typedef struct {
	unsigned int status;
	unsigned int correctAnswerId;
} SubmitAnswerResponse;

typedef struct {
	std::string username;
	unsigned int corrrectAnswerCount;
	unsigned int wrongAnsswerCount;
	unsigned int averageAnswerTime;
} PlayerResults;

typedef struct {
	unsigned int status;
	std::vector<PlayerResults> results;
} GetGameResultsResponse;




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

typedef struct {
	unsigned int roomId;
} GetPlayersInRoomRequest;

typedef struct {
	unsigned int roomId;
} JoinRoomRequest;

typedef struct {
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
} CreateRoomRequest;



typedef struct {
	unsigned int answerId;
} SubmitAnswerRequest;


