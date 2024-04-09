#pragma once

#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"

/// <summary>
/// Session
/// </summary>

class Service;

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;

	enum
	{
		BUFFER_SIZE = 0x1000,
	};

public:
	Session();
	virtual ~Session();

public:
	// 외부에서 사용할 함수들
	void Send(SendBufferRef buffer);
	bool Connect();
	void Disconnect(const WCHAR* cause);

	shared_ptr<Service> GetService() { return _service.lock(); }
	void SetService(shared_ptr<Service> service) { _service = service; }
	

public:
	// 정보 관련
	void SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress GetAddress() { return _netAddress; }
	SOCKET GetSocket() {return _socket; }

	bool IsConnected() { return _conneted; }
	SessionRef GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
	// 인터페이스 구현
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes) override;

private:
	// 전송 관련
	bool RegisterConnect();
	bool RegisterDisConnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int numOfBytes);
	void ProcessSend(int numofBytes);

	void HandleError(int32 errorCode);

protected:
	// 컨텐츠 코드에서 오버라이딩
	virtual void OnConnected() {}
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) {}
	virtual void OnDisconnected() {}

protected:
	weak_ptr<Service> _service;
	SOCKET _socket = INVALID_SOCKET; // ClientSocket
	NetAddress _netAddress = {};
	Atomic<bool> _conneted = false;

private:
	USE_LOCK;
	// 수신 관련
	RecvBuffer _recvBuffer;
	// 송신 관련
	Queue<SendBufferRef> _sendQueue;
	Atomic<bool> _sendRegistered = false;

private:
	// IocpEvent 재사용
	ConnectEvent _connectEvent;
	DisconnectEvent _disConnectEvent;
	RecvEvent _recvEvent;
	SendEvent _sendEvent;
};

// Packet Session

struct PacketHeader
{
	uint16 size; // 가변배열 보내야할 때 사이즈가 필요함.
	uint16 id; // 프로토콜 ID (ex 1 로그인, 2 이동요청)
};

class PacketSession : public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionRef GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32 OnRecv(BYTE* buffer, int32 len) sealed;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) abstract;
};