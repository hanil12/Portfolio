#include "pch.h"
#include "MyClientPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

PacketHandlerFunc GMyPacketHandler[UINT16_MAX];

bool MyHandle_INVALID_S(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool MyHandle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt)
{
	return false;
}

