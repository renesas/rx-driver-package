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
 * Copyright (C) 2013(2020) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usb_hsignal.c
 * Description  : Host USB signalling
 ***********************************************************************************************************************/
/**********************************************************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*           : 30.06.2020 1.20    Added support for RTOS.
 ***********************************************************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_basic_mini_if.h"

#if (BSP_CFG_RTOS_USED == 0)    /* Non-OS */

#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_reg_access.h"
#include "r_usb_extern.h"

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
#define USB_RESPONSE_COUNTER_VALUE     (6000u)

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */


/*******************************************************************************
 Typedef definitions
 ******************************************************************************/


/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* variables */

/* function */
USB_STATIC  void        usb_hstd_bchg_process(void);

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
/* variables */
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
uint16_t                g_usb_hstd_response_counter;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */


/******************************************************************************
 Renesas Abstracted Host Signal functions
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_interrupt_process
 Description     : Execute appropriate process depending on which USB interrupt 
                 : occurred.
 Argument        : none
 Return          : none
******************************************************************************/
void usb_hstd_interrupt_process(void)
{
    uint16_t        inttype;
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    usb_compliance_t disp_param;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    inttype = gp_usb_hstd_hcd_message->keyword;        /* p->inttype; */

    /***** common processing *****/
    switch (inttype)
    {
        /***** PIPE0-MAX_PIPE_NO data processing *****/
        case USB_INT_BRDY:
            usb_hstd_brdy_pipe();
        break;
        case USB_INT_BEMP:
            usb_hstd_bemp_pipe();
        break;
        case USB_INT_NRDY:
            usb_hstd_nrdy_pipe();
        break;

        /***** Setup transaction processing *****/
        case USB_INT_SACK:
            switch (g_usb_hstd_ctsq)
            {
                case USB_SETUPRD:
                    /* Next stage to Control read data */
                    usb_hstd_control_read_start();
                break;
                case USB_SETUPWR:
                    /* Next stage to Control Write data */
                    if (usb_hstd_control_write_start() == USB_FIFOERROR )
                    {
                        /* Control Read/Write End */
                        usb_hstd_control_end(USB_DATA_ERR);
                    }
                break;
                case USB_SETUPNDC:
                    /* Next stage to Control write no data */
                    usb_hstd_status_start();
                break;
                default:
                    /* no processing */
                break;
            }
        break;
        case USB_INT_SIGN:
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            disp_param.status = USB_CT_SETUP_ERR;
            disp_param.pid    = USB_NULL;
            disp_param.vid    = USB_NULL;
            usb_compliance_disp ((void *)&disp_param);
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
            /* Setup Device Ignore count over */
            usb_hstd_control_end(USB_DATA_ERR);
        break;

        /***** VBUS/SOF signal processing *****/
        case USB_INT_VBINT:
            /* User program */
            hw_usb_clear_enb_vbse();
        break;
        case USB_INT_SOFR:
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
        g_usb_hstd_response_counter++;
        if (USB_RESPONSE_COUNTER_VALUE == g_usb_hstd_response_counter)
        {
            hw_usb_clear_enb_sofe();
            disp_param.status = USB_CT_NORES;
            disp_param.pid    = USB_NULL;
            disp_param.vid    = USB_NULL;
            usb_compliance_disp ((void *)&disp_param);
            usb_hstd_control_end((uint16_t)USB_DATA_STOP);
        }
#else  /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
            /* User program */
            hw_usb_clear_enb_sofe();
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
        break;
        /*** ERROR ***/
        case USB_INT_UNKNOWN:
        /* continue */
        default:
            /* no processing */
        break;
    }

    /***** Port0 signal processing *****/
    g_usb_hstd_hcd_port = USB_PORT0;
    switch (inttype)
    {
        case USB_INT_OVRCR0:
            /* Port0 OVCR interrupt function */
            /* OVRCR interrupt disable */
            usb_hstd_ovrcr_disable();
            hw_usb_clear_vbout();
            usb_hstd_status_notification(USB_RTP_OVERCURRENT, USB_PORT0);
        break;
        case USB_INT_EOFERR0:
            /* User program */
        break;
        case USB_INT_ATTCH0:
            /* Port0 ATCH interrupt function */
            usb_hstd_attach_process();
        break;
        case USB_INT_BCHG0:
            /* Device State Control Register - Resume enable check */
            usb_hstd_bchg_process();
        break;
        case USB_INT_DTCH0:
            /* Port0 DTCH interrupt function */
            usb_hstd_detach_process();
        break;
#if USB_CFG_BC == USB_CFG_ENABLE
        case USB_INT_PDDETINT0:
            /* Port0 PDDETINT interrupt function */
            usb_hstd_pddetint_process();
        break;
#endif
        default:
            /* no processing */
        break;
    }
}
/******************************************************************************
 End of function usb_hstd_interrupt_process
 ******************************************************************************/

