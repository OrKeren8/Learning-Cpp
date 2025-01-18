#include "TCPServer.h"


void TCPServer::RunServer() {
	while (true) {
		ServerLoop();
	}
}


void TCPServer::ServerLoop()
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
			if ((requests[i].recv == LISTEN) || (requests[i].recv == RECEIVE)) {
				FD_SET(requests[i].id, &waitRecv);
			}
		}

		fd_set waitSend;
		FD_ZERO(&waitSend);
		for (int i = 0; i < MAX_SOCKETS; i++)
		{
			if (requests[i].send == SEND) {
				FD_SET(requests[i].id, &waitSend);
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
			if (FD_ISSET(requests[i].id, &waitRecv))
			{
				time_t now = time(0);

				if (now - requests[i].requestTime > 120)
				{
					closesocket(requests[i].id);
					requests[i].recv = EMPTY;
					requests[i].send = EMPTY;
				}
				else
				{
					nfd--;
					switch (requests[i].recv)
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
			if (FD_ISSET(requests[i].id, &waitSend))
			{
				nfd--;
				if (requests[i].send == SEND)
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
	bool res = false;

	for (int i = 0; (i < MAX_SOCKETS) && !res; i++)
	{
		if (requests[i].recv == EMPTY)
		{
			requests[i].id = socketDescriptor;
			requests[i].requestTime = time(0);
			requests[i].len = 0;
			requests[i].recv = stateType;
			requests[i].send = IDLE;
			activeSocketCount++;
			res = true;
		}
	}
	return res;
}

void TCPServer::resetSocket(int idx)
{
	requests[idx].recv = EMPTY;
	requests[idx].send = EMPTY;
}

void TCPServer::unregisterSocket(int idx)
{
	resetSocket(idx);
	decrementSocketCount();
}

void TCPServer::handleNewConnection(int idx)
{
	SOCKET id = requests[idx].id;
	struct sockaddr_in clientAddress;		
	int clientAddressLen = sizeof(clientAddress);

	SOCKET clientSocket = accept(id, (struct sockaddr*)&clientAddress, &clientAddressLen);

	if (INVALID_SOCKET == clientSocket)
	{
		cout << "Time Server: Error at accept(): " << WSAGetLastError() << endl;
		return;
	}
	cout << "Time Server: Client " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << " is connected." << endl;

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
	requests[i_SokcetIndex].reqBuffer = bufferRequestStart;

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


		requests[i_SokcetIndex].fileName = "C:\\temp" + fileName;

		static const std::unordered_map<string, eHTTPRequestType> requestMap = {
			{"OPTIONS", OPTIONS},
			{"GET", GET},
			{"HEAD", HEAD},
			{"POST", POST},
			{"PUT", PUT},
			{"DELETE", DELETE_},
			{"TRACE", TRACE}
		};

		requests[i_SokcetIndex].requestType = requestMap.at(requestType);
		requests[i_SokcetIndex].send = SEND;
	}
}

void TCPServer::decrementSocketCount()
{
	activeSocketCount--;
}

void TCPServer::processIncomingMessage(int idx)
{
	SOCKET msgSocket = requests[idx].id;
	int currentLength = requests[idx].len;
	int bytesRecv = recv(msgSocket, &requests[idx].buffer[currentLength], sizeof(requests[idx].buffer) - currentLength, 0);

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
		requests->requestTime = time(0);
		requests[idx].buffer[currentLength + bytesRecv] = '\0'; 
		cout << "HTTP Server: Recieved: \" " << bytesRecv << " bytes of \"" << &requests[idx].buffer[currentLength] << "\" message.\n";
		requests[idx].len += bytesRecv;

		if (requests[idx].len > 0)
		{
			handleHttpRequest(idx, requests[idx].buffer);
		}
	}

}

void TCPServer::processOutgoingMessage(int idx)
{
	SOCKET msgSocket = requests[idx].id;
	string sendbuff, httpHeader, fullMsg;
	int responseBufferLength = 0;
	int bytesSent = 0;

	switch (requests[idx].requestType)
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
		string getMsg = Get(idx, &sendbuff, &responseBufferLength);
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader + sendbuff;
		break;
	}
	case HEAD:
	{
		string getMsg = Get(idx, &sendbuff, &responseBufferLength);
		responseBufferLength = 0;
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader;
		break;
	}
	case POST:
	{
		string getMsg = Post(const_cast<char*>(requests[idx].reqBuffer.c_str()));
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader;
		break;
	}
	case PUT:
	{
		string getMsg = Put(idx);
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader;

		break;
	}
	case DELETE_:
	{
		string getMsg = Delete(idx);
		GetMessagesHEAD(idx, &httpHeader, responseBufferLength);
		fullMsg = getMsg + httpHeader;
		break;
	}
	case TRACE:
	{
		string getMsg = Trace(idx, requests[idx].buffer, &sendbuff, &responseBufferLength);
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

	memset(requests[idx].buffer, '\0', 1024);
	requests[idx].len = 0;
	requests[idx].send = IDLE;
}

void TCPServer::appendLanguageToFileName(int idx)
{
	string lang = requests[idx].fileName.substr(requests[idx].fileName.find("=") + 1);
	requests[idx].fileName.erase(requests[idx].fileName.find("?"));

	if (lang == HEBREW_STR)
	{
		requests[idx].fileName.insert(requests[idx].fileName.find("."), "-he");
	}
	else if (lang == ENGLISH_STR)
	{
		requests[idx].fileName.insert(requests[idx].fileName.find("."), "-en");
	}
	else if (lang == FRANCH_STR)
	{
		requests[idx].fileName.insert(requests[idx].fileName.find("."), "-fr");
	}
}

string TCPServer::parseRequestBody(const char* buffer, int bufferLen)
{
	std::string buf(buffer, bufferLen);
	size_t headerEndPos = buf.find("\r\n\r\n");

	if (headerEndPos != std::string::npos) {
		size_t bodyStartPos = headerEndPos + 4;
		return buf.substr(bodyStartPos);
	}
}

string TCPServer::readFileContent(enum eSupportedLanguages langOfPage, string fileName, string* sendBuff, int* responseBufferLength)
{
	size_t PositionOfDot = fileName.find(".");
	string strArr[3] = { "-he", "-en", "-fr" };

	if (PositionOfDot != string::npos)
	{
		if (fileName.find("?lang") != string::npos)
		{
			fileName.erase(fileName.find("?"));
			fileName.insert(PositionOfDot, strArr[langOfPage]);
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
	oss << "Date: " << ctime(&(requests[idx].requestTime))
		<< "Server: HTTP\r\n"
		<< "Content-length: " << (responseBufferLength) << "\r\n"
		<< "Content-type: text/html\r\n\r\n";

	(*sendbuff) += oss.str();

	return "HTTP/1.1 404 NOT FOUND\r\n";
}

string TCPServer::Get(int idx, string* sendbuff, int* responseBufferLength)
{
	string lang;

	if (requests[idx].fileName.find("?lang") != string::npos)
	{
		lang = requests[idx].fileName.substr(requests[idx].fileName.find("=") + 1);
		if (strncmp(lang.c_str(), HEBREW_STR, 2) == 0)
		{
			return readFileContent(HEBREW, requests[idx].fileName, sendbuff, responseBufferLength);
		}
		else if (strncmp(lang.c_str(), ENGLISH_STR, 2) == 0)
		{
			return  readFileContent(ENGLISH, requests[idx].fileName, sendbuff, responseBufferLength);
		}
		else if (strncmp(lang.c_str(), FRANCH_STR, 2) == 0)
		{
			return readFileContent(FRENCH, requests[idx].fileName, sendbuff, responseBufferLength);
		}
	}
	else
	{
		return readFileContent(ENGLISH, requests[idx].fileName, sendbuff, responseBufferLength);
	}
}

string TCPServer::Put(int idx)
{
	string requestedLangauge;

	if (requests[idx].fileName.find("?lang") != EOF)
	{
		appendLanguageToFileName(idx);
	}
	else
	{
		requests[idx].fileName.insert(requests[idx].fileName.find("."), "-en"); //add english file 
	}
	return manageFileOperations(requests[idx].fileName, const_cast<char*>(requests[idx].reqBuffer.c_str()), requests[idx].len);
}

string TCPServer::Post(char* buffer)
{
	cout << "POST request content:";
	buffer = buffer + ((string)buffer).find("\n\r") + 3; //to find the request body
	cout << buffer << endl;

	return  "HTTP/1.1 200 OK\r\n";
}

string TCPServer::Trace(int idx, char* buffer, string* sendbuff, int* responseBufferLength)
{
	*sendbuff += buffer;
	*responseBufferLength = (*sendbuff).size();

	return "HTTP/1.1 200 No Content\r\n";
}

string TCPServer::Delete(int idx)
{
	if (requests[idx].fileName.find("?lang") != string::npos)
	{
		appendLanguageToFileName(idx);
	}

	if (remove(requests[idx].fileName.c_str()) != 0)
	{
		cout << "The file was not deleted" << endl;
		return "HTTP/1.1 404 NOT FOUND\r\n";
	}

	return "HTTP/1.1 204 No Content\r\n";
}

string TCPServer::GetMessagesOPTIONS(int idx, string* sendbuff, int* responseBufferLength)
{
	(*sendbuff) += "Server Options are:\n HEAD, GET, PUT, POST, TRACE, DELETE, OPTIONS.\n";
	*responseBufferLength += (*sendbuff).length();
	return "HTTP/1.1 204 No Content\r\n";
}