#include "SMBios.h"

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

VOID
FetchSMBiosData(UINT32 Address)
{
  
  SMBIOS_STRUCTURE_POINTER	*SMBiosStructure = NULL;              
  UINT8                     *SMBiosAddress   = NULL; 
  UINT8                     *Addr            = NULL;  
  UINTN                     Flag;  
  UINTN                     x;
  Flag = 0;
  SMBiosSize = 0;
  //Get smbios information
  SMBiosAddress = (UINT8*)(UINTN)Address;
  SMBiosStructure = (SMBIOS_STRUCTURE_POINTER*)AllocateZeroPool(sizeof(SMBIOS_STRUCTURE_POINTER));
  SMBiosStructure->Raw = SMBiosAddress;
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  Print(L"SMBIOS- Type: %03d",SMBiosStructure->Hdr->Type);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
  HandleNum = SMBiosStructure->Hdr->Handle;
  Print(L"Size: %02x, Handle: 0x%04x, Address: 0x%08x", SMBiosSize, SMBiosStructure->Hdr->Handle, SMBiosAddress);

  for(x = 0; x <= 0xFF; x++){
    Addr = SMBiosAddress + x;
    SMBiosData[x] = *Addr;
    if(x >= SMBiosStructure->Hdr->Length && *Addr == 0 && *(Addr-1) == 0 && Flag == 0){	  		
	  SMBiosSize = x + 1;	
	  Flag = 1;	
	}
  }  
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  Print(L"Size: %02x", SMBiosSize);  
  FreePool(SMBiosStructure);  
  return ;
}

VOID 
SMBios(){
  EFI_STATUS                Status;  
  UINT32                    Address;       
  SMBIOS_TABLE_ENTRY_POINT 	*SMBiosTable = NULL;
  UINTN                     x = 0;
  UINTN                     y = 0;
  UINTN                     Offset = 0;
  UINTN                     PreSmbiosSize = 0;
  //clean full scran and leave
  gST->ConOut->ClearScreen(gST->ConOut);
  //SMBios Entry
  Status = EfiLibGetSystemConfigurationTable(
             &SMBIOS_GUID,
		     (VOID**)&SMBiosTable
		   ); 
  if(Status != EFI_SUCCESS || SMBiosTable == NULL || !CompareMem(SMBiosTable->AnchorString, "_SM_", 4)){
    Print(L"Error: SMBIOS table not found.\n");
	return ;
  }
  //Get smbios information  
  HeadAddr = SMBiosTable->TableAddress;
  //
  SMBiosSize = 0;
  Address = HeadAddr;
  FetchSMBiosData(HeadAddr);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, 3*x+3, y+2+1);
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, x+3+1);
    Print(L"%01x0", x);
  }    
