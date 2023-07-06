#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

static int a = 0;
static HANDLE semaphore;

DWORD WINAPI thread_func (LPVOID arg)
{
	DWORD ret = WaitForSingleObject(semaphore, INFINITE);
	
	switch(ret)
	{
	case WAIT_OBJECT_0:
		Sleep(5000);
		std::cout << "Thread hello" << std::endl;
		ReleaseSemaphore(semaphore, 1, NULL);
		break;
	default:
		std::cout << "Error" << std::endl;
		break;
	}
	
	return 0;
}

int main (int argc, char** argv)
{
	HANDLE th[5];
	DWORD nb = 5;
	DWORD id = 0;
	
	semaphore = CreateSemaphore(NULL, 2, 2, NULL);

	for(size_t i = 0; i < nb; i++)
	{
		th[i] = CreateThread (NULL, 0, thread_func, NULL, 0, &id);
	}

	// wait all threads to terminate
	WaitForMultipleObjects (nb, th, TRUE, INFINITE);

	for(size_t i = 0; i < nb; i++)
	{
		CloseHandle (th[i]);
	}

	CloseHandle(semaphore);
	std::cout << "Exit program" << std::endl;
	return EXIT_SUCCESS;
}