/******************************************************************************
Function Name   : usb_hstd_detach_control
 Description     : Set USB register as required when USB device is detached, and 
                 : notify MGR (manager) task that detach occurred.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_detach_control(void)
{
#if USB_CFG_BC == USB_CFG_ENABLE
    gp_usb_hstd_bc_func[g_usb_hstd_bc.state][USB_BC_EVENT_DT]();
#endif /* USB_CFG_BC == USB_CFG_ENABLE */

    /* DVSTCTR clear */
    hw_usb_clear_dvstctr((USB_RWUPE | USB_USBRST | USB_RESUME | USB_UACT));
    usb_cpu_delay_xms((uint16_t)1);
    /* ATTCH interrupt enable */
    usb_hstd_bchg_enable();
    usb_hstd_status_notification(USB_RTP_DETACH, (usb_strct_t)USB_PORT0);
}
/******************************************************************************
 End of function usb_hstd_detach_control
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_attach_control
 Description     : Set USB registers as required when USB device is attached, 
                 : and notify MGR (manager) task that attach event occurred.
 Arguments       : none
 Return value    : none
******************************************************************************/
void usb_hstd_attach_control(void)
{
    /* DTCH interrupt Enable */
    usb_hstd_detach_enable();

    /* Interrupt Enable */
    hw_usb_set_intenb(((USB_BEMPE | USB_NRDYE) | USB_BRDYE));

    /* USB Mng API */
    usb_hstd_status_notification(USB_RTP_ATTACH, (usb_strct_t)USB_PORT0);
#if USB_CFG_BC == USB_CFG_ENABLE
    gp_usb_hstd_bc_func[g_usb_hstd_bc.state][USB_BC_EVENT_AT]();
#endif /* USB_CFG_BC == USB_CFG_ENABLE */
}
/******************************************************************************
 End of function usb_hstd_attach_control
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_bchg_process
 Description     : Execute the process appropriate to the status of the connected 
                 : USB device when a BCHG interrupt occurred.
 Arguments       : none
 Return value    : none
******************************************************************************/
USB_STATIC void usb_hstd_bchg_process(void)
{
    uint16_t    buf;
    uint16_t    connect_inf;

    buf = hw_usb_read_dvstctr();

    /* Check remote wakeup enabled */
    if ((USB_YES == g_usb_hcd_remote_wakeup) && (USB_RESUME == (uint16_t)(buf & USB_RESUME)))
    {
        /* Change device state to resume */
        g_usb_hcd_remote_wakeup = USB_NO;
        usb_hstd_status_notification(USB_RTP_REMOTEWAKEUP, USB_PORT0);
    }
    else
    {
        /* Decide USB Line-State (ATTACH) */
        connect_inf = usb_hstd_check_attach();
        if (USB_LNST_DETACH == connect_inf)
        {
            g_usb_hcd_remote_wakeup = USB_NO;
            usb_hstd_detach_control();
        }
        else if (USB_LNST_ATTACH == connect_inf)
        {
            g_usb_hcd_remote_wakeup = USB_NO;
            usb_hstd_attach_control();
        }
        else
        {
            /* USB_RHST_ATTACH */
            usb_hstd_bchg_enable();
        }
    }
}
/******************************************************************************
 End of function usb_hstd_bchg_process
 ******************************************************************************/

/******************************************************************************
Function Name   : usb_hstd_vbus_control_on
Description     : USB VBUS control
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_vbus_control_on(void)
{
    /* To put 2port processing together on the lebel, */
    /* the function is not developed though nesting becomes deep. */
    hw_usb_set_vbout();

#if USB_CFG_BC == USB_CFG_ENABLE
    gp_usb_hstd_bc_func[g_usb_hstd_bc.state][USB_BC_EVENT_VB]();
#endif
}   /*  End of function usb_hstd_vbus_control_on() */


/******************************************************************************
Function Name   : usb_hstd_vbus_control_off
Description     : USB VBUS control
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_vbus_control_off(void)
{
    /* To put 2port processing together on the lebel, */
    /* the function is not developed though nesting becomes deep. */
    hw_usb_clear_vbout();
}   /*  End of function usb_hstd_vbus_control_off() */


/******************************************************************************
Function Name   : usb_hstd_suspend_process
 Description     : Set USB registers as required when USB Device status is moved
                 : to "Suspend".  
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_suspend_process(void)
{
    /* SOF stop & SOF check & enable remote wakeup & enable BCHG interrupt */
    if (USB_YES == g_usb_hcd_remote_wakeup)
    {
        /* SOF OFF */
        hw_usb_hclear_uact();

        /* Wait */
        usb_cpu_delay_xms((uint16_t)1);

        /* RWUPE=1, UACT=0 */
        hw_usb_hset_rwupe();

        /* Enable port BCHG interrupt */
        usb_hstd_bchg_enable();
    } 
    else
    {
        /* Suspend state of remote wake up prohibition */
        hw_usb_hclear_uact();
    }
    /* 5ms should continue even if the suspend state is the lowest. */
    usb_cpu_delay_xms((uint16_t)5);
}
/******************************************************************************
 End of function usb_hstd_suspend_process
 ******************************************************************************/

#endif  /*  (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#endif /*(BSP_CFG_RTOS_USED == 0)*/
/******************************************************************************
 End  Of File
******************************************************************************/
