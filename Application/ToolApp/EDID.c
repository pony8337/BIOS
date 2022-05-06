#include "ToolApp.h"

VOID EdidBlockAInfob (
    IN  UINT32   EdidSize
)
{
    SetColor(EFI_LIGHTGRAY);
    gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
    Print(L"# EDID #");
    gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
    Print(L"SizeOfEdid: 0x%2x", EdidSize);
    gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
    BLOCK_BOUNDARY;    
}

VOID EdidBlockBInfo (
    IN  UINTN   CurrentEdid,
    IN  UINTN   EdidNum
)
{
    gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
    Print(L"Page:1/1");
    gotoXY(BlockB_Information_X + 4, BlockB_Information_Y);
    Print(L"Screen: [ %d / %d ]", CurrentEdid + 1, EdidNum);
    gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
}

VOID EdidBlockCInfo (
    IN  UINTN                           offset,
    IN  EFI_EDID_DISCOVERED_PROTOCOL    *EdidInfo,
    OUT UINT8                           *EdidData,
    IN  DISPLAY_MODE                    DisplayMode
)
{
    UINTN   Index;
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
}
    

VOID EdidBlockDInfo (
    IN  UINT8   *EdidData
)
{
    UINT16                          ManufacturerName;
    UINT8                           ManufacturerName1;
    UINT8                           ManufacturerName2;
    UINT8                           ManufacturerName3;
    UINT16                          ResolutionX;
    UINT16                          ResolutionY;
  // Get detail information.
    ManufacturerName = (UINT16)(EdidData[8] << 8 | EdidData[9]);
    ManufacturerName1 = ((ManufacturerName >> 10) & 0x1F) + 64;
    ManufacturerName2 = ((ManufacturerName >> 5) & 0x1F) + 64;
    ManufacturerName3 = (ManufacturerName & 0x1F) + 64;
    ResolutionX = ((UINT16)(EdidData[0x3A] >> 4) << 8) | (UINT16)EdidData[0x38];
    ResolutionY = ((UINT16)(EdidData[0x3D] >> 4) << 8) | (UINT16)EdidData[0x3B];

    SetColor(BLOCKD_TITLE_COLOR);
    gotoXY(BlockD_Info_X, BlockD_Info_Y + 1);
    Print(L"Manufacturer Name: ");    
    SetColor(SHOW_DATA_COLOR);
    Print(L"%c%c%c", ManufacturerName1, ManufacturerName2, ManufacturerName3);
    SetColor(BLOCKD_TITLE_COLOR);
    gotoXY(BlockD_Info_X, BlockD_Info_Y + 2);
    Print(L"Manufacturer ID: ");    
    SetColor(SHOW_DATA_COLOR);
    Print(L"0x%04x", ManufacturerName);
    SetColor(BLOCKD_TITLE_COLOR);
    gotoXY(BlockD_Info_X, BlockD_Info_Y + 3);
    Print(L"Product Code: ");    
    SetColor(SHOW_DATA_COLOR);
    Print(L"0x%02x%02x", EdidData[11], EdidData[10]);
    SetColor(BLOCKD_TITLE_COLOR);
    gotoXY(BlockD_Info_X, BlockD_Info_Y + 4);
    Print(L"Resolution: ");    
    SetColor(SHOW_DATA_COLOR);
    Print(L"%4d * %4d", ResolutionX, ResolutionY);

}

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
        
    gST->ConOut->ClearScreen(gST->ConOut);
    DisplayMode = DISPLAY_INFO;
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
    EdidBlockAInfob(EdidInfo->SizeOfEdid);
    // Block B
    EdidBlockBInfo (CurrentEdid, EdidHandleNum);
    // Block C    
    EdidBlockCInfo (offset, EdidInfo, EdidData, DisplayMode);    
    // Block D
    ShowBlockDTitle(DisplayMode);
    
    EdidBlockDInfo(EdidData);
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
            case SCAN_PAGE_UP:
                // DEBUG ((EFI_D_INFO, "\nSCAN_PAGE_UP\n"));
                if(EdidHandleNum == 1) break;                
                break;
            case SCAN_PAGE_DOWN:
                // DEBUG ((EFI_D_INFO, "\nSCAN_PAGE_DOWN\n"));
                if(EdidHandleNum == 1) break;                
                break;
            case SCAN_NULL:
                // TAB
                if(key.UnicodeChar == EFI_KEY_TAB) { 
                    // display mode
                    if(DisplayMode == DISPLAY_INFO) {
                        DisplayMode = DISPLAY_ASCII;            
                        CleanBlockD(DisplayMode);
                        for(Index = 0; Index <= 0xFF; Index++) {
                            Index == offset ? SetColor(SHOW_CHOOSE_DATA) : (EdidData[Index] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR));
                            ShowASCII(Index, EdidData[Index]);
                        }
                    } else if(DisplayMode == DISPLAY_ASCII) {
                        DisplayMode = DISPLAY_INFO;
                        CleanBlockD(DisplayMode);
                        EdidBlockDInfo(EdidData);
                    }
                }
                break;
        }
    } while(key.ScanCode != SCAN_ESC);

    return ;
} 