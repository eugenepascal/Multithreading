#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

SYNCHRONIZATION_BARRIER barrier;

DWORD WINAPI thread_func(LPVOID arg)
{
	intptr_t idx = reinterpret_cast<intptr_t>(arg);
	int a = 0;

	std::cout << "Thread hello" << std::endl;

	for (int i = 0; i < 1000; i++)
	{
		a++;	
	}

	std::cout << "Thread " << idx << " waits" << std::endl;
	EnterSynchronizationBarrier(&barrier, SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY);

	return 0;
}

int main(int argc, char** argv)
{
	HANDLE th[10];
	DWORD nb = 10;
	DWORD id = 0;

	InitializeSynchronizationBarrier(&barrier, nb + 1, -1);

	for (size_t i = 0; i < nb; i++)
	{
		th[i] = CreateThread(NULL, 0, thread_func, (LPVOID)i, 0, &id);
	}

	std::cout << "Wait all threads" << std::endl;
	EnterSynchronizationBarrier(&barrier, SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY);
	std::cout << "All threads are initialized !" << std::endl;

	// wait all threads to terminate
	WaitForMultipleObjects(nb, th, TRUE, INFINITE);

	for (size_t i = 0; i < nb; i++)
	{
		CloseHandle(th[i]);
	}
	
	DeleteSynchronizationBarrier(&barrier);

	std::cout << "Exit program" << std::endl;
	return EXIT_SUCCESS;
}
