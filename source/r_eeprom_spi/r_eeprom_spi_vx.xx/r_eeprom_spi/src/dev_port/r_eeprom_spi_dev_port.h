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
* Copyright (C) 2014(2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_eeprom_spi_dev_port.h
* Version      : 3.00
* Description  : EEPROM driver dev port header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 28.11.2014 2.30    Revised functions of same as Ver.2.30 of other middleware.
*              : 30.01.2015 2.31    Added RX71M.
*              : 29.05.2015 2.32     Added RX231 and RX230.
*              : 04.04.2019 3.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*************************************************************************************************/
#ifndef __EEPROM_SPI_DEV_PORT_H__
#define __EEPROM_SPI_DEV_PORT_H__


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#ifdef EEPROM_SPI_CFG_USE_FIT
#include "platform.h"
#else
#include "iodefine.h"
#endif  /* #ifdef EEPROM_SPI_CFG_USE_FIT */


/************************************************************************************************
Macro definitions
*************************************************************************************************/
/*----------- Definitions of port control ------------*/
#define EEPROM_SPI_HI      (uint8_t)(0x01)                 /* Port "H"                             */
#define EEPROM_SPI_LOW     (uint8_t)(0x00)                 /* Port "L"                             */
#define EEPROM_SPI_OUT     (uint8_t)(0x01)                 /* Port output setting                  */
#define EEPROM_SPI_IN      (uint8_t)(0x00)                 /* Port input setting                   */

/* --- Definitions of endian ---- */
#define EEPROM_SPI_LITTLE_ENDIAN  (defined(__LIT) || \
                                defined(__LITTLE) || \
                                defined(__RX_LITTLE_ENDIAN__))

/* ==== EEPROM device 0 ==== */
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
    /* ---- CS ---- */
    #if (('E' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) && \
         ('e' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) && \
         ('1' == EEPROM_SPI_CS_DEV0_CFG_BITNO))
        #define EEPROM_SPI_DEV0_CS_ODRREGNO            0
        #define EEPROM_SPI_DEV0_CS_ODRBITNO            3
    #else
        #if ('4' > EEPROM_SPI_CS_DEV0_CFG_BITNO)
            #define EEPROM_SPI_DEV0_CS_ODRREGNO        0
        #else
            #define EEPROM_SPI_DEV0_CS_ODRREGNO        1
        #endif
        
        #if   (('0' == EEPROM_SPI_CS_DEV0_CFG_BITNO) || ('4' == EEPROM_SPI_CS_DEV0_CFG_BITNO))
            #define EEPROM_SPI_DEV0_CS_ODRBITNO        0
        #elif (('1' == EEPROM_SPI_CS_DEV0_CFG_BITNO) || ('5' == EEPROM_SPI_CS_DEV0_CFG_BITNO))
            #define EEPROM_SPI_DEV0_CS_ODRBITNO        2
        #elif (('2' == EEPROM_SPI_CS_DEV0_CFG_BITNO) || ('6' == EEPROM_SPI_CS_DEV0_CFG_BITNO))
            #define EEPROM_SPI_DEV0_CS_ODRBITNO        4
        #elif (('3' == EEPROM_SPI_CS_DEV0_CFG_BITNO) || ('7' == EEPROM_SPI_CS_DEV0_CFG_BITNO))
            #define EEPROM_SPI_DEV0_CS_ODRBITNO        6
        #endif
    #endif

    #if   ('0' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  0
    #elif ('1' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  1
    #elif ('2' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  2
    #elif ('3' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  3
    #elif ('4' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  4
    #elif ('5' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  5
    #elif ('6' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  6
    #elif ('7' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  7
    #elif ('8' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  8
    #elif ('9' == EEPROM_SPI_CS_DEV0_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR  9
    #elif (('A' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) || ('a' == EEPROM_SPI_CS_DEV0_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR     A
    #elif (('B' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) || ('b' == EEPROM_SPI_CS_DEV0_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR     B
    #elif (('C' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) || ('c' == EEPROM_SPI_CS_DEV0_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR     C
    #elif (('D' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) || ('d' == EEPROM_SPI_CS_DEV0_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR     D
    #elif (('E' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) || ('e' == EEPROM_SPI_CS_DEV0_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR     E
    #elif (('F' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) || ('f' == EEPROM_SPI_CS_DEV0_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR     F
    #elif (('G' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) || ('g' == EEPROM_SPI_CS_DEV0_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR     G
    #elif (('J' == EEPROM_SPI_CS_DEV0_CFG_PORTNO) || ('j' == EEPROM_SPI_CS_DEV0_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR     J
    #endif

    #if   ('0' == EEPROM_SPI_CS_DEV0_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR   0
    #elif ('1' == EEPROM_SPI_CS_DEV0_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR   1
    #elif ('2' == EEPROM_SPI_CS_DEV0_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR   2
    #elif ('3' == EEPROM_SPI_CS_DEV0_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR   3
    #elif ('4' == EEPROM_SPI_CS_DEV0_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR   4
    #elif ('5' == EEPROM_SPI_CS_DEV0_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR   5
    #elif ('6' == EEPROM_SPI_CS_DEV0_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR   6
    #elif ('7' == EEPROM_SPI_CS_DEV0_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR   7
    #endif
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */


