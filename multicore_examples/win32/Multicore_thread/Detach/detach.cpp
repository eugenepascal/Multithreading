#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

DWORD WINAPI thread_func(LPVOID arg)
{
	int th_arg = *((int*)arg);

	std::cout << "Thread hello" << std::endl;
	std::cout << "Argument " << th_arg << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	DWORD id = 0;
	int arg = 42;
	HANDLE th = CreateThread(NULL, 0, thread_func, &arg, 0, &id);

	// detach thread so no need to WaitForSingleObject
	CloseHandle(th);

	Sleep(5000);

	std::cout << "Exit program" << std::endl;
	return EXIT_SUCCESS;
}
