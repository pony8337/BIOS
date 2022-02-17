#include "MySetup.h"


MY_SETUP_CONFIGURATION  *mMySetupConfiguration = NULL;
CHAR16                  variable1[]  = L"PonySetupNVData";


EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                Status;
  MY_SETUP_CONFIGURATION   *PrivateData;
  EFI_STRING                ConfigRequest;
  UINTN                     BufferSize;

  DEBUG ((EFI_D_INFO, "\nMy Setup Menu ExtractConfig...\n"));

  if(Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = PONY_DRIVER_PRIVATE_FROM_THIS(This);

  BufferSize = sizeof(MY_VARSTORE_CONFIGURATION);
  Status = gRT->GetVariable ( 
                  variable1, 
                  &gPonySetupGuid, 
                  NULL, 
                  &BufferSize, 
                  &PrivateData->varstoreConfig
                );
  if(EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  DEBUG ((EFI_D_INFO, "\nRequest = %s\n", Request));
  ConfigRequest = Request;

  Status = PrivateData->HiiConfigRouting->BlockToConfig (
                              PrivateData->HiiConfigRouting,
                              ConfigRequest,
                              (UINT8 *) &PrivateData->varstoreConfig,
                              BufferSize,
                              Results,
                              Progress
                              );
  DEBUG ((EFI_D_INFO, "\nHiiConfigRouting->BlockToConfig Status %r\n", Status));
  
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  EFI_STATUS                Status;
  MY_SETUP_CONFIGURATION   *PrivateData;
  UINTN                     BufferSize;
  DEBUG ((EFI_D_INFO, "\nMy Setup Menu RouteConfig...\n"));
  PrivateData = PONY_DRIVER_PRIVATE_FROM_THIS(This);

  BufferSize = sizeof(MY_VARSTORE_CONFIGURATION);
  Status = gRT->GetVariable ( 
                  variable1, 
                  &gPonySetupGuid, 
                  NULL, 
                  &BufferSize, 
                  &PrivateData->varstoreConfig
                );
  if(EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }
  DEBUG ((EFI_D_INFO, "\nConfiguration = %s", Configuration));
  BufferSize = sizeof(MY_VARSTORE_CONFIGURATION);
  Status = PrivateData->HiiConfigRouting->ConfigToBlock (
                              PrivateData->HiiConfigRouting,
                              Configuration,
                              (UINT8 *) &PrivateData->varstoreConfig,
                              &BufferSize,
                              Progress
                              );
  DEBUG ((EFI_D_INFO, "HiiConfigRouting->ConfigToBlock %r\n", Status));
  if(EFI_ERROR (Status)) {
    return Status;
  }
  Status = gRT->SetVariable(
                  variable1,
                  &gPonySetupGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof(MY_VARSTORE_CONFIGURATION),
                  &mMySetupConfiguration->varstoreConfig
                );
  DEBUG ((EFI_D_INFO, "SetVariable %r\n", Status));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  DEBUG ((EFI_D_INFO, "\nMy Setup Menu DriverCallback\n"));

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MySetupInit (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                  Status;
  UINTN                       BufferSize;
  EFI_STRING                  CreateRequestHdr;
  BOOLEAN                     ActionFlag;
  MY_VARSTORE_CONFIGURATION  *varstoreConfig;
  
  DEBUG ((EFI_D_INFO, "\nMy setup initialize\n"));
  mMySetupConfiguration = AllocateZeroPool(sizeof(MY_SETUP_CONFIGURATION));
  if(mMySetupConfiguration == NULL){
    DEBUG ((EFI_D_INFO, "\nAllocate memory failed\n"));
    MySetupUnload(ImageHandle);
    return EFI_SUCCESS;
  }
  mMySetupConfiguration->Signature =  PONY_SETUP_PRIVATE_SIGNATURE;
  // Setting HiiConfigAccess Protocol
  mMySetupConfiguration->ConfigAccess.ExtractConfig = ExtractConfig;
  mMySetupConfiguration->ConfigAccess.RouteConfig = RouteConfig;
  mMySetupConfiguration->ConfigAccess.Callback = DriverCallback;
  mMySetupConfiguration->DriverHandleTest = NULL;
  varstoreConfig = &mMySetupConfiguration->varstoreConfig;

  ZeroMem(varstoreConfig, sizeof(MY_VARSTORE_CONFIGURATION));
  
  // Locate HiiConfigRoutingProtocol
  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **)&mMySetupConfiguration->HiiConfigRouting
                );

  if(Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO, "\nLocate HiiConfigRouting failed\n"));
    MySetupUnload(ImageHandle);
    return EFI_SUCCESS;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mMySetupConfiguration->DriverHandleTest,
                  &gEfiDevicePathProtocolGuid,
                  &PONY_SETUPMENU_DEVICEPATH,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mMySetupConfiguration->ConfigAccess,
                  NULL
                );

  ASSERT_EFI_ERROR (Status);

  mMySetupConfiguration->HiiDriverHandle = HiiAddPackages (
                                              &gPonySetupGuid,
                                              mMySetupConfiguration->DriverHandleTest,
                                              MySetupStrings,
                                              MySetupVfrBin,
                                              NULL
                                            );
  BufferSize = sizeof(MY_VARSTORE_CONFIGURATION);
  Status = gRT->GetVariable ( 
                  variable1, 
                  &gPonySetupGuid, 
                  NULL, 
                  &BufferSize, 
                  &mMySetupConfiguration->varstoreConfig
                );
  DEBUG ((EFI_D_INFO, "get variable %r\n", Status));

  if(Status != EFI_SUCCESS){
      Status = gRT->SetVariable (
                        variable1,
                        &gPonySetupGuid,
                        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof(MY_VARSTORE_CONFIGURATION),
                        varstoreConfig
                      );
      DEBUG ((EFI_D_INFO, "SetVariable %r\n", Status));
  }
  // Create Request Header 
  CreateRequestHdr = HiiConstructConfigHdr (
                              &gPonySetupGuid, 
                              variable1, 
                              mMySetupConfiguration->DriverHandleTest
                            );
  DEBUG ((EFI_D_INFO, "HiiConstructConfigHdr:  %s\n", CreateRequestHdr));                      
  ActionFlag = HiiSetToDefaults (CreateRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
  if(ActionFlag) {
    DEBUG ((EFI_D_INFO, "\nHiiSetToDefaults = TRUE\n"));  
  } else {
    DEBUG ((EFI_D_INFO, "HiiSetToDefaults = FALSE\n"));
    MySetupUnload (ImageHandle);
    return EFI_INVALID_PARAMETER;
  }
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
MySetupUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  DEBUG ((EFI_D_INFO, "My Setup Menu Unload Start\n"));

  if(mMySetupConfiguration == NULL){
    return EFI_SUCCESS;
  }

  if (mMySetupConfiguration->DriverHandleTest != NULL) {
      gBS->UninstallMultipleProtocolInterfaces (
              mMySetupConfiguration->DriverHandleTest,
              &gEfiHiiConfigAccessProtocolGuid,
              &mMySetupConfiguration->ConfigAccess,
              NULL
            );
      mMySetupConfiguration->DriverHandleTest = NULL;
  }

  if (mMySetupConfiguration->HiiDriverHandle != NULL) {
    HiiRemovePackages (mMySetupConfiguration->HiiDriverHandle);
  }
  FreePool(mMySetupConfiguration);
  mMySetupConfiguration = NULL;
  return EFI_SUCCESS;
}