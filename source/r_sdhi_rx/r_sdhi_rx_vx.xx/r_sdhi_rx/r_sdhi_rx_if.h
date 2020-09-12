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
* Copyright (C) 2017(2019) Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdhi_rx_if.h
* Device       : RX
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX
* Description  : Interface file for SDHI API for RX
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.07.2017 2.00    First Release
*         : 31.12.2017 2.01    Corresponded to the pin setting of Smart Configurator.
*         :                    There is no change in the FIT module.
*         : 30.11.2018 2.02    Corresponded to FIT guide of Smart Configurator.
*         : 01.02.2019 2.03    Changed Minor version to 2.03.
*         : 20.05.2019 2.04    Added support for GNUC and ICCRX.
*                              Fixed coding style.
*         : 30.07.2019 2.05    Changed Minor version to 2.05.
*         : 22.11.2019 2.06    Changed Minor version to 2.06.
**********************************************************************************************************************/

#ifndef SDHI_RX_H
#define SDHI_RX_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Used to get which MCU is currently being used. */
#include "platform.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* #### SD Memory and common definitions #### */
/* Version Number of API. */
#define RX_SDHI_VERSION_MAJOR       (2)
#define RX_SDHI_VERSION_MINOR       (06)

/* ==== Select use CBSY bit ==== */
/*  #define     USE_INFO2_CBSY      */ /* Use SDHI_SDIMSK2_SCLKDIVEN   */

#define SDHI_CH0                    (0)
#define SDHI_CH1                    (1)

/* ---- SD clock div ---- */
#define SDHI_DIV_512                (0x0080u)   /* PCLK/512 */
#define SDHI_DIV_256                (0x0040u)   /* PCLK/256 */
#define SDHI_DIV_128                (0x0020u)   /* PCLK/128 */
#define SDHI_DIV_64                 (0x0010u)   /* PCLK/64 */
#define SDHI_DIV_32                 (0x0008u)   /* PCLK/32 */
#define SDHI_DIV_16                 (0x0004u)   /* PCLK/16 */
#define SDHI_DIV_8                  (0x0002u)   /* PCLK/8 */
#define SDHI_DIV_4                  (0x0001u)   /* PCLK/4 */
#define SDHI_DIV_2                  (0x0000u)   /* PCLK/2 */
#define SDHI_DIV_1                  (0x00FFu)   /* PCLK/1 */

/* ---- SD port mode ---- */
#define SDHI_PORT_1BIT              ((int32_t)0x0000)   /* 1bit mode */
#define SDHI_PORT_4BIT              ((int32_t)0x0001)   /* 4bits mode */
#define SDHI_PORT_CD                ((int32_t)0x0002)   /* CD mode */

#define SDHI_ADDR_BOUNDARY          (0x00000003ul)

#define SDHI_HWINT_ACCESS_CD        (0x00000001ul)
#define SDHI_HWINT_BUFFER           (0x00000010ul)
#define SDHI_HWINT_PARAM            (0xffffffeeul)

/* ---- SD clock control ---- */
#define SDHI_CLOCK_ENABLE           (1)         /* Supply clock. */
#define SDHI_CLOCK_DISABLE          (0)         /* Halt clock. */
#define SDHI_CLOCK_MASK_SCLKEN      (0x0100u)   /* SCLKENABLE */

/* ==== SDHI register address ==== */
#define SDHI_SDCMD                  (0x00u)         /* SD Command */
#define SDHI_SDARG                  (0x08u)         /* SD Command Argument */
#define SDHI_SDSTOP                 (0x10u)         /* Data Stop */
#define SDHI_SDBLKCNT               (0x14u)         /* Block Count */
#define SDHI_SDRESP10               (0x18u)         /* Response R39-8 */
#define SDHI_SDRESP32               (0x20u)         /* Response R71-40 */
#define SDHI_SDRESP54               (0x28u)         /* Response R103-72 */
#define SDHI_SDRESP76               (0x30u)         /* Response R127-104 */
#define SDHI_SDSTS1                 (0x38u)         /* SD Interrupt Flag(1) */
#define SDHI_SDSTS2                 (0x3cu)         /* SD Interrupt Flag(2) */
#define SDHI_SDIMSK1                (0x40u)         /* SD Interrupt Flag(1) Mask */
#define SDHI_SDIMSK2                (0x44u)         /* SD Interrupt Flag(2) Mask */
#define SDHI_SDCLKCR                (0x48u)         /* SD Clock Control */
#define SDHI_SDSIZE                 (0x4cu)         /* Block Size */
#define SDHI_SDOPT                  (0x50u)         /* Access Option */
#define SDHI_SDERSTS1               (0x58u)         /* CMD,CRC,END Error Status */
#define SDHI_SDERSTS2               (0x5cu)         /* Timeout Error Status */
#define SDHI_SDBUFR                 (0x60u)         /* SD Buffer */
#define SDHI_SDIOMD                 (0x68u)         /* SDIO Mode */
#define SDHI_SDIOSTS                (0x6cu)         /* SDIO Interrupt Flag */
#define SDHI_SDIOIMSK               (0x70u)         /* SDIO Interrupt Flag Mask */
#define SDHI_SDDMAEN                (0x1b0u)        /* DMA Mode Enable */
#define SDHI_SDRST                  (0x1c0u)        /* Soft Reset */
#define SDHI_SDVER                  (0x1c4u)        /* Version */
#define SDHI_SDSWAP                 (0x1e0u)        /* SWAP Control */

