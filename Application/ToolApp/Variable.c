#include "ToolApp.h"

VOID *mGetVariable (
  IN  CHAR16              *Name,
  IN  EFI_GUID            *VendorGuid,
  OUT UINTN               *VariableSize,
  OUT UINT32              *Attr
  )
{
  EFI_STATUS  Status;
  UINTN       BufferSize;
  VOID        *Buffer;

  Buffer = NULL;

  //
  // Pass in a zero size buffer to find the required buffer size.
  //
  BufferSize  = 0;
  Status      = gRT->GetVariable (Name, VendorGuid, Attr, &BufferSize, Buffer);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    //
    // Allocate the buffer to return
    //
    Buffer = AllocateZeroPool (BufferSize);
    if (Buffer == NULL) {
      *VariableSize = 0;
      return NULL;
    }
    //
    // Read variable into the allocated buffer.
    //
    Status = gRT->GetVariable (Name, VendorGuid, Attr, &BufferSize, Buffer);
    if (EFI_ERROR (Status)) {
      FreePool (Buffer);
      BufferSize = 0;
      Buffer     = NULL;
    }
  }

  *VariableSize = BufferSize;
  return Buffer;
}


UINTN ScanVariable() 
{
  UINTN           Num;
  UINTN           NameSize;
  UINTN           OldNameSize;
  UINT32          Attribute;
  CHAR16         *VariableName;
  EFI_GUID        VariableGuid;
  EFI_STATUS      Status;
  
  NameSize = 10;
  Num = 0;
  VariableName = (CHAR16*)AllocateZeroPool(NameSize);
  DEBUG ((EFI_D_INFO, "VariableName Allocation.\n"));
  do {
    OldNameSize = NameSize;
    Status = gRT->GetNextVariableName (
                      &NameSize,
                      VariableName,
                      &VariableGuid              
                    );
    if (Status == EFI_BUFFER_TOO_SMALL) {
      VariableName = (CHAR16*)ReallocatePool(OldNameSize, NameSize, VariableName);
      continue;
    }
    if (Status == EFI_NOT_FOUND) break;

    StrCpyS(VariableList[Num].VariableName, sizeof(VariableList[Num].VariableName), VariableName);
    DEBUG ((EFI_D_INFO, "VariableName = %s.\n", VariableList[Num].VariableName));
    VariableList[Num].VendorGuid = VariableGuid;
    FreePool(mGetVariable (VariableList[Num].VariableName, &VariableList[Num].VendorGuid,  &VariableList[Num].VariableSize, &Attribute));
    Attribute & EFI_VARIABLE_NON_VOLATILE ? StrCpyS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"NV ") : StrCpyS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"");
    Attribute & EFI_VARIABLE_BOOTSERVICE_ACCESS ? StrCatS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"BS ") : StrCatS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"");
    Attribute & EFI_VARIABLE_RUNTIME_ACCESS ? StrCatS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"RT ") : StrCatS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"");
    Attribute & EFI_VARIABLE_HARDWARE_ERROR_RECORD ? StrCatS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"HR ") : StrCatS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"");
    Attribute & EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS ? StrCatS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"AT ") : StrCatS(VariableList[Num].Attributes, sizeof(VariableList[Num].Attributes), L"");
    Num++;
  } while (Status != EFI_NOT_FOUND);

  FreePool(VariableName);
  VariableName = NULL;

  return Num; 
}

VOID ShowVariable (
  IN  VARIABLE_INFO VariableInfo,
  IN  UINTN         Index
)
{
  gotoXY(VARIABLE_LIST_NAME_X, VARIABLE_LIST_Y + Index);
  Print(L"%s", VariableInfo.VariableName);
  gotoXY(VARIABLE_LIST_SIZE_X, VARIABLE_LIST_Y + Index);
  Print(L"%04x", VariableInfo.VariableSize);
  gotoXY(VARIABLE_LIST_GUID_X, VARIABLE_LIST_Y + Index);
  Print(L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", 
                            VariableInfo.VendorGuid.Data1,
                            VariableInfo.VendorGuid.Data2,
                            VariableInfo.VendorGuid.Data3,
                            VariableInfo.VendorGuid.Data4[0],
                            VariableInfo.VendorGuid.Data4[1],
                            VariableInfo.VendorGuid.Data4[2],
                            VariableInfo.VendorGuid.Data4[3],
                            VariableInfo.VendorGuid.Data4[4],
                            VariableInfo.VendorGuid.Data4[5],
                            VariableInfo.VendorGuid.Data4[6],
                            VariableInfo.VendorGuid.Data4[7]);
  
}

