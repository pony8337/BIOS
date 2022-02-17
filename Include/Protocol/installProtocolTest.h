#ifndef __INSTALL_PROTOCOL_TEST_H__
#define __INSTALL_PROTOCOL_TEST_H__


#define PONY_TEST_PROTOCOL_GUID \
  { \
    0x7ffbabb6, 0x4b07, 0x4c21, { 0x22, 0xb3, 0x82, 0xc9, 0x75, 0xed, 0x86, 0x59 }\
  }

typedef struct _PONY_TEST_PROTOCOL  PONY_TEST_PROTOCOL;


typedef EFI_STATUS (EFIAPI *GET_PONY_INFO)(
  IN PONY_TEST_PROTOCOL *This
  );

struct _PONY_TEST_PROTOCOL {
  CHAR8           name[20];
  UINT32          age;
  UINT32          height;
  UINT32          weight;
  GET_PONY_INFO  getInformation;
};

extern EFI_GUID gPonyTestProtocolGuid;

#endif
