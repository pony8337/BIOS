#ifndef _TOOL_H_
#define _TOOL_H_
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SimpleTextIn.h>
#include <Library/MemoryAllocationLib.h>

#define  OffsetX(x)  3*(x%16)+3
#define  OffsetY(y)  (y/16)+3

#define  RowX(x)     3*x+3
#define  RowY(y)     2

#define  ColumnX(x)  0
#define  ColumnY(y)  y+3
#define IsASCIIDigit(Data) (BOOLEAN)(Data >= 0x30 && Data <= 0x39) || (Data >= 0x41 && Data <= 0x5A) || (Data >= 0x61 && Data <= 0x7A)

extern EFI_SYSTEM_TABLE              *gST;
extern EFI_BOOT_SERVICES             *gBS;

EFI_SIMPLE_TEXT_INPUT_PROTOCOL       *Key;
EFI_INPUT_KEY                        KeyData;



#endif