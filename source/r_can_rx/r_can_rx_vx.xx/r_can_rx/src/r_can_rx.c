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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_can_rx.c
* Description  : CAN FIT driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 08.04.2015 2.01    For 64M, 71M.
*                              - When writing 0 to bits NEWDATA, SENTDATA, MSGLOST, TRMABT, RECREQ, and TRMREQ
*                              by a program, do not use the logic operation instruction (AND). Write 0 to only the
*                              specified bit and write 1 to the other bits before using the transfer (MOV) instruction.
*                              See RX64M/71M UM 43.2.8.
*                              - R_CAN_TxCheck() and R_CAN_TxStopMsg() modified because of note
*                              "Bits SENTDATA and TRMREQ cannot be set to 0 simultaneously." See RX64M/71M UM 43.2.8.
*         : 30.10.2015 2.02    - Updates to code packaging for FIT.
*                              - R_CAN_Create(): Added arguments for interrupt callbacks.
*                              - Changed R_CAN_RxRead(): The IDE bit is only valid in mixed mode.
*         : 09.06.2016 2.10    - 65N added.
*                              - Set IDE bit according to requested frame type for mixed ID mode only.
*                              - Fixed R_CAN_RxRead() Mixed mode: Else-block missing. STD frame ID not copied.
*                              - R_CAN_Control()
*                                   o Cases EXITSLEEP_CANMODE and ENTERSLEEP_CANMODE had 0 instead of ch_nr.
*                                   o Case OPERATE_CANMODE: Checking also that Halt Status bit is 0.
*         : 01.30.2017 2.11    - Const was added to CAN_port_map_t so that there is no need to add const to
*                                instantiations.
*                              - Test ran with 65N 2MB.
*                              - Some GSCE coding guidelines implemented. Mulitple lines changed. (Plugin was used.)
*         : 08.14.2017 2.12    - Fixed CAN_ERS_ISR() function to check interrupt sources for all channels.
*         : 10.26.2018 2.13    - Fixed comment field "Function name:  CAN2_TXM2_ISR" causing compilation error
*                                when CAN2 enabled.
*         : 08.01.2019 2.15    - Fixed default callback assignments for can_rx_callback and can_err_callback.
*         : 05.04.2019 3.00    - Added support for GCC and IAR compilers
*         : 16.09.2019 3.11    - Added macro CAN_USE_CAN0, CAN_USE_CAN1, CAN_USE_CAN2 in config_can_interrupts()
*                                function, above CAN0_TXM0_ISR(), CAN0_RXM0_ISR(), CAN1_TXM1_ISR(), CAN1_RXM1_ISR(),
*                                CAN2_TXM2_ISR() and CAN2_RXM2_ISR() function to prevent interrupt vector number are
*                                undefined when does not use CAN0 , CAN1 or CAN2.
*         : 30.12.2019 3.20    - Modified comment of API function to Doxygen style.
*                              - Added support for atomic control.
*                              - Removed support for Generation 1 devices.
*                              - Fixed to comply with GSCE Coding Standards Rev.6.00.
*         : 13.06.2020 4.00    - Added support CAN FIFO.
*                              - Removed source code related to Pin-setting.
*                              - Fixed R_CAN_Control() in case EXITSLEEP_CANMODE.
*                              - Fixed processing in R_CAN_RxSetMask() function.
*         : 04.01.2021 4.10    - Changed can_tx_callback to can_txf_callback in case txf_cb_func is NULL
*                                in R_CAN_Create().
*         : 01.04.2021 5.00    - Added support for setting different bitrate for different channels.
*         : 07.04.2021 5.10    - Fixed comment of R_CAN_RxSetFIFO and R_CAN_RxSetFIFOXid.
*                              - Updated Doxygen comment.
*         : 08.09.2023 5.50    - Updated according to GSCE Code Checker 6.50.
*                              - Added WAIT_LOOP comments.
***********************************************************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include "platform.h"
#include "r_can_rx_config.h"
#include "r_can_rx_if.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Macro definitions
******************************************************************************/
/* These macros are for determining if some can while loop times out. If they do,
the canx_sw_err variable will be non zero. This is to prevent an error in the 
can peripheral and driver from blocking the CPU indefinitely. */
#define DEC_CHK_CAN_SW_TMR      (--can_tmo_cnt != 0)
#define MAX_CAN_SW_DELAY        (0x2000)

#define CHECK_MBX_NR            {if (mbox_nr > 31) return R_CAN_SW_BAD_MBX;}
#define CHECK_TXFIFO_MBX_NR     {if (mbox_nr > 27) return R_CAN_SW_BAD_MBX;}
#define CHECK_RXFIFO_MBX_NR     {if (((mbox_nr > 23) && (mbox_nr < 28)) || mbox_nr > 31 ) return R_CAN_SW_BAD_MBX;}
#define CHECK_MBX_MODE          {if (mb_mode > 1) return R_CAN_SW_BAD_MODE;}

/* Define a mask for MSB of a long to serve as an extended ID mode bit flag. */
/* Extended ID occupies lower 29 bits, so use this to mask off upper 3 bits. */ 
#define XID_MASK                (0xE0000000)

/* Define a mask for the 11 bits that make up a standard ID. */ 
#define SID_MASK                (0x000007FF)

/* make sure that MAX_CHANNELS = the number of CAN channels */
#define MAX_CHANNELS (sizeof(CAN_CHANNELS)/sizeof(can_st_ptr))

/******************************************************************************
Global variables and functions imported (externs)
******************************************************************************/
/******************************************************************************
Constant definitions
******************************************************************************/
/* Mem. area for bit set defines */
static const uint32_t  bit_set[32] = 
{
    MBX_0,  MBX_1,  MBX_2,  MBX_3, 
    MBX_4,  MBX_5,  MBX_6,  MBX_7,
    MBX_8,  MBX_9,  MBX_10, MBX_11, 
    MBX_12, MBX_13, MBX_14, MBX_15,
    MBX_16, MBX_17, MBX_18, MBX_19,
    MBX_20, MBX_21, MBX_22, MBX_23, 
    MBX_24, MBX_25, MBX_26, MBX_27,
    MBX_28, MBX_29, MBX_30, MBX_31, 
};

static const can_st_ptr CAN_CHANNELS[] =
{
    #ifdef CAN0
        &CAN0,
    #endif
    #ifdef CAN1
        &CAN1,
    #endif
    #ifdef CAN2
        &CAN2
    #endif
};

/******************************************************************************
Global variables and functions private to the file
******************************************************************************/
/* Data */
/* Functions */
static uint32_t can_wait_tx_rx (const uint32_t ch_nr, const uint32_t mbox_nr);
static void     config_can_interrupts (const uint32_t ch_nr);
static void     universal_can_callback (void);
static void     can_module_stop_state_cancel (const uint32_t ch_nr);

/* User-function vectors for the CAN interrupts. These will be populated by R_CAN_Create(). */
#ifdef CAN2     /* Three CAN channels exist on device. */
    void (*can_tx_callback[MAX_CHANNELS])(void)     = {NULL, NULL, NULL};
    void (*can_txf_callback[MAX_CHANNELS])(void)    = {NULL, NULL, NULL};
    void (*can_rx_callback[MAX_CHANNELS])(void)     = {NULL, NULL, NULL};
    void (*can_rxf_callback[MAX_CHANNELS])(void)    = {NULL, NULL, NULL};
    void (*can_err_callback[MAX_CHANNELS])(void)    = {NULL, NULL, NULL};
#else
#ifdef CAN1     /* Two CAN channels exist. */
    void (*can_tx_callback[MAX_CHANNELS])(void)     = {NULL, NULL};
    void (*can_txf_callback[MAX_CHANNELS])(void)    = {NULL, NULL};
    void (*can_rx_callback[MAX_CHANNELS])(void)     = {NULL, NULL};
    void (*can_rxf_callback[MAX_CHANNELS])(void)    = {NULL, NULL};
    void (*can_err_callback[MAX_CHANNELS])(void)    = {NULL, NULL};
#else           /* Only one CAN channel on device. */
    void (*can_tx_callback[MAX_CHANNELS])(void)     = {NULL};
    void (*can_txf_callback[MAX_CHANNELS])(void)    = {NULL};
    void (*can_rx_callback[MAX_CHANNELS])(void)     = {NULL};
    void (*can_rxf_callback[MAX_CHANNELS])(void)    = {NULL};
    void (*can_err_callback[MAX_CHANNELS])(void)    = {NULL};
#endif
#endif

