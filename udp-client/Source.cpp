#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string.h>
#include "UDPClient.h"

using namespace std;



void main()
{
	UDPClient udpClient = UDPClient();
	udpClient.init();
	udpClient.sendData();
	udpClient.closeConnection();
}