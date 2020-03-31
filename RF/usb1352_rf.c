#include "usb1352_rf.h"

#define RF_TASK_STACK_SIZE 4096
static Task_Params rf_task_params;
static Task_Struct rf_task_struct;
static uint8_t rf_task_stack[RF_TASK_STACK_SIZE];

static RF_Object rf_object;
static RF_Handle rf_handle;
static RF_Params rf_params;
static dataQueue_t data_queue;
static rfc_dataEntryGeneral_t* current_data_entry;
static rfc_propRxOutput_t rf_output;
#pragma DATA_ALIGN (rx_data_entry_buffer, 4);
static uint8_t rx_data_entry_buffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES, MAX_PAYLOAD_LENGTH, NUM_APPENDED_BYTES)];

static rf_cc1352_frame rx_frame;

uint32_t usb1352_rf_data_rate_table[data_rate_count][4] =
{
    { 0x8000, 0xF, 0x52, 0x64 },
    { 0x20000, 0xF, 0x5A, 0x12C },
    { 0x30000, 0xF, 0x5C, 0x1F4 },
    { 0x40000, 0xF, 0x5E, 0x26C },
    { 0x50000, 0xF, 0x60, 0x334 },
    { 0xA0000, 0xF, 0x64, 0x71C },
};

void usb1352_rf_thread_init(sub1ghz_dev* my_dev);
void usb1352_rf_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);

void usb1352_rf_transfer(void* frame, uint8_t type, uint8_t rx_addr);
uint16_t usb1352_rf_receive(void* frame, uint32_t size, uint32_t time);
uint8_t usb1352_check_frame(void* frame, uint8_t type);
void usb1352_rf_thread_init(sub1ghz_dev* my_dev);
void usb1352_rf_packet_rx(uint32_t rxTime);
void usb1352_rf_packet_tx(void* frame, uint32_t size);
void usb1352_rf_set_data_rate(uint8_t data_rate);

void usb1352_rf_thread(void* pArg)
{
    sub1ghz_dev* my_dev = (sub1ghz_dev*)pArg;

    sub1ghz_i_frame i_frame;
    sub1ghz_s_frame s_frame;
    sub1ghz_u_frame u_frame;

    sub1ghz_spi_frame spi_frame;

    uint8_t tx_seq = 0;
    uint8_t rx_seq = 0;

    uint8_t rx_data_frame_flag = 0;
    uint8_t tx_data_frame_flag = 0;

//    uint32_t start_time, end_time;

    usb1352_rf_set_data_rate(rf_1mbps);

    while (1)
    {
        usb1352_rf_receive(&u_frame, sizeof(sub1ghz_u_frame), 0);
        if (!(usb1352_check_frame(&u_frame, U_FRAME_BEACON)))
        {
            continue;
        }
        usb1352_rf_transfer(&u_frame, U_FRAME_UACK, 0xFF);

        usb1352_rf_receive(&u_frame, sizeof(sub1ghz_u_frame), DATA_SLOT_SIZE);
        if (!(usb1352_check_frame(&u_frame, U_FRAME_POLL_RR)))
        {
            continue;
        }
        tx_seq = u_frame.fc.u_code_2;
        s_frame.fc.rx_seq = rx_seq;
        usb1352_rf_transfer(&s_frame, S_FRAME_RR, 0xFF);

        GPIO_toggle(GPIO_LED_GREEN_LEFT);

        while (1)
        {
            if (!(queue_empty(my_dev->rf_data_tx_queue)))
            {
                memcpy(&spi_frame, &(my_dev->rf_data_tx_queue->buffer[my_dev->rf_data_tx_queue->front]), sizeof(sub1ghz_i_frame));

                i_frame.fc.tx_seq = tx_seq;
                i_frame.fc.rx_seq = rx_seq;
                i_frame.length = spi_frame.length;
                memcpy(i_frame.payload, spi_frame.payload, i_frame.length);

                Task_sleep(Clock_convertUsToTicks(1000));
                usb1352_rf_transfer(&i_frame, I_FRAME_, 0xFF);
                tx_data_frame_flag = 1;
                if (rx_data_frame_flag == 1)
                {
                    rx_data_frame_flag = 0;
                }
            }
            else
            {
                if (rx_data_frame_flag == 1)
                {
                    rx_data_frame_flag = 0;
                    s_frame.fc.rx_seq = rx_seq;
                    Task_sleep(Clock_convertUsToTicks(1000));
                    if (MAX_QUEUE_SIZE - my_dev->rf_data_rx_queue->size < 3)
                    {
                        usb1352_rf_transfer(&s_frame, S_FRAME_RNR, 0xFF);
                    }
                    else
                    {
                        usb1352_rf_transfer(&s_frame, S_FRAME_RR, 0xFF);
                    }
                }
            }

            usb1352_rf_receive(&i_frame, sizeof(sub1ghz_i_frame), DATA_SLOT_SIZE);
            memcpy(&s_frame, &i_frame, sizeof(sub1ghz_s_frame));
            if (usb1352_check_frame(&i_frame, I_FRAME_))
            {
                rx_data_frame_flag = 1;
                if (tx_data_frame_flag == 1)
                {
                    tx_data_frame_flag = 0;
                    Semaphore_pend(my_dev->spi_tx_sema_handle, BIOS_WAIT_FOREVER);
                    queue_remove(my_dev->rf_data_tx_queue);
                    Semaphore_post(my_dev->spi_tx_sema_handle);

                    tx_seq = (tx_seq + 1) % 0b1000;
                }
                rx_seq = i_frame.fc.tx_seq;
                rx_seq = (rx_seq + 1) % 0b1000;
                usb1352_transfer_to_host(my_dev, tx_seq, rx_seq, i_frame.length, i_frame.payload);
            }
            else if (usb1352_check_frame(&s_frame, S_FRAME_RR))
            {
                if (tx_data_frame_flag == 1)
                {
                    tx_data_frame_flag = 0;
                    Semaphore_pend(my_dev->spi_tx_sema_handle, BIOS_WAIT_FOREVER);
                    queue_remove(my_dev->rf_data_tx_queue);
                    Semaphore_post(my_dev->spi_tx_sema_handle);
                }
                rx_data_frame_flag = 0;
                tx_seq = (tx_seq + 1) % 0b1000;
            }
            else if (usb1352_check_frame(&s_frame, S_FRAME_RNR))
            {
                Semaphore_pend(my_dev->spi_tx_sema_handle, BIOS_WAIT_FOREVER);
                queue_remove(my_dev->rf_data_tx_queue);
                Semaphore_post(my_dev->spi_tx_sema_handle);
                Task_sleep(Clock_convertMsToTicks(10));
            }
        }
    }
}

