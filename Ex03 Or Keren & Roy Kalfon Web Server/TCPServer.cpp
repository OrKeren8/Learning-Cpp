#include "TCPServer.h"



void TCPServer::MainServerLoop()
{
	WSAData wsaData;

	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Server: Error at WSAStartup()\n";
		return;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == listenSocket)
	{
		cout << "Time Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}
	sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(TIME_PORT);
	if (SOCKET_ERROR == bind(listenSocket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		cout << "Time Server: Error at bind(): " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return;
	}
	if (SOCKET_ERROR == listen(listenSocket, 5))
	{
		cout << "Time Server: Error at listen(): " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return;
	}
	registerSocket(listenSocket, LISTEN);
	cout << "server is listening at port " << TIME_PORT << ": " << endl;
	while (true)
	{
		fd_set waitRecv;
		FD_ZERO(&waitRecv);
		for (int i = 0; i < MAX_SOCKETS; i++)
		{
			if ((socketStates[i].recv == LISTEN) || (socketStates[i].recv == RECEIVE)) {
				FD_SET(socketStates[i].id, &waitRecv);
			}
		}

		fd_set waitSend;
		FD_ZERO(&waitSend);
		for (int i = 0; i < MAX_SOCKETS; i++)
		{
			if (socketStates[i].send == SEND) {
				FD_SET(socketStates[i].id, &waitSend);
			}
		}
		int nfd;
		timeval time1;
		time1.tv_sec = 120;
		time1.tv_usec = 0;
		nfd = select(0, &waitRecv, &waitSend, NULL, &time1);

		if (nfd == SOCKET_ERROR)
		{
			cout << "Time Server: Error at select(): " << WSAGetLastError() << endl;
			WSACleanup();
			return;
		}

		for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
		{
			if (FD_ISSET(socketStates[i].id, &waitRecv))
			{
				time_t now = time(0);

				if (now - socketStates[i].requestTime > 120)
				{
					closesocket(socketStates[i].id);
					socketStates[i].recv = EMPTY;
					socketStates[i].send = EMPTY;
				}
				else
				{
					nfd--;
					switch (socketStates[i].recv)
					{
					case LISTEN:
						handleNewConnection(i);
						break;

					case RECEIVE:
						processIncomingMessage(i);
						break;
					}
				}
			}
		}

		for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
		{
			if (FD_ISSET(socketStates[i].id, &waitSend))
			{
				nfd--;
				if (socketStates[i].send == SEND)
				{
					processOutgoingMessage(i);
				}
			}
		}
	}

	cout << "Time Server: Closing Connection.\n";
	closesocket(listenSocket);
	WSACleanup();
}

bool TCPServer::registerSocket(SOCKET socketDescriptor, enum eSocketStateType stateType)
{
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (socketStates[i].recv == EMPTY)
		{
			socketStates[i].id = socketDescriptor;
			socketStates[i].requestTime = time(0);
			socketStates[i].len = 0;
			socketStates[i].recv = stateType;
			socketStates[i].send = IDLE;
			activeSocketCount++;
			return (true);
		}
	}
	return (false);
}

void TCPServer::resetSocket(int idx)
{
	socketStates[idx].recv = EMPTY;
	socketStates[idx].send = EMPTY;
}

void TCPServer::unregisterSocket(int idx)
{
	resetSocket(idx);
	decrementSocketCount();
}

void TCPServer::handleNewConnection(int idx)
{
	struct sockaddr_in clientAddress;		// Address of sending partner
	SOCKET id = socketStates[idx].id;
	int clientAddressLen = sizeof(clientAddress);
	SOCKET clientSocket = accept(id, (struct sockaddr*)&clientAddress, &clientAddressLen);

	if (INVALID_SOCKET == clientSocket)
	{
		cout << "Time Server: Error at accept(): " << WSAGetLastError() << endl;
		return;
	}
	cout << "Time Server: Client " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << " is connected." << endl;

	//
	// Set the socket to be in non-blocking mode.
	//
	unsigned long flag = 1;
	if (ioctlsocket(clientSocket, FIONBIO, &flag) != 0)
	{
		cout << "Time Server: Error at ioctlsocket(): " << WSAGetLastError() << endl;
	}

	if (registerSocket(clientSocket, RECEIVE) == false)
	{
		cout << "\t\tToo many connections, dropped!\n";
		closesocket(id);
	}
	return;
}

