#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

static INIT_ONCE once = INIT_ONCE_STATIC_INIT;

BOOL CALLBACK init_once(INIT_ONCE* ponce, void* param, void** lpContext)
{
	std::cout << "Once initialization" << std::endl;
	return TRUE;
}

DWORD WINAPI thread_func (LPVOID arg)
{
	void* lpContext = NULL;

	std::cout << "Thread created" << std::endl;
	if(!InitOnceExecuteOnce(&once, init_once, NULL, &lpContext))
	{
		std::cout << "Once failed" << std::endl;
	}
	return 0;
}

int main (int argc, char** argv)
{
	HANDLE th[5];
	DWORD nb = 5;
	DWORD id = 0;

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

	std::cout << "Exit program" << std::endl;
	return EXIT_SUCCESS;
}
