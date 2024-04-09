#pragma once
#include "JobQueue.h"
class Player;

class Room : public JobQueue
{
public:
	~Room();

	void Init();
	void AddPlayer(PlayerRef player);
	void ErasePlayer(String name);
	void CheckRoom();

	void Chat();

	void SetMyPlayer(PlayerRef player);
	bool IsExist_Name(String name) { return _players.count(name); }

	bool IsActive() { return _myPlayer != nullptr; }
private:
	void SendMsg(String msg);

public:
	bool isActive = false;
private:
	map<String, PlayerRef> _players;
	PlayerRef _myPlayer;
};

extern shared_ptr<Room> GRoom;