#include "ToolApp.h"

VOID About()
{
	EFI_INPUT_KEY  key;
	UINTN          Index;
	//clean full scream
	gST->ConOut->ClearScreen(gST->ConOut);
	gotoXY(5, 5);
	Print(L"=================================================");
	gotoXY(5, 6);
	Print(L"=            UEFI     R/W      Tool             =");
	gotoXY(5, 7);
	Print(L"=                Compal SPT/SW2                 =");
	gotoXY(5, 8);
	Print(L"=                   Pony Yang                   =");
	gotoXY(5, 9);
	Print(L"=              Pony_Yang@compal.com             =");
	gotoXY(5, 10);
	Print(L"=                 Version: 0.1                  =");
	gotoXY(5, 11);
	Print(L"=                    2022.03                    =");
	gotoXY(5, 12);
	Print(L"=================================================");
	do {
		gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &Index);
		gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
	} while(key.ScanCode != SCAN_ESC);  
	return;
}