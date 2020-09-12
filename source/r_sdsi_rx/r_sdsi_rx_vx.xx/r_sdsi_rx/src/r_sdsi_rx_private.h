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
* Copyright (C) 2016(2019) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdsi_rx_private.h
* Device       : RX
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX
* Description  : Private file for SDSI API for RX
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.09.2016 1.00    First Release
*         : 31.07.2017 2.00    Supported RX65N-2MB.
*         :                    Fixed to correspond to Renesas coding rule.
*         : 04.04.2019 2.02    Added support for GNUC and ICCRX.
*                              Fixed coding style.
**********************************************************************************************************************/

#ifndef SDSI_RX_PRIVATE_H_
#define SDSI_RX_PRIVATE_H_

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* User specific options for Flash API */
#include "r_sdsi_rx_config.h"

#ifdef SDSI_CFG_USE_FIT
/* Includes board and MCU related header files. */
#include "platform.h"
#endif  /* SDSI_CFG_USE_FIT */

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#ifdef SDSI_CFG_LONGQ_ENABLE            /* Uses FIT LONGQ module */
#define SDSI_DRIVER_ID      (5)     /* sdsi debug log id */
#define SDSI_DEBUG_ERR_ID   (1)     /* sdsi debug log error id */
#endif /* SDSI_CFG_LONGQ_ENABLE */

#define SDSI_BIG_ENDIAN     (defined(__BIG) || defined(__RX_BIG_ENDIAN__))

/* ==== SDSI register address ==== */
#define SDSI_FN1ACCR        (0x00)
#define SDSI_INTENCR1       (0x04)
#define SDSI_INTSR1         (0x05)
#define SDSI_SDCMDCR        (0x06)
#define SDSI_SDCADD0R       (0x07)
#define SDSI_SDCADD1R       (0x08)
#define SDSI_SDCADD2R       (0x09)
#define SDSI_SDSICR1        (0x0a)
#define SDSI_DMACR1         (0x0b)
#define SDSI_BLKCNT         (0x0c)
#define SDSI_BYTCNT         (0x0e)
#define SDSI_DMATRADDR      (0x10)
#define SDSI_SDSICR2        (0x100)
#define SDSI_SDSICR3        (0x104)
#define SDSI_INTENCR2       (0x108)
#define SDSI_INTSR2         (0x10c)
#define SDSI_DMACR2         (0x110)
#define SDSI_CISDATAR0      (0x200)
#define SDSI_FBR1           (0x270)
#define SDSI_FBR2           (0x274)
#define SDSI_FBR3           (0x278)
#define SDSI_FBR4           (0x27c)
#define SDSI_FBR5           (0x280)
#define SDSI_FN1DATAR10     (0x800)
#define SDSI_FN1DATAR20     (0x900)
#define SDSI_FN1DATAR30     (0xa00)
#define SDSI_FN1INTVECR     (0xb00)
#define SDSI_FN1INTCLRR     (0xb01)
#define SDSI_FN1DATAR50     (0xc00)

/* Register offset */
#define SDSI_CIS_OFFSET_MAX (108)
#define SDSI_OFFSET_256     (256)
#define SDSI_OFFSET_1024    (1024)

/* ---- Shift values ---- */
#define SDSI_BIT_SHIFT_1    (1)
#define SDSI_BIT_SHIFT_2    (2)
#define SDSI_BIT_SHIFT_3    (3)
#define SDSI_BIT_SHIFT_4    (4)
#define SDSI_BIT_SHIFT_5    (5)
#define SDSI_BIT_SHIFT_6    (6)
#define SDSI_BIT_SHIFT_7    (7)
#define SDSI_BIT_SHIFT_8    (8)
#define SDSI_BIT_SHIFT_9    (9)
#define SDSI_BIT_SHIFT_10   (10)
#define SDSI_BIT_SHIFT_11   (11)
#define SDSI_BIT_SHIFT_12   (12)
#define SDSI_BIT_SHIFT_13   (13)
#define SDSI_BIT_SHIFT_14   (14)
#define SDSI_BIT_SHIFT_15   (15)
#define SDSI_BIT_SHIFT_16   (16)
#define SDSI_BIT_SHIFT_17   (17)
#define SDSI_BIT_SHIFT_18   (18)
#define SDSI_BIT_SHIFT_19   (19)
#define SDSI_BIT_SHIFT_20   (20)
#define SDSI_BIT_SHIFT_21   (21)
#define SDSI_BIT_SHIFT_22   (22)
#define SDSI_BIT_SHIFT_23   (23)
#define SDSI_BIT_SHIFT_24   (24)
#define SDSI_BIT_SHIFT_25   (25)
#define SDSI_BIT_SHIFT_26   (26)
#define SDSI_BIT_SHIFT_27   (27)
#define SDSI_BIT_SHIFT_28   (28)
#define SDSI_BIT_SHIFT_29   (29)
#define SDSI_BIT_SHIFT_30   (30)
#define SDSI_BIT_SHIFT_31   (31)

