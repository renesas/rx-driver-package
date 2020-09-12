/************************************************************************************************
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
* Copyright (C) 2011(2012-2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_flash_spi_dev_port.h
* Version      : 2.32
* Description  : FLASH SPI driver device port header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 23.07.2014 2.21     Created
*              : 29.05.2015 2.32     Revised functions of same as Ver.2.32 of EEPROM SPI FIT module.
*              : 04.04.2019 3.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*************************************************************************************************/
#ifndef __FLASH_SPI_DEV_PORT_H__
#define __FLASH_SPI_DEV_PORT_H__


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#ifdef FLASH_SPI_CFG_USE_FIT
#include "platform.h"
#else
#include "iodefine.h"
#endif  /* #ifdef FLASH_SPI_CFG_USE_FIT */


/************************************************************************************************
Macro definitions
*************************************************************************************************/
/*----------- Definitions of port control ------------*/
#define FLASH_SPI_HI      (uint8_t)(0x01)                 /* Port "H"                             */
#define FLASH_SPI_LOW     (uint8_t)(0x00)                 /* Port "L"                             */
#define FLASH_SPI_OUT     (uint8_t)(0x01)                 /* Port output setting                  */
#define FLASH_SPI_IN      (uint8_t)(0x00)                 /* Port input setting                   */

/* --- Definitions of endian ---- */
#define FLASH_SPI_LITTLE_ENDIAN  (defined(__LIT) || \
                                defined(__LITTLE) || \
                                defined(__RX_LITTLE_ENDIAN__))

/* ==== FLASH device 0 ==== */
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
    /* ---- CS ---- */
    #if (('E' == FLASH_SPI_CS_DEV0_CFG_PORTNO) && \
         ('e' == FLASH_SPI_CS_DEV0_CFG_PORTNO) && \
         ('1' == FLASH_SPI_CS_DEV0_CFG_BITNO))
        #define FLASH_SPI_DEV0_CS_ODRREGNO            0
        #define FLASH_SPI_DEV0_CS_ODRBITNO            3
    #else
        #if ('4' > FLASH_SPI_CS_DEV0_CFG_BITNO)
            #define FLASH_SPI_DEV0_CS_ODRREGNO        0
        #else
            #define FLASH_SPI_DEV0_CS_ODRREGNO        1
        #endif
        
        #if   (('0' == FLASH_SPI_CS_DEV0_CFG_BITNO) || ('4' == FLASH_SPI_CS_DEV0_CFG_BITNO))
            #define FLASH_SPI_DEV0_CS_ODRBITNO        0
        #elif (('1' == FLASH_SPI_CS_DEV0_CFG_BITNO) || ('5' == FLASH_SPI_CS_DEV0_CFG_BITNO))
            #define FLASH_SPI_DEV0_CS_ODRBITNO        2
        #elif (('2' == FLASH_SPI_CS_DEV0_CFG_BITNO) || ('6' == FLASH_SPI_CS_DEV0_CFG_BITNO))
            #define FLASH_SPI_DEV0_CS_ODRBITNO        4
        #elif (('3' == FLASH_SPI_CS_DEV0_CFG_BITNO) || ('7' == FLASH_SPI_CS_DEV0_CFG_BITNO))
            #define FLASH_SPI_DEV0_CS_ODRBITNO        6
        #endif
    #endif

    #if   ('0' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  0
    #elif ('1' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  1
    #elif ('2' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  2
    #elif ('3' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  3
    #elif ('4' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  4
    #elif ('5' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  5
    #elif ('6' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  6
    #elif ('7' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  7
    #elif ('8' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  8
    #elif ('9' == FLASH_SPI_CS_DEV0_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR  9
    #elif (('A' == FLASH_SPI_CS_DEV0_CFG_PORTNO) || ('a' == FLASH_SPI_CS_DEV0_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR     A
    #elif (('B' == FLASH_SPI_CS_DEV0_CFG_PORTNO) || ('b' == FLASH_SPI_CS_DEV0_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR     B
    #elif (('C' == FLASH_SPI_CS_DEV0_CFG_PORTNO) || ('c' == FLASH_SPI_CS_DEV0_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR     C
    #elif (('D' == FLASH_SPI_CS_DEV0_CFG_PORTNO) || ('d' == FLASH_SPI_CS_DEV0_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR     D
    #elif (('E' == FLASH_SPI_CS_DEV0_CFG_PORTNO) || ('e' == FLASH_SPI_CS_DEV0_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR     E
    #elif (('F' == FLASH_SPI_CS_DEV0_CFG_PORTNO) || ('f' == FLASH_SPI_CS_DEV0_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR     F
    #elif (('G' == FLASH_SPI_CS_DEV0_CFG_PORTNO) || ('g' == FLASH_SPI_CS_DEV0_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR     G
    #elif (('J' == FLASH_SPI_CS_DEV0_CFG_PORTNO) || ('j' == FLASH_SPI_CS_DEV0_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR     J
    #endif

    #if   ('0' == FLASH_SPI_CS_DEV0_CFG_BITNO)
        #define FLASH_SPI_CS_DEV0_CFG_BITNO_SFR   0
    #elif ('1' == FLASH_SPI_CS_DEV0_CFG_BITNO)
        #define FLASH_SPI_CS_DEV0_CFG_BITNO_SFR   1
    #elif ('2' == FLASH_SPI_CS_DEV0_CFG_BITNO)
        #define FLASH_SPI_CS_DEV0_CFG_BITNO_SFR   2
    #elif ('3' == FLASH_SPI_CS_DEV0_CFG_BITNO)
        #define FLASH_SPI_CS_DEV0_CFG_BITNO_SFR   3
    #elif ('4' == FLASH_SPI_CS_DEV0_CFG_BITNO)
        #define FLASH_SPI_CS_DEV0_CFG_BITNO_SFR   4
    #elif ('5' == FLASH_SPI_CS_DEV0_CFG_BITNO)
        #define FLASH_SPI_CS_DEV0_CFG_BITNO_SFR   5
    #elif ('6' == FLASH_SPI_CS_DEV0_CFG_BITNO)
        #define FLASH_SPI_CS_DEV0_CFG_BITNO_SFR   6
    #elif ('7' == FLASH_SPI_CS_DEV0_CFG_BITNO)
        #define FLASH_SPI_CS_DEV0_CFG_BITNO_SFR   7
    #endif
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */


