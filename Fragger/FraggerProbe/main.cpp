#include <windows.h>
#include <iostream>
#include <sstream>

bool running = true;

void DoFragCheck() 
{
	HANDLE hPipe = 0;
	hPipe = CreateFile(
		L"\\\\.\\pipe\\FRAGGER_PIPE",   // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		0,              // default attributes 
		NULL);          // no template file 

	if (hPipe != INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"Connected to pipe");

		// send sample data
		while (running)
		{
			Sleep(1000);
			std::string data;

			if (malloc(100000000)) // add some bytes... continue here
			{
				OutputDebugString(L"OK");
				data = "OK\n";
			}
			else
			{
				OutputDebugString(L"FAILED");
				data = "FAILED\n";
			}

			DWORD dwBytesToWrite = data.length() + 1;
			DWORD dwBytesWritten = 0;
			if (!WriteFile(hPipe, data.c_str(), dwBytesToWrite, &dwBytesWritten, NULL))
			{
				OutputDebugString(L"Sampling error...");
			}
			
			if (dwBytesToWrite != dwBytesWritten)
			{
				OutputDebugString(L"Not everything written");
			}
		}

        CloseHandle(hPipe);
	}
	else
	{
		OutputDebugString(L"Something went wrong with create file");
	}


}

BOOL WINAPI DllMain(
  __in  HINSTANCE hinstDLL,
  __in  DWORD fdwReason,
  __in  LPVOID lpvReserved
)
{
	int threadData = 1;
	switch (fdwReason) 
	{
		case DLL_PROCESS_ATTACH:
			DoFragCheck();
			break;
		case DLL_PROCESS_DETACH:
			running = false;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			running = false;
			break;
	}
	return 0;
}
