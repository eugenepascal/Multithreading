#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

static int good_a = 0;
CRITICAL_SECTION mtx;
CONDITION_VARIABLE cond;

DWORD WINAPI thread_func(LPVOID arg)
{
	int a = 0;

	std::cout << "Thread hello" << std::endl;

	for (int i = 0; i < 1000000; i++)
	{
		a++;

		if (a == 100000)
		{
			EnterCriticalSection(&mtx);
			good_a = 1;
			WakeAllConditionVariable(&cond);
			LeaveCriticalSection(&mtx);
		}
	}

	return 0;
}

int main(int argc, char** argv)
{
	HANDLE th;
	DWORD nb = 5;
	DWORD id = 0;

	InitializeCriticalSection(&mtx);
	InitializeConditionVariable(&cond);

	th = CreateThread(NULL, 0, thread_func, NULL, 0, &id);


	EnterCriticalSection(&mtx);
	while (good_a != 1)
	{
		SleepConditionVariableCS(&cond, &mtx, INFINITE);
	}
	LeaveCriticalSection(&mtx);

	std::cout << " good_a = " << good_a << std::endl;


	// wait all threads to terminate
	WaitForSingleObject(th, INFINITE);

	CloseHandle(th);
	DeleteCriticalSection(&mtx);

	std::cout << "Exit program" << std::endl;
	return EXIT_SUCCESS;
}
