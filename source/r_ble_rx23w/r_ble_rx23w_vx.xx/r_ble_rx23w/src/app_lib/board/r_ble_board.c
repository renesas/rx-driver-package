/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

#include "r_ble_board.h"

#if (BLE_CFG_BOARD_LED_SW_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0)

#include "r_gpio_rx_if.h"
#include "r_irq_rx_if.h"

#if (BLE_CFG_BOARD_TYPE == 1)   /* for RX23W Target Board(TB) */
#define BLE_BOARD_SW1_IRQ   (IRQ_NUM_5)
#define BLE_BOARD_SW2_IRQ   (IRQ_NUM_5)
#define BLE_BOARD_LED1_PIN  (GPIO_PORT_C_PIN_0)
#define BLE_BOARD_LED2_PIN  (GPIO_PORT_B_PIN_0)
#elif (BLE_CFG_BOARD_TYPE == 2) /* for RX23W RSSK board */
#define BLE_BOARD_SW1_IRQ   (IRQ_NUM_1)
#define BLE_BOARD_SW2_IRQ   (IRQ_NUM_0)
#define BLE_BOARD_LED1_PIN  (GPIO_PORT_4_PIN_2)
#define BLE_BOARD_LED2_PIN  (GPIO_PORT_4_PIN_3)
#else /* BLE_CFG_BOARD_TYPE */  /* for Custom board */
#define BLE_BOARD_SW1_IRQ   (IRQ_NUM_7)
#define BLE_BOARD_SW2_IRQ   (IRQ_NUM_5)
#define BLE_BOARD_LED1_PIN  (GPIO_PORT_C_PIN_5)
#define BLE_BOARD_LED2_PIN  (GPIO_PORT_C_PIN_6)
#endif /* BLE_CFG_BOARD_TYPE */

static irq_handle_t gs_irq_hdl[BLE_BOARD_SW_MAX];
static ble_sw_cb_t gs_sw_cb[BLE_BOARD_SW_MAX];

static void ble_sw_cb(void)
{
    R_BLE_LPC_SetInhibitSoftwareStandby(true);
}

static void irq_cb(void *pargs)
{
    irq_handle_t hld = (irq_handle_t)pargs;

    for( uint16_t i = 0; i < (uint16_t)BLE_BOARD_SW_MAX; i++ )
    {
        if ((hld == gs_irq_hdl[i]) && (gs_sw_cb[i] != NULL))
        {
            R_BLE_SetEvent(gs_sw_cb[i]);
        }
    }
}

