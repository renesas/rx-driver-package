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
 * Copyright (C) 2018(2019) Renesas Electronics Corporation. All rights reserved.
  ******************************************************************************/
/*******************************************************************************
 * File Name    : r_usb_reg_access.c
 * Description  : USB IP register access code
 *****************************************************************************/
/******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 30.11.2018 1.00 First Release
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 ******************************************************************************/
 /******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_reg_access.h"
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
#include "r_usb_dmac.h"
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */


/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/


/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

extern uint16_t     g_usb_cstd_bemp_skip[USB_MAX_PIPE_NO + 1u];
/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void *hw_usb_get_fifosel_adr (uint16_t pipemode);
static void *hw_usb_get_fifoctr_adr (uint16_t pipemode);



/******************************************************************************
 Function Name   : hw_usb_read_syscfg
 Description     : Returns the specified port's SYSCFG register value.
 Arguments       : none
 Return value    : SYSCFG content.
 ******************************************************************************/
uint16_t hw_usb_read_syscfg (void)
{
    return USB0.SYSCFG.WORD;
} /*  End of function hw_usb_read_syscfg */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_write_syscfg
 Description     : Write specified value to the SYSCFG register of the given port.
 Arguments       : uint16_t     data   : Value to write.
 Return value    : none
 ******************************************************************************/
void hw_usb_write_syscfg (uint16_t data)
{
    USB0.SYSCFG.WORD = data;
} /*  End of function hw_usb_write_syscfg */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_set_cnen
 Description     : Enable single end receiver.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_cnen(void)
{
    /* Only USBA module */
    USB0.SYSCFG.WORD |= USB_CNEN;
} /* End of function hw_usb_set_cnen */

/******************************************************************************
 Function Name   : hw_usb_clear_cnen
 Description     : Disable single end receiver.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_cnen(void)
{
    USB0.SYSCFG.WORD &= (~USB_CNEN);
} /* End of function hw_usb_clear_cnen */


#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
/******************************************************************************
 Function Name   : hw_usb_set_dcfm
 Description     : DCFM-bit set of register SYSCFG
                 : (USB Host mode is selected.)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_dcfm (void)
{
    USB0.SYSCFG.WORD |= USB_DCFM;
} /* End of function hw_usb_set_dcfm */
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/******************************************************************************
 Function Name   : hw_usb_clear_dcfm
 Description     : DCFM-bit clear of register SYSCFG.
                 : (USB Peripheral mode is selected.)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_dcfm (void)
{
    USB0.SYSCFG.WORD &= (~USB_DCFM);
} /* End of function hw_usb_clear_dcfm */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_clear_drpd
 Description     : Clear bit of the specified port's SYSCFG DRPD register.
                 : (for USB Host mode; Enable D + / D-line PullDown.)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_drpd (void)
{
    USB0.SYSCFG.WORD &= (~USB_DRPD);
} /* End of function hw_usb_clear_drpd */
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_set_usbe
 Description     : Enable USB operation.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_usbe (void)
{
    USB0.SYSCFG.WORD |= USB_USBE;
} /* End of function hw_usb_set_usbe */

/******************************************************************************
 Function Name   : hw_usb_clear_usbe
 Description     : Enable USB operation.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_usbe (void)
{
    USB0.SYSCFG.WORD &= (~USB_USBE);
} /* End of function hw_usb_clear_usbe */

/******************************************************************************
Function Name   : hw_usb_set_bcctrl
Description     : Set BCCTRL's bits.
Arguments       : uint16_t      data   : Setting value
Return value    : none
******************************************************************************/
void    hw_usb_set_bcctrl (uint16_t data)
{
    USB0.USBBCCTRL0.WORD |= data;
} /* End of function hw_usb_set_bcctrl */

/******************************************************************************
Function Name   : hw_usb_clear_bcctrl
Description     : Clear BCCTRL's bits.
Arguments       : uint16_t  data    : Setting value
Return value    : none
******************************************************************************/
void    hw_usb_clear_bcctrl (uint16_t data)
{
    USB0.USBBCCTRL0.WORD &= (~data);
} /* End of function hw_usb_clear_bcctrl */

/******************************************************************************
 Function Name   : hw_usb_read_syssts
 Description     : Returns the value of the specified port's SYSSTS register.
 Arguments       : none
 Return value    : SYSSTS0 content
 ******************************************************************************/
uint16_t hw_usb_read_syssts (void)
{
    return (uint16_t)(USB0.SYSSTS0.WORD);
} /* End of function hw_usb_read_syssts */

/******************************************************************************
 Function Name   : hw_usb_read_dvstctr
 Description     : Returns the specified port's DVSTCTR register content.
 Arguments       : none
 Return value    : DVSTCTR0 content
 ******************************************************************************/
uint16_t hw_usb_read_dvstctr (void)
{
    return (uint16_t)(USB0.DVSTCTR0.WORD);
} /* End of function hw_usb_read_dvstctr */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_write_dvstctr
 Description     : Write data to the specified port's DVSTCTR register.
 Arguments       : uint16_t  data  : Setting value  
 Return value    : none
 ******************************************************************************/
void hw_usb_write_dvstctr (uint16_t data)
{
    USB0.DVSTCTR0.WORD = data;
} /* End of function hw_usb_write_dvstctr */

/******************************************************************************
 Function Name   : hw_usb_rmw_dvstctr
 Description     : Read-modify-write the specified port's DVSTCTR.
 Arguments       : uint16_t  data  : The value to write.                
                 : uint16_t  bitptn: Bit pattern to read-modify-write.
 Return value    : none
 ******************************************************************************/