/******************************************************************************


                                C A N   A P I


******************************************************************************/
/***********************************************************************************************************************
* Function Name: R_CAN_Create
********************************************************************************************************************//**
* @brief Initializes CAN peripheral - Sets user communication callback functions, configures CAN interrupts, sets
* different bitrate for different channels, mailbox defaults, and enters CAN Operation Mode.\n
* This function sets the CAN interrupt levels and user callbacks. This function will also call R_CAN_SetBitrate()
* and sets the mask to default: not mask any frames.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mb_mode - Normal mailbox (0), FIFO mailbox (1).
* @param[in] p_cfg - It is an address to the data structure containing the BRP, TSEG1, TSEG2, and SJW that constitute
* the bitrate for channel ch_nr.
* @param[in] tx_cb_func - The name of a function in your application which will be called by the CAN driver when
* a mailbox has finished transmitting. If you are using polled mode, or do not want a callback for interrupt mode
* for some reason, specify NULL.
* @param[in] txf_cb_func - The name of a function in your application which will be called by the CAN driver when
* every time mailbox in the transmit FIFO has finished transmitting or the transmit FIFO becomes empty due to completion
* of transmission. If you do not want a callback for interrupt mode for some reason, specify NULL.
* @param[in] rx_cb_func - The name of a function in your application which will be called by the CAN driver when a
* mailbox has finished receiving. If you are using polled mode, or do not want a callback for interrupt mode for
* some reason, specify NULL.
* @param[in] rxf_cb_func - The name of a function in your application which will be called by the CAN driver when
* every time mailbox in  the receive FIFO has finished receiving or the receive FIFO becomes buffer warning by
* completion of reception. If you do not want a callback for interrupt mode for some reason, specify NULL.
* @param[in] err_cb_func - The name of a function in your application which will be called by the CAN driver when
* there is a CAN error. If you are using polled mode, or do not want a callback for interrupt mode for some reason,
* specify NULL.
* @retval R_CAN_OK                Action completed successfully.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_BAD_MODE          The mode number does not exist.
* @retval R_CAN_SW_RST_ERR        The CAN peripheral did not enter Reset mode
* @retval R_CAN_MODULE_STOP_ERR   Whole CAN peripheral is in stop state (low power). Perhaps the PRCR register was
* not used to unlock the module stop register.\n
* See also R_CAN_Control() return values.
* @details This function wakes the peripheral from CAN Sleep mode and puts it in CAN Reset mode. It configures the
* mailboxes with these default settings:\n
* Sets mailbox mode: either normal mailbox mode or FIFO mailbox mode.\n
* Overwrite an unread mailbox data when new frames arrive.\n
* Sets the device to use ID priority (normal CAN behavior, not the optional mailbox number priority).\n
* Sets all mailboxes' masks invalid.\n
* R_CAN_Create calls the R_CAN_SetBitrate function and configures CAN interrupts if USE_CAN_POLL is commented
* in r_can_rx_config.h.\n
* Before returning, it clears all mailboxes, sets the peripheral into Operation mode, and clears any errors.
* @note Users need to declare the baud rate prescaler division and bit timing values to set the bitrate of the CAN
* channel through the p_cfg argument before call R_CAN_Create() function.
*/
uint32_t R_CAN_Create(const uint32_t ch_nr,
                    const uint32_t mb_mode,
                    const can_bitrate_config_t p_cfg,
                    void (*tx_cb_func)(void),
                    void (*txf_cb_func)(void),
                    void (*rx_cb_func)(void),
                    void (*rxf_cb_func)(void),
                    void (*err_cb_func)(void))
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status = R_CAN_OK;

    uint32_t i  = 0;

    uint32_t j  = 0;

    /* A faulty CAN peripheral block, due to HW, FW could potentially block (hang)
    the program at a while-loop. To prevent this, a sw timer in the while-loops
    will time out enabling the CPU to continue. */
    uint32_t can_tmo_cnt = MAX_CAN_SW_DELAY;

    /* check mailbox mode */
    CHECK_MBX_MODE

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    if (NULL == tx_cb_func)
    {
        /* Set the interrupt to a default function as a safety precaution. Even if in polled mode. */
        can_tx_callback[ch_nr] = universal_can_callback;
    }
    else
    {
        /* Set what user function to execute when a CAN transmit interrupt occurs. */
        can_tx_callback[ch_nr] = tx_cb_func;
    }

    if (NULL == txf_cb_func)
    {
        /* Set the interrupt to a default function as a safety precaution. Even if in polled mode. */
        can_txf_callback[ch_nr] = universal_can_callback;
    }
    else
    {
        /* Set what user function to execute when a CAN transmit interrupt occurs. */
        can_txf_callback[ch_nr] = txf_cb_func;
    }

    if (NULL == rx_cb_func)
    {
        /* Set the interrupt to a default function as a safety precaution. Even if in polled mode. */
        can_rx_callback[ch_nr] = universal_can_callback;
    }
    else
    {
        /* Set what user function to execute when a CAN receive interrupt occurs. */
        can_rx_callback[ch_nr] = rx_cb_func;
    }

    if (NULL == rxf_cb_func)
    {
        /* Set the interrupt to a default function as a safety precaution. Even if in polled mode. */
        can_rxf_callback[ch_nr] = universal_can_callback;
    }
    else
    {
        /* Set what user function to execute when a CAN receive interrupt occurs. */
        can_rxf_callback[ch_nr] = rxf_cb_func;
    }

    if (NULL == err_cb_func)
    {
        /* Set the interrupt to a default function as a safety precaution. Even if in polled mode. */
        can_err_callback[ch_nr] = universal_can_callback;
    }
    else
    {
        /* Set what user function to execute when a CAN ERROR interrupt occurs. */
        can_err_callback[ch_nr] = err_cb_func;
    }

    can_module_stop_state_cancel(ch_nr); /* exit module stop state */

    if (SYSTEM.MSTPCRB.LONG & (1 << ch_nr)) /* Check bits 0, 1 or 2 for channel 0, 1 or 2 */
    {
        /* Module stop state bit did not clear. PRCR is probably locked. */
        return (R_CAN_MODULE_STOP_ERR);
    }

    /* Exit Sleep mode. This will also take us from HALT mode to OPERATE_CANMODE. */
    api_status |= R_CAN_Control(ch_nr, EXITSLEEP_CANMODE);

    /*** Set CAN Control register.***/

    /* BOM:    Bus Off recovery mode acc. to IEC11898-1 */
    can_block_p->CTLR.BIT.BOM = 0;

    if (FIFO_MAILBOX_MODE == mb_mode)    /* check normal or fifo mode */
    {
        /* MBM: Select FIFO mailbox mode. */
        can_block_p->CTLR.BIT.MBM = 1;
    }
    else
    {
        /* MBM: Select normal mailbox mode. */
        can_block_p->CTLR.BIT.MBM = 0;
    }

    /* IDFM: Select Frame ID mode. */
    can_block_p->CTLR.BIT.IDFM = FRAME_ID_MODE;

    /*  0 = Overwrite mode: Latest message overwrites old.
        1 = Overrun mode: Latest message discarded. */
    can_block_p->CTLR.BIT.MLM = 0;

    /* TPM: ID priority mode. */
    can_block_p->CTLR.BIT.TPM = 0;

    /* TSRC: Only to be set to 1 in operation mode */
    can_block_p->CTLR.BIT.TSRC = 0;

    /* TSPS: Update every 8 bit times */
    can_block_p->CTLR.BIT.TSPS = 3;

    /* Set BAUDRATE */
    R_CAN_SetBitrate(ch_nr, p_cfg);

    /* Mask invalid for all mailboxes by default. */
    can_block_p->MKIVLR.LONG = 0xFFFFFFFF;

    /* Configure CAN interrupts. */
    config_can_interrupts(ch_nr);

    /* Reset -> HALT mode ************************************************************/
    api_status |= R_CAN_Control(ch_nr, HALT_CANMODE);

    /* Clear mailboxes in Halt mode. */
    /* WAIT_LOOP */
    for (i = 0; i < 32; i++)
    {
        can_block_p->MB[i].ID.LONG = 0x00;

        can_block_p->MB[i].DLC = 0x0000;

        /* WAIT_LOOP */
        for (j = 0; j < 8; j++)
        {
            can_block_p->MB[i].DATA[j] = 0x00;
        }

        /* WAIT_LOOP */
        for (j = 0; j < 2; j++)
        {
            can_block_p->MB[i].TS = 0x0000;
        }
    }

    /* Time Stamp Counter reset. Set the TSRC bit to 1 in CAN Operation mode. */
    can_block_p->CTLR.BIT.TSRC = 1;

    /* WAIT_LOOP */
    while ((can_block_p->CTLR.BIT.TSRC) && DEC_CHK_CAN_SW_TMR)
    {
        /* Wait loop. */
        R_BSP_NOP();
    }
    if (0 == can_tmo_cnt)
    {
        api_status |= R_CAN_SW_TSRC_ERR;
    }

    /* Check for errors so far, report, and clear. */
    if (can_block_p->STR.BIT.EST)
    {
        api_status |= R_CAN_SW_RST_ERR;
    }

    /* Clear Error Interrupt Factor Judge Register. */
    if (can_block_p->EIFR.BYTE)
    {
        api_status |= R_CAN_SW_RST_ERR;
    }
    can_block_p->EIFR.BYTE = 0x00;

    /* Clear Error Code Store Register. */
    if (can_block_p->ECSR.BYTE)
    {
        api_status |= R_CAN_SW_RST_ERR;
    }
    can_block_p->ECSR.BYTE = 0x00;

    return (api_status);
}
/******************************************************************************
 End of function R_CAN_Create
 *****************************************************************************/ /* end R_CAN_Create() */

/***********************************************************************************************************************
* Function Name: R_CAN_PortSet
********************************************************************************************************************//**
* @brief Configures the MCU and transceiver port pins. This function is responsible for configuring the MCU and
* transceiver port pins. Transceiver port pins such as Enable will vary depending on design, and this function must
* then be modified. The function is also used to enter the CAN port test modes, such as Listen Only.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] action_type \n
* Port actions:\n
*  ENABLE                      Enable the CAN port pins and the CAN transceiver.\n
*  DISABLE                     Disable the CAN port pins and the CAN transceiver.\n
*  CANPORT_TEST_LISTEN_ONLY    Set to Listen Only mode. No ACKs or Error frames are sent.\n
*  CANPORT_TEST_0_EXT_LOOPBACK Use external bus and loopback. Useful for initial debug. See separate test section.\n
*  CANPORT_TEST_1_INT_LOOPBACK Only internal mailbox communication. Useful for initial debug. See separate test
*                               section.\n
*  CANPORT_RETURN_TO_NORMAL    Return to normal port usage.\n
* @retval R_CAN_OK                Action completed successfully.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_BAD_ACTION_TYPE   No such action type exists for this function.
* @retval R_CAN_SW_HALT_ERR       The CAN peripheral did not enter Halt mode.
* @retval R_CAN_SW_RST_ERR        The CAN peripheral did not enter Reset mode.\n
* See also R_CAN_Control() return values.
* @details Unless Internal Loopback mode is used (for initial test and debug) make sure this function is called after
* any board default port set up function is used (e.g. 'hwsetup').\n
* Observe that a stray output high/low on an MCU CAN port pin that was set by some other (default) board setup code
* could affect the bus negatively. You may discover that a hard reset on a node could cause other nodes to go into
* error mode. The reason may be that all ports were set as default output hi/low before CAN reconfigures the ports.
* Such code should be removed, or else, for a brief period of time, the ports may be output low/high and disrupt the
* CAN bus voltage level.\n
* You may have to change/add transceiver port pins according to your transceiver.\n
*/
uint32_t R_CAN_PortSet(const uint32_t ch_nr, const uint32_t action_type)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status = R_CAN_OK;

    if (ch_nr < MAX_CHANNELS) 
    { 
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else 
    {
        return (R_CAN_BAD_CH_NR);
    }

    switch (action_type)
    {
        case ENABLE:

            /* Port pin function select register setting */
            MPC.PWPR.BYTE = 0x00;    /* PWPR.PFSWE write protect off */
            MPC.PWPR.BYTE = 0x40;    /* PFS register write protect off */

            #if defined(CAN0) && defined(CAN0_TRX_STB_PORT) && defined(CAN0_TRX_STB_PIN)
            if (ch_nr == 0)
            {
                /* Configure CAN0 STBn pin. See config.h. */
                CAN_TRX_DDR(CAN0_TRX_STB_PORT, CAN0_TRX_STB_PIN) = 1;
                CAN_TRX_DR( CAN0_TRX_STB_PORT, CAN0_TRX_STB_PIN) = CAN0_TRX_STB_LVL;

                /* Configure CAN0 EN pin. */
                CAN_TRX_DDR(CAN0_TRX_ENABLE_PORT, CAN0_TRX_ENABLE_PIN) = 1;
                CAN_TRX_DR( CAN0_TRX_ENABLE_PORT, CAN0_TRX_ENABLE_PIN) = CAN0_TRX_ENABLE_LVL;
            }
            #endif
            #if defined(CAN1) && defined(CAN1_TRX_STB_PORT) && defined(CAN1_TRX_STB_PIN)
            if (ch_nr == 1)
            {
                /* Configure CAN1 STBn pin. See config.h. */
                CAN_TRX_DDR(CAN1_TRX_STB_PORT, CAN1_TRX_STB_PIN) = 1;
                CAN_TRX_DR( CAN1_TRX_STB_PORT, CAN1_TRX_STB_PIN) = CAN1_TRX_STB_LVL;

                /* Configure CAN1 EN pin. */
                CAN_TRX_DDR(CAN1_TRX_ENABLE_PORT, CAN1_TRX_ENABLE_PIN) = 1;
                CAN_TRX_DR( CAN1_TRX_ENABLE_PORT, CAN1_TRX_ENABLE_PIN) = CAN1_TRX_ENABLE_LVL;
            }
            #endif
            #if defined(CAN2) && defined(CAN2_TRX_STB_PORT) && defined(CAN2_TRX_STB_PIN)
            if (ch_nr == 2)
            {
                /* Configure CAN2 STBn pin. See config.h. */
                CAN_TRX_DDR(CAN2_TRX_STB_PORT, CAN2_TRX_STB_PIN) = 1;
                CAN_TRX_DR( CAN2_TRX_STB_PORT, CAN2_TRX_STB_PIN) = CAN2_TRX_STB_LVL;

                /* Configure CAN2 EN pin. */
                CAN_TRX_DDR(CAN2_TRX_ENABLE_PORT, CAN2_TRX_ENABLE_PIN) = 1;
                CAN_TRX_DR( CAN2_TRX_ENABLE_PORT, CAN2_TRX_ENABLE_PIN) = CAN2_TRX_ENABLE_LVL;
            }
            #endif

            MPC.PWPR.BYTE = 0x80;    /* PFS register write protect on */
            break;

        case DISABLE:

            /* Port pin function select register setting */
            MPC.PWPR.BYTE = 0x00;    /* PWPR.PFSWE write protect off */
            MPC.PWPR.BYTE = 0x40;    /* PFS register write protect off */

            #if defined(CAN0) && defined(CAN0_TRX_STB_PORT) && defined(CAN0_TRX_STB_PIN)
            if (ch_nr == 0)
            {
                /* Configure CAN0 STBn pin. See config.h. */
                CAN_TRX_DDR( CAN0_TRX_STB_PORT, CAN0_TRX_STB_PIN ) = 1;
                CAN_TRX_DR( CAN0_TRX_STB_PORT, CAN0_TRX_STB_PIN) = !CAN0_TRX_STB_LVL; /* Negated level. */

                /* Configure CAN0 EN pin. */
                CAN_TRX_DDR(CAN0_TRX_ENABLE_PORT, CAN0_TRX_ENABLE_PIN) = 1;
                CAN_TRX_DR( CAN0_TRX_ENABLE_PORT, CAN0_TRX_ENABLE_PIN) = !CAN0_TRX_ENABLE_LVL; /* Negated level. */
            }
            #endif
            #if defined(CAN1) && defined(CAN1_TRX_STB_PORT) && defined(CAN1_TRX_STB_PIN)
            if (ch_nr == 1)
            {
                /* Configure CAN1 STBn pin. See config.h. */
                CAN_TRX_DDR(CAN1_TRX_STB_PORT, CAN1_TRX_STB_PIN) = 1;
                CAN_TRX_DR( CAN1_TRX_STB_PORT, CAN1_TRX_STB_PIN) = !CAN1_TRX_STB_LVL;

                /* Configure CAN1 EN pin. */
                CAN_TRX_DDR(CAN1_TRX_ENABLE_PORT, CAN1_TRX_ENABLE_PIN ) = 1;
                CAN_TRX_DR( CAN1_TRX_ENABLE_PORT, CAN1_TRX_ENABLE_PIN) = !CAN1_TRX_ENABLE_LVL;
            }
            #endif
            #if defined(CAN2) && defined(CAN2_TRX_STB_PORT) && defined(CAN2_TRX_STB_PIN)
            if (ch_nr == 2)
            {
                /* Configure CAN2 STBn pin. See config.h. */
                CAN_TRX_DDR(CAN2_TRX_STB_PORT, CAN2_TRX_STB_PIN) = 1;
                CAN_TRX_DR( CAN2_TRX_STB_PORT, CAN2_TRX_STB_PIN) = !CAN2_TRX_STB_LVL;

                /* Configure CAN2 EN pin. */
                CAN_TRX_DDR(CAN2_TRX_ENABLE_PORT, CAN2_TRX_ENABLE_PIN) = 1;
                CAN_TRX_DR( CAN2_TRX_ENABLE_PORT, CAN2_TRX_ENABLE_PIN) = !CAN2_TRX_ENABLE_LVL;
            }
            #endif

            MPC.PWPR.BYTE = 0x80;    /* PFS register write protect on */        

            /* Go to OPERATION mode *********************************************************/
            api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
            break;

        /* Run in Listen Only test mode. */
        case CANPORT_TEST_LISTEN_ONLY:
            api_status = R_CAN_Control(ch_nr, HALT_CANMODE);

            can_block_p->TCR.BYTE = 0x03;

            api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
            api_status |= R_CAN_PortSet(ch_nr, ENABLE);
            break;

        /* Run in External Loopback test mode. */
        case CANPORT_TEST_0_EXT_LOOPBACK:
            api_status = R_CAN_Control(ch_nr, HALT_CANMODE);

            can_block_p->TCR.BYTE = 0x05;

            api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
            api_status |= R_CAN_PortSet(ch_nr, ENABLE);
            break;

        /* Run in Internal Loopback test mode. */
        case CANPORT_TEST_1_INT_LOOPBACK:
            api_status = R_CAN_Control(ch_nr, HALT_CANMODE);

            can_block_p->TCR.BYTE = 0x07;

            api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
            break;

        /* Return to default CAN bus mode. 
        This is the default setting at CAN reset. */
        case CANPORT_RETURN_TO_NORMAL:
            api_status = R_CAN_Control(ch_nr, HALT_CANMODE);

            can_block_p->TCR.BYTE = 0x00;

            api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
            api_status |= R_CAN_PortSet(ch_nr, ENABLE);
            break;

        default:

            /* Bad action type. */
            api_status = R_CAN_BAD_ACTION_TYPE;
            break;
    }
    return (api_status);
}
/******************************************************************************
 End of function R_CAN_PortSet
 *****************************************************************************/ /* end R_CAN_PortSet() */

