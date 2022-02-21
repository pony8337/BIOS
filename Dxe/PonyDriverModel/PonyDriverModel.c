#include "PonyDriverModel.h"

// Driver binding protocol implementation for DiskIo driver.
//
EFI_DRIVER_BINDING_PROTOCOL gPonyDriverBinding = {
  PonyDriverBindingSupported,
  PonyDriverBindingStart,
  PonyDriverBindingStop,
  0x1,
  NULL,
  NULL
};

EFI_STATUS
EFIAPI
PonyDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  DEBUG((DEBUG_INFO, "PonyDriverBindingSupported\n"));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PonyDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  DEBUG((DEBUG_INFO, "PonyDriverBindingStart\n"));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PonyDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     ControllerHandle,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  )
{
  DEBUG((DEBUG_INFO, "PonyDriverBindingStop\n"));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitializeDriverModel (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS              Status;

  //
  // Install driver model protocol(s).
  //
  DEBUG((DEBUG_INFO, "PonyDriverModel initialize start .......\n"));

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gPonyDriverBinding,
             ImageHandle,
             &gPonyComponentName,
             &gPonyComponentName2
             );
  ASSERT_EFI_ERROR (Status);
  
  DEBUG((DEBUG_INFO, "PonyDriverModel initialize finish .......\n"));

  return Status;
}
