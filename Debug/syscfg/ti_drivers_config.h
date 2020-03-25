/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  DO NOT EDIT - This file is generated for the CC1352P1F3RGZ
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_CC1352P1F3RGZ

#ifndef DeviceFamily_CC13X2
#define DeviceFamily_CC13X2
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== GPIO ========
 */

/* DIO5 */
#define GPIO_LED_RED                0
/* DIO6 */
#define GPIO_LED_GREEN_LEFT         1
/* DIO7 */
#define GPIO_LED_GREEN_RIGHT        2
/* DIO20 */
#define GPIO_PIN_DEBUG              3
/* DIO16 */
#define GPIO_PIN_INTR               4
/* DIO19 */
#define GPIO_PIN_DEBUG2             5
/* DIO18 */
#define GPIO_PIN_DEBUG3             6

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== PIN ========
 */

/* Includes */
#include <ti/drivers/PIN.h>

/* Externs */
extern const PIN_Config BoardGpioInitTable[];

/* Parent Signal: CONFIG_SPI_0 SCLK, (DIO23) */
#define CONFIG_PIN_3    0x00000017
/* Parent Signal: CONFIG_SPI_0 MISO, (DIO25) */
#define CONFIG_PIN_4    0x00000019
/* Parent Signal: CONFIG_SPI_0 MOSI, (DIO24) */
#define CONFIG_PIN_5    0x00000018
/* Parent Signal: GPIO_LED_RED GPIO Pin, (DIO5) */
#define CONFIG_PIN_0    0x00000005
/* Parent Signal: GPIO_LED_GREEN_LEFT GPIO Pin, (DIO6) */
#define CONFIG_PIN_1    0x00000006
/* Parent Signal: GPIO_LED_GREEN_RIGHT GPIO Pin, (DIO7) */
#define CONFIG_PIN_2    0x00000007
/* Parent Signal: GPIO_PIN_DEBUG GPIO Pin, (DIO20) */
#define CONFIG_PIN_7    0x00000014
/* Parent Signal: GPIO_PIN_INTR GPIO Pin, (DIO16) */
#define CONFIG_PIN_8    0x00000010
/* Parent Signal: CONFIG_SPI_0 SS, (DIO26) */
#define CONFIG_PIN_6    0x0000001a
/* GPIO_PIN_SUB1GHZ (DIO9) */
#define GPIO_PIN_SUB1GHZ    0x00000009
/* GPIO_PIN_2G4 (DIO10) */
#define GPIO_PIN_2G4    0x0000000a
/* GPIO_PIN_RF_PA (DIO11) */
#define GPIO_PIN_RF_PA    0x0000000b
/* Parent Signal: GPIO_PIN_DEBUG2 GPIO Pin, (DIO19) */
#define CONFIG_PIN_9    0x00000013
/* Parent Signal: GPIO_PIN_DEBUG3 GPIO Pin, (DIO18) */
#define CONFIG_PIN_10    0x00000012


/*
 *  ======== RF ========
 */
#define Board_DIO_9_RFSW 0x00000009
/*
 *  =============================== RF Front-end ===============================
 */
extern void initRfFrontend(void);


/*
 *  ======== SPI ========
 */

/*
 *  MOSI: DIO24
 *  MISO: DIO25
 *  SCLK: DIO23
 *  SS: DIO26
 */
#define CONFIG_SPI_0                0


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
