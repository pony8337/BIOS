#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include "GpioRplH.h"

VOID GPIO_Info (UINTN config)
{

  UINTN   GpioNum = 0;
  UINTN   GpioAddr = 0;
  UINT8   PAD_MODE = 0;
  UINT8   Reset = 0;
  CHAR8   *GpioData = NULL; 
  BOOLEAN GPI = FALSE;
  BOOLEAN GPO = FALSE; 
  UINTN   GPIO_NUM;
  UINTN   GPIO_BASE;
  UINTN   Community;
  CHAR16 *GPIO_Group;
  
  switch(config) {
    case 0:
      GPIO_BASE = GPP_A_BASE;
      GPIO_NUM = GPP_A_NUM;
      Community = COMMUNITY0;
      GPIO_Group = L"GPP_A";
      break;
    case 1:
      GPIO_BASE = GPP_B_BASE;
      GPIO_NUM = GPP_B_NUM;
      Community = COMMUNITY0;
      GPIO_Group = L"GPP_B";
      break;
    case 2:
      GPIO_BASE = GPP_C_BASE;
      GPIO_NUM = GPP_C_NUM;
      Community = COMMUNITY4;
      GPIO_Group = L"GPP_C";
      break;
    case 3:
      GPIO_BASE = GPP_D_BASE;
      GPIO_NUM = GPP_D_NUM;
      Community = COMMUNITY1;
      GPIO_Group = L"GPP_D";
      break;
    case 4:
      GPIO_BASE = GPP_E_BASE;
      GPIO_NUM = GPP_E_NUM;
      Community = COMMUNITY4;
      GPIO_Group = L"GPP_E";
      break;
    case 5:
      GPIO_BASE = GPP_F_BASE;
      GPIO_NUM = GPP_F_NUM;
      Community = COMMUNITY4;
      GPIO_Group = L"GPP_F";
      break;
    case 6:
      GPIO_BASE = GPP_H_BASE;
      GPIO_NUM = GPP_H_NUM;
      Community = COMMUNITY1;
      GPIO_Group = L"GPP_H";
      break;
    case 7:
      GPIO_BASE = GPP_R_BASE;
      GPIO_NUM = GPP_R_NUM;
      Community = COMMUNITY5;
      GPIO_Group = L"GPP_R";
      break;
    case 8:
      GPIO_BASE = GPP_S_BASE;
      GPIO_NUM = GPP_S_NUM;
      Community = COMMUNITY1;
      GPIO_Group = L"GPP_S";
      break;
    case 9:
      GPIO_BASE = GPP_T_BASE;
      GPIO_NUM = GPP_T_NUM;
      Community = COMMUNITY0;
      GPIO_Group = L"GPP_T";
      break;
    default:
      GPIO_BASE = GPD_BASE;
      GPIO_NUM = GPD_NUM;
      Community = COMMUNITY2;
      GPIO_Group = L"GPD";
      break;
  }

  for(GpioNum = 0; GpioNum < GPIO_NUM; GpioNum++) {
    GpioData = 0;
    GpioAddr = GPIO_ADDR(Community, GPIO_BASE, GpioNum);
    Print(L"%s%02d: ", GPIO_Group, GpioNum);
    // Print(L" %d, ", (UINT8)*(GpioData + GpioAddr + OFFSET_1));
    PAD_MODE = ((UINT8)*(GpioData + GpioAddr + OFFSET_1) & GPIO_FUNCTION) >> 2;
    if(!PAD_MODE) {
      Print(L"GPIO Mode, ");
    } else {
      Print(L"Native Function %d\n", PAD_MODE);
      continue;
    }
    // GPI
    GPI = (BOOLEAN)((*(GpioData + GpioAddr + OFFSET_1) & RX_DISABLE) >> 1);
    GPI ? Print(L"GPI Disabled, ") : Print(L"GPI Enabled, ");
    (BOOLEAN)((*(GpioData + GpioAddr + OFFSET_0) & RX_STATE_BIT) >> 1) ? Print(L"Input High, ") : Print(L"Input Low, ");
    !GPI && (BOOLEAN)((*(GpioData + GpioAddr + OFFSET_1) & RX_INVERSE) >> 7) ? Print(L"Inversion, ") : Print(L"No Inversion, ");  
    // GPO
    GPO = (BOOLEAN)(*(GpioData + GpioAddr + OFFSET_1) & TX_DISABLE);
    GPO ? Print(L"GPO Disabled, ") : Print(L"GPO Enabled, ");
    (BOOLEAN)(*(GpioData + GpioAddr + OFFSET_0) & TX_STATE_BIT) ? Print(L"Output High, ") : Print(L"Output Low, ");
    // Reset
    Reset = (*(GpioData + GpioAddr + OFFSET_3) & PAD_RESET) >> 6;
    switch(Reset) {
      case 0:
        Print(L"Global Reset or RSMRST#.\n");
        break;
      case 1:
        Print(L"Host deep reset.\n");
        break;
      case 2:
        Print(L"PLTRST#.\n");
        break;
      case 3:
        Print(L"Global Reset.\n");
        break;
      default:
        Print(L"Reset: %d.\n", Reset);
        break;
    }

  }
}

INTN EFIAPI
ShellAppMain (
  IN UINTN Argc,
  IN CHAR16 **Argv
  )
{
  UINTN  gpio;
  UINTN  gpio_num = 11;
  // GPP_A  GPP_B GPP_C GPP_D GPP_E GPP_F GPP_H GPP_R GPP_S GPP_T GPD => 11 
  for(gpio = 0; gpio < gpio_num; gpio++) {
    GPIO_Info(gpio);
    Print(L"\n");
  }
 
  return 0;
}
