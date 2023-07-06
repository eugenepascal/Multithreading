#include <cstdlib>

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

int main(int argc, char** argv)
{
	PROCESS_INFORMATION processInfo;
	STARTUPINFOA si;
	bool ret = false;
	char* module = argv[0];
	char cmdLine[32767];

	if (argc > 1)
	{
		if (!strncmp(argv[1], "arg", strlen(argv[1])))
		{
			// child
			std::cout << "I am the children" << std::endl;
		}
	}
	else
	{
		// create the cmdLine to"fork" a process
		snprintf(cmdLine, sizeof(cmdLine), "%s arg", module);

		std::cout << cmdLine << std::endl;

		ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		ret = CreateProcessA(module, cmdLine, NULL, NULL, false, 0, NULL, NULL, &si, &processInfo);

		if (!ret)
		{
			std::cout << "Error CreateProcess" << std::endl;
			return EXIT_FAILURE;
		}

		std::cout << "Launched" << std::endl;

		Sleep(2000);

		std::cout << "Wait for children to terminate" << std::endl;
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	std::cout << "Program exit" << std::endl;
	return EXIT_SUCCESS;
}
