#include "SMBios_2.h"

UINTN
GuidCompare(
	EFI_GUID	*Guid1,
	EFI_GUID	*Guid2 
  )
{
  UINT8  Index;
  for(Index = 0; Index < 16; Index++){
    if(*((UINT8*)Guid1 + Index) != *((UINT8*)Guid2 + Index)){
      return 0;
	}
  }
  return 1;
}

EFI_STATUS
EfiLibGetSystemConfigurationTable(
  EFI_GUID  *Guid,
  VOID      **Table
  )
{
  UINTN	x, y;
  *Table = NULL;
  y = gST -> NumberOfTableEntries;
  for(x = 0; x < y; x++){
    if(GuidCompare(Guid, &(gST->ConfigurationTable[x].VendorGuid)) == 1){
      *Table = gST->ConfigurationTable[x].VendorTable;
	  return EFI_SUCCESS;
	}		
  }
  return EFI_NOT_FOUND;
}

UINTN
CompareMem(
  VOID   *Dest,
  VOID   *Source,
  UINTN  Length
  )
{
  if(Length == 0 || Dest == Source){
    return 1;
  }
  while(Length > 0){
    if(*(UINT8*)Dest != *(UINT8*)Source){
	  return 0;
	}
	Length--;
 	Dest = (UINT8*)Dest + 1;
	Source = (UINT8*)Source + 1;
  }
  return 1;
}

EFI_STATUS
ScanSMBios()
{
  EFI_STATUS                Status;
  UINT8                     *Address;
  UINT8                     *Addr;
  SMBIOS_TABLE_ENTRY_POINT  *SMBiosTable = NULL;
  SMBIOS_STRUCTURE_POINTER  *SMBiosStructure = NULL;
  UINTN                     Flag = 0;
  UINT32                    HeadAddr = 0;
  UINTN                     x;
  UINTN                     y;

  Status = EfiLibGetSystemConfigurationTable(
             &SMBIOS_GUID,
             (VOID**)&SMBiosTable
           );
  if(Status != EFI_SUCCESS || SMBiosTable == NULL || !CompareMem(SMBiosTable->AnchorString, "_SM_", 4)){
    Print(L"Error: SMBIOS table not found.\n");
    return EFI_NOT_FOUND;
  }
  HeadAddr = SMBiosTable->TableAddress;
  Address = (UINT8*)(UINTN)HeadAddr;
  SMBiosNum = SMBiosTable->NumberOfSmbiosStructures;
  for(x = 0; x < SMBiosNum; x++){
    SMBiosStructure = (SMBIOS_STRUCTURE_POINTER*)AllocateZeroPool(sizeof(SMBIOS_STRUCTURE_POINTER));
    SMBiosStructure->Raw = Address;
    SMBbiosInfo[x].SMBType = SMBiosStructure->Hdr->Type;
    SMBbiosInfo[x].SMBAddress = (UINT32)(UINTN)Address;
    SMBbiosInfo[x].SMBLen = 0;
    SMBbiosInfo[x].SMBHandle = SMBiosStructure->Hdr->Handle;
    //update SMBios Length
    for(y = 0, Flag = 0; ; y++){     
      Addr = Address + y;
      if(Flag == 1 && *Addr != 0){
        SMBbiosInfo[x].SMBLen = (UINT16)y;
        break;
      }
      if(y >= SMBiosStructure->Hdr->Length && *Addr == 0 && *(Addr-1) == 0){
        Flag = 1;        
      }     
    }
    Address += y;
    FreePool(SMBiosStructure); 
  }
  SMBbiosInfo[x].SMBType = 0xFF;
  SMBbiosInfo[x].SMBAddress = 0xFFFFFFFF;
  SMBbiosInfo[x].SMBLen = 0xFFFF;
  SMBbiosInfo[x].SMBHandle = 0xFFFF;
  return EFI_SUCCESS;
}

VOID ShowSmbiosASCII(UINTN Offset)
{
  gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1); 
  IsASCIIDigit(SMBiosData[Offset]) ? Print(L"%c", SMBiosData[Offset]) : Print(L".");
}

VOID
UpdateSMBiosData()
{
  UINTN  Offset = 0;
  UINTN  x;
  UINT8  *AddrStart;
  UINT8  *Addr;
  AddrStart = (UINT8*)(UINTN)SMBbiosInfo[Current].SMBAddress;
  for(x = 0; x <= 0xFF; x++){
    Addr = AddrStart + x;
    SMBiosData[x] = *Addr;
    if(x >= SMBbiosInfo[Current].SMBLen){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_DARKGRAY);
    }else{
     x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);    
    }
    gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x)+1);

    Print(L"%02x", SMBiosData[x]);
    ShowSmbiosASCII(x);
  }
  return ;
}

