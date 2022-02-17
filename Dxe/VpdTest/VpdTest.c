#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>

EFI_STATUS
EFIAPI
VpdTestEntrty (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  
  DEBUG ((EFI_D_INFO, "[PONY] VPD Test...\n"));
  
  DEBUG ((EFI_D_INFO, "PcdGet32 = %x\n", PcdGet32(PcdVpdTest)));
  DEBUG ((EFI_D_INFO, "PcdVpdBaseAddress = %x\n", PcdGet64(PcdVpdBaseAddress64)));

  return EFI_SUCCESS;
}
