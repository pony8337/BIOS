#ifndef MY_SETUP_H
#define MY_SETUP_H

#include "MySetupGuid.h"

#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>

#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DevicePathLib.h>

extern UINT8  MySetupVfrBin[];
extern UINT8  MySetupStrings[];

#pragma pack(1)
///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

HII_VENDOR_DEVICE_PATH  PONY_SETUPMENU_DEVICEPATH = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    PONY_SETUP_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};


typedef struct {
  UINTN                             Signature;
  EFI_HII_HANDLE                    HiiDriverHandle;  // For HiiAddPackages
  EFI_HANDLE                        DriverHandleTest; // For install protocol
  MY_VARSTORE_CONFIGURATION         varstoreConfig;   // For setup configuration
  EFI_HII_CONFIG_ACCESS_PROTOCOL    ConfigAccess;     // For setup config access
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting; 
} MY_SETUP_CONFIGURATION;

#define PONY_SETUP_PRIVATE_SIGNATURE SIGNATURE_32 ('P', 'O', 'N', 'Y')
#define PONY_DRIVER_PRIVATE_FROM_THIS(a) CR(a, MY_SETUP_CONFIGURATION, ConfigAccess, PONY_SETUP_PRIVATE_SIGNATURE)
#endif