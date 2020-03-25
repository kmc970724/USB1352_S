#ifndef __USB1352_RF_H__
#define __USB1352_RF_H__

#include <ti/drivers/rf/RF.h>
#include <ti/drivers/GPIO.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)
#include <usb1352.h>
#include "RFQueue.h"
#include "ti_radio_config.h"

#include "../frame/rf_frame.h"
#include "../frame/rf_protocol.h"
#include "../SPI/usb1352_spi.h"

#define MAX_PAYLOAD_LENGTH     254
#define DATA_ENTRY_HEADER_SIZE 8
#define NUM_DATA_ENTRIES       2
#define NUM_APPENDED_BYTES     2

#define DATA_SLOT_SIZE 100000

#define SUPER_FRAME_SIZE 100000

typedef struct rf_cc1352_frame
{
    int16_t rssi;
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD_LENGTH];
} rf_cc1352_frame;

enum { rf_50kbps, rf_200kbps, rf_300kbps, rf_400kbps, rf_500kbps, rf_1mbps, data_rate_count };

void usb1352_rf_init(sub1ghz_dev* my_dev);

#endif
