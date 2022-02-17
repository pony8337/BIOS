#include <Library/DebugLib.h>
#include <Library/BaseLib.h>


EFI_STATUS
EFIAPI
HelloWorldPei (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  DEBUG ((EFI_D_INFO, "Hi, I'm Pony's PEIM!\n"));
  return EFI_SUCCESS;
}
