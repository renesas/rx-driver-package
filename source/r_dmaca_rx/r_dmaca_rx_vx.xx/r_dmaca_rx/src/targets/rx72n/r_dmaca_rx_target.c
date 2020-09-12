/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_dmaca_rx_target.c
* Device(s)    : RX
* Tool-Chain   : Renesas RXC Toolchain v3.01.00
* OS           : not use
* H/W Platform : not use
* Description  : Functions for DMACA driver
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.12.2019 1.00     First Release for RX72N.
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
/* Definitions for DMACA support */
#include "r_dmaca_rx_if.h"
#include "./src/r_dmaca_rx_private.h"

/* Check MCU Group. */
#if defined(BSP_MCU_RX72N)

/*******************************************************************************
Macro definitions
*******************************************************************************/
typedef void (* DMACI_Callback)(void);

/*******************************************************************************
Private variables and functions
*******************************************************************************/

/* DMACA DMAC0I */
R_BSP_PRAGMA_STATIC_INTERRUPT(r_dmaca_intdmac0i_isr, VECT(DMAC, DMAC0I))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac0i_isr(void);

/* DMACA DMAC1I */
R_BSP_PRAGMA_STATIC_INTERRUPT(r_dmaca_intdmac1i_isr, VECT(DMAC, DMAC1I))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac1i_isr(void);

/* DMACA DMAC2I */
R_BSP_PRAGMA_STATIC_INTERRUPT(r_dmaca_intdmac2i_isr, VECT(DMAC, DMAC2I))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac2i_isr(void);

/* DMACA DMAC3I */
R_BSP_PRAGMA_STATIC_INTERRUPT(r_dmaca_intdmac3i_isr, VECT(DMAC, DMAC3I))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac3i_isr(void);

/* DMACA DMAC74I */
R_BSP_PRAGMA_STATIC_INTERRUPT(r_dmaca_intdmac74i_isr, VECT(DMAC, DMAC74I))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac74i_isr(void);


/* Array of address of DMRSRm (m=DMACA channel number) */
volatile uint8_t R_BSP_EVENACCESS_SFR *g_icu_dmrsr[] =
{
    /* Initialize Array of address of DMRSRm */
    &(ICU.DMRSR0), &(ICU.DMRSR1), &(ICU.DMRSR2), &(ICU.DMRSR3),

    /* Initialize Array of address of DMRSRm */
    &(ICU.DMRSR4), &(ICU.DMRSR5), &(ICU.DMRSR6), &(ICU.DMRSR7)
};

/*******************************************************************************
Exported global variables (to be accessed by other files)
*******************************************************************************/


/*******************************************************************************
* Function Name: r_dmaca_channel_valid_check
* Description  : Checks whether channel is valid or not.
* Arguments    : channel -
*                    Which channel to use
* Return Value : true -
*                    Channel is valid.
*                false -
*                    Channel is invalid.
*******************************************************************************/
bool r_dmaca_channel_valid_check(uint8_t channel)
{
    bool ret = false;

    switch (channel)
    {
        case DMACA_CH0:
        case DMACA_CH1:
        case DMACA_CH2:
        case DMACA_CH3:
        case DMACA_CH4:
        case DMACA_CH5:
        case DMACA_CH6:
        case DMACA_CH7:
        {
            /* The channel number is valid. */
            ret = true;
            break;
        }
        default:
        {
            /* The channel number is invalid. */
            ret = false;
            break;
        }
    }

    return ret;
}
/**************************************************************************
* End of function r_dmaca_channel_valid_check
*************************************************************************/

/************************************************************************************
* Outline      : Interrupt DMAC0I handler
* Function Name: r_dmaca_intdmac0i_isr
* Description  : Types of interrupt requests from DMACA channel 0.
*                If the pointer to function is 'FIT_NO_FUNC' or 'NULL',
*                write a program under "do something" in this
*                r_dmaca_intdmac0i_isr() function.
* Arguments    : None
* Return Value : None
************************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac0i_isr(void)
{
    /* If the pointer to function is not 'NULL' */
    if (((uint32_t)NULL != (uint32_t)g_pdmaci_handlers[DMACA_CH0]) &&

       /* If the pointer to function is not 'FIT_NO_FUNC' */
       ((uint32_t)FIT_NO_FUNC != (uint32_t)g_pdmaci_handlers[DMACA_CH0]))
    {
        /* Callback function handles for channel 0 */
        ((DMACI_Callback)g_pdmaci_handlers[DMACA_CH0])();
    }
    else
    {
        /* do something */
    }

    return;
}
/**************************************************************************
* End of function r_dmaca_intdmac0i_isr
*************************************************************************/

