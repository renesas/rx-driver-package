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
* File Name    : r_usb_bitdefine.h
* Description  : USB-IP define
 ******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 30.11.2018 1.10    Supporting Smart Configurator
******************************************************************************/


/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

#ifndef R_USB_BITDEFINE_H
#define R_USB_BITDEFINE_H
 
/* USB0 Register definition */
/* System Configuration Control Register */
#define USB_SCKE                (0x0400u)   /* b10: USB clock enable */
#define USB_CNEN                (0x0100u)   /* b8: Single end receiver */
#define USB_DCFM                (0x0040u)   /* b6: Function select */
#define USB_DRPD                (0x0020u)   /* b5: D+/D- pull down control */
#define USB_DPRPU               (0x0010u)   /* b4: D+ pull up control */
#define USB_DMRPU               (0x0008u)   /* b3: D- pull up control *//* For low speed */
#define USB_USBE                (0x0001u)   /* b0: USB module enable */

/* System Configuration Status Register */
#define USB_OVCMON              (0xC000u)   /* b15-14: Over-current monitor */
#define USB_OVCBIT              (0x8000u)   /* b15-14: Over-current bit */
#define USB_HTACT               (0x0040u)   /* b6: USB Host Sequencer Status Monitor */
#define USB_IDMON               (0x0004u)   /* b2: ID-pin monitor */
#define USB_LNST                (0x0003u)   /* b1-0: D+, D- line status */
#define USB_SE1                 (0x0003u)   /* SE1 */
#define USB_FS_KSTS             (0x0002u)   /* Full-Speed K State */
#define USB_FS_JSTS             (0x0001u)   /* Full-Speed J State */
#define USB_LS_JSTS             (0x0002u)   /* Low-Speed J State */
#define USB_LS_KSTS             (0x0001u)   /* Low-Speed K State */
#define USB_SE0                 (0x0000u)   /* SE0 */

/* Device State Control Register */
#define USB_HNPBTOA             (0x0800u)   /* b11: Host negotiation protocol (BtoA) */
#define USB_EXICEN              (0x0400u)   /* b10: EXICEN output terminal control */
#define USB_VBUSEN              (0x0200u)   /* b9: VBUS output terminal control */
#define USB_WKUP                (0x0100u)   /* b8: Remote wakeup */
#define USB_RWUPE               (0x0080u)   /* b7: Remote wakeup sense */
#define USB_USBRST              (0x0040u)   /* b6: USB reset enable */
#define USB_RESUME              (0x0020u)   /* b5: Resume enable */
#define USB_UACT                (0x0010u)   /* b4: USB bus enable */
#define USB_RHST                (0x0007u)   /* b2-0: Reset handshake status */
#define USB_HSPROC              (0x0004u)   /* HS handshake processing */
#define USB_HSMODE              (0x0003u)   /* Hi-Speed mode */
#define USB_FSMODE              (0x0002u)   /* Full-Speed mode */
#define USB_LSMODE              (0x0001u)   /* Low-Speed mode */
#define USB_UNDECID             (0x0000u)   /* Undecided */

/* CFIFO/DxFIFO Port Select Register */
#define USB_RCNT                (0x8000u)   /* b15: Read count mode */
#define USB_REW                 (0x4000u)   /* b14: Buffer rewind */
#define USB_DCLRM               (0x2000u)   /* b13: Automatic buffer clear mode */
#define USB_DREQE               (0x1000u)   /* b12: DREQ output enable */
#define USB_MBW                 (0x0400u)   /* b10: Maximum bit width for FIFO access */
#define USB_MBW_16              (0x0400u)   /* FIFO access : 16bit */
#define USB_MBW_8               (0x0000u)   /* FIFO access : 8bit */
#define USB_BIGEND              (0x0100u)   /* b8: Big endian mode */
#define USB_FIFO_BIG            (0x0100u)   /* Big endian */
#define USB_FIFO_LITTLE         (0x0000u)   /* Little endian */
#define USB_ISEL                (0x0020u)   /* b5: DCP FIFO port direction select */
#define USB_CURPIPE             (0x000Fu)   /* b3-0: PIPE select */

/* CFIFO/DxFIFO Port Control Register */
#define USB_BVAL                (0x8000u)   /* b15: Buffer valid flag */
#define USB_BCLR                (0x4000u)   /* b14: Buffer clear */
#define USB_FRDY                (0x2000u)   /* b13: FIFO ready */
#define USB_DTLN                (0x01FFu)   /* b8-0: FIFO data length */


