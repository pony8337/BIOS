#ifndef GPIO_RPL_H
#define GPIO_RPL_H

// PCH Base Address
#define SBREG_BAR	   0xFD000000

// Target Point Id
#define COMMUNITY0     0x6E
#define COMMUNITY1     0x6D
#define COMMUNITY2     0x6C
#define COMMUNITY4     0x6A
#define COMMUNITY5     0x69

//
// Community 0
//
// GPP_B0 - GPP_B23
#define GPP_B_BASE     0x700
#define GPP_B_NUM      24
// GPP_T2 GPP_T3
#define GPP_T_BASE     0x8C0
#define GPP_T_NUM      2
// GPP_A0 - GPP_A23
#define GPP_A_BASE     0x9A0
#define GPP_A_NUM      24

//
// Community 1
//
// GPP_S0 - GPP_S7
#define GPP_S_BASE     0x700
#define GPP_S_NUM      8
// GPP_H0 - GPP_H23
#define GPP_H_BASE     0x780
#define GPP_H_NUM      24
// GPP_D0 - GPP_D19
#define GPP_D_BASE     0x900
#define GPP_D_NUM      20

//
// Community 2
//
// GPD_0 - GPD_11
#define GPD_BASE       0x700
#define GPD_NUM        12

//
// Community 4
//
// GPP_C0 - GPP_C23
#define GPP_C_BASE       0x700
#define GPP_C_NUM        24
// GPP_F0 - GPP_F23
#define GPP_F_BASE       0x880
#define GPP_F_NUM        24
// GPP_E0 - GPP_E23
#define GPP_E_BASE       0xA70
#define GPP_E_NUM        24

//
// Community 5
//
// GPP_R0 - GPP_R7
#define GPP_R_BASE       0x700
#define GPP_R_NUM        8

#define OFFSET_0         0
#define OFFSET_1         1
#define OFFSET_2         2
#define OFFSET_3         3
#define OFFSET_4         4
#define OFFSET_5         5
#define OFFSET_6         6
#define OFFSET_7         7

// OFFSET_0
#define TX_STATE_BIT     0x1
#define RX_STATE_BIT     0x2
// OFFSET_1
#define TX_DISABLE       0x1
#define RX_DISABLE       0x2
#define GPIO_FUNCTION    0x1C
// OFFSET_2
#define GPIO_NMI         0x2
#define GPIO_SMI         0x4
#define GPIO_SCI         0x8
#define GPIO_APIC        0x10
#define RX_INVERSE       0x80
// OFFSET_3
#define RX_LEVEL_EDGE    0x6
#define PAD_RESET        0xC0

// OFFSET_5
#define TERMINATION      0x3C

#define GPIO_ADDR(COMMUNITY, GPIO_BASE, GPIO)  SBREG_BAR | (COMMUNITY << 16) | ( GPIO_BASE + (GPIO << 4))

#endif