#include "USBView.h"

EFI_STATUS 
ScanUSBDevice() 
{	
  EFI_STATUS  Status;
  UINTN       x;
  EFI_USB_IO_PROTOCOL *UsbDevice = NULL;
  EFI_HANDLE *DeviceHandleBuffer = NULL;
  EFI_USB_DEVICE_DESCRIPTOR  UsbDeviceDescriptor; 
  //Device Path
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathProtocol = NULL;
  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DevicePath = NULL;
  

  Status = gBS->LocateHandleBuffer(
                ByProtocol,
                &gEfiUsbIoProtocolGuid,
                NULL,
                &USBDevNum,
                &DeviceHandleBuffer);

  if(Status != EFI_SUCCESS){
    Print(L"Get Handle Fail\n");
    return  EFI_UNSUPPORTED;
  }
  //Search USB Device
  for(x = 0; x < USBDevNum; x++){
    Status = gBS->HandleProtocol(
                    DeviceHandleBuffer[x],
                    &gEfiUsbIoProtocolGuid,
                    (VOID**)&UsbDevice
                  ); 
    
    if(Status != EFI_SUCCESS){
      Print(L"## Get USB Device Fail ##\n");
      continue;
    }    
    Status = UsbDevice->UsbGetDeviceDescriptor(UsbDevice, &UsbDeviceDescriptor);
  
    if(Status != EFI_SUCCESS){
      Print(L"## Get Usb Device Description Fail ##\n");  
      continue;
    }
    USBDevInfo[x].VendorId = UsbDeviceDescriptor.IdVendor;
    USBDevInfo[x].ProductId = UsbDeviceDescriptor.IdProduct;
    USBDevInfo[x].DeviceClass = UsbDeviceDescriptor.DeviceClass;
    USBDevInfo[x].DeviceSubClass = UsbDeviceDescriptor.DeviceSubClass;
    //get Manufacturer string
    USBDevInfo[x].Manufacturer = NULL;
    UsbDevice->UsbGetStringDescriptor(UsbDevice, 0x409, UsbDeviceDescriptor.StrManufacturer, &USBDevInfo[x].Manufacturer);     
    // Print(L"Manufacturer:  %s\n", String);
    // String = NULL;

    //get Product string
    USBDevInfo[x].Product = NULL;
    UsbDevice->UsbGetStringDescriptor(UsbDevice, 0x409, UsbDeviceDescriptor.StrProduct,  &USBDevInfo[x].Product);
    // Print(L"Product:       %s\n", String);
    // String = NULL;

    //get Serial Number
    USBDevInfo[x].SerialNum = NULL;
    UsbDevice->UsbGetStringDescriptor(UsbDevice, 0x409, UsbDeviceDescriptor.StrSerialNumber, &USBDevInfo[x].SerialNum);
    // Print(L"Serial Number: %s\n", String);
    // String = NULL;
    
    //get current handle Device Path Protocol
    Status = gBS->HandleProtocol(
                  DeviceHandleBuffer[x],
                  &gEfiDevicePathProtocolGuid,
                  (VOID**)&DevicePathProtocol);

    if(Status != EFI_SUCCESS){
      Print(L"## Get Device Path Protocol Fail ##\n");
      continue;
    }    
    Status = gBS->LocateProtocol(
                  &gEfiDevicePathToTextProtocolGuid,
                  NULL,
                  (VOID**)&DevicePath);
    USBDevInfo[x].DevicePath = DevicePath->ConvertDevicePathToText(DevicePathProtocol, TRUE, TRUE);
    // Print(L"Device Path:   %s\n", String);
    // String = NULL;

    // Print(L"\n\n");
  }
	return EFI_SUCCESS;
}