/* ---- SDRST ---- */
#define SDHI_SDRST_ENABLE           (0x0006u)   /* Soft reset     RX64M    */
#define SDHI_SDRST_DISABLE          (0x0007u)   /* Soft reset Release */

/* ---- SDSTOP ---- */
#define SDHI_SDSTOP_SEC_ENABLE      (0x0100u)   /* Block count enable bit */
#define SDHI_SDSTOP_STP_STOP        (0x0001u)   /* Stop bit */
#define SDHI_SDSTOP_INIT            (0x0000u)   /* Stop register clear */

/* ---- SDSTS1 interrupt ---- */
#define SDHI_SDSTS1_DET_DAT3        (0x0400u)   /* Card (DAT3) */
#define SDHI_SDSTS1_ISD_CD          (0x0020u)   /* Card (CD) */
#define SDHI_SDSTS1_WP              (0x0080u)

/* ---- SDSTS1 interrupt mask ---- */
#define SDHI_SDIMSK1_INIT           (0x031du)   /* All initialization */
#define SDHI_SDIMSK1_DET_DAT3       (0x0300u)   /* Card Insert and Remove (DAT3) */
#define SDHI_SDIMSK1_DET_CD         (0x0018u)   /* Card Insert and Remove (CD) */
#define SDHI_SDIMSK1_INS_DAT3       (0x0200u)   /* Card Insert (DAT3) */
#define SDHI_SDIMSK1_INS_CD         (0x0010u)   /* Card Insert (CD) */
#define SDHI_SDIMSK1_REM_DAT3       (0x0100u)   /* Card Remove (DAT3) */
#define SDHI_SDIMSK1_REM_CD         (0x0008u)   /* Card Remove (CD) */
#define SDHI_SDIMSK1_DATA_TRNS      (0x0004u)   /* Command sequence end */
#define SDHI_SDIMSK1_TRNS_RESP      (0x0005u)   /* Command sequence end and Response end */
#define SDHI_SDIMSK1_RESP           (0x0001u)   /* Response end */
#define SDHI_SDIMSK1_DET_DAT3_CD    (SDHI_SDIMSK1_DET_DAT3 | SDHI_SDIMSK1_DET_CD)

/* ---- SDSTS2 interrupt mask ---- */
#define SDHI_SDIMSK2_INIT           (0x8b7fu)   /* All initialization */
#define SDHI_SDIMSK2_BWE            (0x8a7fu)   /* Write enable and All errors RX64M  */
#define SDHI_SDIMSK2_BRE            (0x897fu)   /* Read enable and All errors */
#define SDHI_SDIMSK2_ERR            (0x807fu)   /* All errors */
#define SDHI_SDIMSK2_CLEAR          (0x837fu)   /* All initialization clear */
#define SDHI_SDIMSK2_BIT11          (0x0800u)   /* Bit11 clear */
#define SDHI_SDIMSK2_ILA            (0x8000u)
#define SDHI_SDIMSK2_CBSY           (0x4000u)   /* Command type register busy */
#define SDHI_SDIMSK2_SCLKDIVEN      (0x2000u)   /* SD bus busy */
#define SDHI_SDIMSK2_ERR6           (0x0040u)
#define SDHI_SDIMSK2_ERR5           (0x0020u)
#define SDHI_SDIMSK2_ERR4           (0x0010u)
#define SDHI_SDIMSK2_ERR3           (0x0008u)
#define SDHI_SDIMSK2_ERR2           (0x0004u)
#define SDHI_SDIMSK2_ERR1           (0x0002u)
#define SDHI_SDIMSK2_ERR0           (0x0001u)
#define SDHI_SDIMSK2_WE             (0x0200u)   /* Write enable */
#define SDHI_SDIMSK2_RE             (0x0100u)   /* Read enable */

