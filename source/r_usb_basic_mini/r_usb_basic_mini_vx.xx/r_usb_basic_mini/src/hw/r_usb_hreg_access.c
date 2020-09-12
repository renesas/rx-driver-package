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
 * Copyright (C) 2015(2019) Renesas Electronics Corporation. All rights reserved.
  ******************************************************************************/
/*******************************************************************************
 * File Name    : r_usb_hreg_access.c
 * Description  : USB IP Host control register access code
 *****************************************************************************/
/******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 31.11.2018 1.10 Supporting Smart Configurator
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 ******************************************************************************/
/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"
#include "r_usb_extern.h"

#if ( (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST )
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
 Function Name   : hw_usb_hset_rwupe
 Description     : Set the RWUPE-bit specified port's DVSTCTR0 reg-
                 : ister. When host. To allow detection of remote wake-up from 
                 : a USB Function.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_rwupe (void)
{
    USB0.DVSTCTR0.WORD |= USB_RWUPE;
} /* End of function hw_usb_hset_rwupe */

/******************************************************************************
 Function Name   : hw_usb_hclear_rwupe
 Description     : Clear the RWUPE-bit specified port's DVSTCTR0 reg-
                 : ister. When host. To prohibit detection of remote wake-up from 
                 : a USB Function.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_rwupe (void)
{
    USB0.DVSTCTR0.WORD &= (~USB_RWUPE);
} /* End of function hw_usb_hclear_rwupe */


/******************************************************************************
 Function Name   : hw_usb_hset_resume
 Description     : Set the RESUME-bit specified port's DVSTCTR0 register 
                 : When host. To allow output of resume signal to a USB Function.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_resume (void)
{
    USB0.DVSTCTR0.WORD |= USB_RESUME;
} /* End of function hw_usb_hset_resume */


/******************************************************************************
 Function Name   : hw_usb_hclear_resume
 Description     : Clear the RESUME-bit specified port's DVSTCTR0 register 
                 : When host. To prohibit output of resume signal to a USB Func-
                 : tion.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_resume (void)
{
    USB0.DVSTCTR0.WORD &= (~USB_RESUME);
} /* End of function hw_usb_hclear_resume */

 
/******************************************************************************
 Function Name   : hw_usb_hset_uact
 Description     : Set UACT-bit (USB Bus Enable) specified port's DVSTCTR0 
                 : register. When Host, to output SOF.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_uact (void)
{
    USB0.DVSTCTR0.WORD |= USB_UACT;
} /* End of function hw_usb_hset_uact */

/******************************************************************************
 Function Name   : hw_usb_hclear_uact
 Description     : Clear UACT-bit (USB Bus Enable) specified port's DVSTCTR0 
                 : register. When Host, to prohibit output SOF.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_uact (void)
{
    USB0.DVSTCTR0.WORD &= (~USB_UACT);
} /* End of function hw_usb_hclear_uact */

/******************************************************************************
 Function Name   : hw_usb_hwrite_intenb
 Description     : Write the specified data to the specified port's INTENB register.
 Arguments       : uint16_t  data   : Setting value    
 Return value    : none
 ******************************************************************************/
void hw_usb_hwrite_intenb (uint16_t data)
{
    USB0.INTENB1.WORD = data;
} /* End of function hw_usb_hwrite_intenb */


/******************************************************************************
 Function Name   : hw_usb_hset_enb_ovrcre
 Description     : Set specified port's OVRCRE-bit (Overcurrent Input Change Int-
                 : errupt Status Enable) in the INTENB1 register.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_enb_ovrcre (void)
{
    USB0.INTENB1.WORD |= USB_OVRCRE;
} /* End of function hw_usb_hset_enb_ovrcre */

/******************************************************************************
 Function Name   : hw_usb_hclear_enb_ovrcre
 Description     : Clear the OVRCRE-bit of the specified port's INTENB1 register,
                 : to prohibit VBUS interrupts.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_enb_ovrcre (void)
{
    USB0.INTENB1.WORD &= (~USB_OVRCRE);
} /* End of function hw_usb_hclear_enb_ovrcre */

 
/******************************************************************************
 Function Name   : hw_usb_hset_enb_bchge
 Description     : The BCHGE-bit (USB Bus Change Interrupt Enable) is set in the 
                 : specified port's INTENB1 register. This will cause a BCHG 
                 : interrupt when a change of USB bus state has been detected.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_enb_bchge (void)
{
    USB0.INTENB1.WORD |= USB_BCHGE;
} /* End of function hw_usb_hset_enb_bchge */