/* ---- FN1ACCR ---- */
#define SDSI_FN1ACC_ENABLE          (0x00000800)

/* ---- INTENCR1 ----*/
#define SDSI_INTENCR1_DISABLE       (0x00)
#define SDSI_INTENCR1_ENABLE        (0x07)
#define SDSI_INTENCR1_CMD52WIREN    (0x01)
#define SDSI_INTENCR1_CMD53WIREN    (0x02)
#define SDSI_INTENCR1_CMD53RIREN    (0x04)

/* ---- INTSR1 ----*/
#define SDSI_INTSR1_INIT            (0x07)

/* ---- SDCMDCR ----*/
#define SDSI_SDCMDCR_SDCMDINDEX     (0x01)

/* ---- SDCADD0R ----*/
/* ---- SDCADD1R ----*/
/* ---- SDCADD2R ----*/
/* No definition */

/* ---- SDSICR1 ----*/
#define SDSI_SDSICR1_IOE1IOR1       (0x01)
#define SDSI_SDSICR1_EPS            (0x02)
#define SDSI_SDSICR1_EMPC           (0x04)

/* ---- DMACR1 ----*/
#define SDSI_DMACR1_DISABLE             (0x00)
#define SDSI_DMACR1_ENABLE_BUS_UNLOCK   (0x01)
#define SDSI_DMACR1_ENABLE_BUS_LOCK     (0x03)

/* ---- BLKCNT ----*/
/* ---- BYTCNT ----*/
/* ---- DMATRADDR ----*/
/* No definition */

/* ---- SDSICR2 ---- */
#define SDSI_SDSICR2_SWAP_DISABLE   (0x00000000)
#if (SDSI_BIG_ENDIAN)
#define SDSI_SDSICR2_INIT           (0x00000000)
#else
#define SDSI_SDSICR2_INIT           (0x00000005)
#endif /* (SDSI_BIG_ENDIAN) */
#define SDSI_SDSICR2_REG5EN_ENABLE  (0x00000010)

/* ---- SDSICR3 ---- */
#define SDSI_SDSICR3_SRST_DISABLE   (0x00000000)
#define SDSI_SDSICR3_SRST_ENABLE    (0x00000001)
#define SDSI_SDSICR3_IOR0           (0x00000004)
#define SDSI_SDSICR3_CEN            (0x00000008)

/* ---- INTENCR2 ---- */
#define SDSI_INTENCR2_DISABLE       (0x00000000)
#define SDSI_INTENCR2_ENABLE        (0x00000007)
#define SDSI_INTENCR2_CDFEN         (0x00000001)
#define SDSI_INTENCR2_CDREN         (0x00000002)
#define SDSI_INTENCR2_DTEEN         (0x00000004)

/* ---- INTSR2 ---- */
#define SDSI_INTSR2_INIT            (0x00000000)

/* ---- DMACR2 ---- */
#define SDSI_DMACR2_SWAP_DISABLE    (0x00000000)
#if (SDSI_BIG_ENDIAN)
#define SDSI_DMACR2_INIT            (0x0000000f)
#else
#define SDSI_DMACR2_INIT            (0x00000000)
#endif /* (SDSI_BIG_ENDIAN) */
#define SDSI_DMACR2_DMASDSEL        (0x00000100)

/* ---- CISDATARi ---- */
/* ---- FBR1 ---- */
/* ---- FBR2 ---- */
/* ---- FBR3 ---- */
/* ---- FBR4 ---- */
/* ---- FBR5 ---- */
/* ---- FN1DATAR1i ---- */
/* ---- FN1DATAR2i ---- */
/* ---- FN1DATAR3i ---- */
/* ---- FN1INTVECR ---- */
/* ---- FN1INTCLRR ---- */
/* ---- FN1DATAR5i ---- */
/* No definition */

/* SDSI direct transfer mode */
#define SDSI_MODE_DIRECT_NONE       (0x00000000)
#define SDSI_MODE_DIRECT_ADDR_FIXED (0x00000001)
#define SDSI_MODE_DIRECT_ADDR_INC   (0x00000002)
#define SDSI_MODE_DIRECT_ADDR_MASK  (0x00000003)
#define SDSI_MODE_DIRECT_BUS_LOCK   (0x00000010)
#define SDSI_MODE_DIRECT_BUS_UNLOCK (0x00000020)
#define SDSI_MODE_DIRECT_BUS_MASK   (0x00000030)

#define SDSI_GET_HNDL(a)            (g_sdsi_hndl[a])

#define SDSI_OUTREG_LONG(h,offset,data)  (*(volatile uint32_t *)((h)->reg_base+(offset)) = (data))
#define SDSI_INREG_LONG(h,offset)        (*(volatile uint32_t *)((h)->reg_base+(offset)))
#define SDSI_OUTREG_WORD(h,offset,data)  (*(volatile uint16_t *)((h)->reg_base+(offset)) = (data))
#define SDSI_INREG_WORD(h,offset)        (*(volatile uint16_t *)((h)->reg_base+(offset)))
#define SDSI_OUTREG_BYTE(h,offset,data)  (*(volatile uint8_t *)((h)->reg_base+(offset)) = (data))
#define SDSI_INREG_BYTE(h,offset)        (*(volatile uint8_t *)((h)->reg_base+(offset)))

