#pragma once

#include "BufferReader.h"
#include "BufferWriter.h"
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GMyPacketHandler[UINT16_MAX];

// TODO : 자동화
enum MyPacketID : uint16
{
	S_TEST = 1,
	S_LOGIN = 2
};

// TODO : 자동화
bool MyHandle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt);
bool MyHandle_INVALID_S(PacketSessionRef& session, BYTE* buffer, int32 len);

class MyClientPacketHandler
{
public:
	// TODO 자동화
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
		{
			GMyPacketHandler[i] = MyHandle_INVALID_S;
		}

		// 자동화
		GMyPacketHandler[S_TEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
		{
			return HandlePacket<Protocol::S_TEST>(MyHandle_S_TEST, session, buffer, len);
		};
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GMyPacketHandler[header->id](session, buffer, len);
	}

	// 자동화
	static SendBufferRef MakeSendBuffer_S_TEST(Protocol::S_TEST& pkt) { return _MakeSendBuffer(pkt, S_TEST); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType packet;
		void* startPos = buffer + sizeof(PacketHeader);
		if(packet.ParseFromArray(startPos, len - sizeof(PacketHeader)) == false)
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

#pragma region MyPacketTest
// [PKT_S_TEST][BuffsListItem BuffsListItem BuffsListItem][victim victim][victim]
#pragma pack(1) 
struct MyPKT_S_TEST
{
public:
	struct BuffListItem
	{
		uint64 buffId;
		float remainTime;

		// victim List
		uint16 victimsOffset;
		uint16 victimsCount;
	};

	uint16 packetSize; // 공용헤더
	uint16 packetId; // 공용 헤더
	uint64 id;
	uint32 hp;
	uint16 attack;

	uint16 buffsOffset; // 가변 데이터의 시작 위치이며 고정길이데이터들의 합
	uint16 buffsCount;
};
#pragma pack()

template<typename T, typename C>
class PacketIterator
{
public:
	PacketIterator(C& container, uint16 index) : _container(container), _index(index) {}

	bool operator!=(const PacketIterator& other) const { return _index != other._index; }
	const T& operator*() { return _container[_index]; }
	T* operator->() { return &_container[_index]; }
	PacketIterator& operator++() { ++_index; return *this; }
	PacketIterator operator++(int32) { PacketIterator temp = *this; ++_index; return temp; }

private:
	C& _container;
	uint16 _index;
};

// 임시객체를 만드는 작업을 줄이는 작업.
// 버퍼를 그냥 배열로 생각해서 뽑아쓴다.
template<typename T>
class PacketList
{
public:
	PacketList() : _data(nullptr), _count(0) {}
	PacketList(T* data, uint16 count) : _data(data), _count(count) {}

	T& operator[](uint16 index)
	{
		ASSERT_CRASH(index < _count);

		return _data[index];
	}

	uint16 Count() { return _count; }

	PacketIterator<T, PacketList<T>> begin() { return PacketIterator<T, PacketList<T>>(*this, 0); }
	PacketIterator<T, PacketList<T>> end() { return PacketIterator<T, PacketList<T>>(*this, _count); }

private:
	T* _data;
	uint16 _count;
};

class PKT_S_TEST_WRITE
{
public:
	using BuffsListItem = MyPKT_S_TEST::BuffListItem;
	using BuffsList = PacketList<MyPKT_S_TEST::BuffListItem>;
	using BuffsVictimList = PacketList<uint64>;

	PKT_S_TEST_WRITE(uint64 id, uint32 hp, uint16 attack)
	{
		_sendBuffer = GSendBufferManager->Open(4096);
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<MyPKT_S_TEST>();
		_pkt->packetSize = 0; //
		_pkt->packetId = S_TEST;
		_pkt->id = id;
		_pkt->hp = hp;
		_pkt->attack = attack;
		_pkt->buffsOffset = 0; //
		_pkt->buffsCount = 0; // 
	}

	BuffsList ReserveBuffsList(uint16 buffCount)
	{
		BuffsListItem* firstBuffList = _bw.Reserve<BuffsListItem>(buffCount);
		_pkt->buffsCount = buffCount;
		_pkt->buffsOffset = (uint64)firstBuffList - (uint64)_pkt;

		return BuffsList(firstBuffList, buffCount);
	}

	BuffsVictimList ReserveBuffsVictimsList(BuffsListItem* buffsItem, uint16 victimCount)
	{
		uint64* firstVictimsListItem = _bw.Reserve<uint64>(victimCount);
		buffsItem->victimsCount = victimCount;
		buffsItem->victimsOffset = reinterpret_cast<uint64>(firstVictimsListItem) - reinterpret_cast<uint64>(_pkt);

		return BuffsVictimList(firstVictimsListItem, victimCount);
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());

		return _sendBuffer;
	}

private:
	MyPKT_S_TEST* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma endregion