/***********************************************************************************************************************
* Function Name: R_CAN_Control
********************************************************************************************************************//**
* @brief Set CAN operating modes. Controls transition to CAN operating modes determined by the CAN Control
* register. For example, the Halt mode should be used to later configure a receive mailbox.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] action_type \n
* Peripheral actions:\n
*  EXITSLEEP_CANMODE       Exit CAN Sleep mode, the default state when the peripheral starts up.\n
*  ENTERSLEEP_CANMODE      Enter CAN Sleep mode to save power.\n
*  RESET_CANMODE           Put the CAN peripheral into Reset mode.\n
*  HALT_CANMODE            Put the CAN peripheral into Halt mode. CAN peripheral is still connected to the bus,
*                          but stops communicating.\n
*  OPERATE_CANMODE         Put the CAN peripheral into normal Operation mode.\nd
* @retval R_CAN_OK                Action completed successfully.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_BAD_ACTION_TYPE   No such action type exists for this function.
* @retval R_CAN_SW_WAKEUP_ERR     The CAN peripheral did not wake up from Sleep mode.
* @retval R_CAN_SW_SLEEP_ERR      The CAN peripheral did not enter Sleep mode.
* @retval R_CAN_SW_HALT_ERR       The CAN peripheral did not enter Halt mode.
* @retval R_CAN_SW_RST_ERR        The CAN peripheral did not enter Reset mode.\n
* See also R_CAN_PortSet() return values.
* @details Other than calling this API to enter Halt mode, CAN mode transitions are called via the other API
* functions automatically. For example, the default mode when starting up is CAN Sleep mode. Use the API to switch
* to other operating modes, for example first ‘Exit Sleep' followed by ‘Reset' to initialize the CAN registers for
* bitrate and interrupts, then enter ‘Halt' mode to configure mailboxes.
*/
uint32_t R_CAN_Control(const uint32_t  ch_nr, const uint32_t  action_type)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t    api_status  = R_CAN_OK;
    uint32_t    can_tmo_cnt = MAX_CAN_SW_DELAY;

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    switch (action_type)
    {
        case EXITSLEEP_CANMODE:

            /* Set to Not Sleep, and ensure that RCAN exits in Operate mode.
            HW manual note says to write to the SLPM bit in CAN Halt mode,
            but if we currently are in Sleep mode, we should already also be
            in Halt mode. (See ENTERSLEEP_CANMODE below). */
            can_block_p->CTLR.BIT.SLPM = CAN_NOT_SLEEP;

            /* WAIT_LOOP */
            while ((can_block_p->STR.BIT.SLPST) && DEC_CHK_CAN_SW_TMR)
            {
                R_BSP_NOP();
            }
            if (0 == can_tmo_cnt)
            { 
                api_status = R_CAN_SW_WAKEUP_ERR;
            }
            R_CAN_Control(ch_nr, RESET_CANMODE);
            break;
        case ENTERSLEEP_CANMODE:

            /* Set to, and ensure that RCAN returns in, the Sleep state.
            Write to the SLPM bit in CAN Reset or CAN Halt modes. */
            api_status = R_CAN_Control(ch_nr, HALT_CANMODE);

            can_block_p->CTLR.BIT.SLPM = CAN_SLEEP;

            /* WAIT_LOOP */
            while ((!can_block_p->STR.BIT.SLPST) && DEC_CHK_CAN_SW_TMR)
            {
                R_BSP_NOP();
            }
            if (0 == can_tmo_cnt)
            {
                api_status = R_CAN_SW_SLEEP_ERR;
            }
            break;

        case RESET_CANMODE:

            /* Set to, and ensure that RCAN returns in, the Reset state. */
            can_block_p->CTLR.BIT.CANM = CAN_RESET;

            /* WAIT_LOOP */
            while ((!can_block_p->STR.BIT.RSTST) && DEC_CHK_CAN_SW_TMR)
            {
                /* Wait loop. */
                R_BSP_NOP();
            }
            if (0 == can_tmo_cnt)
            {
                api_status = R_CAN_SW_RST_ERR;
            }
            break;

        case HALT_CANMODE:

            /* Set to, and ensure that RCAN returns in, the Halt state. */
            /* The CAN module enters CAN Halt mode after waiting for the end of 
            message reception or transmission. */
            can_block_p->CTLR.BIT.CANM = CAN_HALT;

            /* WAIT_LOOP */
            while ((!can_block_p->STR.BIT.HLTST) && DEC_CHK_CAN_SW_TMR)
            {
                /* Wait loop. */
                R_BSP_NOP();
            }
            if (0 == can_tmo_cnt)
            {
                api_status = R_CAN_SW_HALT_ERR;
            }
            break;

        case OPERATE_CANMODE:  

            /* Take CAN out of Stop mode. */    
            can_module_stop_state_cancel(ch_nr); /* exit module stop state */

            /* Set to Operate mode. */
            can_block_p->CTLR.BIT.CANM = CAN_OPERATION;

            /* Ensure that RCAN is in Operation mode. */
            /* WAIT_LOOP */
            while (((can_block_p->STR.BIT.HLTST) || (can_block_p->STR.BIT.RSTST)) && DEC_CHK_CAN_SW_TMR)
            {
                /* Wait loop. */
                R_BSP_NOP();
            }
            if (0 == can_tmo_cnt)
            {
                api_status = R_CAN_SW_RST_ERR;
            }
            break;
        default:
            api_status = R_CAN_BAD_ACTION_TYPE;
            break;
    }

    return (api_status);
}
/******************************************************************************
 End of function R_CAN_Control
 *****************************************************************************/ /* end R_CAN_Control() */

/***********************************************************************************************************************
* Function Name: R_CAN_TxSet
********************************************************************************************************************//**
* @brief Set up a mailbox to transmit. The API will write to a mailbox the specified ID, data length and data
* frame payload, then set the mailbox to transmit mode and send a frame onto the bus by calling R_CAN_Tx().
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mb_mode - Normal mailbox (0), FIFO mailbox (1).
* @param[in] mbox_nr - Mailbox to use.
* @param[in] frame_p - Pointer to a data frame structure in memory. It is an address to the data structure containing
* the ID, DLC and data that constitute the data frame the mailbox will transmit.
* @param[in] frame_type \n
*  DATA_FRAME      Send a normal data frame.\n
*  REMOTE_FRAME    Send a remote data frame request.
* @retval R_CAN_OK                The mailbox was set up for transmission.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_BAD_MODE          The mode number does not exist.
* @retval CAN_ERR_BOX_FULL        Receive FIFO is full (4 unread messages).
* @retval R_CAN_BAD_ACTION_TYPE   No such action type exists for this function.
* @details This function sets up transmitting for normal mailboxes or transmit FIFO mailboxes.\n
* To transmit FIFO mailboxes, this function first interrupt disables the mailbox temporarily when setting up the maibox.
* It then check to ensure transmit FIFO is not full to perform setting up the mailbox: Copies the data frame payload
* bytes (0-7) into the mailbox, selects data frame or remote frame request, sets the ID value for the mailbox and
* finally the Data Length Code indicated by frame_p. The mailbox is interrupt enabled as well as transmit FIFO interrupt
* generation timing again unless USE_CAN_POLL was defined. Finally R_CAN_Tx is called to deliver the message.\n
* To normal mailboxes, this function first waits for any previous transmission of the specified mailbox to complete.
* It then interrupt disables the mailbox temporarily when setting up the mailbox: Sets the ID value for the mailbox,
* the Data Length Code indicated by frame_p, selects dataframe or remote frame request and finally copies the data frame
* payload bytes (0-7) into the mailbox. The mailbox is interrupt enabled again unless USE_CAN_POLL was defined.
* Finally R_CAN_Tx is called to deliver the message.
*/
uint32_t R_CAN_TxSet(const uint32_t     ch_nr,
                    const uint32_t      mb_mode,
                    const uint32_t      mbox_nr,
                    const can_frame_t*  frame_p,
                    const uint32_t      frame_type)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t    api_status  = R_CAN_OK;

    uint32_t    i = 0;

    /* check mailbox mode */
    CHECK_MBX_MODE

    /* check mailbox for fifo mode: normal mailbox: 0 --> 23,
     * Transmit fifo mailbox: 24 --> 27;
     * return if number of mailbox is receive fifo mailbox(28 -->31) or higher */
    if (FIFO_MAILBOX_MODE == mb_mode)    /* check normal or fifo mailbox mode */
    {
        /* return if MBX is receive FIFO mailbox */
        CHECK_TXFIFO_MBX_NR
    }
    else
    {
        CHECK_MBX_NR
    }

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    if ((FIFO_MAILBOX_MODE == mb_mode)
            && (bit_set[mbox_nr] & 0x0f000000))    /* check fifo mailbox mode and using FIFO mailbox */
    {
        /* In FIFO mailbox mode, change the bits in MIER for the related FIFO only when
         * the TFE bit in TFCR is 0 and the TFEST flag is 1 */
        if ((0 == can_block_p->TFCR.BIT.TFE) && (1 == can_block_p->TFCR.BIT.TFEST))
        {
            /* Interrupt disable the mailbox.in case it was a receive mailbox */
            can_block_p->MIER.LONG &= ((~(bit_set[24])));
        }

        /* return if transmit fifo mailbox is full */
        if (1 == can_block_p->TFCR.BIT.TFFST)
        {
            return (CAN_ERR_BOX_FULL);
        }

        /* Copy frame data into mailbox */
        /* WAIT_LOOP */
        for (i = 0; ((i < frame_p->dlc) && (i<8)); i++)
        {
            can_block_p->MB[24].DATA[i] = frame_p->data[i];
        }

        /* Frame select: Data frame = 0, Remote = 1 */
        if (REMOTE_FRAME == frame_type)
        {
            can_block_p->MB[24].ID.BIT.RTR = 1;
        }
        else
        {
            can_block_p->MB[24].ID.BIT.RTR = 0;
        }

        /*** Set Mailbox ID based on ID mode ***/
        if (frame_p->id & XID_MASK)    /* Check for XID flag bit set in ID field */
        {
            /* Set message mailbox buffer Extended ID, masking off temporary XID flag bit. */
            can_block_p->MB[24].ID.LONG = (frame_p->id & (~XID_MASK));

            if (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM)
            {
                can_block_p->MB[24].ID.BIT.IDE = 1;   /* Mixed mode; select to send extended frame. */
            }
        }
        else
        {
            /* Set message mailbox buffer Standard ID.
             * Put only the lower 11 bit in the SID. */
            can_block_p->MB[24].ID.BIT.SID = (frame_p->id & SID_MASK);

            /* Always set IDE to 0 (unless mixed mode and extended frame as above). */
            can_block_p->MB[24].ID.BIT.IDE = 0;
        }

        /* Set the Data Length Code */
        can_block_p->MB[24].DLC = (unsigned short)frame_p->dlc;

        #if (USE_CAN_POLL == 0)
        /* In FIFO mailbox mode, change the bits in MIER for the related FIFO only when
         * the TFE bit in TFCR is 0 and the TFEST flag is 1 */
            if ((0 == can_block_p->TFCR.BIT.TFE) && (1 == can_block_p->TFCR.BIT.TFEST))
            {
                /* Interrupt enable the mailbox */
                can_block_p->MIER.LONG |= (bit_set[24]);

                /* Generate interrupt when FIFO last message transmission completed */
                if (CAN_CFG_TXFIFO_INT_GEN_TIMING == 1)
                {
                    can_block_p->MIER.LONG |= (bit_set[25]);
                }
            }
        #endif

        if (1 == can_block_p->TFCR.BIT.TFEST) /* confirm that the transmit FIFO empty status bit is set to 1. */
        {
            can_block_p->TFCR.BIT.TFE = 1; /* Transmit FIFO enabled. */
        }

        api_status |= R_CAN_Tx(ch_nr, mb_mode, mbox_nr);
    }

    /* Content of below else will occur in 2 case:
     * 1: Transmit message by MBX normal in FIFO mailbox mode when FIFO_MAILBOX_MODE == mb_mode
     * 2: Transmit message by MBX normal in normal mailbox mode when FIFO_MAILBOX_MODE != mb_mode */
    else
    {
        /* Wait for any previous transmission to complete. */
        api_status = can_wait_tx_rx(ch_nr, mbox_nr);

        /* Interrupt disable the mailbox.in case it was a receive mailbox */
        can_block_p->MIER.LONG &= ((~(bit_set[mbox_nr])));

        /* Clear message mailbox control register.
         * TRMREQ/RECREQ already 0 after can_wait_tx_rx(). (No need to write twice). */
        can_block_p->MCTL[mbox_nr].BYTE = 0;


        /*** Set Mailbox ID based on ID mode ***/
        if (frame_p->id & XID_MASK)    /* Check for XID flag bit set in ID field */
        {
            /* Set message mailbox buffer Extended ID, masking off temporary XID flag bit. */
            can_block_p->MB[mbox_nr].ID.LONG = (frame_p->id & (~XID_MASK));

            if (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM)
            {
                can_block_p->MB[mbox_nr].ID.BIT.IDE = 1;   /* Mixed mode; select to send extended frame. */
            }
        }
        else
        {
            /* Set message mailbox buffer Standard ID. Put only the lower 11 bit in the SID. */
            can_block_p->MB[mbox_nr].ID.BIT.SID = (frame_p->id & SID_MASK);

            /* Always set IDE to 0 (unless mixed mode and extended frame as above). */
            can_block_p->MB[mbox_nr].ID.BIT.IDE = 0;
        }

        /* Set the Data Length Code */
        can_block_p->MB[mbox_nr].DLC = (unsigned short)frame_p->dlc;

        /* Frame select: Data frame = 0, Remote = 1 */
        if (REMOTE_FRAME == frame_type)
        {
            can_block_p->MB[mbox_nr].ID.BIT.RTR = 1;
        }
        else
        {
            can_block_p->MB[mbox_nr].ID.BIT.RTR = 0;
        }

        /* Copy frame data into mailbox */
        /* WAIT_LOOP */
        for (i = 0; ((i < frame_p->dlc) && (i<8)); i++)
        {
            can_block_p->MB[mbox_nr].DATA[i] = frame_p->data[i];
        }

        #if (USE_CAN_POLL == 0)
            /* Interrupt enable the mailbox */
            can_block_p->MIER.LONG |= (bit_set[mbox_nr]);
        #endif

        api_status |= R_CAN_Tx(ch_nr, mb_mode, mbox_nr);
    }
    return (api_status);
}

