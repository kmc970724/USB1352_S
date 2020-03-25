/*
 *  ======== ti_drivers_config.c ========
 *  Configured TI-Drivers module definitions
 *
 *  DO NOT EDIT - This file is generated for the CC1352P1F3RGZ
 *  by the SysConfig tool.
 */

#include <stddef.h>

#ifndef DeviceFamily_CC13X2
#define DeviceFamily_CC13X2
#endif

#include <ti/devices/DeviceFamily.h>

#include "ti_drivers_config.h"


/*
 *  =============================== DMA ===============================
 */

#include <ti/drivers/dma/UDMACC26XX.h>
#include <ti/devices/cc13x2_cc26x2/driverlib/udma.h>
#include <ti/devices/cc13x2_cc26x2/inc/hw_memmap.h>

UDMACC26XX_Object udmaCC26XXObject;

const UDMACC26XX_HWAttrs udmaCC26XXHWAttrs = {
    .baseAddr        = UDMA0_BASE,
    .powerMngrId     = PowerCC26XX_PERIPH_UDMA,
    .intNum          = INT_DMA_ERR,
    .intPriority     = (~0)
};

const UDMACC26XX_Config UDMACC26XX_config[1] = {
    {
        .object         = &udmaCC26XXObject,
        .hwAttrs        = &udmaCC26XXHWAttrs,
    },
};


/*
 *  =============================== GPIO ===============================
 */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

/*
 *  ======== gpioPinConfigs ========
 *  Array of Pin configurations
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* GPIO_LED_RED */
    GPIOCC26XX_DIO_05 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
    /* GPIO_LED_GREEN_LEFT */
    GPIOCC26XX_DIO_06 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
    /* GPIO_LED_GREEN_RIGHT */
    GPIOCC26XX_DIO_07 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_HIGH,
    /* GPIO_PIN_DEBUG */
    GPIOCC26XX_DIO_20 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* GPIO_PIN_INTR */
    GPIOCC26XX_DIO_16 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* GPIO_PIN_DEBUG2 */
    GPIOCC26XX_DIO_19 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
    /* GPIO_PIN_DEBUG3 */
    GPIOCC26XX_DIO_18 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_MED | GPIO_CFG_OUT_LOW,
};

/*
 *  ======== gpioCallbackFunctions ========
 *  Array of callback function pointers
 *
 *  NOTE: Unused callback entries can be omitted from the callbacks array to
 *  reduce memory usage by enabling callback table optimization
 *  (GPIO.optimizeCallbackTableSize = true)
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    /* GPIO_LED_RED */
    NULL,
    /* GPIO_LED_GREEN_LEFT */
    NULL,
    /* GPIO_LED_GREEN_RIGHT */
    NULL,
    /* GPIO_PIN_DEBUG */
    NULL,
    /* GPIO_PIN_INTR */
    NULL,
    /* GPIO_PIN_DEBUG2 */
    NULL,
    /* GPIO_PIN_DEBUG3 */
    NULL,
};

/*
 *  ======== GPIOCC26XX_config ========
 */
const GPIOCC26XX_Config GPIOCC26XX_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = 7,
    .numberOfCallbacks = 7,
    .intPriority = (~0)
};


/*
 *  =============================== PIN ===============================
 */

#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

const PIN_Config BoardGpioInitTable[] = {
    /* Parent Signal: CONFIG_SPI_0 SCLK, (DIO23) */
    CONFIG_PIN_3 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* Parent Signal: CONFIG_SPI_0 MISO, (DIO25) */
    CONFIG_PIN_4 | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_DIS,
    /* Parent Signal: CONFIG_SPI_0 MOSI, (DIO24) */
    CONFIG_PIN_5 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* Parent Signal: GPIO_LED_RED GPIO Pin, (DIO5) */
    CONFIG_PIN_0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* Parent Signal: GPIO_LED_GREEN_LEFT GPIO Pin, (DIO6) */
    CONFIG_PIN_1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* Parent Signal: GPIO_LED_GREEN_RIGHT GPIO Pin, (DIO7) */
    CONFIG_PIN_2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* Parent Signal: GPIO_PIN_DEBUG GPIO Pin, (DIO20) */
    CONFIG_PIN_7 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* Parent Signal: GPIO_PIN_INTR GPIO Pin, (DIO16) */
    CONFIG_PIN_8 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* Parent Signal: CONFIG_SPI_0 SS, (DIO26) */
    CONFIG_PIN_6 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* GPIO_PIN_SUB1GHZ (DIO9) */
    GPIO_PIN_SUB1GHZ | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    /* GPIO_PIN_2G4 (DIO10) */
    GPIO_PIN_2G4 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    /* GPIO_PIN_RF_PA (DIO11) */
    GPIO_PIN_RF_PA | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    /* Parent Signal: GPIO_PIN_DEBUG2 GPIO Pin, (DIO19) */
    CONFIG_PIN_9 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,
    /* Parent Signal: GPIO_PIN_DEBUG3 GPIO Pin, (DIO18) */
    CONFIG_PIN_10 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MED,

    PIN_TERMINATE
};

