#include "ToolApp.h"

VOID EDID()
{
    EFI_STATUS                      Status;
    EFI_INPUT_KEY                   key;
    UINTN                           Index;
    UINTN                           offset = 0;
    UINT8                           EdidData[256];
    // DISPLAY_MODE                    DisplayMode;
    UINTN                           CurrentEdid = 0;
    UINTN                           EdidHandleNum;
    EFI_HANDLE                      *EdidHandleBuffer;
    EFI_EDID_DISCOVERED_PROTOCOL    *EdidInfo;

        
    gST->ConOut->ClearScreen(gST->ConOut);

    // search EDID information
    gBS->LocateHandleBuffer(
            ByProtocol,
            &gEfiEdidDiscoveredProtocolGuid,
            NULL,
            &EdidHandleNum,
            &EdidHandleBuffer
        );
    
    gBS->OpenProtocol(
            EdidHandleBuffer[CurrentEdid],
            &gEfiEdidDiscoveredProtocolGuid,
            &EdidInfo,
            gImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL                        
        );
    // Block A
    SetColor(EFI_LIGHTGRAY);
    gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
    Print(L"# EDID #");
    gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
    Print(L"Screen: [ %d / %d ]", CurrentEdid + 1, EdidHandleNum);
    gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
    BLOCK_BOUNDARY;    
    // Block B
    gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
    Print(L"Page:1/1");

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
        // if(DisplayMode == DISPLAY_ASCII) ShowASCII(Index, EdidData[Index]);
    }
    
    
    do {
        gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
        Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
    } while(key.ScanCode != SCAN_ESC);

    return ;
}