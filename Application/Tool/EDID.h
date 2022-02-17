#ifndef _EDID_H_
#define _EDID_H_

#include "Tool.h"
#include  <Protocol/EdidDiscovered.h>

extern EFI_HANDLE       gImageHandle;
UINT8  EDIDData[256] = {0};
UINTN  EDIDNUM = 128;

#endif