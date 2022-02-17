#ifndef _PCI_H_
#define _PCI_H_
#include <Library/PciLib.h>
#include "Tool.h"

#define  NO_DEVICE   0xFFFFFFFF
#define  BusMax      256
#define  DevMax      32
#define  FunMax      8
#define  DEVICE_NUM  100

UINTN    PciDevNum = 0;
UINTN    CurPci    = 0;
UINT8    PciDevData[256];

typedef struct{
  UINT32  Bus;
  UINT32  Device;
  UINT32  Function;  
  UINT16  VendorID;    // 0x00
  UINT16  DeviceID;    // 0x02
  CHAR16  DeviceName[100];
} PCI_DEVICE_DESCRIPTOR;

PCI_DEVICE_DESCRIPTOR  PciDeviceList[DEVICE_NUM];

#endif