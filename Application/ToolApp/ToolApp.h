#ifndef TOOL_APP_H
#define TOOL_APP_H

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>

extern EFI_SYSTEM_TABLE     *gST;
extern EFI_BOOT_SERVICES    *gBS;

// CMOS
#define CMOS_PORT       0x70
#define CMOS_VALUE      0x71

#define DELAY_TIME      10000000

#define SetColor(color) gST->ConOut->SetAttribute(gST->ConOut, color)
#define gotoXY(x, y)    gST->ConOut->SetCursorPosition(gST->ConOut, x, y)

#define offsetX(offset) 3 * (offset % 16) + 3
#define offsetY(offset) (offset / 16) + 3

// Index Label
#define rowX(x)         3 * x + 3
#define rowY(y)         2
#define columnX(x)      0
#define columnY(y)      y + 3

#define IsDigital(Data) (BOOLEAN)\
                        (Data >= 0x30 && Data <= 0x39) || \
                        (Data >= 0x41 && Data <= 0x5A) || \
                        (Data >= 0x61 && Data <= 0x7A)

typedef struct {
    UINT8   FunNo;
    VOID    (*FuncPtr)();
    CHAR16  *FunName;
} Tool_Menu;


// Common Function
/*
  @param  old  	Old offset
  @param  new  	New offset
  @param  data 	the data's pointer

  @return new offset
*/
UINTN MoveCursor(
  IN  UINTN  Old,
  IN  UINTN  New,
  IN  UINT8 *Data
);

/*
  @param  Data  	 data buffer
  @param  InputData  input value
  @param  offset 	 data's offset

  @return Data    	 the data after modified
*/
UINT8
ChangeInputData (
  IN  UINT8  Data,
  IN  CHAR16 InputData,
  IN  UINTN  offset
);

//
// CMOS 
//
VOID  CMOS();
UINT8 CMOSData[256] = {0};

VOID IOSpace();
VOID PCI();
VOID Memory();
VOID SMBus();
VOID EDID();
VOID SMBios();
VOID USBView();
VOID Variable();

// Tool Menu
Tool_Menu TMenu[] = {
  {0x00,     CMOS,        L"CMOS"},
  {0x01,  IOSpace,     L"IOSpace"},
  {0x02,      PCI,         L"PCI"},
  {0x03,   Memory,      L"Memory"},
  {0x04,    SMBus,       L"SMBus"},
  {0x05,     EDID,        L"EDID"},
  {0x06,   SMBios,      L"SMBios"},
  {0x07,  USBView,     L"USBView"},
  {0x08, Variable,    L"Variable"}
};

#endif