#pragma once

enum
{
	S_TEST = 1
};

class MyServerPacketHandler
{
public:
	static void HandlerPacket(BYTE* buffer, int32 len);

	//static void Handle_S_TEST(BYTE* buffer, int32 len);
	static void Handle_S_TEST_ProtoBuff(BYTE* buffer, int32 len);
};

#pragma region MyPacketTest
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

	PacketIterator<T, PacketList<T>> begin() { return PacketIterator<T,PacketList<T>>(*this, 0); }
	PacketIterator<T, PacketList<T>> end() { return PacketIterator<T,PacketList<T>>(*this, _count); }

private:
	T* _data;
	uint16 _count;
};
#pragma endregion