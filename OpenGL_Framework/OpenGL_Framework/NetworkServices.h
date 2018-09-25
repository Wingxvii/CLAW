#pragma once

#include <winsock2.h>
#include <Windows.h>
class NetworkServices
{
public:
	static int sendMessage(SOCKET curSocket, const char * message, int messageSize);
	static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
};

