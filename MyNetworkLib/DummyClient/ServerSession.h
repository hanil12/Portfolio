#pragma once
#include "ServerPacketHandler.h"
#include "Player.h"

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "ServerSession Delete" << endl;
	}

	virtual void OnConnected() override
	{
		cout << "서버 접속 성공" << endl;

		// 아이디 비밀번호 입력
		String id;
		String passWord;

		wcin.imbue(locale("kor"));
		wcout.imbue(locale("kor"));
		std::wcout << L"Id를 입력해주세요." << endl;
		std::wcin >> id;
		std::wcout << L"비밀번호를 입력주세요." << endl;
		std::wcin>> passWord;

		Protocol::C_LOGIN pkt;

		pkt.set_id(Utils::UTF16toUTF8(id));
		pkt.set_password(Utils::UTF16toUTF8(passWord));

		GPlayer->SetId(id);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		// MyServerPacketHandler::HandlerPacket(buffer, len);
		ServerPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{

	}

	virtual void OnDisconnected() override
	{
		cout << "DisConnected" << endl;
	}
};