
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/TestLib.h>


EFI_STATUS
PonyLibrary ()
{
  DEBUG ((EFI_D_INFO, "Hi, This is Pony's Library\n"));
  return EFI_SUCCESS;
}
