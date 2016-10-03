#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>

bool running = true;
// 1 kb
// 500 kb
// 1 mb
// 10 mb
// 100 mb
std::vector<int> probeSizes = { 1024, 512000, 1048576, 10485760, 104857600, 1073741824};

void DoFragCheck() 
{
	HANDLE hPipe = 0;
	hPipe = CreateFile(
		"\\\\.\\pipe\\FRAGGER_PIPE",   // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		0,              // default attributes 
		NULL);          // no template file 

	if (hPipe != INVALID_HANDLE_VALUE)
	{
		OutputDebugString("Connected to pipe");

		// send sample data
		while (running)
		{
			Sleep(1000);

			int succededAlloc = -1;
			for (auto &allocSize : probeSizes)
			{
				OutputDebugString("Alloc");
				void *allocResult = malloc(allocSize);
				if (allocResult != nullptr)
				{
					free(allocResult);
					succededAlloc = allocSize;
				}
				else
				{
					break;
				}
			}

			// convert int to str
			std::stringstream intToStr;
			intToStr << succededAlloc;
			std::string data = intToStr.str() + "\n";

			// write failed size to pipe
			DWORD dwBytesToWrite = data.length();
			DWORD dwBytesWritten = 0;
			if (!WriteFile(hPipe, data.c_str(), dwBytesToWrite, &dwBytesWritten, NULL))
			{
				OutputDebugString("Sampling error...");
			}
			
			if (dwBytesToWrite != dwBytesWritten)
			{
				OutputDebugString("Not everything written");
			}
		}

        CloseHandle(hPipe);
	}
	else
	{
		OutputDebugString("Something went wrong with create file");
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
