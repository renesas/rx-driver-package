/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_emwin_rx_pid_spi_if.c
 * Version      : 1.00
 * Description  : Functions of SPI touch interface.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version        Description
 *         : 31.08.2022 6.26.c.1.00    First Release
 *         : 31.03.2023 6.32.a.1.00    Update emWin library to v6.32a.
 *                                     Fixed TOUCH_ORIENTATION definition value.
 *         : 31.01.2024 6.34.g.1.00    Update emWin library to v6.34g.
 *                                     Moved SCI and RSPI pin setting process after the Open function.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#include "r_emwin_rx_if.h"

#if ((EMWIN_USE_TOUCH == 1) && ((EMWIN_TOUCH_IF == TOUCH_IF_RSPI) || (EMWIN_TOUCH_IF == TOUCH_IF_SCI_SPI)))

#if(EMWIN_TOUCH_IF == TOUCH_IF_SCI_SPI)

#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"

#else /* (EMWIN_LCD_IF == LCD_IF_RSPI) */

#include "r_rspi_rx_if.h"
#include "r_rspi_rx_pinset.h"

#endif

#include "r_gpio_rx_if.h"
#include "r_emwin_rx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#if (EMWIN_TOUCH_IF == TOUCH_IF_SCI_SPI)

/* SCI_SPI pin setting */
#if (EMWIN_TOUCH_IF_NUMBER == 0)
#define SCI_PIN_SET     R_SCI_PinSet_SCI0
#elif (EMWIN_TOUCH_IF_NUMBER == 1)
#define SCI_PIN_SET     R_SCI_PinSet_SCI1
#elif (EMWIN_TOUCH_IF_NUMBER == 2)
#define SCI_PIN_SET     R_SCI_PinSet_SCI2
#elif (EMWIN_TOUCH_IF_NUMBER == 3)
#define SCI_PIN_SET     R_SCI_PinSet_SCI3
#elif (EMWIN_TOUCH_IF_NUMBER == 4)
#define SCI_PIN_SET     R_SCI_PinSet_SCI4
#elif (EMWIN_TOUCH_IF_NUMBER == 5)
#define SCI_PIN_SET     R_SCI_PinSet_SCI5
#elif (EMWIN_TOUCH_IF_NUMBER == 6)
#define SCI_PIN_SET     R_SCI_PinSet_SCI6
#elif (EMWIN_TOUCH_IF_NUMBER == 7)
#define SCI_PIN_SET     R_SCI_PinSet_SCI7
#elif (EMWIN_TOUCH_IF_NUMBER == 8)
#define SCI_PIN_SET     R_SCI_PinSet_SCI8
#elif (EMWIN_TOUCH_IF_NUMBER == 9)
#define SCI_PIN_SET     R_SCI_PinSet_SCI9
#elif (EMWIN_TOUCH_IF_NUMBER == 10)
#define SCI_PIN_SET     R_SCI_PinSet_SCI10
#elif (EMWIN_TOUCH_IF_NUMBER == 11)
#define SCI_PIN_SET     R_SCI_PinSet_SCI11
#elif (EMWIN_TOUCH_IF_NUMBER == 12)
#define SCI_PIN_SET     R_SCI_PinSet_SCI12
#endif

#else /* (EMWIN_TOUCH_IF == TOUCH_IF_RSPI) */

/* RSPI pin setting */
#if (EMWIN_TOUCH_IF_NUMBER == 0)
#define RSPI_PIN_SET     R_RSPI_PinSet_RSPI0
#elif (EMWIN_TOUCH_IF_NUMBER == 1)
#define RSPI_PIN_SET     R_RSPI_PinSet_RSPI1
#elif (EMWIN_TOUCH_IF_NUMBER == 2)
#define RSPI_PIN_SET     R_RSPI_PinSet_RSPI2
#endif

#endif /* EMWIN_TOUCH_IF */



#if (EMWIN_TOUCH_DRIVER_IC == TOUCH_DRV_IC_XPT2046)
/* Adjustment is necessary for individual differences */
#define X_AD_MIN        (220)
#define X_AD_MAX        (3870)
#define Y_AD_MIN        (320)
#define Y_AD_MAX        (3900)
#define PRESS_THRESHOLD (100)

#define X_COEF ((EMWIN_XSIZE_PHYS * 1000) / (X_AD_MAX - X_AD_MIN))
#define Y_COEF ((EMWIN_YSIZE_PHYS * 1000) / (Y_AD_MAX - Y_AD_MIN))

