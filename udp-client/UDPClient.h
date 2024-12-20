#pragma once
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>

#define TIME_PORT	27015


enum RequestsEnum : char {
	A = 'A',
	B = 'B',
	C = 'C',
	D = 'D'
};


class UDPClient
{
private:
	WSAData m_wsaData;
	SOCKET m_connSocket;
	sockaddr_in m_server;


public:
	void init();
	void sendData();
	void closeConnection();
};

