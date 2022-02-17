#include "Memory.h"

VOID 
UpdateMemoryData(
  UINT8 Offset, 
  UINTN Flag
  )
{
  UINTN  x;
  for(x = 0; x < 256; x++){
    if(MemDataTbl[x] != (UINT8)*(Addr+x)){
      MemDataTbl[x] = (UINT8)*(Addr+x);
      (x == Offset) && (Flag != 0) ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x));
      Print(L"%02x", MemDataTbl[x]);
    } 
  }
  return ;
}

//
//  Offset  => Current Offset
//  Next    => Next Offset
//
VOID
ChangeMemOffset(
  UINT8  *Offset, 
  UINT8  Next
  )
{
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(*Offset), OffsetY(*Offset));
  Print(L"%02x ", MemDataTbl[*Offset]);
  *Offset = Next;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(*Offset), OffsetY(*Offset));
  Print(L"%02x ", MemDataTbl[*Offset]);
}

VOID 
Memory()
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_EVENT   TimerEvent;
  EFI_EVENT   WaitList[2];
  UINTN       Index  = 0;
  UINTN       x      = 0;
  UINT8       Offset = 0;
  UINT8       Next   = 0;  
  UINTN       Flag   = 0;  // Address = 0 : Data = 1
  UINTN       WriteF = 0;  // Write Flag
  UINT32      AddNum = 0;
  UINT8       Number = 0;
  //clean full screan 
  gST->ConOut->ClearScreen(gST->ConOut);
  //position and color
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  Addr  = NULL;
  Print(L"# MMIO # - Address: 0x%08x\n", Addr);
  
  //Table offset
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, RowX(x), RowY(x));
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, ColumnX(x), ColumnY(x));
    Print(L"%01x0", x);
  }

  //Memory Data
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  for(x = 0; x <= 0xFF; x++){
    (x == Offset) && (Flag != 0) ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x));
    Print(L"%02x ", MemDataTbl[x]);
  }
  UpdateMemoryData(Offset, Flag);
  
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
    UpdateMemoryData(Offset, Flag);
    gBS->CloseEvent (TimerEvent);

    Key->ReadKeyStroke(Key, &KeyData);
    
    if(!EFI_ERROR (Status) && Index == 1){   
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
        Print(L"%02x ", MemDataTbl[Offset]);
        gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        Print(L"# MMIO # - Address: 0x%08x\n", Addr);      
        Flag = 0;
      }
    }else if(KeyData.ScanCode == SCAN_LEFT && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
        AddNum = 0;
      }
      Next = (Offset + 256 - 1) % 256;
      ChangeMemOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_RIGHT && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
        AddNum = 0;
      }      
      Next = (Offset + 256 + 1) % 256;
      ChangeMemOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_UP && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
        AddNum = 0;
      }
      Next = (Offset + 256 - 16) % 256;
      ChangeMemOffset(&Offset, Next);     
    }else if(KeyData.ScanCode == SCAN_DOWN && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
        AddNum = 0;
      }
      Next = (Offset + 256 + 16) % 256;
      ChangeMemOffset(&Offset, Next);

    }else if(KeyData.ScanCode == SCAN_PAGE_UP && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
      }
      AddNum = (UINT32)(UINTN)Addr;
      AddNum = (AddNum < 0x100) ? 0 : AddNum-0x100;
      Addr = NULL;
      Addr += AddNum;
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      Print(L"# MMIO # - Address: 0x%08x\n", AddNum);
      UpdateMemoryData(Offset, Flag);
      AddNum = 0;

    }else if(KeyData.ScanCode == SCAN_PAGE_DOWN && Flag != 0){
      if(WriteF == 1){
        WriteF = 0;
        Number = 0;
      }
      AddNum = (UINT32)(UINTN)Addr;
      AddNum = (AddNum >= 0xFFFFFE00) ? 0xFFFFFF00 : AddNum+0x100;
      Addr = NULL;
      Addr += AddNum;
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      Print(L"# MMIO # - Address: 0x%08x\n", AddNum);
      UpdateMemoryData(Offset, Flag);
      AddNum = 0;

    }else if(KeyData.ScanCode == SCAN_NULL){
      if(KeyData.UnicodeChar >= '0' && KeyData.UnicodeChar <= '9'){
        if(Flag == 0){            
          AddNum = (AddNum << 4) | (UINT32)(KeyData.UnicodeChar - '0');        
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"# MMIO # - Address: 0x%08x\n", AddNum);      
          WriteF = 1; 
        }else if(Flag == 1){            
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - '0');        
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"%02x ", Number);      
          WriteF = 1;   
        }
      }else if(KeyData.UnicodeChar >= 'a' && KeyData.UnicodeChar <= 'f'){
        if(Flag == 0){            
          AddNum = (AddNum << 4) | (UINT32)(KeyData.UnicodeChar - 'a' + 10);
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"# MMIO # - Address: 0x%08x\n", AddNum);      
          WriteF = 1;  
        }else if(Flag == 1){            
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - 'a' + 10);        
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"%02x ", Number);      
          WriteF = 1;  
        }
      }else if(KeyData.UnicodeChar >= 'A' && KeyData.UnicodeChar <= 'F'){
        if(Flag == 0){            
          AddNum = (AddNum << 4) | (UINT32)(KeyData.UnicodeChar - 'A' + 10);
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"# MMIO # - Address: 0x%08x\n", AddNum);      
          WriteF = 1;
        }else if(Flag == 1){            
          Number = (Number << 4) | (UINT8)(KeyData.UnicodeChar - 'A' + 10);        
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"%02x ", Number);      
          WriteF = 1; 
        } 
      }else if(KeyData.UnicodeChar == CHAR_BACKSPACE && WriteF == 1){
        if(Flag == 0){
          AddNum >>= 4;
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"# MMIO # - Address: 0x%08x\n", AddNum);  
        }else if(Flag == 1){
          Number >>= 4;
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));  
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          Print(L"%02x ", Number);  
        }
      }else if(KeyData.UnicodeChar == 0x0D){
        if(Flag == 0){          
          if(WriteF == 1){
            Addr = NULL;
            Addr = (AddNum > 0xFFFFFF00) ? Addr+0xFFFFFF00 : Addr+AddNum;    
          }                   
          gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
          gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
          Print(L"# MMIO # - Address: 0x%08x\n", Addr);
          gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
          Print(L"%02x", MemDataTbl[0]);
          Offset = 0;             
          AddNum = 0;
          WriteF = 0;
          Flag = 1;
          UpdateMemoryData(Offset, Flag);
        }else if(Flag == 1 && WriteF == 1){
          *(Addr+Offset) = Number;
          Number = 0;
          WriteF = 0;
        }
      }   
    }    
  }

  return ;
}