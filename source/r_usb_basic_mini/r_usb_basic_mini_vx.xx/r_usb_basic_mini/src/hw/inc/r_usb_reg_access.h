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
 ******************************************************************************/
/*******************************************************************************
 * File Name    : r_usb_reg_access.h
 * Description  : USB IP Register control code
 ******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 ******************************************************************************/


/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#ifndef HW_USB_REG_ACCESS_H
#define HW_USB_REG_ACCESS_H

/******************************************************************************
 Macro definitions
 ******************************************************************************/

#define USB_BUFSIZE_BIT   (10u)
#define USB_SUSPEND_MODE  (1u)
#define USB_NORMAL_MODE   (0)

#define USB0_CFIFO8     (USB0.CFIFO.BYTE.L)
#define USB0_D0FIFO8    (USB0.D0FIFO.BYTE.L)
#define USB0_D1FIFO8    (USB0.D1FIFO.BYTE.L)
#define USB0_CFIFO16    (USB0.CFIFO.WORD)
#define USB0_D0FIFO16   (USB0.D0FIFO.WORD)
#define USB0_D1FIFO16   (USB0.D1FIFO.WORD)

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
/****************/
/*  INITIARIZE  */
/****************/
void         hw_usb_hmodule_init(void);
void         hw_usb_pmodule_init( void );


/************/
/*  SYSCFG  */
/************/
uint16_t hw_usb_read_syscfg (void);
void hw_usb_write_syscfg (uint16_t data);
void hw_usb_set_dcfm (void);
void hw_usb_clear_dcfm (void);
void hw_usb_clear_drpd (void);
void hw_usb_set_usbe (void);
void hw_usb_clear_usbe (void);
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
void hw_usb_pset_dprpu (void);
void hw_usb_pclear_dprpu (void);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */


/************/
/*  SYSSTS0 */
/************/
uint16_t hw_usb_read_syssts (void);

/**************/
/*  DVSTCTR0  */
/**************/
uint16_t hw_usb_read_dvstctr (void);
void hw_usb_write_dvstctr (uint16_t data);
void hw_usb_rmw_dvstctr (uint16_t data, uint16_t width);
void hw_usb_clear_dvstctr (uint16_t data);
void hw_usb_set_vbout (void);
void hw_usb_clear_vbout (void);
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
void hw_usb_hset_rwupe (void);
void hw_usb_hclear_rwupe (void);
void hw_usb_hset_resume (void);
void hw_usb_hclear_resume (void);
void hw_usb_hset_uact (void);
void hw_usb_hclear_uact (void);

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
void hw_usb_pset_wkup (void);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/***************************/
/*  CFIFO, D0FIFO, D1FIFO  */
/***************************/
uint16_t hw_usb_read_fifo16 (uint16_t pipemode);
void hw_usb_write_fifo16 (uint16_t pipemode, uint16_t data);
void hw_usb_write_fifo8 (uint16_t pipemode, uint8_t data);

/************************************/
/*  CFIFOSEL, D0FIFOSEL, D1FIFOSEL  */
/************************************/
uint16_t hw_usb_read_fifosel (uint16_t pipemode);
void hw_usb_rmw_fifosel (uint16_t pipemode, uint16_t data, uint16_t width);
void hw_usb_set_dclrm (uint16_t pipemode);
void hw_usb_clear_dclrm (uint16_t pipemode);
void hw_usb_set_dreqe (uint16_t pipemode);
void hw_usb_clear_dreqe (uint16_t pipemode);
void hw_usb_set_mbw (uint16_t pipemode, uint16_t data);
void hw_usb_set_curpipe (uint16_t pipemode, uint16_t pipeno);

/**********************************/
/* CFIFOCTR, D0FIFOCTR, D1FIFOCTR */
/**********************************/
uint16_t hw_usb_read_fifoctr (uint16_t pipemode);
void hw_usb_set_bval (uint16_t pipemode);
void hw_usb_set_bclr (uint16_t pipemode);

/*************/
/*  INTENB0  */
/*************/
void hw_usb_write_intenb (uint16_t data);
void hw_usb_set_intenb (uint16_t data);
void hw_usb_clear_enb_vbse (void);
void hw_usb_clear_enb_sofe (void);
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
void hw_usb_pset_enb_rsme (void);
void hw_usb_pclear_enb_rsme (void);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/*************/
/*  BRDYENB  */
/*************/
void hw_usb_write_brdyenb (uint16_t data);
void hw_usb_set_brdyenb (uint16_t pipeno);
void hw_usb_clear_brdyenb (uint16_t pipeno);

/*************/
/*  NRDYENB  */
/*************/
void hw_usb_write_nrdyenb (uint16_t data);
void hw_usb_set_nrdyenb (uint16_t pipeno);
void hw_usb_clear_nrdyenb (uint16_t pipeno);

/*************/
/*  BEMPENB  */
/*************/
void hw_usb_write_bempenb (uint16_t data);
void hw_usb_set_bempenb (uint16_t pipeno);
void hw_usb_clear_bempenb (uint16_t pipeno);

/*************/
/*  SOFCFG   */
/*************/
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
void hw_usb_hset_trnensel (void);
void hw_usb_hclear_trnensel (void);

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/*************/
/*  INTSTS0  */
/*************/
void hw_usb_write_intsts (uint16_t data);
void hw_usb_clear_sts_sofr (void);
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
uint16_t hw_usb_read_intsts (void);
void hw_usb_pclear_sts_resm (void);
void hw_usb_pclear_sts_valid (void);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/************/
/* BRDYSTS  */
/************/
uint16_t hw_usb_read_brdysts (void);
void hw_usb_write_brdysts (uint16_t data);
void hw_usb_clear_sts_brdy (uint16_t pipeno);