/******************************************************************************
 End of function R_CAN_TxSet
 *****************************************************************************/ /* end R_CAN_TxSet() */

/***********************************************************************************************************************
* Function Name: R_CAN_TxSetXid
********************************************************************************************************************//**
* @brief Set up a mailbox to transmit.The API will write to a mailbox the specified ID, data length and data
* frame payload, then set the mailbox to transmit mode and send a frame onto the bus by calling R_CAN_Tx(). If this
* function is used, the ID will be a 29-bit ID.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mb_mode - Normal mailbox (0), FIFO mailbox (1).
* @param[in] mbox_nr - Mailbox to use.
* @param[in] frame_p - Pointer to a data frame structure in memory. It is an address to the data structure containing
* the ID, DLC and data that constitute the dataframe the mailbox will transmit.
* @param[in] frame_type \n
*  DATA_FRAME      Send a normal data frame.\n
*  REMOTE_FRAME    Send a remote data frame request.
* @retval R_CAN_OK                The mailbox was set up for transmission.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_BAD_MODE          The mode number does not exist.
* @retval CAN_ERR_BOX_FULL        Receive FIFO is full (4 unread messages).
* @retval R_CAN_BAD_ACTION_TYPE   No such action type exists for this function.
* @details This function sets up transmitting for normal mailboxes or transmit FIFO mailboxes.\n
* To transmit FIFO mailboxes, this function first interrupt disables the mailbox temporarily when setting up the maibox.
* It then check to ensure transmit FIFO is not full to perform setting up the mailbox: Copies the data frame payload
* bytes (0-7) into the mailbox, selects data frame or remote frame request, sets the ID value for the mailbox and
* finally the Data Length Code indicated by frame_p. The mailbox is interrupt enabled as well as transmit FIFO interrupt
* generation timing again unless USE_CAN_POLL was defined. Finally R_CAN_Tx is called to deliver the message.\n
* To normal mailboxes, this function first waits for any previous transmission of the specified mailbox to complete.
* It then interrupt disables the mailbox temporarily when setting up the mailbox: Sets the ID value for the mailbox,
* the Data Length Code indicated by frame_p, selects dataframe or remote frame request and finally copies the data frame
* payload bytes (0-7) into the mailbox. The mailbox is interrupt enabled again unless USE_CAN_POLL was defined.
* Finally R_CAN_Tx is called to deliver the message.
*/
uint32_t R_CAN_TxSetXid(const uint32_t     ch_nr,
                        const uint32_t     mb_mode,
                        const uint32_t     mbox_nr,
                        can_frame_t*       frame_p,
                        const uint32_t     frame_type)
{
    can_frame_t temp_frame;
    uint32_t    api_status;

    /* Copy the user frame to a temporary frame to which we add the Xid bit, 
    so that 29-bit ID will be used by R_CAN_TxSet(). The original frame is left 
    untouched (in case user later wants to send the same frame with standard ID). */
    memcpy(&temp_frame, frame_p, sizeof(can_frame_t));

    temp_frame.id |= XID_MASK;    /* Set XID flag bit set in ID field */

    api_status = R_CAN_TxSet(ch_nr, mb_mode, mbox_nr, (can_frame_t*)&temp_frame, frame_type);

    return (api_status);
}
/******************************************************************************
 End of function R_CAN_TxSetXid
 *****************************************************************************//* end R_CAN_TxSetXid() */

/***********************************************************************************************************************
* Function Name: R_CAN_Tx
********************************************************************************************************************//**
* @brief Starts actual message transmission onto the CAN bus. This API will wait until the mailbox finishes handling
* a prior frame, then set the mailbox to transmit mode.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mb_mode - Normal mailbox (0), FIFO mailbox (1).
* @param[in] mbox_nr - Which CAN mailbox to use. (0-32)
* @retval R_CAN_OK - The mailbox was set to transmit a previously configured mailbox.
* @retval R_CAN_SW_BAD_MBX - Bad mailbox number.
* @retval R_CAN_BAD_CH_NR - The channel number does not exist.
* @retval R_CAN_BAD_MODE - The mode number does not exist.
* @retval R_CAN_SW_SET_TX_TMO - Waiting for previous transmission to finish timed out.
* @retval R_CAN_SW_SET_RX_TMO - Waiting for previous reception to complete timed out.
* @details R_CAN_TxSet must have been called at least once for this mailbox after system start to set up the mailbox
* content, as this function only tells the mailbox to send its content.
*/
uint32_t R_CAN_Tx(const uint32_t  ch_nr, const uint32_t  mb_mode, const uint32_t  mbox_nr)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status = R_CAN_OK;

    /* check mailbox mode */
    CHECK_MBX_MODE

    /* check mailbox for fifo mode: normal mailbox: 0 --> 23,
     * Transmit fifo mailbox: 24 --> 27;
     * return if number of mailbox is receive fifo mailbox(28 -->31) or higher */
    if (FIFO_MAILBOX_MODE == mb_mode)    /* check normal or fifo mailbox mode */
    {
        /* return if MBX is receive FIFO mailbox */
        CHECK_TXFIFO_MBX_NR
    }
    else
    {
        CHECK_MBX_NR
    }

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }
    if ((FIFO_MAILBOX_MODE == mb_mode) && (bit_set[mbox_nr] & 0x0f000000))    /* check normal or fifo mode */
    {
        /* Write FFh to transmit FIFO pointer control register */
        can_block_p->TFPCR = 0xFF;
    }
    else   /* Don't using FIFO mode */
    {
    /* Wait for any previous transmission to complete. */
    api_status = can_wait_tx_rx(ch_nr, mbox_nr);

    /* Clear SentData flag since we are about to send anew.
     * Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. See RX64M/71M UM 43.2.8.
     * TRMREQ/RECREQ already 0 after can_wait_tx_rx(). (No need to write twice). */
    can_block_p->MCTL[mbox_nr].BYTE = 0;

    /* Set TrmReq bit to "1" */
    can_block_p->MCTL[mbox_nr].BIT.TX.TRMREQ = 1;
    }
    return (api_status);
}
/******************************************************************************
 End of function R_CAN_Tx
 *****************************************************************************/ /* end R_CAN_Tx() */

/***********************************************************************************************************************
* Function Name: R_CAN_TxCheck
********************************************************************************************************************//**
* @brief Check for successful data frame transmission. Use to check a mailbox for a successful data frame transmission.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mbox_nr - Which CAN mailbox to use. (0-32)
* @retval R_CAN_OK            Transmission was completed successfully.
* @retval R_CAN_SW_BAD_MBX    Bad mailbox number.
* @retval R_CAN_BAD_CH_NR     The channel number does not exist.
* @retval R_CAN_MSGLOST       Message was overwritten or lost.
* @retval R_CAN_NO_SENTDATA   No message was sent.
* @details This function is only needed if an application needs to verify that a message has been transmitted for
* example so that it can progress a state machine, or if messages are sent back-to-back. With CAN’s level of transport
* control built into the silicon, it can reasonably be assumed that once a mailbox has been asked to send with the API
* that the message will indeed be sent. Safest if of course to use this function after a transmission.
*/
uint32_t R_CAN_TxCheck(const uint32_t ch_nr, const uint32_t mbox_nr)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status = R_CAN_OK;

    CHECK_MBX_NR
    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    /* Read and clear SentData flag. */
    if (0 == can_block_p->MCTL[mbox_nr].BIT.TX.SENTDATA)
    {
        api_status = R_CAN_NO_SENTDATA;
    }
    else
    {
        /* Clear SENTDATA.
         * Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. See RX64M/71M UM 43.2.8.
         * TRMREQ must be set to 0 (or will send again).
         * Do it twice since "Bits SENTDATA and TRMREQ cannot be set to 0 simultaneously." */
        can_block_p->MCTL[mbox_nr].BYTE = 0;
        can_block_p->MCTL[mbox_nr].BYTE = 0;
    }

    return (api_status);
}
/******************************************************************************
 End of function R_CAN_TxCheck
 *****************************************************************************/ /* end R_CAN_TxCheck() */

