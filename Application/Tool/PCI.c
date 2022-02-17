#include "PCI.h"

CHAR16* 
Vendor(
  UINT16 VID
  )
{
  switch(VID){    
    case 0x8086:
    case 0x8087:    
      return L"Intel ";
      break;
    
    case 0x10DE:
      return L"NVIDIA ";
      break;
    
    case 0x10EC:
      return L"Realtek ";
      break;
    
    case 0x0402:
      return L"Acer aspire one ";
      break;

    case 0x046D:
      return L"Logitech Inc. ";
      break;

    case 0x04A9:
      return L"Canon ";
      break;

    case 0x04B3:
      return L"IBM ";
      break;

    case 0x3411:
      return L"Quantum ";  
      break;

    case 0x05AC:
      return L"Apple ";  
      break;

    default:
      return L"";
      break;
  }
}

CHAR16* 
DeviceType(
  UINT8 Base, 
  UINT8 Sub, 
  UINT8 ProgIF
  )
{
  switch(Base){
    case 0x00:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"None(Not for VGA-Compatible devices) ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"VGA-Compatible Device ";
      }else{
        return L"Unknow Device ";
      }
      break;
    case 0x01:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"SCSI Bus Controller ";
      }else if(Sub == 0x01){
        return L"IDE Controller ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"Floppy Disk Controller ";
      }else if(Sub == 0x03 && ProgIF == 0x00){
        return L"IPI Bus Controller ";
      }else if(Sub == 0x04 && ProgIF == 0x00){
        return L"RAID Controller ";
      }else if(Sub == 0x05 && ProgIF == 0x20){
        return L"ATA Controller (Single DMA) ";
      }else if(Sub == 0x05 && ProgIF == 0x30){
        return L"ATA Controller (Chained DMA) ";
      }else if(Sub == 0x06 && ProgIF == 0x00){
        return L"Serial ATA (Vendor Specific Interface) ";
      }else if(Sub == 0x06 && ProgIF == 0x01){
        return L"Serial ATA (AHCI 1.0) ";
      }else if(Sub == 0x07 && ProgIF == 0x00){
        return L"Serial Attached SCSI (SAS) ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Mass Storage Controller ";
      }else{
        return L"Mass Storage Controller ";
      }
      break;
    case 0x02:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"Ethernet Controller ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"Token Ring Controller ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"FDDI Controller ";
      }else if(Sub == 0x03 && ProgIF == 0x00){
        return L"ATM Controller ";
      }else if(Sub == 0x04 && ProgIF == 0x00){
        return L"ISDN Controller ";
      }else if(Sub == 0x05 && ProgIF == 0x00){
        return L"WorldFip Controller ";
      }else if(Sub == 0x06){
        return L"PICMG 2.14 Multi Computing ";
      }else if(Sub == 0x80 && 0x00){
        return L"Network Controller ";
      }else{
        return L"Network Controller ";
      }
      break;
    case 0x03:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"VGA-Compatible Controller ";
      }else if(Sub == 0x00 && ProgIF == 0x01){
        return L"8512-Compatible Controller ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"XGA Controller ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"3D Controller (Not VGA-Compatible) ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Display Controller ";
      }else{
        return L"Display Controller ";
      }
      break;
    case 0x04:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"Video Device ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"Audio Device ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"Computer Telephony Device ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Multimedia Device ";
      }else{
        return L"Multimedia Controller ";
      }
      break;
    case 0x05:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"RAM Controller ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"Flash Controller ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Memory Controller ";
      }else{
        return L"Memory controller ";
      }
      break;
    case 0x06:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"Host Bridge ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"ISA Bridge ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"EISA Bridge ";
      }else if(Sub == 0x03 && ProgIF == 0x00){
        return L"MCA Bridge ";
      }else if(Sub == 0x04 && ProgIF == 0x00){
        return L"PCI-to-PCI Bridge ";
      }else if(Sub == 0x04 && ProgIF == 0x01){
        return L"PCI-to-PCI Bridge (Subtractive Decode) ";
      }else if(Sub == 0x05 && ProgIF == 0x00){
        return L"PCMCIA Bridge ";
      }else if(Sub == 0x06 && ProgIF == 0x00){
        return L"NuBus Bridge ";
      }else if(Sub == 0x07 && ProgIF == 0x00){
        return L"CardBus Bridge ";
      }else if(Sub == 0x08){
        return L"RACEway Bridge ";
      }else if(Sub == 0x09 && ProgIF == 0x40){
        return L"PCI-to-PCI Bridge - Primary ";
      }else if(Sub == 0x09 && ProgIF == 0x80){
        return L"PCI-to-PCI Bridge - Secondary ";
      }else if(Sub == 0x0A && ProgIF == 0x00){
        return L"InfiniBrand-to-PCI Host Bridge ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Bridge Device ";
      }else{
        return L"Bridge ";
      }
      break;
    case 0x07:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"Generic XT-Compatible Serial Controller ";
      }else if(Sub == 0x00 && ProgIF == 0x01){
        return L"Compatible Serial Controller - 16450 ";
      }else if(Sub == 0x00 && ProgIF == 0x02){
        return L"Compatible Serial Controller - 16550 ";
      }else if(Sub == 0x00 && ProgIF == 0x03){
        return L"Compatible Serial Controller - 16650 ";
      }else if(Sub == 0x00 && ProgIF == 0x04){
        return L"Compatible Serial Controller - 16750 ";
      }else if(Sub == 0x00 && ProgIF == 0x05){
        return L"Compatible Serial Controller - 16850 ";
      }else if(Sub == 0x00 && ProgIF == 0x06){
        return L"Compatible Serial Controller - 16950 ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"Parallel Port ";
      }else if(Sub == 0x01 && ProgIF == 0x01){
        return L"Bi-Directional Parallel Port ";
      }else if(Sub == 0x01 && ProgIF == 0x02){
        return L"ECP 1.X Compliant Parallel Port ";
      }else if(Sub == 0x01 && ProgIF == 0x03){
        return L"IEEE 1284 Controller ";
      }else if(Sub == 0x01 && ProgIF == 0xFE){
        return L"IEEE 1284 Target Device ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"Multiport Serial Controller ";
      }else if(Sub == 0x03 && ProgIF == 0x00){
        return L"Generic Modem ";
      }else if(Sub == 0x03 && ProgIF == 0x01){
        return L"Hayes Compatible Modem - 16450 ";
      }else if(Sub == 0x03 && ProgIF == 0x02){
        return L"Hayes Compatible Modem - 16550 ";
      }else if(Sub == 0x03 && ProgIF == 0x03){
        return L"Hayes Compatible Modem - 16650 ";
      }else if(Sub == 0x03 && ProgIF == 0x04){
        return L"Hayes Compatible Modem - 16750 ";
      }else if(Sub == 0x04 && ProgIF == 0x00){
        return L"IEEE 488.1/2 (GPIB) Controller ";
      }else if(Sub == 0x05 && ProgIF == 0x00){
        return L"Smart Card ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Communications Device ";
      }else{
        return L"Communication controller ";
      }
      break;
    case 0x08:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"Generic 8259 PIC ";
      }else if(Sub == 0x00 && ProgIF == 0x01){
        return L"ISA PIC ";
      }else if(Sub == 0x00 && ProgIF == 0x02){
        return L"EISA PIC ";
      }else if(Sub == 0x00 && ProgIF == 0x10){
        return L"I/O APIC Interrupt Controller ";
      }else if(Sub == 0x00 && ProgIF == 0x20){
        return L"I/O(x) APIC Interrupt Controller ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"Generic 8237 DMA Controller ";
      }else if(Sub == 0x01 && ProgIF == 0x01){
        return L"ISA DMA Controller ";
      }else if(Sub == 0x01 && ProgIF == 0x02){
        return L"EISA DMA Controller ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"Generic 8254 System Timer ";
      }else if(Sub == 0x02 && ProgIF == 0x01){
        return L"ISA System Timer ";
      }else if(Sub == 0x02 && ProgIF == 0x02){
        return L"EISA System Timer ";
      }else if(Sub == 0x03 && ProgIF == 0x00){
        return L"Generic RTC Controller ";
      }else if(Sub == 0x03 && ProgIF == 0x01){
        return L"ISA RTC Controller ";
      }else if(Sub == 0x04 && ProgIF == 0x00){
        return L"Generic PCI Hot-Plug Controller ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"System Peripheral ";
      }else{
        return L"Generic system peripheral ";
      }
      break;
    case 0x09:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"iRDA Compatible Controller ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"Digitizer ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"Mouse Controller ";
      }else if(Sub == 0x03 && ProgIF == 0x00){
        return L"Scanner Controller ";
      }else if(Sub == 0x04 && ProgIF == 0x00){
        return L"Gameport Controller (Generic) ";
      }else if(Sub == 0x04 && ProgIF == 0x10){
        return L"Gameport Controller (Legacy) ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Input Controller ";
      }else{
        return L"Input device controller ";
      }
      break;
    case 0x0A:    
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"Generic Docking Station ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Docking Station ";
      }else{
        return L"Docking station ";
      }
      break;
    case 0x0B:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"386 Processor ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"486 Processor ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"Pentium Processor ";
      }else if(Sub == 0x10 && ProgIF == 0x00){
        return L"Alpha Processor ";
      }else if(Sub == 0x20 && ProgIF == 0x00){
        return L"PowerPC Processor ";
      }else if(Sub == 0x30 && ProgIF == 0x00){
        return L"MIPS Processor ";
      }else if(Sub == 0x40 && ProgIF == 0x00){
        return L"Co-Processor ";
      }else{
        return L"Processor ";
      }
      break;
    case 0x0C:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"IEEE 1394 Controller (FireWire) ";
      }else if(Sub == 0x00 && ProgIF == 0x10){
        return L"IEEE 1394 Controller (1394 OpenHCI Spec) ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"ACCESS.bus ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"SSA ";
      }else if(Sub == 0x03 && ProgIF == 0x00){
        return L"USB (Universal Host Controller Spec) ";
      }else if(Sub == 0x03 && ProgIF == 0x10){
        return L"USB (Open Host Controller Spec) ";
      }else if(Sub == 0x03 && ProgIF == 0x20){
        return L"USB2 Host Controller ";
      }else if(Sub == 0x03 && ProgIF == 0x30){
        return L"USB3 XHCI Controller ";
      }else if(Sub == 0x03 && ProgIF == 0x80){
        return L"Unspecified USB Controller ";
      }else if(Sub == 0xFE){
        return L"Fibre Channel USB ";
      }else if(Sub == 0x04 && ProgIF == 0x00){
        return L"Fibre Channel ";
      }else if(Sub == 0x05 && ProgIF == 0x00){
        return L"SMBus ";
      }else if(Sub == 0x06 && ProgIF == 0x00){
        return L"InfiniBand ";
      }else if(Sub == 0x07 && ProgIF == 0x00){
        return L"IPMI SMIC Interface ";
      }else if(Sub == 0x07 && ProgIF == 0x01){
        return L"IPMI Kybd Controller Style Interface ";
      }else if(Sub == 0x07 && ProgIF == 0x02){
        return L"IPMI Block Transfer Interface ";
      }else if(Sub == 0x08 && ProgIF == 0x00){
        return L"SERCOS Interface Standard (IEC 61491) ";
      }else{
        return L"Serial bus controller ";
      }
      break;
    case 0x0D:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"iRDA Compatible Controller";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"Consumer IR Controller ";
      }else if(Sub == 0x10 && ProgIF == 0x00){
        return L"RF Controller ";
      }else if(Sub == 0x11 && ProgIF == 0x00){
        return L"BlueTooth Controller ";
      }else if(Sub == 0x12 && ProgIF == 0x00){
        return L"Broadband Controller ";
      }else if(Sub == 0x20 && ProgIF == 0x00){
        return L"Ethernet Controller (802.11a) ";
      }else if(Sub == 0x21 && ProgIF == 0x00){
        return L"Ethernet Controller (802.11b) ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Wireless Controller ";
      }else{
        return L"Wireless controller ";
      }
      break;
    case 0x0E:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"Message FIFO ";
      }else if(Sub == 0x00){
        return L"I20 Architecture ";
      }else{
        return L"Intelligent controller ";
      }
      break;
    case 0x0F:
      if(Sub == 0x01 && ProgIF == 0x00){
        return L"TV Controller ";
      }else if(Sub == 0x02 && ProgIF == 0x00){
        return L"Audio Controller ";
      }else if(Sub == 0x03 && ProgIF == 0x00){
        return L"Voice Controller ";
      }else if(Sub == 0x04 && ProgIF == 0x0){
        return L"Data Controller";
      }else{
        return L"Satellite communications controller ";
      }
      break;
    case 0x10:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"Network,Computing Encrpytion/Decryption ";
      }else if(Sub == 0x10 && ProgIF == 0x00){
        return L"Entertainment Encryption/Decryption ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Encryption/Decryption ";
      }else{
        return L"Encryption controller ";
      }
      break;
    case 0x11:
      if(Sub == 0x00 && ProgIF == 0x00){
        return L"DPIO Modules ";
      }else if(Sub == 0x01 && ProgIF == 0x00){
        return L"Performance Counters ";
      }else if(Sub == 0x10 && ProgIF == 0x00){
        return L"Communications Syncrhonization ";
      }else if(Sub == 0x10 && ProgIF == 0x00){
        return L"Management Card ";
      }else if(Sub == 0x80 && ProgIF == 0x00){
        return L"Signal Processing Controller ";
      }else{
        return L"ignal processing controller ";
      }
      break;
    case 0x12:
      return L"Processing accelerators ";
      break;
    case 0x13:
      return L"Non-Essential Instrumentation ";
      break;
    case 0x40:
      return L"Coprocessor ";
      break;
    default:      
      return L"Unknow Device ";
      break;
  }
}

