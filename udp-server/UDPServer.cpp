#define _CRT_SECURE_NO_WARNINGS
#include "UDPServer.h"
#include <string.h>
#include <iostream>


void UDPServer::InitServer() {

	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &m_wsaData))
	{
		cout << "Time Server: Error at WSAStartup()\n";
		return;
	}

	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == m_socket)
	{
		cout << "Time Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	m_serverService.sin_family = AF_INET;
	m_serverService.sin_addr.s_addr = INADDR_ANY;	//inet_addr("127.0.0.1");
	m_serverService.sin_port = htons(TIME_PORT);

	if (SOCKET_ERROR == bind(m_socket, (SOCKADDR*)&m_serverService, sizeof(m_serverService)))
	{
		cout << "Time Server: Error at bind(): " << WSAGetLastError() << endl;
		CloseConnection();
		return;
	}

	m_clientAddrLen = sizeof(m_clientAddr);
	cout << "Time Server: Wait for clients' requests.\n";
}

void UDPServer::CloseConnection() {
	cout << "Time Server: Closing Connection.\n";
	closesocket(m_socket);
	WSACleanup();
}

bool UDPServer::closeConnectionAfterException(int i_status) {
	bool ifError = false;

	if (SOCKET_ERROR == i_status)
	{
		cout << "Time Server: " << WSAGetLastError() << endl;
		CloseConnection();
		ifError = true;
	}
	return ifError;
}

void UDPServer::ServerRun() {
	bool runServerLoop = true;
	while (runServerLoop)
	{
		m_bytesRecv = recvfrom(m_socket, m_recvBuff, 2, 0, &m_clientAddr, &m_clientAddrLen);
		if (closeConnectionAfterException(m_bytesRecv))
		{
			return;
		}
		
		m_recvBuff[1] = '\0';
		cout << "Time Server: Recieved: " << m_bytesRecv << " bytes of \"" << m_recvBuff << "\" message.\n";

		fillDataByCurrentRequest();

		m_bytesSent = sendto(m_socket, m_sendBuff, (int)strlen(m_sendBuff), 0, (const sockaddr*)&m_clientAddr, m_clientAddrLen);
		if (closeConnectionAfterException(m_bytesSent))
		{
			return;
		}

		cout << "Time Server: Sent: " << m_bytesSent << "\\" << strlen(m_sendBuff) << " bytes of \"" << m_sendBuff << "\" message.\n";
	}
}

void UDPServer::fillDataByCurrentRequest() {
	char userChoice = m_recvBuff[0];
	switch (userChoice)
	{
	case 'A':
		getFullTime(m_sendBuff);
		break;
	case 'B':
		getTimeWithoutDate(m_sendBuff);
		break;
	case 'C':
		getTimeSinceEpoch(m_sendBuff);
		break;
	case 'D':
		getTimeSinceReset(m_sendBuff);
		break;
	case 'F':
		getTimeWithoutDateOrSeconds(m_sendBuff);
		break;
	case 'G':
		getYear(m_sendBuff);
		break;
	default:
		break;
	}
}

void UDPServer::getFullTime(char o_buffer[SEND_BUFFER_SIZE]) {
	time_t timer;
	time(&timer);
	strcpy(o_buffer, ctime(&timer));
	o_buffer[strlen(o_buffer) - 1] = '\0'; //to remove the new-line from the created string
}

void UDPServer::getTimeWithoutDate(char o_buffer[SEND_BUFFER_SIZE]) {
	addTimeByFilterToBuffer(o_buffer, "%H:%M:%S");
}

void UDPServer::getTimeWithoutDateOrSeconds(char o_buffer[SEND_BUFFER_SIZE]) {
	addTimeByFilterToBuffer(o_buffer, "%H:%M");
}

void UDPServer::getTimeSinceEpoch(char o_buffer[SEND_BUFFER_SIZE]) {
	time_t timer;
	time(&timer);

	snprintf(o_buffer, SEND_BUFFER_SIZE, "%ld", static_cast<long>(timer));
}

void UDPServer::getYear(char o_buffer[SEND_BUFFER_SIZE]) {
	addTimeByFilterToBuffer(o_buffer, "%Y");
}

void UDPServer::getTimeSinceReset(char o_buffer[SEND_BUFFER_SIZE]) {
	DWORD currTime = GetTickCount();
	snprintf(o_buffer, SEND_BUFFER_SIZE, "%lu", static_cast<unsigned long>(currTime));
}

//void UDPServer::getMonthAndDay(char o_buffer[SEND_BUFFER_SIZE]) {
	
//	addTimeByFilterToBuffer(o_buffer, "")
//}

void UDPServer::addTimeByFilterToBuffer(char o_buffer[SEND_BUFFER_SIZE], string i_filter) {
	time_t timer;
	time(&timer);

	struct tm* timeInfo = localtime(&timer);
	strftime(o_buffer, SEND_BUFFER_SIZE, i_filter.c_str(), timeInfo);
}
