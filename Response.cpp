#include "INCLUDE.h"

extern DWORD WINAPI DoResponse(LPVOID par)
{
	Response *response  = (Response*)par;

	printf("Socket Open IP:%s ...\n",inet_ntoa(response->addrClient.sin_addr));

	while(true)
	{
		char revBuffer[1000];
		int recvCode = recv(response->Client , revBuffer , sizeof(revBuffer) , 0);
		if(recvCode == 0)
		{
			cout << "Socket Close " << "IP:" << inet_ntoa(response->addrClient.sin_addr) << endl;
			closesocket(response->Client);
			return 0;
		}

		/*cout << "////////////// recvCode Begin //////////////" << endl;
		cout << recvCode << endl;
		cout << "////////////// recvCode End //////////////" << endl;*/

		int i=0;
		while(revBuffer[i] != -52)
		{
			i++;
		}
		revBuffer[i] = '\0';

		if(revBuffer == NULL || strlen(revBuffer) < 30)
		{
			/*cout << "---------- Error Request Begin: ----------" << endl;*/
			char *responseText = "HTTP/1.1 404 Not Fund \r\nServer:LiangMinZhao\r\nContent-Type: text/html\r\nContent-Length: 0\r\nConnection:keep-alive\r\n\r\n";
			send(response->Client,responseText ,strlen(responseText) , 0);
			/*cout << "---------- Error Request End: ----------" << endl;*/
			continue;
			//return -1;
		}

		/*cout << "*********** revBuffer Data Begin ************" << endl 
			<< revBuffer << endl
			<< "*********** revBuffer Data End ************" << endl ;
*/
		int index = PassProtocol(revBuffer);

		int urlIndex = 0;//获取文件路径
		char pageUrl[1000];
		memset(pageUrl,0,sizeof(pageUrl));
		while(revBuffer[index] != ' ')
		{
			pageUrl[urlIndex++] = revBuffer[index++];
		}

		cout << "URL:" << pageUrl << endl;

		char* htmlPageName = NULL;
		if(strlen(pageUrl) == 1)
		{
			htmlPageName = "./HTMLPage.html";
		}
		else
		{
			htmlPageName = new char[strlen(pageUrl)+1];
			memset(htmlPageName,0,sizeof(htmlPageName));
			htmlPageName[0] = '.';
			strcat(htmlPageName,pageUrl);
		}

		char* html = ReadHtml(htmlPageName);//响应请求
		char* responseText ;
		char* Content_Type;
		char* Content_Length;

		char* httpVersion = "HTTP/1.1 ";
		char* httpServer = " \r\nServer: LiangMinZhao\r\n";

		//通过url获取后缀 设置Content-Type:
		i = 0;
		for(i=strlen(pageUrl)-1 ; i-- ; i>=0)
		{
			if(pageUrl[i] == '.')
				break;
		}
		Content_Type = new char[strlen(pageUrl) - i];
		strcpy(Content_Type,pageUrl+i);
		//

		//-52 未定义字符
		if(strlen(Content_Type) == 2 && Content_Type[0] == -52)
		{
			Content_Type = "\r\n\r\n";
		}

		//Content-Type 设置
		if(KMP(Content_Type,"css") >= 0 || KMP(Content_Type,"CSS") >=0 )
		{
			Content_Type = "Content-Type: text/css\r\n\r\n";
		}
		else if(KMP(Content_Type,"js") >=0 || KMP(Content_Type,"JS") >= 0 )
		{
			Content_Type = "Content-Type: application/x-javascript\r\n\r\n";
		}
		else if(KMP(Content_Type,"html") >= 0 || KMP(Content_Type,"htm") >=0 
			|| KMP(Content_Type,"HTML") >= 0 || KMP(Content_Type,"HTM") >= 0 || html != NULL)
		{
			Content_Type = "Content-Type: text/html\r\n\r\n";
		}


		if(html != NULL){
			char* ContentLength = new char[strlen(html)+1];
			sprintf(ContentLength,"%d",strlen(html));

			Content_Length = new char[strlen("Content-Length: \r\n") + strlen(ContentLength)];
			memset(Content_Length , 0 , sizeof(Content_Length));
			strcat(Content_Length , "Content-Length: ");
			strcat(Content_Length , ContentLength);
			strcat(Content_Length , "\r\n");

			//文件存在
			responseText = "HTTP/1.1 200 OK \r\nServer: LiangMinZhao\r\nConnection:keep-alive\r\n";
			char* temp = responseText;
			responseText = new char[strlen(responseText) + strlen(Content_Type) + strlen(html) + strlen(Content_Length)];
			memset(responseText,0,sizeof(responseText));

			strcat(responseText,temp);
			strcat(responseText , Content_Length);//
			strcat(responseText,Content_Type);

			strcat(responseText,html);

			/*cout << "######### 200 OK ###########" << endl;*/
		}
		else
		{
			//文件不存在
			responseText = "HTTP/1.1 404 Not Fund \r\nServer: LiangMinZhao\r\nContent-Length: 0\r\nConnection:keep-alive\r\n";

			char* temp = responseText;
			responseText = new char[strlen(responseText) + strlen(Content_Type)];
			memset(responseText,0,sizeof(responseText));

			strcat(responseText,temp);
			strcat(responseText,Content_Type);

			/*cout << "######### 404 Not Fund ###########" << endl;*/
		}
		send(response->Client,responseText ,strlen(responseText) , 0);
		/*cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		cout << responseText << endl;
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;*/

		cout << response << endl << endl;
	}
	closesocket(response->Client);
}