#if (EMWIN_USE_RUNTIME_ORIENTATION == 1)
#define TOUCH_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)  /* X and Y-mirror */
#else
#define TOUCH_ORIENTATION (GUI_MIRROR_Y)                 /* Y-mirror */
#endif

#else /* (EMWIN_TOUCH_DRIVER_IC == TOUCH_DRV_IC_OTHER) */

#warning "Warning!! It is necessary to set definition of the touch driver ic."

#endif





/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static uint8_t s_a_buffer[TOUCH_DATA_BUFFER_SIZE] = {0};

#if(EMWIN_TOUCH_IF == TOUCH_IF_SCI_SPI)

static sci_hdl_t s_touch_sci_spi_handle;
static sci_err_t s_touch_sci_spi_err;
static sci_cfg_t s_touch_sci_spi_setting;

#else /* (EMWIN_TOUCH_IF == TOUCH_IF_RSPI) */

static rspi_command_word_t  s_touch_rspi_command;
static rspi_err_t           s_touch_rspi_err;
static rspi_handle_t        s_touch_rspi_handle;
static rspi_chnl_settings_t s_touch_rspi_setting;

#endif


#if (EMWIN_TOUCH_DRIVER_IC == TOUCH_DRV_IC_XPT2046)

static volatile uint8_t s_touch_step = 0;
static volatile bool s_transfer_busy = false;
static uint8_t s_xpos_command = 0xD0;
static uint8_t s_ypos_command = 0x90;
static uint8_t s_press_command = 0xB0;
static uint8_t s_xpos_ad[2] = { 0 };
static uint8_t s_ypos_ad[2] = { 0 };
static uint8_t s_press_ad[2] = { 0 };
volatile static uint16_t s_xpos = 0;
volatile static uint16_t s_ypos = 0;
volatile static uint8_t s_press = 0;

#else /* (EMWIN_TOUCH_DRIVER_IC == TOUCH_DRV_IC_OTHER) */

#warning "Warning!! It is necessary to set definition of the touch driver ic."

#endif



#if (EMWIN_TOUCH_DRIVER_IC == TOUCH_DRV_IC_XPT2046)
/**********************************************************************************************************************
 * Function Name: read_command
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void read_command (void *p_src, uint16_t const length)
{

#if (EMWIN_USE_TOUCH_CS_PIN == 1)
    /* Chip select pin is High */
    R_GPIO_PinDirectionSet(EMWIN_TOUCH_CS_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_TOUCH_CS_PIN, GPIO_LEVEL_HIGH);
#endif

#if (EMWIN_USE_TOUCH_CS_PIN == 1)
    /* Chip select pin is Low */
    R_GPIO_PinDirectionSet(EMWIN_TOUCH_CS_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_TOUCH_CS_PIN, GPIO_LEVEL_LOW);
#endif

#if(EMWIN_TOUCH_IF == TOUCH_IF_SCI_SPI)

    s_touch_sci_spi_err = R_SCI_Send(s_touch_sci_spi_handle, p_src, length);

    if (SCI_SUCCESS != s_touch_sci_spi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();  /* no operation */
        }
    }

#else /* (EMWIN_TOUCH_IF == TOUCH_IF_RSPI) */

    s_touch_rspi_err = R_RSPI_Write(s_touch_rspi_handle, s_touch_rspi_command, p_src, length);

    if (RSPI_SUCCESS != s_touch_rspi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#endif
}
/**********************************************************************************************************************
 * End of function read_command
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: read_data
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void read_data (void *p_dst, uint16_t const length)
{
#if(EMWIN_TOUCH_IF == TOUCH_IF_SCI_SPI)

    s_touch_sci_spi_err = R_SCI_Receive(s_touch_sci_spi_handle, p_dst, length);

    if (SCI_SUCCESS != s_touch_sci_spi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#else /* (EMWIN_TOUCH_IF == TOUCH_IF_RSPI) */

    s_touch_rspi_err = R_RSPI_Read(s_touch_rspi_handle, s_touch_rspi_command, p_dst, length);

    if (RSPI_SUCCESS != s_touch_rspi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#endif
}
/**********************************************************************************************************************
 * End of function read_data
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: cb_spi
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_spi(void * args)
{
    volatile e_emwin_rx_err_t emwin_ret;
    volatile uint16_t temp;
    volatile uint8_t orientation = TOUCH_ORIENTATION;

    switch (s_touch_step)
    {
        case 0:
            {
            read_data(s_xpos_ad, sizeof(s_xpos_ad));

            s_touch_step++;
            break;
        }

        case 1:
            {
            read_command(&s_ypos_command, sizeof(s_ypos_command));

            s_touch_step++;
            break;
        }

        case 2:
            {
            read_data(s_ypos_ad, sizeof(s_ypos_ad));

            s_touch_step++;
            break;
        }

        case 3:
            {
            read_command(&s_press_command, sizeof(s_press_command));

            s_touch_step++;
            break;
        }

        case 4:
            {
            read_data(s_press_ad, sizeof(s_press_ad));

            s_touch_step++;
            break;
        }

        case 5:
            {
#if (EMWIN_USE_TOUCH_CS_PIN == 1)
            /* Chip select pin is High */
            R_GPIO_PinDirectionSet(EMWIN_TOUCH_CS_PIN, GPIO_DIRECTION_OUTPUT);
            R_GPIO_PinWrite(EMWIN_TOUCH_CS_PIN, GPIO_LEVEL_HIGH);