void usb1352_rf_transfer(void* frame, uint8_t type, uint8_t rx_addr)
{
    sub1ghz_i_frame i_frame;
    sub1ghz_s_frame s_frame;
    sub1ghz_u_frame u_frame;

    if (type == I_FRAME_)
    {
        memcpy(&i_frame, frame, sizeof(sub1ghz_i_frame));

        i_frame.fc.id = I_FRAME_ID;
        i_frame.tx_addr = MY_ADDR;
        i_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&i_frame, sizeof(sub1ghz_i_frame));
    }
    else if (type == S_FRAME_RR)
    {
        memcpy(&s_frame, frame, sizeof(sub1ghz_s_frame));

        s_frame.fc.id = S_FRAME_ID;
        s_frame.fc.s_code = S_CODE_RR;
        s_frame.tx_addr = MY_ADDR;
        s_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&s_frame, sizeof(sub1ghz_s_frame));
    }
    else if (type == S_FRAME_REJ)
    {
        memcpy(&s_frame, frame, sizeof(sub1ghz_s_frame));

        s_frame.fc.id = S_FRAME_ID;
        s_frame.fc.s_code = S_CODE_REJ;
        s_frame.tx_addr = MY_ADDR;
        s_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&s_frame, sizeof(sub1ghz_s_frame));
    }
    else if (type == S_FRAME_RNR)
    {
        memcpy(&s_frame, frame, sizeof(sub1ghz_s_frame));

        s_frame.fc.id = S_FRAME_ID;
        s_frame.fc.s_code = S_CODE_RNR;
        s_frame.tx_addr = MY_ADDR;
        s_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&s_frame, sizeof(sub1ghz_s_frame));
    }
    else if (type == S_FRAME_SREJ)
    {
        memcpy(&s_frame, frame, sizeof(sub1ghz_s_frame));

        s_frame.fc.id = S_FRAME_ID;
        s_frame.fc.s_code = S_CODE_SREJ;
        s_frame.tx_addr = MY_ADDR;
        s_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&s_frame, sizeof(sub1ghz_s_frame));
    }
    else if (type == U_FRAME_BEACON)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_BEACON;
        u_frame.fc.u_code_2 = U_CODE_2_BEACON;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_UN_NUM_POLL)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_UN_NUM_POLL;
        u_frame.fc.u_code_2 = U_CODE_2_UN_NUM_POLL;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_POLL_RR)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_POLL_RR;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_UACK)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_UACK;
        u_frame.fc.u_code_2 = U_CODE_2_UACK;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_CHRC_REQ)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_CHRC_REQ;
        u_frame.fc.u_code_2 = U_CODE_2_CHRC_REQ;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_CHRC_ACK)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_CHRC_ACK;
        u_frame.fc.u_code_2 = U_CODE_2_CHRC_ACK;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_RESET)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_RESET;
        u_frame.fc.u_code_2 = U_CODE_2_RESET;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_SNRM)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_SNRM;
        u_frame.fc.u_code_2 = U_CODE_2_SNRM;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_SABM)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_SABM;
        u_frame.fc.u_code_2 = U_CODE_2_SABM;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
    else if (type == U_FRAME_EXID)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));

        u_frame.fc.id = U_FRAME_ID;
        u_frame.fc.u_code_1 = U_CODE_1_EXID;
        u_frame.fc.u_code_2 = U_CODE_2_EXID;
        u_frame.tx_addr = MY_ADDR;
        u_frame.rx_addr = rx_addr;

        usb1352_rf_packet_tx(&u_frame, sizeof(sub1ghz_u_frame));
    }
}

