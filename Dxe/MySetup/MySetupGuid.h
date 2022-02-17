#ifndef MY_SETUP_GUID_H
#define MY_SETUP_GUID_H

typedef struct {
  UINT32  age;
  UINT32  heigh;
  UINT32  weight;
} MY_VARSTORE_CONFIGURATION;

#define PONY_SETUP_GUID \
  { \
    0x39AB1C25, 0xDB06, 0x4E16, {0x9F, 0xDE, 0xE7, 0x2C, 0xCE, 0x87, 0xA1, 0x32 } \
  }

extern EFI_GUID gPonySetupGuid;

#endif