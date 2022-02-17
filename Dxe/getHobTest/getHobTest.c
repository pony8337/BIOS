#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Hob/createHobTest.h>

EFI_STATUS
EFIAPI
getHobTest (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
 
  CREATE_HOB_INFO  *getHob;
  DEBUG ((EFI_D_INFO, "Hi, This Pony's Hob Get Test!\n"));
  getHob = GetFirstGuidHob(&gcreateHobTestGuid);
  DEBUG ((EFI_D_INFO, "###### Pony Hob Information ######\n"));
  DEBUG ((EFI_D_INFO, "name: %a\n", getHob->name));
  DEBUG ((EFI_D_INFO, "age: %d\n", getHob->age));
  DEBUG ((EFI_D_INFO, "weight: %d\n", getHob->weight));
  DEBUG ((EFI_D_INFO, "height: %d\n", getHob->height));
  DEBUG ((EFI_D_INFO, "###### Pony Hob Information ######\n"));
  return EFI_SUCCESS;
}