/******************************************************************************
 Function Name   : hw_usb_hclear_enb_bchge
 Description     : The BCHGE-bit (USB Bus Change Interrupt Enable) is cleared in 
                 : the specified port's INTENB1 register.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_enb_bchge (void)
{
    USB0.INTENB1.WORD &= (~USB_BCHGE);
} /* End of function hw_usb_hclear_enb_bchge */

/******************************************************************************
 Function Name   : hw_usb_hset_enb_dtche
 Description     : Enable the specified port's DTCHE-interrupt "Disconnection 
                 : Detection" by setting the DTCHE-bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_enb_dtche (void)
{
    USB0.INTENB1.WORD |= USB_DTCHE;
} /* End of function hw_usb_hset_enb_dtche */


/******************************************************************************
 Function Name   : hw_usb_hclear_enb_dtche
 Description     : Disable the specified port's DTCHE-interrupt "Disconnection 
                 : Detection" by clearing the DTCHE-bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_enb_dtche (void)
{
    USB0.INTENB1.WORD &= (~USB_DTCHE);
} /* End of function hw_usb_hclear_enb_dtche */

/******************************************************************************
 Function Name   : hw_usb_hset_enb_attche
 Description     : Enable the specified port's ATTCHE-interrupt "Connection 
                 : Detection" by setting the ATTCHE-bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_enb_attche (void)
{
    USB0.INTENB1.WORD |= USB_ATTCHE;
} /* End of function hw_usb_hset_enb_attche */

/******************************************************************************
 Function Name   : hw_usb_hclear_enb_attche
 Description     : Disable the specified port's ATTCHE-interrupt "Disconnection 
                 : Detection" by clearing the ATTCHE-bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_enb_attche (void)
{
    USB0.INTENB1.WORD &= (~USB_ATTCHE);
} /* End of function hw_usb_hclear_enb_attche */

 
/******************************************************************************
 Function Name   : hw_usb_hset_enb_signe
 Description     : Enable the SIGNE-interrupt "Setup Transaction
                 : Error" by setting the SIGNE-bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_enb_signe (void)
{
    USB0.INTENB1.WORD |= USB_SIGNE;
} /* End of function hw_usb_hset_enb_signe */


/******************************************************************************
 Function Name   : hw_usb_hset_enb_sacke
 Description     : Enable the SACKE-interrupt "Setup Transaction 
                 : Normal Response" by setting the SACKE-bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_enb_sacke (void)
{
    USB0.INTENB1.WORD |= USB_SACKE;
} /* End of function hw_usb_hset_enb_sacke */


/******************************************************************************
 Function Name   : hw_usb_hset_enb_pddetinte
 Description     : Enable the PDDETINT-interrupt "Connection Detection for 
                 : Battery Charging Supporting Device" by setting 
                 : the PDDETINTE-bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_enb_pddetinte (void)
{
    USB0.INTENB1.WORD |= USB_PDDETINTE;
} /* End of function hw_usb_hset_enb_pddetinte */

/******************************************************************************
 Function Name   : hw_usb_hwrite_intsts
 Description     : Write the specified data to the specified port's INTSTS1 reg-
                 : ister.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_hwrite_intsts (uint16_t data)
{
    USB0.INTSTS1.WORD = data;
} /* End of function hw_usb_hwrite_intsts */

/******************************************************************************
 Function Name   : hw_usb_hclear_sts_ovrcr
 Description     : Clear the specified port's OVRCR-bit; "Overcurrent 
                 : Input Change Interrupt Status".
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_sts_ovrcr (void)
{
    USB0.INTSTS1.WORD = (uint16_t) ((~USB_OVRCR) & USB_INTSTS1_MASK);
} /* End of function hw_usb_hclear_sts_ovrcr */