static void irq_pin_set(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

#if   (BLE_CFG_BOARD_TYPE == 1)
    /*Set IRQ5 pin */
    PORT1.PMR.BIT.B5 = 0U;
    PORT1.PDR.BIT.B5 = 0U;
    MPC.P15PFS.BYTE  = 0x40U;
#elif (BLE_CFG_BOARD_TYPE == 2)
    /*Set IRQ0 pin */
    PORT3.PMR.BIT.B0 = 0U;
    PORT3.PDR.BIT.B0 = 0U;
    MPC.P30PFS.BYTE  = 0x40U;
    /*Set IRQ1 pin */
    PORT3.PMR.BIT.B1 = 0U;
    PORT3.PDR.BIT.B1 = 0U;
    MPC.P31PFS.BYTE  = 0x40U;
#else /* (BLE_CFG_BOARD_TYPE == x) */
    /*Set IRQ5 pin */
    PORT1.PMR.BIT.B5 = 0U;
    PORT1.PDR.BIT.B5 = 0U;
    MPC.P15PFS.BYTE  = 0x40U;
    /*Set IRQ7 pin */
    PORT1.PMR.BIT.B7 = 0U;
    PORT1.PDR.BIT.B7 = 0U;
    MPC.P17PFS.BYTE  = 0x40U;
#endif /* (BLE_CFG_BOARD_TYPE == x) */

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

}
void R_BLE_BOARD_Init(void)
{

#ifdef BLE_BOARD_LED1_PIN
    /* LED1 */
    R_GPIO_PinControl(BLE_BOARD_LED1_PIN, GPIO_CMD_ASSIGN_TO_GPIO);
    R_GPIO_PinDirectionSet(BLE_BOARD_LED1_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(BLE_BOARD_LED1_PIN, GPIO_LEVEL_HIGH);
#endif /* BLE_BOARD_LED1_PIN */

#ifdef BLE_BOARD_LED2_PIN
    /* LED2 */
    R_GPIO_PinControl(BLE_BOARD_LED2_PIN, GPIO_CMD_ASSIGN_TO_GPIO);
    R_GPIO_PinDirectionSet(BLE_BOARD_LED2_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(BLE_BOARD_LED2_PIN, GPIO_LEVEL_HIGH);
#endif /* BLE_BOARD_LED2_PIN */

    /* pin set for IRQ port */
    irq_pin_set();

    /* Set wake up trigger */
    R_BLE_BOARD_RegisterSwitchCb(BLE_BOARD_SW1, ble_sw_cb);
}

void R_BLE_BOARD_SetLEDState(e_ble_led_t led, bool onoff)
{
    if (led == BLE_BOARD_LED1)
    {
#ifdef BLE_BOARD_LED1_PIN
        R_GPIO_PinWrite(BLE_BOARD_LED1_PIN, (onoff) ? GPIO_LEVEL_LOW : GPIO_LEVEL_HIGH);
#endif /* BLE_BOARD_LED1_PIN */
    }
    else if (led == BLE_BOARD_LED2)
    {
#ifdef BLE_BOARD_LED2_PIN
        R_GPIO_PinWrite(BLE_BOARD_LED2_PIN, (onoff) ? GPIO_LEVEL_LOW : GPIO_LEVEL_HIGH);
#endif /* BLE_BOARD_LED2_PIN */
    }
}

void R_BLE_BOARD_ToggleLEDState(e_ble_led_t led)
{
    gpio_level_t level;

    if (led == BLE_BOARD_LED1)
    {
#ifdef BLE_BOARD_LED1_PIN
        level = R_GPIO_PinRead(BLE_BOARD_LED1_PIN);
        R_GPIO_PinWrite(BLE_BOARD_LED1_PIN, (level == GPIO_LEVEL_HIGH) ? GPIO_LEVEL_LOW : GPIO_LEVEL_HIGH);
#endif /* BLE_BOARD_LED1_PIN */
    }
    else if (led == BLE_BOARD_LED2)
    {
#ifdef BLE_BOARD_LED2_PIN
        level = R_GPIO_PinRead(BLE_BOARD_LED2_PIN);
        R_GPIO_PinWrite(BLE_BOARD_LED2_PIN, (level == GPIO_LEVEL_HIGH) ? GPIO_LEVEL_LOW : GPIO_LEVEL_HIGH);
#endif /* BLE_BOARD_LED2_PIN */
    }
}

void R_BLE_BOARD_RegisterSwitchCb(e_ble_sw_t sw, ble_sw_cb_t cb)
{
    if (sw == BLE_BOARD_SW1)
    {
#ifdef BLE_BOARD_SW1_IRQ
        gs_sw_cb[0] = cb;
        R_IRQ_Open(BLE_BOARD_SW1_IRQ, IRQ_TRIG_FALLING, IRQ_PRI_1, &gs_irq_hdl[0], irq_cb);
        R_IRQ_InterruptEnable(gs_irq_hdl[0], true);
#endif /* BLE_BOARD_SW1_IRQ */
    }
    else if (sw == BLE_BOARD_SW2)
    {
#ifdef BLE_BOARD_SW2_IRQ
        gs_sw_cb[1] = cb;
#ifdef BLE_BOARD_SW1_IRQ
        if(BLE_BOARD_SW1_IRQ == BLE_BOARD_SW2_IRQ)
        {
            /* Use the same IRQ as SW1 */
            gs_irq_hdl[1] = gs_irq_hdl[0];
        }
        else
#endif /* BLE_BOARD_SW1_IRQ */
        {
            R_IRQ_Open(BLE_BOARD_SW2_IRQ, IRQ_TRIG_FALLING, IRQ_PRI_1, &gs_irq_hdl[1], irq_cb);
            R_IRQ_InterruptEnable(gs_irq_hdl[1], true);
        }
#endif /* BLE_BOARD_SW2_IRQ */
    }
}

#else /* (BLE_CFG_BOARD_LED_SW_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */

void R_BLE_BOARD_Init(void)
{
}

void R_BLE_BOARD_SetLEDState(e_ble_led_t led, bool onoff)
{
    (void)led;
    (void)onoff;
}

void R_BLE_BOARD_ToggleLEDState(e_ble_led_t led)
{
    (void)led;
}

void R_BLE_BOARD_RegisterSwitchCb(e_ble_sw_t sw, ble_sw_cb_t cb)
{
    (void)sw;
    (void)cb;
}

#endif /* (BLE_CFG_BOARD_LED_SW_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */
