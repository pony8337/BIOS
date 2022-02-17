#include "Variable.h"

VOID *
EFIAPI
mGetVariable(
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


VOID 
ScanVariable()
{
  UINTN       VarSize;
  UINTN       TmpSize;
  UINTN       DataSize;
  UINT32      Attr;
  CHAR16      *VarName;
  EFI_GUID    VendGuid;
  EFI_STATUS  Status;

  VariableNum = 0;
  VarSize = 100;
  VarName = (CHAR16*)AllocateZeroPool(VarSize);
  while(1){   
    //save old variable size
    TmpSize = VarSize;   
    //VarName = AllocateZeroPool(VarSize);
    Status = gRT->GetNextVariableName(&VarSize, VarName, &VendGuid);
    
    //End of Scan Variable
    if(Status == EFI_NOT_FOUND){
      FreePool(VarName);
      VarName = NULL;
      break;
    }
  
    if(Status == EFI_BUFFER_TOO_SMALL){
      // Print(L"Too Small~\n");
      VarName = (CHAR16*)ReallocatePool (TmpSize, VarSize, VarName);
      continue;
    }


    //Error
    if(EFI_ERROR(Status)){
      Print(L"Error Status = %r!!\n", Status);
      break;
    }
    //Variable List Data
    // gRT->GetVariable(VarName, &VendGuid, &Attr, &DataSize, NULL);
    FreePool(mGetVariable(VarName, &VendGuid, &DataSize, &Attr));
    //Name
    VariableList[VariableNum].VariableName[0] = '\0';
    StrCat(VariableList[VariableNum].VariableName, VarName);
    //size
    VariableList[VariableNum].VariableSize = DataSize;
    //Attribute
    VariableList[VariableNum].Attributes[0] = '\0';
    if(Attr & EFI_VARIABLE_NON_VOLATILE){
      StrCat(VariableList[VariableNum].Attributes, L"NV ");
    }
    if(Attr & EFI_VARIABLE_BOOTSERVICE_ACCESS){
      StrCat(VariableList[VariableNum].Attributes, L"BS ");
    }
    if(Attr & EFI_VARIABLE_RUNTIME_ACCESS){
      StrCat(VariableList[VariableNum].Attributes, L"RT ");
    }
    if(Attr & EFI_VARIABLE_HARDWARE_ERROR_RECORD){
      StrCat(VariableList[VariableNum].Attributes, L"HR ");
    }
    if(Attr & EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS){
      StrCat(VariableList[VariableNum].Attributes, L"AT ");
    }
    if(Attr & EFI_VARIABLE_APPEND_WRITE){
      StrCat(VariableList[VariableNum].Attributes, L"AW ");
    }  
    //GUID
    VariableList[VariableNum].VendorGuid = VendGuid;
    VariableNum++;
  }
  return ;
}


VOID
UpdateVariableData(
  UINTN  DataSize,
  UINT8  *VarData
  )
{
  UINTN  TMP;
  UINTN  x;
  TMP = CurPage*256;
  for(x = 0; x<256; x++)
    VariableData[x] = TMP+x < DataSize ? VarData[TMP+x] : 0xFF;
}

VOID
VarDataInit()
{
  UINT32  Attributes;
  UINTN   DataSize;
  UINT8   *VarData;
  UINTN   x;
  
  CurPage = 0;
  VarData = NULL;
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);

  DataSize = 0;  
  VarData = (UINT8*)mGetVariable(VariableList[CurVariable].VariableName, &VariableList[CurVariable].VendorGuid, &DataSize, &Attributes);
  Page = DataSize/256;
  Print(L"                                                                                ");
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY); 
  Print(L"# Variable # - %s ",  VariableList[CurVariable].VariableName);      
  gST->ConOut->SetCursorPosition(gST->ConOut, 72, 0);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  Print(L"Page:%d/%d", CurPage+1, Page+1);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY); 
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
  Print(L"                                                                                ");
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
  Print(L"[ GUID: %08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x ]", VariableList[CurVariable].VendorGuid.Data1,
                                                                       VariableList[CurVariable].VendorGuid.Data2,
                                                                       VariableList[CurVariable].VendorGuid.Data3,
                                                                       VariableList[CurVariable].VendorGuid.Data4[0],
                                                                       VariableList[CurVariable].VendorGuid.Data4[1],
                                                                       VariableList[CurVariable].VendorGuid.Data4[2],
                                                                       VariableList[CurVariable].VendorGuid.Data4[3],
                                                                       VariableList[CurVariable].VendorGuid.Data4[4],
                                                                       VariableList[CurVariable].VendorGuid.Data4[5],
                                                                       VariableList[CurVariable].VendorGuid.Data4[6],
                                                                       VariableList[CurVariable].VendorGuid.Data4[7]
                                                                       );  
  gST->ConOut->SetCursorPosition(gST->ConOut, 59, 1);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGREEN);
  Print(L"[ F2: Variable List ]");  
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2);
   gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  Print(L"********************************************************************************");
  //Table offset
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, RowX(x), RowY(x)+1);
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, ColumnX(x), ColumnY(x)+1);
    Print(L"%01x0", x);
  }
  UpdateVariableData(DataSize, VarData);
  for(x = 0; x <= 0xFF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x)+1);
    if(x == 0){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
    }else if(VariableData[x] == 0xFF){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_DARKGRAY);
    }else{
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    }
    Print(L"%02x", VariableData[x]);
  }
  //Detail table
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 3);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  Print(L"[  Variable   Information  ]");
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 4);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  Print(L"Data Size: ");  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  Print(L"%03x", VariableList[CurVariable].VariableSize);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 5); 
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  Print(L"Attr: ");  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  Print(L"%s", VariableList[CurVariable].Attributes);
}

