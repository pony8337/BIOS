#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/installProtocolTest.h>

EFI_STATUS
EFIAPI
LocateProtocolTestEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS status;
  PONY_TEST_PROTOCOL *ponyInformation;
  DEBUG ((EFI_D_INFO, "Pony's Protocol Locate Protocol Test\n"));
  status = gBS->LocateProtocol(
                  &gPonyTestProtocolGuid, 
                  NULL, 
                  (VOID **)&ponyInformation
                  );
  DEBUG ((EFI_D_INFO, "Pony's Protocol Locate Protocol Test status = %r\n", status));
  if(status == EFI_SUCCESS){
    DEBUG ((EFI_D_INFO, "Pony's Protocol Test Information\n"));
    ponyInformation->getInformation(ponyInformation);
  }
  return EFI_SUCCESS;
}