void hw_usb_rmw_dvstctr (uint16_t data, uint16_t bitptn)
{
    uint16_t buf;

    buf = USB0.DVSTCTR0.WORD;
    buf &= (~bitptn);
    buf |= (data & bitptn);
    USB0.DVSTCTR0.WORD = buf;
} /* End of function hw_usb_rmw_dvstctr */

/******************************************************************************
 Function Name   : hw_usb_clear_dvstctr
 Description     : Clear the bit pattern specified in argument, of the specified 
                 : port's DVSTCTR register.
 Arguments       : uint16_t  bitptn: Bit pattern to read-modify-write.
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_dvstctr (uint16_t bitptn)
{
    USB0.DVSTCTR0.WORD &= (~bitptn);
} /* End of function hw_usb_clear_dvstctr */

/******************************************************************************
 Function Name   : hw_usb_set_vbout
 Description     : Set specified port's VBOUT-bit in the DVSTCTR register.
                 : (To output a "High" to pin VBOUT.) 
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_vbout (void)
{
#if USB_CFG_VBUS == USB_CFG_HIGH
    USB0.DVSTCTR0.WORD |= USB_VBUSEN;
#else  /* USB_CFG_VBUS == USB_CFG_HIGH */
    USB0.DVSTCTR0.WORD &= (~USB_VBUSEN);
#endif /* USB_CFG_VBUS == USB_CFG_HIGH */
} /* End of function hw_usb_set_vbout */

/******************************************************************************
 Function Name   : hw_usb_clear_vbout
 Description     : Clear specified port's VBOUT-bit in the DVSTCTR register.
                 : (To output a "Low" to pin VBOUT.)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_vbout (void)
{
#if USB_CFG_VBUS == USB_CFG_HIGH
    USB0.DVSTCTR0.WORD &= (~USB_VBUSEN);
#else  /* USB_CFG_VBUS == USB_CFG_HIGH */
    USB0.DVSTCTR0.WORD |= USB_VBUSEN;
#endif /* USB_CFG_VBUS == USB_CFG_HIGH */
} /* End of function hw_usb_clear_vbout */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_read_fifo16
 Description     : Data is read from the specified pipemode's FIFO register, 16-bits 
                 : wide, corresponding to the specified PIPEMODE.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA
 Return value    : CFIFO/D0FIFO/D1FIFO content (16-bit)
 ******************************************************************************/
uint16_t hw_usb_read_fifo16 (uint16_t pipemode)
{
    uint16_t data;

    switch (pipemode)
    {
        case USB_CUSE :
            data = USB0.CFIFO.WORD;
        break;
        case USB_D0USE :
            data = USB0.D0FIFO.WORD;
        break;
        case USB_D1USE :
            data = USB0.D1FIFO.WORD;
        break;
        default :
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_STD | USB_DEBUG_HOOK_CODE4);
        break;
    }

    return data;
} /* End of function hw_usb_read_fifo16 */

/******************************************************************************
 Function Name   : hw_usb_write_fifo16
 Description     : Data is written to the specified pipemode's FIFO register, 16-bits 
                 : wide, corresponding to the specified PIPEMODE.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA
                 : uint16_t  data      : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_fifo16 (uint16_t pipemode, uint16_t data)
{
    switch (pipemode)
    {
        case USB_CUSE :
            USB0_CFIFO16 = data;
        break;
        case USB_D0USE :
            USB0_D0FIFO16 = data;
        break;
        case USB_D1USE :
            USB0_D1FIFO16 = data;
        break;
        default :
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_STD | USB_DEBUG_HOOK_CODE6);
        break;
    }
} /* End of function hw_usb_write_fifo16 */

/******************************************************************************
 Function Name   : hw_usb_write_fifo8
 Description     : Data is written to the specified pipemode's FIFO register, 8-bits 
                 : wide, corresponding to the specified PIPEMODE.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA
                 : uint8_t  data       : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_fifo8 (uint16_t pipemode, uint8_t data)
{
    switch (pipemode)
    {
        case USB_CUSE :
            USB0_CFIFO8 = data;
        break;
        case USB_D0USE :
            USB0_D0FIFO8 = data;
        break;
        case USB_D1USE :
            USB0_D1FIFO8 = data;
        break;
        default :
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_STD | USB_DEBUG_HOOK_CODE9);
        break;
    }
} /* End of function hw_usb_write_fifo8 */

/******************************************************************************
 Function Name   : hw_usb_get_fifosel_adr
 Description     : Returns the *address* of the FIFOSEL register corresponding to 
                 : specified PIPEMODE.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA
 Return value    : none
 ******************************************************************************/
static void *hw_usb_get_fifosel_adr (uint16_t pipemode)
{
    void *p_reg;

    switch (pipemode)
    {
        case USB_CUSE:
            p_reg = (void *)&(USB0.CFIFOSEL);
        break;
        case USB_D0USE:
            p_reg = (void *)&(USB0.D0FIFOSEL);
        break;
        case USB_D1USE:
            p_reg = (void *)&(USB0.D1FIFOSEL);
        break;
        default:
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_STD | USB_DEBUG_HOOK_CODE12);
        break;
    }

    return p_reg;
} /* End of function hw_usb_get_fifosel_adr */

/******************************************************************************
 Function Name   : hw_usb_read_fifosel
 Description     : Returns the value of the specified pipemode's FIFOSEL register.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA       
 Return value    : FIFOSEL content
 ******************************************************************************/
