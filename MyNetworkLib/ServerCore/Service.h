#pragma once
#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

// 서버도 클라역할을 할수가 있다 -> 분산서버

enum class ServiceType : uint8
{
	Server,
	Client
};

// 서비스
using SessionFactory = function<SessionRef(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool Start() abstract;
	bool CanStart() {return _sessionFactory != nullptr; }
	virtual void CloseService();
	void SetSessionFactory(SessionFactory func) {_sessionFactory = func;}

	// TEMP
	void BroadCast(SendBufferRef sendBuffer);
	SessionRef CreateSession();

	void AddSession(SessionRef session);
	void ReleaseSession(SessionRef session);
	int32 GetCurrentSessionCount() {return _sessionCount;}
	int32 GetMaxSessionCount() {return _maxSessionCount; }

public:
	ServiceType GetServiceType() {return _type; }
	NetAddress GetNetAddress() {return _netAddress;}
	IocpCoreRef& GetIocpCore() { return _iocpCore; }

protected:
	USE_LOCK;

	ServiceType _type;
	NetAddress _netAddress = {};
	IocpCoreRef _iocpCore;

	Set<SessionRef> _sessions;
	int32 _sessionCount = 0;
	int32 _maxSessionCount = 0;
	SessionFactory _sessionFactory;
};

// Client Service
class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService() {}

	virtual bool Start() override;
};


// Server Service
class ServerService : public Service
{
public:
	ServerService(NetAddress Address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService() {}

	virtual bool Start() override;
	virtual void CloseService() override;

private:
	ListenerRef _listener = nullptr;
};
