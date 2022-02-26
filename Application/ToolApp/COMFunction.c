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
	gotoXY(offsetX(Old), offsetY(Old));
	Print(L"%02x ", Data[Old]);
	SetColor(EFI_WHITE);
	gotoXY(offsetX(New), offsetY(New));
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
		gotoXY(offsetX(offset), offsetY(offset));
		Print(L"%02x ", Data);
		return Data;
	} else if (InputData >= 'a' && InputData <= 'f') {
		Data = (Data << 4) | (UINT8)(InputData - 'a' + 10);
		SetColor(EFI_WHITE);
		gotoXY(offsetX(offset), offsetY(offset));
		Print(L"%02x ", Data);
		return Data;
	} else if (InputData >= 'A' && InputData <= 'F') {
		Data = (Data << 4) | (UINT8)(InputData - 'A' + 10);
		SetColor(EFI_WHITE);
		gotoXY(offsetX(offset), offsetY(offset));
		Print(L"%02x ", Data);
		return Data;
	}
	return Data;
}