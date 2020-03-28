#include <SPI/usb1352_intr_thread.h>

#define INTR_TASK_STACK_SIZE 256
static Task_Params      intr_task_params;
static Task_Struct      intr_task_struct;
static uint8_t          intr_task_stack[INTR_TASK_STACK_SIZE];

static void usb1352_intr_thread(void* pArg);

void usb1352_intr_thread_init(sub1ghz_dev* my_dev)
{
    Task_Params_init(&intr_task_params);

    intr_task_params.stackSize = INTR_TASK_STACK_SIZE;
    intr_task_params.priority = 4;
    intr_task_params.stack = &intr_task_stack;
    intr_task_params.arg0 = (xdc_UArg)my_dev;

    Task_construct(&intr_task_struct, (ti_sysbios_knl_Task_FuncPtr)usb1352_intr_thread, &intr_task_params, NULL);
}

static void usb1352_intr_thread(void* pArg)
{
    sub1ghz_dev* my_dev = (sub1ghz_dev*)pArg;

    while (1)
    {
        {
            if (!(queue_empty(my_dev->rf_data_rx_queue)))
            {
//                SPI_transferCancel(my_dev->spi_handle);

                usb1352_interrupt();
            }
            Task_sleep(Clock_convertMsToTicks(50));
        }
    }
}
