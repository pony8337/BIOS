#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/PcdLib.h>
#include <Protocol/SimpleTextIn.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL       *Key;
  EFI_INPUT_KEY                        KeyData;
  EFI_STATUS                           Status;
  Print(L"Hello, This is Pony's Application Test\n");
  gBS->HandleProtocol(
      gST->ConsoleInHandle,
      &gEfiSimpleTextInProtocolGuid,
      (VOID**)&Key
      );
  
   
  while(1){    
    Status = Key->ReadKeyStroke(Key, &KeyData);
    if(Status == EFI_SUCCESS){
      break;
    }
  }
  return EFI_SUCCESS;
}
