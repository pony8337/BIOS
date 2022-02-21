#include "PonyDriverModel.h"

//
// EFI Component Name Protocol
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME_PROTOCOL  gPonyComponentName = {
  PonyComponentNameGetDriverName,
  PonyComponentNameGetControllerName,
  "eng"
};

//
// EFI Component Name 2 Protocol
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME2_PROTOCOL gPonyComponentName2 = {
  (EFI_COMPONENT_NAME2_GET_DRIVER_NAME) PonyComponentNameGetDriverName,
  (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) PonyComponentNameGetControllerName,
  "en"
};

//
// Driver name table.
// It is shared by the implementation of ComponentName & ComponentName2 Protocol.
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_UNICODE_STRING_TABLE mPonyDriverNameTable[] = {
  {
    "eng;en",
    (CHAR16 *)L"Pony Driver Binding Test"
  },
  {
    NULL,
    NULL
  }
};

EFI_STATUS
EFIAPI
PonyComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  )
{
  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mPonyDriverNameTable,
           DriverName,
           (BOOLEAN)(This == &gPonyComponentName)
           );
}


EFI_STATUS
EFIAPI
PonyComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
  )
{
  return EFI_UNSUPPORTED;
}