VOID 
ScanPciDev()
{
  UINT32      BusNum;
  UINT32      DevNum;
  UINT32      FunNum;

  PciDevNum = 0;

  for(BusNum = 0; BusNum < BusMax; BusNum++){
    for(DevNum = 0; DevNum < DevMax; DevNum++){
      for(FunNum = 0; FunNum < FunMax; FunNum++){
        if(PciRead32(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x00)) != NO_DEVICE){
          PciDeviceList[PciDevNum].Bus = (UINT8)BusNum;
          PciDeviceList[PciDevNum].Device = (UINT8)DevNum;
          PciDeviceList[PciDevNum].Function = (UINT8)FunNum;
          PciDeviceList[PciDevNum].VendorID = PciRead16(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x00));
          PciDeviceList[PciDevNum].DeviceID = PciRead16(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x02));
          //Device Name
          PciDeviceList[PciDevNum].DeviceName[0] = '\0';
          StrCat(PciDeviceList[PciDevNum].DeviceName, Vendor(PciDeviceList[PciDevNum].VendorID));
          StrCat(PciDeviceList[PciDevNum].DeviceName, DeviceType( PciRead8(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x0B)),   
                                                                  PciRead8(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x0A)), 
                                                                  PciRead8(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x09))
                                                                  )
                );
          PciDevNum++;
        }
      }
    }
  }
}

