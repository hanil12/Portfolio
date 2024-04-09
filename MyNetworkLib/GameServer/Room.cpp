#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"

shared_ptr<Room> GRoom = make_shared<Room>();

void Room::Enter(PlayerRef player)
{
	_players[player->name] = player;
}

void Room::Leave(PlayerRef player)
{
	if(_players.count(player->name) != 0)
		_players.erase(player->name);

	Protocol::S_LEAVE_GAME leavePkt;
	leavePkt.set_name(Utils::UTF16toUTF8(player->name));
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leavePkt);

	DoAsync(&Room::Broadcast, sendBuffer);
}

void Room::Broadcast(SendBufferRef sendBuffer)
{
	for (auto& p : _players)
	{
		p.second->ownerSession->Send(sendBuffer);
	}
}

vector<PlayerRef> Room::GetPlayers()
{
	vector<PlayerRef> result;
	result.reserve(_players.size());
	for (auto player : _players)
	{
		result.push_back(player.second);
	}

	return result;
}
