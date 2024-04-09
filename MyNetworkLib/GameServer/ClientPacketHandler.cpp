#include "pch.h"
#include "ClientPacketHandler.h"
#include "GameSession.h"
#include "Player.h"
#include "Room.h"
#include "DBConnectionPool.h"
#include "DBBind.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// dbo.user 
// [id] [userid] [passWord] [name]

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log;

	return true;
}

bool Handle_C_CREATE_ACCOUNT(PacketSessionRef& session, Protocol::C_CREATE_ACCOUNT& pkt)
{
	String id = Utils::UTF8toUTF16(pkt.id());
	String passWord = Utils::UTF8toUTF16(pkt.password());
	String name = Utils::UTF8toUTF16(pkt.name());

	int32 count = 0;
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		const WCHAR* query = L"SELECT COUNT(id) FROM [dbo].[user] WHERE userID = (?)";
		{
			DBBind<1, 1> bindHelper(*dbConn, query);

			bindHelper.BindParam(0, id.c_str());
			bindHelper.BindCol(0, count);

			bindHelper.Execute();
			bindHelper.Fetch();
		}

		GDBConnectionPool->Push(dbConn);
	}

	Protocol::S_CREATE_ACCOUNT accountPkt;

	if (count > 0)
	{
		accountPkt.set_success(false);
	}
	else
	{
		accountPkt.set_success(true);

		// Insert
		{
			DBConnection* dbConn = GDBConnectionPool->Pop();

			const WCHAR* query = L"INSERT INTO [dbo].[user]([userId], [passWord], [name]) VALUES(?,?,?)";
			{
				DBBind<3, 0> bindHelper(*dbConn, query);

				bindHelper.BindParam(0, id.c_str());
				bindHelper.BindParam(1, passWord.c_str());
				bindHelper.BindParam(2, name.c_str());

				bindHelper.Execute();
			}

			GDBConnectionPool->Push(dbConn);
		}

		accountPkt.set_id(pkt.id());
		accountPkt.set_password(pkt.password());
		accountPkt.set_name(pkt.name());
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(accountPkt);
	session->Send(sendBuffer);


	return true;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	String id = String(pkt.id().begin(), pkt.id().end());
	String passWord = String(pkt.password().begin(), pkt.password().end());

	int32 count = 0;
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		const WCHAR* query = L"SELECT COUNT(id) FROM [dbo].[user] WHERE userID = (?) AND passWord = (?)";
		{
			DBBind<2, 1> bindHelper(*dbConn, query);

			bindHelper.BindParam(0, id.c_str());
			bindHelper.BindParam(1, passWord.c_str());
			bindHelper.BindCol(0, count);

			bindHelper.Execute();
			bindHelper.Fetch();
		}

		GDBConnectionPool->Push(dbConn);
	}

	Protocol::S_LOGIN logPkt;

	if (count == 0)
	{
		logPkt.set_success(false);
		// TODO : 로그인 실패
	}
	else
	{
		logPkt.set_success(true);
		WCHAR name[50];
		{
			DBConnection* dbConn = GDBConnectionPool->Pop();

			const WCHAR* query = L"SELECT name FROM [dbo].[user] WHERE userID = (?) AND passWord = (?)";
			{
				DBBind<2, 1> bindHelper(*dbConn, query);

				bindHelper.BindParam(0, id.c_str());
				bindHelper.BindParam(1, passWord.c_str());
				bindHelper.BindCol(0, name, 50);

				bindHelper.Execute();
				bindHelper.Fetch();
			}

			GDBConnectionPool->Push(dbConn);
		}

		logPkt.set_name(Utils::UTF16toUTF8(name));
		// TODO : 로그인 성공
	}
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(logPkt);
	session->Send(sendBuffer);

	return true;
}


bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = make_shared<Player>();
	player->name = Utils::UTF8toUTF16(pkt.name());
	player->ownerSession = gameSession;
	gameSession->SetPlayer(player);
	gameSession->SetRoom(GRoom);

	GRoom->Enter(player);

	Protocol::S_ENTER_GAME enterPkt;
	enterPkt.set_success(true);

	for (auto playerData : GRoom->GetPlayers())
	{
		auto player = enterPkt.add_players();
		player->set_name(Utils::UTF16toUTF8(playerData->name));
	}

	auto enterSendBuffer = ClientPacketHandler::MakeSendBuffer(enterPkt);
	GRoom->Broadcast(enterSendBuffer);

	// 입장 Announce
	Protocol::S_CHAT announcePkt;
	wstring temp = player->name.c_str();
	temp += L"께서 입장하셧습니다.";
	String announceMsg = temp.c_str();
	announcePkt.set_msgtype(Protocol::MsgType::ANNOUNCE);
	announcePkt.set_msg(Utils::UTF16toUTF8(announceMsg));

	auto announceSendBuffer =ClientPacketHandler::MakeSendBuffer(announcePkt);
	GRoom->DoAsync(&Room::Broadcast, announceSendBuffer);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	String playerName = Utils::UTF8toUTF16(pkt.name());
	String msg = Utils::UTF8toUTF16(pkt.msg());

	wcout.imbue(locale("kor"));

	std::wcout << playerName << ": " << msg << std::endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_name(pkt.name());
	chatPkt.set_msgtype(Protocol::MsgType::PLAYER);
	chatPkt.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	GRoom->DoAsync(&Room::Broadcast, sendBuffer);

	return true;
}