VOID
UpdatePciData(
  UINT8  Offset, 
  UINTN  CurPci
  )
{
  UINTN  RunNum;
  UINT8  PciTMP;
  for(RunNum = 0; RunNum <= 0xFF; RunNum++){
    PciTMP = PciRead8(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, (UINT8)RunNum));
    if(PciDevData[RunNum] != PciTMP){
      PciDevData[RunNum] = PciTMP;
      RunNum == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(RunNum), OffsetY(RunNum)+1);
      Print(L"%02x", PciDevData[RunNum]);
    } 
  }
  return;
}


VOID 
UpdatDetailData()
{
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 63, 4);
  Print(L"0x%04x", PciDeviceList[CurPci].VendorID);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 63, 5);
  Print(L"0x%04x", PciDeviceList[CurPci].DeviceID);
//BAR1
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 68, 6);
  Print(L"0x%08x", PciRead32(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, 0x10)));
//BAR2
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 68, 7);
  Print(L"0x%08x", PciRead32(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, 0x14)));
//BAR3
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 68, 8);
  Print(L"0x%08x", PciRead32(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, 0x18)));
//BAR4
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 68, 9);
  Print(L"0x%08x", PciRead32(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, 0x1C)));
//BAR5
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 68, 10);
  Print(L"0x%08x", PciRead32(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, 0x20)));
