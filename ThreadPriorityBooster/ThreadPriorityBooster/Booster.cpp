#include <ntifs.h>

#include "BoosterCommon.h"


static void
BoosterUnload(PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING symlink = RTL_CONSTANT_STRING(LR"(\??\Booster)");
	IoDeleteSymbolicLink(&symlink);

	IoDeleteDevice(DriverObject->DeviceObject);
}

static NTSTATUS
BoosterCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	auto retstat = STATUS_SUCCESS;
	Irp->IoStatus.Status = retstat;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return retstat;
}

static NTSTATUS
BoosterWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	auto status = STATUS_SUCCESS;
	ULONG_PTR info = 0; // track used bytes

	// irpSp is of type PIO_STACK_LOCATION
	auto irpSp = IoGetCurrentIrpStackLocation(Irp);
	do {
		if (irpSp->Parameters.Write.Length < sizeof(ThreadData)) {
			status = STATUS_BUFFER_TOO_SMALL;
			break;
		}

		auto data = static_cast<ThreadData*>(Irp->UserBuffer);
		if (data == nullptr || data->Priority < 1 || data->Priority > 31) {
			status = STATUS_INVALID_PARAMETER;
			break;
		}

		PETHREAD thread;
		status = PsLookupThreadByThreadId(ULongToHandle(data->ThreadId), &thread);
		if (!NT_SUCCESS(status))
			break;

		auto oldPriority = KeSetPriorityThread(thread, data->Priority);
		KdPrint(("[rosxnb] Priority change for thread %u from %d to %d succeeded!\n", data->ThreadId, oldPriority, data->Priority));

		ObDereferenceObject(thread);
		info = sizeof(data);
	} while (false);

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = info;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

extern "C" NTSTATUS
DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	// Called when driver unloads, use to cleanup resources open in driver
	DriverObject->DriverUnload = BoosterUnload;

	// Required for clients to be able to open handle to Device Object to communitcate to this driver
	DriverObject->MajorFunction[IRP_MJ_CREATE] = BoosterCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = BoosterCreateClose;

	// For client/driver communication
	DriverObject->MajorFunction[IRP_MJ_WRITE] = BoosterWrite;

	// Create Device Object
	UNICODE_STRING deviceName = RTL_CONSTANT_STRING(L"\\Device\\Booster");
	PDEVICE_OBJECT DeviceObject;
	NTSTATUS status = IoCreateDevice(
		DriverObject,			// the driver object
		0,						// we don't need extra bytes
		&deviceName,			// the device name
		FILE_DEVICE_UNKNOWN,	// device type
		0,						// characteristics flags
		FALSE,					// not exclusive
		&DeviceObject);			// the resulting pointer

	if (!NT_SUCCESS(status)) {
		KdPrint(("[rosxnb] Failed to create device object (0x%08X)\n", status));
		return status;
	}

	// Create a symlink for the client to access the Device Object
	UNICODE_STRING symlink = RTL_CONSTANT_STRING(LR"(\??\Booster)");
	status = IoCreateSymbolicLink(&symlink, &deviceName);
	if (!NT_SUCCESS(status)) {
		KdPrint(("[rosxnb] Failed to create symbolic link (0x%08X)\n", status));
		IoDeleteDevice(DeviceObject);
		return status;
	}

	return STATUS_SUCCESS;
}