#ifndef _SMBIOS_H_
#define _SMBIOS_H_
#include <IndustryStandard/SmBios.h>
#include <Library/MemoryAllocationLib.h>
#include "Tool.h"

EFI_GUID SMBIOS_GUID = { 0xeb9d2d31, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d }};

UINT8   SMBiosData[256] = {0};
UINTN   SMBiosSize = 0;
UINT32  HeadAddr   = 0;
UINT16  HandleNum  = 0;   


#endif