uint16_t hw_usb_read_fifosel (uint16_t pipemode)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *) hw_usb_get_fifosel_adr(pipemode);

    return *p_reg;
} /* End of function hw_usb_read_fifosel */

/******************************************************************************
 Function Name   : hw_usb_rmw_fifosel
 Description     : Data is written to the specified pipemode's FIFOSEL register 
                 : (the FIFOSEL corresponding to the specified PIPEMODE), using 
                 : read-modify-write.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA
                 : uint16_t  data      : Setting value
                 : uint16_t bitptn   : bitptn: Bit pattern to read-modify-write.
 Return value    : none
 ******************************************************************************/
void hw_usb_rmw_fifosel (uint16_t pipemode, uint16_t data, uint16_t bitptn)
{
    uint16_t buf;
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *) hw_usb_get_fifosel_adr(pipemode);

    buf = *p_reg;
    buf &= (~bitptn);
    buf |= (data & bitptn);
    *p_reg = buf;
} /*  End of function hw_usb_rmw_fifosel */

/******************************************************************************
 Function Name   : hw_usb_set_dclrm
 Description     : Set DCLRM-bits (FIFO buffer auto clear) of the FIFOSEL cor-
                 : responding to specified PIPEMODE.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA
 Return value    : none
 ******************************************************************************/
void hw_usb_set_dclrm (uint16_t pipemode)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *) hw_usb_get_fifosel_adr(pipemode);

    (*p_reg) |= USB_DCLRM;
} /* End of function hw_usb_set_dclrm */

/******************************************************************************
 Function Name   : hw_usb_clear_dclrm
 Description     : Reset DCLRM-bits (FIFO buffer not auto-cleared) of the FIFOSEL 
                 : corresponding to the specified PIPEMODE.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_dclrm (uint16_t pipemode)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = hw_usb_get_fifosel_adr(pipemode);

    (*p_reg) &= (~USB_DCLRM);
} /*  End of function hw_usb_clear_dclrm */

/******************************************************************************
 Function Name   : hw_usb_set_dreqe
 Description     : Set DREQE-bits (to output signal DxREQ_Na) of the FIFOSEL cor-
                 : responding to specified PIPEMODE.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA
 Return value    : none
 ******************************************************************************/
void hw_usb_set_dreqe (uint16_t pipemode)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = hw_usb_get_fifosel_adr(pipemode);

    (*p_reg) |= USB_DREQE;
} /*  End of function hw_usb_set_dreqe */

/******************************************************************************
 Function Name   : hw_usb_clear_dreqe
 Description     : Clear DREQE-bits (To prohibit the output of the signal DxREQ_N)
                 : of the FIFOSEL corresponding to the specified PIPEMODE.
 Arguments       : uint16_t  pipemode  : CUSE/D0DMA/D1DMA      
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_dreqe (uint16_t pipemode)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = hw_usb_get_fifosel_adr(pipemode);

    (*p_reg) &= (~USB_DREQE);
}  /*  End of function hw_usb_clear_dreqe */

/******************************************************************************
 Function Name   : hw_usb_set_mbw
 Description     : Set MBW-bits (CFIFO Port Access Bit Width) of the FIFOSEL cor-
                 : responding to the specified PIPEMODE, to select 8 or 16-bit 
                 : wide FIFO port access.
 Arguments       : uint16_t  pipemode   : CUSE/D0DMA/D1DMA
                 : uint16_t  data       : Setting value
                 : (data = 0x0400), 32 bit (data = 0x0800) access mode.
 Return value    : none
 ******************************************************************************/
void hw_usb_set_mbw (uint16_t pipemode, uint16_t data)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = hw_usb_get_fifosel_adr(pipemode);

    (*p_reg) &= (~USB_MBW);
    if (0 != data)
    {
        (*p_reg) |= data;
    }
} /* End of function hw_usb_set_mbw */

/******************************************************************************
 Function Name   : hw_usb_set_curpipe
 Description     : Set pipe to the number given; in the FIFOSEL corresponding 
                 : to specified PIPEMODE.    
                 : uint16_t  pipeno     : Pipe number.
 Return value    : none
 ******************************************************************************/
void hw_usb_set_curpipe (uint16_t pipemode, uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;
    R_BSP_VOLATILE_EVENACCESS uint16_t reg;

    p_reg = hw_usb_get_fifosel_adr(pipemode);
    reg = *p_reg;

    if ((USB_D0USE == pipemode) || (USB_D1USE == pipemode))
    {
      reg &= (~USB_DREQE);
    }

    reg &= (~USB_CURPIPE);
    *p_reg = reg;
    /* WAIT_LOOP */
    while (0 != ((*p_reg) & USB_CURPIPE))
    {
        /* Wait Clear CURPIPE */
    }

    reg |= pipeno;

    *p_reg = reg;
} /* End of function hw_usb_set_curpipe */

/******************************************************************************
 Function Name   : hw_usb_get_fifoctr_adr
 Description     : Returns the *address* of the FIFOCTR register corresponding to 
                 : specified PIPEMODE.
                 : (FIFO Port Control Register.)
 Arguments       : uint16_t  pipemode   : CUSE/D0DMA/D1DMA
 Return value    : none
 ******************************************************************************/