VOID
USBView()
{
  EFI_STATUS Status;
  UINTN      Flag = 1;
  //clean full scran
  gST->ConOut->ClearScreen(gST->ConOut);
  Status = ScanUSBDevice();
  if(Status != EFI_SUCCESS){
    Print(L"Error - %r", Status);    
    Flag = 0;
  }else{
  	gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
    Print(L"****         Usb Device Info [%d / %d]   (PageUp / PageDown)      ****", CurUsbNum + 1, USBDevNum);
    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2);
    Print(L"    Vendor: 0x%04x", USBDevInfo[CurUsbNum].VendorId);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 3);
    Print(L"    Product: 0x%04x", USBDevInfo[CurUsbNum].ProductId);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 4);
    Print(L"    Device Class: 0x%02x", USBDevInfo[CurUsbNum].DeviceClass);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 5);
    Print(L"    Device Sub Class: 0x%02x", USBDevInfo[CurUsbNum].DeviceSubClass);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 6);
    Print(L"                                                        ");
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 6);
    Print(L"    Manufacturer: %s", USBDevInfo[CurUsbNum].Manufacturer);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 7);
    Print(L"                                                        ");
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 7);
    Print(L"    Product: %s", USBDevInfo[CurUsbNum].Product);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 8);
    Print(L"                                                        ");
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 8);
    Print(L"    Serial Number: %s", USBDevInfo[CurUsbNum].SerialNum);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 9);
    Print(L"                                                        ");
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 9);
    Print(L"    PATH: %s", USBDevInfo[CurUsbNum].DevicePath);
    gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 11);
    Print(L"****         Usb Device Info [%d / %d]   (PageUp / PageDown)      ****", CurUsbNum + 1, USBDevNum);
  } 
  while(1){
  Status = Key->ReadKeyStroke(Key, &KeyData);
    if(KeyData.ScanCode == SCAN_ESC){ 
      break;        
    }else if(KeyData.ScanCode == SCAN_PAGE_UP){
      CurUsbNum == 0 ? (CurUsbNum = USBDevNum - 1) : CurUsbNum--;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
      Print(L"****         Usb Device Info [%d / %d]   (PageUp / PageDown)      ****", CurUsbNum + 1, USBDevNum);
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2);
      Print(L"    Vendor: 0x%04x", USBDevInfo[CurUsbNum].VendorId);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 3);
      Print(L"    Product: 0x%04x", USBDevInfo[CurUsbNum].ProductId);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 4);
      Print(L"    Device Class: 0x%02x", USBDevInfo[CurUsbNum].DeviceClass);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 5);
      Print(L"    Device Sub Class: 0x%02x", USBDevInfo[CurUsbNum].DeviceSubClass);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 6);
      Print(L"                                                        ");
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 6);
      Print(L"    Manufacturer: %s", USBDevInfo[CurUsbNum].Manufacturer);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 7);
      Print(L"                                                        ");
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 7);
      Print(L"    Product: %s", USBDevInfo[CurUsbNum].Product);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 8);
      Print(L"                                                        ");
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 8);
      Print(L"    Serial Number: %s", USBDevInfo[CurUsbNum].SerialNum);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 9);
      Print(L"                                                        ");
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 9);
      Print(L"    PATH: %s", USBDevInfo[CurUsbNum].DevicePath);
      gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 11);
      Print(L"****         Usb Device Info [%d / %d]   (PageUp / PageDown)      ****", CurUsbNum + 1, USBDevNum);
    }else if(KeyData.ScanCode  == SCAN_PAGE_DOWN){
      CurUsbNum == USBDevNum - 1 ? CurUsbNum = 0 : CurUsbNum++;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
      Print(L"****         Usb Device Info [%d / %d]   (PageUp / PageDown)      ****", CurUsbNum + 1, USBDevNum);
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2);
      Print(L"    Vendor: 0x%04x", USBDevInfo[CurUsbNum].VendorId);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 3);
      Print(L"    Product: 0x%04x", USBDevInfo[CurUsbNum].ProductId);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 4);
      Print(L"    Device Class: 0x%02x", USBDevInfo[CurUsbNum].DeviceClass);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 5);
      Print(L"    Device Sub Class: 0x%02x", USBDevInfo[CurUsbNum].DeviceSubClass);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 6);
      Print(L"                                                        ");
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 6);
      Print(L"    Manufacturer: %s", USBDevInfo[CurUsbNum].Manufacturer);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 7);
      Print(L"                                                        ");
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 7);
      Print(L"    Product: %s", USBDevInfo[CurUsbNum].Product);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 8);
      Print(L"                                                        ");
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 8);
      Print(L"    Serial Number: %s", USBDevInfo[CurUsbNum].SerialNum);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 9);
      Print(L"                                                        ");
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 9);
      Print(L"    PATH: %s", USBDevInfo[CurUsbNum].DevicePath);
      gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 11);
      Print(L"****         Usb Device Info [%d / %d]   (PageUp / PageDown)      ****", CurUsbNum + 1, USBDevNum);
    }
  }
  return ;
}