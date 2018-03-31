/**
 ******************************************************************************
 * @file    button.h
 * @author  Eshen Wang
 * @version V1.0.0
 * @date    1-May-2015
 * @brief   user key operation.
 ******************************************************************************
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */

#ifndef _IO_BUTTON_H_
#define _IO_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup MICO_Drivers_interface
  * @{
  */
    
/** @defgroup MICO_keypad_Driver MiCO keypad Driver
  * @brief Provide driver interface for keypad devices
  * @{
  */


/** @addtogroup MICO_keypad_Driver
  * @{
  */
/** @defgroup MICO_Button_Driver MiCO Button Driver
  * @brief Provide driver interface for button
  * @{
  */


//--------------------------------  pin defines --------------------------------

enum _button_idle_state_e{
    IOBUTTON_IDLE_STATE_LOW = 0,
    IOBUTTON_IDLE_STATE_HIGH,
};

typedef uint8_t btn_idle_state;

typedef void (*button_pressed_cb)(void) ;
typedef void (*button_long_pressed_cb)(void) ;

typedef struct {
  const uint8_t port;
  const uint8_t pin;
  void (*io_irq)(void);
  btn_idle_state idle;
  uint32_t long_pressed_timeout;
  button_pressed_cb pressed_func;
  button_long_pressed_cb long_pressed_func;
  /* Use by driver, do not initialize */
  volatile bool clicked;
  volatile bool timer_enabled;
  volatile uint32_t start_time;
} btn_instance_t;

//------------------------------ user interfaces -------------------------------


/**
 * @brief Initialize button device.
 *
 * @param btn: button driver context data, should be persist at button's life time
 *
 * @return none
 */
void button_init(btn_instance_t * const btn);

/**
 * @brief Button driver service, should be called in main loop.
 *
 * @param btn: button driver context data, should be persist at button's life time
 *
 * @return none
 */
void button_srv(btn_instance_t * const btn);

/**
 * @brief This function should be called by IRQ routine.
 *
 * @param btn: button driver context data, should be persist at button's life time
 *
 * @return none
 */
void button_irq_handler(btn_instance_t * const btn);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  // __BUTTON_H_
