// BoosterClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <print>
#include "../../ThreadPriorityBooster/ThreadPriorityBooster/BoosterCommon.h"


int Error(char const* message)
{
	std::println(stderr, "{} (error={})", message, GetLastError());
	return 1;
}

int main(int argc, char const** argv)
{
	if (argc < 3) {
		std::println(stderr, "Usage: {} <ThreadId> <Priority>", argv[0]);
		return 0;
	}

	int tid = std::atoi(argv[1]);
	int priority = std::atoi(argv[2]);

	HANDLE hDevice = CreateFile(LR"(\\.\Booster)", GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hDevice == INVALID_HANDLE_VALUE)
		return Error("Failed to open device");

	ThreadData data;
	data.ThreadId = tid;
	data.Priority = priority;

	DWORD returned;
	BOOL success = WriteFile(hDevice, &data, sizeof data, &returned, nullptr);
	if (!success)
		return Error("Priority change failed");

	CloseHandle(hDevice);
}
