#include <stdio.h>
#include "mx_hal.h"

#include "system_msp432p401r.h"
#include "driverlib.h"


struct _i2c_instance {
    uint32_t place_holder;
};

/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
    EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
    12000000,                               // SMCLK = 12MHz
    EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
    0,                                      // No byte counter threshold
    EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};

#define I2C_TIMEOUT  5000

/* NULL handler for this driver, complete in future */
static struct _i2c_instance i2c;

void* mx_hal_i2c_init(void *config)
{
    UNUSED_PARAMETER(config);

    /* Select Port 6 for I2C - Set Pin 4, 5 to input Primary Module Function,
     *   (UCB1SIMO/UCB1SDA, UCB1SOMI/UCB1SCL).
     */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
            GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initializing I2C Master to SMCLK at 400kbs with no autostop */
    MAP_I2C_initMaster(EUSCI_B1_BASE, &i2cConfig);

    /* Set Master in transmit mode */
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(EUSCI_B1_BASE);

#if 0
    /* Enable and clear the interrupt flag */
    MAP_I2C_clearInterruptFlag(EUSCI_B0_BASE,
            EUSCI_B_I2C_TRANSMIT_INTERRUPT0 + EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    //Enable master Receive interrupt
    MAP_I2C_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableInterrupt(INT_EUSCIB0);
#endif

    return &i2c;

}


/**
 * \brief Sync version of i2c write command
 */
int32_t mx_hal_i2c_cmd_write(void *instance, uint16_t slave_addr, uint8_t reg, uint8_t *buffer, uint8_t length)
{
    UNUSED_PARAMETER(instance);
    mx_status err = kNoErr;
    bool ret;

    /* Set Master in transmit mode */
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B1_BASE, slave_addr);

    ret = MAP_I2C_masterSendMultiByteStartWithTimeout(EUSCI_B1_BASE, reg, I2C_TIMEOUT);
    require_action(ret, exit, err = kNotPreparedErr);

    if (length == 0) goto exit;
    ret = MAP_I2C_masterSendMultiByteNextWithTimeout(EUSCI_B1_BASE, reg, I2C_TIMEOUT);
    require_action(ret, exit, err = kNotPreparedErr);

    while (length--){
        ret = MAP_I2C_masterSendMultiByteNextWithTimeout(EUSCI_B1_BASE, *buffer++, I2C_TIMEOUT);
        require_action(ret, exit, err = kWriteErr);
    }

exit:
    MAP_I2C_masterSendMultiByteStopWithTimeout(EUSCI_B1_BASE, I2C_TIMEOUT);
	return err;
}

static bool I2C_masterReceiveSingleByteTimeout(uint32_t moduleInstance, uint8_t *rxData, uint32_t timeout)
{
    //Set USCI in Receive mode
    BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->CTLW0,EUSCI_B_CTLW0_TR_OFS) = 0;

    //Send start
    EUSCI_B_CMSIS(moduleInstance)->CTLW0 |= (EUSCI_B_CTLW0_TXSTT + EUSCI_B_CTLW0_TXSTP);

    //Poll for receive interrupt flag.
    while (!BITBAND_PERI(EUSCI_B_CMSIS(moduleInstance)->IFG, EUSCI_B_IFG_RXIFG_OFS)
           && --timeout)
        ;

        //Check if transfer timed out
    if (timeout == 0)
        return false;

    //read single byte data.
    *rxData = (EUSCI_B_CMSIS(moduleInstance)->RXBUF & EUSCI_B_RXBUF_RXBUF_MASK);
    return true;
}

int32_t i2c_m_sync_cmd_read(void *instance, uint16_t slave_addr, uint8_t reg, uint8_t *buffer, uint8_t length)
{
    UNUSED_PARAMETER(instance);
    mx_status err = kNoErr;
    bool ret;
    uint_fast16_t status;
    uint32_t retry = 100;
    printf("Send to %x\r\n", reg);

    /* Set Master in transmit mode */
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(EUSCI_B1_BASE, slave_addr);

    ret = MAP_I2C_masterSendMultiByteStartWithTimeout(EUSCI_B1_BASE, reg, I2C_TIMEOUT);
    require_action(ret, exit, err = kNotPreparedErr);


    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_MODE);

while(1);
    mx_hal_delay_ms(50);


    if (length == 1) {
        while (--retry) {
            ret = I2C_masterReceiveSingleByteTimeout(EUSCI_B1_BASE, buffer, I2C_TIMEOUT);
            if (ret == true) {
                goto exit;
            }
            mx_hal_delay_ms(10);
        }
        require_action(retry > 0, exit, err = kTimeoutErr);
    }
    else {
        
MAP_I2C_masterReceiveStart(EUSCI_B1_BASE);
mx_hal_delay_ms(500);
 while( !(EUSCI_B_I2C_RECEIVE_INTERRUPT0 & MAP_I2C_getEnabledInterruptStatus(EUSCI_B1_BASE)));
        //while (--retry) {

          //  MAP_I2C_masterReceiveStart(EUSCI_B1_BASE);
            //mx_hal_delay_ms(50);
            // status = MAP_I2C_getEnabledInterruptStatus(EUSCI_B1_BASE);

            // if ( status & EUSCI_B_I2C_RECEIVE_INTERRUPT0 ) {
            //     break;
            // }
            // mx_hal_delay_ms(10);
        //}

        require_action(retry > 0, exit, err = kTimeoutErr);

        while (length--){
            printf("receive, %d\r\n", retry);
            if (length == 1) {
                MAP_I2C_masterReceiveMultiByteStop(EUSCI_B1_BASE);
            }
            
            *buffer++ = MAP_I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);
            mx_hal_delay_ms(5);

            // if (length) {
            //     /* Wait for the next byte ready*/
            //     while( !(EUSCI_B_I2C_RECEIVE_INTERRUPT0 & MAP_I2C_getEnabledInterruptStatus(EUSCI_B1_BASE)));
            // }

            
        }
    }
exit:
    if (err != kNoErr)   MAP_I2C_masterSendMultiByteStopWithTimeout(EUSCI_B1_BASE, I2C_TIMEOUT);
    return err;
}