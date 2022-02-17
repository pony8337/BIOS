#ifndef _DEVICELIST_H_
#define _DEVICELIST_H_

typedef struct {
    UINT8   FunNo;
    VOID    (*FuncPtr)();
    CHAR16  *FunName;
} Tool_Menu;

VOID CMOS();
VOID IOSpace();
VOID PCI();
VOID Memory();
VOID SMBus();
VOID Eeprom();
VOID EDID();
VOID SMBios();
VOID USBView();
VOID Variable();

Tool_Menu TMenu [] = {
  {0x00,     CMOS,          L"CMOS"},
  {0x01,  IOSpace,       L"IOSpace"},
  {0x02,      PCI,           L"PCI"},
  {0x03,   Memory,        L"Memory"},
  {0x04,    SMBus,         L"SMBus"},
  {0x05,   Eeprom,  L"CompalEeprom"},
  {0x06,     EDID,          L"EDID"},
  {0x07,   SMBios,        L"SMBios"},
  {0x08,  USBView,       L"USBView"},
  {0x09, Variable,      L"Variable"}
};


#endif