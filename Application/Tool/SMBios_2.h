#ifndef _SMBIOS_H_
#define _SMBIOS_H_
#include <IndustryStandard/SmBios.h>
#include <Library/MemoryAllocationLib.h>
#include "Tool.h"
#define   MAXOFSMBIOS  0xFFFF

EFI_GUID SMBIOS_GUID = { 0xeb9d2d31, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d }};

UINT8   SMBiosData[256] = {0};
UINT16  Current    = 0;
UINT16  SMBiosNum  = 0;

typedef struct 
{
  UINT8  SMBType;
  UINT32 SMBAddress;
  UINT16 SMBLen;
  UINT16 SMBHandle;
}SMBIOS_INFO;  
SMBIOS_INFO  SMBbiosInfo[MAXOFSMBIOS];

#endif