/***********************************************************************************************************************
* Function Name: R_CAN_TxStopMsg
********************************************************************************************************************//**
* @brief Stop a mailbox that has been asked to transmit a frame
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mb_mode - Normal mailbox (0), FIFO mailbox (1).
* @param[in] mbox_nr - Which CAN mailbox to use. (0-32)
* @retval R_CAN_OK            Action completed successfully.
* @retval R_CAN_SW_BAD_MBX    Bad mailbox number.
* @retval R_CAN_BAD_MODE      The mode number does not exist.
* @retval R_CAN_BAD_CH_NR     The channel number does not exist.
* @retval R_CAN_SW_ABORT_ERR  Waiting for an abort timed out.
* @details This function clears the mailbox control or transmit FIFO control flags so that a transmission is stopped
* (TrmReq is set to 0 for normal mailboxes or TFE is set to 0 for transmit FIFO mailboxes). A software counter then
* waits for an abort for a maximum period of time.\n
* If the message was not stopped, R_CAN_SW_ABORT_ERR is returned. Note that the cause of this could be that the
* message was already sent.
*/
uint32_t R_CAN_TxStopMsg(const uint32_t ch_nr, const uint32_t  mb_mode, const uint32_t mbox_nr)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;
    
    uint32_t api_status     = R_CAN_OK;
    uint32_t can_tmo_cnt    = MAX_CAN_SW_DELAY;

    /* check mailbox mode */
    CHECK_MBX_MODE

    CHECK_MBX_NR
    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    if ((FIFO_MAILBOX_MODE == mb_mode)
            && (bit_set[mbox_nr] & 0x0f000000))   /* check fifo mailbox mode and using FIFO mailbox */
    {
        /* To abort FIFO transmission, it is necessary to clear the transmit FIFO enable bit (TFE) to 0 */
        can_block_p->TFCR.BIT.TFE = 0; /* Transmit FIFO enabled. */

        /* Wait for abort. */
        /* WAIT_LOOP */
        while ((!can_block_p->TFCR.BIT.TFEST) && DEC_CHK_CAN_SW_TMR)
        {
            /* Wait loop. */
            R_BSP_NOP();
        }
        if (0 == can_tmo_cnt)
        {
            api_status = R_CAN_SW_ABORT_ERR;
        }
        else
        {
            /* Do nothing */
            R_BSP_NOP();
        }
    }
    else
    {
        /* Clear message mailbox control register. Setting TRMREQ to 0 should abort.
         * Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. See RX64M/71M UM 43.2.8.
         * Do it twice since "Bits SENTDATA and TRMREQ cannot be set to 0 simultaneously." */
        can_block_p->MCTL[mbox_nr].BYTE = 0;
        can_block_p->MCTL[mbox_nr].BYTE = 0;

        /* Wait for abort. */
        /* WAIT_LOOP */
        while ((can_block_p->MCTL[mbox_nr].BIT.TX.TRMABT) && DEC_CHK_CAN_SW_TMR)
        {
            /* Wait loop. */
            R_BSP_NOP();
        }
        if (0 == can_tmo_cnt)
        {
            api_status = R_CAN_SW_ABORT_ERR;
        }

        /* Clear abort flag. Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. */
        can_block_p->MCTL[mbox_nr].BYTE = 0;
    }

    return (api_status);
}
/******************************************************************************
 End of function R_CAN_TxStopMsg
 *****************************************************************************//* end R_CAN_TxStopMsg() */

/***********************************************************************************************************************
* Function Name: R_CAN_RxSet
********************************************************************************************************************//**
* @brief Set up a mailbox to receive.\n
* The API sets up a given mailbox to receive data frames with the given CAN 11-bit ID. Incoming data frames with the
* same ID will be stored in the mailbox.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mbox_nr - Which CAN mailbox to use. (0-32)
* @param[in] id - The CAN ID which the mailbox should receive.
* @param[in] frame_type \n
*  DATA_FRAME      Send a normal data frame. \n
*  REMOTE_FRAME    Send a remote data frame request.
* @retval R_CAN_OK                Action completed successfully.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_SW_SET_TX_TMO     Waiting for previous transmission to finish timed out.
* @retval R_CAN_SW_SET_RX_TMO     Waiting for previous reception to complete timed out.
* @details The function will first wait for any previous transmission/reception to complete, then temporarily
* interrupt disable the mailbox. It sets the mailbox to the given standard ID value, and whether to receive normal
* CAN dataframes or remote frame requests.
*/
uint32_t R_CAN_RxSet(const uint32_t ch_nr,
                    const uint32_t  mbox_nr,
                    const uint32_t  id,
                    const uint32_t  frame_type)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status = R_CAN_OK;

    CHECK_MBX_NR
    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    /* Wait for any previous transmission/reception to complete. */
    api_status = can_wait_tx_rx(ch_nr, mbox_nr);

    /* Interrupt disable the mailbox. */
    can_block_p->MIER.LONG &= (~(bit_set[mbox_nr]));
    
    /* Clear message mailbox control register.
     * TRMREQ/RECREQ already 0 after can_wait_tx_rx(). (No need to write twice). */
    can_block_p->MCTL[mbox_nr].BYTE = 0;

    /*** Set Mailbox ID based on ID mode ***/
    /* Check for XID flag bit set in ID argument */
    if (id & XID_MASK)
    {
        /* Set message mailbox buffer Extended ID, masking off temporary XID flag bit. */
        can_block_p->MB[mbox_nr].ID.LONG = (id & (~XID_MASK));
    }
    else
    {
        /* Set message mailbox buffer Standard ID */ 
        can_block_p->MB[mbox_nr].ID.BIT.SID = (id & SID_MASK); /* Now put the lower 11 bit in the SID. */
    }

    /* Set IDE bit depending on if want to receive SID or XID frame. Only for mixed mode. */
    if (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM)
    {
        if (id & XID_MASK)
        {
            can_block_p->MB[mbox_nr].ID.BIT.IDE = 1;
        }
        else
        {
            can_block_p->MB[mbox_nr].ID.BIT.IDE = 0;
        }
    }
    else
    {
        /* When not MIXED mode, IDE should be written with 0. */
        can_block_p->MB[mbox_nr].ID.BIT.IDE = 0;
    }

    /* Data frame = 0, Remote frame = 1    */
    if (REMOTE_FRAME == frame_type)
    {
        can_block_p->MB[mbox_nr].ID.BIT.RTR = 1;
    }
    else
    {
        can_block_p->MB[mbox_nr].ID.BIT.RTR = 0;
    }

    #if (USE_CAN_POLL == 0)
        /* Interrupt enable the mailbox */
        can_block_p->MIER.LONG |= (bit_set[mbox_nr]);
    #endif

    /* Request to receive the frame. (Set RecReq bit). */
    can_block_p->MCTL[mbox_nr].BYTE = 0x40;

    return (api_status);
}
/******************************************************************************
 End of function R_CAN_RxSet
 *****************************************************************************/ /* end R_CAN_RxSet() */

/***********************************************************************************************************************
* Function Name: R_CAN_RxSetFIFO
********************************************************************************************************************//**
* @brief Set up a mailbox to receive.\n
* The API sets up a given FIFO mailbox to receive data frames with the given CAN 11-bit ID. Incoming data frames with
* the same ID will be stored in the mailbox.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mb_mode - Normal mailbox (0), FIFO mailbox (1).
* @param[in] mbox_nr - Which CAN mailbox to use. (0-32)
* @param[in] fidcr0_value - The CAN ID which the mailbox should receive.
* @param[in] fidcr1_value - The CAN ID which the mailbox should receive.
* @param[in] mkr6_value - The mask register.
* @param[in] mkr7_value - The mask register.
* @param[in] frame_type \n
*  DATA_FRAME      receive a normal data frame. \n
*  REMOTE_FRAME    receive a remote data frame request.
* @retval R_CAN_OK                Action completed successfully.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_BAD_MODE          The mode number does not exist.
* @retval CAN_ERR_NOT_FIFO_MODE   Current mailbox mode is not FIFO mailbox mode.
* @details The function will first temporarily interrupt disable the mailbox. Then it sets whether to receive normal
* CAN data frames or remote frame requests. It also sets the FIFO mailbox to the given ID value. Next, it performs
* setting value for mask register, refer to section R_CAN_RxSetMask in the application note for details.
*/
uint32_t R_CAN_RxSetFIFO(const uint32_t  ch_nr,
                        const uint32_t  mb_mode,
                        const uint32_t  mbox_nr,
                        const uint32_t  fidcr0_value,
                        const uint32_t  fidcr1_value,
                        const uint32_t  frame_type,
                        const uint32_t  mkr6_value,
                        const uint32_t  mkr7_value)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status = R_CAN_OK;

    /* check mailbox mode */
    CHECK_MBX_MODE

    /* return if not FIFO_MAILBOX_MODE */
    if (FIFO_MAILBOX_MODE != mb_mode)    /* check normal or fifo mode */
    {
        return (CAN_ERR_NOT_FIFO_MODE);
    }

    /* return if mailbox is not receive fifo mailbox(28 -->31) */
    if ((mbox_nr < 28) || (mbox_nr > 31))
    {
        return (R_CAN_SW_BAD_MBX);
    }

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    /* Shift to CAN Halt mode only when the CAN operation mode is not Halt or Reset */
    if ((CAN_RESET != can_block_p->CTLR.BIT.CANM) && (CAN_HALT != can_block_p->CTLR.BIT.CANM))
    {
        /* Write to fidcr0, fidcr1 and mkr6, mkr7 in CAN reset mode or CAN halt mode. */
        R_CAN_Control(ch_nr, HALT_CANMODE);
    }

    /* In FIFO mailbox mode, change the bits in MIER for the related FIFO only
     * when the RFE bit in RFCR is 0 and the RFEST flag in RFCR is 1 */
    if ((0 == can_block_p->RFCR.BIT.RFE) && (1 == can_block_p->RFCR.BIT.RFEST))
    {
        /* Interrupt disable the mailbox. */
        can_block_p->MIER.LONG &= (~(bit_set[28]));
    }

    /* Data frame = 0, Remote frame = 1    */
    if (REMOTE_FRAME == frame_type)
    {
        can_block_p->FIDCR0.BIT.RTR = 1;
        can_block_p->FIDCR1.BIT.RTR = 1;
    }
    else
    {
        can_block_p->FIDCR0.BIT.RTR = 0;
        can_block_p->FIDCR1.BIT.RTR = 0;
    }

    /*** Set Mailbox ID based on ID mode ***/
    /* Check for XID flag bit set in fidcr0_value argument */
    if (fidcr0_value & XID_MASK)
    {
        /* Set message mailbox buffer Extended ID, masking off temporary XID flag bit. */
        can_block_p->FIDCR0.LONG = (fidcr0_value & (~XID_MASK));
    }
    else
    {
        /* Set message mailbox buffer Standard ID */
        can_block_p->FIDCR0.BIT.SID = (fidcr0_value & SID_MASK);
    }

    /* Set IDE bit depending on if want to receive SID or XID frame. Only for mixed mode. */
    if (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM)
    {
        if (fidcr0_value & XID_MASK)
        {
        /* Mixed mode; select to send extended frame. */
        can_block_p->FIDCR0.BIT.IDE = 1;
        }
        else
        {
        /* Always set IDE to 0 (unless mixed mode and extended frame as above). */
        can_block_p->FIDCR0.BIT.IDE = 0;
        }
    }
    else
    {
        /* When not MIXED mode, IDE should be written with 0. */
        can_block_p->FIDCR0.BIT.IDE = 0;
    }

    /* Check for XID flag bit set in fidcr1_value argument */
    if (fidcr1_value & XID_MASK)
    {
        /* Set message mailbox buffer Extended ID, masking off temporary XID flag bit. */
        can_block_p->FIDCR1.LONG = (fidcr1_value & (~XID_MASK));
    }
    else
    {
        /* Set message mailbox buffer Standard ID */
        can_block_p->FIDCR1.BIT.SID = (fidcr1_value & SID_MASK);
    }

    /* Set IDE bit depending on if want to receive SID or XID frame. Only for mixed mode. */
    if (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM)
    {
        if (fidcr1_value & XID_MASK)
        {
        /* Mixed mode; select to send extended frame. */
        can_block_p->FIDCR1.BIT.IDE = 1;
        }
        else
        {
        /* Always set IDE to 0 (unless mixed mode and extended frame as above). */
        can_block_p->FIDCR1.BIT.IDE = 0;
        }
    }
    else
    {
        /* When not MIXED mode, IDE should be written with 0. */
        can_block_p->FIDCR1.BIT.IDE = 0;
    }

    if ((EXT_ID_MODE == can_block_p->CTLR.BIT.IDFM) || (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM))
    {
        /* Set XID 29-bit mask value in mask register. */
        can_block_p->MKR[6].LONG = (mkr6_value & (~XID_MASK)); /* Set XID 29-bit mask value in mask register 6. */
        can_block_p->MKR[7].LONG = (mkr7_value & (~XID_MASK)); /* Set XID 29-bit mask value in mask register 7. */
    }
    else
    {
        /* Set SID 11-bit mask value in mask register. */
        can_block_p->MKR[6].BIT.SID = mkr6_value; /* Set SID 11-bit mask value in mask register 6. */
        can_block_p->MKR[7].BIT.SID = mkr7_value; /* Set SID 11-bit mask value in mask register 7. */
    }

    /* Set mailbox mask to be used. (0 = mask VALID.).
     *  Set bits 31 to 24 to 0 in FIFO mailbox mode */
    can_block_p->MKIVLR.LONG &= (~(0xFF000000));

    #if (USE_CAN_POLL == 0)
    /* In FIFO mailbox mode, change the bits in MIER for the related FIFO only
     * when the RFE bit in RFCR is 0 and the RFEST flag in RFCR is 1 */
        if ((0 == can_block_p->RFCR.BIT.RFE) && (1 == can_block_p->RFCR.BIT.RFEST))
        {
            /* Interrupt enable the mailbox */
            can_block_p->MIER.LONG |= (bit_set[28]);

            /* Generate interrupt when Receive FIFO warning */
            if (CAN_CFG_RXFIFO_INT_GEN_TIMING == 1)
            {
                can_block_p->MIER.LONG |= (bit_set[29]);
            }
        }
    #endif

    /* Receive FIFO enable */
    can_block_p->RFCR.BIT.RFE = 1;

    return (api_status);
}
/******************************************************************************
 End of function R_CAN_RxSetFIFO
 *****************************************************************************/ /* end R_CAN_RxSetFIFO() */

