#include "Room.h"

Room::Room(RoomData data)
{
	this->m_metadata = data;
}

void Room::addUser(LoggedUser user)
{
	if (this->m_metadata.maxPlayers < this->m_users.size())//checking if adding the player will overflow the room size.
	{
		throw std::exception("The room is full.");
	}
	this->m_users.push_back(user);
}

void Room::deleteUser(LoggedUser user)
{
	// get the user to delete
	auto it = this->m_users.begin();
	for (; it != this->m_users.end(); ++it) {
		if (it->getUserName() == user.getUserName()) {
			break;  // Found the user, stop the loop
		}
	}
	// Delete the user if found
	if (it != this->m_users.end()) {
		this->m_users.erase(it);
	}
	else {
		throw std::exception("User not in the room.");
	}
}

std::vector<LoggedUser> Room::getAllUsers() const
{
	std::vector<LoggedUser> Users;
	for (auto i = this->m_users.begin(); i != this->m_users.end(); i++) // loop through the user list and add theyre username to string vector.
	{
		Users.push_back(*i);
	}
	return Users;
}