/* ==== FLASH device 1 ==== */
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
    /* ---- CS ---- */
    #if (('E' == FLASH_SPI_CS_DEV1_CFG_PORTNO) && \
         ('e' == FLASH_SPI_CS_DEV1_CFG_PORTNO) && \
         ('1' == FLASH_SPI_CS_DEV1_CFG_BITNO))
        #define FLASH_SPI_DEV1_CS_ODRREGNO            0
        #define FLASH_SPI_DEV1_CS_ODRBITNO            3
    #else
        #if ('4' > FLASH_SPI_CS_DEV1_CFG_BITNO)
            #define FLASH_SPI_DEV1_CS_ODRREGNO        0
        #else
            #define FLASH_SPI_DEV1_CS_ODRREGNO        1
        #endif
        
        #if   (('0' == FLASH_SPI_CS_DEV1_CFG_BITNO) || ('4' == FLASH_SPI_CS_DEV1_CFG_BITNO))
            #define FLASH_SPI_DEV1_CS_ODRBITNO        0
        #elif (('1' == FLASH_SPI_CS_DEV1_CFG_BITNO) || ('5' == FLASH_SPI_CS_DEV1_CFG_BITNO))
            #define FLASH_SPI_DEV1_CS_ODRBITNO        2
        #elif (('2' == FLASH_SPI_CS_DEV1_CFG_BITNO) || ('6' == FLASH_SPI_CS_DEV1_CFG_BITNO))
            #define FLASH_SPI_DEV1_CS_ODRBITNO        4
        #elif (('3' == FLASH_SPI_CS_DEV1_CFG_BITNO) || ('7' == FLASH_SPI_CS_DEV1_CFG_BITNO))
            #define FLASH_SPI_DEV1_CS_ODRBITNO        6
        #endif
    #endif

    #if   ('0' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  0
    #elif ('1' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  1
    #elif ('2' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  2
    #elif ('3' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  3
    #elif ('4' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  4
    #elif ('5' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  5
    #elif ('6' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  6
    #elif ('7' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  7
    #elif ('8' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  8
    #elif ('9' == FLASH_SPI_CS_DEV1_CFG_PORTNO)
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR  9
    #elif (('A' == FLASH_SPI_CS_DEV1_CFG_PORTNO) || ('a' == FLASH_SPI_CS_DEV1_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR     A
    #elif (('B' == FLASH_SPI_CS_DEV1_CFG_PORTNO) || ('b' == FLASH_SPI_CS_DEV1_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR     B
    #elif (('C' == FLASH_SPI_CS_DEV1_CFG_PORTNO) || ('c' == FLASH_SPI_CS_DEV1_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR     C
    #elif (('D' == FLASH_SPI_CS_DEV1_CFG_PORTNO) || ('d' == FLASH_SPI_CS_DEV1_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR     D
    #elif (('E' == FLASH_SPI_CS_DEV1_CFG_PORTNO) || ('e' == FLASH_SPI_CS_DEV1_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR     E
    #elif (('F' == FLASH_SPI_CS_DEV1_CFG_PORTNO) || ('f' == FLASH_SPI_CS_DEV1_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR     F
    #elif (('G' == FLASH_SPI_CS_DEV1_CFG_PORTNO) || ('g' == FLASH_SPI_CS_DEV1_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR     G
    #elif (('J' == FLASH_SPI_CS_DEV1_CFG_PORTNO) || ('j' == FLASH_SPI_CS_DEV1_CFG_PORTNO))
        #define FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR     J
    #endif

    #if   ('0' == FLASH_SPI_CS_DEV1_CFG_BITNO)
        #define FLASH_SPI_CS_DEV1_CFG_BITNO_SFR   0
    #elif ('1' == FLASH_SPI_CS_DEV1_CFG_BITNO)
        #define FLASH_SPI_CS_DEV1_CFG_BITNO_SFR   1
    #elif ('2' == FLASH_SPI_CS_DEV1_CFG_BITNO)
        #define FLASH_SPI_CS_DEV1_CFG_BITNO_SFR   2
    #elif ('3' == FLASH_SPI_CS_DEV1_CFG_BITNO)
        #define FLASH_SPI_CS_DEV1_CFG_BITNO_SFR   3
    #elif ('4' == FLASH_SPI_CS_DEV1_CFG_BITNO)
        #define FLASH_SPI_CS_DEV1_CFG_BITNO_SFR   4
    #elif ('5' == FLASH_SPI_CS_DEV1_CFG_BITNO)
        #define FLASH_SPI_CS_DEV1_CFG_BITNO_SFR   5
    #elif ('6' == FLASH_SPI_CS_DEV1_CFG_BITNO)
        #define FLASH_SPI_CS_DEV1_CFG_BITNO_SFR   6
    #elif ('7' == FLASH_SPI_CS_DEV1_CFG_BITNO)
        #define FLASH_SPI_CS_DEV1_CFG_BITNO_SFR   7
    #endif
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */


