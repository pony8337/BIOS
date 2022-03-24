#ifndef TOOL_APP_H
#define TOOL_APP_H

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

//
// CMOS
// 
#define CMOS_PORT                   0x70
#define CMOS_VALUE                  0x71
#define RTC_SECONDs                 0x00
#define RTC_MINUTES                 0x02
#define RTC_HOURS                   0x04
#define RTC_DATE                    0x07
#define RTC_MONTH                   0x08
#define RTC_YEAR                    0x09

//
// Variable
//
#define MAX_VARIABLE_NUM            100
#define VARIABLE_LIST_Y             2
#define VARIABLE_LIST_NAME_X        0
#define VARIABLE_LIST_SIZE_X        36
#define VARIABLE_LIST_GUID_X        44

typedef struct {    
    CHAR16     VariableName[100];
    UINTN      VariableSize;
    CHAR16     Attributes[20];
    EFI_GUID   VendorGuid;
} VARIABLE_INFO;

/*
  @param  Name  	      Variable name
  @param  VendorGuid    Variable Guid pointer
  @param  VariableSize  Variable size pointer
  @param  Attr          Variable attribute pointer

  @return Data    	    Variable data
*/
VOID
*mGetVariable (
  IN  CHAR16              *Name,
  IN  EFI_GUID            *VendorGuid,
  OUT UINTN               *VariableSize,
  OUT UINT32              *Attr
);

/*
  @param  VariableInfo  Variable information
*/
VOID
VariableDetail (
  IN  VARIABLE_INFO VariableInfo
);


//
// Common Function
//
extern EFI_SYSTEM_TABLE             *gST;
extern EFI_BOOT_SERVICES            *gBS;
extern EFI_RUNTIME_SERVICES         *gRT;
       
#define SHOW_CHOOSE_DATA            EFI_WHITE
#define SHOW_DATA_COLOR             EFI_LIGHTGRAY   
#define NO_DATA_COLOR               EFI_DARKGRAY   

#define EFI_KEY_TAB                 0x09
#define CLEAN_DATA                  Print(L"                                                                                ")
#define DELAY_TIME                  10000000
#define SetColor(color)             gST->ConOut->SetAttribute(gST->ConOut, color)
#define gotoXY(x, y)                gST->ConOut->SetCursorPosition(gST->ConOut, x, y)
#define BLOCK_BOUNDARY              Print(L"********************************************************************************")

/*
     --------------------------------
     |               |              |
     |    Block A    |    Block B   |
     |               |              |
     ********************************
     |               |              |
     |               |              |
     |    Block C    |    Block D   |
     |               |              |
     |               |              |
     --------------------------------
*/

// Block A
#define BlockA_Function_Name_X      0
#define BlockA_Function_Name_Y      0
#define BlockA_Function_Detail_X    0
#define BlockA_Function_Detail_Y    1
#define BlockA_Boundary_X           0
#define BlockA_Boundary_Y           2


// Block B
#define BlockB_Page_Num_X           72
#define BlockB_Page_Num_Y           0
#define BlockB_Information_X        59
#define BlockB_Information_Y        1


// Block C
#define BlockC_RowX(offset)         3 * offset + 3
#define BlockC_RowY(offset)         3
#define BlockC_ColumnX(offset)      0
#define BlockC_ColumnY(offset)      offset + 4
#define BlockC_OffsetX(offset)      3 * (offset % 16) + 3
#define BlockC_OffsetY(offset)      (offset / 16) + 4


// Block D
#define BLOCKD_TITLE_COLOR          EFI_LIGHTRED
#define BlockD_Info_X               52
#define BlockD_Info_Y               3
#define BlockD_ShowASCII_X(offset)  offset % 16 + 54      
#define CLEAN_BLOCK_D               Print(L"                            ")


#define IsDigital(Data)             (BOOLEAN)\
                                    (Data >= 0x30 && Data <= 0x39) || \
                                    (Data >= 0x41 && Data <= 0x5A) || \
                                    (Data >= 0x61 && Data <= 0x7A)
typedef struct {
    UINT8   FunNo;
    VOID    (*FuncPtr)();
    CHAR16  *FunName;
} Tool_Menu;

typedef enum {
    DISPLAY_INFOR,
    DISPLAY_ASCII
} DISPLAY_MODE;

/*
  @param  old  	        Old offset
  @param  new  	        New offset
  @param  data 	        The data's pointer
  @param  DisplayMode   Show Block D or not

  @return new offset
*/
UINTN 
MoveCursor (
  IN  UINTN         Old,
  IN  UINTN         New,
  IN  UINT8         *Data,
  IN  DISPLAY_MODE  DispalyMode
);

/*
  @param  Data  	   Data buffer
  @param  InputData  Input value
  @param  offset 	   Data's offset

  @return Data    	 The data after modified
*/
UINT8
ChangeInputData (
  IN  UINT8  Data,
  IN  CHAR16 InputData,
  IN  UINTN  offset
);

/*
  @param  SourceData Data buffer.
  @param  DataSize   Data size.
  @param  CurrPage 	 Current data page. (one page 256)
  @param  DataArray  Data array which show on the screen.
*/
VOID
UpdateArrayData (
  IN  	 UINT8  *SourceData,
  IN  	 UINTN  DataSize,
  IN 	   UINTN  CurrPage,
  IN OUT UINT8  *DataArray
);

/*
  @param  DisplayMode	  Block D show information.
*/
VOID 
CleanBlockD (
  IN  DISPLAY_MODE  DisplayMode
);

/*
  @param  Offset	data offset.
  @param  Data   	data.
*/
VOID 
ShowASCII (
  IN    UINTN offset,
  IN    UINT8 data
);

// Tool Function
VOID CMOS();
VOID IOSpace();
VOID PCI();
VOID Memory();
VOID SMBus();
VOID EDID();
VOID SMBios();
VOID USBView();
VOID Variable();
VOID About();

// Tool Menu
Tool_Menu TMenu[] = {
  {0x00,     CMOS,        L"CMOS"},
  {0x01,  IOSpace,     L"IOSpace"},
  {0x02,      PCI,         L"PCI"},
  {0x03,   Memory,      L"Memory"},
  {0x04,    SMBus,       L"SMBus"},
  {0x05,     EDID,        L"EDID"},
  {0x06,   SMBios,      L"SMBios"},
  {0x07,  USBView,     L"USBView"},
  {0x08, Variable,    L"Variable"},
  {0x09,   About,        L"About"}
};

#endif