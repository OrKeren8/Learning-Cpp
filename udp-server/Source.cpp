#define _CRT_SECURE_NO_WARNINGS
#include "UDPServer.h"


void main()
{
	UDPServer udpServer;
	udpServer.InitServer();
	udpServer.ServerRun();
	udpServer.CloseConnection();
}

	