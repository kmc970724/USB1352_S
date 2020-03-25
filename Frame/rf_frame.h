#ifndef __RF_FRAME_H__
#define __RF_FRAME_H__

#include <stdint.H>

#define RF_BUF_SIZE 200

typedef struct i_frame_fc
{
    uint8_t id:2;
    uint8_t tx_seq:3;
    uint8_t rx_seq:3;
} i_frame_fc;

typedef struct s_frame_fc
{
    uint8_t id:2;
    uint8_t s_code:2;
    uint8_t p_f:1;
    uint8_t rx_seq:3;
} s_frame_fc;

typedef struct u_frame_fc
{
    uint8_t id:2;
    uint8_t u_code_1:2;
    uint8_t p_f:1;
    uint8_t u_code_2:3;
} u_frame_fc;

typedef struct sub1ghz_i_frame
{
    i_frame_fc fc;
    uint8_t tx_addr;
    uint8_t rx_addr;
    uint8_t length;
    uint8_t payload[RF_BUF_SIZE];
} sub1ghz_i_frame;

typedef struct sub1ghz_s_frame
{
    s_frame_fc fc;
    uint8_t tx_addr;
    uint8_t rx_addr;
} sub1ghz_s_frame;

typedef struct sub1ghz_u_frame
{
    u_frame_fc fc;
    uint8_t tx_addr;
    uint8_t rx_addr;
} sub1ghz_u_frame;

#endif
