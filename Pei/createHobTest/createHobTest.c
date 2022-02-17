#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Hob/createHobTest.h>
#include <Pi/PiHob.h>



EFI_STATUS
EFIAPI
createHobTest (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  DEBUG ((EFI_D_INFO, "Hi, This Pony's Hob Create Test!\n"));
  CREATE_HOB_INFO   *ponyHobInfo = NULL;

  (*PeiServices)->CreateHob(
                   PeiServices,
                   EFI_HOB_TYPE_GUID_EXTENSION, 
                   sizeof(CREATE_HOB_INFO),
                   &ponyHobInfo
                   );
  
  ponyHobInfo->hobInfo.Name = gcreateHobTestGuid;
  ponyHobInfo->name = "Pony Yang";
  ponyHobInfo->weight = 90;
  ponyHobInfo->height = 165;
  ponyHobInfo->age = 28;
  
  return EFI_SUCCESS;
}