void TCPServer::handleHttpRequest(int i_SokcetIndex, char* i_BuffRequest)
{
	string bufferRequestStart(i_BuffRequest);
	socketStates[i_SokcetIndex].reqBuffer = bufferRequestStart;

	string request(i_BuffRequest);
	size_t position = request.find(' ');
	if (position != string::npos)
	{
		string requestType = request.substr(0, position);
		string fileName = request.substr(position + 1);
		size_t positionState = fileName.find(' ');
		if (positionState != string::npos)
		{
			fileName = fileName.substr(0, positionState);
		}


		socketStates[i_SokcetIndex].fileName = "C:\\temp" + fileName;

		static const std::unordered_map<string, eHTTPRequestType> requestMap = {
			{"OPTIONS", OPTIONS},
			{"GET", GET},
			{"HEAD", HEAD},
			{"POST", POST},
			{"PUT", PUT},
			{"DELETE", DDELETE},
			{"TRACE", TRACE}
		};

		socketStates[i_SokcetIndex].requestType = requestMap.at(requestType);
		socketStates[i_SokcetIndex].send = SEND;
	}
}

void TCPServer::decrementSocketCount()
{
	activeSocketCount--;
}

void TCPServer::processIncomingMessage(int idx)
{
	SOCKET msgSocket = socketStates[idx].id;
	int currentLength = socketStates[idx].len;
	//int ou = sizeof(socketStates[index].buffer);
	int bytesRecv = recv(msgSocket, &socketStates[idx].buffer[currentLength], sizeof(socketStates[idx].buffer) - currentLength, 0);

	if (SOCKET_ERROR == bytesRecv)
	{
		cout << "HTTP Server: Error at recv(): " << WSAGetLastError() << endl;
		closesocket(msgSocket);
		unregisterSocket(idx);
		return;
	}
	if (bytesRecv == 0)
	{
		closesocket(msgSocket);
		unregisterSocket(idx);
		return;
	}
	else
	{
		socketStates->requestTime = time(0);
		socketStates[idx].buffer[currentLength + bytesRecv] = '\0'; //add the null-terminating to make it a string
		cout << "HTTP Server: Recieved: \" " << bytesRecv << " bytes of \"" << &socketStates[idx].buffer[currentLength] << "\" message.\n";
		socketStates[idx].len += bytesRecv;

		if (socketStates[idx].len > 0)
		{
			handleHttpRequest(idx, socketStates[idx].buffer);
		}
	}

}

void TCPServer::processOutgoingMessage(int idx)
{
	SOCKET msgSocket = socketStates[idx].id;
	string sendbuff, httpHeader, fullMsg;
	int responseBufferLength = 0;
	int bytesSent = 0;

	switch (socketStates[idx].requestType)
	{
	case OPTIONS:
	{
		string optionsMsg = GetMessagesOPTIONS(idx, &sendbuff, &responseBufferLength);
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = optionsMsg + httpHeader + sendbuff;
		break;
	}
	case GET:
	{
		string getMsg = GetMessagesGET(idx, &sendbuff, &responseBufferLength);
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader + sendbuff;
		break;
	}
	case HEAD:
	{
		string getMsg = GetMessagesGET(idx, &sendbuff, &responseBufferLength);
		responseBufferLength = 0;
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader;
		break;
	}
	case POST:
	{
		string getMsg = GetMessagesPOST(const_cast<char*>(socketStates[idx].reqBuffer.c_str()));
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader;
		break;
	}
	case PUT:
	{
		string getMsg = GetMessagesPUT(idx);
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader;

		break;
	}
	case DDELETE:
	{
		string getMsg = GetMessagesDELETE(idx);
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader;
		break;
	}
	case TRACE:
	{
		string getMsg = GetMessagesTRACE(idx, socketStates[idx].buffer, &sendbuff, &responseBufferLength);
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader + sendbuff;
		break;
	}

	default:
	{
		fullMsg = " HTTP / 1.1 405 Not Allowed\r\n";
	}
	}

	bytesSent = send(msgSocket, fullMsg.c_str(), fullMsg.size(), 0);
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "HTTP Server: Error at send(): " << WSAGetLastError() << endl;
		return;
	}
	cout << "HTTP Server: Sent: " << bytesSent << "\\" << fullMsg.size() << " bytes of \"" << fullMsg << "\" message.\n";

	memset(socketStates[idx].buffer, '\0', 1024);
	socketStates[idx].len = 0;
	socketStates[idx].send = IDLE;
}

