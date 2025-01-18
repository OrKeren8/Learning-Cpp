#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <sstream>
#include <string.h>
#include <time.h>
#include  <fstream>
#include <unordered_map>
#include <winsock2.h>
using namespace std;

#define MAX_SOCKETS 60
#define TIME_PORT 8080


enum eHTTPRequestType
{
	OPTIONS,
	GET,
	HEAD,
	POST,
	PUT,
	DDELETE,
	TRACE,
};

enum eSocketStateType
{
	EMPTY,
	LISTEN,
	RECEIVE,
	IDLE,
	SEND,
};


enum eSupportedLanguages
{
	HEBREW,
	ENGLISH,
	FRENCH,
	NONE,
};

struct SocketState
{
	SOCKET id;
	string reqBuffer;
	string fileName;
	int	recv;
	int	send;
	char buffer[1024];
	eHTTPRequestType requestType;
	int len;
	time_t requestTime;
};

extern int activeSocketCount;
extern struct SocketState socketStates[MAX_SOCKETS];

class TCPServer
{

public:
	
	struct SocketState socketStates[MAX_SOCKETS] = { 0 };
	int activeSocketCount = 0;

	bool registerSocket(SOCKET socketDescriptor, enum eSocketStateType stateType); //registerSocket
	void resetSocket(int idx); //added to looks different
	void unregisterSocket(int idx); //unregisterSocket
	void handleNewConnection(int idx); //handleNewConnection
	void handleHttpRequest(int i_SokcetIndex, char* i_BuffRequest); //handleHttpRequest
	void decrementSocketCount(); //added to looks different
	void processIncomingMessage(int idx); //processIncomingMessage
	void processOutgoingMessage(int idx); //processOutgoingMessage
	void appendLanguageToFileName(int idx); //appendLanguageToFileName
	string parseRequestBody(const char* buffer, int bufferLen); //parseRequestBody
	string readFileContent(enum eSupportedLanguages langOfPage, string fileName, string* sendBuff, int* responseBufferLength); //readFileContent
	string manageFileOperations(const string& fileName, char* buffer, int buffLen); //manageFileOperations
	string GetMessagesHEAD(int idx, string* sendbuff, int responseBufferLength); //GetMessagesHEAD
	string GetMessagesGET(int idx, string* sendbuff, int* responseBufferLength); //GetMessagesGET
	string GetMessagesPUT(int idx); //GetMessagesPUT
	string GetMessagesPOST(char* buffer); //GetMessagesPOST
	string GetMessagesTRACE(int idx, char* buffer, string* sendbuff, int* responseBufferLength); //GetMessagesTRACE
	string GetMessagesDELETE(int idx); //GetMessagesDELETE
	string GetMessagesOPTIONS(int idx, string* sendbuff, int* responseBufferLength); //GetMessagesOPTIONS
	void MainServerLoop();

};

