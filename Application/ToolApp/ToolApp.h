#ifndef TOOL_APP_H
#define TOOL_APP_H
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define SetColor(color)   gST->ConOut->SetAttribute(gST->ConOut, color)
#define GotoXY(x, y)      gST->ConOut->SetCursorPosition(gST->ConOut, x, y) 


typedef struct {
    UINT8   FunNo;
    VOID    (*FuncPtr)();
    CHAR16  *FunName;
} Tool_Menu;

// CMOS 
VOID CMOS();
VOID IOSpace();
VOID PCI();
VOID Memory();
VOID SMBus();
VOID EDID();
VOID SMBios();
VOID USBView();
VOID Variable();

#endif