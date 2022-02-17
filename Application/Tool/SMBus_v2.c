/*main.c */
#include "SMBus.h"

BOOLEAN 
CleanSMBusController(
  IN UINT32 Addr
  )
{
  UINT8  HST_STS;
  HST_STS = IoRead8(Addr);    
  IoWrite8(Addr, 0xFE);  
  
  while(HST_STS != 0x00){    
    HST_STS = IoRead8(Addr)&0x0F;
    gBS->Stall(5000);
  }    
  if(HST_STS == 0x00){
    return TRUE;
  }else{ 
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, 11);
    Print(L"Status = Clean Error!");
    return FALSE;
  }
}


EFI_STATUS
SMBusReadByteData(
  IN UINT32 Addr
  )
{
  UINT8   HST_CNT;
  UINT8   HST_STS;
  gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 1);
  Print(L"Wainting...");
  IoWrite8(Addr+0x02, 0x48); 
  do{  
    HST_STS = IoRead8(Addr)&0x04;
    if(HST_STS == 0x04)
       break;
    HST_STS = IoRead8(Addr)&0x02;
  }while(HST_STS != 0x02);


  gBS->Stall(5000);

  HST_CNT = IoRead8(Addr+0x02);
  if(HST_CNT != 0x08){
    return EFI_DEVICE_ERROR;
  }else{ 
    return EFI_SUCCESS;
  }

}


EFI_STATUS
ScanSlaveAddress(
  IN UINT32 Addr
  )
{
 
  UINT16      XMIT_SLAVA;
  UINT8       HST_STS;
  EFI_STATUS  Status;

  SlaveAddressNum = 0;  
  for(XMIT_SLAVA = 1; XMIT_SLAVA <= 0xFF; XMIT_SLAVA += 2){
      if(!CleanSMBusController(Addr)){
        Print(L"XMIT_SLAVA = %x", XMIT_SLAVA&0xFE);
        return EFI_DEVICE_ERROR;
      }
      //Slave address
      IoWrite8(Addr+0x04, (UINT8)XMIT_SLAVA);
      //offset
      IoWrite8(Addr+0x03, 0x00);
      //CMD  
      Status = SMBusReadByteData(Addr);
      if(Status == EFI_DEVICE_ERROR){
        Print(L"Device Error\n");
        return EFI_DEVICE_ERROR;
      }
      //check Status
      HST_STS = IoRead8(Addr)&0x06; 
      if(HST_STS == 0x06){
         continue;
      }else if(HST_STS == 0x02){
        SlaveAddress[SlaveAddressNum] = (UINT8)(XMIT_SLAVA&0xFE);
        SlaveAddressNum++;
      }
  }  
  return EFI_SUCCESS;
}


