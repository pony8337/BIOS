#include "ToolApp.h"

/*
  @param  old  	Old offset
  @param  new  	New offset
  @param  data 	the data's pointer

  @return new   offset
*/
UINTN 
MoveCursor (
  IN  UINTN         Old,
  IN  UINTN         New,
  IN  UINT8         *Data,
  IN  DISPLAY_MODE  DispalyMode
  )
{
	Data[Old] == 0xFF ? SetColor(NO_DATA_COLOR) : SetColor(SHOW_DATA_COLOR);
	gotoXY(BlockC_OffsetX(Old), BlockC_OffsetY(Old));
	Print(L"%02x ", Data[Old]);
  if(DispalyMode == DISPLAY_ASCII) ShowASCII(Old, Data[Old]);  

	SetColor(SHOW_CHOOSE_DATA);
	gotoXY(BlockC_OffsetX(New), BlockC_OffsetY(New));
	Print(L"%02x ", Data[New]);
  if(DispalyMode == DISPLAY_ASCII) ShowASCII(New, Data[New]);

	return New;
}


/*
  @param  Data  	    data buffer
  @param  InputData   input value
  @param  offset 	    data's offset

  @return Data    	  the data after modified
*/
UINT8
ChangeInputData (
  IN  UINT8  Data,
  IN  CHAR16 InputData,
  IN  UINTN  offset
  )
{
	if (InputData >= '0' && InputData <= '9') {
		Data = (Data << 4) | (UINT8)(InputData - '0');
		SetColor(INPUT_DATA_COLOR);
		gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
		Print(L"%02x ", Data);
		return Data;
	} else if (InputData >= 'a' && InputData <= 'f') {
		Data = (Data << 4) | (UINT8)(InputData - 'a' + 10);
		SetColor(INPUT_DATA_COLOR);
		gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
		Print(L"%02x ", Data);
		return Data;
	} else if (InputData >= 'A' && InputData <= 'F') {
		Data = (Data << 4) | (UINT8)(InputData - 'A' + 10);
		SetColor(INPUT_DATA_COLOR);
		gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
		Print(L"%02x ", Data);
		return Data;
	}
	return Data;
}

/*
  @param  SourceData data buffer.
  @param  DataSize   data size.
  @param  CurrPage 	 current data page. (one page 256)
  @param  DataArray  data array which show on the screen.
*/
VOID
UpdateArrayData (
  IN  	 UINT8  *SourceData,
  IN  	 UINTN  DataSize,
  IN  	 UINTN  CurrPage,
  IN OUT UINT8  *DataArray
  )
{
	UINTN  StartData;
  	UINTN  Index;
  	StartData = (CurrPage - 1) * 256;
  	for(Index = 0; Index < 256; Index++)
    	DataArray[Index] = (StartData + Index) < DataSize ? SourceData[StartData + Index] : 0xFF;
	return ;
}

VOID
ShowBlocCIndex ()
{
  UINTN Index;
  SetColor(EFI_BROWN);
  for(Index = 0; Index <= 0xF; Index++){
    gotoXY(BlockC_RowX(Index), BlockC_RowY(Index));
    Print(L"%02x", Index);
    gotoXY(BlockC_ColumnX(Index), BlockC_ColumnY(Index));
    Print(L"%01x0", Index);
  }
}


/*
  @param  DisplayMode	  Block D show information.
*/
VOID
ShowBlockDTitle (
  IN	  DISPLAY_MODE  DisplayMode
)
{
  DisplayMode == DISPLAY_ASCII ? SetColor(EFI_YELLOW) : SetColor(EFI_BROWN);
  gotoXY(BlockD_Info_X, BlockD_Info_Y);
  Print(L"  Text ");
  SetColor(EFI_BROWN);
  gotoXY(BlockD_Info_X + 7, BlockD_Info_Y);
  Print(L"/");
  DisplayMode == DISPLAY_INFO ? SetColor(EFI_YELLOW) : SetColor(EFI_BROWN);
  gotoXY(BlockD_Info_X + 8, BlockD_Info_Y);
  Print(L" Information ");
}

/*
  @param  DisplayMode	  Block D show information.
*/
VOID 
CleanBlockD (
  IN  DISPLAY_MODE  DisplayMode
)
{
  UINTN x;
  ShowBlockDTitle(DisplayMode);
  for(x = 1; x <= 17; x++) {
    gotoXY(BlockD_Info_X, BlockD_Info_Y + x); 
    CLEAN_BLOCK_D;
  }
  return ;
}

/*
  @param  Offset	data offset.
  @param  Data   	data.
*/
VOID 
ShowASCII (
  IN	  UINTN offset,
  IN    UINT8 data
  )
{
  gotoXY(offset % 16 + BlockD_Info_X + 3 , BlockC_OffsetY(offset)); 
  IsDigital(data) ? Print(L"%c", data) : Print(L".");
}


