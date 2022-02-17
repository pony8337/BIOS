#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/PcdLib.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  CHAR16 *test2;
  UINTN len;
  test2 = L"ChangeWorld";
  len = StrLen(test2);
  Print(L"Hello, This is Pony's Application Test\n");
  Print(L"Pony's Patchable Pcd Test\n");
  Print(L"PcdPatchableInModuleTest = %s\n", (CHAR16*)PatchPcdGetPtr(PcdPatchableInModuleTest));
  PatchPcdSetPtr(PcdPatchableInModuleTest, &len, test2);
  Print(L"Change PCD value\n");
  Print(L"PcdPatchableInModuleTest = %s\n", (CHAR16*)PatchPcdGetPtr(PcdPatchableInModuleTest));
  return EFI_SUCCESS;
}