//BAR6
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 68, 11);
  Print(L"0x%08x", PciRead32(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, 0x24)));

  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 66, 12);
  Print(L"0x%04x", PciRead16(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, 0x2E)));
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 73, 13);
  Print(L"0x%04x", PciRead16(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, 0x2C)));
  return ;
}


VOID 
PciDataInit()
{
  UINTN  x = 0;
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  Print(L"# PCI # - B:0x%02x  D:0x%02x  F:0x%02x", PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 0);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGREEN);
  Print(L"[ F2: PCI Device List ]");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
  Print(L"                                                                                ");
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
  Print(L"[ Device: %s]", PciDeviceList[CurPci].DeviceName);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2);
  Print(L"********************************************************************************");
  // Print(L"                                                                                ");
  //Table offset
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, RowX(x), RowY(x)+1);
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, ColumnX(x), ColumnY(x)+1);
    Print(L"%01x0", x);
  }
  //Detail table
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 3);
  Print(L"[ PCI Device Information ]");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 4);
  Print(L"Vendor ID: ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 5);
  Print(L"Device ID: ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 6);
  Print(L"Address (BAR1): ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 7);
  Print(L"Address (BAR2): ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 8);
  Print(L"Address (BAR3): ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 9);
  Print(L"Address (BAR4): ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 10);
  Print(L"Address (BAR5): ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 11);
  Print(L"Address (BAR6): ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 12);
  Print(L"Subsystem ID: ");
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
  gST->ConOut->SetCursorPosition(gST->ConOut, 52, 13);
  Print(L"Subsystem Vendor ID: ");
  //PCI Data
  UpdatDetailData();
  for(x = 0; x <= 0xFF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x)+1);
    x == 0 ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    Print(L"%02x", PciDevData[x]);
  }
  return ;
}

//
//  Item  => Current Item
//  Next  => Next Item
//
VOID 
ChangePciListItem(  
  UINTN  *Item,
  UINTN   Next
  )
{
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, *Item%16+1);
  Print(L"%02x   %02x   %02x   %04x    %04x    %s", PciDeviceList[*Item].Bus, 
                                                    PciDeviceList[*Item].Device, 
                                                    PciDeviceList[*Item].Function, 
                                                    PciDeviceList[*Item].VendorID,
                                                    PciDeviceList[*Item].DeviceID,
                                                    PciDeviceList[*Item].DeviceName
                                                    );
  *Item = Next;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, *Item%16+1);
  Print(L"%02x   %02x   %02x   %04x    %04x    %s", PciDeviceList[*Item].Bus, 
                                                    PciDeviceList[*Item].Device, 
                                                    PciDeviceList[*Item].Function, 
                                                    PciDeviceList[*Item].VendorID,
                                                    PciDeviceList[*Item].DeviceID,
                                                    PciDeviceList[*Item].DeviceName
                                                    );
}

//
//  Page  => Current Page
//  Item  => Current Item
//  Next  => Next Item
//
VOID 
ChangePciListPage(
  UINTN  *Page, 
  UINTN  *Item,
  UINTN   Next
  )
{
  UINTN  IRun;
  UINTN  TMP;

  *Item = Next;
  *Page = Next/16;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
  Print(L"Bus  Dev  Fun  Vendor  Device  Name                                      [%02d/%02d]", *Page+1, PciDevNum/16+1);
  for(IRun = 0; IRun < 16; IRun++){
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, IRun+1);   
    TMP = IRun + 16 * *Page;       
    if(TMP < PciDevNum){
      Print(L"                                                                                ");         
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, IRun+1);
      TMP == *Item ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);          
      Print(L"%02x   %02x   %02x   %04x    %04x    %s", PciDeviceList[TMP].Bus, 
                                                        PciDeviceList[TMP].Device, 
                                                        PciDeviceList[TMP].Function, 
                                                        PciDeviceList[TMP].VendorID,
                                                        PciDeviceList[TMP].DeviceID,
                                                        PciDeviceList[TMP].DeviceName
                                                        );
    }else{
      Print(L"                                                                                "); //clean
    }
  }
}

