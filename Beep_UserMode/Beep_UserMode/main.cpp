// Opening Devices Directly

#include <Windows.h>
#include <winternl.h>  // NtOpenFile
#include <ntddbeep.h>  // Beep-Specific Definitions

#include <print>


#pragma comment(lib, "ntdll")


int main(int argc, char const** argv)
{
	int freq = 800;
	int duration = 1000;

	if (argc == 3) {
		std::println("Usage: {} <frequency> <duration_in_ms>", argv[0]);
		freq = std::atoi(argv[1]);
		duration = std::atoi(argv[2]);
	}

	HANDLE hFile;
	OBJECT_ATTRIBUTES attr;
	UNICODE_STRING name;
	IO_STATUS_BLOCK ioStatus;

	RtlInitUnicodeString(&name, LR"(\Device\Beep)");
	InitializeObjectAttributes(&attr, &name, OBJ_CASE_INSENSITIVE, nullptr, nullptr);
	NTSTATUS status = ::NtOpenFile(&hFile, GENERIC_WRITE, &attr, &ioStatus, 0, 0);

	if (NT_SUCCESS(status)) {
		BEEP_SET_PARAMETERS params;
		params.Frequency = freq;
		params.Duration = duration;
		DWORD bytes;

		std::println("Playing freq: {}, duration: {}", freq, duration);
		::DeviceIoControl(hFile, IOCTL_BEEP_SET, &params, sizeof params, nullptr, 0, &bytes, nullptr);

		// the sound starts plaing and the call returns immediately
		// wait so that the app doesn't close
		::Sleep(duration);
		::CloseHandle(hFile);
	}

	return 0;
}
