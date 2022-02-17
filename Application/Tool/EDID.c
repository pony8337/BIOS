#include "EDID.h"

EFI_STATUS 
EDIDInfo()
{
  EFI_STATUS                   Status;
  EFI_HANDLE                   *EDIDHandleBuffer;
  EFI_EDID_DISCOVERED_PROTOCOL *EDID;
  UINTN                        EDIDHandleCount;
  UINTN                        index;
  UINTN                        x = 0;
  UINT16                       ManufacturerId;
  UINTN                        Word1;
  UINTN                        Word2;
  UINTN                        Word3;
  UINT16                       Resolutionx = 0;
  UINT16                       Resolutiony = 0;
  UINTN                        Offset = 0;

  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);

  Status = gBS->LocateHandleBuffer( ByProtocol, 
                                    &gEfiEdidDiscoveredProtocolGuid, 
                                    NULL, 
                                    &EDIDHandleCount, 
                                    &EDIDHandleBuffer
                                   );  
  if(EFI_ERROR (Status)){
    Print(L"ERROR : Can't get EdidDiscoveredProtocol.\n");   
    return FALSE;
  }

// Total 
  index = 0;
  Print(L"EDID count = %d/%d\n", index+1, EDIDHandleCount);
    
  Status = gBS->OpenProtocol( EDIDHandleBuffer[index],
                              &gEfiEdidDiscoveredProtocolGuid,
                              (VOID**)&EDID,
                              gImageHandle,
                              NULL,
                              EFI_OPEN_PROTOCOL_GET_PROTOCOL
                            );
  if(EFI_ERROR (Status)){
    Print(L"ERROR : Open EDID Protocol failed.\n");   
    return FALSE;
  } 
  
  //Load data
  for(x = 0; x < EDIDNUM; x++){
    EDIDData[x] = EDID->Edid[x];
  }

  //Edid Information
  ManufacturerId = (UINT16)((EDIDData[8]<<8)|EDIDData[9]);
  Word1 = (ManufacturerId>>10)&0x1F;
  Word2 = (ManufacturerId>>5)&0x1F;
  Word3 = ManufacturerId&0x1F;  
  Resolutionx = ((UINT16)(EDIDData[0x3A]>>4)<<8)|(UINT16)EDIDData[0x38];
  Resolutiony = ((UINT16)(EDIDData[0x3D]>>4)<<8)|(UINT16)EDIDData[0x3B];
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);  
  Print(L"--------------------------------------------------");
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2);  
  Print(L"Manufacturer ID: 0x%04x / Manufacturer Name: %c%c%c",ManufacturerId, 'A'+Word1-1, 'A'+Word2-1, 'A'+Word3-1);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 3);  
  Print(L"Product Code: 0x%02x%02x / Resolution: %4d * %4d", EDIDData[11], EDIDData[10], Resolutionx, Resolutiony);
  gST->ConOut->SetCursorPosition(gST->ConOut, 0, 4);  
  Print(L"--------------------------------------------------");

  //Table offset  
  gST->ConOut->SetAttribute(gST->ConOut, EFI_BROWN);
  for(x = 0; x <= 0xF; x++){
    gST->ConOut->SetCursorPosition(gST->ConOut, RowX(x), RowY(x)+4);
    Print(L"%02x", x);
    gST->ConOut->SetCursorPosition(gST->ConOut, ColumnX(x), ColumnY(x)+4);
    Print(L"%01x0", x);
  }
  gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
  //show data
  for(x = 0; x < 256; x++){
    x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
    gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x)+4);
    Print(L"%02x ", EDIDData[x]);
  }

  //select which edid table
  while(1)
  {
    Key->ReadKeyStroke(Key, &KeyData);
    if(KeyData.ScanCode == SCAN_ESC){      
      Status = gBS->CloseProtocol( EDIDHandleBuffer[index], 
  	                               &gEfiEdidDiscoveredProtocolGuid, 
  	                               gImageHandle, 
  	                               NULL
  	                             ); 
      if(EFI_ERROR (Status)){
        Print(L"ERROR : Close EDID device handle failed.\n");   
        return FALSE;
      }
      break;  

    }else if(KeyData.ScanCode == SCAN_RIGHT){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+4);
      Print(L"%02x ", EDIDData[Offset]);
      Offset = (Offset + 256 + 1) % 256;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+4);
      Print(L"%02x ", EDIDData[Offset]);
    }else if(KeyData.ScanCode == SCAN_LEFT){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+4);
      Print(L"%02x ", EDIDData[Offset]);
      Offset = (Offset + 256 - 1) % 256;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+4);
      Print(L"%02x ", EDIDData[Offset]);      
    }else if(KeyData.ScanCode == SCAN_UP){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+4);
      Print(L"%02x ", EDIDData[Offset]);
      Offset = (Offset + 256 - 16) % 256;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+4);
      Print(L"%02x ", EDIDData[Offset]);      
    }else if(KeyData.ScanCode == SCAN_DOWN){
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+4);
      Print(L"%02x ", EDIDData[Offset]);
      Offset = (Offset + 256 + 16) % 256;
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(Offset), OffsetY(Offset)+4);
      Print(L"%02x ", EDIDData[Offset]);      
    }else if((KeyData.ScanCode == SCAN_PAGE_UP) && (EDIDHandleCount > 1)){
       Status = gBS->CloseProtocol( EDIDHandleBuffer[index], 
  	                                &gEfiEdidDiscoveredProtocolGuid, 
  	                                gImageHandle, 
  	                                NULL
  	                              ); 
      if(EFI_ERROR (Status)){
        Print(L"ERROR : Close EDID device handle failed.\n");   
        return FALSE;
      }
      index = (index+EDIDHandleCount-1)%EDIDHandleCount;
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
      Print(L"EDID count = %d/%d\n", index+1, EDIDHandleCount);
      Status = gBS->OpenProtocol( EDIDHandleBuffer[index],
                                  &gEfiEdidDiscoveredProtocolGuid,
                                  (VOID**)&EDID,
                                  gImageHandle,
                                  NULL,
                                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                );									
      if(EFI_ERROR(Status)){
        Print(L"Error Status = %r", Status);
      }
      
      for(x = 0; x < EDIDNUM; x++){
        if(EDIDData[x] != EDID->Edid[x]){
          EDIDData[x] = EDID->Edid[x];
          x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x)+4);
          Print(L"%02x ", EDIDData[x]);
        }
      }
      //Edid Information
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      ManufacturerId = (UINT16)((EDIDData[8]<<8)|EDIDData[9]);
      Word1 = (ManufacturerId>>10)&0x1F;
      Word2 = (ManufacturerId>>5)&0x1F;
      Word3 = ManufacturerId&0x1F;  
      Resolutionx = ((UINT16)(EDIDData[0x3A]>>4)<<8)|(UINT16)EDIDData[0x38];
      Resolutiony = ((UINT16)(EDIDData[0x3D]>>4)<<8)|(UINT16)EDIDData[0x3B];
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2);  
      Print(L"Manufacturer ID: 0x%04x / Manufacturer Name: %c%c%c",ManufacturerId, 'A'+Word1-1, 'A'+Word2-1, 'A'+Word3-1);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 3);  
      Print(L"Product Code: 0x%02x%02x / Resolution: %4d * %4d", EDIDData[11], EDIDData[10], Resolutionx, Resolutiony);
    }else if((KeyData.ScanCode == SCAN_PAGE_DOWN) && (EDIDHandleCount > 1)){
      Status = gBS->CloseProtocol( EDIDHandleBuffer[index], 
  	                               &gEfiEdidDiscoveredProtocolGuid, 
  	                               gImageHandle, 
  	                               NULL
  	                             ); 
      if(EFI_ERROR (Status)){
        Print(L"ERROR : Close EDID device handle failed.\n");   
        return FALSE;
      }

      index = (index+EDIDHandleCount+1)%EDIDHandleCount;
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 0);
      Print(L"EDID count = %d/%d\n", index+1, EDIDHandleCount);
      Status = gBS->OpenProtocol( EDIDHandleBuffer[index],
                                  &gEfiEdidDiscoveredProtocolGuid,
                                  (VOID**)&EDID,
                                  gImageHandle,
                                  NULL,
                                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                );
      if(EFI_ERROR(Status)){
        Print(L"Error Status = %r", Status);
      }
      
      for(x = 0; x < EDIDNUM; x++){
        if(EDIDData[x] != EDID->Edid[x]){
          EDIDData[x] = EDID->Edid[x];
          x == Offset ? gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE):gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
          gST->ConOut->SetCursorPosition(gST->ConOut, OffsetX(x), OffsetY(x)+4);
          Print(L"%02x ", EDIDData[x]);
        }
      }
      //Edid Information
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY);
      ManufacturerId = (UINT16)((EDIDData[8]<<8)|EDIDData[9]);
      Word1 = (ManufacturerId>>10)&0x1F;
      Word2 = (ManufacturerId>>5)&0x1F;
      Word3 = ManufacturerId&0x1F;  
      Resolutionx = ((UINT16)(EDIDData[0x3A]>>4)<<8)|(UINT16)EDIDData[0x38];
      Resolutiony = ((UINT16)(EDIDData[0x3D]>>4)<<8)|(UINT16)EDIDData[0x3B];
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 2);  
      Print(L"Manufacturer ID: 0x%04x / Manufacturer Name: %c%c%c",ManufacturerId, 'A'+Word1-1, 'A'+Word2-1, 'A'+Word3-1);
      gST->ConOut->SetCursorPosition(gST->ConOut, 0, 3);  
      Print(L"Product Code: 0x%02x%02x / Resolution: %4d * %4d", EDIDData[11], EDIDData[10], Resolutionx, Resolutiony);

    }
  }  
  return EFI_SUCCESS;    
}

VOID 
EDID()
{
  EFI_STATUS Status = EFI_SUCCESS;

  gST->ConOut->ClearScreen(gST->ConOut);
  Status = EDIDInfo();
  return ;
}