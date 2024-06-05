#pragma once
#include "Room.h"
#include "LoggedUser.h"
#include <map>
#include <mutex>

class RoomManger {
private:
	std::map<int, Room> m_rooms;
public:

	// creates a room, adds it to the map.
	void createRoom(LoggedUser creator, RoomData data);

	// deletes a rooom , removes from map.
	void deleteRoom(const int id);

	// checks if room is active.
	unsigned int getRoomState(const int id);

	// gets all rooms.
	std::vector<RoomData> getRooms();

	// get a specific room
	Room& getRoom(const int id);

	////singleton by reference.
	static RoomManger& getInstance() {
		static RoomManger theRoomManager;
		return theRoomManager;
	}
};
