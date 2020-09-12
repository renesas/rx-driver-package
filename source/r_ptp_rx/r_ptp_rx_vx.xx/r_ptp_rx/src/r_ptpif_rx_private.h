/**********************************************************************************************************************
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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_ptpif_rx_private.h
* Description  : PTP (PTPEDMAC part) for the RX64M/RX71M
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.07.2014 1.00    FIT spec version
*         : 20.08.2014 1.01    State transition and worst10 setting added
*         : 31.12.2014 1.02    Supported RX71M device and added BMC function
*                              Changed module name (eliminate "_api")
*         : 31.03.2016 1.10    Data structures changed
*         : 11.11.2016 1.12    Corrected the internal operation of getting version functions
*                              Corrected wait operation to information retention of GETINFOR
*                              Corrected the array index deviation of ptp_tmr_isr.
*         : 31.03.2017 1.13    Corrected BC and P2P TC setting.
*                              Modified listening state operations of R_PTP_Init, R_PTP_Start and R_PTP_Stop functions.
*                              Added registering user function to MINT interrupt handler.
*                              Changed MINT interrupt handler operation.
*                              Changed TC&OC combined opeartion to TC only operation.
*                              Added my clockIdentity setting function.
*                              Added domainnumber field updating function.
*                              Added announce message fields updating function.
*                              Added and modified transmission interval setting function.
*                              Added offsetFromMaster updating wait function when device state is P2P slave.
*                              Added PTP port state setting function.
*         : 30.04.2017 1.14    Corrected underflow operation of R_PTP_GetSyncInfo function.
*                              Corrected logMessageInterval out of range judgment.
*		  : 31.07.2019 1.15    Added support for GNUC and ICCRX.
*                              Added "WAIT_LOOP" comments for every loop operation.
*		  : 31.08.2019 1.16    Supported RX72M device.
*                              Added Bypass setting.
**********************************************************************************************************************/

#ifndef _PTPIF_RX_PRIVATE_H
#define _PTPIF_RX_PRIVATE_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Get r_bsp info. */
#include <string.h>
#include "platform.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
	#error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Total PTPEDMAC buffer size */ 
#define EMACP_NUM_BUFFERS   (PTPIF_CFG_NUM_RX_DESCRIPTORS + PTPIF_CFG_NUM_TX_DESCRIPTORS)

/* Bit definition of interrupt factor of Ethernet interrupt */
#define EMACP_TYPE_INT      (0xF)
#define EMACP_PVER_INT      (1ul << 4)
#define EMACP_RPORT_INT     (1ul << 7)
#define EMACP_MACE_INT      (1ul << 8)  /* MAC address error */
#define EMACP_RFOF_INT      (1ul << 16) /* Receive buffer overflow error */
#define EMACP_RDE_INT       (1ul << 17) /* Lack of receiving descriptor error */
#define EMACP_FR_INT        (1ul << 18) /* Frame receive complete */
#define EMACP_TFUF_INT      (1ul << 19) /* Transmit buffer overflow error */
#define EMACP_TDE_INT       (1ul << 20) /* Lack of transmiting descriptor error */
#define EMACP_TC_INT        (1ul << 21) /* Frame transmit complete */
#define EMACP_ADE_INT       (1ul << 23) /* Address error */
#define EMACP_RFCOF_INT     (1ul << 24) /* The number of received frame overflow error */
#define EMACP_TABT_INT      (1ul << 26)
#define EMACP_TWB_INT       (1ul << 30)

/* Bit definitions of status member of DescriptorS */
#define  TACT               (0x80000000)
#define  RACT               (0x80000000)
#define  TDLE               (0x40000000)
#define  RDLE               (0x40000000)
#define  TFP1               (0x20000000)
#define  RFP1               (0x20000000)
#define  TFP0               (0x10000000)
#define  RFP0               (0x10000000)
#define  TFE                (0x08000000)
#define  RFE                (0x08000000)
#define  RFOF               (0x00000200)
#define  PORT               (0x00000080)
#define  PVER               (0x00000010)
#define  TYPE3              (0x00000008)
#define  TYPE2              (0x00000004)
#define  TYPE1              (0x00000002)
#define  TYPE0              (0x00000001)

#define  TWBI               (0x04000000)
#define  TFS0_MACE          (0x00000001)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/**
 * EDMACP descriptor as defined in the hardware manual. It is
 * modifed to support little endian CPU mode.
 */
typedef struct DescriptorP
{
    R_BSP_EVENACCESS uint32_t   status;
#if __LIT
/* Little endian */
    R_BSP_EVENACCESS uint16_t   size;
    R_BSP_EVENACCESS uint16_t   bufsize;
#else
/* Big endian */
    R_BSP_EVENACCESS uint16_t   bufsize;
    R_BSP_EVENACCESS uint16_t   size;
#endif
    uint8_t                 *buf_p;
    struct DescriptorP      *next;
} descriptor_p;

/* Ethernet buffer type */
typedef struct EtherBufferP
{
    uint8_t  buffer[EMACP_NUM_BUFFERS][PTPIF_CFG_BUFSIZE];
} etherbuffer_p;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
void ptpif_dev_enab(void);
ptpif_return_t ptpif_dev_start(void);
ptpif_return_t ptpif_dev_stop(void);
void Ptpedmac_isr(void);

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* _PTPIF_RX_PRIVATE_H */
