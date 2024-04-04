#pragma once
#include "LoggedUser.h"
#include "StructHelper.h"
#include <vector>

class Room {
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
public:
	Room(const unsigned int id, const std::string& name, const unsigned int maxPlayers, const unsigned int numOfQuestionsInGame, const unsigned int timePerQuestion, const unsigned int isActive);
	void addUser(LoggedUser user);
	void deleteUser(LoggedUser user);
	std::vector<std::string> getAllUsers();

};
