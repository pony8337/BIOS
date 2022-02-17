#include "CMOS.h"

VOID
UpdateCMOSData(
  UINT8 Offset
  )
{
  UINTN  x;
  UINT8  Data;
  for(x = 0; x <= 0xFF; x++){
    IoWrite8(0x70, (UINT8)x);
    Data = IoRead8(0x71); 
    if(CMOSData[x] != Data){      
      x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x));
      CMOSData[x] = Data;
      Print(L"%02x ", CMOSData[x]);
    }
  }
  return ;
}

VOID 
CMOS(){
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_EVENT   TimerEvent;
  EFI_EVENT   WaitList[2];
  UINTN       Index  = 0;
  UINTN       x = 0;
  UINTN       Offset = 0;
  UINT8       Number = 0;
  UINT8       Flag = 0;
  //clean full scran and leave
  gST->ConOut->ClearScreen(gST->ConOut);

  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);

  Print(L"CMOS - 70/71\n");
  //Table offset
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
    Print(L"%02x ", CMOSData[x]);
  }

  //Update CMOS Data
  UpdateCMOSData((UINT8)Offset);
  
  while(1)
  {
    Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
    //    
    // Set a timer event of 1 second expiration    
    //  
    Status = gBS->SetTimer (TimerEvent, TimerRelative, 10000000);
    //
    // Wait for the keystroke event or the timer    
    //  
    WaitList[0] = gST->ConIn->WaitForKey;  
    WaitList[1] = TimerEvent; 
    Status = gBS->WaitForEvent (2, WaitList, &Index); 
    UpdateCMOSData((UINT8)Offset);
    gBS->CloseEvent (TimerEvent);
    Key->ReadKeyStroke(Key, &KeyData);

    if(!EFI_ERROR (Status) && Index == 1){   
      Status = EFI_TIMEOUT;  
      continue;
    }    
    //key  
    if(KeyData.ScanCode == SCAN_ESC){
      break;
    }else if(KeyData.ScanCode == SCAN_RIGHT){
      if(Flag == 1){
        Number = 0;
        Flag = 0;
      }
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", CMOSData[Offset]);
      Offset = (Offset + 256 + 1) % 256;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", CMOSData[Offset]);
    }else if(KeyData.ScanCode == SCAN_LEFT){
      if(Flag == 1){
        Number = 0;
        Flag = 0;
      }
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", CMOSData[Offset]);
      Offset = (Offset + 256 - 1) % 256;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", CMOSData[Offset]);
    }else if(KeyData.ScanCode == SCAN_DOWN){
      if(Flag == 1){
        Number = 0;
        Flag = 0;
      }
    	gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", CMOSData[Offset]);
      Offset = (Offset + 256 + 16) % 256;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", CMOSData[Offset]);
    }else if(KeyData.ScanCode == SCAN_UP){
      if(Flag == 1){
        Number = 0;
        Flag = 0;
      }
    	gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", CMOSData[Offset]);
      Offset = (Offset + 256 - 16) % 256;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", CMOSData[Offset]);
    }else if((KeyData.ScanCode == SCAN_NULL && KeyData.UnicodeChar == CHAR_BACKSPACE) && Flag == 1){
      Number >>= 4;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", Number);
    }else if((KeyData.ScanCode == SCAN_NULL && KeyData.UnicodeChar == 0x0D) && Flag == 1){
      IoWrite8(0x70, (UINT8)Offset);
      IoWrite8(0x71, Number);
      Flag = 0;
      Number = 0;
    }else if(KeyData.ScanCode == SCAN_NULL && (KeyData.UnicodeChar >= '0' && KeyData.UnicodeChar <= '9')){
      Flag = 1;        
      Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - '0');
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", Number);
    }else if(KeyData.ScanCode == SCAN_NULL && (KeyData.UnicodeChar >= 'a' && KeyData.UnicodeChar <= 'f')){
      Flag = 1;
      Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - 'a' + 10);
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", Number);
    }else if(KeyData.ScanCode == SCAN_NULL && (KeyData.UnicodeChar >= 'A' && KeyData.UnicodeChar <= 'F')){
      Flag = 1;
      Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - 'A' + 10);
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", Number);
    }
  }  
  return ;
}