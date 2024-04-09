#pragma once
#include "Session.h"
#include "Room.h"

class GameSession : public PacketSession
{
public:
	GameSession()
	{
		cout <<"GameSession Create" << endl;
	}

	~GameSession()
	{
		cout << "GameSession Delete" << endl;
		_currentPlayer = nullptr;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	void SetPlayer(PlayerRef player) { _currentPlayer = player; }
	void SetRoom(shared_ptr<Room> room) { _room = room; }
private:
	PlayerRef _currentPlayer;
	weak_ptr<class Room> _room;
};

