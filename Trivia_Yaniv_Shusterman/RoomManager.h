#pragma once
#include "Room.h"
#include "LoggedUser.h"
#include <map>
#include <mutex>

class RoomManger {
private:
	std::map<int, Room> m_rooms;
public:
	void createRoom(LoggedUser creator, RoomData data);
	void deleteRoom(const int id);
	unsigned int getRoomState(const int id);
	std::vector<RoomData> getRooms();
	Room& getRoom(const int id);
};
