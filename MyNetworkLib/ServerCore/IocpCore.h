#pragma once

// IOCP Obj

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes) abstract;
};


/// <summary>
/// IOCO Core
/// </summary>

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() {return _iocpHandle; }

	bool Register(IocpObjectRef iocpObject);
	bool Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE _iocpHandle;
};


