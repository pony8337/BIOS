#include "ToolApp.h"

UINT8 IOPortRead (
    UINTN   IOPort
)
{
    return IoRead8(IOPort);
}

UINT8 IOPortWrite (
    IN  UINTN   Port,
    IN  UINT8   data
)
{
    IoWrite8(Port, data);
    return data;
}


VOID updateIOData (
    IN  UINTN   Port,
    IN  UINT8   *DataArray, 
    IN  UINTN   offset,
    IN  BOOLEAN ModifyPortMode
    )
{
    UINTN   Index;
    UINT8   Data;
    for(Index = 0; Index <= 0xFF; Index++) {        
        Data = IOPortRead (Port + Index);
        if(DataArray[Index] != Data) {
            DataArray[Index] = Data;  
            ModifyPortMode && Index == offset ? (DataArray[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR)) : (Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (DataArray[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR)));
            gotoXY(BlockC_OffsetX (Index), BlockC_OffsetY (Index));
            Print(L"%02x ", DataArray[Index]);
            ShowASCII(Index, DataArray[Index]);
        }
    }
    return ;
}


UINTN
ChangeInputPort (
  IN  UINTN  Port,
  IN  CHAR16 InputValue
  )
{
	if (InputValue >= '0' && InputValue <= '9') {
		Port = (Port << 4) | (InputValue - '0');		
        SetColor(INPUT_DATA_COLOR);
        gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
        Print(L"Port: 0x%04x", Port);
		return Port;
	} else if (InputValue >= 'a' && InputValue <= 'f') {
		Port = (Port << 4) | (InputValue - 'a' + 10);
		SetColor(INPUT_DATA_COLOR);
        gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
        Print(L"Port: 0x%04x", Port);
		return Port;
	} else if (InputValue >= 'A' && InputValue <= 'F') {
		Port = (Port << 4) | (InputValue - 'A' + 10);
		SetColor(INPUT_DATA_COLOR);
        gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
        Print(L"Port: 0x%04x", Port);
		return Port;
	}
	return Port;
}



