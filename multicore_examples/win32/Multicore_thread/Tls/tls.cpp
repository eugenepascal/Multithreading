#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

DWORD key = 0;

struct thread_info
{
	char name[255];
	intptr_t val;
};

DWORD WINAPI thread_func (LPVOID arg)
{
	intptr_t val = reinterpret_cast<intptr_t>(arg);
	std::cout << "Thread created" << std::endl;

	struct thread_info* info = reinterpret_cast<struct thread_info*>(TlsGetValue(key));

	if(!info)
	{
		info = new struct thread_info;
		snprintf(info->name, sizeof(info->name), "Thread %Id", val);
		info->val = val;

		if(!TlsSetValue (key, info))
		{
			std::cout << "Failed to set value in TLS" << std::endl;
		}
	}

	Sleep(1000);

	info = reinterpret_cast<struct thread_info*>(TlsGetValue (key));
	if(info)
	{
		std::cout << info->name << " " << info->val << std::endl;
	}
	
	// don't forget to release memory before quit
	delete info;
	
	return 0;
}

int main (int argc, char** argv)
{
	HANDLE th[5];
	DWORD nb = 5;
	DWORD id = 0;

	key = TlsAlloc ();

	if(key == TLS_OUT_OF_INDEXES)
	{
		std::cout << "Failed to allocate TLS" << std::endl;
		exit(EXIT_FAILURE);
	}

	for(size_t i = 0; i < nb; i++)
	{
		th[i] = CreateThread (NULL, 0, thread_func, reinterpret_cast<void*>(i), 0, &id);
	}

	// wait all threads to terminate
	WaitForMultipleObjects (nb, th, TRUE, INFINITE);

	for(size_t i = 0; i < nb; i++)
	{
		CloseHandle (th[i]);
	}

	TlsFree(key);

	std::cout << "Exit program" << std::endl;
	return EXIT_SUCCESS;
}
