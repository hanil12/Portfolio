#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "ServerPacketHandler.h"
#include "JobQueue.h"

shared_ptr<Room> GRoom = make_shared<Room>();

Room::~Room()
{
	Init();
}

void Room::Init()
{
	_myPlayer = nullptr;
	_players.clear();
}

void Room::AddPlayer(PlayerRef player)
{
	_players[player->GetName()] = player;
}

void Room::ErasePlayer(String name)
{
	ASSERT_CRASH(name != _myPlayer->GetName());

	wcout.imbue(locale("kor"));
	wcout << name << L"님 께서 방을 나가셨습니다." << endl;

	_players.erase(name);
	CheckRoom();
}

void Room::CheckRoom()
{
	wcout.imbue(locale("kor"));
	wcout << L"현재 방에 있는 플레이어 : ";

	for (auto iter : _players)
	{
		wcout << iter.first << " ";
	}
	wcout << endl;
}

void Room::Chat()
{
	if(isActive == false)	return;

	//String msg;
	String sendMsg;

	wcin.imbue(locale("kor"));

	std::getline(std::wcin, sendMsg);
	DoAsync(&Room::SendMsg, sendMsg);
}

void Room::SetMyPlayer(PlayerRef player)
{
	// TEMP
	AddPlayer(player);
	_myPlayer = player;
}

void Room::SendMsg(String msg)
{
	Protocol::C_CHAT pkt;
	pkt.set_msg(Utils::UTF16toUTF8(msg));
	pkt.set_name(Utils::UTF16toUTF8(_myPlayer->GetName()));
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	_myPlayer->GetSession()->Send(sendBuffer);
}