/* Interrupt Enable Register 0 */
#define USB_VBSE                (0x8000u)   /* b15: VBUS interrupt */
#define USB_RSME                (0x4000u)   /* b14: Resume interrupt */
#define USB_SOFE                (0x2000u)   /* b13: Frame update interrupt */
#define USB_DVSE                (0x1000u)   /* b12: Device state transition interrupt */
#define USB_CTRE                (0x0800u)   /* b11: Control transfer stage transition interrupt */
#define USB_BEMPE               (0x0400u)   /* b10: Buffer empty interrupt */
#define USB_NRDYE               (0x0200u)   /* b9: Buffer notready interrupt */
#define USB_BRDYE               (0x0100u)   /* b8: Buffer ready interrupt */

/* Interrupt Enable Register 1 */
#define USB_OVRCRE              (0x8000u)   /* b15: Over-current interrupt */
#define USB_BCHGE               (0x4000u)   /* b14: USB bus change interrupt */
#define USB_DTCHE               (0x1000u)   /* b12: Detach sense interrupt */
#define USB_ATTCHE              (0x0800u)   /* b11: Attach sense interrupt */
#define USB_EOFERRE             (0x0040u)   /* b6: EOF error interrupt */
#define USB_SIGNE               (0x0020u)   /* b5: SETUP IGNORE interrupt */
#define USB_SACKE               (0x0010u)   /* b4: SETUP ACK interrupt */
#define USB_PDDETINTE           (0x0001u)   /* b0: PDDET detection interrupt */

/* BRDY Interrupt Enable/Status Register */
#define USB_BRDY9               (0x0200u)   /* b9: PIPE9 */
#define USB_BRDY8               (0x0100u)   /* b8: PIPE8 */
#define USB_BRDY7               (0x0080u)   /* b7: PIPE7 */
#define USB_BRDY6               (0x0040u)   /* b6: PIPE6 */
#define USB_BRDY5               (0x0020u)   /* b5: PIPE5 */
#define USB_BRDY4               (0x0010u)   /* b4: PIPE4 */
#define USB_BRDY3               (0x0008u)   /* b3: PIPE3 */
#define USB_BRDY2               (0x0004u)   /* b2: PIPE2 */
#define USB_BRDY1               (0x0002u)   /* b1: PIPE1 */
#define USB_BRDY0               (0x0001u)   /* b0: PIPE0 */

/* NRDY Interrupt Enable/Status Register */
#define USB_NRDY9               (0x0200u)   /* b9: PIPE9 */
#define USB_NRDY8               (0x0100u)   /* b8: PIPE8 */
#define USB_NRDY7               (0x0080u)   /* b7: PIPE7 */
#define USB_NRDY6               (0x0040u)   /* b6: PIPE6 */
#define USB_NRDY5               (0x0020u)   /* b5: PIPE5 */
#define USB_NRDY4               (0x0010u)   /* b4: PIPE4 */
#define USB_NRDY3               (0x0008u)   /* b3: PIPE3 */
#define USB_NRDY2               (0x0004u)   /* b2: PIPE2 */
#define USB_NRDY1               (0x0002u)   /* b1: PIPE1 */
#define USB_NRDY0               (0x0001u)   /* b0: PIPE0 */

/* BEMP Interrupt Enable/Status Register */
#define USB_BEMP9               (0x0200u)   /* b9: PIPE9 */
#define USB_BEMP8               (0x0100u)   /* b8: PIPE8 */
#define USB_BEMP7               (0x0080u)   /* b7: PIPE7 */
#define USB_BEMP6               (0x0040u)   /* b6: PIPE6 */
#define USB_BEMP5               (0x0020u)   /* b5: PIPE5 */
#define USB_BEMP4               (0x0010u)   /* b4: PIPE4 */
#define USB_BEMP3               (0x0008u)   /* b3: PIPE3 */
#define USB_BEMP2               (0x0004u)   /* b2: PIPE2 */
#define USB_BEMP1               (0x0002u)   /* b1: PIPE1 */
#define USB_BEMP0               (0x0001u)   /* b0: PIPE0 */

/* SOF Pin Configuration Register */
#define USB_TRNENSEL            (0x0100u)   /* b8: Select transaction enable period */
#define USB_BRDYM               (0x0040u)   /* b6: BRDY clear timing */
#define USB_EDGESTS             (0x0010u)   /* b4:  */