//show smbios information
  for(x = 0; x <= 0xFF; x++){
    if(x >= SMBiosSize){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_DARKGRAY);
    }else{
     x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);    
    }
    gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x)+1);
    Print(L"%02x", SMBiosData[x]);    
  }
  
  //SMBios Detail
  for(x = 0; x <= 0xFF; x++){
    if(x > SMBiosSize-1){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_DARKGRAY);
    }else{
      x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    }
    gST->ConOut->SetCursorPosition(gST->ConOut, x%16 + 54 , OffsetY(x)+1);
    if(SMBiosData[x] >= '0' && SMBiosData[x] <= '9'){
      Print(L"%c", SMBiosData[x]);
    }else if(SMBiosData[x] >= 'A' && SMBiosData[x] <= 'Z'){
      Print(L"%c", SMBiosData[x]);
    }else if(SMBiosData[x] >= 'a' && SMBiosData[x] <= 'z'){
      Print(L"%c", SMBiosData[x]);
    }else{
      Print(L".");
    }
  }
  // Input Keyboard
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
        gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1);
        if(SMBiosData[Offset] >= '0' && SMBiosData[Offset] <= '9'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'A' && SMBiosData[Offset] <= 'Z'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'a' && SMBiosData[Offset] <= 'z'){
          Print(L"%c", SMBiosData[Offset]);
        }else{
          Print(L".");
        }
        x = SMBiosSize + 16 - (SMBiosSize % 16);
        Offset = (Offset + x - 16) % x;
        if(Offset > SMBiosSize-1) Offset = (Offset + x - 16) % x;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x ", SMBiosData[Offset]); 
        gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1);
        if(SMBiosData[Offset] >= '0' && SMBiosData[Offset] <= '9'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'A' && SMBiosData[Offset] <= 'Z'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'a' && SMBiosData[Offset] <= 'z'){
          Print(L"%c", SMBiosData[Offset]);
        }else{
          Print(L".");
        }       
      }else if(KeyData.ScanCode == SCAN_DOWN){
      	gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", SMBiosData[Offset]);
        gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1);
        if(SMBiosData[Offset] >= '0' && SMBiosData[Offset] <= '9'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'A' && SMBiosData[Offset] <= 'Z'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'a' && SMBiosData[Offset] <= 'z'){
          Print(L"%c", SMBiosData[Offset]);
        }else{
          Print(L".");
        }
        x = SMBiosSize + 16 - (SMBiosSize % 16);
        Offset = (Offset + x + 16) % x;
        if(Offset > SMBiosSize-1 ) Offset = (Offset + x + 16) % x;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x ", SMBiosData[Offset]);
        gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1);
        if(SMBiosData[Offset] >= '0' && SMBiosData[Offset] <= '9'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'A' && SMBiosData[Offset] <= 'Z'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'a' && SMBiosData[Offset] <= 'z'){
          Print(L"%c", SMBiosData[Offset]);
        }else{
          Print(L".");
        }
      }else if(KeyData.ScanCode == SCAN_LEFT){
      	gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", SMBiosData[Offset]);
        gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1);
        if(SMBiosData[Offset] >= '0' && SMBiosData[Offset] <= '9'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'A' && SMBiosData[Offset] <= 'Z'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'a' && SMBiosData[Offset] <= 'z'){
          Print(L"%c", SMBiosData[Offset]);
        }else{
          Print(L".");
        }
        Offset = (Offset + SMBiosSize - 1) % SMBiosSize;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x ", SMBiosData[Offset]);
        gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1);
        if(SMBiosData[Offset] >= '0' && SMBiosData[Offset] <= '9'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'A' && SMBiosData[Offset] <= 'Z'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'a' && SMBiosData[Offset] <= 'z'){
          Print(L"%c", SMBiosData[Offset]);
        }else{
          Print(L".");
        }
      }else if(KeyData.ScanCode == SCAN_RIGHT){
      	gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", SMBiosData[Offset]);
        gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1);
        if(SMBiosData[Offset] >= '0' && SMBiosData[Offset] <= '9'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'A' && SMBiosData[Offset] <= 'Z'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'a' && SMBiosData[Offset] <= 'z'){
          Print(L"%c", SMBiosData[Offset]);
        }else{
          Print(L".");
        }
        Offset = (Offset + SMBiosSize + 1) % SMBiosSize;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x ", SMBiosData[Offset]);
        gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)+1);
        if(SMBiosData[Offset] >= '0' && SMBiosData[Offset] <= '9'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'A' && SMBiosData[Offset] <= 'Z'){
          Print(L"%c", SMBiosData[Offset]);
        }else if(SMBiosData[Offset] >= 'a' && SMBiosData[Offset] <= 'z'){
          Print(L"%c", SMBiosData[Offset]);
        }else{
          Print(L".");
        }
      }else if(KeyData.ScanCode == SCAN_PAGE_UP){
        if(Address == HeadAddr){
          PreSmbiosSize = 0;
          continue;
        }else{
          //Address = Address - PreSmbiosSize;
          // FetchSMBiosData(Address);          
        }
      }else if(KeyData.ScanCode == SCAN_PAGE_DOWN){
        //if()
      }
    }
  }  
  return ;
}