#include "ToolApp.h"

UINTN scanUsbDevice()
{
    UINTN                               x;
    UINTN                               UsbNum;    
    EFI_STATUS                          Status;
    EFI_USB_IO_PROTOCOL                 *UsbDevice;
    EFI_HANDLE                          *UsbDeviceBuffer;    
    EFI_USB_DEVICE_DESCRIPTOR           UsbDeviceInfo;    
    EFI_DEVICE_PATH_PROTOCOL            *DevicePath;    // Device Path
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL    *DevicePathToText;   // Device Path to text

    ZeroMem(UsbDeviceList, USB_MAX_NUM * sizeof(USB_DEVICE_INFO));
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiUsbIoProtocolGuid,
                    NULL,
                    &UsbNum,
                    &UsbDeviceBuffer
                    );
    if(EFI_ERROR(Status)) {
        return 0;        
    }
    for(x = 0; x < UsbNum; x++) {
        gBS->OpenProtocol(
            UsbDeviceBuffer[x],
            &gEfiUsbIoProtocolGuid,
            &UsbDevice,
            gImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL                        
            );

        Status = UsbDevice->UsbGetDeviceDescriptor(UsbDevice, &UsbDeviceInfo);
        if(EFI_ERROR(Status)) continue;
        UsbDeviceList[x].VendorId = UsbDeviceInfo.IdVendor;
        UsbDeviceList[x].ProductId = UsbDeviceInfo.IdVendor;
        UsbDeviceList[x].DeviceClass = UsbDeviceInfo.DeviceClass;
        UsbDeviceList[x].DeviceSubClass = UsbDeviceInfo.DeviceSubClass;
        UsbDevice->UsbGetStringDescriptor(UsbDevice, EN_US, UsbDeviceInfo.StrManufacturer, &UsbDeviceList[x].Manufacturer);
        UsbDevice->UsbGetStringDescriptor(UsbDevice, EN_US, UsbDeviceInfo.StrProduct, &UsbDeviceList[x].Product);
        UsbDevice->UsbGetStringDescriptor(UsbDevice, EN_US, UsbDeviceInfo.StrSerialNumber, &UsbDeviceList[x].SerialNum);
        
        gBS->CloseProtocol(
            UsbDeviceBuffer[x],
            &gEfiUsbIoProtocolGuid,
            gImageHandle,
            NULL
            );
        
        Status = gBS->OpenProtocol(
                    UsbDeviceBuffer[x],
                    &gEfiDevicePathProtocolGuid,
                    &DevicePath,
                    gImageHandle,
                    NULL,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL                        
                    );
        if(EFI_ERROR(Status)) continue;

        gBS->LocateProtocol(
            &gEfiDevicePathToTextProtocolGuid,
            NULL,
            &DevicePathToText
            );
        UsbDeviceList[x].DevicePath = DevicePathToText->ConvertDevicePathToText(DevicePath, TRUE, TRUE);
        gBS->CloseProtocol(
            UsbDeviceBuffer[x],
            &gEfiDevicePathProtocolGuid,
            gImageHandle,
            NULL
            );        
        
    }
    return UsbNum;

}

