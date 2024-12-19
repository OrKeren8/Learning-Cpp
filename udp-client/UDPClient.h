#pragma once
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>

class UDPClient
{
private:
	WSAData wsaData;
	SOCKET connSocket;
	sockaddr_in server;

public:
	void init();
	void sendData();
};

