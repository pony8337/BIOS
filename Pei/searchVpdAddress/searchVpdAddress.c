#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <PiPei.h>
#include <Library/PcdLib.h>

/**
  Get Vpd binary address

  Parse through each FV for VPD FFS file and return the address

  @retval Address on VPD FFS detection else returns 0

**/
UINTN
EFIAPI
GetVpdFfsAddress (
  )
{
  EFI_STATUS            Status;
  VOID                  *Address;
  UINTN                  Instance;
  EFI_PEI_FV_HANDLE      VolumeHandle;
  EFI_PEI_FILE_HANDLE    FileHandle;

  Address = NULL;

  VolumeHandle = NULL;
  Instance = 0;
  while (TRUE) {
    //
    // Traverse all firmware volume instances.
    //
    Status = PeiServicesFfsFindNextVolume (Instance, &VolumeHandle);
    if (EFI_ERROR (Status)) {
      break;
    }

    FileHandle = NULL;
    Status = PeiServicesFfsFindFileByName (&gVpdFfsTestGuid, VolumeHandle, &FileHandle);
    if (!EFI_ERROR (Status)) {
      //
      // Search RAW section.
      //
      Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, FileHandle, &Address);
      if (!EFI_ERROR (Status)) {
        return (UINTN)Address;
      }
    }

    //
    // Search the next volume.
    //
    Instance++;
  }

  DEBUG ((EFI_D_ERROR, " PEI get VPD address: %r\n", EFI_NOT_FOUND));
  return 0;
}


EFI_STATUS
EFIAPI
searchVpdAddressEntry (
  IN       EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  UINTN VpdAddress;
  DEBUG ((DEBUG_INFO, "Pony Get Vpd Address Test ...\n"));
  VpdAddress = GetVpdFfsAddress();
  PcdSet64S(PcdVpdBaseAddress64, VpdAddress);
  DEBUG ((DEBUG_INFO, "Address = 0x%x\n", VpdAddress));
  
  return EFI_SUCCESS;
}
