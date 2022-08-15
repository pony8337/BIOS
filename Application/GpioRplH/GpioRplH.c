#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include "GpioRplH.h"

INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  // CHAR8 *Address;
  UINTN  GpioNum;
  Print(L"[Community 0]\n");
  // GPP_B
  for(GpioNum = 0; GpioNum < GPP_B_NUM; GpioNum++) {
    Print(L"GPP_B%d: 0x%x\n", GpioNum, SBREG_BAR|(COMMUNITY0<<16)|(GPP_B_BASE+(GpioNum<<4)));
  }
  
  return 0;
}
