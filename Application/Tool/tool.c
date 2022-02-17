/*main.c */
#include "Tool.h"
#include "DeviceList.h"

EFI_STATUS EFIAPI UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{ 
  UINT32       CurrMode  = 0;
  UINTN        CurrPost  = 0;
  UINTN        TotalNum  = 0;
  EFI_STATUS   Status = EFI_SUCCESS;
  UINTN        x = 0;
  UINTN        y = 0;


  // Text in Protocol
  gBS->HandleProtocol(
       gST->ConsoleInHandle,
       &gEfiSimpleTextInProtocolGuid,
       (VOID**)&Key
       );

  // Clean full screan
  gST->ConOut->ClearScreen(gST->ConOut);

  // Disable show Curson  
  gST->ConOut->EnableCursor(gST->ConOut, FALSE);  

  //save current Mode  
  CurrMode = gST->ConOut->Mode->Mode;  
  
  //set mode
  gST->ConOut->SetMode(gST->ConOut, 0);
  
  //Menu List
  TotalNum = sizeof(TMenu) / sizeof(Tool_Menu);
  for(y = 0; y < TotalNum; y++){
    y == CurrPost ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    gST->ConOut->SetCursorPosition(gST->ConOut, x, y);
    Print(L"%s\n", TMenu[y].FunName);
  }
  
  while(1){    
    Status = Key->ReadKeyStroke(Key, &KeyData);
    if(Status == EFI_SUCCESS){
      if(KeyData.ScanCode == SCAN_ESC){      
        break;        
      }else if(KeyData.ScanCode == SCAN_DOWN || KeyData.ScanCode == SCAN_RIGHT){
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, x, CurrPost); 
        Print(L"%s\n", TMenu[CurrPost].FunName);
        CurrPost = (CurrPost + TotalNum + 1) % TotalNum;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, x, CurrPost); 
        Print(L"%s\n", TMenu[CurrPost].FunName);
      }else if(KeyData.ScanCode == SCAN_UP || KeyData.ScanCode == SCAN_LEFT){
        gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
        gST->ConOut->SetCursorPosition(gST->ConOut, x, CurrPost); 
        Print(L"%s\n", TMenu[CurrPost].FunName);
        CurrPost = (CurrPost + TotalNum - 1) % TotalNum;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, x, CurrPost); 
        Print(L"%s\n", TMenu[CurrPost].FunName);
      }else if(KeyData.ScanCode == SCAN_NULL && KeyData.UnicodeChar == 0x0D){
        TMenu[CurrPost].FuncPtr();
        //return back from function
        gST->ConOut->ClearScreen(gST->ConOut); 
        for(y = 0; y < TotalNum; y++){
          y == CurrPost ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
          gST->ConOut->SetCursorPosition(gST->ConOut, x, y);
          Print(L"%s\n", TMenu[y].FunName);
        }
      }
    }

  }
  //return back origional mode
  gST->ConOut->SetMode(gST->ConOut, CurrMode);
  //clean full scran and leave
  gST->ConOut->ClearScreen(gST->ConOut);
  // enable show cursor
  gST->ConOut->EnableCursor(gST->ConOut, TRUE);  

  return Status;
}