//
//  Item  => Current Item
//  Next  => Next Item
//
VOID 
ChangeVariableListItem(  
  UINTN  *Item,
  UINTN   Next
  )
{
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, *Item%16+1);
  Print(L"%s", VariableList[*Item].VariableName);
  gST->ConOut->SetCursorPosition(gST->ConOut, 36, *Item%16+1);
  Print(L"%04x", VariableList[*Item].VariableSize);
  gST->ConOut->SetCursorPosition(gST->ConOut, 44, *Item%16+1);    
  Print(L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", VariableList[*Item].VendorGuid.Data1,
                                                             VariableList[*Item].VendorGuid.Data2,
                                                             VariableList[*Item].VendorGuid.Data3,
                                                             VariableList[*Item].VendorGuid.Data4[0],
                                                             VariableList[*Item].VendorGuid.Data4[1],
                                                             VariableList[*Item].VendorGuid.Data4[2],
                                                             VariableList[*Item].VendorGuid.Data4[3],
                                                             VariableList[*Item].VendorGuid.Data4[4],
                                                             VariableList[*Item].VendorGuid.Data4[5],
                                                             VariableList[*Item].VendorGuid.Data4[6],
                                                             VariableList[*Item].VendorGuid.Data4[7]
                                                             );
  *Item = Next;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, *Item%16+1);
  Print(L"%s", VariableList[*Item].VariableName);
  gST->ConOut->SetCursorPosition(gST->ConOut, 36, *Item%16+1);
  Print(L"%04x", VariableList[*Item].VariableSize);
  gST->ConOut->SetCursorPosition(gST->ConOut, 44, *Item%16+1);    
  Print(L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", VariableList[*Item].VendorGuid.Data1,
                                                             VariableList[*Item].VendorGuid.Data2,
                                                             VariableList[*Item].VendorGuid.Data3,
                                                             VariableList[*Item].VendorGuid.Data4[0],
                                                             VariableList[*Item].VendorGuid.Data4[1],
                                                             VariableList[*Item].VendorGuid.Data4[2],
                                                             VariableList[*Item].VendorGuid.Data4[3],
                                                             VariableList[*Item].VendorGuid.Data4[4],
                                                             VariableList[*Item].VendorGuid.Data4[5],
                                                             VariableList[*Item].VendorGuid.Data4[6],
                                                             VariableList[*Item].VendorGuid.Data4[7]
                                                             );
}

//
//  Page  => Current Page
//  Item  => Current Item
//  Next  => Next Item
//
VOID 
ChangeVariableListPage(
  UINTN  *Page, 
  UINTN  *Item,
  UINTN   Next
  )
{
  UINTN  IRun;
  UINTN  TMP;

  *Item = Next;
  *Page = Next/16;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
  Print(L"Variable Name                       Size    GUID                         [%02d/%02d]", *Page+1, VariableNum/16+1);
  for(IRun = 0; IRun < 16; IRun++){
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, IRun+1);   
    TMP = IRun + 16 * *Page;       
    if(TMP < VariableNum){
      Print(L"                                                                                ");     
      TMP == *Item ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY); 
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, IRun+1);         
      Print(L"%s", VariableList[TMP].VariableName);
      gST->ConOut->SetCursorPosition(gST->ConOut, 36, IRun+1);
      Print(L"%04x", VariableList[TMP].VariableSize);
      gST->ConOut->SetCursorPosition(gST->ConOut, 44, IRun+1);    
      Print(L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", VariableList[TMP].VendorGuid.Data1,
                                                                 VariableList[TMP].VendorGuid.Data2,
                                                                 VariableList[TMP].VendorGuid.Data3,
                                                                 VariableList[TMP].VendorGuid.Data4[0],
                                                                 VariableList[TMP].VendorGuid.Data4[1],
                                                                 VariableList[TMP].VendorGuid.Data4[2],
                                                                 VariableList[TMP].VendorGuid.Data4[3],
                                                                 VariableList[TMP].VendorGuid.Data4[4],
                                                                 VariableList[TMP].VendorGuid.Data4[5],
                                                                 VariableList[TMP].VendorGuid.Data4[6],
                                                                 VariableList[TMP].VendorGuid.Data4[7]
                                                                 );
    }else{
      Print(L"                                                                                "); //clean
    }
  }
}

