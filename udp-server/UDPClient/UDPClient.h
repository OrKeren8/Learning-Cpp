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

	int m_bytesSent = 0;
	int m_bytesRecv = 0;
	char m_sendBuff[2] = "A";
	char m_recvBuff[255];

	void sendData(bool i_toPrint=false);
	void receiveData(bool i_toPrint=false);
	void printMenu();
	void getClientToServerDelayEstimation(char i_userChoice);
	void implementTaskByCurrentRequest(char i_userChoice);
	void basicReq(char i_userChoice);
	void measureRTT(char i_userChoice);

public:
	void init();
	void run();
	void closeConnection();
};