/************/
/* NRDYSTS  */
/************/
void hw_usb_write_nrdy_sts (uint16_t data);
void hw_usb_clear_status_nrdy (uint16_t pipeno);

/************/
/* BEMPSTS  */
/************/
void hw_usb_write_bempsts (uint16_t data);
void hw_usb_clear_status_bemp (uint16_t pipeno);

/************/
/* FRMNUM   */
/************/
uint16_t hw_usb_read_frmnum (void);

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/************/
/* USBREQ   */
/************/
void hw_usb_hwrite_usbreq (uint16_t data);

/************/
/* USBVAL   */
/************/
void hw_usb_hset_usbval (uint16_t data);

/************/
/* USBINDX  */
/************/
void hw_usb_hset_usbindx (uint16_t data);

/************/
/* USBLENG  */
/************/
void hw_usb_hset_usbleng (uint16_t data);

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
/************/
/* USBREQ   */
/************/
uint16_t hw_usb_read_usbreq (void);

/************/
/* USBVAL   */
/************/
uint16_t hw_usb_read_usbval (void);

/************/
/* USBINDX  */
/************/
uint16_t hw_usb_read_usbindx (void);

/************/
/* USBLENG  */
/************/
uint16_t hw_usb_read_usbleng (void);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/************/
/* DCPCFG   */
/************/
uint16_t hw_usb_read_dcpcfg (void);
void hw_usb_write_dcpcfg (uint16_t data);

/************/
/* DCPMAXP  */
/************/
uint16_t hw_usb_read_dcpmaxp (void);
void hw_usb_write_dcpmxps (uint16_t data);

/************/
/* DCPCTR   */
/************/
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
void hw_usb_hwrite_dcpctr (uint16_t data);
void hw_usb_hset_sureq (void);

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
uint16_t hw_usb_read_dcpctr (void);
void hw_usb_pset_ccpl (void);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/************/
/* PIPESEL  */
/************/
void hw_usb_write_pipesel (uint16_t data);

/************/
/* PIPECFG  */
/************/
uint16_t hw_usb_read_pipecfg (void);
void hw_usb_write_pipecfg (uint16_t data);

/************/
/* PIPEMAXP */
/************/
uint16_t hw_usb_read_pipemaxp (void);
void hw_usb_write_pipemaxp (uint16_t data);

/************/
/* PIPEPERI */
/************/
void hw_usb_write_pipeperi (uint16_t data);

/********************/
/* DCPCTR, PIPEnCTR */
/********************/
uint16_t hw_usb_read_pipectr (uint16_t pipeno);
void hw_usb_write_pipectr (uint16_t pipeno, uint16_t data);
void hw_usb_set_aclrm (uint16_t pipeno);
void hw_usb_clear_aclrm (uint16_t pipeno);
void hw_usb_set_sqclr (uint16_t pipeno);
void hw_usb_set_sqset (uint16_t pipeno);
void hw_usb_set_pid (uint16_t pipeno, uint16_t data);
void hw_usb_clear_pid (uint16_t pipeno, uint16_t data);

/************/
/* PIPEnTRE */
/************/
void hw_usb_set_trenb (uint16_t pipeno);
void hw_usb_clear_trenb (uint16_t pipeno);
void hw_usb_set_trclr (uint16_t pipeno);

/************/
/* PIPEnTRN */
/************/
void hw_usb_write_pipetrn (uint16_t pipeno, uint16_t data);

/************/
/* BCCTRL   */
/************/
void hw_usb_set_bcctrl (uint16_t data);
void hw_usb_clear_bcctrl (uint16_t data);
uint16_t hw_usb_read_bcctrl (void);
void hw_usb_set_vdmsrce (void);
void hw_usb_clear_vdmsrce (void);
void hw_usb_set_idpsinke (void);
void hw_usb_clear_idpsinke (void);
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
void hw_usb_hset_dcpmode (void);
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

void hw_usb_set_vdcen(void);
void hw_usb_set_vddusbe(void);
void hw_usb_set_batchge(void);
void hw_usb_clear_batchge(void);
void hw_usb_set_cnen(void);
void hw_usb_clear_cnen(void);

/**********************************/
/*  DMA0CFG, DMA1CFG  for 597ASSP */
/**********************************/

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/*************/
/*  INTENB1  */
/*************/
void hw_usb_hwrite_intenb (uint16_t data);
void hw_usb_hset_enb_ovrcre (void);
void hw_usb_hclear_enb_ovrcre (void);
void hw_usb_hset_enb_bchge (void);
void hw_usb_hclear_enb_bchge (void);
void hw_usb_hset_enb_dtche (void);
void hw_usb_hclear_enb_dtche (void);
void hw_usb_hset_enb_attche (void);
void hw_usb_hclear_enb_attche (void);
void hw_usb_hset_enb_signe (void);
void hw_usb_hset_enb_sacke (void);
void hw_usb_hset_enb_pddetinte (void);

/*************/
/*  INTSTS1  */
/*************/
void hw_usb_hwrite_intsts (uint16_t data);
void hw_usb_hclear_sts_ovrcr (void);
void hw_usb_hclear_sts_bchg (void);
void hw_usb_hclear_sts_dtch (void);
void hw_usb_hclear_sts_attch (void);
void hw_usb_hclear_sts_sign (void);
void hw_usb_hclear_sts_sack (void);
void hw_usb_hclear_sts_pddetint (void);

/************/
/* DEVADDn  */
/************/

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

#endif /* HW_USB_REG_ACCESS_H */
/******************************************************************************
 End of file
 ******************************************************************************/