#endif

            /* A/D value to coordinate conversion */
            temp   = (((uint16_t) s_xpos_ad[0] << 5) + ((uint16_t) s_xpos_ad[1] >> 3));
            s_xpos = (X_COEF * (temp - X_AD_MIN)) / 1000;

            temp   = (((uint16_t) s_ypos_ad[0] << 5) + ((uint16_t) s_ypos_ad[1] >> 3));
            s_ypos = (Y_COEF * (temp - Y_AD_MIN)) / 1000;

            temp = ((uint16_t) s_press_ad[0] << 5) + ((uint16_t) s_press_ad[1] >> 3);

            if (PRESS_THRESHOLD > temp)
            {
                s_press = 0;
            }
            else
            {
                s_press = 1;
            }

            /* Default touch orientation */
            if (GUI_MIRROR_X & orientation)
            {
                /* X - Mirror */
                s_xpos = (EMWIN_XSIZE_PHYS - 1) - s_xpos;
            }

            if (GUI_MIRROR_Y & orientation)
            {
                /* Y - Mirror */
                s_ypos = (EMWIN_YSIZE_PHYS - 1) - s_ypos;
            }

            /* Set buffer */
            s_a_buffer[0] = (uint8_t) (s_xpos >> 8);
            s_a_buffer[1] = (uint8_t) (s_xpos & 0x00FF);
            s_a_buffer[2] = (uint8_t) (s_ypos >> 8);
            s_a_buffer[3] = (uint8_t) (s_ypos & 0x00FF);
            s_a_buffer[4] = (uint8_t) (s_press);

            emwin_ret = r_emwin_rx_pidconf_cb(s_a_buffer, TOUCH_DATA_BUFFER_SIZE);

            if (EMWIN_RX_SUCCESS == emwin_ret)
            {
                s_transfer_busy = false;
            }

            s_touch_step++;
            break;
        }

        default:
            {
            R_BSP_NOP(); /* no operation */
        }
    }
}
/**********************************************************************************************************************
 * End of function cb_spi
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: cb_timer_touch_event
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_timer_touch_event(cb_timer_arg_t arg)
{
    if (s_transfer_busy)
    {
        return;
    }

    s_transfer_busy  = true;
    s_touch_step     = 0;

    /* Start acquisition of coordinate data */
    read_command(&s_xpos_command, sizeof(s_xpos_command));

}
/**********************************************************************************************************************
 End of function cb_timer_touch_event
 *********************************************************************************************************************/
#else /* (EMWIN_TOUCH_DRIVER_IC == TOUCH_DRV_IC_OTHER) */

#warning "Warning!! It is necessary to coding of the touch driver ic."

/**********************************************************************************************************************
 * Function Name: cb_spi
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_spi(void * args)
{
    /* user program */
}
/**********************************************************************************************************************
 * End of function cb_spi
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: cb_timer_touch_event
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_timer_touch_event(cb_timer_arg_t arg)
{
    /* user program */
}

