#include "ToolApp.h"

Tool_Menu TMenu[] = {
  {0x00,     CMOS,        L"CMOS"},
  {0x01,  IOSpace,     L"IOSpace"},
  {0x02,      PCI,         L"PCI"},
  {0x03,   Memory,      L"Memory"},
  {0x04,    SMBus,       L"SMBus"},
  {0x05,     EDID,        L"EDID"},
  {0x06,   SMBios,      L"SMBios"},
  {0x07,  USBView,     L"USBView"},
  {0x08, Variable,    L"Variable"}
};

VOID tool()
{
  UINTN         x = 0;
  UINTN         y = 0;
  UINTN         Index;
  UINTN         CurrPost  = 0;
  UINTN         TotalNum  = 0;
  UINT32        CurrMode  = 0;
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_INPUT_KEY key;

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
    y == CurrPost ? SetColor(EFI_WHITE) : SetColor(EFI_LIGHTGRAY);
    GotoXY(x, y);
    Print(L"%s\n", TMenu[y].FunName);
  }
  
  do {    
      gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
      Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
      switch(key.ScanCode)
      {
        case SCAN_DOWN:    
        case SCAN_RIGHT:   
            SetColor(EFI_LIGHTGRAY);
            GotoXY(x, CurrPost);
            Print(L"%s\n", TMenu[CurrPost].FunName);
            CurrPost = (CurrPost + TotalNum + 1) % TotalNum;
            SetColor(EFI_WHITE);
            GotoXY(x, CurrPost);
            Print(L"%s\n", TMenu[CurrPost].FunName);
        break;
        case SCAN_UP:
        case SCAN_LEFT:
            SetColor(EFI_LIGHTGRAY);
            GotoXY(x, CurrPost);
            Print(L"%s\n", TMenu[CurrPost].FunName);
            CurrPost = (CurrPost + TotalNum - 1) % TotalNum;
            SetColor(EFI_WHITE);
            GotoXY(x, CurrPost);
            Print(L"%s\n", TMenu[CurrPost].FunName);
        break;
        case SCAN_NULL:
            if(key.UnicodeChar == 0x0D) { // Enter Function
                TMenu[CurrPost].FuncPtr();
                gST->ConOut->ClearScreen(gST->ConOut); 
                for(y = 0; y < TotalNum; y++){
                    y == CurrPost? SetColor(EFI_WHITE) : SetColor(EFI_LIGHTGRAY);
                    GotoXY(x, y);
                    Print(L"%s\n", TMenu[y].FunName);
                }
            }
        break;
      }
  } while(key.ScanCode != SCAN_ESC);
          
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
   }// else if(Argv[1][0] == '-' || '/'){
  //   Print(L"TEST3\n");
  // } else {
  //   Print(L"use -h for help\n");
  // }

  return 0;
}
