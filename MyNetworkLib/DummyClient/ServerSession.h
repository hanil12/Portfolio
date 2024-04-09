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
		cout << "���� ���� ����" << endl;

		// ���̵� ��й�ȣ �Է�
		String id;
		String passWord;

		wcin.imbue(locale("kor"));
		wcout.imbue(locale("kor"));
		std::wcout << L"Id�� �Է����ּ���." << endl;
		std::wcin >> id;
		std::wcout << L"��й�ȣ�� �Է��ּ���." << endl;
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