/************************************************************************************
* Outline      : Interrupt DMAC1I handler
* Function Name: r_dmaca_intdmac1i_isr
* Description  : Types of interrupt requests from DMACA channel1.
*                If the pointer to function is 'FIT_NO_FUNC' or 'NULL',
*                write a program under "do something" in this 
*                r_dmaca_intdmac1i_isr() function.
* Arguments    : None
* Return Value : None
************************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac1i_isr(void)
{
    /* If the pointer to function is not 'NULL' */
    if (((uint32_t)NULL != (uint32_t)g_pdmaci_handlers[DMACA_CH1]) &&

       /* If the pointer to function is not 'FIT_NO_FUNC' */
       ((uint32_t)FIT_NO_FUNC != (uint32_t)g_pdmaci_handlers[DMACA_CH1]))
    {
        /* Callback function handles for channel 1 */
        ((DMACI_Callback)g_pdmaci_handlers[DMACA_CH1])();
    }
    else
    {
        /* do something */
    }

    return;
}
/**************************************************************************
* End of function r_dmaca_intdmac1i_isr
*************************************************************************/

/************************************************************************************
* Outline      : Interrupt DMAC2I handler
* Function Name: r_dmaca_intdmac2i_isr
* Description  : Types of interrupt requests from DMACA channel2.
*                If the pointer to function is 'FIT_NO_FUNC' or 'NULL',
*                write a program under "do something" in this 
*                r_dmaca_intdmac2i_isr() function.
* Arguments    : None
* Return Value : None
************************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac2i_isr(void)
{
    /* If the pointer to function is not 'NULL' */
    if (((uint32_t)NULL != (uint32_t)g_pdmaci_handlers[DMACA_CH2]) &&

       /* If the pointer to function is not 'FIT_NO_FUNC' */
       ((uint32_t)FIT_NO_FUNC != (uint32_t)g_pdmaci_handlers[DMACA_CH2]))
    {
        /* Callback function handles for channel 2 */
        ((DMACI_Callback)g_pdmaci_handlers[DMACA_CH2])();
    }
    else
    {
        /* do something */
    }

    return;
}
/**************************************************************************
* End of function r_dmaca_intdmac2i_isr
*************************************************************************/

/************************************************************************************
* Outline      : Interrupt DMAC3I handler
* Function Name: r_dmaca_intdmac3i_isr
* Description  : Types of interrupt requests from DMACA channel3.
*                If the pointer to function is 'FIT_NO_FUNC' or 'NULL',
*                write a program under "do something" in this 
*                r_dmaca_intdmac3i_isr() function.
* Arguments    : None
* Return Value : None
************************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac3i_isr(void)
{
    /* If the pointer to function is not 'NULL' */
    if (((uint32_t)NULL != (uint32_t)g_pdmaci_handlers[DMACA_CH3]) &&

       /* If the pointer to function is not 'FIT_NO_FUNC' */
       ((uint32_t)FIT_NO_FUNC != (uint32_t)g_pdmaci_handlers[DMACA_CH3]))
    {
        /* Callback function handles for channel 3 */
        ((DMACI_Callback)g_pdmaci_handlers[DMACA_CH3])();
    }
    else
    {
        /* do something */
    }

    return;
}
/**************************************************************************
* End of function r_dmaca_intdmac3i_isr
*************************************************************************/

