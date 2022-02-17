#include "IOSpace.h"

VOID 
UpdateIoData(
  UINT8 Offset, 
  UINTN Flag
  )
{
  UINTN  x;
  UINT8  Data;
  for(x = 0; x < 256; x++){  
    Data = IoRead8(IoPort+x);
    if(IoDataTbl[x] != Data){      
      IoDataTbl[x] = Data;
      (x == Offset) && (Flag != 0) ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x));
      Print(L"%02x", IoDataTbl[x]);
    } 
  }
  return ;
}


//
//  Offset  => Current Offset
//  Next    => Next Offset
//
VOID
ChangIOSpaceOffset(
  UINT8  *Offset,
  UINT8  Next
  )
{
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(*Offset), OffsetY(*Offset));
  Print(L"%02x ", IoDataTbl[*Offset]);
  *Offset = Next;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(*Offset), OffsetY(*Offset));
  Print(L"%02x ", IoDataTbl[*Offset]);
}

VOID 
IOSpace()
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_EVENT   TimerEvent;
  EFI_EVENT   WaitList[2];
  UINTN       Index  = 0;
  UINTN       x      = 0;
  UINT8       Offset = 0;
  UINT8       Next   = 0;
  UINT8       Number = 0;
  UINTN       Flag   = 0;
  UINTN       WriteF = 0;
  UINTN       IoNumP = 0;  
  //clean full scran and leave
  gST->ConOut->ClearScreen(gST->ConOut);
  //position and color
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  IoPort = 0;
  Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoPort, IoPort+0xFF);

  //Table offset
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, RowX(x), RowY(x));
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, ColumnX(x), ColumnY(x));
    Print(L"%01x0", x);
  }
  UpdateIoData(Offset, Flag); 
  //IoPort Data
  for(x = 0; x <= 0xFF; x++){
    (x == Offset) && (Flag != 0) ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x));
    Print(L"%02x ", IoDataTbl[x]);
  }
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
    UpdateIoData(Offset, Flag);   
    gBS->CloseEvent (TimerEvent);
    Key->ReadKeyStroke(Key, &KeyData);    
    if (!EFI_ERROR (Status) && Index == 1) {   
      Status = EFI_TIMEOUT;  
      continue;
    }

    if(KeyData.ScanCode == SCAN_ESC){
      if(Flag == 0){
        break;
      }else{
        WriteF = 0;
        Number = 0;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoPort, IoPort+0xFF);     
        Flag = 0;
      }
    }else if(KeyData.ScanCode == SCAN_LEFT && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
        IoNumP = 0;
      }
      Next = (Offset + 256 - 1) % 256;
      ChangIOSpaceOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_RIGHT && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
        IoNumP = 0;
      }
      Next = (Offset + 256 + 1) % 256;
      ChangIOSpaceOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_UP && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
        IoNumP = 0;
      }
      Next = (Offset + 256 - 16) % 256;
      ChangIOSpaceOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_DOWN && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
        IoNumP = 0;
      }
      Next = (Offset + 256 + 16) % 256;
      ChangIOSpaceOffset(&Offset, Next);     
    }else if(KeyData.ScanCode == SCAN_PAGE_UP && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
      }
      IoPort = IoPort<0x100 ? 0 : IoPort-0x100;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
      Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoPort, IoPort+0xFF);
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", IoDataTbl[Offset]);
      UpdateIoData(Offset, Flag);

    }else if(KeyData.ScanCode == SCAN_PAGE_DOWN && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
      }
      IoPort = IoPort>0xFE00 ? 0xFF00 : IoPort+0x100;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
      Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoPort, IoPort+0xFF);
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
      Print(L"%02x ", IoDataTbl[Offset]);
      UpdateIoData(Offset, Flag);

    }else if(KeyData.ScanCode == SCAN_NULL){
      if(KeyData.UnicodeChar == CHAR_BACKSPACE && WriteF == 1){
        if(Flag == 0){
          IoNumP >>= 4;
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoNumP, IoNumP+0xFF);  
        }else if(Flag == 1){
          Number >>= 4;
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"%02x ", Number);  
        }
      }else if(KeyData.UnicodeChar == 0x0D){
        if(Flag == 0){
          if(WriteF == 1){
            IoPort = IoNumP;
            Offset = 0;
          }
          gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
          Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoPort, IoPort+0xFF);
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
          Print(L"%02x ", IoDataTbl[Offset]);             
          IoNumP = 0;
          WriteF = 0;
          Flag = 1;         
          UpdateIoData(Offset, Flag); 
        }else if(Flag == 1 && WriteF == 1){
          IoWrite8(IoPort+Offset, Number);
          Number = 0;
          WriteF = 0;
        }
      }else if(KeyData.UnicodeChar >= '0' && KeyData.UnicodeChar <= '9'){
        if(Flag == 0){            
          IoNumP = (IoNumP << 4) | (UINT32)(KeyData.UnicodeChar - '0');    
          if(IoNumP > 0xFF00)
             IoNumP = 0xFF00;         
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoNumP, IoNumP+0xFF);      
          WriteF = 1; 
        }else if(Flag == 1){            
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - '0');        
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"%02x ", Number);      
          WriteF = 1;   
        }
      }else if(KeyData.UnicodeChar >= 'A' && KeyData.UnicodeChar <= 'F'){
        if(Flag == 0){            
          IoNumP = (IoNumP << 4) | (UINT32)(KeyData.UnicodeChar - 'A' + 10);
          if(IoNumP > 0xFF00)
             IoNumP = 0xFF00;          
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoNumP, IoNumP+0xFF);      
          WriteF = 1; 
        }else if(Flag == 1){            
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - 'A' + 10);   
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"%02x ", Number);      
          WriteF = 1;   
        }
      }else if(KeyData.UnicodeChar >= 'a' && KeyData.UnicodeChar <= 'f'){
        if(Flag == 0){            
          IoNumP = (IoNumP << 4) | (UINT32)(KeyData.UnicodeChar - 'a' + 10);
          if(IoNumP > 0xFF00)
             IoNumP = 0xFF00;          
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoNumP, IoNumP+0xFF);      
          WriteF = 1; 
        }else if(Flag == 1){            
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - 'a' + 10);        
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"%02x ", Number);      
          WriteF = 1;   
        }
      }
    }
  
  }
  return;
}