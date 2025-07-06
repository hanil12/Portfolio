#include "pch.h"
#include "ServerPacketHandler.h"
#include "Room.h"
#include "Player.h"
#include "Enum.pb.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log

	return true;
}

bool Handle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt)
{
	return true;
}

bool Handle_S_CREATE_ACCOUNT(PacketSessionRef& session, Protocol::S_CREATE_ACCOUNT& pkt)
{
	bool success = pkt.success();

	if (success)
	{
		Protocol::C_LOGIN logInPkt;

		logInPkt.set_id(pkt.id());
		logInPkt.set_password(pkt.password());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(logInPkt);
		session->Send(sendBuffer);
		
		return true;
	}
	else
	{
		Protocol::C_CREATE_ACCOUNT accountPkt;

		String id;
		String passWord;
		String name;

		wcin.imbue(locale("kor"));
		wcout.imbue(locale("kor"));
		std::wcout << L"ȸ������ ����" << endl;
		std::wcout << L"�ٽ� Id�� �Է����ּ���." << endl;
		std::wcin >> id;
		std::wcout << L"�ٽ� ��й�ȣ�� �Է��ּ���." << endl;
		std::wcin >> passWord;
		std::wcout << L"�ٽ� �г����� �Է��ּ���." << endl;
		std::wcin >> name;

		accountPkt.set_id(Utils::UTF16toUTF8(id));
		accountPkt.set_id(Utils::UTF16toUTF8(passWord));
		accountPkt.set_id(Utils::UTF16toUTF8(name));

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(accountPkt);
		session->Send(sendBuffer);
	}

	return true;;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	bool logIn_Success = pkt.success();
	String name = Utils::UTF8toUTF16(pkt.name());

	if (logIn_Success)
	{
		GPlayer->SetSession(session);
		GPlayer->SetName(name);
		// TODO : �α��� ����, ������ 

		Protocol::C_ENTER_GAME enterPkt;
		enterPkt.set_name(Utils::UTF16toUTF8(name));

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterPkt);
		session->Send(sendBuffer);

		GRoom->isActive = true;
		GRoom->SetMyPlayer(GPlayer);
	}
	else
	{
		GPlayer->Init();
		// TODO : �α��� ���� ��α��� ��û
		Protocol::C_CREATE_ACCOUNT accountPkt;

		String id;
		String passWord;
		String name;

		wcin.imbue(locale("kor"));
		wcout.imbue(locale("kor"));
		std::wcout << L"�α��� ����!!!" << endl;
		std::wcout << L"ȸ�������� �Ϸ���..." << endl;
		std::wcout << L"Id�� �Է����ּ���." << endl;
		std::wcin >> id;
		std::wcout << L"��й�ȣ�� �Է��ּ���." << endl;
		std::wcin >> passWord;
		std::wcout << L"�г����� �Է��ּ���." << endl;
		std::wcin >> name;

		accountPkt.set_id(Utils::UTF16toUTF8(id));
		accountPkt.set_password(Utils::UTF16toUTF8(passWord));
		accountPkt.set_name(Utils::UTF16toUTF8(name));

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(accountPkt);
		session->Send(sendBuffer);
	}

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	bool enter_Success = pkt.success();

	if (enter_Success)
	{
		for (auto player : pkt.players())
		{
			String playerName = Utils::UTF8toUTF16(player.name());
			if (!GRoom->IsExist_Name(playerName))
			{
				PlayerRef player = make_shared<Player>();
				player->SetName(playerName);
				GRoom->AddPlayer(player);
			}
		}
		GRoom->DoAsync(&Room::CheckRoom);
	}
	else
	{
		// TODO : ������ ����
		GRoom->Init();
		GRoom->isActive = false;
	}

	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	String leavedPlayer = Utils::UTF8toUTF16(pkt.name());

	GRoom->DoAsync(&Room::ErasePlayer, leavedPlayer);

	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	Protocol::MsgType msgType = pkt.msgtype();
	switch (msgType)
	{
	case Protocol::ANNOUNCE:
	{
		String msg = Utils::UTF8toUTF16(pkt.msg());
		wcout.imbue(locale("kor"));

		wcout << msg << endl;
	}
		break;
	case Protocol::PLAYER:
	{
		String name = Utils::UTF8toUTF16(pkt.name());
		String msg = Utils::UTF8toUTF16(pkt.msg());
		wcout.imbue(locale("kor"));

		wcout << name << ": " << msg << endl;
	}
		break;
	default:
		break;
	}



	return true;
}
