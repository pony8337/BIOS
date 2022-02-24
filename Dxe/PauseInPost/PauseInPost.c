#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleTextIn.h>

EFI_STATUS
EFIAPI
PauseInPostEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                      Status;
  UINTN                           Index;
  EFI_INPUT_KEY                   KeyData;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *Key;

  DEBUG ((EFI_D_INFO, "\nPauseInPostEntry...\n"));
  Status = gBS->LocateProtocol (
                  &gEfiSimpleTextInProtocolGuid, 
                  NULL, 
                  (VOID **)&Key
                );
  DEBUG ((EFI_D_INFO, "\ngEfiSimpleTextInProtocolGuid LocateProtocol %r\n", Status));
  gBS->WaitForEvent (1, &Key->WaitForKey, &Index);
  Status = Key->ReadKeyStroke (Key, &KeyData);
  DEBUG ((EFI_D_INFO, "ReadKeyStroke %r\n", Status));
  DEBUG ((EFI_D_INFO, "KeyData = %c\n", KeyData.UnicodeChar));
  return EFI_SUCCESS;
}
