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


UINTN ScanVariable(
  IN OUT VARIABLE_INFO *VariableList
) 
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


VOID Variable()
{
  // EFI_STATUS      Status;
  EFI_INPUT_KEY   key;
  UINTN           Index;
  UINTN           offset;
  UINTN           EventIndex;
  UINTN           VariableNum;
  UINTN           PageNum;
  UINTN           CurrVariable;
  UINTN           CurrPage;
  VARIABLE_INFO   VariableList[MAX_VARIABLE_NUM];
  
  ZeroMem(VariableList, MAX_VARIABLE_NUM*sizeof(VARIABLE_INFO));
  CurrVariable = 0;
  gST->ConOut->ClearScreen(gST->ConOut);
  VariableNum = ScanVariable(VariableList);
  PageNum = VariableList[CurrVariable].VariableSize / 256;
  CurrPage = 0;

  // Block A
  SetColor(EFI_LIGHTGRAY);
  gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
  CleanVariableName;
  gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
  Print(L"# Variable # - %s", VariableList[CurrVariable].VariableName);
  gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
  Print(L"[ GUID: %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x ]", 
                  VariableList[CurrVariable].VendorGuid.Data1,
                  VariableList[CurrVariable].VendorGuid.Data2,
                  VariableList[CurrVariable].VendorGuid.Data3,
                  VariableList[CurrVariable].VendorGuid.Data4[0],
                  VariableList[CurrVariable].VendorGuid.Data4[1],
                  VariableList[CurrVariable].VendorGuid.Data4[2],
                  VariableList[CurrVariable].VendorGuid.Data4[3],
                  VariableList[CurrVariable].VendorGuid.Data4[4],
                  VariableList[CurrVariable].VendorGuid.Data4[5],
                  VariableList[CurrVariable].VendorGuid.Data4[6],
                  VariableList[CurrVariable].VendorGuid.Data4[7]);
  gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
  Block_Boundary;
  
  // Block B
  SetColor(EFI_LIGHTGRAY);
  gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
  Print(L"Page:%d/%d", CurrPage + 1, PageNum + 1);
  gotoXY(BlockB_Information_X, BlockB_Information_Y);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGREEN);
  Print(L"[ F2: Variable List ]");

  // Block C
  SetColor(EFI_BROWN);
  for(Index = 0; Index <= 0xF; Index++){
    gotoXY(BlockC_RowX(Index), BlockC_RowY(Index));
    Print(L"%02x", Index);
    gotoXY(BlockC_ColumnX(Index), BlockC_ColumnY(Index));
    Print(L"%01x0", Index);
  }
  
  // for(Index = 0; Index <= 0xFF; Index++) {
  //   Index == offset ? SetColor(EFI_WHITE) : SetColor(EFI_LIGHTGRAY);
  //   gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
  //   Print(L"%02x ", CMOSData[Index]);
  // }
  do {
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
  } while(key.ScanCode != SCAN_ESC);
  
  return ;
}