#ifdef SDSI_CFG_LONGQ_ENABLE    /* Uses FIT LONGQ module */
typedef enum e_sdsi_log
{
    SDSI_RX  = 0,
    SDSI_DEV = 1
} sdsi_logid_t;
#endif /* SDSI_CFG_LONGQ_ENABLE */


/* ==== SDSI Driver work buffer ==== */
typedef struct                                              /* SDSI handle */
{
    uint32_t            reg_base;                           /* SDSI base address */
    uint32_t            intsr1;                             /* INTSR1 register */
    uint32_t            intsr2;                             /* INTSR2 register */
    uint32_t            direct_trans_mode;                  /* SDSI direct transfer mode */
    sdsi_status_t       (*int_callback)(sdsi_cmd_t *);      /* Callback function for cmd interrupt */
    sdsi_status_t       (*int_cd_callback)(uint32_t);       /* Callback function for card detection interrupt */
    sdsi_status_t       (*int_dt_callback)(sdsi_cmd_t *);   /* Callback function for direct transfer end interrupt */
} sdsi_hndl_t; /* Total 28 bytes */

extern const void * g_sdsi_cfg_ip_base[1];
extern sdsi_hndl_t * g_sdsi_hndl[((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t))];

/* ==== Prototype ==== */
#ifdef SDSI_CFG_LONGQ_ENABLE                                /* Uses FIT LONGQ module                */
uint32_t      r_sdsi_log(uint32_t flg, uint32_t fid, uint32_t line);
#define SDSI_LOG_FUNC(x, y, z)   (r_sdsi_log((x), (y), (z)))
#else
#define SDSI_LOG_FUNC(x, y, z)
#endif /* SDSI_CFG_LONGQ_ENABLE */

/* ---- Target MCU I/F ---- */
bool          r_sdsi_dev_hardware_lock(uint32_t channel);
bool          r_sdsi_dev_hardware_unlock(uint32_t channel);
sdsi_status_t r_sdsi_dev_module_enable(uint32_t channel);
sdsi_status_t r_sdsi_dev_module_disable(uint32_t channel);
sdsi_status_t r_sdsi_dev_disable_system_int(uint32_t channel);
sdsi_status_t r_sdsi_dev_enable_system_int(uint32_t channel);
sdsi_status_t r_sdsi_dev_open(uint32_t channel);
sdsi_status_t r_sdsi_dev_close(uint32_t channel);
sdsi_status_t r_sdsi_dev_initialize(uint32_t channel);
sdsi_status_t r_sdsi_dev_end(uint32_t channel);
sdsi_status_t r_sdsi_dev_c_flag_polling(uint32_t channel);
sdsi_status_t r_sdsi_dev_write_cis_reg(uint32_t channel, sdsi_cis_reg_t * p_sdsi_reg);
sdsi_status_t r_sdsi_dev_read_cis_reg(uint32_t channel, sdsi_cis_reg_t * p_sdsi_reg);
sdsi_status_t r_sdsi_dev_write_func_reg(uint32_t channel, sdsi_func_reg_t * p_sdsi_reg);
sdsi_status_t r_sdsi_dev_read_func_reg(uint32_t channel, sdsi_func_reg_t * p_sdsi_reg);
sdsi_status_t r_sdsi_dev_write_int_vector_reg(uint32_t channel, uint8_t vector);
sdsi_status_t r_sdsi_dev_read_int_vector_reg(uint32_t channel, uint8_t * p_vector);
sdsi_status_t r_sdsi_dev_read_int_clear_reg(uint32_t channel, uint8_t * p_vector);
sdsi_status_t r_sdsi_dev_chk_direct_trans_adr(uint32_t channel, uint32_t adr);
sdsi_status_t r_sdsi_dev_enable_direct_trans(uint32_t channel, sdsi_direct_trans_t * p_sdsi_direct_trans);
sdsi_status_t r_sdsi_dev_disable_direct_trans(uint32_t channel);
sdsi_status_t r_sdsi_dev_set_direct_trans_adr(uint32_t channel, uint32_t adr);
sdsi_status_t r_sdsi_dev_get_direct_trans_adr(uint32_t channel, uint32_t * p_adr);
sdsi_status_t r_sdsi_dev_enable_int(uint32_t channel);
sdsi_status_t r_sdsi_dev_disable_int(uint32_t channel);
sdsi_status_t r_sdsi_dev_get_int(uint32_t channel);
sdsi_status_t r_sdsi_dev_clear_int(uint32_t channel);
sdsi_status_t r_sdsi_dev_get_cmd_info(uint32_t channel, sdsi_cmd_t * p_sdsi_cmd);
sdsi_status_t r_sdsi_dev_update_direct_trans_adr(uint32_t channel);

#endif /* _SDSI_RX_PRIVATE_H_ */

