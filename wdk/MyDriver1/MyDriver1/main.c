#include <ntddk.h>    //��������ر�ͷ�ļ�

NTSTATUS UnloadDriver(PDRIVER_OBJECT DriverObject)
{
    DbgPrint("Chapter Driver By WingSummer,Unloaded Successfully!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    DbgPrint("Chapter Driver By WingSummer,Loaded Successfully!");
    DriverObject->DriverUnload = UnloadDriver;

    return STATUS_SUCCESS;
}