/***********************************************************************************************************************
* Function Name: R_CAN_RxSetXid
********************************************************************************************************************//**
* @brief Set up a mailbox to receive.\n
* The API sets up a given mailbox to receive data frames with the given CAN 11-bit ID. Incoming data frames with the
* same ID will be stored in the mailbox, except the ID will be a 29-bit ID.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mbox_nr - Which CAN mailbox to use. (0-32)
* @param[in] xid - The CAN ID which the mailbox should receive.
* @param[in] frame_type \n
*  DATA_FRAME      Send a normal data frame. \n
*  REMOTE_FRAME    Send a remote data frame request.
* @retval R_CAN_OK                Action completed successfully.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_SW_SET_TX_TMO     Waiting for previous transmission to finish timed out.
* @retval R_CAN_SW_SET_RX_TMO     Waiting for previous reception to complete timed out.
* @details The function will first wait for any previous transmission/reception to complete, then temporarily
* interrupt disable the mailbox. It sets the mailbox to the given standard ID value, and whether to receive normal
* CAN dataframes or remote frame requests.
*/
uint32_t R_CAN_RxSetXid(const uint32_t     ch_nr, 
                        const uint32_t     mbox_nr,
                        uint32_t           xid,
                        const uint32_t     frame_type)
{
    /* Add the Xid bit so that 29-bit ID will be used by R_CAN_RxSet(). */
    return (R_CAN_RxSet(ch_nr, mbox_nr, (xid | XID_MASK) , frame_type));
}
/******************************************************************************
 End of function R_CAN_RxSetXid
 *****************************************************************************/ /* end R_CAN_RxSetXid() */

/***********************************************************************************************************************
* Function Name: R_CAN_RxSetFIFOXid
********************************************************************************************************************//**
 * @brief Set up a mailbox to receive.\n
* The API sets up a given FIFO mailbox to receive data frames with the given CAN 29-bit ID. Incoming data frames with
* the same ID will be stored in the mailbox.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mb_mode - Normal mailbox (0), FIFO mailbox (1).
* @param[in] mbox_nr - Which CAN mailbox to use. (0-32)
* @param[in] xfidcr0_value - The CAN ID which the mailbox should receive.
* @param[in] xfidcr1_value - The CAN ID which the mailbox should receive.
* @param[in] mkr6_value - The mask register.
* @param[in] mkr7_value - The mask register.
* @param[in] frame_type \n
*  DATA_FRAME      receive a normal data frame. \n
*  REMOTE_FRAME    receive a remote data frame request.
* @retval R_CAN_OK                Action completed successfully.
* @retval R_CAN_SW_BAD_MBX        Bad mailbox number.
* @retval R_CAN_BAD_CH_NR         The channel number does not exist.
* @retval R_CAN_BAD_MODE          The mode number does not exist.
* @retval CAN_ERR_NOT_FIFO_MODE   Current mailbox mode is not FIFO mailbox mode.
* @details The function will first temporarily interrupt disable the mailbox. Then it sets whether to receive normal
* CAN data frames or remote frame requests. It also sets the FIFO mailbox to the given ID value. Next, it performs
* setting value for mask register, refer to section R_CAN_RxSetMask in the application note for details.
*/
uint32_t R_CAN_RxSetFIFOXid(const uint32_t     ch_nr,
                            const uint32_t     mb_mode,
                            const uint32_t     mbox_nr,
                            uint32_t           xfidcr0_value,
                            uint32_t           xfidcr1_value,
                            const uint32_t     frame_type,
                            const uint32_t     mkr6_value,
                            const uint32_t     mkr7_value)
{
    /* Add the Xid bit so that 29-bit ID will be used by R_CAN_RxSetFIFO(). */
    return (R_CAN_RxSetFIFO(ch_nr, mb_mode, mbox_nr, (xfidcr0_value | XID_MASK),
                            (xfidcr1_value | XID_MASK), frame_type, mkr6_value, mkr7_value));
}
/******************************************************************************
 End of function R_CAN_RxSetFIFOXid
 *****************************************************************************/ /* end R_CAN_RxSetXid() */

/***********************************************************************************************************************
* Function Name: R_CAN_RxPoll
********************************************************************************************************************//**
* @brief Checks if a mailbox has received a message
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mbox_nr - Which CAN mailbox to check (0-32).
* @retval R_CAN_OK            There is a message waiting.
* @retval R_CAN_NOT_OK        No message waiting.
* @retval R_CAN_RXPOLL_TMO    Message pending but timed out.
* @retval R_CAN_SW_BAD_MBX    Bad mailbox number.
* @retval R_CAN_BAD_CH_NR     The channel number does not exist.
* @details When a mailbox is set up to receive certain messages, it is important to determine when it has finished
* receiving successfully. There are two methods for doing this: \n
* Polling. Call the API regularly to check for new messages. USE_CAN_POLL must be defined in the CAN configuration
* file. If there is a message use R_CAN_RxRead to fetch it. \n
* Using the CAN receive interrupt (USE_CAN_POLL not defined): Use this API to check which mailbox received. Then
* notify the application. \n
* The function returns R_CAN_OK if new data was found in the mailbox.
*
*/
uint32_t R_CAN_RxPoll(const uint32_t  ch_nr, const uint32_t  mbox_nr)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status = R_CAN_NOT_OK;
    uint32_t poll_delay = MAX_CANREG_POLLCYCLES;

    CHECK_MBX_NR
    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    /* Wait if new data is currently being received. */
    /* WAIT_LOOP */
    while ((can_block_p->MCTL[mbox_nr].BIT.RX.INVALDATA) && poll_delay)
    {
        poll_delay--;
    }

    if (0 == poll_delay)    /* Still updating mailbox. Come back later. */
    {
        api_status = R_CAN_RXPOLL_TMO;
    }
    else /* Message received? */
    {
        /* If message received, tell user. */
        if (1 == can_block_p->MCTL[mbox_nr].BIT.RX.NEWDATA)
        {
            api_status = R_CAN_OK;
        }
    }
    return (api_status);
}
/******************************************************************************
 End of function R_CAN_RxPoll
 *****************************************************************************//* end R_CAN_RxPoll() */

/***********************************************************************************************************************
* Function Name: R_CAN_RxRead
********************************************************************************************************************//**
* @brief Read the CAN data frame content from a mailbox. The API checks if a given mailbox has received a message.
* If so, a copy of the mailbox’s dataframe will be written to the given structure.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] mb_mode - Normal mailbox (0), FIFO mailbox (1).
* @param[in] mbox_nr - Which CAN mailbox to check (0-32).
* @param[in] frame_p - Refers to a pointer to a data frame structure in memory. It is an address to the data structure
*                       into which the function will place a copy of the mailbox’s received CAN data frame.
* @retval R_CAN_OK            There is a message waiting.
* @retval R_CAN_BAD_MODE      The mode number does not exist.
* @retval R_CAN_SW_BAD_MBX    Bad mailbox number.
* @retval R_CAN_BAD_CH_NR     The channel number does not exist.
* @retval CAN_ERR_BOX_EMPTY   No unread message in receive FIFO.
* @retval R_CAN_MSGLOST       Message was overwritten or lost.
* @details This function is used to receive the message by the normal mailboxes or receive FIFO mailboxes.\n
To receive FIFO mailboxes, it checks Receive FIFO Empty Status Flag to ensure unread message in receive FIFO.
If have, it loads the ID value, the Data Length Code and the data frame payload bytes (0-7) of message into the mailbox.
Finally, it checks Message Lost then write FF to Receive FIFO Pointer Control Register.\n
To normal mailboxes, Use R_CAN_RxPoll() first to check whether the mailbox has received a message.\n
This function is used to fetch the message from a mailbox, either when using polled mode or from a CAN receive
interrupt.
*/
uint32_t R_CAN_RxRead(const uint32_t    ch_nr,
                    const uint32_t     mb_mode,
                    const uint32_t     mbox_nr,
                    can_frame_t* const frame_p)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t i = 0;

    uint32_t api_status = R_CAN_OK;

    /* check mailbox mode*/
    CHECK_MBX_MODE

    /* check mailbox for fifo mailbox mode:
     * Normal mailbox: 0 --> 23,
     * Receive fifo mailbox: 28 --> 31;
     * return if number of mailbox is transmit fifo mailbox(24 -->27) or other mailbox */
    if (FIFO_MAILBOX_MODE == mb_mode)    /* check normal or fifo mailbox mode */
    {
        /* return if MBX is receive FIFO mailbox */
        CHECK_RXFIFO_MBX_NR
    }
    else
    {
    CHECK_MBX_NR
    }

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    if ((FIFO_MAILBOX_MODE == mb_mode) && (bit_set[mbox_nr] & 0xf0000000))    /* check normal or fifo mode */
    {
        /* return if FIFO is empty */
        if (can_block_p->RFCR.BIT.RFEST)
        {
            return (CAN_ERR_BOX_EMPTY);
        }

        /* EXT_ID_MODE */
        /* Copy received data from message mailbox to memory. The IDE bit is only valid in mixed mode. */
        if (EXT_ID_MODE == can_block_p->CTLR.BIT.IDFM)
        {
            /* Get mailbox Extended ID, keeping only lower 29 bits. */
            frame_p->id = (can_block_p->MB[28].ID.LONG & (~XID_MASK));
        }
        /* MIXED_ID_MODE */
        else if (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM)
        {
            if (1 == can_block_p->MB[28].ID.BIT.IDE) /* Check for XID control bit set. */
            {
                /* Get mailbox Extended ID, keeping only lower 29 bits. */
                frame_p->id = (can_block_p->MB[28].ID.LONG & (~XID_MASK));
            }
            else
            {
                /* Get only the lower 11 bits from the SID. */
                frame_p->id = can_block_p->MB[28].ID.BIT.SID;
            }
        }
        /* STD_ID_MODE */
        else
        {
            /* Get lower 11 bits from SID. */
            frame_p->id = can_block_p->MB[28].ID.BIT.SID;
        }

        /* Casting the register into uint8_t */
        frame_p->dlc = (uint8_t)can_block_p->MB[28].DLC;

        /* WAIT_LOOP */
        for (i = 0; i < can_block_p->MB[mbox_nr].DLC; i++)
        {
            frame_p->data[i] = can_block_p->MB[28].DATA[i];
        }

        /* check receive FIFO message lost */
        if (can_block_p->RFCR.BIT.RFMLF)
        {
            /* Clear receive FIFO message lost flag to 0 */
            can_block_p->RFCR.BYTE = 0x01;

            api_status = R_CAN_MSGLOST;
        }

        /* Write FFh to receive FIFO pointer control register */
        can_block_p->RFPCR = 0xFF;
    }

    /* Content of below else will occur in 2 case:
     * 1: Read message by MBX normal in FIFO mailbox mode when FIFO_MAILBOX_MODE == mb_mode
       2: Read message by MBX normal in normal mailbox mode when FIFO_MAILBOX_MODE != mb_mode */
    else
    {
        /* EXT_ID_MODE */
        /* Copy received data from message mailbox to memory. The IDE bit is only valid in mixed mode. */
        if (EXT_ID_MODE == can_block_p->CTLR.BIT.IDFM)
        {
            /* Get mailbox Extended ID, keeping only lower 29 bits. */
            frame_p->id = (can_block_p->MB[mbox_nr].ID.LONG & (~XID_MASK));
        }
        /* MIXED_ID_MODE */
        else if (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM)
        {
            if (1 == can_block_p->MB[mbox_nr].ID.BIT.IDE) /* Check for XID control bit set. */
            {
                /* Get mailbox Extended ID, keeping only lower 29 bits. */
                frame_p->id = (can_block_p->MB[mbox_nr].ID.LONG & (~XID_MASK));
            }
            else
            {
                /* Get only the lower 11 bits from the SID. */
                frame_p->id = can_block_p->MB[mbox_nr].ID.BIT.SID;
            }
        }
        /* STD_ID_MODE */
        else
        {
            /* Get lower 11 bits from SID. */
            frame_p->id = can_block_p->MB[mbox_nr].ID.BIT.SID;
        }

        /* Casting the register into uint8_t */
        frame_p->dlc = (uint8_t)can_block_p->MB[mbox_nr].DLC;

        /* WAIT_LOOP */
        for (i = 0; i < can_block_p->MB[mbox_nr].DLC; i++)
        {
            frame_p->data[i] = can_block_p->MB[mbox_nr].DATA[i];
        }

        /* Check if message was lost/overwritten. */
        if (can_block_p->MCTL[mbox_nr].BIT.RX.MSGLOST)
        {
            api_status = R_CAN_MSGLOST;
        }

        /* Clear MSGLOST.
         * Also set NEWDATA to 0 since the mailbox was just emptied and start over with new RxPolls.
         * Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. See RX64M/71M UM 43.2.8.*/
        can_block_p->MCTL[mbox_nr].BYTE = 0x40;
    }

    return (api_status);
}
/******************************************************************************
 End of function R_CAN_RxRead
 *****************************************************************************//* end R_CAN_RxRead() */