void TCPServer::appendLanguageToFileName(int idx)
{
	string requestedLangauge = socketStates[idx].fileName.substr(socketStates[idx].fileName.find("=") + 1);
	socketStates[idx].fileName.erase(socketStates[idx].fileName.find("?"));

	if (requestedLangauge == "he")
	{
		socketStates[idx].fileName.insert(socketStates[idx].fileName.find("."), "-he");
	}
	else if (requestedLangauge == "en")
	{
		socketStates[idx].fileName.insert(socketStates[idx].fileName.find("."), "-en");
	}
	else if (requestedLangauge == "fr")
	{
		socketStates[idx].fileName.insert(socketStates[idx].fileName.find("."), "-fr");
	}
}

string TCPServer::parseRequestBody(const char* buffer, int bufferLen)
{
	// Convert the buffer to a string to use string operations
	std::string bufferStr(buffer, bufferLen);

	// Find the position of the end of the headers
	size_t headerEndPos = bufferStr.find("\r\n\r\n");

	if (headerEndPos != std::string::npos) {
		// The body starts after the end of the headers
		size_t bodyStartPos = headerEndPos + 4;
		return bufferStr.substr(bodyStartPos);
	}
}

string TCPServer::readFileContent(enum eSupportedLanguages langOfPage, string fileName, string* sendBuff, int* responseBufferLength)
{
	size_t PositionOfDot = fileName.find(".");

	if (PositionOfDot != string::npos)
	{
		if (fileName.find("?lang") != string::npos)
		{
			fileName.erase(fileName.find("?"));

			switch (langOfPage)
			{
			case HEBREW:
			{
				fileName.insert(PositionOfDot, "-he");
				break;
			}
			case ENGLISH:
			{
				fileName.insert(PositionOfDot, "-en");
				break;
			}
			case FRENCH:
			{
				fileName.insert(PositionOfDot, "-fr");
				break;
			}
			}
		}
		else
		{
			fileName.insert(PositionOfDot, "-en");
		}
	}

	ifstream inFile(fileName);
	ostringstream tempStream;

	if (inFile.is_open())
	{
		string line;
		while (getline(inFile, line))
		{
			tempStream << line;
			(*responseBufferLength) += line.length();
		}

		inFile.close();

		*sendBuff += tempStream.str();
		*sendBuff += "\r\n";
		(*responseBufferLength) += 2;

		return "HTTP/1.1 200 OK\r\n";
	}
	else
	{
		inFile.close();
		return "HTTP/1.1 404 NOT FOUND\r\n";
	}
}

string TCPServer::manageFileOperations(const string& fileName, char* buffer, int bufferLen)
{
	ofstream onFile(fileName, ios::app);
	string body = parseRequestBody(buffer, bufferLen);
	char* fileText = new char(1024);
	strcpy(fileText, body.c_str());
	if (!onFile.is_open())
	{
		onFile.open(fileName, ios::trunc);
		if (!onFile.is_open())
		{
			return "HTTP/1.1 412 Precondition failed\r\n";
		}
		else
		{
			if (strlen(fileText) != 0)
			{
				while (*fileText != 0)
				{
					onFile << *fileText;
					fileText++;
				}

				if (onFile.fail()) {
					return "HTTP/1.1 501 Not Implemented\r\n";
				}

				onFile.flush();
				onFile.close();
				return "HTTP/1.1 201 Created\r\n";
			}
			else
			{
				if (onFile.fail()) {
					return "HTTP/1.1 501 Not Implemented\r\n";
				}

				onFile.close();
				return "HTTP/1.1 204 No Content\r\n";
			}
		}
	}
	else
	{
		if (strlen(fileText) != 0)
		{
			while (*fileText != 0)
			{
				onFile << *fileText;
				fileText++;
			}
			if (onFile.fail()) {
				return "HTTP/1.1 501 Not Implemented\r\n";
			}
			onFile.flush();
			onFile.close();
			return "HTTP/1.1 200 OK\r\n";
		}
		else
		{
			if (onFile.fail()) {
				return "HTTP/1.1 501 Not Implemented\r\n";
			}
			onFile.close();
			return "HTTP/1.1 204 No Content\r\n";
		}
	}
	delete(fileText);
}