/* ==== EEPROM device 1 ==== */
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
    /* ---- CS ---- */
    #if (('E' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) && \
         ('e' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) && \
         ('1' == EEPROM_SPI_CS_DEV1_CFG_BITNO))
        #define EEPROM_SPI_DEV1_CS_ODRREGNO            0
        #define EEPROM_SPI_DEV1_CS_ODRBITNO            3
    #else
        #if ('4' > EEPROM_SPI_CS_DEV1_CFG_BITNO)
            #define EEPROM_SPI_DEV1_CS_ODRREGNO        0
        #else
            #define EEPROM_SPI_DEV1_CS_ODRREGNO        1
        #endif
        
        #if   (('0' == EEPROM_SPI_CS_DEV1_CFG_BITNO) || ('4' == EEPROM_SPI_CS_DEV1_CFG_BITNO))
            #define EEPROM_SPI_DEV1_CS_ODRBITNO        0
        #elif (('1' == EEPROM_SPI_CS_DEV1_CFG_BITNO) || ('5' == EEPROM_SPI_CS_DEV1_CFG_BITNO))
            #define EEPROM_SPI_DEV1_CS_ODRBITNO        2
        #elif (('2' == EEPROM_SPI_CS_DEV1_CFG_BITNO) || ('6' == EEPROM_SPI_CS_DEV1_CFG_BITNO))
            #define EEPROM_SPI_DEV1_CS_ODRBITNO        4
        #elif (('3' == EEPROM_SPI_CS_DEV1_CFG_BITNO) || ('7' == EEPROM_SPI_CS_DEV1_CFG_BITNO))
            #define EEPROM_SPI_DEV1_CS_ODRBITNO        6
        #endif
    #endif

    #if   ('0' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  0
    #elif ('1' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  1
    #elif ('2' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  2
    #elif ('3' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  3
    #elif ('4' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  4
    #elif ('5' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  5
    #elif ('6' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  6
    #elif ('7' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  7
    #elif ('8' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  8
    #elif ('9' == EEPROM_SPI_CS_DEV1_CFG_PORTNO)
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR  9
    #elif (('A' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) || ('a' == EEPROM_SPI_CS_DEV1_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR     A
    #elif (('B' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) || ('b' == EEPROM_SPI_CS_DEV1_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR     B
    #elif (('C' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) || ('c' == EEPROM_SPI_CS_DEV1_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR     C
    #elif (('D' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) || ('d' == EEPROM_SPI_CS_DEV1_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR     D
    #elif (('E' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) || ('e' == EEPROM_SPI_CS_DEV1_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR     E
    #elif (('F' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) || ('f' == EEPROM_SPI_CS_DEV1_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR     F
    #elif (('G' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) || ('g' == EEPROM_SPI_CS_DEV1_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR     G
    #elif (('J' == EEPROM_SPI_CS_DEV1_CFG_PORTNO) || ('j' == EEPROM_SPI_CS_DEV1_CFG_PORTNO))
        #define EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR     J
    #endif

    #if   ('0' == EEPROM_SPI_CS_DEV1_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR   0
    #elif ('1' == EEPROM_SPI_CS_DEV1_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR   1
    #elif ('2' == EEPROM_SPI_CS_DEV1_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR   2
    #elif ('3' == EEPROM_SPI_CS_DEV1_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR   3
    #elif ('4' == EEPROM_SPI_CS_DEV1_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR   4
    #elif ('5' == EEPROM_SPI_CS_DEV1_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR   5
    #elif ('6' == EEPROM_SPI_CS_DEV1_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR   6
    #elif ('7' == EEPROM_SPI_CS_DEV1_CFG_BITNO)
        #define EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR   7
    #endif
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */


