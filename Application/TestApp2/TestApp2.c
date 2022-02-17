#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>


INTN
EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  if(Argc!=2){
    Print(L"use -h for help\n");
    return 0;
  }
  if(Argv[1][0] == '-'){
    if(Argv[1][1] == 'h' || Argv[1][1] == 'H'){
      Print(L"-h for help\n-p show information\n");
    } else if (Argv[1][1] == 'p' || Argv[1][1] == 'P'){
      Print(L"Hello, This is test app\n");
    } else {
      Print(L"use -h for help\n");
    }
  } else {
    Print(L"use -h for help\n");
  }
  return 0;
}
