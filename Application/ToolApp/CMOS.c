#include "ToolApp.h"

VOID updateCMOSData (
  UINT8         *DataArray,
  UINT8         offset,
  DISPLAY_MODE  DisplayMode
  )
{
  UINTN  x;
  UINT8  Data;
  for(x = 0; x <= 0xFF; x++) {
    IoWrite8(CMOS_PORT, (UINT8)x);
    Data = IoRead8(CMOS_VALUE); 
    if(DataArray[x] != Data) {
      DataArray[x] = Data;  
      x == offset ? SetColor(SHOW_CHOOSE_DATA) : (DataArray[x] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
      gotoXY(BlockC_OffsetX(x), BlockC_OffsetY(x));
      Print(L"%02x ", DataArray[x]);
      if(DisplayMode == DISPLAY_INFOR) {
        switch(x) {
          case RTC_YEAR:
            SetColor(SHOW_DATA_COLOR);
            gotoXY(BlockD_Info_X + 10, BlockD_Info_Y + 1);
            Print(L"%02x", DataArray[RTC_YEAR]);
            break;
          case RTC_MONTH:
            SetColor(SHOW_DATA_COLOR);
            gotoXY(BlockD_Info_X + 15, BlockD_Info_Y + 1);
            Print(L"%02x", DataArray[RTC_MONTH]);
            break;
          case RTC_DATE:
            SetColor(SHOW_DATA_COLOR);
            gotoXY(BlockD_Info_X + 20, BlockD_Info_Y + 1);
            Print(L"%02x", DataArray[RTC_DATE]);
            break;
          case RTC_HOURS:
            SetColor(SHOW_DATA_COLOR);
            gotoXY(BlockD_Info_X + 10, BlockD_Info_Y + 2);
            Print(L"%02x", DataArray[RTC_HOURS]);
            break;
          case RTC_MINUTES:
            SetColor(SHOW_DATA_COLOR);
            gotoXY(BlockD_Info_X + 15, BlockD_Info_Y + 2);
            Print(L"%02x", DataArray[RTC_MINUTES]);
            break;
          case RTC_SECONDs:
            SetColor(SHOW_DATA_COLOR);
            gotoXY(BlockD_Info_X + 20, BlockD_Info_Y + 2);
            Print(L"%02x", DataArray[RTC_SECONDs]);
            break;
        }
      } else if (DisplayMode == DISPLAY_ASCII) {
        ShowASCII(x, DataArray[x]);
      }
    }
  }
  return ;
}

VOID CMOS() {
  EFI_INPUT_KEY key;
  UINTN         offset;
  UINTN         Index;
  UINTN         EventIndex;
  EFI_EVENT     TimerEvent;
  EFI_EVENT     WaitList[2];
  BOOLEAN       InputMode;
  UINT8         InputData;
  UINT8         CMOSData[256];
  DISPLAY_MODE  DisplayMode;

  //clean full scream
  gST->ConOut->ClearScreen(gST->ConOut);
  offset = 0;
  InputData = 0;
  InputMode = FALSE;
  DisplayMode = DISPLAY_ASCII;
  ZeroMem(CMOSData, 256 * sizeof(UINT8));
  // Block A
  SetColor(EFI_LIGHTGRAY);
  gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
  Print(L"# CMOS #");
  gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
  Print(L"[ 0x70 / 0x71 ]");
  gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
  BLOCK_BOUNDARY;
  // Block B
  SetColor(EFI_LIGHTGRAY);
  gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
  Print(L"Page:1/1");
  // Block C
  SetColor(EFI_BROWN);
  for(Index = 0; Index <= 0xF; Index++){
    gotoXY(BlockC_RowX(Index), BlockC_RowY(Index));
    Print(L"%02x", Index);
    gotoXY(BlockC_ColumnX(Index), BlockC_ColumnY(Index));
    Print(L"%01x0", Index);
  }
  
  for(Index = 0; Index <= 0xFF; Index++) {
    Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (CMOSData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
    gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
    Print(L"%02x ", CMOSData[Index]);
    // Block D information
    if(DisplayMode == DISPLAY_ASCII) ShowASCII(Index, CMOSData[Index]);
  }
  // Block D
  ShowBlockDTitle(DisplayMode);
  //Update CMOS Data
  updateCMOSData(CMOSData, (UINT8)offset, DisplayMode);

  do {
    // Set a timer event of 1 second expiration  
    gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
    gBS->SetTimer (TimerEvent, TimerRelative, DELAY_TIME);
    WaitList[0] = gST->ConIn->WaitForKey;  
    WaitList[1] = TimerEvent;
    // waiting event ... (Input Key or time to update data)
    gBS->WaitForEvent (2, WaitList, &EventIndex); 
    gBS->CloseEvent (TimerEvent);
    updateCMOSData(CMOSData, (UINT8)offset, DisplayMode);
    if(EventIndex == 1) continue;
    gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
    switch(key.ScanCode) {
      case SCAN_UP:
        InputMode = FALSE;
        offset = MoveCursor(offset, (offset + 256 - 16) % 256, CMOSData, DisplayMode);
        break;
      case SCAN_DOWN:
        InputMode = FALSE;
        offset = MoveCursor(offset, (offset + 256 + 16) % 256, CMOSData, DisplayMode);
        break;
      case SCAN_LEFT:
        InputMode = FALSE;
        offset = MoveCursor(offset, (offset + 256 - 1) % 256, CMOSData, DisplayMode);
        break;
      case SCAN_RIGHT:
        InputMode = FALSE;
        offset = MoveCursor(offset, (offset + 256 + 1) % 256, CMOSData, DisplayMode);
        break;
      case SCAN_NULL:
        if(key.UnicodeChar == CHAR_BACKSPACE && InputMode) {
          InputData >>= 4;
          SetColor(SHOW_CHOOSE_DATA);
          gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
          Print(L"%02x ", InputData);
        } else if (key.UnicodeChar == 0x0D && InputMode) {
          IoWrite8(CMOS_PORT, (UINT8)offset);
          IoWrite8(CMOS_VALUE, InputData);
          InputMode = FALSE;
        } else if(IsDigital(key.UnicodeChar)) {
          InputData = InputMode ? InputData : 0;
          InputMode = TRUE;
          InputData = ChangeInputData(InputData, key.UnicodeChar, offset);
        } else if(key.UnicodeChar == EFI_KEY_TAB) {
          // display mode
          if(DisplayMode == DISPLAY_INFOR) {
            DisplayMode = DISPLAY_ASCII;            
            CleanBlockD(DisplayMode);
            for(Index = 0; Index <= 0xFF; Index++) {
              Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (CMOSData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
              ShowASCII(Index, CMOSData[Index]);
            }
          } else if(DisplayMode == DISPLAY_ASCII) {
            DisplayMode = DISPLAY_INFOR;
            CleanBlockD(DisplayMode);
            gotoXY(BlockD_Info_X, BlockD_Info_Y + 1);
            SetColor(BLOCKD_TITLE_COLOR);
            Print(L"RTC Date: ");
            SetColor(SHOW_DATA_COLOR);
            Print(L"%02x / %02x / %02x", CMOSData[RTC_YEAR], CMOSData[RTC_MONTH], CMOSData[RTC_DATE]);
            gotoXY(BlockD_Info_X, BlockD_Info_Y + 2);
            SetColor(BLOCKD_TITLE_COLOR);
            Print(L"RTC Time: ");
            SetColor(SHOW_DATA_COLOR);
            Print(L"%02x : %02x : %02x", CMOSData[RTC_HOURS], CMOSData[RTC_MINUTES], CMOSData[RTC_SECONDs]);
          }
        }
        break;
    }
  } while(key.ScanCode!=SCAN_ESC);  
  return ;
}