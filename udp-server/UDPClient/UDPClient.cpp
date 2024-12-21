#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "UDPClient.h"

using namespace std;


void UDPClient::init() {


	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &m_wsaData))
	{
		cout << "Time Client: Error at WSAStartup()\n";
		return;
	}

	m_connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_connSocket)
	{
		cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	m_server.sin_family = AF_INET;
	m_server.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_server.sin_port = htons(TIME_PORT);
}

void UDPClient::sendData(bool i_toPrint) {

	m_bytesSent = sendto(m_connSocket, m_sendBuff, (int)strlen(m_sendBuff), 0, (const sockaddr*)&m_server, sizeof(m_server));
	if (SOCKET_ERROR == m_bytesSent)
	{
		cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
		closesocket(m_connSocket);
		WSACleanup();
		return;
	}
	
	if (i_toPrint) {
	cout << "Time Client: Sent: " << m_bytesSent << "/" << strlen(m_sendBuff) << " bytes of \"" << m_sendBuff << "\" message.\n";
	}
}

void UDPClient::receiveData(bool i_toPrint) {
	m_bytesRecv = recv(m_connSocket, m_recvBuff, 255, 0);
	if (SOCKET_ERROR == m_bytesRecv)
	{
		cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
		closesocket(m_connSocket);
		WSACleanup();
		return;
	}

	m_recvBuff[m_bytesRecv] = '\0'; //add the null-terminating to make it a string
	if (i_toPrint) {
		cout << "Time Client: Recieved: " << m_bytesRecv << " bytes of \"" << m_recvBuff << "\" message.\n";
	}
}


void UDPClient::closeConnection() {
	cout << "Time Client: Closing Connection.\n";
	closesocket(m_connSocket);

	system("pause");
}

void UDPClient::getClientToServerDelayEstimation(char i_userChoice) {
	for (int i = 0; i < 100; i++) {

	}
}


void UDPClient::run() {
	bool runningClient = true;
	char userTaskChoice;
	
	cout << endl;

	while (runningClient) {
		printMenu();
		cout << "Enter Task char to excecute:" << endl;
		cin >> userTaskChoice;
		cout << endl;
		implementTaskByCurrentRequest(userTaskChoice);
		cout << endl;
	}
}

void UDPClient::printMenu() {
	cout << "=============== Menu ===============" << endl;
	cout << "A: get time" << endl;
	cout << "B: get time without date" << endl;
	cout << "C: get time since Epoch" << endl;
	cout << "D: get client to server estimated delay" << endl;
	cout << endl;
}

void UDPClient::implementTaskByCurrentRequest(char i_userChoice) {
	switch (i_userChoice)
	{
	case 'A':
		basicReq(i_userChoice);
		break;
	case 'B':
		basicReq(i_userChoice);
		break;
	case 'C':
		basicReq(i_userChoice);
		break;
	case 'D':
		getClientToServerDelayEstimation(i_userChoice);
		break;
	default:
		break;
	}
}

void UDPClient::basicReq(char i_userChoice) {
	m_sendBuff[0] = i_userChoice;
	sendData(true);
	receiveData(true);
}