VOID ShowUsbDevice(
    IN  UINTN   UsbNum,
    IN  UINTN   CurUsb
)
{
    // Block A        
    SetColor(NO_DATA_COLOR);
    gotoXY(BlockA_Function_Detail_X, BlockA_Function_Detail_Y);
    Print(L"[ USB Device: %d / %d ]", CurUsb+1, UsbNum);

    // Block B
    SetColor(NO_DATA_COLOR);
    gotoXY(BlockB_Page_Num_X , BlockB_Page_Num_Y);
    Print(L"Page:1/1");
    SetColor(EFI_LIGHTGREEN);
    gotoXY(BlockB_Information_X + 3, BlockB_Information_Y);
    Print(L"[ F5: Rescan USB ]");
    // Block C    
    SetColor(SHOW_DATA_COLOR);
    gotoXY(BlockC_ColumnX(0), BlockC_ColumnY(0));
    Print(L"Vendor: 0x%04x", UsbDeviceList[CurUsb].VendorId);
    gotoXY(BlockC_ColumnX(1), BlockC_ColumnY(1));
    Print(L"Product: 0x%04x", UsbDeviceList[CurUsb].ProductId);
    gotoXY(BlockC_ColumnX(2), BlockC_ColumnY(2));
    Print(L"Device Class: 0x%02x", UsbDeviceList[CurUsb].DeviceClass);
    gotoXY(BlockC_ColumnX(3), BlockC_ColumnY(3));
    Print(L"Device Sub Class: 0x%02x", UsbDeviceList[CurUsb].DeviceSubClass);
    gotoXY(BlockC_ColumnX(4), BlockC_ColumnY(4));
    CLEAN_DATA;
    gotoXY(BlockC_ColumnX(4), BlockC_ColumnY(4));
    Print(L"Manufacturer: %s", UsbDeviceList[CurUsb].Manufacturer);
    gotoXY(BlockC_ColumnX(5), BlockC_ColumnY(5));
    CLEAN_DATA;
    gotoXY(BlockC_ColumnX(5), BlockC_ColumnY(5));
    Print(L"Product: %s", UsbDeviceList[CurUsb].Product);
    gotoXY(BlockC_ColumnX(6), BlockC_ColumnY(6));
    CLEAN_DATA;
    gotoXY(BlockC_ColumnX(6), BlockC_ColumnY(6));
    Print(L"Serial Number: %s", UsbDeviceList[CurUsb].SerialNum);
    gotoXY(BlockC_ColumnX(7), BlockC_ColumnY(7));
    CLEAN_DATA;
    gotoXY(BlockC_ColumnX(7), BlockC_ColumnY(7));
    Print(L"USB Device PATH: %s", UsbDeviceList[CurUsb].DevicePath);
}

VOID USBView()
{
    UINTN           UsbNum;
    UINTN           CurUsb;
    UINTN           Index;
    EFI_INPUT_KEY   key;    
    gST->ConOut->ClearScreen(gST->ConOut);
    UsbNum = scanUsbDevice();
    CurUsb = 0;
    SetColor(NO_DATA_COLOR);
    gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
    Print(L"# USB Viewer #");    
    gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
    BLOCK_BOUNDARY;
    if(UsbNum == 0){
        SetColor(SHOW_DATA_COLOR);
        gotoXY(BlockC_ColumnX(0), BlockC_ColumnY(0));
        Print(L"No Usb Device~");
    } else {
        ShowUsbDevice(UsbNum, CurUsb);
    }
    do {
        gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
        gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
        switch(key.ScanCode){
            case SCAN_PAGE_UP:
                if(UsbNum == 0){
                    SetColor(SHOW_DATA_COLOR);
                    gotoXY(BlockC_ColumnX(0), BlockC_ColumnY(0));
                    Print(L"No Usb Device~");
                } else {
                    CurUsb = CurUsb == 0 ? UsbNum - 1 : CurUsb - 1;
                    ShowUsbDevice(UsbNum, CurUsb);
                }
                break;
            case SCAN_PAGE_DOWN:
                if(UsbNum == 0){
                    SetColor(SHOW_DATA_COLOR);
                    gotoXY(BlockC_ColumnX(0), BlockC_ColumnY(0));
                    Print(L"No Usb Device~");
                } else {
                    CurUsb = CurUsb + 1 == UsbNum ? 0 : CurUsb + 1;
                    ShowUsbDevice(UsbNum, CurUsb);
                }
                break;
            case SCAN_F5:
                UsbNum = scanUsbDevice();
                CurUsb = 0;
                gST->ConOut->ClearScreen(gST->ConOut);
                SetColor(NO_DATA_COLOR);
                gotoXY(BlockA_Function_Name_X, BlockA_Function_Name_Y);
                Print(L"# USB Viewer #");    
                gotoXY(BlockA_Boundary_X, BlockA_Boundary_Y);
                BLOCK_BOUNDARY;
                if(UsbNum == 0){
                    SetColor(SHOW_DATA_COLOR);
                    gotoXY(BlockC_ColumnX(0), BlockC_ColumnY(0));
                    Print(L"No Usb Device~");
                } else {
                    ShowUsbDevice(UsbNum, CurUsb);
                }
                break;

        }
       
    } while(key.ScanCode!=SCAN_ESC);
    return ;

}