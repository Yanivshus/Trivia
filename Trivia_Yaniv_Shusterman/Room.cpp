#include "Room.h"

Room::Room(const unsigned int id, const std::string& name, const unsigned int maxPlayers, const unsigned int numOfQuestionsInGame, const unsigned int timePerQuestion, const unsigned int isActive)
{
	//set room data.
	RoomData data;
	data.id = id;
	data.name = name;
	data.maxPlayers = maxPlayers;
	data.numOfQuestionsInGame = numOfQuestionsInGame;
	data.timePerQuestion = timePerQuestion;
	data.isActive = isActive;
	this->m_metadata = data;

}

void Room::addUser(LoggedUser user)
{
	this->m_users.push_back(user);
}

void Room::deleteUser(LoggedUser user)
{
	// look for the user to delete.
	for (auto i = this->m_users.begin(); i != this->m_users.end(); i++)
	{
		// if user found i will delete him from the vector.
		if (i->getUserName() == user.getUserName()) 
		{
			i = this->m_users.erase(i);
		}
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
