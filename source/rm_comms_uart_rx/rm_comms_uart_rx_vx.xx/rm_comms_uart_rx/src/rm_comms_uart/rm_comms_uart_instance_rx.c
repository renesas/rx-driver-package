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
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/******************************************************************************************************************//**
 * @addtogroup rm_comms_uart_instance_rx
 * @{
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_smc_entry.h"
#include "rm_comms_uart.h"
#include "../rm_comms_lock/rm_comms_lock.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* R_x_y */
#define FUNC(x, y) (R_ ## x ## y)
/* R_x_Start */
#define XSTART(x)  (FUNC(x, _Start))
/* R_x_Stop */
#define XSTOP(x)   (FUNC(x, _Stop))
/* R_x_Serail_Send */
#define XWRITE(x)  (FUNC(x, _Serial_Send))
/* R_x_Serial_Receive */
#define XREAD(x)   (FUNC(x, _Serial_Receive))

/* SCI + channel_No */
#define SCI_CONNECTED(x) (SCI ## x)
/* SCIx */
#define SCI_COMPONENT(x) (SCI_CONNECTED(x))

/* UART MODULE API */
#define RM_COMMS_UART_DEVICE0_START (XSTART(COMMS_UART_CFG_BUS0_SCI_UART_COMPONENT))
#define RM_COMMS_UART_DEVICE0_STOP  (XSTOP(COMMS_UART_CFG_BUS0_SCI_UART_COMPONENT))
#define RM_COMMS_UART_DEVICE0_WRITE (XWRITE(COMMS_UART_CFG_BUS0_SCI_UART_COMPONENT))
#define RM_COMMS_UART_DEVICE0_READ  (XREAD(COMMS_UART_CFG_BUS0_SCI_UART_COMPONENT))
/* SCI register name SCIx */
#define COMMS_UART_SCI_PLUS_CH      (SCI_COMPONENT(COMMS_UART_CFG_BUS0_SCI_UART_CH))


/* rm_comms_uart_sci_init_bit_rate(): sci_init_bit_rate() of Refer to RX SCI FIT module */
/* RX Common Setting */
#define SCI_CFG_PARAM_CHECKING_ENABLE           (RM_COMMS_UART_CFG_PARAM_CHECKING_ENABLE)
#define SCI_CFG_ASYNC_INCLUDED                  (1)
#define SCI_CFG_SSPI_INCLUDED                   (0)
#define SCI_CFG_SYNC_INCLUDED                   (0)
#define SCI_MODE_ASYNC                          (1)

/* RX65N Setting */
#if ((COMMS_UART_CFG_BUS0_SCI_UART_CH >= 10) && (COMMS_UART_CFG_BUS0_SCI_UART_CH <= 11))
    #define SCI_CFG_FIFO_INCLUDED               (1)
#endif
#define NUM_DIVISORS_ASYNC                      (9)

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* BAUD DIVISOR INFO */
/* BRR = (PCLK/(divisor * baud)) - 1 */
/* when abcs=1, divisor = 32*pow(2,2n-1) */
/* when abcs=0, divisor = 64*pow(2,2n-1) */
typedef struct st_baud_divisor
{
    int16_t     divisor;    // clock divisor
    uint8_t     abcs;       // abcs value to get divisor
    uint8_t     bgdm;       // bdgm value to get divisor
    uint8_t     cks;        // cks  value to get divisor (cks = n)
} baud_divisor_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* BAUD DIVISOR INFO */
/* Asynchronous */
/* BRR = (PCLK/(divisor * baud)) - 1 */
/* when abcs=0 & bgdm=0, divisor = 64*pow(2,2n-1) */
/* when abcs=1 & bgdm=0 OR abcs=0 & bgdm=1, divisor = 32*pow(2,2n-1) */
/* when abcs=1 & bgdm=1, divisor = 16*pow(2,2n-1) */
#if (SCI_CFG_ASYNC_INCLUDED)
/* NOTE: diff than SCI async baud table, but should provide same results */
const baud_divisor_t async_baud[NUM_DIVISORS_ASYNC]=
{
    /* divisor result, abcs, bgdm, n */
    {8,    1, 1, 0},
    {16,   0, 1, 0},
    {32,   0, 0, 0},
    {64,   0, 1, 1},
    {128,  0, 0, 1},
    {256,  0, 1, 2},
    {512,  0, 0, 2},
    {1024, 0, 1, 3},
    {2048, 0, 0, 3}
};
#endif

