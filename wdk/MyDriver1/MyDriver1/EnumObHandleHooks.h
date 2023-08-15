#pragma once

#include <ntddk.h>


//遍历所有 进程  线程  注册的ObRegisterCallBacks回调
ULONG EnumObRegisterCallBacks();