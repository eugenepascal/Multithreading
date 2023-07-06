#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

DWORD WINAPI thread_func(LPVOID arg)
{
	int th_arg = *(reinterpret_cast<int*>(arg));

	std::cout << "Thread hello" << std::endl;
	std::cout << "Argument " << th_arg << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	DWORD id = 0;
	int arg = 42;
	HANDLE th = CreateThread(NULL, 0, thread_func, reinterpret_cast<void*>(&arg), 0, &id);

	// wait thread to terminate
	WaitForSingleObject(th, INFINITE);
	CloseHandle(th);

	return EXIT_SUCCESS;
}
