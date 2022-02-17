#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/TestLib.h>
#include <Library/PcdLib.h>

EFI_STATUS
EFIAPI
HelloWorldDxe (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS Status;
  
  DEBUG ((EFI_D_INFO, "Hi, This is Pony's Dxe\n"));
  
  Status = PonyLibrary();
  
  DEBUG ((EFI_D_INFO, "RUN Pony's Library Status = %r\n", Status));
 
  DEBUG ((EFI_D_INFO, "Get Pony's Feature PCD\n"));

  if(FeaturePcdGet(PcdPonyFeatureFlagTest)) {
      DEBUG ((EFI_D_INFO, "PcdPonyFeatureFlagTest = TRUE\n"));
  } else {
      DEBUG ((EFI_D_INFO, "PcdPonyFeatureFlagTest = FALSE\n"));
  }

  DEBUG ((EFI_D_INFO, "Get Pony's Patchable PCD\n"));

  DEBUG ((EFI_D_INFO, "PcdPatchableInModuleTest = %s\n", PatchPcdGetPtr(PcdPatchableInModuleTest)));
  DEBUG ((EFI_D_INFO, "PcdPatchableValueTest = 0x%x\n", PatchPcdGet32(PcdPatchableValueTest)));

  return EFI_SUCCESS;
}
