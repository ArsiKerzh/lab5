#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <mutex>

using namespace std;

#define FILE_NUM 4

typedef bool(*foo)(const char*, const char*, unsigned int, unsigned int);	

HINSTANCE dll;	
mutex write;
mutex stop;

DWORD WINAPI threadWrite(LPVOID ptr)
{
	char path[] = "D:/Lab5SPOVm/windll/Debug/files/result.txt";
	int pos = 0;
	int quantity = 0;

	foo writeF = (foo)GetProcAddress(dll, "writeFileFoo");  
	if (!writeF)
	{
		std::cout << "Error getting write function" << std::endl;
		system("pause");
		return 1;
	}

	while (!stop.try_lock())
	{
		write.lock();
		for (int i = 0; i != BUFSIZ && ((char*)ptr)[i] != '\0'; i++)
			quantity++;
		writeF(path, (const char*)ptr, pos, quantity);
		pos += quantity;
		quantity = 0;
		write.unlock();
		Sleep(70);
	}
	stop.unlock();
	std::cout << "End writing" << std::endl;
	return 0;
}

int main()
{
	dll = LoadLibraryA("D:/Lab5SPOVm//windll/Debug/windll.dll");		
	if (dll == NULL)
	{
		std::cout << "Error loading dll" << std::endl;
		system("pause");
		return 1;
	}
	foo readF = (foo)GetProcAddress(dll, "readFileFoo");		
	if (!readF)
	{
		std::cout << "Error getting read function" << std::endl;
		system("pause");
		return 1;
	}
	char path[] = "D:/Lab5SPOVm/windll/Debug/files/file0.txt";
	unsigned int count = 1;
	char buffer[BUFSIZ] = {};

	write.lock();
	stop.lock();

	CreateThread(NULL, NULL, threadWrite, buffer, NULL, NULL);
	for (; count <= FILE_NUM; count++)
	{
		readF(path, buffer, 0, 0);
		write.unlock();
		Sleep(50);
		write.lock();
		if (count + 1 > FILE_NUM)
			stop.unlock();
		for (int i = 0; i != BUFSIZ && buffer[i] != '\0'; i++)
			buffer[i] = {};
		path[36]++;
		Sleep(300);
	}

	FreeLibrary(dll);   
	write.unlock();
	std::cout << "End program" << std::endl;
	system("pause");
	return 0;
}