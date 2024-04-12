#pragma once
#include "LoggedUser.h"
#include "StructHelper.h"
#include <vector>

class Room {
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
public:
	Room(RoomData data);
	Room() {};

	/// <summary>
	/// adds user to list of users in the room.
	/// </summary>
	/// <param name="user:">user to add.</param>
	void addUser(LoggedUser user);

	/// <summary>
	/// delete user from users in the room.
	/// </summary>
	/// <param name="user:">user to delete.</param>
	void deleteUser(LoggedUser user);

	std::vector<LoggedUser> getAllUsers();
	
	unsigned int isRoomActive() { return this->m_metadata.isActive; };

	RoomData getRoomData() const { return this->m_metadata; };
};
