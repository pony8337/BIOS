#include "ToolApp.h"

BOOLEAN MemioWrite8(
    IN  UINTN   Address,
    IN  UINT8   data
)
{
    CHAR8 *Addr = NULL;
    Addr += Address;
    *Addr = data;
    return data == *Addr ? TRUE : FALSE;
}

UINT8 MemioRead8 (
    UINTN   Address
)
{
    CHAR8 *Addr = NULL;
    Addr += Address;
    return (UINT8)(*Addr);
}


VOID updateMmioData (
    IN  UINTN   Address,
    IN  UINT8   *DataArray, 
    IN  UINTN   offset,
    IN  BOOLEAN ModifyAddrMode
    )
{
    UINTN   Index;
    UINT8   Data;
    for(Index = 0; Index <= 0xFF; Index++) {        
        Data = MemioRead8 (Address + Index);
        if(DataArray[Index] != Data) {
            DataArray[Index] = Data;  
            ModifyAddrMode && Index == offset ? (DataArray[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR)) : (Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (DataArray[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR)));
            gotoXY(BlockC_OffsetX (Index), BlockC_OffsetY (Index));
            Print(L"%02x ", DataArray[Index]);
            ShowASCII(Index, DataArray[Index]);
        }
    }
    return ;
}


// UINT8
// ChangeInputData (
//   IN  UINT8  Data,
//   IN  CHAR16 InputData,
//   IN  UINTN  offset
//   )
// {
// 	if (InputData >= '0' && InputData <= '9') {
// 		Data = (Data << 4) | (UINT8)(InputData - '0');
// 		SetColor(INPUT_DATA_COLOR);
// 		gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
// 		Print(L"%02x ", Data);
// 		return Data;
// 	} else if (InputData >= 'a' && InputData <= 'f') {
// 		Data = (Data << 4) | (UINT8)(InputData - 'a' + 10);
// 		SetColor(INPUT_DATA_COLOR);
// 		gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
// 		Print(L"%02x ", Data);
// 		return Data;
// 	} else if (InputData >= 'A' && InputData <= 'F') {
// 		Data = (Data << 4) | (UINT8)(InputData - 'A' + 10);
// 		SetColor(INPUT_DATA_COLOR);
// 		gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
// 		Print(L"%02x ", Data);
// 		return Data;
// 	}
// 	return Data;
// }



VOID MMIO()
{
    EFI_INPUT_KEY   key;
    UINTN           offset;
    UINTN           InputAddr;
    UINTN           Address;
    UINTN           Index;
    UINTN           EventIndex;
    EFI_EVENT       TimerEvent;
    EFI_EVENT       WaitList[2];
    UINT8           MMIOData[256];
    DISPLAY_MODE    DisplayMode;
    BOOLEAN         ModifyAddrMode;
    BOOLEAN         InputMode;
    UINT8           InputData;
    //clean full scream
    gST->ConOut->ClearScreen(gST->ConOut);
    DisplayMode = DISPLAY_ASCII;
    InputMode = FALSE;
    ModifyAddrMode = FALSE;
    InputData = 0;
    Address = 0;
    offset = 0;
    InputAddr = 0;
    // Block A    
    SetColor(TITLE_COLOR);
    gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
    Print(L"# MMIO #");
    gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
    Print(L"Address: 0x%08x", Address);
    gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
    BLOCK_BOUNDARY;
    // Block B
    gotoXY(BlockB_Page_Num_X-17 , BlockB_Page_Num_Y);
    Print(L"F2: Modified MMIO Address");
    // Block C    
    ShowBlocCIndex();
    for(Index = 0; Index <= 0xFF; Index++) {
        (Index == offset) ? SetColor(SHOW_CHOOSE_DATA) : (MMIOData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
        MMIOData[Index] = MemioRead8(Address+Index);
        // SetColor(SHOW_CHOOSE_DATA);
        gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));        
        Print(L"%02x ", MMIOData[Index]);
        // Block D information
        ShowASCII(Index, MMIOData[Index]);
    }
    // updateMmioData (Address, MMIOData, offset);

    do {
        // Set a timer event of 1 second expiration  
        gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
        gBS->SetTimer (TimerEvent, TimerRelative, DELAY_TIME);
        WaitList[0] = gST->ConIn->WaitForKey;  
        WaitList[1] = TimerEvent;
        // waiting event ... (Input Key or time to update data)
        gBS->WaitForEvent (2, WaitList, &EventIndex); 
        gBS->CloseEvent (TimerEvent);
        updateMmioData (Address, MMIOData, offset, ModifyAddrMode);
        if(EventIndex == 1) continue;
        gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
        switch(key.ScanCode) {
        case SCAN_UP:
            InputMode = FALSE;
            if(!ModifyAddrMode)
                offset = MoveCursor(offset, (offset + 256 - 16) % 256, MMIOData, DisplayMode);            
            break;
        case SCAN_DOWN:
            InputMode = FALSE;
            if(!ModifyAddrMode)
                offset = MoveCursor(offset, (offset + 256 + 16) % 256, MMIOData, DisplayMode);
            break;
        case SCAN_LEFT:
            InputMode = FALSE;
            if(!ModifyAddrMode)
                offset = MoveCursor(offset, (offset + 256 - 1) % 256, MMIOData, DisplayMode);
            break;
        case SCAN_RIGHT:
            InputMode = FALSE;
            if(!ModifyAddrMode)
                offset = MoveCursor(offset, (offset + 256 + 1) % 256, MMIOData, DisplayMode);
            break;
        case SCAN_F2:
            InputMode = FALSE;
            ModifyAddrMode = TRUE;
            InputAddr = Address;
            SetColor(INPUT_DATA_COLOR);
            gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
            Print(L"Address: 0x%08x", InputAddr);            
            // Change Color
            SetColor(SHOW_DATA_COLOR);
            gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));        
            Print(L"%02x ", MMIOData[offset]);
            ShowASCII(offset, MMIOData[offset]);
            
            break;
        case SCAN_NULL:
            // key.ScanCode=SCAN_ESC;
            if(key.UnicodeChar == CHAR_BACKSPACE && InputMode) {
                InputData >>= 4;
                SetColor(INPUT_DATA_COLOR);
                gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
                Print(L"%02x ", InputData);
            } else if (key.UnicodeChar == 0x0D && InputMode) {        
                MemioWrite8(Address + offset, InputData);
                InputMode = FALSE;
            } else if(IsDigital(key.UnicodeChar)) {
                if(!ModifyAddrMode) {
                    InputData = InputMode ? InputData : 0;
                    InputMode = TRUE;
                    InputData = ChangeInputData(InputData, key.UnicodeChar, offset);
                }
            } 
            break;
        case SCAN_ESC:
            if(ModifyAddrMode) {
                ModifyAddrMode = FALSE;
                key.ScanCode = SCAN_NULL;
                // change color
                SetColor(TITLE_COLOR);
                gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
                Print(L"Address: 0x%08x", Address);             
                SetColor(SHOW_CHOOSE_DATA);
                gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));        
                Print(L"%02x ", MMIOData[offset]);
                ShowASCII(offset, MMIOData[offset]);
            }
            break;
        
        }
    } while(key.ScanCode!=SCAN_ESC);
    return ;
}