EFI_STATUS
SMBusData(
  IN UINT32 Addr,
  IN UINT8 SlaveAddress
  )
{
  UINT8      HST_STS;
  UINT32     Number;  
  EFI_STATUS Status;
  //initial Data
  for(Number = 0; Number < 256; Number++){
      SMBD[(UINT8)Number] = 0xFF;
    }
  //Slave address
  IoWrite8(Addr+0x04, SlaveAddress|0x01);

  for(Number = 0; Number <= 0xFF; Number++){
      //init controller 
      if(!CleanSMBusController(Addr)){
        return EFI_DEVICE_ERROR;
      }      
      //offset
      IoWrite8(Addr+0x03, (UINT8)Number);
      //CMD  
      Status = SMBusReadByteData(Addr);
      if(Status == EFI_DEVICE_ERROR){
        return EFI_DEVICE_ERROR;
      }else if(Status == EFI_TIMEOUT){
        return EFI_TIMEOUT;
      }  
      //check Status
      HST_STS = IoRead8(Addr)&0x0F; 
      if(HST_STS == 0x06 || HST_STS == 0x04){
         SMBD[(UINT8)Number] = 0xFF;
         continue;
      }else if(HST_STS == 0x02){
         SMBD[(UINT8)Number] = IoRead8(Addr+0x05);
      }else{
        SMBD[(UINT8)Number] = 0xF0|HST_STS;
      }
  }
  
   gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY);
   Print(L"#");
   gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY+1);               
   Print(L"# SMBus Data - SlaveAddress: 0x%02x  (F5 Flash)", SlaveAddress);
   gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY+2);               
   Print(L"#");

  for(Number = 0; Number < 256; Number++){
      if(Number>>4 == 0){       
        gST->ConOut->SetAttribute(gST->ConOut, 0x0C);
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX + 3*Number + 3, CursionY + 4);
        Print(L"%02x", Number);        
        if(Number == 0){
          gST->ConOut->SetCursorPosition(gST->ConOut, CursionX + 3*Number, CursionY + 4);
          Print(L"%02x", Number);       
        } 
      }
      if(Number%16 == 0){
        gST->ConOut->SetAttribute(gST->ConOut, 0x0C);
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 4 + (Number>>4) + 1);
        Print(L"%02x", Number);
      }
      //Set color
      if(SMBD[(UINT8)Number] == 0x00){
        gST->ConOut->SetAttribute(gST->ConOut, 0x07);
      }else if(SMBD[(UINT8)Number] == 0xFF){
        gST->ConOut->SetAttribute(gST->ConOut, 0x0F);
      }else{
        gST->ConOut->SetAttribute(gST->ConOut, 0x0E);
      }
      gST->ConOut->SetCursorPosition(gST->ConOut, CursionX + 3*(Number%16) + 3, CursionY + 4 + Number/16 + 1);
      Print(L"%02x", SMBD[(UINT8)Number]);
    }
    gST->ConOut->SetAttribute(gST->ConOut, 0x0F);

  while(1){
    Status = Key->ReadKeyStroke(Key, &KeyData);
    if(Status == EFI_SUCCESS){
     if(KeyData.ScanCode == SCAN_ESC){   
        gST->ConOut->ClearScreen(gST->ConOut);
        break;
     }else if(KeyData.ScanCode == SCAN_F5){
        gST->ConOut->ClearScreen(gST->ConOut);
        SMBusData(Addr, SlaveAddress);
        break;
     }
   }
 }
  return EFI_SUCCESS;
}


VOID
SlaveAddressMenu(
  IN UINT32 SMBAdd
  )
{ 
  UINT32      Number;
  UINT32      Choice;
  EFI_STATUS  Status;
  if(SlaveAddressNum <= 0){
     gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 4);
     Print(L"Sorry! The Deivce with SMBus is not ready yet!");
     do{
       Status = Key->ReadKeyStroke(Key, &KeyData);
     }while(Status != EFI_SUCCESS && KeyData.ScanCode != SCAN_ESC);
     return;
  }
  for(Number = 0; Number < SlaveAddressNum; Number++){
     gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 4 + Number);
     Number==0?gST->ConOut->SetAttribute(gST->ConOut, 0x0A):gST->ConOut->SetAttribute(gST->ConOut, 0x0F);
     Print(L"Device %d  - Slave Address: 0x%02x", Number+1, SlaveAddress[Number]);
  }
  gST->ConOut->SetAttribute(gST->ConOut, 0x0F);
  Choice = 0;

  while(1){
    Status = Key->ReadKeyStroke(Key, &KeyData);
    if(Status == EFI_SUCCESS){
     if(KeyData.ScanCode == SCAN_ESC){      
        break;
     }else if(KeyData.ScanCode == SCAN_DOWN || KeyData.ScanCode == SCAN_RIGHT){
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 4 + Choice);
        gST->ConOut->SetAttribute(gST->ConOut, 0x0F);
        Print(L"Device %d  - Slave Address: 0x%02x", Choice+1, SlaveAddress[Choice]);
        Choice = (Choice+SlaveAddressNum+1)%SlaveAddressNum;
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 4 + Choice);
        gST->ConOut->SetAttribute(gST->ConOut, 0x0A);
        Print(L"Device %d  - Slave Address: 0x%02x", Choice+1, SlaveAddress[Choice]);
        gST->ConOut->SetAttribute(gST->ConOut, 0x0F);

     }else if(KeyData.ScanCode == SCAN_UP || KeyData.ScanCode == SCAN_LEFT){
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 4 + Choice);
        gST->ConOut->SetAttribute(gST->ConOut, 0x0F);
        Print(L"Device %d  - Slave Address: 0x%02x", Choice+1, SlaveAddress[Choice]);
        Choice = (Choice+SlaveAddressNum-1)%SlaveAddressNum;
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 4 + Choice);
        gST->ConOut->SetAttribute(gST->ConOut, 0x0A);
        Print(L"Device %d  - Slave Address: 0x%02x", Choice+1, SlaveAddress[Choice]);
        gST->ConOut->SetAttribute(gST->ConOut, 0x0F);
     }else if(KeyData.ScanCode == SCAN_NULL && KeyData.UnicodeChar == 0x0D){  
        gST->ConOut->ClearScreen(gST->ConOut);    
        Status = SMBusData(SMBAdd, SlaveAddress[Choice]);
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY);
        Print(L"#");
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY+2);               
        Print(L"#");
        gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 1);
        VendId == 0x1022?Print(L"# AMD SMBus Controller: 0x%x ", SMBAdd):Print(L"# Intel SMBus Controller: 0x%x ", SMBAdd);
        for(Number = 0; Number < SlaveAddressNum; Number++){
            gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 4 + Number);
            Number==0?gST->ConOut->SetAttribute(gST->ConOut, 0x0A):gST->ConOut->SetAttribute(gST->ConOut, 0x0F);
            Print(L"Device %d  - Slave Address: 0x%02x", Number+1, SlaveAddress[Number]);
        }
        gST->ConOut->SetAttribute(gST->ConOut, 0x0F);
        Choice = 0;
     }
    }  
  }
  gST->ConOut->ClearScreen(gST->ConOut);
  return;
}