/* **** Definitions of string conversion to access I/O registers **** */
#define CAT_2(a,b) a##b                                         /* String "ab"                  */
#define CAT(a,b) CAT_2(a,b)                                     /* Refer to CAT_2(a,b)          */

#ifdef EEPROM_SPI_CFG_USE_GPIO_MPC_FIT
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
#endif  /* #ifdef EEPROM_SPI_CFG_USE_GPIO_MPC_FIT */


/* **** Definitions of I/O registers of ports used for CS **** */
/* ==== EEPROM device 0 ==== */
/* ---- Registers of the port used as CS ---- */
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
 #ifdef EEPROM_SPI_CFG_USE_GPIO_MPC_FIT
  #define EEPROM_SPI_DEV0_GPIO_CS   SFR2_GPIO( EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR )
 #else
  #define EEPROM_SPI_DEV0_PDR_CS    SFR2_PDR(  EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR )
  #define EEPROM_SPI_DEV0_PODR_CS   SFR2_PODR( EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR )
  #define EEPROM_SPI_DEV0_PMR_CS    SFR2_PMR(  EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR )
  #define EEPROM_SPI_DEV0_MPC_CS    SFR2_MPC(  EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV0_CFG_BITNO_SFR )
  #define EEPROM_SPI_DEV0_ODR_CS    SFR2_ODR(  EEPROM_SPI_CS_DEV0_CFG_PORTNO_SFR , EEPROM_SPI_DEV0_CS_ODRREGNO , \
                                            EEPROM_SPI_DEV0_CS_ODRBITNO )
 #endif  /* #ifdef EEPROM_SPI_CFG_USE_GPIO_MPC_FIT */
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */

/* ==== EEPROM device 1 ==== */
/* ---- Registers of the port used as CS ---- */
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
 #ifdef EEPROM_SPI_CFG_USE_GPIO_MPC_FIT
  #define EEPROM_SPI_DEV1_GPIO_CS   SFR2_GPIO( EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR )
 #else
  #define EEPROM_SPI_DEV1_PDR_CS    SFR2_PDR(  EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR )
  #define EEPROM_SPI_DEV1_PODR_CS   SFR2_PODR( EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR )
  #define EEPROM_SPI_DEV1_PMR_CS    SFR2_PMR(  EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR )
  #define EEPROM_SPI_DEV1_MPC_CS    SFR2_MPC(  EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR , EEPROM_SPI_CS_DEV1_CFG_BITNO_SFR )
  #define EEPROM_SPI_DEV1_ODR_CS    SFR2_ODR(  EEPROM_SPI_CS_DEV1_CFG_PORTNO_SFR , EEPROM_SPI_DEV1_CS_ODRREGNO , \
                                           EEPROM_SPI_DEV1_CS_ODRBITNO )
 #endif  /* #ifdef EEPROM_SPI_CFG_USE_GPIO_MPC_FIT */
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */

/* ---- Registers and bits of write protection ---- */
#if !defined(EEPROM_SPI_CFG_USE_GPIO_MPC_FIT)
 #define EEPROM_SPI_PWPR_BOWI      MPC.PWPR.BIT.B0WI           /* PFSWE Bit Write Disable (PWPR)   */
 #define EEPROM_SPI_PWPR_PFSWE     MPC.PWPR.BIT.PFSWE          /* PFS Register Write Enable (PWPR) */
#endif  /* #if !defined(EEPROM_SPI_CFG_USE_GPIO_MPC_FIT) */


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


#endif /* __EEPROM_SPI_DEV_PORT_H__ */

/* End of File */
