#include "INCLUDE.h"

int main()
{
	/*cout << ReadHtml("HTMLPage.htm");
	ReadHtml_Default();*/

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(1,1),&wsa) != 0)
	{
		return 0;
	}

	SOCKET Listen_Sock = socket(AF_INET,SOCK_STREAM,6);
	SOCKADDR_IN serverAddr;
	serverAddr.sin_port = htons(26584);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_family = AF_INET;

	bind(Listen_Sock,(SOCKADDR*)&serverAddr,sizeof(SOCKADDR));

	listen(Listen_Sock,5);

	char revBuffer[1000];
	printf( "Server Started.\n");

	//<ip,socket>
	map<string,Response*> Map;

	while(1)
	{
		SOCKADDR_IN addrClient;
		int len = sizeof(SOCKADDR);
		SOCKET sockCon = accept(Listen_Sock,(SOCKADDR*)&addrClient,&len);

		Response *response = new Response();
		response->addrClient = addrClient;
		response->Client = sockCon;
		response->len = len;

		//新建线程，使用说明：http://baike.baidu.com/view/1191444.htm?fr=aladdin
		HANDLE resp = CreateThread(NULL , 0 , DoResponse , response , 0 , NULL);
		//CloseHandle(resp);
		//Map.insert(pair<HANDLE,Response*>(resp,response));

		HANDLE close = CreateThread(NULL , 0 , Close_Handle , &resp , 0 , NULL);
	}
	closesocket(Listen_Sock);
	WSACleanup();

	return 0;
}