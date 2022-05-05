#include "ToolApp.h"

VOID EDID()
{
    EFI_STATUS                      Status;
    EFI_INPUT_KEY                   key;
    UINTN                           Index;
    UINTN                           offset = 0;
    UINT8                           EdidData[256];
    DISPLAY_MODE                    DisplayMode;
    UINTN                           CurrentEdid = 0;
    UINTN                           EdidHandleNum;
    EFI_HANDLE                      *EdidHandleBuffer;
    EFI_EDID_DISCOVERED_PROTOCOL    *EdidInfo;
    UINT8                           ManufacturerName1;
    UINT8                           ManufacturerName2;
    UINT8                           ManufacturerName3;
        
    gST->ConOut->ClearScreen(gST->ConOut);
    DisplayMode = DISPLAY_INFOR;
    // search EDID information
    Status = gBS->LocateHandleBuffer(
                ByProtocol,
                &gEfiEdidDiscoveredProtocolGuid,
                NULL,
                &EdidHandleNum,
                &EdidHandleBuffer
            );
    if(Status != EFI_SUCCESS) {
        Print(L"Locate EDID Discovered Protocol %r", Status);
        gBS->Stall(STALL_ONE_SECOND);
        return;
    }
    Status = gBS->OpenProtocol(
                EdidHandleBuffer[CurrentEdid],
                &gEfiEdidDiscoveredProtocolGuid,
                &EdidInfo,
                gImageHandle,
                NULL,
                EFI_OPEN_PROTOCOL_GET_PROTOCOL                        
            );
    if(Status != EFI_SUCCESS) {
        Print(L"Open EDID Discovered Protocol %r", Status);
        gBS->Stall(STALL_ONE_SECOND);
        return;
    }
    // Block A
    SetColor(EFI_LIGHTGRAY);
    gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
    Print(L"# EDID #");
    gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
    Print(L"SizeOfEdid: 0x%2x",EdidInfo->SizeOfEdid);
    gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
    BLOCK_BOUNDARY;    
    // Block B
    gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
    Print(L"Page:1/1");
    gotoXY(BlockB_Information_X + 4, BlockB_Information_Y);
    Print(L"Screen: [ %d / %d ]", CurrentEdid + 1, EdidHandleNum);
    gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);

    // Block C    
    SetColor(BLOCKC_INDEX_COLOR);
    for(Index = 0; Index <= 0xF; Index++){
        gotoXY(BlockC_RowX(Index), BlockC_RowY(Index));
        Print(L"%02x", Index);
        gotoXY(BlockC_ColumnX(Index), BlockC_ColumnY(Index));
        Print(L"%01x0", Index);
    }   

    for(Index = 0; Index <= 0xFF; Index++) {
        EdidData[Index] = (Index < EdidInfo->SizeOfEdid) ? *(EdidInfo->Edid + Index) : 0xFF;
        Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (EdidData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
        gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
        Print(L"%02x ", EdidData[Index]);
        // Block D information
        if(DisplayMode == DISPLAY_ASCII) ShowASCII(Index, EdidData[Index]);
    }
    
    // Block D
    ShowBlockDTitle(DisplayMode);
    // Byte 8/9 ID Manufacturer Name 
    ManufacturerName1 = ((EdidData[8] >> 2) & 0x1F) + 64;
    ManufacturerName2 = (((EdidData[8] << 6) & 0xC0) | ((EdidData[9] >> 5) & 0x07)) + 64;
    ManufacturerName3 = (EdidData[9] & 0x1F) + 64;
    SetColor(BLOCKD_TITLE_COLOR);
    gotoXY(BlockD_Info_X, BlockD_Info_Y + 1);
    Print(L"Manufacturer Name: ");
    SetColor(SHOW_DATA_COLOR);
    Print(L"%c%c%c", ManufacturerName1, ManufacturerName2, ManufacturerName3);
    do {
        gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
        gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
        switch(key.ScanCode) {
            case SCAN_UP:
                offset = MoveCursor(offset, (offset + 256 - 16) % 256, EdidData, DisplayMode);
                break;
            case SCAN_DOWN:
                offset = MoveCursor(offset, (offset + 256 + 16) % 256, EdidData, DisplayMode);
                break;
            case SCAN_LEFT:
                offset = MoveCursor(offset, (offset + 256 - 1) % 256, EdidData, DisplayMode);
                break;
            case SCAN_RIGHT:
                offset = MoveCursor(offset, (offset + 256 + 1) % 256, EdidData, DisplayMode);
                break;
            // case SCAN_PAGE_UP:
            //     DEBUG ((EFI_D_INFO, "\nSCAN_PAGE_UP\n"));
            //     if(PageNum == 1) break;
            //     CurrPage = CurrPage == 1 ? PageNum : CurrPage - 1;
            //     UpdateArrayData(VarData, VariableInfo.VariableSize, CurrPage, VariableData);
            //     // update display data
            //     SetColor(SHOW_DATA_COLOR);
            //     gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
            //     Print(L"Page:%d/%d", CurrPage, PageNum);
            //     for(Index = 0; Index <= 0xFF; Index++) {
            //     Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (VariableData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
            //     gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
            //     Print(L"%02x ", VariableData[Index]);
            //     }
            //     break;
            // case SCAN_PAGE_DOWN:
            //     DEBUG ((EFI_D_INFO, "\nSCAN_PAGE_DOWN\n"));
            //     if(PageNum == 1) break;
            //     CurrPage = CurrPage == PageNum ? 1 : CurrPage + 1;
            //     UpdateArrayData(VarData, VariableInfo.VariableSize, CurrPage, VariableData);
            //     // update display data
            //     SetColor(SHOW_DATA_COLOR);
            //     gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
            //     Print(L"Page:%d/%d", CurrPage, PageNum);
            //     for(Index = 0; Index <= 0xFF; Index++) {
            //     Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (VariableData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
            //     gotoXY(BlockC_OffsetX(Index), BlockC_OffsetY(Index));
            //     Print(L"%02x ", VariableData[Index]);
            //     }                
            //     break;
            case SCAN_NULL:
                // TAB
                if(key.UnicodeChar == EFI_KEY_TAB) { 
                    // display mode
                    if(DisplayMode == DISPLAY_INFOR) {
                        DisplayMode = DISPLAY_ASCII;            
                        CleanBlockD(DisplayMode);
                        for(Index = 0; Index <= 0xFF; Index++) {
                            Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (EdidData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
                            ShowASCII(Index, EdidData[Index]);
                        }
                    } else if(DisplayMode == DISPLAY_ASCII) {
                        DisplayMode = DISPLAY_INFOR;
                        CleanBlockD(DisplayMode);
                        SetColor(BLOCKD_TITLE_COLOR);
                        gotoXY(BlockD_Info_X, BlockD_Info_Y + 1);
                        Print(L"Manufacturer Name: ");
                        SetColor(SHOW_DATA_COLOR);
                        Print(L"%c%c%c", ManufacturerName1, ManufacturerName2, ManufacturerName3);
                        // gotoXY(BlockD_Info_X, BlockD_Info_Y + 1);
                        // SetColor(BLOCKD_TITLE_COLOR);
                        // Print(L"Size: "); 
                        // SetColor(SHOW_DATA_COLOR);
                        // Print(L"%03x", VariableInfo.VariableSize);
                        // gotoXY(BlockD_Info_X, BlockD_Info_Y + 2);
                        // SetColor(BLOCKD_TITLE_COLOR);
                        // Print(L"Attribute: "); 
                        // SetColor(SHOW_DATA_COLOR);
                        // Print(L"%s", VariableInfo.Attributes);
                    }
                }
                break;
        }
    } while(key.ScanCode != SCAN_ESC);

    return ;
} 