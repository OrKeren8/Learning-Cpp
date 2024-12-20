#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string.h>
#include "UDPClient.h"

using namespace std;



int main()
{
	UDPClient udpClient = UDPClient();
	udpClient.init();
	udpClient.run();
	udpClient.closeConnection();

	return 0;
}