#endif

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_pid_open
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_pid_open(void)
{
    e_emwin_rx_err_t ret = EMWIN_RX_FAIL;
    e_emwin_rx_err_t timer_ret;

#if (EMWIN_USE_TOUCH_CS_PIN == 1)
    /* Chip select pin is High */
    R_GPIO_PinWrite(EMWIN_TOUCH_CS_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinDirectionSet(EMWIN_TOUCH_CS_PIN, GPIO_DIRECTION_OUTPUT);
#endif

#if (EMWIN_TOUCH_IF == TOUCH_IF_SCI_SPI)

    /* Open the SSPI module */
    s_touch_sci_spi_setting.sspi.bit_rate     = EMWIN_TOUCH_BAUDRATE;
    s_touch_sci_spi_setting.sspi.int_priority = 5;
    s_touch_sci_spi_setting.sspi.invert_data  = false;
    s_touch_sci_spi_setting.sspi.msb_first    = true;
    s_touch_sci_spi_setting.sspi.spi_mode     = SCI_SPI_MODE_0;

    /* Open the SSPI channel using the API function */
    s_touch_sci_spi_err = R_SCI_Open( EMWIN_TOUCH_IF_NUMBER,      /* SSPI channel number */
            SCI_MODE_SSPI,               /* SCI mode */
            &s_touch_sci_spi_setting,    /* Address of the SSPI settings structure.      */
            &cb_spi,                     /* Address of user-defined callback function.   */
            &s_touch_sci_spi_handle);    /* Address of where the handle is to be stored. */

    /* If there were an error this would demonstrate error detection of API calls. */
    if (SCI_SUCCESS == s_touch_sci_spi_err)
    {
        /* Initialize SCI pins */
        SCI_PIN_SET();

        /* Casting emwin_rx_cb_timer pointer is used for callback function. */
        timer_ret = r_emwin_rx_guix_timer_create(20, (emwin_rx_cb_timer *)cb_timer_touch_event);

        if (EMWIN_RX_SUCCESS == timer_ret)
        {
            ret = EMWIN_RX_SUCCESS;
        }
    }
    else
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#else /* (EMWIN_TOUCH_IF == TOUCH_IF_RSPI) */

    /* Open the RSPI module */
    s_touch_rspi_command.cpha          = RSPI_SPCMD_CPHA_SAMPLE_ODD;
    s_touch_rspi_command.cpol          = RSPI_SPCMD_CPOL_IDLE_LO;
    s_touch_rspi_command.br_div        = RSPI_SPCMD_BR_DIV_1;
    s_touch_rspi_command.ssl_assert    = RSPI_SPCMD_ASSERT_SSL0;
    s_touch_rspi_command.ssl_negate    = RSPI_SPCMD_SSL_KEEP;
    s_touch_rspi_command.bit_length    = RSPI_SPCMD_BIT_LENGTH_8;
    s_touch_rspi_command.bit_order     = RSPI_SPCMD_ORDER_MSB_FIRST;
    s_touch_rspi_command.next_delay    = RSPI_SPCMD_NEXT_DLY_SSLND;
    s_touch_rspi_command.ssl_neg_delay = RSPI_SPCMD_SSL_NEG_DLY_SSLND;
    s_touch_rspi_command.clock_delay   = RSPI_SPCMD_CLK_DLY_SPCKD;
    s_touch_rspi_command.dummy         = RSPI_SPCMD_DUMMY;

    /* Configure the RSPI settings structure */
    s_touch_rspi_setting.bps_target        = EMWIN_TOUCH_BAUDRATE; /* Ask for 8Mbps bit-rate. */
    s_touch_rspi_setting.master_slave_mode = RSPI_MS_MODE_MASTER;  /* Configure the RSPI as SPI Master. */
    s_touch_rspi_setting.gpio_ssl          = RSPI_IF_MODE_3WIRE;   /* Set interface mode to 4-wire. */

    /* Open the RSPI channel using the API function */
    s_touch_rspi_err = R_RSPI_Open(EMWIN_TOUCH_IF_NUMBER,      /* RSPI channel number */
            &s_touch_rspi_setting,    /* Address of the RSPI settings structure.      */
            s_touch_rspi_command,
            &cb_spi,                  /* Address of user-defined callback function.   */
            &s_touch_rspi_handle);    /* Address of where the handle is to be stored. */

    /* If there were an error this would demonstrate error detection of API calls. */
    if (RSPI_SUCCESS == s_touch_rspi_err)
    {
        /* Initialize RSPI pins */
        RSPI_PIN_SET();

        /* Casting emwin_rx_cb_timer pointer is used for callback function. */
        timer_ret = r_emwin_rx_guix_timer_create(20, (emwin_rx_cb_timer *)cb_timer_touch_event);

        if (EMWIN_RX_SUCCESS == timer_ret)
        {
            ret = EMWIN_RX_SUCCESS;
        }
    }
    else
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#endif /* EMWIN_TOUCH_IF */

    return ret;
}
/**********************************************************************************************************************
 End of function r_emwin_rx_pid_open
 *********************************************************************************************************************/

#endif /* ((EMWIN_TOUCH_IF == TOUCH_IF_RSPI) || (EMWIN_TOUCH_IF == TOUCH_IF_SCI_SPI)) */