static void *hw_usb_get_fifoctr_adr (uint16_t pipemode)
{
    void *p_reg;

    switch (pipemode)
    {
        case USB_CUSE:
            p_reg = (void *)&(USB0.CFIFOCTR);

        break;

        case    USB_D0USE:
            p_reg = (void *)&(USB0.D0FIFOCTR);

            break;

        case    USB_D1USE:
            p_reg = (void *)&(USB0.D1FIFOCTR);

        break;
        default:
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_STD | USB_DEBUG_HOOK_CODE13);
        break;
    }

    return p_reg;
} /* End of function hw_usb_get_fifoctr_adr */

/******************************************************************************
 Function Name   : hw_usb_read_fifoctr
 Description     : Returns the value of the FIFOCTR register corresponding to 
                 : specified PIPEMODE.
 Arguments       : uint16_t  pipemode   : CUSE/D0DMA/D1DMA    
 Return value    : FIFOCTR content
 ******************************************************************************/
uint16_t hw_usb_read_fifoctr (uint16_t pipemode)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *) hw_usb_get_fifoctr_adr(pipemode);

    return *p_reg;
} /* End of function hw_usb_read_fifoctr */

/******************************************************************************
 Function Name   : hw_usb_set_bval
 Description     : Set BVAL (Buffer Memory Valid Flag) to the number given; in 
                 : the FIFOCTR corresponding to the specified PIPEMODE.
 Arguments       : uint16_t  pipemode   : CUSE/D0DMA/D1DMA
 Return value    : none
 ******************************************************************************/
void hw_usb_set_bval (uint16_t pipemode)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *) hw_usb_get_fifoctr_adr(pipemode);

    (*p_reg) |= USB_BVAL;
} /* End of function hw_usb_set_bval */

/******************************************************************************
 Function Name   : hw_usb_set_bclr
 Description     : Set BCLR (CPU Buffer Clear) to the number given; in the 
                 : FIFOCTR corresponding to the specified PIPEMODE.
 Arguments       : uint16_t  pipemode   : CUSE/D0DMA/D1DMA      
 Return value    : none
 ******************************************************************************/
void hw_usb_set_bclr (uint16_t pipemode)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *) hw_usb_get_fifoctr_adr(pipemode);

    *p_reg = USB_BCLR;
} /* End of function hw_usb_set_bclr */

/******************************************************************************
 Function Name   : hw_usb_write_intenb
 Description     : Data is written to INTENB register, 
                 : enabling/disabling the various USB interrupts.
 Arguments       : uint16_t  data  : The value to write.
 Return value    : none
 ******************************************************************************/
void hw_usb_write_intenb (uint16_t data)
{
    USB0.INTENB0.WORD = data;
}
/******************************************************************************
 End of function hw_usb_write_intenb
 ******************************************************************************/

/******************************************************************************
 Function Name   : hw_usb_set_intenb
 Description     : Bit(s) to be set in INTENB register, 
                 : enabling the respective USB interrupt(s).
 Arguments       : uint16_t  data  : Bit pattern: Respective interrupts with '1' 
                 :                   will be enabled.
 Return value    : none
 ******************************************************************************/
void hw_usb_set_intenb (uint16_t data)
{
    USB0.INTENB0.WORD |= data;
} /* End of function hw_usb_set_intenb */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_clear_enb_vbse
 Description     : Clear the VBE-bit of INTENB register,
                 : to prohibit VBUS interrupts.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_enb_vbse (void)
{
    USB0.INTENB0.WORD &= (~USB_VBSE);
} /* End of function hw_usb_clear_enb_vbse */

/******************************************************************************
 Description     : Clear the SOFE-bit of INTENB register,
 : to prohibit Frame Number Update interrupts.
 Function Name   : hw_usb_clear_enb_sofe
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_enb_sofe (void)
{
    USB0.INTENB0.WORD &= (~USB_SOFE);

} /* End of function hw_usb_clear_enb_sofe */

/******************************************************************************
 Function Name   : hw_usb_write_brdyenb
 Description     : Data is written to BRDYENB register, 
                 : enabling/disabling each respective pipe's BRDY interrupt. 
                 : (The BRDY interrupt indicates that a FIFO port is accessible.)
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_brdyenb (uint16_t data)
{
    USB0.BRDYENB.WORD = data;

}  /* End of function hw_usb_write_brdyenb */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_set_brdyenb
 Description     : A bit is set in the specified pipe's BRDYENB, enabling the 
                 : respective pipe BRDY interrupt(s).
 Arguments       : uint16_t  pipeno : Pipe number.
 Return value    : none
 ******************************************************************************/
void hw_usb_set_brdyenb (uint16_t pipeno)
{
    USB0.BRDYENB.WORD |= (1 << pipeno);
} /* End of function hw_usb_set_brdyenb */

/******************************************************************************
 Function Name   : hw_usb_clear_brdyenb
 Description     : Clear the PIPExBRDYE-bit of the specified pipe to prohibit 
                 : BRDY interrupts of that pipe.
 Arguments       : uint16_t  pipeno : Pipe number.   
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_brdyenb (uint16_t pipeno)
{
    USB0.BRDYENB.WORD &= (~(1 << pipeno));
} /* End of function hw_usb_clear_brdyenb */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_write_nrdyenb
 Description     : Data is written to NRDYENB register, 
                 : enabling/disabling each respective pipe's NRDY interrupt
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_nrdyenb (uint16_t data)
{
    USB0.NRDYENB.WORD = data;
} /* End of function hw_usb_write_nrdyenb */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_set_nrdyenb
 Description     : A bit is set in the specified pipe's NRDYENB, enabling the 
                 : respective pipe NRDY interrupt(s).
 Arguments       : uint16_t  pipeno : Pipe number      
 Return value    : none
 ******************************************************************************/