/* **** Definitions of string conversion to access I/O registers **** */
#define CAT_2(a,b) a##b                                         /* String "ab"                  */
#define CAT(a,b) CAT_2(a,b)                                     /* Refer to CAT_2(a,b)          */

#if (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 1)
 #define SFR1_GPIO( x,y )       GPIO_PORT_ ## x ## _PIN_ ## y   /* String "GPIO_PORT_x_PIN_y"   */
 #define SFR2_GPIO( x,y )       SFR1_GPIO( x , y )              /* Refer to __GPIO( x )         */
#else
 #define SFR1_PDR( x,y )        PORT ## x.PDR.BIT.B ## y        /* String "PORTx.PDR.BIT.By"    */
 #define SFR2_PDR( x,y )        SFR1_PDR( x , y )               /* Refer to __PDR( x )          */
 #define SFR1_PODR( x,y )       PORT ## x.PODR.BIT.B ## y       /* String "PORTx.PODR.BIT.By"   */
 #define SFR2_PODR( x,y )       SFR1_PODR( x , y )              /* Refer to __PODR( x )         */
 #define SFR1_PMR( x,y )        PORT ## x.PMR.BIT.B ## y        /* String "PORTx.PMR.BIT.By"    */
 #define SFR2_PMR( x,y )        SFR1_PMR( x , y )               /* Refer to __PMR( x )          */
 #define SFR1_MPC( x,y )        MPC.P ## x ## y ## PFS.BYTE     /* String "MPC.PxyPFS.BYTE"     */
 #define SFR2_MPC( x,y )        SFR1_MPC( x , y )               /* Refer to __MPC( x )          */
 #define SFR1_ODR( x,y,z )      PORT ## x.ODR ## y.BIT.B ## z   /* String "PORTx.ODRy.BIT.Bz"   */
 #define SFR2_ODR( x,y,z )      SFR1_ODR( x , y , z)            /* Refer to __ODR( x )          */
