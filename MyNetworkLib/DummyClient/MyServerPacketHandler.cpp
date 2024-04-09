#include "pch.h"
#include "MyServerPacketHandler.h"
#include "BufferReader.h"
#include "Protocol.pb.h"

void MyServerPacketHandler::HandlerPacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;


	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST_ProtoBuff(buffer,len);
		break;

	default:
		break;
	}

}

#pragma region MyPacketTest
#pragma pack(1) // dummy byte
// => 유효 검사를 가변 데이터를 하나씩 검사하면서 하는 것은 비효율 적이다.
// 고정길이 부터 다 보내고, 이후에 가변 데이터를 놓는다.
// [PKT_S_TEST] [    가변 데이터들     ]
// [PKT_S_TEST][BuffsListItem BuffsListItem BuffsListItem][victim victim victim][victim][victim victim victim]
struct MyPKT_S_TEST
{
	struct BuffListItem
	{
		uint64 buffId;
		float remainTime;

		uint16 victimsOffset;
		uint16 victimsCount;

		bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size)
		{
			if(victimsOffset + victimsCount * sizeof(uint64) > packetSize)
				return false;

			size += victimsCount * sizeof(uint64);
			return true;
		}
	};


	uint16 packetSize; // 공용헤더
	uint16 packetId; // 공용 헤더
	uint64 id;
	uint32 hp;
	uint16 attack;

	uint16 buffsOffset; // 가변 데이터의 시작 위치이며 고정길이데이터들의 합
	uint16 buffsCount;
	//vector<BuffData> buffs;
	// 가변 데이터
	// 1. 문자열
	// 2. 일반 바이트 배열
	// 3. 리스트
	//wstring name;

	// 유효 검사를 처음 패킷 받을 때 한번만 하도록
	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(MyPKT_S_TEST);
		if(packetSize < size)
			return false;

		if (buffsOffset + buffsCount * sizeof(BuffListItem) > packetSize)
			return false;

		size += buffsCount * sizeof(BuffListItem);

		BuffsList buffList = GetBuffList();
		for (int32 i = 0; i < buffList.Count(); i++)
		{
			if(buffList[i].Validate(reinterpret_cast<BYTE*>(this), packetSize, OUT size) == false)
				return false;
		}

		// 최종 크기 비교
		if(size != packetSize)
			return false;

		return true;
	}

	using BuffsList = PacketList<MyPKT_S_TEST::BuffListItem>;
	using BuffsVictimsList = PacketList<uint64>;

	BuffsList GetBuffList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsOffset; // 배열 시작 위치
		MyPKT_S_TEST::BuffListItem* start = reinterpret_cast<MyPKT_S_TEST::BuffListItem*>(data);

		return BuffsList(start, buffsCount);
	}

	BuffsVictimsList GetBuffsVictimList(const BuffListItem* buffsItem)
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsItem->victimsOffset;
		return BuffsVictimsList(reinterpret_cast<uint64*>(data), buffsItem->victimsCount);
	}
};
#pragma pack()
#pragma endregion

/*
void MyServerPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	//Vaild Test
	if(len < sizeof(MyPKT_S_TEST))
		return;

	MyPKT_S_TEST* pkt = reinterpret_cast<MyPKT_S_TEST*>(buffer);

	//PKT_S_TEST pkt;
	//br >> pkt;

	if(pkt->Validate() == false)
		return;

	// cout << "ID : " << id << " HP : " << hp << " Attack : " << attack << endl;

	MyPKT_S_TEST::BuffsList buffsList = pkt->GetBuffList();


	cout << "BufCount : " << buffsList.Count() << endl;

	for (auto& buff : buffsList)
	{
		cout << "BuffId : " << buff.buffId << " BuffRemainTime : " << buff.remainTime << endl;

		auto victimList = pkt->GetBuffsVictimList(&buff);

		cout << "Victim Count : " << victimList.Count() << endl;

		for (auto& victim : victimList)
		{
			cout << "Victim : " << victim << endl;
		}
	}
}
*/

void MyServerPacketHandler::Handle_S_TEST_ProtoBuff(BYTE* buffer, int32 len)
{
	Protocol::S_TEST packet;

	void* startPos = buffer + sizeof(PacketHeader);
	ASSERT_CRASH(packet.ParseFromArray(startPos, len - sizeof(PacketHeader)))

	cout << "ID: " << packet.id() << endl;
	cout << "HP: " << packet.hp() << endl;
	cout << "ATTACK: " << packet.attack() << endl;
	cout << "Buffs Count : " << packet.buffs().size() << endl;

	for (auto buff : packet.buffs())
	{
		cout << "  Buff ID : " << buff.buffid() << endl;
		cout << "  Buff RT : " << buff.remaintime() << endl;
		cout << "  Victims Count : " << buff.victims().size() << endl;

		for (auto victim : buff.victims())
		{
			cout << "    Victim ID : " << victim << endl;
		}
	}
}
