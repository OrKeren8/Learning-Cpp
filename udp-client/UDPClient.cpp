#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "UDPClient.h"

using namespace std;

#define TIME_PORT	27015

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

void UDPClient::sendData() {
	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[2] = "A";
	char recvBuff[255];

	bytesSent = sendto(m_connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&m_server, sizeof(m_server));
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
		closesocket(m_connSocket);
		WSACleanup();
		return;
	}
	cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

	bytesRecv = recv(m_connSocket, recvBuff, 255, 0);
	if (SOCKET_ERROR == bytesRecv)
	{
		cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
		closesocket(m_connSocket);
		WSACleanup();
		return;
	}

	recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
	cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";

}

void UDPClient::closeConnection() {
	cout << "Time Client: Closing Connection.\n";
	closesocket(m_connSocket);

	system("pause");
}
