#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/SmmServicesTableLib.h>  // gSmst
#include <Protocol/SmmSwDispatch2.h>       // EFI_SMM_SW_DISPATCH2_PROTOCOL
#include <Library/IoLib.h>

EFI_SMM_SW_REGISTER_CONTEXT     PonyRegisterContext;


EFI_STATUS  
PonySmiTestFunction(IN EFI_HANDLE  DispatchHandle,
                    IN CONST VOID  *Context         OPTIONAL,
                    IN OUT VOID    *CommBuffer      OPTIONAL,
                    IN OUT UINTN   *CommBufferSize  OPTIONAL)
{
  IoWrite8(0xB2, 0x11);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PonySmiTest (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                       Status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL   *PonySmiTest;
  EFI_HANDLE                       PonySmiHandle;
  DEBUG ((EFI_D_INFO, "[PONY] Register SMI...\n"));
  Status = gSmst->SmmLocateProtocol(
                      &gEfiSmmSwDispatch2ProtocolGuid,
                      NULL,
                      &PonySmiTest
                  );
  DEBUG ((EFI_D_INFO, "[PONY] Locat SMI Protocol %r\n", Status));
  PonyRegisterContext.SwSmiInputValue = 0xDD;

  if(!EFI_ERROR(Status)){
    Status = PonySmiTest->Register( PonySmiTest,
                                    PonySmiTestFunction, 
                                    &PonyRegisterContext, 
                                    &PonySmiHandle );
    }
  return EFI_SUCCESS;
}