uint16_t usb1352_rf_receive(void* frame, uint32_t size, uint32_t time)
{
    memset(&rx_frame, 0, sizeof(rf_cc1352_frame));

    usb1352_rf_packet_rx(time);
    memcpy(frame, rx_frame.payload, size);

    return rx_frame.rssi;
}

uint8_t usb1352_check_frame(void* frame, uint8_t type)
{
    sub1ghz_i_frame i_frame;
    sub1ghz_s_frame s_frame;
    sub1ghz_u_frame u_frame;

    if (type == I_FRAME_)
    {
        memcpy(&i_frame, frame, sizeof(sub1ghz_i_frame));
        if (i_frame.fc.id == I_FRAME_ID && i_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == S_FRAME_RR)
    {
        memcpy(&s_frame, frame, sizeof(sub1ghz_s_frame));
        if (s_frame.fc.id == S_FRAME_ID && s_frame.fc.s_code == S_CODE_RR && s_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == S_FRAME_REJ)
    {
        memcpy(&s_frame, frame, sizeof(sub1ghz_s_frame));
        if (s_frame.fc.id == S_FRAME_ID && s_frame.fc.s_code == S_CODE_REJ && s_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == S_FRAME_RNR)
    {
        memcpy(&s_frame, frame, sizeof(sub1ghz_s_frame));
        if (s_frame.fc.id == S_FRAME_ID && s_frame.fc.s_code == S_CODE_RNR && s_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == S_FRAME_SREJ)
    {
        memcpy(&s_frame, frame, sizeof(sub1ghz_s_frame));
        if (s_frame.fc.id == S_FRAME_ID && s_frame.fc.s_code == S_CODE_SREJ && s_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_BEACON)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_BEACON && u_frame.fc.u_code_2 == U_CODE_2_BEACON && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_UN_NUM_POLL)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_UN_NUM_POLL && u_frame.fc.u_code_2 == U_CODE_2_UN_NUM_POLL && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_POLL_RR)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_POLL_RR && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_UACK)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_UACK && u_frame.fc.u_code_2 == U_CODE_2_UACK && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_CHRC_REQ)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_CHRC_REQ && u_frame.fc.u_code_2 == U_CODE_2_CHRC_REQ && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_CHRC_ACK)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_CHRC_ACK && u_frame.fc.u_code_2 == U_CODE_2_CHRC_ACK && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_RESET)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_RESET && u_frame.fc.u_code_2 == U_CODE_2_RESET && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_SNRM)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_SNRM && u_frame.fc.u_code_2 == U_CODE_2_SNRM && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_SABM)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_SABM && u_frame.fc.u_code_2 == U_CODE_2_SABM && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }
    else if (type == U_FRAME_EXID)
    {
        memcpy(&u_frame, frame, sizeof(sub1ghz_u_frame));
        if (u_frame.fc.id == U_FRAME_ID && u_frame.fc.u_code_1 == U_CODE_1_EXID && u_frame.fc.u_code_2 == U_CODE_2_EXID && u_frame.rx_addr == MY_ADDR)
        {
            return TRUE;
        }
    }

    return FALSE;
}

