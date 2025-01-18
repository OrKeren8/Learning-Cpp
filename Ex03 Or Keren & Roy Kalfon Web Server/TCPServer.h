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

#define ENGLISH_STR "en"
#define HEBREW_STR "he"
#define FRANCH_STR "en"


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

	bool registerSocket(SOCKET socketDescriptor, enum eSocketStateType stateType); 
	void resetSocket(int idx); 
	void unregisterSocket(int idx); 
	void handleNewConnection(int idx); 
	void handleHttpRequest(int i_SokcetIndex, char* i_BuffRequest); 
	void decrementSocketCount(); 
	void processIncomingMessage(int idx); 
	void processOutgoingMessage(int idx); 
	void appendLanguageToFileName(int idx); 
	string parseRequestBody(const char* buffer, int bufferLen); 
	string readFileContent(enum eSupportedLanguages langOfPage, string fileName, string* sendBuff, int* responseBufferLength); 
	string manageFileOperations(const string& fileName, char* buffer, int buffLen); 
	string GetMessagesHEAD(int idx, string* sendbuff, int responseBufferLength); 
	string GetMessagesGET(int idx, string* sendbuff, int* responseBufferLength); 
	string GetMessagesPUT(int idx); 
	string GetMessagesPOST(char* buffer);
	string GetMessagesTRACE(int idx, char* buffer, string* sendbuff, int* responseBufferLength); 
	string GetMessagesDELETE(int idx);
	string GetMessagesOPTIONS(int idx, string* sendbuff, int* responseBufferLength);
	void MainServerLoop();

};

