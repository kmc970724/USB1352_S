#include <usb1352.h>

#include "SPI/usb1352_spi.h"
#include "SPI/usb1352_intr_thread.h"
#include "RF/usb1352_rf.h"
#include "RF/usb1352_amp.h"

int main(void)
{
    Board_init();

    sub1ghz_dev* my_dev;
    Semaphore_Params spi_sema_params;

    my_dev = (sub1ghz_dev*)malloc(sizeof(sub1ghz_dev));

    my_dev->rf_data_rx_queue = (queue*)malloc(sizeof(queue));
    my_dev->rf_data_tx_queue = (queue*)malloc(sizeof(queue));

    queue_init(my_dev->rf_data_tx_queue);
    queue_init(my_dev->rf_data_rx_queue);

    GPIO_init();

    Semaphore_Params_init(&spi_sema_params);
    my_dev->spi_tx_sema_handle = Semaphore_create(1, &spi_sema_params, NULL);
    my_dev->spi_rx_sema_handle = Semaphore_create(1, &spi_sema_params, NULL);

    my_dev->cmd_event = Event_create(NULL, NULL);
    my_dev->intr_event = Event_create(NULL, NULL);

    usb1352_amp_init();
    usb1352_spi_init(my_dev);
    usb1352_rf_init(my_dev);
    usb1352_intr_thread_init(my_dev);

    BIOS_start();

    return 0;
}
