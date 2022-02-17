#ifndef _USBVIEW_H_
#define _USBVIEW_H_

#include <Protocol/UsbIo.h>
#include <Protocol/DevicePathToText.h>
#include "Tool.h"

#define  USB_MAX_NUM  0xFF

typedef struct 
{
  UINT16  VendorId;
  UINT16  ProductId;
  UINT8   DeviceClass;
  UINT8   DeviceSubClass;
  CHAR16  *Manufacturer;
  CHAR16  *Product;
  CHAR16  *SerialNum;
  CHAR16  *DevicePath;  
}USB_DEVICE_INFO;

UINTN            USBDevNum = 0;
UINTN            CurUsbNum = 0;
USB_DEVICE_INFO  USBDevInfo[USB_MAX_NUM];

#endif