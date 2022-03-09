#include "ToolApp.h"

/*
  @param  old  	Old offset
  @param  new  	New offset
  @param  data 	the data's pointer

  @return new offset
*/
UINTN 
MoveCursor (
  IN  UINTN  Old,
  IN  UINTN  New,
  IN  UINT8  *Data
  )
{
	SetColor(EFI_LIGHTGRAY);
	gotoXY(BlockC_OffsetX(Old), BlockC_OffsetY(Old));
	Print(L"%02x ", Data[Old]);
	SetColor(EFI_WHITE);
	gotoXY(BlockC_OffsetX(New), BlockC_OffsetY(New));
	Print(L"%02x ", Data[New]);
	return New;
}


/*
  @param  Data  	 data buffer
  @param  InputData  input value
  @param  offset 	 data's offset

  @return Data    	 the data after modified
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
		SetColor(EFI_WHITE);
		gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
		Print(L"%02x ", Data);
		return Data;
	} else if (InputData >= 'a' && InputData <= 'f') {
		Data = (Data << 4) | (UINT8)(InputData - 'a' + 10);
		SetColor(EFI_WHITE);
		gotoXY(BlockC_OffsetX(offset), BlockC_OffsetY(offset));
		Print(L"%02x ", Data);
		return Data;
	} else if (InputData >= 'A' && InputData <= 'F') {
		Data = (Data << 4) | (UINT8)(InputData - 'A' + 10);
		SetColor(EFI_WHITE);
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
  IN 	 UINTN  CurrPage,
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