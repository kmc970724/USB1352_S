#ifndef __USB1352_INTERRUPT_H__
#define __USB1352_INTERRUPT_H__

#include <ti/drivers/GPIO.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <usb1352.h>

#include "../Frame/usb1352_event.h"

void usb1352_intr_thread_init(sub1ghz_dev* my_dev);

#endif