VOID 
PciList()
{
  UINTN  Item = 0;
  UINTN  Page = 0;
  UINTN  IRun;
  UINTN  TMP;
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);

  Print(L"Bus  Dev  Fun  Vendor  Device  Name                                      [%02d/%02d]", Page+1, PciDevNum/16+1);    
  for(IRun = 0; IRun < 16; IRun++){
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, IRun+1);
    IRun == Item ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    Print(L"%02x   %02x   %02x   %04x    %04x    %s", PciDeviceList[IRun].Bus, 
                                                      PciDeviceList[IRun].Device, 
                                                      PciDeviceList[IRun].Function, 
                                                      PciDeviceList[IRun].VendorID,
                                                      PciDeviceList[IRun].DeviceID,
                                                      PciDeviceList[IRun].DeviceName
                                                      );
  }

  while(1){
    Key->ReadKeyStroke(Key, &KeyData);
    if(KeyData.ScanCode == SCAN_ESC){
      PciDataInit();
      break;

    }else if(KeyData.ScanCode == SCAN_UP || KeyData.ScanCode == SCAN_LEFT){      
      TMP = (Item + PciDevNum - 1) % PciDevNum;
      TMP/16 == Page ? ChangePciListItem(&Item, TMP) : ChangePciListPage(&Page, &Item, TMP);  

    }else if(KeyData.ScanCode == SCAN_DOWN || KeyData.ScanCode == SCAN_RIGHT){
      TMP = (Item + PciDevNum + 1) % PciDevNum;
      TMP/16 == Page ? ChangePciListItem(&Item, TMP) : ChangePciListPage(&Page, &Item, TMP); 

    }else if(KeyData.ScanCode == SCAN_NULL && KeyData.UnicodeChar == 0x0D){
      CurPci = Item;
      PciDataInit();
      break;

    }
  }
  return ;
}


