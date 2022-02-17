#include "tool.h"

#define MAX_VARIABLE_NUM 1000
extern EFI_RUNTIME_SERVICES	*gRT;


struct VARIABLE_INFO{    
  CHAR16  VariableName[100];
  UINTN   VariableSize;
  CHAR16  Attributes[20];
  EFI_GUID  VendorGuid;
}VariableList[MAX_VARIABLE_NUM];

UINT8  VariableData[256];
UINTN  VariableNum = 0;
UINTN  CurVariable = 0;
UINTN  Page;
UINTN  CurPage;