#pragma once


/// <summary>
/// RecvBuffer
/// </summary>

class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void Clean();
	bool OnRead(int32 numOfBtyes);
	bool OnWrite(int32 numOfBtyes);

	BYTE* ReadPos() { return &_buffer[_readPos]; }
	BYTE* WritePos() { return &_buffer[_writePos]; }
	int32 DataSize() { return _writePos - _readPos; }
	int32 FreeSize() { return _capacity - _writePos; }

private:
	int32 _capacity = 0;
	int32 _bufferSize = 0;
	int32 _readPos = 0;
	int32 _writePos = 0;

	// r,w 겹칠 때 처음 위치로 / w가 더 이상 +될 수 없을 때 복사
	Vector<BYTE> _buffer;
};