#endif  /* #if (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 1) */


/* **** Definitions of I/O registers of ports used for CS **** */
/* ==== FLASH device 0 ==== */
/* ---- Registers of the port used as CS ---- */
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
 #if (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 1)
  #define FLASH_SPI_DEV0_GPIO_CS   SFR2_GPIO( FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV0_CFG_BITNO_SFR )
 #else
  #define FLASH_SPI_DEV0_PDR_CS    SFR2_PDR(  FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV0_CFG_BITNO_SFR )
  #define FLASH_SPI_DEV0_PODR_CS   SFR2_PODR( FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV0_CFG_BITNO_SFR )
  #define FLASH_SPI_DEV0_PMR_CS    SFR2_PMR(  FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV0_CFG_BITNO_SFR )
  #define FLASH_SPI_DEV0_MPC_CS    SFR2_MPC(  FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV0_CFG_BITNO_SFR )
  #define FLASH_SPI_DEV0_ODR_CS    SFR2_ODR(  FLASH_SPI_CS_DEV0_CFG_PORTNO_SFR , FLASH_SPI_DEV0_CS_ODRREGNO , \
                                            FLASH_SPI_DEV0_CS_ODRBITNO )
 #endif  /* #if (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 1) */
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */

/* ==== FLASH device 1 ==== */
/* ---- Registers of the port used as CS ---- */
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
 #if (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 1)
  #define FLASH_SPI_DEV1_GPIO_CS   SFR2_GPIO( FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV1_CFG_BITNO_SFR )
 #else
  #define FLASH_SPI_DEV1_PDR_CS    SFR2_PDR(  FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV1_CFG_BITNO_SFR )
  #define FLASH_SPI_DEV1_PODR_CS   SFR2_PODR( FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV1_CFG_BITNO_SFR )
  #define FLASH_SPI_DEV1_PMR_CS    SFR2_PMR(  FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV1_CFG_BITNO_SFR )
  #define FLASH_SPI_DEV1_MPC_CS    SFR2_MPC(  FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR , FLASH_SPI_CS_DEV1_CFG_BITNO_SFR )
  #define FLASH_SPI_DEV1_ODR_CS    SFR2_ODR(  FLASH_SPI_CS_DEV1_CFG_PORTNO_SFR , FLASH_SPI_DEV1_CS_ODRREGNO , \
                                           FLASH_SPI_DEV1_CS_ODRBITNO )
 #endif  /* #if (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 1) */
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */

/* ---- Registers and bits of write protection ---- */
#if (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 0)
 #define FLASH_SPI_PWPR_BOWI      MPC.PWPR.BIT.B0WI           /* PFSWE Bit Write Disable (PWPR)   */
 #define FLASH_SPI_PWPR_PFSWE     MPC.PWPR.BIT.PFSWE          /* PFS Register Write Enable (PWPR) */
#endif  /* #if (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 0) */


/************************************************************************************************
Typedef definitions
*************************************************************************************************/


/************************************************************************************************
Exported global variables
*************************************************************************************************/


/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/


/************************************************************************************************/
/*  Prototypes                                                                                  */
/************************************************************************************************/


#endif /* __FLASH_SPI_DEV_PORT_H__ */

/* End of File */