EFI_STATUS
SMBusController()
{
  
  UINT32      BusNum;
  UINT32      DevNum;
  UINT32      FunNum;
  UINT32      SMBAdd; 
  SlaveAddressNum = 0;  
  SMBAdd = Empity;
  //Scan Pci Device 
  for(BusNum = 0; BusNum < BusMax; BusNum++){
   for(DevNum = 0; DevNum < DevMax; DevNum++){
     if(PciRead32(PCI_LIB_ADDRESS(BusNum, DevNum, 0x00, 0x00)) == Empity)
       continue;
       for(FunNum = 0; FunNum < FunMax; FunNum++){                     
        if(PciRead32(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x09)) >> 8 == ClassCode){
            VendId = PciRead16(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x00));
            if(VendId == 0x1022){
               IoWrite8(0xCD6, 0x01);
               SMBAdd = (UINT32)IoRead8(0xCD7)<<8;              
               if(SMBAdd == Empity){
                  return EFI_UNSUPPORTED;
               }
               if(!CleanSMBusController(SMBAdd)){
                  return EFI_DEVICE_ERROR;
               }              
           
            }else if(VendId == 0x8086){
               SMBAdd = PciRead32(PCI_LIB_ADDRESS(BusNum, DevNum, FunNum, 0x20));
               SMBAdd = SMBAdd&0xFFFFFFFE;                 
               if(SMBAdd == Empity){
                  return EFI_UNSUPPORTED;
               }
               if(!CleanSMBusController(SMBAdd)){
                  return EFI_DEVICE_ERROR;
               }                      
            }else{
              Print(L"Error - VendId = %x", VendId);
              return EFI_DEVICE_ERROR;
            }
           }
         }
      }
  }

  //Check Slave Address
  ScanSlaveAddress(SMBAdd);

  gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY);
  Print(L"#");
  gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 1);               
  VendId==0x1022?Print(L"# AMD SMBus Controller: 0x%x, Total Device: %d", SMBAdd, SlaveAddressNum):Print(L"# Intel SMBus Controller: 0x%x, Total Device: %d", SMBAdd, SlaveAddressNum);
  gST->ConOut->SetCursorPosition(gST->ConOut, CursionX, CursionY + 2);               
  Print(L"#");
  SlaveAddressMenu(SMBAdd);

  return EFI_SUCCESS;
}


VOID
SMBus()
{ 
  EFI_STATUS  Status;

  // Clean full screan
  gST->ConOut->ClearScreen(gST->ConOut);
  // Disable show Curson  

  //SMBus Controller
  Status = SMBusController();
  if(Status == EFI_DEVICE_ERROR){
    Print(L"Device Error\n");    
  }else if(Status == EFI_UNSUPPORTED){
    Print(L"Devie Unsupported\n");
  }

  //clean full scran and leave
  gST->ConOut->ClearScreen(gST->ConOut);

  return ;
}