/* Interrupt Status Register 0 */
#define USB_VBINT               (0x8000u)   /* b15: VBUS interrupt */
#define USB_RESM                (0x4000u)   /* b14: Resume interrupt */
#define USB_SOFR                (0x2000u)   /* b13: SOF update interrupt */
#define USB_DVST                (0x1000u)   /* b12: Device state transition interrupt */
#define USB_CTRT                (0x0800u)   /* b11: Control transfer stage transition interrupt */
#define USB_BEMP                (0x0400u)   /* b10: Buffer empty interrupt */
#define USB_NRDY                (0x0200u)   /* b9: Buffer notready interrupt */
#define USB_BRDY                (0x0100u)   /* b8: Buffer ready interrupt */
#define USB_VBSTS               (0x0080u)   /* b7: VBUS input port */
#define USB_DVSQ                (0x0070u)   /* b6-4: Device state */
#define USB_DS_SPD_CNFG         (0x0070u)   /* Suspend Configured */
#define USB_DS_SPD_ADDR         (0x0060u)   /* Suspend Address */
#define USB_DS_SPD_DFLT         (0x0050u)   /* Suspend Default */
#define USB_DS_SPD_POWR         (0x0040u)   /* Suspend Powered */
#define USB_DS_SUSP             (0x0040u)   /* Suspend */
#define USB_DS_CNFG             (0x0030u)   /* Configured */
#define USB_DS_ADDS             (0x0020u)   /* Address */
#define USB_DS_DFLT             (0x0010u)   /* Default */
#define USB_DS_POWR             (0x0000u)   /* Powered */
#define USB_DVSQS               (0x0030u)   /* b5-4: Device state */
#define USB_VALID               (0x0008u)   /* b3: Setup packet detect flag */
#define USB_CTSQ                (0x0007u)   /* b2-0: Control transfer stage */
#define USB_CS_SQER             (0x0006u)   /* Sequence error */
#define USB_CS_WRND             (0x0005u)   /* Ctrl write nodata status stage */
#define USB_CS_WRSS             (0x0004u)   /* Ctrl write status stage */
#define USB_CS_WRDS             (0x0003u)   /* Ctrl write data stage */
#define USB_CS_RDSS             (0x0002u)   /* Ctrl read status stage */
#define USB_CS_RDDS             (0x0001u)   /* Ctrl read data stage */
#define USB_CS_IDST             (0x0000u)   /* Idle or setup stage */

/* Interrupt Status Register 1 */
#define USB_OVRCR               (0x8000u)   /* b15: Over-current interrupt */
#define USB_BCHG                (0x4000u)   /* b14: USB bus change interrupt */
#define USB_DTCH                (0x1000u)   /* b12: Detach sense interrupt */
#define USB_ATTCH               (0x0800u)   /* b11: Attach sense interrupt */
#define USB_EOFERR              (0x0040u)   /* b6: EOF-error interrupt */
#define USB_SIGN                (0x0020u)   /* b5: Setup ignore interrupt */
#define USB_SACK                (0x0010u)   /* b4: Setup ack interrupt */
#define USB_PDDETINT            (0x0001u)   /* b0: PDDET detection interrupt */

/* USB IO Register Reserved bit mask */
#define USB_INTSTS1_MASK        (0xD870u)   /* INTSTS1 Reserved bit mask */
#define USB_BRDYSTS_MASK        (0x03FFu)   /* BRDYSTS Reserved bit mask */
#define USB_NRDYSTS_MASK        (0x03FFu)   /* NRDYSTS Reserved bit mask */
#define USB_BEMPSTS_MASK        (0x03FFu)   /* BEMPSTS Reserved bit mask */

/* Frame Number Register */
#define USB_OVRN                (0x8000u)   /* b15: Overrun error */
#define USB_CRCE                (0x4000u)   /* b14: Received data error */
#define USB_FRNM                (0x07FFu)   /* b10-0: Frame number */

/* USB Request Type Register */
#define USB_BMREQUESTTYPE       (0x00FFu)   /* b7-0: USB_BMREQUESTTYPE */
#define USB_BMREQUESTTYPEDIR    (0x0080u)   /* b7  : Data transfer direction */
#define USB_BMREQUESTTYPETYPE   (0x0060u)   /* b6-5: Type */
#define USB_BMREQUESTTYPERECIP  (0x001Fu)   /* b4-0: Recipient */

/* USB Request Value Register */
#define USB_WVALUE              (0xFFFFu)   /* b15-0: wValue */
#define USB_DT_TYPE             (0xFF00u)
#define USB_GET_DT_TYPE(v)      (((v) & USB_DT_TYPE) >> 8)
#define USB_DT_INDEX            (0x00FFu)
#define USB_CONF_NUM            (0x00FFu)
#define USB_ALT_SET             (0x00FFu)

