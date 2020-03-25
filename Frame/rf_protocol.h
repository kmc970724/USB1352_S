#ifndef __RF_PROTOCOL_H__
#define __RF_PROTOCOL_H__

// #define I_FRAME 0x1
#define I_FRAME_ 0x10

#define I_FRAME_ID 0b00

// #define S_FRAME 0x2
#define S_FRAME_RR 0x20
#define S_FRAME_REJ 0x21
#define S_FRAME_RNR 0x22
#define S_FRAME_SREJ 0x23

#define S_FRAME_ID 0b10

#define S_CODE_RR 0b00
#define S_CODE_REJ 0b01
#define S_CODE_RNR 0b10
#define S_CODE_SREJ 0b11

// #define U_FRAME 0x3
#define U_FRAME_BEACON 0x30
#define U_FRAME_UN_NUM_POLL 0x31
#define U_FRAME_POLL_RR 0x32
#define U_FRAME_UACK 0x34
#define U_FRAME_CHRC_REQ 0x35
#define U_FRAME_CHRC_ACK 0x36
#define U_FRAME_RESET 0x37
#define U_FRAME_SNRM 0x38
#define U_FRAME_SABM 0x39
#define U_FRAME_EXID 0x3A

#define U_FRAME_ID 0b11

#define U_CODE_1_BEACON 0b00
#define U_CODE_1_UN_NUM_POLL 0b00
#define U_CODE_1_POLL_RR 0b01
#define U_CODE_1_UACK 0b00
#define U_CODE_1_CHRC_REQ 0b10
#define U_CODE_1_CHRC_ACK 0b10
#define U_CODE_1_RESET 0b11
#define U_CODE_1_SNRM 0b11
#define U_CODE_1_SABM 0b11
#define U_CODE_1_EXID 0b11

#define U_CODE_2_BEACON 0b000
#define U_CODE_2_UN_NUM_POLL 0b100
#define U_CODE_2_UACK 0b110
#define U_CODE_2_CHRC_REQ 0b001
#define U_CODE_2_CHRC_ACK 0b010
#define U_CODE_2_RESET 0b001
#define U_CODE_2_SNRM 0b011
#define U_CODE_2_SABM 0b100
#define U_CODE_2_EXID 0b101

#endif
