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
      gST->ConOut->SetCursorPosition(gST->ConOut, 3*(x%16)+3, (x/16)+3);
      Print(L"%02x", IoDataTbl[x]);
    } 
  }
  return ;
}

VOID 
IOSpace()
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       x      = 0;
  UINTN       y      = 0;
  UINT8       Offset = 0;
  UINTN       Flag   = 0;
  UINTN       Timer  = 0;
  UINT8       Number = 0;
  UINTN       WriteF = 0;
  UINTN       IoNumP = 0;
  //clean full scran and leave
  gST->ConOut->ClearScreen(gST->ConOut);
  //position and color
  gST->ConOut->SetCursorPosition(gST->ConOut, x, y);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  Print(L"IOSpace - Port: 0x%04x - 0x%04x", IoPort, IoPort+0xFF);

  //Table offset
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, 3*x+3, y+2);
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, x+3);
    Print(L"%01x0", x);
  }
  
  UpdateIoData(Offset, Flag); 
  //IoPort Data
  for(x = 0; x <= 0xFF; x++){
    (x == Offset) && (Flag != 0) ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    gST->ConOut->SetCursorPosition(gST->ConOut, 3*(x%16)+3, (x/16)+3);
    Print(L"%02x ", IoDataTbl[x]);
  }
  while(1)
  {    
    if(Timer == 5000000){
      UpdateIoData(Offset, Flag);
      Timer = 0;
    }
    Status = Key->ReadKeyStroke(Key, &KeyData);
    if(Status == EFI_SUCCESS){
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
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);
        Offset = (Offset + 256 - 1) % 256;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);
      }else if(KeyData.ScanCode == SCAN_RIGHT && Flag != 0){
        if(WriteF == 1){
          WriteF = 0;
          Number = 0;
          IoNumP = 0;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);
        Offset = (Offset + 256 + 1) % 256;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);
      }else if(KeyData.ScanCode == SCAN_UP && Flag != 0){
        if(WriteF == 1){
          WriteF = 0;
          Number = 0;
          IoNumP = 0;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);
        Offset = (Offset + 256 - 16) % 256;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);        
      }else if(KeyData.ScanCode == SCAN_DOWN && Flag != 0){
        if(WriteF == 1){
          WriteF = 0;
          Number = 0;
          IoNumP = 0;
        }
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);
        Offset = (Offset + 256 + 16) % 256;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset));
        Print(L"%02x ", IoDataTbl[Offset]);        
      }else if(KeyData.ScanCode == SCAN_NULL){
        if(KeyData.UnicodeChar == 0x0D){
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
    Timer++;
  }
  return;
}