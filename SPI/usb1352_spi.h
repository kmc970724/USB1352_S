#ifndef __USB1352_SPI_H__
#define __USB1352_SPI_H__

#include <string.h>

#include <ti/drivers/SPI.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <usb1352.h>

#include "../Frame/usb1352_event.h"

#include "../Frame/spi_frame.h"
#include "../Frame/spi_protocol.h"

void usb1352_spi_init(sub1ghz_dev* myDev);
void usb1352_transfer_to_host(sub1ghz_dev* my_dev, uint8_t tx_seq, uint8_t rx_seq, uint8_t size, void* payload);
//void usb1352_transfer_to_host(sub1ghz_dev* my_dev, uint8_t size, void* payload);

#endif