void usb1352_rf_init(sub1ghz_dev* my_dev)
{
    RF_Params_init(&rf_params);

    if (RFQueue_defineQueue(&data_queue, rx_data_entry_buffer, sizeof(rx_data_entry_buffer), NUM_DATA_ENTRIES, MAX_PAYLOAD_LENGTH + NUM_APPENDED_BYTES))
    {
        while (1);
    }

    RF_cmdPropRx.pQueue = &data_queue;
    RF_cmdPropRx.pOutput = (uint8_t*)&rf_output;
    RF_cmdPropRx.rxConf.bAutoFlushIgnored = 1;
    RF_cmdPropRx.rxConf.bAutoFlushCrcErr = 1;
    RF_cmdPropRx.pktConf.bRepeatOk = 0;
    RF_cmdPropRx.pktConf.bRepeatNok = 0;
    RF_cmdPropRx.maxPktLen = MAX_PAYLOAD_LENGTH;

    rf_handle = RF_open(&rf_object, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rf_params);
    RF_postCmd(rf_handle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    usb1352_rf_thread_init(my_dev);
}

void usb1352_rf_thread_init(sub1ghz_dev* my_dev)
{
    Task_Params_init(&rf_task_params);

    rf_task_params.stackSize = RF_TASK_STACK_SIZE;
    rf_task_params.priority = 2;
    rf_task_params.stack = &rf_task_stack;
    rf_task_params.arg0 = (xdc_UArg)my_dev;

    Task_construct(&rf_task_struct, (ti_sysbios_knl_Task_FuncPtr)usb1352_rf_thread, &rf_task_params, NULL);
}

void usb1352_rf_packet_rx(uint32_t rxTime)
{
    if (rxTime == 0)
    {
        RF_cmdPropRx.endTime = 0;
        RF_cmdPropRx.endTrigger.triggerType = TRIG_NEVER;
    }
    else
    {
        RF_cmdPropRx.endTime = RF_getCurrentTime() + RF_convertUsToRatTicks(rxTime);
        RF_cmdPropRx.endTrigger.triggerType = TRIG_ABSTIME;
    }

    RF_runCmd(rf_handle, (RF_Op*)&RF_cmdPropRx, RF_PriorityNormal, &usb1352_rf_callback, RF_EventRxEntryDone | RF_EventCmdDone);
}

void usb1352_rf_packet_tx(void* frame, uint32_t size)
{
    RF_cmdPropTx.pPkt = frame;
    RF_cmdPropTx.pktLen = size;

    RF_runCmd(rf_handle, (RF_Op*)&RF_cmdPropTx, RF_PriorityNormal, NULL, 0);
}

void usb1352_rf_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    if (e & RF_EventRxEntryDone)
    {
        GPIO_toggle(GPIO_PIN_DEBUG3);

        current_data_entry = RFQueue_getDataEntry();

        rx_frame.length = current_data_entry->data;
        memcpy(rx_frame.payload, (&(current_data_entry->data) + 1), rx_frame.length);
        rx_frame.rssi = rf_output.lastRssi;

        RFQueue_nextEntry();
    }
    else if (e & RF_EventCmdDone)
    {
        // pass;
    }
    else
    {
        memset(&rx_frame, 0, sizeof(rf_cc1352_frame));
    }
}

void usb1352_rf_set_data_rate(uint8_t data_rate)
{
    RF_close(rf_handle);

    RF_cmdPropRadioDivSetup.symbolRate.rateWord = usb1352_rf_data_rate_table[data_rate][0];
    RF_cmdPropRadioDivSetup.symbolRate.preScale = usb1352_rf_data_rate_table[data_rate][1];
    RF_cmdPropRadioDivSetup.rxBw = usb1352_rf_data_rate_table[data_rate][2];
    RF_cmdPropRadioDivSetup.modulation.deviation = usb1352_rf_data_rate_table[data_rate][3];

    rf_handle = RF_open(&rf_object, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rf_params);
    RF_postCmd(rf_handle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);
}
