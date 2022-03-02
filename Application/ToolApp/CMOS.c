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
      gotoXY(offsetX(x), offsetY(x));
      Print(L"%02x ", DataArray[x]);
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
  gotoXY(0, 0);
  SetColor(EFI_LIGHTGRAY);
  Print(L"CMOS - 70/71\n");
  //Table offset
  SetColor(EFI_BROWN);
  for(Index = 0; Index <= 0xF; Index++){
    gotoXY(rowX(Index), rowY(Index));
    Print(L"%02x", Index);
    gotoXY(columnX(Index), columnY(Index));
    Print(L"%01x0", Index);
  }

  for(Index = 0; Index <= 0xFF; Index++) {
    Index == offset ? SetColor(EFI_WHITE) : SetColor(EFI_LIGHTGRAY);
    gotoXY(offsetX(Index), offsetY(Index));
    Print(L"%02x ", CMOSData[Index]);
  }

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
          gotoXY(offsetX(offset), offsetY(offset));
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