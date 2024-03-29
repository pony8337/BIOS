#include "ToolApp.h"

VOID VariableDetail (
    IN  VARIABLE_INFO VariableInfo
)
{
  EFI_INPUT_KEY key;
  UINTN         offset;
  UINTN         Index;
  UINTN         EventIndex;
  UINT8         VariableData[256];
  UINTN         CurrPage;
  UINTN         PageNum;
  UINT8         *VarData;
  UINT32        Attributes;
  DISPLAY_MODE  DisplayMode;

  DisplayMode = DISPLAY_ASCII;
  //clean full scream
  gST->ConOut->ClearScreen(gST->ConOut);
  offset = 0;
  // Block A
  SetColor(TITLE_COLOR);
  gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
  Print(L"[ %s ]", VariableInfo.VariableName);
  SetColor(TITLE_COLOR);
  gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
  Print(L"GUID: ");
  Print(L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", 
                              VariableInfo.VendorGuid.Data1,
                              VariableInfo.VendorGuid.Data2,
                              VariableInfo.VendorGuid.Data3,
                              VariableInfo.VendorGuid.Data4[0],
                              VariableInfo.VendorGuid.Data4[1],
                              VariableInfo.VendorGuid.Data4[2],
                              VariableInfo.VendorGuid.Data4[3],
                              VariableInfo.VendorGuid.Data4[4],
                              VariableInfo.VendorGuid.Data4[5],
                              VariableInfo.VendorGuid.Data4[6],
                              VariableInfo.VendorGuid.Data4[7]); 
  gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
  BLOCK_BOUNDARY;
  // Block B  
  SetColor(TITLE_COLOR);
  gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
  CurrPage = 1;
  PageNum = (VariableInfo.VariableSize % 256) == 0 ? VariableInfo.VariableSize / 256 : VariableInfo.VariableSize / 256 + 1;
  Print(L"Page:%d/%d", CurrPage, PageNum);
   
  ZeroMem(VariableData, 256 * sizeof(UINT8));
  // Get Variable Data
  VarData = (UINT8*)mGetVariable(VariableInfo.VariableName, &VariableInfo.VendorGuid, &VariableInfo.VariableSize, &Attributes);
  UpdateArrayData(VarData, VariableInfo.VariableSize, CurrPage, VariableData);

  // Block C
  ShowBlocCIndex();  
  for(Index = 0; Index <= 0xFF; Index++) {
    Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (VariableData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
    gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
    Print(L"%02x ", VariableData[Index]);
    // Block D information
    if(DisplayMode == DISPLAY_ASCII) ShowASCII(Index, VariableData[Index]);
  }
  // Block D
  ShowBlockDTitle(DisplayMode);
  

  // key event
  do {
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &EventIndex); 
    if(EventIndex == 1) continue;
    gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
    switch(key.ScanCode) {
      case SCAN_UP:
        offset = MoveCursor(offset, (offset + 256 - 16) % 256, VariableData, DisplayMode);
      break;
      case SCAN_DOWN:
        offset = MoveCursor(offset, (offset + 256 + 16) % 256, VariableData, DisplayMode);
      break;
      case SCAN_LEFT:
        offset = MoveCursor(offset, (offset + 256 - 1) % 256, VariableData, DisplayMode);
      break;
      case SCAN_RIGHT:
        offset = MoveCursor(offset, (offset + 256 + 1) % 256, VariableData, DisplayMode);
      break;
      case SCAN_PAGE_UP:
        DEBUG ((EFI_D_INFO, "\nSCAN_PAGE_UP\n"));
        if(PageNum == 1) break;
        CurrPage = CurrPage == 1 ? PageNum : CurrPage - 1;
        UpdateArrayData(VarData, VariableInfo.VariableSize, CurrPage, VariableData);
        // update display data
        SetColor(SHOW_DATA_COLOR);
        gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
        Print(L"Page:%d/%d", CurrPage, PageNum);
        for(Index = 0; Index <= 0xFF; Index++) {
          Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (VariableData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
          gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
          Print(L"%02x ", VariableData[Index]);
          // Block D information
          if(DisplayMode == DISPLAY_ASCII) ShowASCII(Index, VariableData[Index]);
        }
        break;
      case SCAN_PAGE_DOWN:
        DEBUG ((EFI_D_INFO, "\nSCAN_PAGE_DOWN\n"));
        if(PageNum == 1) break;
        CurrPage = CurrPage == PageNum ? 1 : CurrPage + 1;
        UpdateArrayData(VarData, VariableInfo.VariableSize, CurrPage, VariableData);
        // update display data
        SetColor(SHOW_DATA_COLOR);
        gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
        Print(L"Page:%d/%d", CurrPage, PageNum);
        for(Index = 0; Index <= 0xFF; Index++) {
          Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (VariableData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
          gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
          Print(L"%02x ", VariableData[Index]);
          // Block D information
          if(DisplayMode == DISPLAY_ASCII) ShowASCII(Index, VariableData[Index]);
        }        
        break;
      case SCAN_NULL:
        // TAB
        if(key.UnicodeChar == EFI_KEY_TAB) { 
           // display mode
          if(DisplayMode == DISPLAY_INFO) {
            DisplayMode = DISPLAY_ASCII;            
            CleanBlockD(DisplayMode);
            for(Index = 0; Index <= 0xFF; Index++) {
              Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (VariableData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
              ShowASCII(Index, VariableData[Index]);
            }
          } else if(DisplayMode == DISPLAY_ASCII) {
            DisplayMode = DISPLAY_INFO;
            CleanBlockD(DisplayMode);
            gotoXY(BlockD_Info_X, BlockD_Info_Y + 1);
            SetColor(BLOCKD_TITLE_COLOR);
            Print(L"Size: "); 
            SetColor(SHOW_DATA_COLOR);
            Print(L"%03x", VariableInfo.VariableSize);
            gotoXY(BlockD_Info_X, BlockD_Info_Y + 2);
            SetColor(BLOCKD_TITLE_COLOR);
            Print(L"Attribute: "); 
            SetColor(SHOW_DATA_COLOR);
            Print(L"%s", VariableInfo.Attributes);
          }
        }
      break;
    }
  } while(key.ScanCode!=SCAN_ESC);  
  return ;

}