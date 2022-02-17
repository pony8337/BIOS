#include <Library/DebugLib.h>
#include <PiPei.h>
#include <Ppi/installPpiTest.h>

EFI_STATUS
EFIAPI
locatePpiTestEntry (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS status = EFI_SUCCESS;
  PONY_TEST_PPI *locateDateInfo;
  
  DEBUG ((DEBUG_INFO, "Pony locate Ppi Test Entry.\n"));
  status = (*PeiServices)->LocatePpi(
                                    PeiServices,
                                    &gPonyTestPpiGuid,
                                    0,
                                    NULL,
                                    (VOID**)&locateDateInfo
                                    );
  DEBUG ((DEBUG_INFO, "locate Ppi %r\n", status));
  if(status == EFI_SUCCESS){
    locateDateInfo->getPpiInformation(locateDateInfo);
  }
  return EFI_SUCCESS;
}