#if (1 <= COMMS_UART_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1 // ThreadX
 #if COMMS_UART_CFG_RTOS_TX_BUS_LOCK_SUPPORT_ENABLE
 rm_comms_mutex_t g_comms_uart0_tx_mutex =
 {
     .p_name = "g_comms_uart0 tx mutex",
 };
 #endif
 #if COMMS_UART_CFG_RTOS_RX_BUS_LOCK_SUPPORT_ENABLE
 rm_comms_mutex_t g_comms_uart0_rx_mutex =
 {
     .p_name = "g_comms_uart0 rx mutex",
 };
 #endif
 #if COMMS_UART_CFG_RTOS_TX_BLOCKING_SUPPORT_ENABLE
 rm_comms_semaphore_t g_comms_uart0_tx_semaphore =
 {
     .p_name = "g_comms_uart0 tx semaphore",
 };
 #endif
 #if COMMS_UART_CFG_RTOS_RX_BLOCKING_SUPPORT_ENABLE
 rm_comms_semaphore_t g_comms_uart0_rx_semaphore =
 {
     .p_name = "g_comms_uart0 rx semaphore",
 };
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_UART_CFG_RTOS_TX_BUS_LOCK_SUPPORT_ENABLE
 rm_comms_mutex_t g_comms_uart0_tx_mutex;
 #endif
 #if COMMS_UART_CFG_RTOS_RX_BUS_LOCK_SUPPORT_ENABLE
 rm_comms_mutex_t g_comms_uart0_rx_mutex;
 #endif
 #if COMMS_UART_CFG_RTOS_TX_BLOCKING_SUPPORT_ENABLE
 rm_comms_semaphore_t g_comms_uart0_tx_semaphore;
 #endif
 #if COMMS_UART_CFG_RTOS_RX_BLOCKING_SUPPORT_ENABLE
 rm_comms_semaphore_t g_comms_uart0_rx_semaphore;
 #endif
#else
#endif /* BSP_CFG_RTOS == 1 */

const uart_module_api_t g_comms_uart_bus0_module_api =
{
    .start = RM_COMMS_UART_DEVICE0_START,
    .stop  = RM_COMMS_UART_DEVICE0_STOP,
    .write = RM_COMMS_UART_DEVICE0_WRITE,
    .read  = RM_COMMS_UART_DEVICE0_READ,
};
#endif /* 1 <= COMMS_UART_CFG_BUS_NUM_MAX */

/* Bit Rate Setting */
int32_t rm_comms_uart_sci_init_bit_rate (uint32_t const pclk, uint32_t const baud);

/* COMMS UART Device */
#if (1 <= COMMS_UART_CFG_DEVICE_NUM_MAX)
/* callback function */
void COMMS_UART_CFG_DEVICE0_CALLBACK (rm_comms_uart_callback_args_t * p_args); /* User callback function for device0 */
#endif

/* COMMS UART Shared Bus */
#if (1 <= COMMS_UART_CFG_BUS_NUM_MAX)
/* callback function */
void rm_comms_uart_bus0_callback (rm_comms_uart_event_t event); /* User callback function for UART driver for bus0 */
/* app api */
void g_comms_uart_bus0_app_api_open (void);
void g_comms_uart_bus0_app_api_close (void);
#endif

/* COMMS UART Shared Bus (Common) */
extern void rm_comms_uart_callback (rm_comms_uart_ctrl_t const * p_api_ctrl, rm_comms_uart_event_t event);