VOID ShowVariableListTitle (
  IN  UINTN         CurrPage,
  IN  UINTN         TotalPage
) 
{
  SetColor(EFI_BACKGROUND_MAGENTA|EFI_LIGHTGRAY);
  gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
  Print(L"Variable Name                          Size GUID                         [%02d/%02d]", CurrPage, TotalPage);
  gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
  SetColor(EFI_LIGHTGRAY);
  BLOCK_BOUNDARY;
}

VOID Variable()
{
  EFI_INPUT_KEY   key;
  UINTN           Index;
  UINTN           EventIndex;
  UINTN           VariableNum = 0;
  UINTN           PageNum;
  UINTN           CurrVariable;
  UINTN           CurrPage;
  UINTN           PreVariable;
  BOOLEAN         ChangePageMode = FALSE;
  
  
  ZeroMem(VariableList, MAX_VARIABLE_NUM * sizeof(VARIABLE_INFO));
  gST->ConOut->ClearScreen(gST->ConOut);
  VariableNum = ScanVariable();

  CurrVariable = 0;
  // Variable List
  CurrPage = 1;
  PageNum = (VariableNum % 20) == 0 ? VariableNum / 20 : VariableNum / 20 + 1;
  ShowVariableListTitle(CurrPage, PageNum);
  for(Index = 0; Index < 20; Index++) {
    (CurrVariable % 20) == Index ? SetColor(SHOW_CHOOSE_DATA) : SetColor(SHOW_DATA_COLOR); 
    ShowVariable(VariableList[Index], Index);
  }

  do {
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
    switch(key.ScanCode) {
      case SCAN_DOWN:    
      case SCAN_RIGHT:
        PreVariable = CurrVariable;
        ChangePageMode = (CurrVariable % 20 == 19 || CurrVariable == VariableNum - 1) ? TRUE : FALSE;
        CurrVariable = CurrVariable == VariableNum - 1 ? 0 : CurrVariable + 1;
        if(ChangePageMode) {
          gST->ConOut->ClearScreen(gST->ConOut);
          CurrPage = CurrPage == PageNum ? 1 : CurrPage + 1;
          ShowVariableListTitle(CurrPage, PageNum);
          for(Index = 0; Index < 20 && Index + CurrVariable < VariableNum; Index++) {
            (CurrVariable % 20) == Index ? SetColor(SHOW_CHOOSE_DATA) : SetColor(SHOW_DATA_COLOR); 
            ShowVariable(VariableList[CurrVariable + Index], Index);
          }
        } else {
          SetColor(SHOW_DATA_COLOR);
          ShowVariable(VariableList[PreVariable], PreVariable % 20);
          SetColor(SHOW_CHOOSE_DATA);
          ShowVariable(VariableList[CurrVariable], CurrVariable % 20);
        }
        break;
      case SCAN_UP:
      case SCAN_LEFT:
        PreVariable = CurrVariable;
        ChangePageMode = (CurrVariable % 20 == 0) ? TRUE : FALSE;    
        CurrVariable = CurrVariable == 0 ? VariableNum - 1 : CurrVariable - 1;
        if(ChangePageMode) {
          gST->ConOut->ClearScreen(gST->ConOut);
          CurrPage = CurrPage == 1 ? PageNum : CurrPage - 1;
          ShowVariableListTitle(CurrPage, PageNum);
          for(Index = 0; Index < 20 && 20 * (CurrPage - 1) + Index < VariableNum; Index++) {
            (CurrVariable % 20) == Index ? SetColor(SHOW_CHOOSE_DATA) : SetColor(SHOW_DATA_COLOR); 
            ShowVariable(VariableList[20 * (CurrPage - 1) + Index], Index);
          }
        } else {
          SetColor(SHOW_DATA_COLOR);
          ShowVariable(VariableList[PreVariable], PreVariable % 20);
          SetColor(SHOW_CHOOSE_DATA);
          ShowVariable(VariableList[CurrVariable], CurrVariable % 20);
        }       
        break;
      case SCAN_NULL:
          // Enter Function
          if(key.UnicodeChar == 0x0D) { 
            VariableDetail(VariableList[CurrVariable]);
            gST->ConOut->ClearScreen(gST->ConOut);
            ShowVariableListTitle(CurrPage, PageNum);
            for(Index = 0; Index < 20 && 20 * (CurrPage - 1) + Index < VariableNum; Index++) {
              (CurrVariable % 20) == Index ? SetColor(SHOW_CHOOSE_DATA) : SetColor(SHOW_DATA_COLOR); 
              ShowVariable(VariableList[20 * (CurrPage - 1) + Index], Index);
            }
          }
        break;
    }
  } while(key.ScanCode != SCAN_ESC);
  
  return ;
}