/***********************************************************************************************************************
* Function Name: R_CAN_RxSetMask
********************************************************************************************************************//**
* @brief Sets the CAN ID Acceptance Masks. To accept only one ID, set mask to all ones. To accept all messages, set
* mask to all zeros. To accept a range of messages, set the corresponding ID bits to zero.
* @param[in] ch_nr -  CAN channel to use (0-2 MCU dependent).
* @param[in] mbox_nr - Which mailbox to mask (0-32). Four mailboxes will be affected within its group.
* @param[in] mask_value - Mask value. (0-0x7FF)
* @details Receive mailboxes can use a mask to filter out one message, or expand receiving to a range of messages
* (CAN IDs). The mask enables this using the mailbox group’s ID field. There is one mask for mailbox 0-3, one
* for 4-7, etc. Changing a mask will therefore affect the behavior of adjacent mailboxes. \n
*  Each '0' in the mask means "mask this bit", or "don't look at that bit"; accept anything.\n
*  Each '1' means check if the CAN-ID bit in this position matches the CAN-ID of the mailbox.\n
*  See section R_CAN_RxSetMask in the application note for details.
*/
void R_CAN_RxSetMask(const uint32_t  ch_nr,
                    const uint32_t  mbox_nr,
                    const uint32_t  mask_value)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return;
    }

    /* Shift to CAN Halt mode only when the CAN operation mode is not Halt or Reset */
    if ((CAN_RESET != can_block_p->CTLR.BIT.CANM) && (CAN_HALT != can_block_p->CTLR.BIT.CANM))
    {
        /* Write to MKR0 to MKR7 in CAN reset mode or CAN halt mode.*/
        R_CAN_Control(ch_nr, HALT_CANMODE);
    }

    /* Set mask for the group of mailboxes. */
    if ((EXT_ID_MODE == can_block_p->CTLR.BIT.IDFM) || (MIXED_ID_MODE == can_block_p->CTLR.BIT.IDFM))
    {
        /* Set XID 29-bit mask value in mask register. */
        can_block_p->MKR[mbox_nr/4].LONG = (mask_value & (~XID_MASK));
    }
    else
    {
        /* Set SID 11-bit mask value in mask register. */
        can_block_p->MKR[mbox_nr/4].BIT.SID = mask_value;
    }

    /* Set mailbox mask to be used. (0 = mask VALID.) */
    can_block_p->MKIVLR.LONG &= (~(bit_set[mbox_nr]));

}
/******************************************************************************
 End of function R_CAN_RxSetMask
 *****************************************************************************//* end R_CAN_RxSetMask() */

/*****************************************************************************
Function name:  can_wait_tx_rx
Arguments:      Channel nr.
                Mailbox nr.
Description:    Wait for communicating mailbox to complete action. This would 
                be apporopriate for example if a mailbox all of a sudden needs 
                to be reconfigured but the user wants any pending receive or 
                transmit to finish.
Return value:   R_CAN_OK            There is a message waiting.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
                R_CAN_SW_SET_TX_TMO Waiting for previous transmission to finish 
                                    timed out.
                R_CAN_SW_SET_RX_TMO Waiting for previous reception to complete 
                                    timed out.
*****************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(can_wait_tx_rx)
uint32_t can_wait_tx_rx(const uint32_t  ch_nr, const uint32_t  mbox_nr)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status     = R_CAN_OK;
    uint32_t can_tmo_cnt    = MAX_CAN_SW_DELAY;

    /* Mailbox and channel nr already checked by caller. */
    can_block_p = CAN_CHANNELS[ch_nr];

    /* Wait for any previous transmission to complete. */
    if (can_block_p->MCTL[mbox_nr].BIT.TX.TRMREQ)
    {
        /* WAIT_LOOP */
        while ((0 == can_block_p->MCTL[mbox_nr].BIT.TX.SENTDATA) && DEC_CHK_CAN_SW_TMR)
        {
            /* Wait loop. */
            R_BSP_NOP();
        }
        if (0 == can_tmo_cnt)
        {
            api_status = R_CAN_SW_SET_TX_TMO;
        }
    }
    /* Wait for any previous reception to complete. */
    else if (can_block_p->MCTL[mbox_nr].BIT.RX.RECREQ)
    {
        /* WAIT_LOOP */
        while ((1 == can_block_p->MCTL[mbox_nr].BIT.RX.INVALDATA) && DEC_CHK_CAN_SW_TMR)
        {
            /* Wait loop. */
            R_BSP_NOP();
        }
        if (0 == can_tmo_cnt)
        {
            api_status = R_CAN_SW_SET_RX_TMO;
        }
    }
    else
    {
        /* Do nothing */
        R_BSP_NOP();

    }
    return (api_status);
}
/******************************************************************************
 End of function can_wait_tx_rx
 *****************************************************************************/
/* end can_wait_tx_rx() */

/***********************************************************************************************************************
* Function Name: R_CAN_CheckErr
********************************************************************************************************************//**
* @brief Check for bus errors. The API checks the CAN status, or Error State, of the CAN peripheral.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @retval R_CAN_BAD_CH_NR               The channel number does not exist.
* @retval R_CAN_STATE_ERROR_ACTIVE      CAN bus status is normal.
* @retval R_CAN_STATE_ERROR_PASSIVE     Node has sent at least 127 Error frames for either the Transmit Error Counter,
*                                       or the Receive Error Counter.
* @retval R_CAN_STATE_BUSOFF            Node’s Transmit Error Counter has surpassed 255 due to the node’s failure to
*                                       transmit correctly.
* @details The API checks the CAN status flags of the CAN peripheral and returns the status error code. It tells
* whether the node is in a functioning state or not and is used for application error handling.\n
* It should be polled either routinely from the main loop, or via the CAN error interrupt. Since the peripheral
* automatically handles retransmissions and Error frames it is usually of no advantage to include an error interrupt
* routine. \n
* If an error state is encountered the application can just wait and monitor for the peripheral to recover, as the
* CAN peripheral takes itself on or off line depending on its state. After a recovery is discovered, the application
* should restart.\n
* See section R_CAN_CheckErr in the application note for details.
*/
uint32_t R_CAN_CheckErr(const uint32_t ch_nr)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    uint32_t api_status = R_CAN_STATUS_ERROR_ACTIVE;

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return (R_CAN_BAD_CH_NR);
    }

    /* Check CAN error state */
    if (can_block_p->STR.BIT.EST)
    {
        /* Check error-passive state */
        if (can_block_p->STR.BIT.EPST)
        {
            api_status = R_CAN_STATUS_ERROR_PASSIVE;
        }
        /* Check if bus off state */
        else if (can_block_p->STR.BIT.BOST)
        {
            api_status = R_CAN_STATUS_BUSOFF;
        }
        else
        {
            /* Keep api_status as R_CAN_STATUS_ERROR_ACTIVE */
            R_BSP_NOP();
        }
    }
    
    return (api_status);
}
/******************************************************************************
 End of function R_CAN_CheckErr
 *****************************************************************************//* end R_CAN_CheckErr() */

/***********************************************************************************************************************
* Function Name: R_CAN_SetBitrate
********************************************************************************************************************//**
* @brief Set the CAN bitrate (communication speed). The baud rate and bit timing must always be set during the
* configuration process. It can be changed later if reset mode is entered.
* @param[in] ch_nr - CAN channel to use (0-2 MCU dependent).
* @param[in] p_cfg - It is an address to the data structure containing the BRP, TSEG1, TSEG2, and SJW that constitute
* the bitrate for channel ch_nr.
* @details Setting the baud rate or data speed on the CAN bus requires some understanding of CAN bit timing and MCU
* frequency, as well as reading hardware manual figures and tables.\n
* Some calculations need to be done to set up the baud rate: Selects the baud rate prescaler division value, time
* segment 2 control, time segment 1 control, and resynchronization jump width control indicated by p_cfg.
* First some explanations. The CAN system clock, fcanclk, is the internal clock period of the CAN peripheral.
* This CAN system clock is determined by the CAN Baud Rate Prescaler value and the peripheral bus clock. One Time
* Quantum is equal to the period of the CAN clock.\n
* One CAN bus bit-time is an integer sum of a number of Time Quanta, Tq. Each bitrate register is then given a
* certain number of Tq of the total number of Time Quanta that make up one CAN bit period, or Tqtot.\n
* See section R_CAN_SetBitrate in the application note for details.
*/
void R_CAN_SetBitrate(const uint32_t ch_nr, const can_bitrate_config_t p_cfg)
{
    volatile struct st_can R_BSP_EVENACCESS_SFR * can_block_p;

    if (ch_nr < MAX_CHANNELS)
    {
        can_block_p = CAN_CHANNELS[ch_nr];
    }
    else
    {
        return;
    }

    /* Shift to CAN Reset mode only when the CAN operation mode is not Reset */
    if (CAN_RESET != can_block_p->CTLR.BIT.CANM)
    {
        /* Change to Reset mode */
        R_CAN_Control(ch_nr, RESET_CANMODE);
    }

    /* Set CAN clock select to be either PLL (default) or crystal direct. */
    can_block_p->BCR.BIT.CCLKS = 0; /* 0 = PLL. */

    /* Set TSEG1, TSEG2 and SJW. */
    can_block_p->BCR.BIT.BRP = p_cfg.BRP - 1;

    can_block_p->BCR.BIT.TSEG1 = p_cfg.TSEG1 - 1;

    can_block_p->BCR.BIT.TSEG2 = p_cfg.TSEG2 - 1;

    can_block_p->BCR.BIT.SJW = p_cfg.SJW - 1;
}
/******************************************************************************
 End of function R_CAN_SetBitrate
 *****************************************************************************/ /* end R_CAN_SetBitrate() */