/*******************************************************************************************************************//**
* Function Name: rm_comms_uart_sci_init_bit_rate (base module RX65N FIT module v4.81 sci_init_bit_rate())
* Description  : This function determines the best possible settings for the
*                baud rate registers for the specified peripheral clock speed
*                and baud rate. Note that this does not guarantee a low bit 
*                error rate, just the best possible one. The bit rate error is
*                returned in .1% increments. If the hardware cannot support
*                the specified combination, a value of 1000 (100% error) is
*                returned.
*
* NOTE: The transmitter and receiver (TE and RE bits in SCR) must be disabled 
*       prior to calling this function.
*
*       The application must pause for 1 bit time after the BRR register
*       is loaded before transmitting/receiving to allow time for the clock
*       to settle. 
*
* Arguments    : 
*                    Handle for channel (ptr to chan control block)
*                    NOTE: mode element must be already set
*                pclk -
*                    Peripheral clock speed; e.g. 24000000 for 24MHz
*                baud -
*                    Baud rate; 19200, 57600, 115200, etc.
* Return Value : bit error in .1% increments; e.g. 16 = 1.6% bit rate error
*                a value of 1000 denotes 100% error; no registers set
 **********************************************************************************************************************/
int32_t rm_comms_uart_sci_init_bit_rate(uint32_t const   pclk,
                                        uint32_t const   baud)
{
    uint32_t i;
    uint32_t num_divisors = 0;
    uint32_t ratio;
    uint32_t tmp;
    baud_divisor_t const *p_baud_info = NULL;

    uint32_t divisor;
    uint32_t int_M;
    float    float_M;
    float    error;
    float    abs_error;

    /* Set mode */
    uint32_t mode = SCI_MODE_ASYNC;

#if SCI_CFG_FIFO_INCLUDED
    uint8_t brr;
#endif

#if SCI_CFG_PARAM_CHECKING_ENABLE
    if ((0 == pclk) || (0 == baud))
    {
        return 1000;
    }
#endif

    /* SELECT PROPER TABLE BASED UPON MODE */
    if (SCI_MODE_ASYNC == mode)
    {
#if (SCI_CFG_ASYNC_INCLUDED)
        p_baud_info = async_baud;
        num_divisors = NUM_DIVISORS_ASYNC;
#endif
    }
    else
    {
        /* SYNC or SSPI */
#if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
        p_baud_info = sync_baud;
        num_divisors = NUM_DIVISORS_SYNC;
#endif
    }

    /* FIND DIVISOR; table has associated ABCS, BGDM and CKS values */
    /* BRR must be 255 or less */
    /* the "- 1" is ignored in some steps for approximations */
    /* BRR = (PCLK/(divisor * baud)) - 1 */
    /* BRR = (ratio / divisor) - 1 */
    ratio = pclk/baud;
    /* WAIT_LOOP */
    for(i = 0; i < num_divisors; i++)
    {
        /* Casting int16_t to uint32_t is valid. Because clock divisor is positive integer */
        if (ratio < (uint32_t)(p_baud_info[i].divisor * 256))
        {
            break;
        }
    }

    /* RETURN IF BRR WILL BE >255 OR LESS THAN 0 */
    if (i == num_divisors)
    {
        return(1000);           // impossible baud rate requested; return 100% error
    }

    /* Casting int16_t to uint32_t is valid. Because clock divisor is a positive integer */
    divisor = (uint32_t)p_baud_info[i].divisor;
    tmp = ratio/(divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1
    if(0 == tmp)
    {
        return(1000);            // illegal value; return 100% error
    }

    /* SET BRR, ABCS, BDGM, and CKS */
    tmp = ratio / (divisor/2);  // divide by half the divisor

#if SCI_CFG_FIFO_INCLUDED
    /* Casting is valid. Because result of calculation is in range uint8_t type */
    brr = (uint8_t)((tmp & 0x01) ? (tmp/2) : ((tmp/2)-1));
    if (0 == brr)
    {
        if (true == hdl->fifo_ctrl)
        {
            if (1 == COMMS_UART_SCI_PLUS_CH.SMR.BIT.CM)
            {
                if (0 == COMMS_UART_SCI_PLUS_CH.SMR.BIT.CKS)
                {
                    return(1000);
                }
            }
        }
    }
#endif

    /* if odd, "round up" by ignoring -1; divide by 2 again for rest of divisor */
    COMMS_UART_SCI_PLUS_CH.BRR = (uint8_t)((tmp & 0x01) ? (tmp/2) : ((tmp/2)-1));
    COMMS_UART_SCI_PLUS_CH.SEMR.BIT.ABCS = p_baud_info[i].abcs;
    COMMS_UART_SCI_PLUS_CH.SEMR.BIT.BGDM = p_baud_info[i].bgdm;
    COMMS_UART_SCI_PLUS_CH.SMR.BIT.CKS = p_baud_info[i].cks;

    /* CALCULATE BIT RATE ERROR.
     * RETURN IF ERROR LESS THAN 1% OR IF IN SYNCHRONOUS/SSPI MODE.
     */
    tmp = ratio/(divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1

    /* Casting uint32_t to float is valid */
    error = ( ((float)pclk / ((baud * divisor) * tmp)) - 1) * 100;
    abs_error  = (error < 0) ? (-error) : error;

    if ((abs_error <= 1.0) || (SCI_MODE_ASYNC != mode))
    {
        COMMS_UART_SCI_PLUS_CH.SEMR.BIT.BRME = 0;          // disable MDDR

        /* Casting float to uint32_t */
        return (uint32_t)(error*10);
    }

    /* CALCULATE M ASSUMING A 0% ERROR then WRITE REGISTER */
    COMMS_UART_SCI_PLUS_CH.BRR = (uint8_t)(tmp-1);

    /* Casting uint32_t to float is valid  */
    float_M = ((float)((baud * divisor) * 256) * tmp) / pclk;
    float_M *= 2;

    /* Casting float to uint32_t */
    int_M = (uint32_t)float_M;
    int_M = (int_M & 0x01) ? ((int_M/2) + 1) : (int_M/2);

    COMMS_UART_SCI_PLUS_CH.MDDR = (uint8_t)int_M;      // write M
    COMMS_UART_SCI_PLUS_CH.SEMR.BIT.BRME = 1;          // enable MDDR

    /* Casting uint32_t to float is valid*/
    error = (( (float)(pclk) / (((divisor * tmp) * baud) * ((float)(256)/int_M)) ) - 1) * 100;

    /* Casting float to int32_t */
    return (int32_t)(error*10);
}
/**********************************************************************************************************************
 End of function rm_comms_uart_sci_init_bit_rate()
 *********************************************************************************************************************/

#if (1 <= COMMS_UART_CFG_BUS_NUM_MAX)
/*******************************************************************************************************************//**
 * Function Name: g_comms_uart_bus0_app_api_open
 * @brief Configures the UART modes and starts the SCI channel using R_Config_SCIX_Start(). ("X": Channel No. (0- ))
 * @retval None
 **********************************************************************************************************************/
void g_comms_uart_bus0_app_api_open(void)
{
    /* Configuration */
    uint8_t stop_bits;
    uint8_t parity;
    uint8_t data_size;
    uint8_t data_size_type;

    /* Clear the control register */
    COMMS_UART_SCI_PLUS_CH.SCR.BYTE = 0x00U;

    /* Set clock enable */
    if (SCI_CLK_INT == COMMS_UART_CFG_BUS0_CLK_SRC)
    {
        /* SCK disabled */
        COMMS_UART_SCI_PLUS_CH.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;
    }
    else
    {
        /* SCK enabled */
        COMMS_UART_SCI_PLUS_CH.SCR.BYTE = _02_SCI_EXTERNAL;
    }
      
    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit, and set SPMR */
    COMMS_UART_SCI_PLUS_CH.SIMR1.BIT.IICM = 0U;
    COMMS_UART_SCI_PLUS_CH.SPMR.BYTE      = _00_SCI_RTS | _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Stop bits */
    if (SCI_STOPBITS_1 == COMMS_UART_CFG_BUS0_STOP_BITS)
    {
        stop_bits = _00_SCI_STOP_1;
    }
    else
    {
        stop_bits = _08_SCI_STOP_2;
    }

    /* Parity */
    if (SCI_PARITY_OFF == COMMS_UART_CFG_BUS0_PARITY_EN)
    {
        parity = _00_SCI_PARITY_DISABLE;
    }
    else
    {
        if (SCI_EVEN_PARITY == COMMS_UART_CFG_BUS0_PARITY_TYPE)
        {
            parity = _00_SCI_PARITY_EVEN | _20_SCI_PARITY_ENABLE;
        }
        else // SCI_ODD_PARITY
        {
            parity = _10_SCI_PARITY_ODD | _20_SCI_PARITY_ENABLE;
        }
    }

    /* Data size */
    if (SCI_DATA_7BIT == COMMS_UART_CFG_BUS0_DATA_SIZE)
    {
        data_size = _40_SCI_DATA_LENGTH_7;
    }
    else // SCI_DATA_8BIT or SCI_DATA_9BIT
    {
        data_size = _00_SCI_DATA_LENGTH_8;
    }

    if ((SCI_DATA_7BIT == COMMS_UART_CFG_BUS0_DATA_SIZE) || (SCI_DATA_8BIT == COMMS_UART_CFG_BUS0_DATA_SIZE))
    {
        data_size_type = _10_SCI_DATA_LENGTH_8_OR_7;
    }
    else
    {
        data_size_type = _00_SCI_DATA_LENGTH_9;
    }

    /* Set control registers */
    if (SCI_CLK_INT == COMMS_UART_CFG_BUS0_CLK_SRC)
    {
        COMMS_UART_SCI_PLUS_CH.SMR.BYTE = _00_SCI_CLOCK_PCLK | _00_SCI_MULTI_PROCESSOR_DISABLE | stop_bits | parity | 
                        data_size | _00_SCI_ASYNCHRONOUS_OR_I2C_MODE;
        COMMS_UART_SCI_PLUS_CH.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                        data_size_type | _62_SCI_SCMR_DEFAULT;
        COMMS_UART_SCI_PLUS_CH.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE | _00_SCI_16_BASE_CLOCK | _00_SCI_NOISE_FILTER_DISABLE | 
                        _00_SCI_BAUDRATE_SINGLE | _00_SCI_LOW_LEVEL_START_BIT;
    }
    else if (SCI_CLK_EXT8X == COMMS_UART_CFG_BUS0_CLK_SRC)
    {
        COMMS_UART_SCI_PLUS_CH.SMR.BYTE = _00_SCI_MULTI_PROCESSOR_DISABLE | stop_bits | parity | 
                        data_size | _00_SCI_ASYNCHRONOUS_OR_I2C_MODE;
        COMMS_UART_SCI_PLUS_CH.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                        data_size_type | _62_SCI_SCMR_DEFAULT;
        COMMS_UART_SCI_PLUS_CH.SEMR.BYTE = _00_SCI_ASYNC_SOURCE_EXTERNAL | _10_SCI_8_BASE_CLOCK | _00_SCI_NOISE_FILTER_DISABLE | 
                        _00_SCI_LOW_LEVEL_START_BIT;
    }
    else // SCI_CLK_EXT16X
    {
        COMMS_UART_SCI_PLUS_CH.SMR.BYTE = _00_SCI_MULTI_PROCESSOR_DISABLE | stop_bits | parity | 
                        data_size | _00_SCI_ASYNCHRONOUS_OR_I2C_MODE;
        COMMS_UART_SCI_PLUS_CH.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                        data_size_type | _62_SCI_SCMR_DEFAULT;
        COMMS_UART_SCI_PLUS_CH.SEMR.BYTE = _00_SCI_ASYNC_SOURCE_EXTERNAL | _00_SCI_16_BASE_CLOCK | _00_SCI_NOISE_FILTER_DISABLE | 
                        _00_SCI_LOW_LEVEL_START_BIT;
    }

    /* Set bit rate */
    uint32_t    pclk = COMMS_UART_CFG_BUS0_PCLK * 1000000; /* [MHz] to [Hz] for BUS0 */
    uint32_t    rate = COMMS_UART_CFG_BUS0_RATE;           /* Bit Rate for BUS0 */

    rm_comms_uart_sci_init_bit_rate(pclk, rate);

    /* Start */
    g_comms_uart_bus0_module_api.start();
}
/**********************************************************************************************************************
 End of function g_comms_uart_bus0_app_api_open
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: g_comms_uart_bus0_app_api_close
 * @brief Stops the SCI channel using R_Config_SCIX_Stop(). ("X": Channel No. (0- ))
 * @retval None
 **********************************************************************************************************************/
void g_comms_uart_bus0_app_api_close(void)
{
    /* Stop */
    g_comms_uart_bus0_module_api.stop();
}
/**********************************************************************************************************************
 End of function g_comms_uart_bus0_app_api_close
 *********************************************************************************************************************/
#endif /* 1 <= COMMS_UART_CFG_BUS_NUM_MAX */

#if (1 <= COMMS_UART_CFG_BUS_NUM_MAX)
const uart_driver_app_api_t g_comms_uart_bus0_app_api = 
{
    .open  = g_comms_uart_bus0_app_api_open,
    .close = g_comms_uart_bus0_app_api_close,
};
const uart_instance_t g_comms_uart_bus0_driver_instance =
{
    .p_module_api = &g_comms_uart_bus0_module_api,
    .p_app_api    = &g_comms_uart_bus0_app_api,
};

rm_comms_uart_extended_cfg_t g_comms_uart0_extended_cfg =
{
#if BSP_CFG_RTOS

 #if COMMS_UART_CFG_RTOS_TX_BUS_LOCK_SUPPORT_ENABLE
    .p_tx_mutex = &g_comms_uart0_tx_mutex,
 #else
    .p_tx_mutex = NULL,
 #endif

 #if COMMS_UART_CFG_RTOS_RX_BUS_LOCK_SUPPORT_ENABLE
    .p_rx_mutex = &g_comms_uart0_rx_mutex,
 #else
    .p_rx_mutex = NULL,
 #endif

 #if COMMS_UART_CFG_RTOS_TX_BLOCKING_SUPPORT_ENABLE
    .p_tx_semaphore = &g_comms_uart0_tx_semaphore,
 #else
    .p_tx_semaphore = NULL,
 #endif

 #if COMMS_UART_CFG_RTOS_RX_BLOCKING_SUPPORT_ENABLE
    .p_rx_semaphore = &g_comms_uart0_rx_semaphore,
 #else
    .p_rx_semaphore = NULL,
 #endif
    .mutex_timeout  = 0xFFFFFFFF,
#endif /* BSP_CFG_RTOS */
  .p_uart = &g_comms_uart_bus0_driver_instance,
};
#endif /* 1 <= COMMS_UART_CFG_BUS_NUM_MAX */

 /* COMMS UART Device */
#if (1 <= COMMS_UART_CFG_DEVICE_NUM_MAX)
rm_comms_uart_instance_ctrl_t g_comms_uart_device0_ctrl;
const rm_comms_uart_cfg_t          g_comms_uart_device0_cfg =
{ .semaphore_timeout = COMMS_UART_CFG_DEVICE0_BLOCKING_TIMEOUT,
  .p_lower_level_cfg = NULL,
  .p_extend = (void*) &g_comms_uart0_extended_cfg,
  .p_callback = COMMS_UART_CFG_DEVICE0_CALLBACK,
};
const rm_comms_uart_instance_t g_comms_uart_device0 =
{
    .p_ctrl = &g_comms_uart_device0_ctrl,
    .p_cfg  = &g_comms_uart_device0_cfg,
    .p_api  = &g_comms_on_comms_uart,
};
#endif /* 1 <= COMMS_UART_CFG_DEVICE_NUM_MAX */

#if (1 <= COMMS_UART_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_uart_bus0_callback
 * @brief User callback function called in R_Config_SCI UART driver
 * @param[in] event
 * @retval None
 *********************************************************************************************************************/
void rm_comms_uart_bus0_callback(rm_comms_uart_event_t event)
{
    rm_comms_uart_callback(&g_comms_uart_device0_ctrl, event);
}
/**********************************************************************************************************************
 * End of function rm_comms_uart_bus0_callback
 *********************************************************************************************************************/
#endif /* 1 <= COMMS_UART_CFG_BUS_NUM_MAX */

/******************************************************************************************************************//**
 * @} (end addtogroup rm_comms_uart_instance_rx)
 *********************************************************************************************************************/
