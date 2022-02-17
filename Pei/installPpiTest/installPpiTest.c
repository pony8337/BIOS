#include <Library/DebugLib.h>
#include <PiPei.h>
#include <Ppi/installPpiTest.h>

EFI_STATUS
EFIAPI
installPpiFunctionTest(
  IN   PONY_TEST_PPI     *This
)
{
  DEBUG ((DEBUG_INFO, "===== Profile =====\n"));
  DEBUG ((DEBUG_INFO, "name= %a\n", This->name));
  DEBUG ((DEBUG_INFO, "age= %d\n", This->age));
  DEBUG ((DEBUG_INFO, "height= %d\n", This->height));
  DEBUG ((DEBUG_INFO, "weight= %d\n", This->weight));
  DEBUG ((DEBUG_INFO, "===================\n"));
  return EFI_SUCCESS;
}

PONY_TEST_PPI PpiTestData = 
{
  "Pony Yang (for PPI Test)",
  28,
  90,
  165,
  installPpiFunctionTest
};

EFI_PEI_PPI_DESCRIPTOR PpiDescriptorTest = 
{
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPonyTestPpiGuid,
  &PpiTestData
};

EFI_STATUS
EFIAPI
installPpiTestEntry (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS status = EFI_SUCCESS;
  
  DEBUG ((DEBUG_INFO, "Pony install Ppi Test Entry.\n"));
  status = (*PeiServices)->InstallPpi(PeiServices, &PpiDescriptorTest);
  DEBUG ((DEBUG_INFO, "install Ppi %r\n", status));
  return EFI_SUCCESS;
}