/******************************************************************************
 Function Name   : hw_usb_hclear_sts_bchg
 Description     : Clear the specified port's BCHG-bit; "USB Bus Change Interrupt 
                 : Status".
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_sts_bchg (void)
{
    USB0.INTSTS1.WORD = (uint16_t) ((~USB_BCHG) & USB_INTSTS1_MASK);
} /* End of function hw_usb_hclear_sts_bchg() */

/******************************************************************************
 Function Name   : hw_usb_hclear_sts_dtch
 Description     : Clear the specified port's DTCH-bit; "USB Disconnection Detec-
                 : tion Interrupt Status".
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_sts_dtch (void)
{
    USB0.INTSTS1.WORD = (uint16_t) ((~USB_DTCH) & USB_INTSTS1_MASK);
} /* End of function hw_usb_hclear_sts_dtch */


/******************************************************************************
 Function Name   : hw_usb_hclear_sts_attch
 Description     : Clear the specified port's ATTCH-bit; "ATTCH Interrupt Status".
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_sts_attch (void)
{
    USB0.INTSTS1.WORD = (uint16_t) ((~USB_ATTCH) & USB_INTSTS1_MASK);
} /* End of function hw_usb_hclear_sts_attch */


/******************************************************************************
 Function Name   : hw_usb_hclear_sts_sign
 Description     : Clear the SIGN-bit; "Setup Transaction Error
                 : Interrupt Status".
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_sts_sign (void)
{
    USB0.INTSTS1.WORD = (uint16_t) ((~USB_SIGN) & USB_INTSTS1_MASK);
} /* End of function hw_usb_hclear_sts_sign */


/******************************************************************************
 Function Name   : hw_usb_hclear_sts_sack
 Description     : Clear the SACK-bit; "Setup Transaction Normal
                 : Response Interrupt Status".
                 : Interrupt Status".
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_sts_sack (void)
{
    USB0.INTSTS1.WORD = (uint16_t) ((~USB_SACK) & USB_INTSTS1_MASK);
} /* End of function hw_usb_hclear_sts_sack */


/******************************************************************************
 Function Name   : hw_usb_hclear_sts_pddetint
 Description     : Clear the PDDETINT-bit; "
                 : ".
                 : Interrupt Status".
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_sts_pddetint (void)
{
    USB0.INTSTS1.WORD = (uint16_t) ((~USB_PDDETINT) & USB_INTSTS1_MASK);
} /* End of function hw_usb_hclear_sts_pddetint */

/******************************************************************************
 Function Name   : hw_usb_hwrite_usbreq
 Description     : Write bRequest and bmRequestType to USBREQ register.
                 : When Host, the values of bRequest and bmRequestType 
                 : to be transmitted are written. (When Function, the received 
                 : values of bRequest and bmRequestType are stored in USBREQ).
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_hwrite_usbreq (uint16_t data)
{
    USB0.USBREQ.WORD = data;
} /* End of function hw_usb_hwrite_usbreq */

/******************************************************************************
 Function Name   : hw_usb_hset_usbval
 Description     : Write the specified 'wValue' to USBVAL register,
                 : to write the USB request. When Host, the value of 
                 : wValue to be transmitted is set. (When Function, the value of 
                 : wValue that has been received is stored in USBREQ.)
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_usbval (uint16_t data)
{
    USB0.USBVAL = data;
} /* End of function hw_usb_hset_usbval */

/******************************************************************************
 Function Name   : hw_usb_hset_usbindx
 Description     : Write the specified 'wIndex', the USB request, to USBINDX
                 : register, for host setup requests for control 
                 : transfers.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_usbindx (uint16_t data)
{
    USB0.USBINDX = data;
} /* End of function hw_usb_hset_usbindx */

/******************************************************************************
 Function Name   : hw_usb_hset_usbleng
 Description     : Write the specified 'wLength' value to USBINDX register, 
                 : for host setup requests for control.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_usbleng (uint16_t data)
{
    USB0.USBLENG = data;
} /* End of function hw_usb_hset_usbleng */

/******************************************************************************
 Function Name   : hw_usb_hwrite_dcpctr
 Description     : Write the specified data value to the DCPCTR register.
 Arguments       : uint16_t  data   : Setting value
 Return value    : none
 ******************************************************************************/
void hw_usb_hwrite_dcpctr (uint16_t data)
{
    USB0.DCPCTR.WORD = data;
} /* End of function hw_usb_hwrite_dcpctr */