void hw_usb_set_nrdyenb (uint16_t pipeno)
{
    USB0.NRDYENB.WORD |= (1 << pipeno);
} /* End of function hw_usb_set_nrdyenb */

/******************************************************************************
 Function Name   : hw_usb_clear_nrdyenb
 Description     : Clear the PIPExNRDYE-bit of the specified pipe to prohibit 
                 : NRDY interrupts of that pipe.
 Arguments       : uint16_t  pipeno : Pipe number.
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_nrdyenb (uint16_t pipeno)
{

    USB0.NRDYENB.WORD &= (~(1 << pipeno));

} /* End of function hw_usb_clear_nrdyenb */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_write_bempenb
 Description     : Data is written to BEMPENB register, 
                 : enabling/disabling each respective pipe's BEMP interrupt. 
                 : (The BEMP interrupt indicates that the USB buffer is empty, 
                 : and so the FIFO can now be written to.)
 Arguments       : uint16_t  data   : The value to write.
 Return value    : none
 ******************************************************************************/
void hw_usb_write_bempenb (uint16_t data)
{
    USB0.BEMPENB.WORD = data;
} /* End of function hw_usb_write_bempenb */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_set_bempenb
 Description     : A bit is set in the specified pipe's BEMPENB enabling the 
                 : respective pipe's BEMP interrupt(s).
 Arguments       : uint16_t  pipeno  : Pipe number.
 Return value    : none
 ******************************************************************************/
void hw_usb_set_bempenb (uint16_t pipeno)
{

    g_usb_cstd_bemp_skip[pipeno] = USB_OFF;
    USB0.BEMPENB.WORD |= (1 << pipeno);

} /* End of function hw_usb_set_bempenb */

/******************************************************************************
 Function Name   : hw_usb_clear_bempenb
 Description     : Clear the PIPExBEMPE-bit of the specified pipe to prohibit 
                 : BEMP interrupts of that pipe.
 Arguments       : uint16_t  pipeno : Pipe number.
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_bempenb (uint16_t pipeno)
{
    USB0.BEMPENB.WORD &= (~(1 << pipeno));
} /* End of function hw_usb_clear_bempenb */

/******************************************************************************
 Function Name   : hw_usb_read_intsts
 Description     : Returns INTSTS0 register content.
 Arguments       : none
 Return value    : INTSTS0 content
 ******************************************************************************/
uint16_t hw_usb_read_intsts (void)
{
    return USB0.INTSTS0.WORD;
} /* End of function hw_usb_read_intsts */

/******************************************************************************
 Function Name   : hw_usb_write_intsts
 Description     : Data is written to INTSTS0 register.
 Arguments       : uint16_t  data  : The value to write.      
 Return value    : none
 ******************************************************************************/
void hw_usb_write_intsts (uint16_t data)
{
    USB0.INTSTS0.WORD = data;
} /* End of function hw_usb_write_intsts */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_clear_sts_sofr
 Description     : Clear the SOFR-bit (Frame Number Refresh Interrupt Status) of 
                 : the clear SOF interrupt status.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_sts_sofr (void)
{
    USB0.INTSTS0.WORD = (uint16_t) ~USB_SOFR;

} /*  End of function hw_usb_clear_sts_sofr */

/******************************************************************************
 Function Name   : hw_usb_read_brdysts
 Description     : Returns BRDYSTS register content.
 Arguments       : none
 Return value    : BRDYSTS content
 ******************************************************************************/
uint16_t hw_usb_read_brdysts (void)
{
    return USB0.BRDYSTS.WORD;

} /* End of function hw_usb_read_brdysts */

/******************************************************************************
 Function Name   : hw_usb_write_brdysts
 Description     : Data is written to BRDYSTS register, to set the BRDY interrupt status.
 Arguments       : uint16_t  data  : The value to write.
 Return value    : none
 ******************************************************************************/
void hw_usb_write_brdysts (uint16_t data)
{
    USB0.BRDYSTS.WORD = data;
} /* End of function hw_usb_write_brdysts() */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_clear_sts_brdy
 Description     : Clear the PIPExBRDY status bit of the specified pipe to clear 
                 : its BRDY interrupt status.
 Arguments       : uint16_t  pipeno: Pipe number.
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_sts_brdy (uint16_t pipeno)
{
    USB0.BRDYSTS.WORD = (uint16_t) (~(1 << pipeno)) & USB_BRDYSTS_MASK;
} /* End of function hw_usb_clear_sts_brdy */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_write_nrdy_sts
 Description     : Data is written to NRDYSTS register, to
                 : set the NRDY interrupt status.
 Arguments       : uint16_t  data  : The value to write.
 Return value    : none
 ******************************************************************************/
void hw_usb_write_nrdy_sts (uint16_t data)
{
    USB0.NRDYSTS.WORD = data;
} /*  End of function hw_usb_write_nrdy_sts */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_clear_status_nrdy
 Description     : Clear the PIPExNRDY status bit of the specified pipe to clear 
                 : its NRDY interrupt status.
 Arguments       : uint16_t  pipeno: Pipe number.
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_status_nrdy (uint16_t pipeno)
{
    USB0.NRDYSTS.WORD = (uint16_t) ((~(1 << pipeno)) & USB_NRDYSTS_MASK);
} /* End of function hw_usb_clear_status_nrdy */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_write_bempsts
 Description     : Data is written to BEMPSTS register, to set the BEMP interrupt status.
 Arguments       : uint16_t  data  : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_bempsts (uint16_t data)
{
    USB0.BEMPSTS.WORD = data;
} /* End of function hw_usb_write_bempsts */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_clear_status_bemp
 Description     : Clear the PIPExBEMP status bit of the specified pipe to clear 
                 : its BEMP interrupt status.
 Arguments       : uint16_t  pipeno: Pipe number.
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_status_bemp (uint16_t pipeno)
{
    USB0.BEMPSTS.WORD = (uint16_t) ((~(1 << pipeno)) & USB_BEMPSTS_MASK);
} /* End of function hw_usb_clear_status_bemp */

