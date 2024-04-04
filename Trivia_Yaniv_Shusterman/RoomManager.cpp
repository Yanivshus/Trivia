#include "RoomManager.h"

void RoomManger::createRoom(LoggedUser creator, RoomData data)
{
	auto it = this->m_rooms.find(data.id);
	if (it != this->m_rooms.end()) // check if there is already a room in the map so there wont be an error.
	{
		throw std::exception("Room already exists");
	}
	else {
		Room newRoom(data);
		this->m_rooms[data.id] = data;// add room to the mao
		try {
			this->m_rooms[data.id].addUser(creator);// try to add user to the room.
		}
		catch(std::exception& e){
			throw e; // rom is full (max players is 0.)
		}
	}
}

void RoomManger::deleteRoom(const int id)
{
	auto it = this->m_rooms.find(id);
	if (it != this->m_rooms.end()) // check if there is already a room in the map so there wont be an error.
	{
		this->m_rooms.erase(id);//delete map entry.
	}
	else {
		throw std::exception("Room doesn't exists");
	}
}

unsigned int RoomManger::getRoomState(const int id)
{
	auto it = this->m_rooms.find(id);
	if (it != this->m_rooms.end()) // check if there is already a room in the map so there wont be an error.
	{
		return this->m_rooms[id].isRoomActive();
	}
	else {
		throw std::exception("Room doesn't exists");
	}
}

std::vector<RoomData> RoomManger::getRooms()
{
	std::vector<RoomData> dataOnRooms;

	// run on the room and get theyre data.
	for (auto i = this->m_rooms.begin(); i != this->m_rooms.end(); i++)
	{
		dataOnRooms.push_back(i->second.getRoomData());
	}

	return dataOnRooms;
}

Room& RoomManger::getRoom(const int id)
{
	auto it = this->m_rooms.find(id);
	if (it != this->m_rooms.end()) // check if there is already a room in the map so there wont be an error.
	{
		return this->m_rooms[id];
	}
	else {
		throw std::exception("Room doesn't exists");
	}
}
