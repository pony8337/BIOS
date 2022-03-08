#include "ToolApp.h"

VOID updateCMOSData (
  UINT8 *DataArray,
  UINT8  offset
  )
{
  UINTN  x;
  UINT8  Data;
  for(x = 0; x <= 0xFF; x++) {
    IoWrite8(CMOS_PORT, (UINT8)x);
    Data = IoRead8(CMOS_VALUE); 
    if(DataArray[x] != Data) {
      DataArray[x] = Data;  
      x == offset ? SetColor(EFI_WHITE) : SetColor(EFI_LIGHTGRAY);
      gotoXY(BlockC_OffsetX(x), BlockC_OffsetY(x));
      Print(L"%02x ", DataArray[x]);
      SetColor(EFI_WHITE);
      if(x == RTC_YEAR) {
        gotoXY(BlockD_Info_X + 10, BlockD_Info_Y + 1);
        Print(L"%02x", DataArray[RTC_YEAR]);
      } else if(x == RTC_MONTH) {
        gotoXY(BlockD_Info_X + 15, BlockD_Info_Y + 1);
        Print(L"%02x", DataArray[RTC_MONTH]);
      } else if(x == RTC_DATE) {
        gotoXY(BlockD_Info_X + 20, BlockD_Info_Y + 1);
        Print(L"%02x", DataArray[RTC_DATE]);
      } else if(x == RTC_HOURS) {
        gotoXY(BlockD_Info_X + 10, BlockD_Info_Y + 2);
        Print(L"%02x", DataArray[RTC_HOURS]);
      } else if(x == RTC_MINUTES) {
        gotoXY(BlockD_Info_X + 15, BlockD_Info_Y + 2);
        Print(L"%02x", DataArray[RTC_MINUTES]);
      } else if(x == RTC_SECONDs) {
        gotoXY(BlockD_Info_X + 20, BlockD_Info_Y + 2);
        Print(L"%02x", DataArray[RTC_SECONDs]);
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

  //clean full scream
  gST->ConOut->ClearScreen(gST->ConOut);
  offset = 0;
  InputData = 0;
  InputMode = FALSE;
  ZeroMem(CMOSData, 256*sizeof(UINT8));
  // Block A
  SetColor(EFI_LIGHTGRAY);
  gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
  Print(L"# CMOS #");
  gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
  Print(L"[ 0x70 / 0x71 ]");
  gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
  Block_Boundary;
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
    Index == offset ? SetColor(EFI_WHITE) : SetColor(EFI_LIGHTGRAY);
    gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
    Print(L"%02x ", CMOSData[Index]);
  }
  // Block D
  SetColor(EFI_BROWN);
  gotoXY(BlockD_Info_X, BlockD_Info_Y);
  Print(L"[ CMOS RTC Information ]");
  gotoXY(BlockD_Info_X, BlockD_Info_Y + 1);
  SetColor(EFI_LIGHTRED);
  Print(L"RTC Date: ");
  SetColor(EFI_WHITE);
  Print(L"%02x / %02x / %02x", CMOSData[RTC_YEAR], CMOSData[RTC_MONTH], CMOSData[RTC_DATE]);
  gotoXY(BlockD_Info_X, BlockD_Info_Y + 2);
  SetColor(EFI_LIGHTRED);
  Print(L"RTC Time: ");
  SetColor(EFI_WHITE);
  Print(L"%02x : %02x : %02x", CMOSData[RTC_HOURS], CMOSData[RTC_MINUTES], CMOSData[RTC_SECONDs]);


  //Update CMOS Data
  updateCMOSData(&CMOSData[0], (UINT8)offset);

  do {
    // Set a timer event of 1 second expiration  
    gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
    gBS->SetTimer (TimerEvent, TimerRelative, DELAY_TIME);
    WaitList[0] = gST->ConIn->WaitForKey;  
    WaitList[1] = TimerEvent;
    // waiting event ... (Input Key or time to update data)
    gBS->WaitForEvent (2, WaitList, &EventIndex); 
    gBS->CloseEvent (TimerEvent);
    updateCMOSData(&CMOSData[0], (UINT8)offset);
    if(EventIndex == 1) continue;
    gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
    switch(key.ScanCode) {
      case SCAN_UP:
        InputMode = FALSE;
        offset = MoveCursor(offset, (offset + 256 - 16) % 256, CMOSData);
      break;
      case SCAN_DOWN:
        InputMode = FALSE;
        offset = MoveCursor(offset, (offset + 256 + 16) % 256, CMOSData);
      break;
      case SCAN_LEFT:
        InputMode = FALSE;
        offset = MoveCursor(offset, (offset + 256 - 1) % 256, CMOSData);
      break;
      case SCAN_RIGHT:
        InputMode = FALSE;
        offset = MoveCursor(offset, (offset + 256 + 1) % 256, CMOSData);
      break;
      case SCAN_NULL:
        if(key.UnicodeChar == CHAR_BACKSPACE && InputMode) {
          InputData >>= 4;
          SetColor(EFI_WHITE);
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
        }    
      break;
    }
  } while(key.ScanCode!=SCAN_ESC);  
  return ;
}