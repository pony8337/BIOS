#include "ToolApp.h"

VOID IOSpace(){return;}
VOID PCI(){return;}
VOID Memory(){return;}
VOID SMBus(){return;}
VOID EDID(){return;}
VOID SMBios(){return;}
VOID USBView(){return;}

VOID tool()
{
  UINTN         x = 0;
  UINTN         y = 0;
  UINTN         Index;
  UINTN         CurrPost;
  UINTN         TotalNum;
  UINT32        CurrMode;
  EFI_STATUS    Status;
  EFI_INPUT_KEY key;

  // Clean full screen
  gST->ConOut->ClearScreen(gST->ConOut);
  // Disable show Curson  
  gST->ConOut->EnableCursor(gST->ConOut, FALSE);  
  //save current Mode 
  CurrMode = gST->ConOut->Mode->Mode;  
  //set mode
  gST->ConOut->SetMode(gST->ConOut, 0);
  //Menu List
  TotalNum = sizeof(TMenu) / sizeof(Tool_Menu);
  CurrPost = 0;
  for(y = 0; y < TotalNum; y++){
    y == CurrPost ? SetColor(SHOW_CHOOSE_DATA) : SetColor(SHOW_DATA_COLOR);
    gotoXY(x, y);
    Print(L"%s\n", TMenu[y].FunName);
  }
  
  do {    
      gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
      Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
      switch(key.ScanCode)
      {
        case SCAN_DOWN:    
        case SCAN_RIGHT:   
            SetColor(SHOW_DATA_COLOR);
            gotoXY(x, CurrPost);
            Print(L"%s\n", TMenu[CurrPost].FunName);
            CurrPost = (CurrPost + TotalNum + 1) % TotalNum;
            SetColor(SHOW_CHOOSE_DATA);
            gotoXY(x, CurrPost);
            Print(L"%s\n", TMenu[CurrPost].FunName);
        break;
        case SCAN_UP:
        case SCAN_LEFT:
            SetColor(SHOW_DATA_COLOR);
            gotoXY(x, CurrPost);
            Print(L"%s\n", TMenu[CurrPost].FunName);
            CurrPost = (CurrPost + TotalNum - 1) % TotalNum;
            SetColor(SHOW_CHOOSE_DATA);
            gotoXY(x, CurrPost);
            Print(L"%s\n", TMenu[CurrPost].FunName);
        break;
        case SCAN_NULL:
            if(key.UnicodeChar == 0x0D) { // Enter Function
                TMenu[CurrPost].FuncPtr();
                gST->ConOut->ClearScreen(gST->ConOut); 
                for(y = 0; y < TotalNum; y++){
                    y == CurrPost ? SetColor(SHOW_CHOOSE_DATA) : SetColor(SHOW_DATA_COLOR);
                    gotoXY(x, y);
                    Print(L"%s\n", TMenu[y].FunName);
                }
            }
        break;
      }
  } while(key.ScanCode != SCAN_ESC);
          
  SetColor(SHOW_DATA_COLOR);
  //return back original mode
  gST->ConOut->SetMode(gST->ConOut, CurrMode);
  //clean full scan and leave
  gST->ConOut->ClearScreen(gST->ConOut);
  // enable show cursor
  gST->ConOut->EnableCursor(gST->ConOut, TRUE);
  return;
}


INTN 
EFIAPI 
ShellAppMain (
      IN UINTN Argc,
      IN CHAR16 **Argv
    )
{
  Print(L"TEST %d\n", Argc);
  if(Argc == 1) {
     tool();
   } else if(Argc == 3){
     Print(L"TEST3\n");
  } else {
    Print(L"use -h for help\n");
  }

  return 0;
}