/******************************************************************************
 Function Name   : hw_usb_read_frmnum
 Description     : Returns FRMNUM register content.
 Arguments       : none
 Return value    : FRMNUM content
 ******************************************************************************/
uint16_t hw_usb_read_frmnum (void)
{
    return (uint16_t)USB0.FRMNUM.WORD;
} /* End of function hw_usb_read_frmnum */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
/******************************************************************************
 Function Name   : hw_usb_read_usbreq
 Description     : Returns USBREQ register content.
 Arguments       : none
 Return value    : USBREQ content
 ******************************************************************************/
uint16_t hw_usb_read_usbreq (void)
{
    return (uint16_t)USB0.USBREQ.WORD;
} /* End of function hw_usb_read_usbreq */

/******************************************************************************
 Function Name   : hw_usb_read_usbval
 Description     : Returns USBVAL register content.
 Arguments       : none
 Return value    : USBVAL content
 ******************************************************************************/
uint16_t hw_usb_read_usbval (void)
{
    return (uint16_t)USB0.USBVAL;
} /* End of function hw_usb_read_usbval */

/******************************************************************************
 Function Name   : hw_usb_read_usbindx
 Description     : Returns USBINDX register content.
 Arguments       : none
 Return value    : USBINDX content
 ******************************************************************************/
uint16_t hw_usb_read_usbindx (void)
{
    return (uint16_t)USB0.USBINDX;
} /* End of function hw_usb_read_usbindx */

/******************************************************************************
 Function Name   : hw_usb_read_usbleng
 Description     : Returns USBLENG register content.
 Arguments       : none
 Return value    : USBLENG content
 ******************************************************************************/
uint16_t hw_usb_read_usbleng (void)
{
    return (uint16_t)USB0.USBLENG;
} /* End of function hw_usb_read_usbleng */
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/******************************************************************************
 Function Name   : hw_usb_read_dcpcfg
 Description     : Returns DCPCFG register content.
 Arguments       : none
 Return value    : DCPCFG content
 ******************************************************************************/
uint16_t hw_usb_read_dcpcfg (void)
{
    return (uint16_t)USB0.DCPCFG.WORD;
} /* End of function hw_usb_read_dcpcfg */

/******************************************************************************
 Function Name   : hw_usb_write_dcpcfg
 Description     : Specified data is written to DCPCFG register.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_dcpcfg (uint16_t data)
{
    USB0.DCPCFG.WORD = data;
} /* End of function hw_usb_write_dcpcfg */

/******************************************************************************
 Function Name   : hw_usb_read_dcpmaxp
 Description     : Returns DCPMAXP register content.
 Arguments       : void
 Return value    : DCPMAXP content
 ******************************************************************************/
uint16_t hw_usb_read_dcpmaxp (void)
{
    return (uint16_t)USB0.DCPMAXP.WORD;
} /* End of function hw_usb_read_dcpmaxp */

/******************************************************************************
 Function Name   : hw_usb_write_dcpmxps
 Description     : Specified data is written to DCPMAXP register.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_dcpmxps (uint16_t data)
{
    USB0.DCPMAXP.WORD = data;
} /* End of function hw_usb_write_dcpmxps */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
/******************************************************************************
 Function Name   : hw_usb_read_dcpctr
 Description     : Returns DCPCTR register content.
 Arguments       : none
 Return value    : DCPCTR content
 ******************************************************************************/
uint16_t hw_usb_read_dcpctr (void)
{
    return (uint16_t)USB0.DCPCTR.WORD;
} /* End of function hw_usb_read_dcpctr */

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/******************************************************************************
 Function Name   : hw_usb_write_pipesel
 Description     : Specified data is written to PIPESEL register.
 Arguments       : uint16_t  data   : The value to write.
 Return value    : none
 ******************************************************************************/
void hw_usb_write_pipesel (uint16_t data)
{
    USB0.PIPESEL.WORD = data;
} /* End of function hw_usb_write_pipesel */

/******************************************************************************
 Function Name   : hw_usb_read_pipecfg
 Description     : Returns PIPECFG register content.
 Arguments       : none
 Return value    : PIPECFG content
 ******************************************************************************/
uint16_t hw_usb_read_pipecfg (void)
{
    return (uint16_t)USB0.PIPECFG.WORD;
} /* End of function hw_usb_read_pipecfg */

/******************************************************************************
 Function Name   : hw_usb_write_pipecfg
 Description     : Specified data is written to PIPECFG register.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_pipecfg (uint16_t data)
{
    USB0.PIPECFG.WORD = data;
} /*  End of function hw_usb_write_pipecfg */

/******************************************************************************
 Function Name   : hw_usb_read_pipemaxp
 Description     : Returns PIPEMAXP register content.
 Arguments       : none
 Return value    : PIPEMAXP content
 ******************************************************************************/
uint16_t hw_usb_read_pipemaxp (void)
{
    return (uint16_t)USB0.PIPEMAXP.WORD;
} /* End of function hw_usb_read_pipemaxp */

