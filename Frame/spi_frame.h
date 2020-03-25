#ifndef __FRAME_H__
#define __FRAME_H__

#include <stdint.h>

#define SPI_BUF_SIZE 200

typedef struct spi_fc_type
{
    uint8_t frameType:3;
    uint8_t cmdType:2;
    uint8_t cmdSubType:3;
} spi_fc_type;

typedef struct spi_rfc
{
    uint8_t queue_size;
} spi_rfc;

typedef struct sub1ghz_spi_frame
{
    uint8_t sync1;
    uint8_t sync2;
    spi_fc_type fc;
    spi_rfc rfc;
    uint8_t tx_seq;
    uint8_t rx_seq;
    uint8_t length;
    uint8_t payload[SPI_BUF_SIZE];
} sub1ghz_spi_frame;

#endif
