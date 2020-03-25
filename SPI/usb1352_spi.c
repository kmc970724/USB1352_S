#include "usb1352_spi.h"

#define SPI_TASK_STACK_SIZE 2048
static Task_Params      spi_task_params;
static Task_Struct      spi_task_struct;
static uint8_t          spi_task_stack[SPI_TASK_STACK_SIZE];

static SPI_Handle       spi_handle;
static SPI_Params       spi_params;
static SPI_Transaction  spi_trans;

void usb1352_spi_thread(void* pArg);
void usb1352_spi_thread_init(sub1ghz_dev* myDev);
void usb1352_spi_read_write(sub1ghz_spi_frame* tx_frame, sub1ghz_spi_frame* rx_frame);
void usb1352_spi_callback(SPI_Handle spi_handle, SPI_Transaction* spi_trans);

static Event_Handle usb1352_event_spi_handle;

void usb1352_spi_init(sub1ghz_dev* my_dev)
{
    SPI_init();
    SPI_Params_init(&spi_params);
    spi_params.mode         = SPI_SLAVE;
    spi_params.frameFormat  = SPI_POL1_PHA1;
    spi_params.transferMode = SPI_MODE_CALLBACK;
    spi_params.transferCallbackFxn = usb1352_spi_callback;

    spi_handle = SPI_open(0, &spi_params);
    my_dev->spi_handle = spi_handle;

    usb1352_event_spi_handle = Event_create(NULL, NULL);

    usb1352_spi_thread_init(my_dev);
}

void usb1352_spi_thread_init(sub1ghz_dev* my_dev)
{
    Task_Params_init(&spi_task_params);

    spi_task_params.stackSize = SPI_TASK_STACK_SIZE;
    spi_task_params.priority = 3;
    spi_task_params.stack = &spi_task_stack;
    spi_task_params.arg0 = (xdc_UArg)my_dev;

    Task_construct(&spi_task_struct, (ti_sysbios_knl_Task_FuncPtr)usb1352_spi_thread, &spi_task_params, NULL);
}

void usb1352_spi_read_write(sub1ghz_spi_frame* tx_frame, sub1ghz_spi_frame* rx_frame)
{
    tx_frame->sync1 = SPI_SYNC_BYTE1;
    tx_frame->sync2 = SPI_SYNC_BYTE2;

    spi_trans.txBuf = tx_frame;
    spi_trans.rxBuf = rx_frame;
    spi_trans.count = sizeof(sub1ghz_spi_frame);

    SPI_transfer(spi_handle, &spi_trans);
}

void usb1352_spi_callback(SPI_Handle spi_handle, SPI_Transaction* spi_trans)
{
    if (spi_trans->status == SPI_TRANSFER_COMPLETED)
    {
        Event_post(usb1352_event_spi_handle, usb1352_event_spi_complete);
    }
    else
    {
        Event_post(usb1352_event_spi_handle, usb1352_event_spi_cancel);
    }
}

void usb1352_spi_thread(void* pArg)
{
    sub1ghz_dev* my_dev = (sub1ghz_dev*)pArg;

    sub1ghz_spi_frame tx_frame;
    sub1ghz_spi_frame rx_frame;

    uint8_t transfer_flag = 0;
    uint8_t spi_event;

    while (1)
    {
        if (!(queue_empty(my_dev->rf_data_rx_queue)))
        {
            memcpy(&tx_frame, &(my_dev->rf_data_rx_queue->buffer[my_dev->rf_data_rx_queue->front]), sizeof(sub1ghz_spi_frame));

            transfer_flag = 1;
        }
        else
        {
            memset(&tx_frame, 0, sizeof(sub1ghz_spi_frame));
        }

        tx_frame.rfc.queue_size = MAX_QUEUE_SIZE - my_dev->rf_data_tx_queue->size;
        usb1352_spi_read_write(&tx_frame, &rx_frame);
        spi_event = Event_pend(usb1352_event_spi_handle, NULL, usb1352_event_spi_complete + usb1352_event_spi_cancel, BIOS_WAIT_FOREVER);

        if (spi_event & usb1352_event_spi_complete)
        {
            if (transfer_flag == 1)
            {
                GPIO_toggle(GPIO_PIN_DEBUG2);

                transfer_flag = 0;
                Semaphore_pend(my_dev->spi_rx_sema_handle, BIOS_WAIT_FOREVER);
                queue_remove(my_dev->rf_data_rx_queue);
                Semaphore_post(my_dev->spi_rx_sema_handle);
            }
        }
        else
        {
            continue;
        }

        if (rx_frame.sync1 == SPI_SYNC_BYTE1 && rx_frame.sync2 == SPI_SYNC_BYTE2)
        {
            if (rx_frame.fc.frameType != SPI_DUMMY)
            {
                Event_post(my_dev->intr_event, usb1352_event_intr);
            }

            if (rx_frame.fc.frameType == SPI_DATA_TRANSFER_REQ)
            {
                if (rx_frame.fc.cmdType == SPI_DATA_CMD_COMMON)
                {
                    if (rx_frame.fc.cmdSubType == SPI_DATA_CMD_COMMON_NORMAL)
                    {
                        GPIO_toggle(GPIO_PIN_DEBUG);
                        Semaphore_pend(my_dev->spi_tx_sema_handle, BIOS_WAIT_FOREVER);
                        queue_insert(my_dev->rf_data_tx_queue, &rx_frame);
                        Semaphore_post(my_dev->spi_tx_sema_handle);
                    }
                }
            }
            else if (rx_frame.fc.frameType == SPI_COMMAND_REQ)
            {
                if (rx_frame.fc.cmdType == SPI_CMD_RFSTATE)
                {
                    if (rx_frame.fc.cmdSubType == SPI_CMD_RFSTATE_RSSI)
                    {
                        Event_post(my_dev->cmd_event, usb1352_event_rssi);
                    }
                }
            }
        }
        memset(&tx_frame, 0, sizeof(sub1ghz_spi_frame));
        memset(&rx_frame, 0, sizeof(sub1ghz_spi_frame));
    }
}

//void usb1352_transfer_to_host(sub1ghz_dev* my_dev, uint8_t size, void* payload)
void usb1352_transfer_to_host(sub1ghz_dev* my_dev, uint8_t tx_seq, uint8_t rx_seq, uint8_t size, void* payload)
{
    sub1ghz_spi_frame spi_frame;

    spi_frame.fc.frameType = SPI_INTERRUPT;
    spi_frame.fc.cmdType = SPI_INTR_NORMAL;
    spi_frame.fc.cmdSubType = SPI_INTR_NORMAL_DATA;

    spi_frame.tx_seq = tx_seq;
    spi_frame.rx_seq = rx_seq;

    spi_frame.length = size;
    memcpy(spi_frame.payload, payload, spi_frame.length);

    Semaphore_pend(my_dev->spi_rx_sema_handle, BIOS_WAIT_FOREVER);
    queue_insert(my_dev->rf_data_rx_queue, &spi_frame);
    Semaphore_post(my_dev->spi_rx_sema_handle);
}
