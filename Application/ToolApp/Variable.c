#include "ToolApp.h"

UINTN ScanVariable() {
  UINTN           Num;
  UINTN           NameSize;
  UINTN           OldNameSize;
  CHAR16         *VariableName;
  EFI_GUID        VariableGuid;
  EFI_STATUS      Status;

  NameSize = 10;
  Num = 0;
  VariableName = (CHAR16*)AllocateZeroPool(NameSize);
  do {
    OldNameSize = NameSize;
    Status = gRT->GetNextVariableName (
                      &NameSize,
                      VariableName,
                      &VariableGuid              
                    );
    if(Status == EFI_BUFFER_TOO_SMALL) {
      VariableName = (CHAR16*)ReallocatePool(OldNameSize, NameSize, VariableName);
      continue;
    }
    Num++;
  } while (Status != EFI_NOT_FOUND);

  FreePool(VariableName);
  VariableName = NULL;

  return Num; 
}


VOID Variable()
{
  // EFI_STATUS      Status;
  EFI_INPUT_KEY   key;
  UINTN           EventIndex;
  UINTN           variableNum;

  gST->ConOut->ClearScreen(gST->ConOut);
  SetColor(EFI_LIGHTGRAY);
  gotoXY(0, 0);
  variableNum = ScanVariable();
  Print(L"variable ");
  SetColor(EFI_RED);
  Print(L"[ Numer: %d ]", variableNum);
  SetColor(EFI_LIGHTGRAY);
  do {
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
  } while(key.ScanCode != SCAN_ESC);
  
  return ;
}