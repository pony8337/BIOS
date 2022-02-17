#include "Eeprom.h"

VOID 
Wait4IBE()
{
  while((IoRead8(0x66)&0x02));
  return ;
}
VOID 
Wait4OBF()
{
  while(!(IoRead8(0x66)&0x01));
  return;
}

UINT8 
ReadEeprom(
  UINT8 offset
  )
{
  Wait4IBE();
  IoWrite8(0x66, 0x4E);
  Wait4IBE();
  IoWrite8(0x62, offset);
  Wait4IBE();
  Wait4OBF();
  return IoRead8(0x62);
}

VOID 
WriteEeprom(
  UINT8 offset, 
  UINT8 data
  )
{
  Wait4IBE();
  IoWrite8(0x66, 0x4D);
  Wait4IBE();
  IoWrite8(0x62, offset);
  Wait4IBE();
  IoWrite8(0x62, data);
  return ;
}

VOID AssignBank(
  UINT8 bank
  )
{
  Wait4IBE();
  IoWrite8(0x66, 0x42);
  Wait4IBE();
  IoWrite8(0x62, bank);
  return ;
}

VOID ShowEepromASCII(
  UINTN Offset
  )
{
  gST->ConOut->SetCursorPosition(gST->ConOut, Offset%16 + 54 , OffsetY(Offset)); 
  IsASCIIDigit(EepromData[Offset]) ? Print(L"%c", EepromData[Offset]) : Print(L".");
}

VOID 
Eeprom()
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       x = 0;
  UINTN       Offset = 0;
  UINTN       Flag = 0;
  UINT8       Number = 0;
  UINT8       Bank = 0;
  //clean full scran and leave
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  Print(L"Please Wait!!!!");
  AssignBank(Bank);
  for(x = 0; x <= 0xFF; x++)
    EepromData[x] = ReadEeprom((UINT8)x);
   

  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  Print(L"Eeprom - Bank%02d", Bank);

  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, RowX(x), RowY(x));
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, ColumnX(x), ColumnY(x));
    Print(L"%01x0", x);
  }  
  for(x = 0; x <= 0xFF; x++){
    x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x));       
    Print(L"%02x ", EepromData[x]);
    ShowEepromASCII(x);
  }  

  while(1)
  {
    Status = Key->ReadKeyStroke(Key, &KeyData);
    if(Status == EFI_SUCCESS){
      if(KeyData.ScanCode == SCAN_ESC){      
        break;        
      }else if(KeyData.ScanCode == SCAN_PAGE_UP){
        if(Flag == 1){
          Number = 0;
          Flag = 0;
        }
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        Print(L"Please Wait!!!!");
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        Offset = 0;
        Bank = (Bank+7)%8;
        AssignBank(Bank);
        for(x = 0; x <= 0xFF; x++)
          EepromData[x] = ReadEeprom((UINT8)x);
        
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        Print(L"Eeprom - Bank%02d", Bank);
        for(x = 0; x <= 0xFF; x++){
          x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x));
          Print(L"%02x ", EepromData[x]);
          ShowEepromASCII(x);
        }  

      }else if(KeyData.ScanCode == SCAN_PAGE_DOWN){
        if(Flag == 1){
          Number = 0;
          Flag = 0;
        }
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        Print(L"Please Wait!!!!");
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        Offset = 0;
        Bank = (Bank+9)%8;
        AssignBank(Bank);
        for(x = 0; x <= 0xFF; x++)
          EepromData[x] = ReadEeprom((UINT8)x);
        
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        Print(L"Eeprom - Bank%02d", Bank);
        for(x = 0; x <= 0xFF; x++){
          x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x));
          Print(L"%02x ", EepromData[x]);
          ShowEepromASCII(x);
        }  
      }else if(KeyData.ScanCode == SCAN_RIGHT){
        if(Flag == 1){
          Number = 0;
          Flag = 0;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        ShowEepromASCII(Offset);
        Offset = (Offset + 256 + 1) % 256;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        ShowEepromASCII(Offset);
      }else if(KeyData.ScanCode == SCAN_LEFT){
        if(Flag == 1){
          Number = 0;
          Flag = 0;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        ShowEepromASCII(Offset);
        Offset = (Offset + 256 - 1) % 256;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        ShowEepromASCII(Offset);
      }else if(KeyData.ScanCode == SCAN_DOWN){
        if(Flag == 1){
          Number = 0;
          Flag = 0;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        ShowEepromASCII(Offset);
        Offset = (Offset + 256 + 16) % 256;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        ShowEepromASCII(Offset);
      }else if(KeyData.ScanCode == SCAN_UP){
        if(Flag == 1){
          Number = 0;
          Flag = 0;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        ShowEepromASCII(Offset);
        Offset = (Offset + 256 - 16) % 256;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", EepromData[Offset]);
        ShowEepromASCII(Offset);
      }else if(KeyData.ScanCode == SCAN_NULL){
        if(KeyData.UnicodeChar == 0x0D){
          if(Number != EepromData[Offset] && Flag == 1){
            EepromData[Offset] = Number;
            WriteEeprom((UINT8)Offset, Number);
            ShowEepromASCII(Offset);
          }
          Flag = 0;
          Number = 0;
        }else if(KeyData.UnicodeChar >= '0' && KeyData.UnicodeChar <= '9'){
          Flag = 1;
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - '0');
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
          Print(L"%02x ", Number);
        }else if(KeyData.UnicodeChar >= 'a' && KeyData.UnicodeChar <= 'f'){
          Flag = 1;
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - 'a' + 10);
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
          Print(L"%02x ", Number);
        }else if(KeyData.UnicodeChar >= 'A' && KeyData.UnicodeChar <= 'F'){
          Flag = 1;
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - 'A' + 10);
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
          Print(L"%02x ", Number);
        }       
      }
    }
  }

  return ;
}

