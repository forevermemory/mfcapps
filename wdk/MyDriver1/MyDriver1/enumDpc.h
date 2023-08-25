#pragma once

BOOLEAN FindKiWaitNeverAndKiWaitAlways(UINT64* never, UINT64* always);


void EnumKernelDPC();

void InstallKernelDPC(PDEVICE_OBJECT pOurDevice);