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
  PONY_TEST_PROTOCOL  *PonyTest;
  EFI_STATUS          Status;

  DEBUG((DEBUG_INFO, "PonyDriverBindingSupported\n"));

  //[test] Attach gPonyTestProtocolGuid Driver ++

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gPonyTestProtocolGuid,
                  (VOID **) &PonyTest,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

   gBS->CloseProtocol (
         ControllerHandle,
         &gPonyTestProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );

  //[test] Attach gPonyTestProtocolGuid Driver --
  
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
  PONY_TEST_PROTOCOL  *PonyTest;
  EFI_STATUS          Status;

  DEBUG((DEBUG_INFO, "PonyDriverBindingStart\n"));

  //[test] Attach gPonyTestProtocolGuid Driver ++

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gPonyTestProtocolGuid,
                  (VOID **) &PonyTest,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //[test] Attach gPonyTestProtocolGuid Driver --
  
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
  EFI_STATUS          Status;

  DEBUG((DEBUG_INFO, "PonyDriverBindingStop\n"));

  //[test] Attach gPonyTestProtocolGuid Driver ++

  Status = gBS->CloseProtocol (
            ControllerHandle,
            &gPonyTestProtocolGuid,
            This->DriverBindingHandle,
            ControllerHandle
          );
  return Status;
  //[test] Attach gPonyTestProtocolGuid Driver --
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
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gPonyDriverBinding,
             ImageHandle,
             &gPonyComponentName,
             &gPonyComponentName2
             );
  ASSERT_EFI_ERROR (Status);
  
  return Status;
}