/************************************************************************************
* Outline      : Interrupt DMAC74I handler
* Function Name: r_dmaca_intdmac74i_isr
* Description  : Types of interrupt requests from DMACA channel4 to channel7.
*                If the pointer to function is 'FIT_NO_FUNC' or 'NULL',
*                write a program under the "do something" in this
*                r_dmaca_intdmac74i_isr() function.
* Arguments    : None
* Return Value : None
************************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void r_dmaca_intdmac74i_isr(void)
{
    uint8_t channel;

    /* Get DMAC interrupt status */
    channel = DMAC.DMIST.BYTE;

    if (0x00 != (channel & DMACA_DMIS4_BIT_MASK))
    {
        /* If the pointer to function is not 'NULL' */
        if (((uint32_t)NULL != (uint32_t)g_pdmaci_handlers[DMACA_CH4]) &&

           /* If the pointer to function is not 'FIT_NO_FUNC' */
           ((uint32_t)FIT_NO_FUNC != (uint32_t)g_pdmaci_handlers[DMACA_CH4]))
        {
            /* Callback function handles for channel 4 */
            ((DMACI_Callback)g_pdmaci_handlers[DMACA_CH4])();
        }
        else
        {
            /* do something */
        }
    }

    if (0x00 != (channel & DMACA_DMIS5_BIT_MASK))
    {
        /* If the pointer to function is not 'NULL' */
        if (((uint32_t)NULL != (uint32_t)g_pdmaci_handlers[DMACA_CH5]) &&

           /* If the pointer to function is not 'FIT_NO_FUNC' */
           ((uint32_t)FIT_NO_FUNC != (uint32_t)g_pdmaci_handlers[DMACA_CH5]))
        {
            /* Callback function handles for channel 5 */
            ((DMACI_Callback)g_pdmaci_handlers[DMACA_CH5])();
        }
        else
        {
            /* do something */
        }
    }

    if (0x00 != (channel & DMACA_DMIS6_BIT_MASK))
    {
        /* If the pointer to function is not 'NULL' */
        if (((uint32_t)NULL != (uint32_t)g_pdmaci_handlers[DMACA_CH6]) &&

           /* If the pointer to function is not 'FIT_NO_FUNC' */
           ((uint32_t)FIT_NO_FUNC != (uint32_t)g_pdmaci_handlers[DMACA_CH6]))
        {
            /* Callback function handles for channel 6 */
            ((DMACI_Callback)g_pdmaci_handlers[DMACA_CH6])();
        }
        else
        {
            /* do something */
        }
    }

    if (0x00 != (channel & DMACA_DMIS7_BIT_MASK))
    {
        /* If the pointer to function is not 'NULL' */
        if (((uint32_t)NULL != (uint32_t)g_pdmaci_handlers[DMACA_CH7]) &&

           /* If the pointer to function is not 'FIT_NO_FUNC' */
           ((uint32_t)FIT_NO_FUNC != (uint32_t)g_pdmaci_handlers[DMACA_CH7]))
        {
            /* Callback function handles for channel 7 */
            ((DMACI_Callback)g_pdmaci_handlers[DMACA_CH7])();
        }
        else
        {
            /* do something */
        }
    }

    return;
}
/**************************************************************************
* End of function r_dmaca_intdmac74i_isr
*************************************************************************/

