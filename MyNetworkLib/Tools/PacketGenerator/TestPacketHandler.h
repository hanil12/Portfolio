#pragma once

#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
// Python
	PKT_S_TEST = 1000,
	PKT_C_LOGIN = 1001,
	PKT_S_LOGIN = 1002,
	PKT_C_ENTER_GAME = 1003,
	PKT_S_ENTER_GAME = 1004,
	PKT_S_CHAT = 1005,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);

// Python
bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt);
bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt);

class TestPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
		{
			GPacketHandler[i] = Handle_INVALID;
		}

// Python
		GPacketHandler[PKT_C_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
		{
			return HandlePacket<Protocol::C_LOGIN>(Handle_C_LOGIN, session, buffer, len);
		};
		GPacketHandler[PKT_C_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
		{
			return HandlePacket<Protocol::C_ENTER_GAME>(Handle_C_ENTER_GAME, session, buffer, len);
		};
		
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

// Python
	static SendBufferRef MakeSendBuffer_S_TEST(Protocol::S_TEST& pkt) { return _MakeSendBuffer(pkt, PKT_S_TEST); }
	static SendBufferRef MakeSendBuffer_S_LOGIN(Protocol::S_LOGIN& pkt) { return _MakeSendBuffer(pkt, PKT_S_LOGIN); }
	static SendBufferRef MakeSendBuffer_S_ENTER_GAME(Protocol::S_ENTER_GAME& pkt) { return _MakeSendBuffer(pkt, PKT_S_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer_S_CHAT(Protocol::S_CHAT& pkt) { return _MakeSendBuffer(pkt, PKT_S_CHAT); }


private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType packet;
		void* startPos = buffer + sizeof(PacketHeader);
		if (packet.ParseFromArray(startPos, len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, packet);
	}

	template<typename T>
	static SendBufferRef _MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;

		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));

		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};