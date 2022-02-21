#ifndef PONY_DRIVER_MODEL_H
#define PONY_DRIVER_MODEL_H

#include <Protocol/ComponentName.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/DriverBinding.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Protocol/installProtocolTest.h>
#include <Library/UefiBootServicesTableLib.h>

//
// Global Variables
//
extern EFI_DRIVER_BINDING_PROTOCOL   gPonyDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   gPonyComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  gPonyComponentName2;

EFI_STATUS
EFIAPI
PonyDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
);

EFI_STATUS
EFIAPI
PonyDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
);

EFI_STATUS
EFIAPI
PonyDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     ControllerHandle,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
);

EFI_STATUS
EFIAPI
PonyComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
);

EFI_STATUS
EFIAPI
PonyComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
);

#endif