/*******************************************************************************
* Function Name: r_dmaca_int_disable
* Description  : Disables DMACmI interrupt. (m = 0 to 3, or 74)
* Arguments    : channel -
*                    Which channel to use
* Return Value : DMACA_SUCCESS -
*                    DMACmI interrupt is disabled successfully.
*                DMACA_ERR_INVALID_CH -
*                    Channel is invalid.
*******************************************************************************/
dmaca_return_t r_dmaca_int_disable(uint8_t channel)
{

    /* Clear the DMACmI interrupt Enable bit. */
    /* Clear the DMACmI priority level. */
    switch (channel)
    {
        case DMACA_CH0:
        {
            /* Clear the DMAC0I interrupt Enable bit. */
        	R_BSP_InterruptRequestDisable(VECT(DMAC,DMAC0I));

            /* Clear the DMAC0I priority level. */
            IPR(DMAC, DMAC0I) = 0;
            break;
        }
        case DMACA_CH1:
        {
            /* Clear the DMAC1I interrupt Enable bit. */
        	R_BSP_InterruptRequestDisable(VECT(DMAC,DMAC1I));

            /* Clear the DMAC1I priority level. */
            IPR(DMAC, DMAC1I) = 0;
            break;
        }
        case DMACA_CH2:
        {
            /* Clear the DMAC2I interrupt Enable bit. */
        	R_BSP_InterruptRequestDisable(VECT(DMAC,DMAC2I));

            /* Clear the DMAC2I priority level. */
            IPR(DMAC, DMAC2I) = 0;
            break;
        }
        case DMACA_CH3:
        {
            /* Clear the DMAC3I interrupt Enable bit. */
        	R_BSP_InterruptRequestDisable(VECT(DMAC,DMAC3I));

            /* Clear the DMAC3I priority level. */
            IPR(DMAC, DMAC3I) = 0;
            break;
        }
        case DMACA_CH4:
        {
            if (!((g_locking_sw[DMACA_CH5] || g_locking_sw[DMACA_CH6]) || g_locking_sw[DMACA_CH7]))
            {

                /* Clear the DMAC74I interrupt Enable bit. */
            	R_BSP_InterruptRequestDisable(VECT(DMAC,DMAC74I));

                /* Clear the DMAC74I priority level. */
                IPR(DMAC, DMAC74I) = 0;
            }
            break;
        }
        case DMACA_CH5:
        {
            if (!((g_locking_sw[DMACA_CH4] || g_locking_sw[DMACA_CH6]) || g_locking_sw[DMACA_CH7]))
            {

                /* Clear the DMAC74I interrupt Enable bit. */
            	R_BSP_InterruptRequestDisable(VECT(DMAC,DMAC74I));

                /* Clear the DMAC74I priority level. */
                IPR(DMAC, DMAC74I) = 0;
            }
            break;
        }
        case DMACA_CH6:
        {
            if (!((g_locking_sw[DMACA_CH4] || g_locking_sw[DMACA_CH5]) || g_locking_sw[DMACA_CH7]))
            {

                /* Clear the DMAC74I interrupt Enable bit. */
            	R_BSP_InterruptRequestDisable(VECT(DMAC,DMAC74I));

                /* Clear the DMAC74I priority level. */
                IPR(DMAC, DMAC74I) = 0;
            }
            break;
        }
        case DMACA_CH7:
        {
            if (!((g_locking_sw[DMACA_CH4] || g_locking_sw[DMACA_CH5]) || g_locking_sw[DMACA_CH6]))
            {

                /* Clear the DMAC74I interrupt Enable bit. */
            	R_BSP_InterruptRequestDisable(VECT(DMAC,DMAC74I));

                /* Clear the DMAC74I priority level. */
                IPR(DMAC, DMAC74I) = 0;
            }
            break;
        }
        default:
        {
            /* The channel number is invalid. */
            return DMACA_ERR_INVALID_CH;
            break;
        }
    }

    /* Check interrupt priority of DMAC0I*/
    if (0x00 == IPR(DMAC, DMAC0I))
    {
        /* do nothing */
    }

    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function r_dmaca_int_disable
*************************************************************************/

/*******************************************************************************
* Function Name: r_dmaca_int_enable
* Description  : Enables DMACmI interrupt. (m = 0 to 3, or 74)
* Arguments    : channel -
*                    Which channel to use
*              : priority -
*                    DMACmI interrupt priority level
* Return Value : DMACA_SUCCESS -
*                    DMACmI interrupt is enabled successfully.
*                DMACA_ERR_INVALID_CH -
*                    Channel is invalid.
*******************************************************************************/
dmaca_return_t r_dmaca_int_enable(uint8_t channel, uint8_t priority)
{
    /* Set the DMACmI priority level. */
    /* Set the DMACmI interrupt Enable bit. */
    switch (channel)
    {
        case DMACA_CH0:
        {

            /* Set the DMAC0I priority level. */
            IPR(DMAC, DMAC0I) = priority;

            /* Set the DMAC0I interrupt Enable bit. */
            R_BSP_InterruptRequestEnable(VECT(DMAC,DMAC0I));
            break;
        }
        case DMACA_CH1:
        {
            /* Set the DMAC1I priority level. */
            IPR(DMAC, DMAC1I) = priority;

            /* Set the DMAC1I interrupt Enable bit. */
            R_BSP_InterruptRequestEnable(VECT(DMAC,DMAC1I));
            break;
        }
        case DMACA_CH2:
        {

            /* Set the DMAC2I priority level. */
            IPR(DMAC, DMAC2I) = priority;

            /* Set the DMAC2I interrupt Enable bit. */
            R_BSP_InterruptRequestEnable(VECT(DMAC,DMAC2I));
            break;
        }
        case DMACA_CH3:
        {

            /* Set the DMAC3I priority level. */
            IPR(DMAC, DMAC3I) = priority;

            /* Set the DMAC3I interrupt Enable bit. */
            R_BSP_InterruptRequestEnable(VECT(DMAC,DMAC3I));
            break;
        }
        case DMACA_CH4:
        case DMACA_CH5:
        case DMACA_CH6:
        case DMACA_CH7:
        {
            /* The interrupt number is DMAC74I. */
            if (IPR(DMAC, DMAC74I) < priority)
            {

                /* Set the DMAC74I priority level. */
                IPR(DMAC, DMAC74I) = priority;
            }

            /* Set the DMAC3I interrupt Enable bit. */
            R_BSP_InterruptRequestEnable(VECT(DMAC,DMAC74I));
            break;
        }
        default:
        {
            /* The channel number is invalid. */
            return DMACA_ERR_INVALID_CH;
            break;
        }
    }

    /* Check interrupt enable of DMAC0I*/
    if (0x00 == IEN(DMAC, DMAC0I))
    {
        /* do nothing */
    }

    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function r_dmaca_int_enable
*************************************************************************/

/*******************************************************************************
* Function Name: r_dmaca_module_enable
* Description  : Releases module stop state.
* Arguments    : None
* Return Value : None
*******************************************************************************/
void r_dmaca_module_enable(void)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
bsp_int_ctrl_t int_ctrl;
#endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

    /* Release from module stop state. */
    MSTP(DMAC) = 0;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}
/**************************************************************************
* End of function r_dmaca_module_enable
*************************************************************************/

/*******************************************************************************
* Function Name: r_dmaca_module_disable
* Description  : Sets to module stop state.
* Arguments    : None
* Return Value : None
*******************************************************************************/
void r_dmaca_module_disable(void)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
bsp_int_ctrl_t int_ctrl;
#endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

    /* Set to module stop state. */
    MSTP(DMAC) = 1;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}
