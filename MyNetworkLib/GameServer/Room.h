#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{

public:
	~Room() { cout << "Delete Room" << endl; }

	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

	vector<PlayerRef> GetPlayers();

private:
	map<String, PlayerRef> _players;
};

extern shared_ptr<Room> GRoom;