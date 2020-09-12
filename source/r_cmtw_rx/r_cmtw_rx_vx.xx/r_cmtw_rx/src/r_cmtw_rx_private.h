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
* Copyright (C) 2014-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_cmtw_rx_private.h
* Description  : This module creates a timer tick using a CMTW channel based on an input by the user.
*              : The module also supports output compare and input capture features to generate
*              : waveforms and detect external events.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description               
*         : 29.08.2014 1.00    First GSCE Release.
*         : 15.03.2016 1.20    Error in description of Comment amended
*         : 21.07.2017 1.30    Corrected file name.
*         : 20.05.2019 2.00    Added support for GNUC and ICCRX.
***********************************************************************************************************************/
#ifndef CMTW_RX_PRIVATE_H
#define CMTW_RX_PRIVATE_H

#include "r_cmtw_rx_if.h"

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Bit position masks */
#define BIT0_MASK   (1<<0)
#define BIT1_MASK   (1<<1)
#define BIT2_MASK   (1<<2)
#define BIT3_MASK   (1<<3)
#define BIT4_MASK   (1<<4)
#define BIT5_MASK   (1<<5)
#define BIT6_MASK   (1<<6)
#define BIT7_MASK   (1<<7)
#define BIT8_MASK   (1<<8)
#define BIT9_MASK   (1<<9)
#define BIT10_MASK  (1<<10)
#define BIT11_MASK  (1<<11)
#define BIT12_MASK  (1<<12)
#define BIT13_MASK  (1<<13)
#define BIT14_MASK  (1<<14)
#define BIT15_MASK  (1<<15)
#define BIT16_MASK  (1<<16)
#define BIT17_MASK  (1<<17)
#define BIT18_MASK  (1<<18)
#define BIT19_MASK  (1<<19)
#define BIT20_MASK  (1<<20)
#define BIT21_MASK  (1<<21)
#define BIT22_MASK  (1<<22)
#define BIT23_MASK  (1<<23)
#define BIT24_MASK  (1<<24)
#define BIT25_MASK  (1<<25)
#define BIT26_MASK  (1<<26)
#define BIT27_MASK  (1<<27)
#define BIT28_MASK  (1<<28)
#define BIT29_MASK  (1<<29)
#define BIT30_MASK  (1<<30)
#define BIT31_MASK  (1<<31)

#define IEN0   (1<<0)
#define IEN1   (1<<1)
#define IEN2   (1<<2)
#define IEN3   (1<<3)
#define IEN4   (1<<4)
#define IEN5   (1<<5)
#define IEN6   (1<<6)
#define IEN7   (1<<7)


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    CMTW_PRV_CHANNEL_STATE_CLOSED = 0,    /* Channel is not yet initialed */
    CMTW_PRV_CHANNEL_STATE_OPENED,        /* Channel is enabled and ready  */
    CMTW_PRV_CHANNEL_STATE_RUNNING        /* Channel is running */
} cmtw_prv_channel_state_t;


typedef struct
{
    volatile  struct st_cmtw R_BSP_EVENACCESS_SFR   *regs;              /* Pointer to the registers */
    cmtw_channel_t                          channel;            /* Channel number */
    uint32_t                                stop_mask;          /* MSTP mask bit */
    uint8_t                                 cm_priorty;         /* Compare match interrupt priority level */
    uint8_t                                 oc0_priorty;        /* Output Compare 0 interrupt priority level */
    uint8_t                                 oc1_priorty;        /* Output Compare 1 interrupt priority level */
    uint8_t                                 ic0_priorty;        /* Input capture 0 interrupt priority level */
    uint8_t                                 ic1_priorty;        /* Input capture 1 interrupt priority level */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ir_cm;             /* Pointer to compare match ICU IR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ipr_cm;            /* Pointer to compare match ICU IPR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ier_cm;            /* Pointer to compare match ICU IER register */
    uint8_t                                 cm_enable_mask;     /* Compare match IEN mask bit */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ir_oc0;            /* Pointer to output compare 0 ICU IR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ipr_oc0;           /* Pointer to output compare 0 ICU IPR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ier_oc0;           /* Pointer to output compare 0 ICU IER register */
    uint8_t                                 oc0_enable_mask;    /* Output compare 0 IEN mask bit */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ir_oc1;            /* Pointer to output compare 1 ICU IR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ipr_oc1;           /* Pointer to output compare 1 ICU IPR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ier_oc1;           /* Pointer to output compare 1 ICU IER register */
    uint8_t                                 oc1_enable_mask;    /* Output compare 1 IEN mask bit */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ir_ic0;            /* Pointer to input capture 0 ICU IR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ipr_ic0;           /* Pointer to input capture 0 ICU IPR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ier_ic0;           /* Pointer to input capture 0 ICU IER register */
    uint8_t                                 ic0_enable_mask;    /* Input capture 0 IEN mask bit */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ir_ic1;            /* Pointer to input capture 0 ICU IR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ipr_ic1;           /* Pointer to input capture 0 ICU IPR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR           *ier_ic1;           /* Pointer to input capture 0 ICU IER register */
    uint8_t                                 ic1_enable_mask;    /* Input capture 1 IEN mask bit */
} cmtw_prv_ch_rom_info_t;

typedef struct
{
    const cmtw_prv_ch_rom_info_t    *rom;                       /* Pointer to ROM info */
    void                            (*pcallback)(void *pata);   /* Pointer to callback */
    cmtw_prv_channel_state_t        state;                      /* Channel state */
    cmtw_actions_t                  cm_action;                  /* Compare match action */
    cmtw_actions_t                  oc0_action;                 /* Output compare 0 action */
    cmtw_actions_t                  oc1_action;                 /* Output compare 1 action */
    cmtw_actions_t                  ic0_action;                 /* Input capture 0 action */
    cmtw_actions_t                  ic1_action;                 /* Input capture 1 action */
} cmtw_prv_ch_ctrl_info_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif  /* CMTW_RX_PRIVATE_H */