VOID
VariableListMenu()
{
  UINTN  ListPage = 0;
  UINTN  CurListItem = 0;
  UINTN  TMP;
  UINTN  IRun;
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
//Print(L"Bus  Dev  Fun  Vendor  Device  Name                                      [%02d/%02d]", Page+1, PciDevNum/16+1);   
//Print(L"                                                                                ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  Print(L"Variable Name                       Size    GUID                         [%02d/%02d]", ListPage+1, VariableNum/16+1);
  for(IRun = 0; IRun < 16; IRun++){
    CurListItem == IRun ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE) : gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, IRun+1);
    Print(L"%s", VariableList[IRun].VariableName);
    gST->ConOut->SetCursorPosition(gST->ConOut, 36, IRun+1);
    Print(L"%04x", VariableList[IRun].VariableSize);
    gST->ConOut->SetCursorPosition(gST->ConOut, 44, IRun+1);    
    Print(L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", VariableList[IRun].VendorGuid.Data1,
                                                               VariableList[IRun].VendorGuid.Data2,
                                                               VariableList[IRun].VendorGuid.Data3,
                                                               VariableList[IRun].VendorGuid.Data4[0],
                                                               VariableList[IRun].VendorGuid.Data4[1],
                                                               VariableList[IRun].VendorGuid.Data4[2],
                                                               VariableList[IRun].VendorGuid.Data4[3],
                                                               VariableList[IRun].VendorGuid.Data4[4],
                                                               VariableList[IRun].VendorGuid.Data4[5],
                                                               VariableList[IRun].VendorGuid.Data4[6],
                                                               VariableList[IRun].VendorGuid.Data4[7]
                                                               );
  }  
  while(1){
    Key->ReadKeyStroke(Key, &KeyData);
    if(KeyData.ScanCode == SCAN_ESC){
      gST->ConOut->ClearScreen(gST->ConOut);   
      break;        
    }else if(KeyData.ScanCode == SCAN_UP || KeyData.ScanCode == SCAN_LEFT){
      TMP = (CurListItem + VariableNum - 1) % VariableNum;
      TMP/16 == ListPage ? ChangeVariableListItem(&CurListItem, TMP) : ChangeVariableListPage(&ListPage, &CurListItem, TMP);  
    }else if(KeyData.ScanCode == SCAN_DOWN || KeyData.ScanCode == SCAN_RIGHT){
      TMP = (CurListItem + VariableNum + 1) % VariableNum;
      TMP/16 == ListPage ? ChangeVariableListItem(&CurListItem, TMP) : ChangeVariableListPage(&ListPage, &CurListItem, TMP);  
    }else if(KeyData.ScanCode == SCAN_NULL && KeyData.UnicodeChar == 0x0D){
      CurVariable = CurListItem;
      gST->ConOut->ClearScreen(gST->ConOut);
      break;
    }
  }
}

//
//  Offset  => Current Offset
//  Next    => Next Offset
//
VOID
ChangeVarDataOffset(  
  UINT8  *Offset,
  UINT8   Next
  )
{
  VariableData[*Offset] == 0xFF ? gST->ConOut->SetAttribute(gST->ConOut, EFI_DARKGRAY) : gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(*Offset), OffsetY(*Offset)+1);
  Print(L"%02x ", VariableData[*Offset]);
  *Offset = Next;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(*Offset), OffsetY(*Offset)+1);
  Print(L"%02x ", VariableData[*Offset]);
}

VOID 
Variable()
{
  EFI_STATUS  Status;
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);   
  ScanVariable();
  CurVariable = 0;
  VarDataInit();
  while(1)
  {
    Status = Key->ReadKeyStroke(Key, &KeyData);
    if(Status == EFI_SUCCESS){
      if(KeyData.ScanCode == SCAN_ESC){      
        break;        
      }else if(KeyData.ScanCode == SCAN_UP){
        
      }else if(KeyData.ScanCode == SCAN_DOWN){

      }else if(KeyData.ScanCode == SCAN_LEFT){

      }else if(KeyData.ScanCode == SCAN_RIGHT){

      }else if(KeyData.ScanCode == SCAN_PAGE_UP){

      }else if(KeyData.ScanCode == SCAN_DOWN){
        
      }else if(KeyData.ScanCode == SCAN_F2){
        VariableListMenu();
        VarDataInit(); 
      }
    }
  }
  return ;
}