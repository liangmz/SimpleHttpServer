#include "INCLUDE.h"


extern DWORD WINAPI Close_Handle(LPVOID par)
{
	HANDLE *handle = (HANDLE*) par; 
	WaitForSingleObject(handle,INFINITE);
	CloseHandle(handle);

	cout << "******* Close Handel : " << handle <<" *******" << "\r\n\r\n\r\n\r\n\r\n";

	return NULL;
}