/* USB Request Index Register */
#define USB_WINDEX              (0xFFFFu)   /* b15-0: wIndex */
#define USB_TEST_SELECT         (0xFF00u)   /* b15-b8: Test Mode Selectors */
#define USB_TEST_J              (0x0100u)   /* Test_J */
#define USB_TEST_K              (0x0200u)   /* Test_K */
#define USB_TEST_SE0_NAK        (0x0300u)   /* Test_SE0_NAK */
#define USB_TEST_PACKET         (0x0400u)   /* Test_Packet */
#define USB_TEST_FORCE_ENABLE   (0x0500u)   /* Test_Force_Enable */
#define USB_TEST_STSelectors    (0x0600u)   /* Standard test selectors */
#define USB_TEST_RESERVED       (0x4000u)   /* Reserved */
#define USB_TEST_VSTMODES       (0xC000u)   /* VendorSpecific test modes */
#define USB_EP_DIR              (0x0080u)   /* b7: Endpoint Direction */
#define USB_EP_DIR_IN           (0x0080u)
#define USB_EP_DIR_OUT          (0x0000u)

/* USB Request Length Register */
#define USB_WLENGTH             (0xFFFFu)   /* b15-0: wLength */

/* Pipe Window Select Register */
#define USB_PIPENM              (0x000Fu)   /* b3-0: Pipe select */

/* Default Control Pipe Configuration Register */
/* Pipe Configuration Register */
#define USB_TYPE                (0xC000u)   /* b15-14: Transfer type */
#define USB_BFRE                (0x0400u)   /* b10: Buffer ready interrupt mode select */
#define USB_DBLB                (0x0200u)   /* b9: Double buffer mode select */
#define USB_SHTNAK              (0x0080u)   /* b7: Transfer end NAK */
#define USB_DIR                 (0x0010u)   /* b4: Transfer direction select */
#define USB_EPNUM               (0x000Fu)   /* b3-0: Endpoint number select */

/* Default Control Pipe Maxpacket Size Register */
/* Pipe Maxpacket Size Register */
#define USB_DEVSEL              (0xF000u)   /* b15-12: Device address select */
#define USB_MAXP                (0x007Fu)   /* b6-0: Maxpacket size of default control pipe */

/* Pipe Cycle Configuration Register */
#define USB_IITV            (0x0007u)   /* b2-0: Isochronous interval */

/* Default Control Pipe Control Register */
#define USB_BSTS                (0x8000u)   /* b15: Buffer status */
#define USB_SUREQ               (0x4000u)   /* b14: Send USB request  */
#define USB_INBUFM              (0x4000u)   /* b14: IN buffer monitor (Only for PIPE1 to 5) */
#define USB_SUREQCLR            (0x0800u)   /* b11: stop setup request */
#define USB_ATREPM              (0x0400u)   /* b10: Auto repeat mode */
#define USB_ACLRM               (0x0200u)   /* b9: buffer auto clear mode */
#define USB_SQCLR               (0x0100u)   /* b8: Sequence bit clear */
#define USB_SQSET               (0x0080u)   /* b7: Sequence bit set */
#define USB_SQMON               (0x0040u)   /* b6: Sequence bit monitor */
#define USB_PBUSY               (0x0020u)   /* b5: pipe busy */
#define USB_CCPL                (0x0004u)   /* b2: Enable control transfer complete */
#define USB_PID                 (0x0003u)   /* b1-0: Response PID */
#define USB_PID_STALL           (0x0002u)   /* STALL */
#define USB_PID_BUF             (0x0001u)   /* BUF */
#define USB_PID_NAK             (0x0000u)   /* NAK */


/* PIPExTRE */
#define USB_TRENB               (0x0200u)   /* b9: Transaction count enable */
#define USB_TRCLR               (0x0100u)   /* b8: Transaction count clear */


/* PIPExTRN */
#define USB_TRNCNT              (0xFFFFu)   /* b15-0: Transaction counter */

/* Battery Charging Control Register */
/* BCCTRL */
#define USB_PDDETSTS            (0x0200u)
#define USB_CHGDETSTS           (0x0100u)
#define USB_BATCHGE             (0x0080u)
#define USB_DCPMODE             (0x0040u)
#define USB_VDMSRCE             (0x0020u)
#define USB_IDPSINKE            (0x0010u)
#define USB_VDPSRCE             (0x0008u)
#define USB_IDMSINKE            (0x0004u)
#define USB_IDPSRCE             (0x0002u)
#define USB_RPDME               (0x0001u)

/* DEVADDx */
#define USB_USBSPD              (0x00C0u)   /* b7-6: Device speed */
#define   USB_NOTUSED           (0x0000u)
#define   USB_FULLSPEED         (0x0080u)       /* Full-Speed connect */
#define   USB_HIGHSPEED         (0x00C0u)       /* Hi-Speed connect */
#define   USB_LOWSPEED          (0x0040u)       /* Low-Speed connect */

/* USB Module Control Register */
/*  USB_USBMC     (*((REGP*)(USB_BASE+0xCC))) */
#define USB_VDDUSBE             (0x0001)
#define USB_VDCEN               (0x0080)



/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

#endif /* R_USB_BITDEFINE_H */
/******************************************************************************
End of file
******************************************************************************/