/******************************************************************************
Function name:  can_module_stop_state_cancel
Description:    Release CAN peripherals from standby.
Arguments:      -
Return value:   -
******************************************************************************/
void can_module_stop_state_cancel(const uint32_t ch_nr)
{
    /* CAN: Module Stop Control Register B (MSTPCRB) bits 0-2 are for the CAN
    Peripherals.
    0: The module stop state is canceled.
    1: Transition to the module stop state is made. */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
    bsp_int_ctrl_t int_ctrl;
#endif
    /* First unlock the protect register. */
    /* Enable writing to PRCR bits while simultaneously enabling PRC1. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    if (0 == ch_nr)
    {
        MSTP(CAN0) = 0;
    }
#ifdef CAN1
    else if (1 == ch_nr)
    {
        MSTP(CAN1) = 0;
    }
#endif /* CAN1 */
#ifdef CAN2
    else if (2 == ch_nr)
    {
        MSTP(CAN2) = 0;
    }
#endif /* CAN2 */
    else
    {
        /* Do nothing */
        R_BSP_NOP();
    }
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Re-lock the protect register. */
    /* Enable writing to PRCR bits while simultaneously disabling PRC1. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
}
/******************************************************************************
 End of function can_module_stop_state_cancel
 *****************************************************************************/
/* end CAN_ModuleStopState_cancel() */

/**********************************************************************************
Function name:  config_can_interrupts
Description:    Configuration of CAN interrupts.    
Arguments:      Channel nr.
Return value :  -
***********************************************************************************/
static void config_can_interrupts(const uint32_t ch_nr)
{
#if (USE_CAN_POLL == 0)
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
bsp_int_ctrl_t int_ctrl;
#endif

    if (0 == ch_nr)
    {
#if (CAN_USE_CAN0 == 1)
        /* Configure CAN Tx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN0, TXM0));  /* 1 = interrupt enabled. */
        IPR(CAN0, TXM0) = CAN0_INT_LVL; /* priority */

        /* Configure CAN FIFO Tx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN0, TXF0));  /* 1 = interrupt enabled. */
        IPR(CAN0, TXF0) = CAN0_INT_LVL; /* priority */

        /* Configure CAN Rx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN0, RXM0));
        IPR(CAN0, RXM0) = CAN0_INT_LVL;

        /* Configure CAN FIFO Rx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN0, RXF0));
        IPR(CAN0, RXF0) = CAN0_INT_LVL;

        /* Configure CAN Error interrupt. Must enable group that it belongs to */
        /* in addition to individual source. */
        R_BSP_InterruptRequestEnable(VECT(ICU, GROUPBE0));
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        EN(CAN0, ERS0) = 1; /* resolves to:  ICU.GEN[GEN_CAN0_ERS0].BIT.EN0 = 1; */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
        ICU.IPR[IPR_ICU_GROUPBE0].BIT.IPR = CAN0_INT_LVL;
        R_BSP_InterruptWrite(BSP_INT_SRC_BE0_CAN0_ERS0, (bsp_int_cb_t)can_err_callback[0]);

        /* Enable all CAN error interrupts. */
        CAN0.EIER.BYTE = 0xFF;

        /* Mailbox interrupt enable registers. Disable interrupts for all slots. 
        They will be enabled individually by the API. */
        CAN0.MIER.LONG = 0x00000000;
#endif /* CAN_USE_CAN0 == 1 */
    }
#ifdef CAN1
#if (CAN_USE_CAN1 == 1)
    else if (1 == ch_nr)
    {
        /* Configure CAN Tx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN1, TXM1)); /* 1 = interrupt enabled. */
        IPR(CAN1, TXM1) = CAN1_INT_LVL; /* priority */

        /* Configure CAN FIFO Tx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN1, TXF1)); /* 1 = interrupt enabled. */
        IPR(CAN1, TXF1) = CAN1_INT_LVL; /* priority */

        /* Configure CAN Rx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN1, RXM1));
        IPR(CAN1, RXM1) = CAN1_INT_LVL;

        /* Configure CAN FIFO Rx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN1, RXF1));
        IPR(CAN1, RXF1) = CAN1_INT_LVL;

        /* Configure CAN Error interrupt. Must enable group that it belongs to */
        /* in addition to individual source. */
        R_BSP_InterruptRequestEnable(VECT(ICU, GROUPBE0));
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        EN(CAN1, ERS1) = 1;  /* Resolves to:  ICU.GEN[GEN_CAN1_ERS1].BIT.EN1 = 1; */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
        ICU.IPR[IPR_ICU_GROUPBE0].BIT.IPR = CAN1_INT_LVL; /* EDIT */
        R_BSP_InterruptWrite(BSP_INT_SRC_BE0_CAN1_ERS1, (bsp_int_cb_t) can_err_callback[1]);

        /* Enable all CAN error interrupts. */
        CAN1.EIER.BYTE = 0xFF;

        /* Mailbox interrupt enable registers. Disable interrupts for all slots. 
        They will be enabled individually by the API. */
        CAN1.MIER.LONG = 0x00000000;
    }
#endif /* CAN_USE_CAN1 == 1 */
#endif /* CAN1 */
#ifdef CAN2
#if (CAN_USE_CAN2 == 1)
    else if (2 == ch_nr)
    {
        /* Configure CAN Tx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN2, TXM2)); /* 1 = interrupt enabled. */
        IPR(CAN2, TXM2) = CAN2_INT_LVL; /* priority */

        /* Configure CAN FIFO Tx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN2, TXF2)); /* 1 = interrupt enabled. */
        IPR(CAN2, TXF2) = CAN2_INT_LVL; /* priority */

        /* Configure CAN Rx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN2, RXM2));
        IPR(CAN2, RXM2) = CAN2_INT_LVL;

        /* Configure CAN FIFO Rx interrupt. */
        R_BSP_InterruptRequestEnable(VECT(CAN2, RXF2));
        IPR(CAN2, RXF2) = CAN2_INT_LVL;

        /* Configure CAN Error interrupt. Must enable group that it belongs to */
        /* in addition to individual source. */
        R_BSP_InterruptRequestEnable(VECT(ICU, GROUPBE0)); /* Enable group JWP Edit */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        EN(CAN2, ERS2) = 1; /* resolves to:  ICU.GEN[GEN_CAN2_ERS2].BIT.EN2 = 1; */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
        ICU.IPR[IPR_ICU_GROUPBE0].BIT.IPR = CAN2_INT_LVL;
        R_BSP_InterruptWrite(BSP_INT_SRC_BE0_CAN2_ERS2, (bsp_int_cb_t) can_err_callback[2]);

        /* Enable all CAN error interrupts. */
        CAN2.EIER.BYTE = 0xFF;

        /* Mailbox interrupt enable registers. Disable interrupts for all slots.
        They will be enabled individually by the API. */
        CAN2.MIER.LONG = 0x00000000;
    }
#endif /* CAN_USE_CAN2 == 1 */
#endif /* CAN2 */
#else /* USE_CAN_POLL == 0 */
    R_BSP_NOP();
#endif /* USE_CAN_POLL == 0 */
}
/******************************************************************************
 End of function config_can_interrupts
 *****************************************************************************//* end config_can_interrupts() */

#if !USE_CAN_POLL
/*********************************************************************************


        C A N   I N T E R R U P T S


**********************************************************************************/
#if (CAN_USE_CAN0 == 1)
/*****************************************************************************
Function name:  CAN0_TXM0_ISR
Parameters:     -
Returns:        -
Description:    CAN0 Transmit interrupt (channel 0).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT (CAN0_TXM0_ISR, VECT_CAN0_TXM0)    /* See mcu_mapped_interrupts.h. */
R_BSP_ATTRIB_INTERRUPT void CAN0_TXM0_ISR(void)
{
    can_tx_callback[0]();
}
/******************************************************************************
 End of function CAN0_TXM0_ISR
 *****************************************************************************//* end CAN0_TXM0_ISR() */

/*****************************************************************************
Function name:  CAN0_TXF0_ISR
Parameters:     -
Returns:        -
Description:    CAN0 fifo Transmit interrupt (channel 0).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT (CAN0_TXF0_ISR, VECT_CAN0_TXF0)    /* See mcu_mapped_interrupts.h. */
R_BSP_ATTRIB_INTERRUPT void CAN0_TXF0_ISR(void)
{
    can_txf_callback[0]();
}
/******************************************************************************
 End of function CAN0_TXF0_ISR
 *****************************************************************************//* end CAN0_TXF0_ISR() */

/*****************************************************************************
Function name:  CAN0_RXM0_ISR()
Parameters:     -
Returns:        -
Description:    CAN0 Receive interrupt (channel 1).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT (CAN0_RXM0_ISR, VECT_CAN0_RXM0)
R_BSP_ATTRIB_INTERRUPT void CAN0_RXM0_ISR(void)
{
    can_rx_callback[0]();
}
/******************************************************************************
 End of function CAN0_RXM0_ISR
 *****************************************************************************//* end CAN0_RXM0_ISR() */

/*****************************************************************************
Function name:  CAN0_RXF0_ISR()
Parameters:     -
Returns:        -
Description:    CAN0 fifo Receive interrupt (channel 1).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT (CAN0_RXF0_ISR, VECT_CAN0_RXF0)
R_BSP_ATTRIB_INTERRUPT void CAN0_RXF0_ISR(void)
{
    can_rxf_callback[0]();
}
/******************************************************************************
 End of function CAN0_RXF0_ISR
 *****************************************************************************//* end CAN0_RXF0_ISR() */
#endif /* CAN_USE_CAN0 == 1 */
#ifdef CAN1
#if (CAN_USE_CAN1 == 1)
/*****************************************************************************
Function name:  CAN1_TXM1_ISR
Parameters:     -
Returns:        -
Description:    CAN1 Transmit interrupt (channel 1).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT(CAN1_TXM1_ISR, VECT_CAN1_TXM1)
R_BSP_ATTRIB_INTERRUPT void CAN1_TXM1_ISR(void)
{
    can_tx_callback[1]();
}
/******************************************************************************
 End of function CAN1_TXM1_ISR
 *****************************************************************************/ /* end CAN1_TXM1_ISR() */

/*****************************************************************************
Function name:  CAN0_TXF1_ISR
Parameters:     -
Returns:        -
Description:    CAN0 fifo Transmit interrupt (channel 1).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT(CAN1_TXF1_ISR, VECT_CAN1_TXF1)    /* See mcu_mapped_interrupts.h. */
R_BSP_ATTRIB_INTERRUPT void CAN1_TXF1_ISR(void)
{
    can_txf_callback[1]();
}
/******************************************************************************
 End of function CAN1_TXF1_ISR
 *****************************************************************************//* end CAN1_TXF1_ISR() */

/*****************************************************************************
Function name:  CAN1_RXM1_ISR()
Parameters:     -
Returns:        -
Description:    CAN1 Receive interrupt (channel 1).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT(CAN1_RXM1_ISR, VECT_CAN1_RXM1)
R_BSP_ATTRIB_INTERRUPT void CAN1_RXM1_ISR(void)
{
    can_rx_callback[1]();
}
/******************************************************************************
 End of function CAN1_RXM1_ISR
 *****************************************************************************/ /* end CAN1_RXM1_ISR() */

/*****************************************************************************
Function name:  CAN1_RXF1_ISR()
Parameters:     -
Returns:        -
Description:    CAN1 fifo Receive interrupt (channel 1).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT(CAN1_RXF1_ISR, VECT_CAN1_RXF1)
R_BSP_ATTRIB_INTERRUPT void CAN1_RXF1_ISR(void)
{
    can_rxf_callback[1]();
}
/******************************************************************************
 End of function CAN1_RXF1_ISR
 *****************************************************************************/ /* end CAN1_RXF1_ISR() */
#endif /* CAN_USE_CAN1 == 1 */
#endif /* CAN1 */

#ifdef CAN2
#if (CAN_USE_CAN2 == 1)
/*Function name:  CAN2_TXM2_ISR
Parameters:     -
Returns:        -
Description:    CAN2 Transmit interrupt (channel 2).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT(CAN2_TXM2_ISR, VECT_CAN2_TXM2)    /* See mcu_mapped_interrupts.h. */
R_BSP_ATTRIB_INTERRUPT void CAN2_TXM2_ISR(void)
{
    can_tx_callback[2]();
}
/******************************************************************************
 End of function CAN2_TXM2_ISR
 *****************************************************************************/ /* end CAN2_TXM2_ISR() */

/*Function name:  CAN2_TXF2_ISR
Parameters:     -
Returns:        -
Description:    CAN2 fifo Transmit interrupt (channel 2).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT(CAN2_TXF2_ISR, VECT_CAN2_TXF2)    /* See mcu_mapped_interrupts.h. */
R_BSP_ATTRIB_INTERRUPT void CAN2_TXF2_ISR(void)
{
    can_txf_callback[2]();
}
/******************************************************************************
 End of function CAN2_TXF2_ISR
 *****************************************************************************/ /* end CAN2_TXF2_ISR() */

/*****************************************************************************
Name:           CAN2_RXM2_ISR()
Parameters:     -
Returns:        -
Description:    CAN2 Receive interrupt (channel 2).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT(CAN2_RXM2_ISR, VECT_CAN2_RXM2)    /* See mcu_mapped_interrupts.h. */
R_BSP_ATTRIB_INTERRUPT void CAN2_RXM2_ISR(void)
{
    can_rx_callback[2]();
}
/******************************************************************************
 End of function CAN2_RXM2_ISR
 *****************************************************************************//* end CAN2_RXM2_ISR() */

/*****************************************************************************
Name:           CAN2_RXF2_ISR()
Parameters:     -
Returns:        -
Description:    CAN2 fifo Receive interrupt (channel 2).
*****************************************************************************/
R_BSP_PRAGMA_INTERRUPT(CAN2_RXF2_ISR, VECT_CAN2_RXF2)    /* See mcu_mapped_interrupts.h. */
R_BSP_ATTRIB_INTERRUPT void CAN2_RXF2_ISR(void)
{
    can_rxf_callback[2]();
}
/******************************************************************************
 End of function CAN2_RXR2_ISR
 *****************************************************************************//* end CAN2_RXR2_ISR() */
#endif /* CAN_USE_CAN2 == 1 */
#endif /* CAN2 */

#endif /* !USE_CAN_POLL */

/*****************************************************************************
Function name:  universal_can_callback
Parameters:     -
Returns:        -
Description:    Callback in case user did not provide one. Will execute for
                ALL CAN interrupts! Probably not what you want. See CAN API
                application note.
*****************************************************************************/
void universal_can_callback(void)
{
    R_BSP_NOP();
}
/******************************************************************************
 End of function universal_can_callback
 *****************************************************************************/
/* end universal_can_callback() */

/* file end */
