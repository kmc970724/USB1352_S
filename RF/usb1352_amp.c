#include "usb1352_amp.h"

#include <ti/drivers/rf/RF.h>

#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

static PIN_Handle amp_pin_handle;
static PIN_State amp_pin_state;

PIN_Config amp_pin_config[] = {
    GPIO_PIN_2G4 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,      /* Path disabled */
    GPIO_PIN_RF_PA | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,      /* Path disabled */
    GPIO_PIN_SUB1GHZ | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,      /* Path disabled */
    PIN_TERMINATE
};

void usb1352_amp_init(void)
{
    amp_pin_handle = PIN_open(&amp_pin_state, amp_pin_config);

    PINCC26XX_setMux(amp_pin_handle, GPIO_PIN_2G4, PINCC26XX_MUX_GPIO);
    PINCC26XX_setMux(amp_pin_handle, GPIO_PIN_RF_PA, PINCC26XX_MUX_RFC_GPO3);
    PINCC26XX_setMux(amp_pin_handle, GPIO_PIN_SUB1GHZ, PINCC26XX_MUX_RFC_GPO0);
}
