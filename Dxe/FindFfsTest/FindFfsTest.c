#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Pi/Pihob.h>
#include <Library/HobLib.h>

EFI_STATUS
EFIAPI
FindFfsTest (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  // EFI_PEI_HOB_POINTERS FindHobInfo;

  // DEBUG ((DEBUG_INFO, "Pony Get Vpd Ffs from Hob Test ...\n"));
  // // EFI_HOB_FIRMWARE_VOLUME();
  
  // if(FindHobInfo.Raw != NULL){

  //   DEBUG ((DEBUG_INFO, "Address = 0x%x\n", FindHobInfo.Raw));

  // } else {

  //   DEBUG ((DEBUG_INFO, "FV Not found in Hob\n"));

  // }

  return EFI_SUCCESS;

}
