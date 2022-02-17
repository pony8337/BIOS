#ifndef _CREATE_HOB_TEST_H
#define _CREATE_HOB_TEST_H

typedef struct {
    EFI_HOB_GUID_TYPE  hobInfo;
    CHAR8             *name;
    UINT32             weight;
    UINT32             height;
    UINT32             age;
} CREATE_HOB_INFO;


extern EFI_GUID gcreateHobTestGuid;

#endif