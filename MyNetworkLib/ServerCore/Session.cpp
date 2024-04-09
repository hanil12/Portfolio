#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

Session::Session()
: _recvBuffer(BUFFER_SIZE)
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

void Session::Send(SendBufferRef buffer)
{
	if(IsConnected() == false)
		return;

	bool registerSend = false;

	// ���� RegisterSend�� �ɷ����� �ʴٸ� �ɾ��ش�.
	{
		WRITE_LOCK;

		_sendQueue.push(buffer);

		if (_sendRegistered.exchange(true) == false)
			registerSend = true;
	}

	if(registerSend)
		RegisterSend();
}

bool Session::Connect()
{

	return RegisterConnect();
}


void Session::Disconnect(const WCHAR* cause)
{
	if(_conneted.exchange(false) == false)
		return;

	// TEMP
	wcout << "DisConnect : " << cause << endl;

	RegisterDisConnect();
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
	{
		ProcessConnect();
	}
		break;
	case EventType::Disconnect:
	{
		ProcessDisconnect();
	}
	break;
	case EventType::Recv:
	{
		ProcessRecv(numOfBytes);
	}
		break;
	case EventType::Send:
	{
		ProcessSend(numOfBytes);
	}
		break;

	default:
		break;
	}
}

bool Session::RegisterConnect()
{
	if(IsConnected())
		return false;

	if(GetService()->GetServiceType() != ServiceType::Client)
		return false;

	if(SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	if(SocketUtils::BindAnyAddress(_socket,0/*���°�*/) == false)
		return false;

	_connectEvent.Init();
	_connectEvent.owner = shared_from_this();

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();

	if (false == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_connectEvent.owner = nullptr;
			return false;
		}
	}

	return true;
}

bool Session::RegisterDisConnect()
{
	_disConnectEvent.Init();
	_disConnectEvent.owner = shared_from_this();

	if (false == SocketUtils::DisconnectEx(_socket, &_disConnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_disConnectEvent.owner = nullptr; // Release Ref

			return false;
		}
	}

	return true;
}

void Session::RegisterRecv()
{
	if(IsConnected() == false)
		return;

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this(); // ADD REF

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	wsaBuf.len = _recvBuffer.FreeSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT &numOfBytes, OUT &flags, &_recvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_recvEvent.owner = nullptr; // RELASE REF
		}
	}
}

void Session::RegisterSend()
{
	if(IsConnected() == false)
		return;

	_sendEvent.Init();
	_sendEvent.owner = shared_from_this(); // ADD REF

	// ���� �����͸� SendEvent�� ���
	{
		WRITE_LOCK;

		int32 writeSize = 0;
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();

			writeSize += sendBuffer->WriteSize();
			// TODO ����üũ

			_sendQueue.pop();
			_sendEvent.sendBuffers.push_back(sendBuffer);
		}
	}

	// Scatter - Gather (����� �ִ� �����͵��� ��Ƽ� �ѹ濡 ������.)
	Vector<WSABUF> wsaBufs;
	wsaBufs.reserve(_sendEvent.sendBuffers.size());
	for (SendBufferRef sendBuffer : _sendEvent.sendBuffers)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<LONG>(sendBuffer->WriteSize());
		wsaBufs.push_back(wsaBuf);
	}


	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT & numOfBytes, 0, &_sendEvent, nullptr))
	{
		int32 errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_sendEvent.owner = nullptr; // Release Ref
			_sendEvent.sendBuffers.clear(); // Release Ef
			_sendRegistered.store(false);
		}
	}
}

void Session::ProcessConnect()
{
	_connectEvent.owner = nullptr; // Release Ref


	// Ŀ�ؼ��� �ξ�����.
	_conneted.store(true);

	// ���� ���
	GetService()->AddSession(GetSessionRef());

	// ������ �ڵ忡�� �������̵�
	OnConnected();

	// ���� ���
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	_disConnectEvent.owner = nullptr; // Release Ref

	OnDisconnected(); // ������ �ڵ忡�� �������̵�
	GetService()->ReleaseSession(GetSessionRef());
}

void Session::ProcessRecv(int numOfBytes)
{
	_recvEvent.owner = nullptr; // Release Ref

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	if (_recvBuffer.OnWrite(numOfBytes) == false)
	{
		Disconnect(L"OnWrite OverFlow");
		return;
	}

	int32 dataSize = _recvBuffer.DataSize();

	// ������ �����ε�
	// processLen : ���� �����Ϳ��� ó���� ������ �ǹ��Ѵ�.
	int32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);
	if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false)
	{
		Disconnect(L"OnRead OverFlow");
		return;
	}

	// Read, Write Ŀ�� ����
	_recvBuffer.Clean();

	// ���� ����
	RegisterRecv();
}

void Session::ProcessSend(int numofBytes)
{
	_sendEvent.owner = nullptr; // Release Ref
	_sendEvent.sendBuffers.clear();

	if (numofBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	// ������ �ڵ忡�� �������̵�
	OnSend(numofBytes);

	WRITE_LOCK;
	if(_sendQueue.empty())
		_sendRegistered.store(false);
	else
		RegisterSend();
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"Handle Error");
		break;

	default:
		cout << "Handle Error : " << errorCode << endl;

		break;
	}
}

PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

// [size(2)][id(2)][data..]
int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
	int32 processLen = 0;

	while (true)
	{
		int32 dataSize = len - processLen;

		// �ּ��� ����� �Ľ��� �� �־���Ѵ�.
		if(dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *reinterpret_cast<PacketHeader*>(&buffer[processLen]);
		// ����� ��ϵ� ��Ŷ ũ�⸦ �Ľ��� �� �־���Ѵ�.
		if(dataSize < header.size)
			break;

		// ��Ŷ ���� ����
		OnRecvPacket(&buffer[0], header.size);

		processLen += header.size;
	}

	return processLen;
}