const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = {
    .intPriority = (~0),
    .swiPriority = 0
};


/*
 *  =============================== Power ===============================
 */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include "ti_drivers_config.h"

extern void PowerCC26XX_standbyPolicy(void);
extern bool PowerCC26XX_calibrate(unsigned int);

const PowerCC26X2_Config PowerCC26X2_config = {
    .enablePolicy             = true,
    .policyInitFxn            = NULL,
    .policyFxn                = PowerCC26XX_standbyPolicy,
    .calibrateFxn             = PowerCC26XX_calibrate,
    .calibrateRCOSC_LF        = true,
    .calibrateRCOSC_HF        = true,
    .enableTCXOFxn            = NULL
};


/*
 *  =============================== RF Front-end ===============================
 */

/*
 * ======== Front-end control ========
 */
static PIN_Handle frontendPins;
static PIN_State frontendState;

void initRfFrontend(void)
{
    static bool initialized = false;

    if (!initialized) {
        PIN_Config frontendConfig[] = {
            GPIO_PIN_2G4 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,      /* Path disabled */
            GPIO_PIN_RF_PA | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,      /* Path disabled */
            GPIO_PIN_SUB1GHZ | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,      /* Path disabled */
            PIN_TERMINATE
        };
        frontendPins = PIN_open(&frontendState, frontendConfig);
        initialized = true;
    }
}

/*
 *  =============================== RF Driver ===============================
 */

#include <ti/drivers/rf/RF.h>

const RFCC26XX_HWAttrsV2 RFCC26XX_hwAttrs = {
    .hwiPriority        = (~0),
    .swiPriority        = (uint8_t)0,
    .xoscHfAlwaysNeeded = true,
    .globalCallback     = NULL,
    .globalEventMask    = RF_GlobalEventRadioPowerDown | RF_GlobalEventRadioSetup
};



/*
 *  =============================== SPI DMA ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC26X2DMA.h>

#define CONFIG_SPI_COUNT 1

/*
 *  ======== spiCC26X2DMAObjects ========
 */
SPICC26X2DMA_Object spiCC26X2DMAObjects[CONFIG_SPI_COUNT];

/*
 *  ======== spiCC26X2DMAHWAttrs ========
 */
const SPICC26X2DMA_HWAttrs spiCC26X2DMAHWAttrs[CONFIG_SPI_COUNT] = {
    /* CONFIG_SPI_0 */
    {
        .baseAddr = SSI0_BASE,
        .intNum = INT_SSI0_COMB,
        .intPriority = (~0),
        .swiPriority = 0,
        .powerMngrId = PowerCC26XX_PERIPH_SSI0,
        .defaultTxBufValue = ~0,
        .rxChannelBitMask = 1<<UDMA_CHAN_SSI0_RX,
        .txChannelBitMask = 1<<UDMA_CHAN_SSI0_TX,
        .minDmaTransferSize = 10,
        .mosiPin = IOID_24,
        .misoPin = IOID_25,
        .clkPin  = IOID_23,
        .csnPin  = IOID_26
    },
};

/*
 *  ======== SPI_config ========
 */
const SPI_Config SPI_config[CONFIG_SPI_COUNT] = {
    /* CONFIG_SPI_0 */
    {
        .fxnTablePtr = &SPICC26X2DMA_fxnTable,
        .object = &spiCC26X2DMAObjects[CONFIG_SPI_0],
        .hwAttrs = &spiCC26X2DMAHWAttrs[CONFIG_SPI_0]
    },
};

const uint_least8_t SPI_count = CONFIG_SPI_COUNT;


#include <ti/drivers/Board.h>

/*
 *  ======== Board_initHook ========
 *  Perform any board-specific initialization needed at startup.  This
 *  function is declared weak to allow applications to override it if needed.
 */
void __attribute__((weak)) Board_initHook(void)
{
}

/*
 *  ======== Board_init ========
 *  Perform any initialization needed before using any board APIs
 */
void Board_init(void)
{
    /* ==== /ti/drivers/Power initialization ==== */
    Power_init();

    /* ==== /ti/drivers/PIN initialization ==== */
    if (PIN_init(BoardGpioInitTable) != PIN_SUCCESS) {
        /* Error with PIN_init */
        while (1);
    }
    Board_initHook();
}