VOID 
SMBios()
{
  UINTN       x = 0;
  UINTN       Offset = 0;
  EFI_STATUS  Status;
  //clean full scran and leave
  gST->ConOut->ClearScreen(gST->ConOut);
  Status = ScanSMBios();
  if(Status != EFI_SUCCESS){
    Print(L"Error~");
    return;
  }
  Current = 0;
  //show SMBios information
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  Print(L"SMBIOS [Type: %03d, SMBiosNum: %02d/%02d]",SMBbiosInfo[Current].SMBType, Current + 1, SMBiosNum);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1); 
  Print(L"Size: %02x, Handle: 0x%04x, Address: 0x%08x", SMBbiosInfo[Current].SMBLen, SMBbiosInfo[Current].SMBHandle, SMBbiosInfo[Current].SMBAddress); 
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2); 
  Print(L"--------------------------------------------------");
  //Column Row
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, RowX(x), RowY(x)+1);
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, ColumnX(x), ColumnY(x)+1);
    Print(L"%01x0", x);
  } 

  UpdateSMBiosData();

  while(1)
  {
    Status = Key->ReadKeyStroke(Key, &KeyData);
    if(Status == EFI_SUCCESS){
      if(KeyData.ScanCode == SCAN_ESC){ 
        break;        
      }else if(KeyData.ScanCode == SCAN_UP){
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", SMBiosData[Offset]);
        ShowSmbiosASCII(Offset);
        x = SMBbiosInfo[Current].SMBLen + 16 - (SMBbiosInfo[Current].SMBLen % 16);
        Offset = (Offset + x - 16) % x;
        if(Offset >= SMBbiosInfo[Current].SMBLen){
          Offset = (Offset + x - 16) % x;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x ", SMBiosData[Offset]); 
        ShowSmbiosASCII(Offset);
      }else if(KeyData.ScanCode == SCAN_DOWN){
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", SMBiosData[Offset]);
        ShowSmbiosASCII(Offset);
        x = SMBbiosInfo[Current].SMBLen + 16 - (SMBbiosInfo[Current].SMBLen % 16);
        Offset = (Offset + x + 16) % x;
        if(Offset >= SMBbiosInfo[Current].SMBLen){
          Offset = (Offset + x + 16) % x;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x ", SMBiosData[Offset]); 
        ShowSmbiosASCII(Offset);
      }else if(KeyData.ScanCode == SCAN_LEFT){
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", SMBiosData[Offset]);
        ShowSmbiosASCII(Offset);
        Offset = (Offset + SMBbiosInfo[Current].SMBLen - 1) % SMBbiosInfo[Current].SMBLen;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x ", SMBiosData[Offset]);
        ShowSmbiosASCII(Offset);
      }else if(KeyData.ScanCode == SCAN_RIGHT){
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", SMBiosData[Offset]);
        ShowSmbiosASCII(Offset);
        Offset = (Offset + SMBbiosInfo[Current].SMBLen + 1) % SMBbiosInfo[Current].SMBLen;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x ", SMBiosData[Offset]);
        ShowSmbiosASCII(Offset);
      }else if(KeyData.ScanCode == SCAN_PAGE_UP){
        Current == 0 ? Current = SMBiosNum - 1 : Current--;
        Offset = 0;
        //show SMBios information
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        Print(L"SMBIOS [Type: %03d, SMBiosNum: %02d/%02d]",SMBbiosInfo[Current].SMBType, Current + 1, SMBiosNum);
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1); 
        Print(L"Size: %02x, Handle: 0x%04x, Address: 0x%08x", SMBbiosInfo[Current].SMBLen, SMBbiosInfo[Current].SMBHandle, SMBbiosInfo[Current].SMBAddress);  
        UpdateSMBiosData();
      }else if(KeyData.ScanCode == SCAN_PAGE_DOWN){
        Current == (SMBiosNum - 1) ? Current = 0 : Current++;
        Offset = 0;
        //show SMBios information
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        Print(L"SMBIOS [Type: %03d, SMBiosNum: %02d/%02d]",SMBbiosInfo[Current].SMBType, Current + 1, SMBiosNum);
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1); 
        Print(L"Size: %02x, Handle: 0x%04x, Address: 0x%08x", SMBbiosInfo[Current].SMBLen, SMBbiosInfo[Current].SMBHandle, SMBbiosInfo[Current].SMBAddress); 
        UpdateSMBiosData();
      }
    }
  }
  return ;
}