/* ---- SDIOSTS interrupt mask ---- */
#define SDHI_SDIOIMSK_INIT          (0xc007u)
#define SDHI_SDIOIMSK_EXWT          (0x8000u)
#define SDHI_SDIOIMSK_EXPUB52       (0x4000u)
#define SDHI_SDIOIMSK_IOIRQ         (0x0001u)   /* Interrupt from IO Card */

/* ---- SDOPT ---- */
#define SDHI_SDOPT_INIT             (0xc000u)   /* Initialization */
#define SDHI_SDOPT_WIDTH            (0x8000u)

/* ---- SDSWAP ---- */
#define SDHI_SDSWAP_DISABLE         (0x0000)
#define SDHI_SDSWAP_ENABLE          (0x00c0)

/* ---- SDIOSTS interrupt mask ---- */
#define SDHI_SDIOSTS_INIT           (0x0006u)
#define SDHI_SDIOSTS_EXWT           (0x8000u)
#define SDHI_SDIOSTS_EXPUB52        (0x4000u)
#define SDHI_SDIOSTS_IOIRQ          (0x0001u)   /* Interrupt from IO Card */

/* ---- SDIOMD ---- */
#define SDHI_SDIOMD_C52PUB          (0x0200u)
#define SDHI_SDIOMD_IOABT           (0x0100u)
#define SDHI_SDIOMD_RWREQ           (0x0004u)
#define SDHI_SDIOMD_INTEN           (0x0001u)   /* Interrupt from IO Card */
#define SDHI_SDIOMD_CLEAR           (0x0000u)

/* ---- SDDMAEN ---- */
#define SDHI_SDDMAEN_DMASDRW        (0x0002u)

/* ---- Error code ---- */
typedef enum e_sdhi_status
{
    SDHI_SUCCESS =   0,     /* OK */
    SDHI_ERR     =  -1,     /* General error */
} sdhi_status_t;

typedef struct
{
    uint32_t sdrsp10;
    uint32_t sdrsp32;
    uint32_t sdrsp54;
    uint32_t sdrsp76;
} sdhi_get_resp_t;

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
sdhi_status_t R_SDHI_Open(uint32_t channel);
sdhi_status_t R_SDHI_Close(uint32_t channel);
void          R_SDHI_IntHandler0(void * vect);
sdhi_status_t R_SDHI_IntCallback(uint32_t channel, sdhi_status_t (* callback)(uint32_t, uint32_t));
sdhi_status_t R_SDHI_IntSDBuffCallback(uint32_t channel, sdhi_status_t (* callback)(void *));
sdhi_status_t R_SDHI_IntSdioCallback(uint32_t channel, sdhi_status_t (* callback)(uint32_t));
sdhi_status_t R_SDHI_EnableIcuInt(uint32_t channel, uint32_t select);
sdhi_status_t R_SDHI_DisableIcuInt(uint32_t channel, uint32_t select);
sdhi_status_t R_SDHI_SetIntMask(uint32_t channel, uint32_t mask1, uint32_t mask2);
sdhi_status_t R_SDHI_ClearIntMask(uint32_t channel, uint32_t mask1, uint32_t mask2);
sdhi_status_t R_SDHI_ClearSdstsReg(uint32_t channel, uint32_t clear_sdsts1, uint32_t clear_sdsts2);
sdhi_status_t R_SDHI_SetSdioIntMask(uint32_t channel, uint32_t mask);
sdhi_status_t R_SDHI_ClearSdioIntMask(uint32_t channel, uint32_t mask);
sdhi_status_t R_SDHI_ClearSdiostsReg(uint32_t channel, uint32_t clear);
sdhi_status_t R_SDHI_SetClock(uint32_t channel, uint32_t div, int32_t enable);
sdhi_status_t R_SDHI_SetBus(uint32_t channel, int32_t width);
sdhi_status_t R_SDHI_GetResp(uint32_t channel, sdhi_get_resp_t * p_resp_reg);
sdhi_status_t R_SDHI_OutReg(uint32_t channel, uint32_t reg, uint32_t data);
sdhi_status_t R_SDHI_InReg(uint32_t channel, uint32_t reg, uint32_t * p_data);
sdhi_status_t R_SDHI_CDLayout(uint32_t channel);
sdhi_status_t R_SDHI_WPLayout(uint32_t channel);
sdhi_status_t R_SDHI_GetWP(uint32_t channel, uint32_t * p_wp);
sdhi_status_t R_SDHI_GetSpeedType(uint32_t channel);
sdhi_status_t R_SDHI_GetBuffRegAddress(uint32_t channel, uint32_t * p_reg_buff);
uint32_t      R_SDHI_GetVersion(void);
sdhi_status_t R_SDHI_SetLogHdlAddress(uint32_t user_long_que);
uint32_t      R_SDHI_Log(uint32_t flg, uint32_t fid, uint32_t line);

#endif /* SDHI_RX_H */