/**************************************************************************
* End of function r_dmaca_module_disable
*************************************************************************/

#if ((0 == BSP_CFG_USER_LOCKING_ENABLED) && (bsp_lock_t == BSP_CFG_USER_LOCKING_TYPE))
#if (1 != DMACA_CFG_USE_DTC_FIT_MODULE)
/*******************************************************************************
* Function Name: r_dmaca_check_dtc_locking_byuser
* Description  : Checks DTC locking by user.
* Arguments    : none -
* Return Value : DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED -
*                    All DMAC channels and DTC are unlocked.
*                DMACA_ALL_CH_UNLOCKED_BUT_DTC_LOCKED -
*                    All DMAC channels are unlocked, but DTC is locked.
*******************************************************************************/
dmaca_chk_locking_sw_t r_dmaca_check_dtc_locking_byuser(void)
{
    dmaca_chk_locking_sw_t  ret = DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED;
    
    /* User has to check the locking of DTC by themselves. */
    /* do something */

    return ret;
}
/**************************************************************************
* End of function r_dmaca_check_dtc_locking_byuser
*************************************************************************/
#endif  /* defined(1 != DMACA_CFG_USE_DTC_FIT_MODULE) */

#else
/*******************************************************************************
* Function Name: r_dmaca_check_DMACA_DTC_locking_byUSER
* Description  : Checks DMACA and DTC locking by user.
* Arguments    : none -
* Return Value : DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED -
*                    All DMAC channels and DTC are unlocked.
*                DMACA_ALL_CH_UNLOCKED_BUT_DTC_LOCKED -
*                    All DMAC channels are unlocked, but DTC is locked.
*                DMACA_LOCKED_CH_EXIST -
*                    Other DMAC channels are locked.
*******************************************************************************/
dmaca_chk_locking_sw_t r_dmaca_check_DMACA_DTC_locking_byUSER(void)
{
    dmaca_chk_locking_sw_t  ret = DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED;

    /* User has to check the locking of DMACA and DTC by themselves. */
    /* do something */

    return ret;
}

#endif

#endif /* defined(BSP_MCU_RX72N) */

/* End of File */

