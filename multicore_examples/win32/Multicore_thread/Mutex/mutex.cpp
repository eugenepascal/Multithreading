#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

static int a = 0;
CRITICAL_SECTION mtx;

DWORD WINAPI thread_func(LPVOID arg)
{
	std::cout << "Thread hello" << std::endl;
	
	while(true)
	for (int i = 0; i < 100000; i++)
	{
		EnterCriticalSection(&mtx);
		a++;
		LeaveCriticalSection(&mtx);
	}

	return 0;
}

int main(int argc, char** argv)
{
	HANDLE th[5];
	DWORD nb = 5;
	DWORD id = 0;
	
	InitializeCriticalSection(&mtx);

	for (size_t i = 0; i < nb; i++)
	{
		th[i] = CreateThread(NULL, 0, thread_func, NULL, 0, &id);
	}

	// wait all threads to terminate
	WaitForMultipleObjects(nb, th, TRUE, INFINITE);

	std::cout << " a = " << a << std::endl;

	for (size_t i = 0; i < nb; i++)
	{
		CloseHandle(th[i]);
	}

	DeleteCriticalSection(&mtx);

	std::cout << "Exit program" << std::endl;
	return EXIT_SUCCESS;
}
