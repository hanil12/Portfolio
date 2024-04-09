#pragma once

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "Container.h"

#include <windows.h>
#include <iostream>
#include <locale>
#include <codecvt>

using namespace std;

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "Lock.h"

#include "ObjectPool.h"
#include "TypeCast.h"
#include "Memory.h"
#include "Session.h"
#include "SendBuffer.h"
#include "LockQueue.h"
