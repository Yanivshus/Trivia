#include "Room.h"

Room::Room(RoomData data)
{
	this->m_metadata = data;
}

void Room::addUser(LoggedUser user)
{
	if (this->m_metadata.maxPlayers < this->m_users.size() + 1)//checking if adding the player will overflow the room size.
	{
		throw std::exception("The room is full.");
	}
	this->m_users.push_back(user);
}

void Room::deleteUser(LoggedUser user)
{
	int found = 0;
	// look for the user to delete.
	for (auto i = this->m_users.begin(); i != this->m_users.end(); i++)
	{
		// if user found i will delete him from the vector.
		if (i->getUserName() == user.getUserName()) 
		{
			i = this->m_users.erase(i);
			found = 1;
		}
	}
	if (found == 0) {
		throw std::exception("User not in the room.");
	}
}

std::vector<std::string> Room::getAllUsers()
{
	std::vector<std::string> stringUsers;
	for (auto i = this->m_users.begin(); i != this->m_users.end(); i++) // loop through the user list and add theyre username to string vector.
	{
		stringUsers.push_back(i->getUserName());
	}
	return stringUsers;
}
