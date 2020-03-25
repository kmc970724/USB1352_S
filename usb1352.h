#ifndef __USB1352_H__
#define __USB1352_H__

#include <unistd.h>
#include <stdint.h>

#include <ti/drivers/SPI.h>

#include <ti/sysbios/BIOS.h>
#include <ti/drivers/Board.h>
#include <ti/drivers/GPIO.h>

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>

#include "ti_drivers_config.h"

#include "Frame/spi_protocol.h"
#include "Frame/spi_frame.h"
#include "Queue/queue.h"

#define Clock_convertMsToTicks(milliseconds) (((milliseconds) * 1000) / Clock_tickPeriod)
#define Clock_convertUsToTicks(microseconds) ((microseconds) / Clock_tickPeriod)

#define MY_ADDR 0x01

typedef struct sub1ghz_dev
{
    queue* rf_data_tx_queue;
    queue* rf_data_rx_queue;

    Semaphore_Handle spi_tx_sema_handle;
    Semaphore_Handle spi_rx_sema_handle;

    Event_Handle intr_event;
    Event_Handle cmd_event;

    SPI_Handle spi_handle;
} sub1ghz_dev;

#endif