/******************************************************************************
 Function Name   : hw_usb_hset_sureq
 Description     : Set te SUREQ-bit in the DCPCTR register 
                 : (Set SETUP packet send when HostController function is selected)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_sureq (void)
{
    USB0.DCPCTR.WORD |= USB_SUREQ;
} /* End of function hw_usb_hset_sureq */

/******************************************************************************
 Function Name   : hw_usb_hset_dcpmode
 Description     : Set DCPMODE bit.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_dcpmode (void)
{
    USB0.USBBCCTRL0.WORD |= USB_DCPMODE;
} /* End of function hw_usb_hset_dcpmode */


/******************************************************************************
 Function Name   : hw_usb_hmodule_init
 Description     : USB module initialization for USB Host mode
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hmodule_init (void)
{
    uint16_t sts;

    USB0.SYSCFG.WORD |= USB_SCKE;
    /* WAIT_LOOP */
    while ( USB_SCKE != (USB0.SYSCFG.WORD & USB_SCKE))
    {
        /* none */
    }

    USB0.SYSCFG.WORD |= USB_DCFM;

    USB0.SYSCFG.WORD |= USB_DRPD;

    sts = usb_chattering((uint16_t *) &USB0.SYSSTS0.WORD);

    USB0.SYSCFG.WORD |= USB_USBE;

    USB0.CFIFOSEL.WORD  = USB_MBW_16;
    USB0.D0FIFOSEL.WORD = USB_MBW_16;
    USB0.D1FIFOSEL.WORD = USB_MBW_16;

#if USB_CFG_ENDIAN == USB_CFG_BIG
    USB0.CFIFOSEL.WORD |= USB_BIGEND;
    USB0.D0FIFOSEL.WORD |= USB_BIGEND;
    USB0.D1FIFOSEL.WORD |= USB_BIGEND;

#endif  /* USB_CFG_ENDIAN == USB_CFG_BIG */

    switch (sts)
    {
        case USB_FS_JSTS : /* USB device already connected */
        case USB_LS_JSTS :
            USB0.DVSTCTR0.WORD |= USB_USBRST;
            usb_cpu_delay_xms((uint16_t) 50); /* Need to wait greater equal 10ms in USB spec */
            USB0.DVSTCTR0.WORD &= (~USB_USBRST);
            /* WAIT_LOOP */
            while ( USB_HSPROC == (USB0.DVSTCTR0.WORD & USB_HSPROC))
            {
                /* HSPROC = 0100b */
            }

            if ( USB_LSMODE == (USB0.DVSTCTR0.WORD & USB_RHST))
            {
                USB0.SOFCFG.WORD |= USB_TRNENSEL;
            }

            USB0.DVSTCTR0.WORD |= USB_UACT;
        break;

        case USB_SE0 : /* USB device no connected */
            USB0.INTENB1.WORD = USB_ATTCH;
        break;

        default :
        break;
    }
    USB0.INTSTS1.WORD &= ((~USB_OVRCRE) & USB_INTSTS1_MASK);
    USB0.INTENB0.WORD = ((USB_BEMPE | USB_NRDYE) | USB_BRDYE);
    USB0.INTENB1.WORD = (USB_OVRCRE | USB_ATTCH);
} /* End of function hw_usb_hmodule_init */


/******************************************************************************
 Function Name   : hw_usb_hset_trnensel
 Description     : When host, set the TRNENSEL-bit; "Transac-
                 : tion-Enabled Time Select" for low-speed USB communication.
                 : This bit should be set to 0 if USB Function.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hset_trnensel (void)
{
    USB0.SOFCFG.WORD |= USB_TRNENSEL;
} /* End of function hw_usb_hset_trnensel */

/******************************************************************************
 Function Name   : hw_usb_hclear_trnensel
 Description     : When host, set the TRNENSEL-bit; "Transac-
                 : tion-Enabled Time Select" for low-speed USB communication.
                 : This bit should be set to 0 if USB Function.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void hw_usb_hclear_trnensel (void)
{
    USB0.SOFCFG.WORD &= (~USB_TRNENSEL);
} /* End of function hw_usb_hclear_trnensel */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 End of file
 ******************************************************************************/