string TCPServer::GetMessagesHEAD(int idx, string* sendbuff, int responseBufferLength)
{
	ostringstream oss;
	oss << "Date: " << ctime(&(socketStates[idx].requestTime))
		<< "Server: HTTP\r\n"
		<< "Content-length: " << (responseBufferLength) << "\r\n"
		<< "Content-type: text/html\r\n\r\n";

	(*sendbuff) += oss.str();

	return "HTTP/1.1 404 NOT FOUND\r\n";
}

string TCPServer::GetMessagesGET(int idx, string* sendbuff, int* responseBufferLength)
{
	string requestedLangauge;

	if (socketStates[idx].fileName.find("?lang") != string::npos)
	{
		requestedLangauge = socketStates[idx].fileName.substr(socketStates[idx].fileName.find("=") + 1);
		if (strncmp(requestedLangauge.c_str(), "he", 2) == 0)
		{
			return readFileContent(HEBREW, socketStates[idx].fileName, sendbuff, responseBufferLength);
		}
		else if (strncmp(requestedLangauge.c_str(), "en", 2) == 0)
		{
			return  readFileContent(ENGLISH, socketStates[idx].fileName, sendbuff, responseBufferLength);
		}
		else if (strncmp(requestedLangauge.c_str(), "fr", 2) == 0)
		{
			return readFileContent(FRENCH, socketStates[idx].fileName, sendbuff, responseBufferLength);
		}
	}
	else
	{
		return readFileContent(ENGLISH, socketStates[idx].fileName, sendbuff, responseBufferLength); //for defult send english
	}
}

string TCPServer::GetMessagesPUT(int idx)
{
	string requestedLangauge;

	if (socketStates[idx].fileName.find("?lang") != EOF)
	{
		appendLanguageToFileName(idx);
	}
	else
	{
		socketStates[idx].fileName.insert(socketStates[idx].fileName.find("."), "-en"); //add english file 
	}
	return manageFileOperations(socketStates[idx].fileName, const_cast<char*>(socketStates[idx].reqBuffer.c_str()), socketStates[idx].len);
}

string TCPServer::GetMessagesPOST(char* buffer)
{
	cout << "POST request content:";
	buffer = buffer + ((string)buffer).find("\n\r") + 3; //to find the request body
	cout << buffer << endl;

	return  "HTTP/1.1 200 OK\r\n";
}

string TCPServer::GetMessagesTRACE(int idx, char* buffer, string* sendbuff, int* responseBufferLength)
{
	*sendbuff += buffer;
	*responseBufferLength = (*sendbuff).size();

	return "HTTP/1.1 200 No Content\r\n";
}

string TCPServer::GetMessagesDELETE(int idx)
{
	if (socketStates[idx].fileName.find("?lang") != string::npos)
	{
		appendLanguageToFileName(idx);
	}

	if (remove(socketStates[idx].fileName.c_str()) != 0)
	{
		cout << "The file was not deleted" << endl;
		return "HTTP/1.1 404 NOT FOUND\r\n";
	}

	return "HTTP/1.1 204 No Content\r\n";
}

string TCPServer::GetMessagesOPTIONS(int idx, string* sendbuff, int* responseBufferLength)
{
	(*sendbuff) += "Allow: HEAD, GET, PUT, POST, TRACE, DELETE, OPTIONS.\n";
	*responseBufferLength += (*sendbuff).length();
	return "HTTP/1.1 204 No Content\r\n";
}