VOID IOSpace()
{
    EFI_INPUT_KEY   key;
    UINTN           offset;
    UINTN           InputPort;
    UINTN           Port;
    UINTN           Index;
    UINTN           EventIndex;
    EFI_EVENT       TimerEvent;
    EFI_EVENT       WaitList[2];
    UINT8           IOSpaceData[256];
    DISPLAY_MODE    DisplayMode;
    BOOLEAN         ModifyPortMode;
    BOOLEAN         InputMode;
    UINT8           InputData;
    //clean full scream
    gST->ConOut->ClearScreen(gST->ConOut);
    DisplayMode = DISPLAY_ASCII;
    InputMode = FALSE;
    ModifyPortMode = FALSE;
    InputData = 0;
    Port = 0;
    offset = 0;
    InputPort = 0;
    ZeroMem(IOSpaceData, 256 * sizeof(UINT8));
    // Block A    
    SetColor(TITLE_COLOR);
    gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
    Print(L"# IOSpace #");
    gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
    Print(L"Port: 0x%04x", Port);
    gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
    BLOCK_BOUNDARY;
    // Block B
    gotoXY(BlockB_Page_Num_X-17 , BlockB_Page_Num_Y);
    Print(L"F2: Modified I/O Port");
    // Block C    
    ShowBlocCIndex();
    for(Index = 0; Index <= 0xFF; Index++) {
        (Index == offset) ? SetColor(SHOW_CHOOSE_DATA) : (IOSpaceData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
        
        gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));        
        Print(L"%02x ", IOSpaceData[Index]);
        // Block D information
        ShowASCII(Index, IOSpaceData[Index]);
    }

    do {
        // Set a timer event of 1 second expiration  
        gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
        gBS->SetTimer (TimerEvent, TimerRelative, DELAY_TIME);
        WaitList[0] = gST->ConIn->WaitForKey;  
        WaitList[1] = TimerEvent;
        // waiting event ... (Input Key or time to update data)
        gBS->WaitForEvent (2, WaitList, &EventIndex); 
        gBS->CloseEvent (TimerEvent);
        updateIOData (Port, IOSpaceData, offset, ModifyPortMode);
        if(EventIndex == 1) continue;
        gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
        switch(key.ScanCode) {
        case SCAN_UP:
            InputMode = FALSE;
            if(!ModifyPortMode)
                offset = MoveCursor(offset, (offset + 256 - 16) % 256, IOSpaceData, DisplayMode);            
            break;
        case SCAN_DOWN:
            InputMode = FALSE;
            if(!ModifyPortMode)
                offset = MoveCursor(offset, (offset + 256 + 16) % 256, IOSpaceData, DisplayMode);
            break;
        case SCAN_LEFT:
            InputMode = FALSE;
            if(!ModifyPortMode)
                offset = MoveCursor(offset, (offset + 256 - 1) % 256, IOSpaceData, DisplayMode);
            break;
        case SCAN_RIGHT:
            InputMode = FALSE;
            if(!ModifyPortMode)
                offset = MoveCursor(offset, (offset + 256 + 1) % 256, IOSpaceData, DisplayMode);
            break;
        case SCAN_F2:
            InputMode = FALSE;
            ModifyPortMode = TRUE;
            InputPort = Port;
            SetColor(INPUT_DATA_COLOR);
            gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
            Print(L"Port: 0x%04x", InputPort);            
            // Change Color
            SetColor(SHOW_DATA_COLOR);
            gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));        
            Print(L"%02x ", IOSpaceData[offset]);
            ShowASCII(offset, IOSpaceData[offset]);
            
            break;
        case SCAN_NULL:
            if(key.UnicodeChar == CHAR_BACKSPACE) {
                if(ModifyPortMode) {
                    InputPort >>= 4;
                    SetColor(INPUT_DATA_COLOR);
                    gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
                    Print(L"Port: 0x%04x", InputPort);    
                } else if(InputMode) {
                    InputData >>= 4;
                    SetColor(INPUT_DATA_COLOR);
                    gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
                    Print(L"%02x ", InputData);
                }
            } else if (key.UnicodeChar == EFI_KEY_ENTER) {
                if(ModifyPortMode) {
                    Port = InputPort;
                    ModifyPortMode = FALSE;
                    Port = (Port > 0xFFFFFF00) ? 0xFFFFFF00 : Port;  
                    SetColor(TITLE_COLOR);
                    gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
                    Print(L"Port: 0x%04x", Port);
                    updateIOData (Port, IOSpaceData, offset, ModifyPortMode);                    
                } else if(InputMode) {
					IOSpaceData[offset] = InputData;
				    SetColor(SHOW_CHOOSE_DATA);
					gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
                    Print(L"%02x ", IOSpaceData[offset]);
                    IOPortWrite(Port + offset, IOSpaceData[offset]);
                    InputMode = FALSE;
                }
            } else if(IsDigital(key.UnicodeChar)) {
                if(!ModifyPortMode) {
                    InputData = InputMode ? InputData : 0;
                    InputMode = TRUE;
                    InputData = ChangeInputData(InputData, key.UnicodeChar, offset);
                } else {
                    InputPort = ChangeInputPort(InputPort, key.UnicodeChar);
                }
            } 
            break;
        case SCAN_ESC:
            if(ModifyPortMode) {
                ModifyPortMode = FALSE;
                key.ScanCode = SCAN_NULL;
                // change color
                SetColor(TITLE_COLOR);
                gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
                Print(L"Port: 0x%04x", Port);             
                SetColor(SHOW_CHOOSE_DATA);
                gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));        
                Print(L"%02x ", IOSpaceData[offset]);
                ShowASCII(offset, IOSpaceData[offset]);
            }
            break;
        
        }
    } while(key.ScanCode!=SCAN_ESC);
    return ;
}