/******************************************************************************
 Function Name   : hw_usb_write_pipemaxp
 Description     : Specified data is written to PIPEMAXP register.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_pipemaxp (uint16_t data)
{
    USB0.PIPEMAXP.WORD = data;
} /* End of function hw_usb_write_pipemaxp*/

/******************************************************************************
 Function Name   : hw_usb_write_pipeperi
 Description     : Specified data is written to PIPEPERI register.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_pipeperi (uint16_t data)
{
    USB0.PIPEPERI.WORD = data;
} /* End of function hw_usb_write_pipeperi*/

/******************************************************************************
 Function Name   : hw_usb_read_pipectr
 Description     : Returns DCPCTR or the specified pipe's PIPECTR register content.
                 : The Pipe Control Register returned is determined by the speci-
                 : fied pipe number.
 Arguments       : uint16_t  pipeno : Pipe number.
 Return value    : PIPExCTR content
 ******************************************************************************/
uint16_t hw_usb_read_pipectr (uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    if (USB_PIPE0 == pipeno)
    {
        p_reg = (uint16_t *)&(USB0.DCPCTR);
    }
    else
    {
        p_reg = (uint16_t *)&(USB0.PIPE1CTR) + (pipeno - 1);
    }
    return *p_reg;
} /*  End of function hw_usb_read_pipectr */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Function Name   : hw_usb_write_pipectr
 Description     : Specified data is written to the specified pipe's PIPEPERI register.
 Arguments       : uint16_t  pipeno : Pipe number
                 : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_write_pipectr (uint16_t pipeno, uint16_t data)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    if (USB_PIPE0 == pipeno)
    {
        p_reg = (uint16_t *) &(USB0.DCPCTR);
    }
    else
    {
        p_reg = (uint16_t *) &(USB0.PIPE1CTR) + (pipeno - 1);
    }
    *p_reg = data;

} /* End of function hw_usb_write_pipectr */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_set_aclrm
 Description     : The ACLRM-bit (Auto Buffer Clear Mode) is set in the specified 
                 : pipe's control register.
 Arguments       : uint16_t  pipeno : Pipe number    
 Return value    : none
 ******************************************************************************/
void hw_usb_set_aclrm (uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *)&(USB0.PIPE1CTR) + (pipeno - 1);
    (*p_reg) |= USB_ACLRM;
}  /*  End of function hw_usb_set_aclrm */

/******************************************************************************
 Function Name   : hw_usb_clear_aclrm
 Description     : Clear the ACLRM bit in the specified pipe's control register
                 : to disable Auto Buffer Clear Mode.
                 : its BEMP interrupt status.
 Arguments       : uint16_t  pipeno : Pipe number
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_aclrm (uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *)&(USB0.PIPE1CTR) + (pipeno - 1);
    (*p_reg) &= (~USB_ACLRM);
}  /* End of function hw_usb_clear_aclrm */

/******************************************************************************
 Function Name   : hw_usb_set_sqclr
 Description     : The SQCLR-bit (Toggle Bit Clear) is set in the specified pipe's 
                 : control register. Setting SQSET to 1 makes DATA0 the expected 
                 : data in the pipe's next transfer.
 Arguments       : uint16_t  pipeno : Pipe number
 Return value    : none
 ******************************************************************************/
void hw_usb_set_sqclr (uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    if (USB_PIPE0 == pipeno)
    {
        USB0.DCPCTR.WORD |= USB_SQCLR;
    }
    else
    {
        p_reg = ((uint16_t *)&(USB0.PIPE1CTR) + (pipeno - 1));
        (*p_reg) |= USB_SQCLR;
    }
} /* End of function hw_usb_set_sqclr*/

/******************************************************************************
 Function Name   : hw_usb_set_sqset
 Description     : The SQSET-bit (Toggle Bit Set) is set in the specified pipe's 
                 : control register. Setting SQSET to 1 makes DATA1 the expected 
                 : data in the next transfer.
 Arguments       : uint16_t  pipeno : Pipe number
 Return value    : none
 ******************************************************************************/
void hw_usb_set_sqset (uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    if (USB_PIPE0 == pipeno)
    {
        USB0.DCPCTR.WORD |= USB_SQSET;
    }
    else
    {
        p_reg = ((uint16_t *)&(USB0.PIPE1CTR) + (pipeno - 1));
        (*p_reg) |= USB_SQSET;
    }
} /* End of function hw_usb_set_sqset */

/******************************************************************************
 Function Name   : hw_usb_set_pid
 Description     : Set the specified PID of the specified pipe's DCPCTR/PIPECTR 
                 : register.
 Arguments       : uint16_t  pipeno : Pipe number
                 : uint16_t  data   : NAK/BUF/STALL.
 Return value    : none
 ******************************************************************************/
void hw_usb_set_pid (uint16_t pipeno, uint16_t data)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    if (USB_PIPE0 == pipeno)
    {
        p_reg = ((uint16_t *)&(USB0.DCPCTR));
    }
    else
    {
        p_reg = ((uint16_t *)&(USB0.PIPE1CTR) + (pipeno - 1));
    }

    (*p_reg) &= (~USB_PID);
    (*p_reg) |= data;
} /* End of function hw_usb_set_pid */

