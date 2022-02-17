#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/installProtocolTest.h>

EFI_STATUS EFIAPI getBodyInformation(
  IN    PONY_TEST_PROTOCOL  *This
);

PONY_TEST_PROTOCOL ProtocolTest = {
  "Pony Yang",
  28,
  165,
  90,
  getBodyInformation
};


EFI_STATUS EFIAPI getBodyInformation(
  IN    PONY_TEST_PROTOCOL  *This
)
{
  DEBUG ((EFI_D_INFO, "==== Pony's Profile ====\n"));
  DEBUG ((EFI_D_INFO, "name: %a\n", This->name));
  DEBUG ((EFI_D_INFO, "age: %d\n", This->age));
  DEBUG ((EFI_D_INFO, "height: %d\n", This->height));
  DEBUG ((EFI_D_INFO, "weight: %d\n", This->weight));
  DEBUG ((EFI_D_INFO, "========================\n"));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InstallProtocolTestEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS status;
  DEBUG ((EFI_D_INFO, "Pony's Protocol Install Protocol Test\n"));
  status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gPonyTestProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &ProtocolTest
                  );
  DEBUG ((EFI_D_INFO, "Install Pony's Protocol Status = %r\n", status));
  
  return EFI_SUCCESS;
}