//
//  Offset  => Current Offset
//  Next    => Next Offset
//
VOID
ChangePciDataOffset(  
  UINT8  *Offset,
  UINT8   Next
  )
{
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(*Offset), OffsetY(*Offset)+1);
  Print(L"%02x ", PciDevData[*Offset]);
  *Offset = Next;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
  gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(*Offset), OffsetY(*Offset)+1);
  Print(L"%02x ", PciDevData[*Offset]);
}

VOID
ChangePciDataPage(
  UINT8  Offset
  )
{
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  Print(L"# PCI # - B:0x%02x  D:0x%02x  F:0x%02x", PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
  Print(L"                                                                                ");
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
  Print(L"[ Device: %s]", PciDeviceList[CurPci].DeviceName);
  //Detail
  UpdatDetailData();
  //Update Pci Data
  UpdatePciData(Offset, CurPci);
}

VOID 
PCI()
{
  EFI_STATUS Status;
  UINT8       Offset = 0;
  UINT8       Input  = 0;
  UINT8       Next   = 0;
  UINTN       Index  = 0;  
  BOOLEAN     WriteF = FALSE;
  EFI_EVENT   TimerEvent;
  EFI_EVENT   WaitList[2];

  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
  Print(L"Please waiting~");
  ScanPciDev();
  CurPci = 0;
  gST->ConOut->ClearScreen(gST->ConOut);  
  PciDataInit();
  UpdatePciData(Offset, CurPci);

  while(1){
    Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
    //    
    // Set a timer event of 1 second expiration    
    //  
    Status = gBS->SetTimer (TimerEvent, TimerRelative, 10000000);
    //
    // Wait for the keystroke event or the timer    
    //  
    WaitList[0] = gST->ConIn->WaitForKey;  
    WaitList[1] = TimerEvent;
    
    Status = gBS->WaitForEvent (2, WaitList, &Index); 
    UpdatePciData(Offset, CurPci);
    gBS->CloseEvent (TimerEvent);

    Key->ReadKeyStroke(Key, &KeyData);
    
    if(!EFI_ERROR (Status) && Index == 1){   
      Status = EFI_TIMEOUT;  
      continue;
    } 

    if(KeyData.ScanCode == SCAN_ESC){       
      break;        
    }else if(KeyData.ScanCode == SCAN_UP){
      if(WriteF){
        WriteF = FALSE;
        Input = 0;
      }
      Next = (Offset + 256 - 16) % 256;
      ChangePciDataOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_DOWN){
      if(WriteF){
        WriteF = FALSE;
        Input = 0;
      }
      Next = (Offset + 256 + 16) % 256;
      ChangePciDataOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_LEFT){
      if(WriteF){
        WriteF = FALSE;
        Input = 0;
      }
      Next = (Offset + 256 - 1) % 256;
      ChangePciDataOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_RIGHT){     
      if(WriteF){
        WriteF = FALSE;
        Input = 0;
      }
      Next = (Offset + 256 + 1) % 256;
      ChangePciDataOffset(&Offset, Next);
    }else if(KeyData.ScanCode == SCAN_PAGE_UP){
      if(WriteF){
        WriteF = FALSE;
        Input = 0;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", PciDevData[Offset]);
      }
      CurPci = (CurPci + PciDevNum - 1) % PciDevNum;
      ChangePciDataPage(Offset);
    }else if(KeyData.ScanCode == SCAN_PAGE_DOWN){
      if(WriteF){
        WriteF = FALSE;
        Input = 0;
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", PciDevData[Offset]);
      }
      CurPci = (CurPci + PciDevNum + 1) % PciDevNum;
      ChangePciDataPage(Offset);
    }else if(KeyData.ScanCode == SCAN_F2){
      Offset = 0;
      if(WriteF){
        WriteF = FALSE;
        Input = 0;
      }
      PciList();
      UpdatePciData(Offset, CurPci);      
    }else if(KeyData.ScanCode == SCAN_NULL){
      if(KeyData.UnicodeChar >= '0' && KeyData.UnicodeChar <= '9'){
        WriteF = TRUE;
        Input = (Input<<4) | (UINT8)(KeyData.UnicodeChar - '0');
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", Input);

      }else if(KeyData.UnicodeChar >= 'a' && KeyData.UnicodeChar <= 'f'){
        WriteF = TRUE;
        Input = (Input<<4) | (UINT8)(KeyData.UnicodeChar - 'a' + 10);
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", Input);
      }else if(KeyData.UnicodeChar >= 'A' && KeyData.UnicodeChar <= 'F'){
        WriteF = TRUE;
        Input = (Input<<4) | (UINT8)(KeyData.UnicodeChar - 'A' + 10);
        gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
        gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+1);
        Print(L"%02x", Input);
      }else if(KeyData.UnicodeChar == 0x0D && WriteF){
        PciWrite8(PCI_LIB_ADDRESS(PciDeviceList[CurPci].Bus, PciDeviceList[CurPci].Device, PciDeviceList[CurPci].Function, Offset), Offset);
        Input = 0;
        WriteF = FALSE;
      }
    }
  }
  
  return ;
}