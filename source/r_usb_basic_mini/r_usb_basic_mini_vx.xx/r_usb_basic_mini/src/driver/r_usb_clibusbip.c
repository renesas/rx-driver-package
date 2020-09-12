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
 * Copyright (C) 2015(2018) Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * File Name    : r_usb_clibusbip.c
 * Description  : USB IP Host and Peripheral low level library
 ******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 29.12.2015 1.02    Minor Update.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_reg_access.h"   /* Definition of the USB register access macro */
#include "r_usb_extern.h"

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_cstd_nrdy_enable
 Description     : Enable NRDY interrupt of the specified pipe.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : none
 ******************************************************************************/
void usb_cstd_nrdy_enable (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Enable NRDY */
    hw_usb_set_nrdyenb(pipe);
} /* End of function usb_cstd_nrdy_enable */

/******************************************************************************
 Function Name   : usb_cstd_get_pid
 Description     : Fetch specified pipe's PID.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint16_t PID-bit status
 ******************************************************************************/
uint16_t usb_cstd_get_pid (uint16_t pipe)
{
    uint16_t buf;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

    /* PIPE control reg read */
    buf = hw_usb_read_pipectr(pipe);
    return (uint16_t) (buf & USB_PID);
} /* End of function usb_cstd_get_pid */

/******************************************************************************
 Function Name   : usb_cstd_get_pipe_dir
 Description     : Get PIPE DIR
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint16_t pipe direction.
 ******************************************************************************/
uint16_t usb_cstd_get_pipe_dir (uint16_t pipe)
{
    uint16_t buffer;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

    /* Pipe select */
    hw_usb_write_pipesel(pipe);

    /* Read Pipe direction */
    buffer = hw_usb_read_pipecfg();
    return (uint16_t) (buffer & USB_DIRFIELD);
}/* End of function usb_cstd_get_pipe_dir */

/******************************************************************************
 Function Name   : usb_cstd_get_pipe_type
 Description     : Fetch and return PIPE TYPE.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint16_t pipe type
 ******************************************************************************/
uint16_t usb_cstd_get_pipe_type (uint16_t pipe)
{
    uint16_t buffer;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

    /* Pipe select */
    hw_usb_write_pipesel(pipe);

    /* Read Pipe direction */
    buffer = hw_usb_read_pipecfg();
    return (uint16_t) (buffer & USB_TYPFIELD);
} /* End of function usb_cstd_get_pipe_type */

/******************************************************************************
 Function Name   : usb_cstd_do_aclrm
 Description     : Set the ACLRM-bit (Auto Buffer Clear Mode) of the specified 
                 : pipe.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : none
 ******************************************************************************/
void usb_cstd_do_aclrm (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Control ACLRM */
    hw_usb_set_aclrm(pipe);
    hw_usb_clear_aclrm(pipe);
} /* End of function usb_cstd_do_aclrm */

/******************************************************************************
 Function Name   : usb_cstd_set_buf
 Description     : Set PID (packet ID) of the specified pipe to BUF.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : none
 ******************************************************************************/
void usb_cstd_set_buf (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* PIPE control reg set */
    hw_usb_set_pid(pipe, USB_PID_BUF);
} /* End of function usb_cstd_set_buf */

/******************************************************************************
 Function Name   : usb_cstd_clr_stall
 Description     : Set up to NAK the specified pipe, and clear the STALL-bit set
                 : to the PID of the specified pipe.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : none
 Note            : PID is set to NAK.
 ******************************************************************************/
void usb_cstd_clr_stall (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Set NAK */
    usb_cstd_set_nak(pipe);

    /* Clear STALL */
    hw_usb_clear_pid(pipe, USB_PID_STALL);
} /* End of function usb_cstd_clr_stall */
 
 /******************************************************************************
Function Name   : usb_cstd_port_speed
Description     : Check current port speed
Arguments       : none
Return value    : uint16_t          ; FSCONNECT : Full-Speed
                :                   ; LSCONNECT : Low-Speed
                :                   ; NOCONNECT : not connect
******************************************************************************/
uint16_t usb_cstd_port_speed(void)
{
    uint16_t    buf;

    /* Reset handshake status get */
    buf = hw_usb_read_dvstctr();
    buf = (uint16_t) (buf & USB_RHST);

    /* Check RHST */
    if (USB_FSMODE == buf)
    {
        return  (USB_FSCONNECT);
    }
    if (USB_LSMODE == buf)
    {
        return  (USB_LSCONNECT);
    }
    return  (USB_NOCONNECT);
} /* End of function usb_cstd_port_speed */

/******************************************************************************
 End  Of File
 ******************************************************************************/
