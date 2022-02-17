#ifndef _SMBUS_H_
#define _SMBUS_H_

#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include "Tool.h"

#define  BusMax      256
#define  DevMax      32
#define  FunMax      8
#define  Empity      0xFFFFFFFF
#define  ClassCode   0x0C0500

UINT8                                     SlaveAddress[256];
UINT32                                    SlaveAddressNum;
UINT8                                     SMBD[256];
UINT16                                    VendId;
UINT32                                    CursionX = 0;
UINT32                                    CursionY = 0;

#endif