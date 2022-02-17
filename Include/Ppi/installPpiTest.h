#ifndef _PONY_INSTALL_PPI_TEST_H__
#define _PONY_INSTALL_PPI_TEST_H__


#define PONY_TEST_PPI_GUID \
  { \
    0x95fa53dd, 0xac47, 0x41a1, { 0x63, 0x8b, 0x64, 0xd5, 0x7d, 0x4f, 0x5a, 0x71 }\
  }

typedef struct  _PONY_TEST_PPI  PONY_TEST_PPI;

typedef EFI_STATUS (EFIAPI *GET_PONY_INFO_PPI)(
  IN       PONY_TEST_PPI        *This
  );

struct _PONY_TEST_PPI {
  CHAR8              name[50];
  UINT32             age;
  UINT32             weight;
  UINT32             height;
  GET_PONY_INFO_PPI  getPpiInformation;
};

extern EFI_GUID gPonyTestPpiGuid;

#endif
