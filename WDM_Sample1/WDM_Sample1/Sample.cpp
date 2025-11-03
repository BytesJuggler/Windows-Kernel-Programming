#include <ntddk.h>


// Every action the driver performs must be undone when the driver is unloaded.
void SampleUnload(
	_In_ PDRIVER_OBJECT DriverObject
)
{
	UNREFERENCED_PARAMETER(DriverObject);

	KdPrint(("[rosxnb] SampleUnload called\n"));
}

extern "C" NTSTATUS
DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING
)
{
	DriverObject->DriverUnload = SampleUnload;

	KdPrint(("[rosxnb] DriverEntry called\n"));

	return STATUS_SUCCESS;
}