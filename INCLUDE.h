
#include <stdio.h>
#include <WinSock2.h>  
#pragma comment(lib,"ws2_32.lib")

#ifndef _IOSTREAM_H_
#define _IOSTREAM_H_
#include <iostream>
using namespace std;
#endif


#ifndef _STRING_H_
#define _STRING_H_
#include <string.h>
#endif


#ifndef _FSTREAM_H_
#define _FSTREAM_H_
#include <fstream>
#endif


extern void ReadHtml_Default();
extern char* ReadHtml(char *fileName);
extern int KMP(char*S,char*T);
extern int PassProtocol(char *request);

typedef struct Response
{
	SOCKET Client;
	SOCKADDR_IN addrClient;
	int len;
}Response;



#include <windows.h>
#include <process.h> 

extern DWORD WINAPI DoResponse(LPVOID par);
extern DWORD WINAPI Close_Handle(LPVOID par);

#include <map>