#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "MyClientPacketHandler.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include "Room.h"
#include "DBConnectionPool.h"
#include "DBBind.h"

#include <tchar.h>

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지
		service->GetIocpCore()->Dispatch(10);

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

int main()
{
	// DB
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\ProjectsV13;Database=ServerDb;Trusted_Connection=Yes;"));

	// Create Table
	//{
	//	auto query = L"									\
	//		DROP TABLE IF EXISTS [dbo].[user];			\
	//		CREATE TABLE [dbo].[user]					\
	//		(											\
	//			[id] INT NOT NULL PRIMARY KEY IDENTITY, \
	//			[userId] NVARCHAR(50) NULL,						\
	//			[passWord] NVARCHAR(50) NULL,				\
	//			[name] NVARCHAR(50) NULL,				\
	//			[createDate] DATETIME NULL				\
	//		);";

	//	DBConnection* dbConn = GDBConnectionPool->Pop();
	//	ASSERT_CRASH(dbConn->Execute(query));
	//	GDBConnectionPool->Push(dbConn);
	//}

	ClientPacketHandler::Init();
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	#pragma region MyPacket 직렬화 연습
	/*
	while (true)
	{
		// [PKT_S_TEST]
		PKT_S_TEST_WRITE pktWriter(1001,100,10);

		// [PKT_S_TEST][BuffsListItem BuffsListItem BuffsListItem]
		PKT_S_TEST_WRITE::BuffsList buffList = pktWriter.ReserveBuffsList(3);
		buffList[0] = { 1,30.0f };
		buffList[1] = { 2,5.0f };
		buffList[2] = { 3,13.0f };

		// [PKT_S_TEST][BuffsListItem BuffsListItem BuffsListItem][victim victim victim][victim][victim victim victim]
		PKT_S_TEST_WRITE::BuffsVictimList vic0 = pktWriter.ReserveBuffsVictimsList(&buffList[0], 3);
		{
			vic0[0] = 1000;
			vic0[1] = 2000;
			vic0[2] = 3000;
		}
		PKT_S_TEST_WRITE::BuffsVictimList vic1 = pktWriter.ReserveBuffsVictimsList(&buffList[1], 1);
		{
			vic1[0] = 1000;
		}

		PKT_S_TEST_WRITE::BuffsVictimList vic2 = pktWriter.ReserveBuffsVictimsList(&buffList[2], 2);
		{
			vic2[0] = 500;
			vic2[1] = 5000;
		}


		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();
		this_thread::sleep_for(250ms);


	}
	*/
	#pragma endregion

	// MainThread
	DoWorkerJob(service);

	GThreadManager->Join();
}