/******************************************************************************
 Function Name   : hw_usb_clear_pid
 Description     : Clear the specified PID-bits of the specified pipe's DCPCTR/
                 : PIPECTR register.
 Arguments       : uint16_t  pipeno : Pipe number
                 : uint16_t  data  : NAK/BUF/STALL - to be cleared.
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_pid (uint16_t pipeno, uint16_t data)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    if (USB_PIPE0 == pipeno)
    {
        p_reg = ((uint16_t *)&(USB0.DCPCTR));
    }
    else
    {
        p_reg = ((uint16_t *)&(USB0.PIPE1CTR) + (pipeno - 1));
    }

    (*p_reg) &= (~data);
} /* End of function hw_usb_clear_pid */

/******************************************************************************
 Function Name   : hw_usb_set_trenb
 Description     : The TRENB-bit (Transaction Counter Enable) is set in the speci-
                 : fied pipe's control register, to enable the counter.
 Arguments       : uint16_t  pipeno : Pipe number
 Return value    : none
 ******************************************************************************/
void hw_usb_set_trenb (uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *)&(USB0.PIPE1TRE) + ((pipeno - 1) * 2);
    (*p_reg) |= USB_TRENB;
} /* End of function hw_usb_set_trenb*/

/******************************************************************************
 Function Name   : hw_usb_clear_trenb
 Description     : The TRENB-bit (Transaction Counter Enable) is cleared in the 
                 : specified pipe's control register, to disable the counter.
 Arguments       : uint16_t  pipeno : Pipe number
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_trenb (uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *)&(USB0.PIPE1TRE) + ((pipeno - 1) * 2);
    (*p_reg) &= (~USB_TRENB);
} /* End of function hw_usb_clear_trenb */

/******************************************************************************
 Function Name   : hw_usb_set_trclr
 Description     : The TRENB-bit (Transaction Counter Clear) is set in the speci-
                 : fied pipe's control register to clear the current counter 
                 : value.
 Arguments       : uint16_t  pipeno : Pipe number
 Return value    : none
 ******************************************************************************/
void hw_usb_set_trclr (uint16_t pipeno)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;
    p_reg = (uint16_t *)&(USB0.PIPE1TRE) + ((pipeno - 1) * 2);

    (*p_reg) |= USB_TRCLR;
} /* End of function hw_usb_set_trclr */

/******************************************************************************
 Function Name   : hw_usb_write_pipetrn
 Description     : Specified data is written to the specified pipe's PIPETRN reg-
                 : ister.
 Arguments       : uint16_t  pipeno : Pipe number
                 : uint16_t  data   : The value to write.
 Return value    : none
 ******************************************************************************/
void hw_usb_write_pipetrn (uint16_t pipeno, uint16_t data)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;

    p_reg = (uint16_t *)&(USB0.PIPE1TRN) + ((pipeno - 1) * 2);
    *p_reg = data;
} /*  End of function hw_usb_write_pipetrn */



#if USB_CFG_BC == USB_CFG_ENABLE
/******************************************************************************
 Function Name   : hw_usb_read_bcctrl
 Description     : Returns BCCTRL register content.
 Arguments       : none
 Return value    : BCCTRL content
 ******************************************************************************/
uint16_t hw_usb_read_bcctrl (void)
{
    return USB0.USBBCCTRL0.WORD;
} /* End of function hw_usb_read_bcctrl() */
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */


#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
#if USB_CFG_BC == USB_CFG_ENABLE
/******************************************************************************
 Function Name   : hw_usb_set_vdmsrce
 Description     : Set VDMSRCE bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_vdmsrce (void)
{
    USB0.USBBCCTRL0.WORD |= USB_VDMSRCE;
} /* End of function hw_usb_set_vdmsrce */

/******************************************************************************
 Function Name   : hw_usb_clear_vdmsrce
 Description     : Clear VDMSRCE bits.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_vdmsrce (void)
{
    USB0.USBBCCTRL0.WORD &= (~USB_VDMSRCE);
} /* End of function hw_usb_clear_vdmsrce */

/******************************************************************************
 Function Name   : hw_usb_set_idpsinke
 Description     : Set IDPSINKE bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_idpsinke (void)
{
    USB0.USBBCCTRL0.WORD |= USB_IDPSINKE;
} /* End of function hw_usb_set_idpsinke */

/******************************************************************************
 Function Name   : hw_usb_clear_idpsinke
 Description     : Clear IDPSINKE bits.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_idpsinke (void)
{
    USB0.USBBCCTRL0.WORD &= (~USB_IDPSINKE);
} /* End of function hw_usb_clear_idpsinke */

#endif  /* USB_CFG_BC == USB_CFG_ENABLE */
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Function Name   : hw_usb_set_vdcen
 Description     : Set VDCEN bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_vdcen(void)
{
    USB0.USBMC.WORD |= USB_VDCEN;
} /* End of function  hw_usb_set_vdcen */

/******************************************************************************
 Function Name   : hw_usb_set_vddusbe
 Description     : Set VDDUSBE bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_vddusbe(void)
{
    USB0.USBMC.WORD |= USB_VDDUSBE;
} /* End of function hw_usb_set_vddusbe */

/******************************************************************************
 Function Name   : hw_usb_set_batchge
 Description     : Set BATCHGE bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_set_batchge(void)
{
    USB0.USBBCCTRL0.WORD |= USB_BATCHGE;
} /* End of function hw_usb_set_batchge */

/******************************************************************************
 Function Name   : hw_usb_clear_batchge
 Description     : Clear BATCHGE bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_clear_batchge(void)
{
    USB0.USBBCCTRL0.WORD &= (~USB_BATCHGE);
} /* End of function hw_usb_clear_batchge */

/******************************************************************************
 End of file
 ******************************************************************************/
