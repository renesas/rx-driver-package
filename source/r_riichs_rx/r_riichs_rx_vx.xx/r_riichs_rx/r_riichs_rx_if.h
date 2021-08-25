/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
 * applicable laws, including copyright laws. 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO 
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the 
 * following link:
 * http://www.renesas.com/disclaimer 
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_riichs_rx_if.h
 * Description  : Functions for using RIICHS on RX devices. 
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.06.2021 1.00     First Release
 **********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef RIICHS_IF_H
    #define RIICHS_IF_H
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
/* Includes board and MCU related header files. */
    #include "platform.h"
/* Used for configuring the RIICHS code */
    #include "r_riichs_rx_config.h"

R_BSP_PRAGMA_UNPACK
/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
#if R_BSP_VERSION_MAJOR < 6
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
    #define RIICHS_VERSION_MAJOR      (1)
    #define RIICHS_VERSION_MINOR      (00)

/*----------------------------------------------------------------------------*/
/*   Defines the argument of the R_RIICHS_Control function.                     */
/*----------------------------------------------------------------------------*/
    #define RIICHS_GEN_START_CON      ((uint8_t)(0x01))   /* Generate start condition */
    #define RIICHS_GEN_STOP_CON       ((uint8_t)(0x02))   /* Generate stop condition */
    #define RIICHS_GEN_RESTART_CON    ((uint8_t)(0x04))   /* Generate restart condition */
    #define RIICHS_GEN_SDA_HI_Z       ((uint8_t)(0x08))   /* Generate nack output */
    #define RIICHS_GEN_SCL_ONESHOT    ((uint8_t)(0x10))   /* Generate one-shot scl clock output */
    #define RIICHS_GEN_RESET          ((uint8_t)(0x20))   /* Reset riichs module */

/*----------------------------------------------------------------------------*/
/*   Define values of channel state flag.                                     */
/*----------------------------------------------------------------------------*/
    #define RIICHS_NO_INIT        ((riichs_ch_dev_status_t)(0)) /* Uninitialized state */
    #define RIICHS_IDLE           ((riichs_ch_dev_status_t)(1)) /* Successful operation */
    #define RIICHS_FINISH         ((riichs_ch_dev_status_t)(2)) /* Already idle state */
    #define RIICHS_NACK           ((riichs_ch_dev_status_t)(3)) /* Already idle state */
    #define RIICHS_COMMUNICATION  ((riichs_ch_dev_status_t)(4)) /* Successful operation */
    #define RIICHS_AL             ((riichs_ch_dev_status_t)(5)) /* Arbitration lost */
    #define RIICHS_TMO            ((riichs_ch_dev_status_t)(6)) /* Time Out */
    #define RIICHS_ERROR          ((riichs_ch_dev_status_t)(7)) /* error */

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/
/*----------------------------------------------------------------------------*/
/*   Define return values and values of channel state flag.                   */
/*----------------------------------------------------------------------------*/
typedef uint8_t riichs_ch_dev_status_t;

typedef enum
{
    RIICHS_SUCCESS = 0U, /* Successful operation */
    RIICHS_ERR_LOCK_FUNC, /* Lock has already been acquired by another task. */
    RIICHS_ERR_INVALID_CHAN, /* None existent channel number */
    RIICHS_ERR_INVALID_ARG, /* Parameter error */
    RIICHS_ERR_NO_INIT, /* Uninitialized state */
    RIICHS_ERR_BUS_BUSY, /* Channel is on communication. */
    RIICHS_ERR_AL, /* Arbitration lost error */
    RIICHS_ERR_TMO, /* Time Out error */
    RIICHS_ERR_OTHER /* Other error */
} riichs_return_t;

/* Set slave address */
typedef enum
{
    RIICHS_ADDR_FORMAT_NONE      = (0U), /* Disable slave address */
    RIICHS_SEVEN_BIT_ADDR_FORMAT = (1U), /* 7bit address format */
    RIICHS_TEN_BIT_ADDR_FORMAT   = (2U)  /* 10bit address format */
} riichs_addr_format_t;

/* Select General call address */
typedef enum
{
    RIICHS_GCA_DISABLE = (0x0), /* not use */
    RIICHS_GCA_ENABLE  = (0x1)  /* use(General call address detection is enabled.) */
} riichs_gca_t;

/* Set the priority level for the riichs interrupt */
typedef enum
{
    RIICHS_IPL_1  = (0x1), /* interrupt priority level 1 */
    RIICHS_IPL_2  = (0x2), /* interrupt priority level 2 */
    RIICHS_IPL_3  = (0x3), /* interrupt priority level 3 */
    RIICHS_IPL_4  = (0x4), /* interrupt priority level 4 */
    RIICHS_IPL_5  = (0x5), /* interrupt priority level 5 */
    RIICHS_IPL_6  = (0x6), /* interrupt priority level 6 */
    RIICHS_IPL_7  = (0x7), /* interrupt priority level 7 */
    RIICHS_IPL_8  = (0x8), /* interrupt priority level 8 */
    RIICHS_IPL_9  = (0x9), /* interrupt priority level 9 */
    RIICHS_IPL_10 = (0x10), /* interrupt priority level 10 */
    RIICHS_IPL_11 = (0x11), /* interrupt priority level 11 */
    RIICHS_IPL_12 = (0x12), /* interrupt priority level 12 */
    RIICHS_IPL_13 = (0x13), /* interrupt priority level 13 */
    RIICHS_IPL_14 = (0x14), /* interrupt priority level 14 */
    RIICHS_IPL_15 = (0x15)  /* interrupt priority level 15 */
} riichs_priority_t;

/* Select Master Arbitration-Lost Detection enable or disable */
typedef enum
{
    RIICHS_MASTER_ARB_LOST_DISABLE = (0x0), /* Master arbitration-lost detection is disabled. */
    RIICHS_MASTER_ARB_LOST_ENABLE  = (0x1)  /* Master arbitration-lost detection is enabled. */
} riichs_master_arb_t;

/* Set using digital filter(Selected IIC phi cycle is filtered out) */
typedef enum
{
    RIICHS_DIGITAL_FILTER_0  = (0x0), /* not use*/
    RIICHS_DIGITAL_FILTER_1  = (0x1), /* one IIC phi */
    RIICHS_DIGITAL_FILTER_2  = (0x2), /* two IIC phi */
    RIICHS_DIGITAL_FILTER_3  = (0x3), /* three IIC phi */
    RIICHS_DIGITAL_FILTER_4  = (0x4), /* four IIC phi */
    RIICHS_DIGITAL_FILTER_5  = (0x5), /* five IIC phi */
    RIICHS_DIGITAL_FILTER_6  = (0x6), /* six IIC phi */
    RIICHS_DIGITAL_FILTER_7  = (0x7), /* seven IIC phi */
    RIICHS_DIGITAL_FILTER_8  = (0x8), /* eight IIC phi */
    RIICHS_DIGITAL_FILTER_9  = (0x9), /* nine IIC phi */
    RIICHS_DIGITAL_FILTER_10 = (0x10), /* ten IIC phi */
    RIICHS_DIGITAL_FILTER_11 = (0x11), /* eleven IIC phi */
    RIICHS_DIGITAL_FILTER_12 = (0x12), /* twelve IIC phi */
    RIICHS_DIGITAL_FILTER_13 = (0x13), /* thirteen IIC phi */
    RIICHS_DIGITAL_FILTER_14 = (0x14), /* fourteen IIC phi */
    RIICHS_DIGITAL_FILTER_15 = (0x15), /* fifteen IIC phi */
    RIICHS_DIGITAL_FILTER_16 = (0x16)  /* sixteen IIC phi */
} riichs_filter_t;

/* Select Timeout function enable or disable */
typedef enum
{
    RIICHS_TMO_DISABLE = (0x0), /* Timeout detection is disabled */
    RIICHS_TMO_ENABLE  = (0x1)  /* Timeout detection is enabled */
} riichs_timeout_t;

/* Select NACK Detection enable or disable */
typedef enum
{
    RIICHS_NACK_DETC_DISABLE = (0x0), /* NACK detection is disabled */
    RIICHS_NACK_DETC_ENABLE  = (0x1)  /* NACK detection is enabled */
} riichs_nack_detc_t;

/* Select Arbitration Lost enable or disable */
typedef enum
{
    RIICHS_ARB_LOST_DISABLE = (0x0), /* Arbitration lost detection is disabled */
    RIICHS_ARB_LOST_ENABLE  = (0x1)  /* Arbitration lost detection is enabled */
} riichs_arb_lost_t;

/* Select 16bit,14bit,8bit,6bit for the timeout detection time */
typedef enum
{
    RIICHS_COUNTER_BIT16 = (0x0), /* Timeout counter is 16 bits (up to 65536 counts) */
    RIICHS_COUNTER_BIT14 = (0x1), /* Timeout counter is 14 bits (up to 16384 counts) */
    RIICHS_COUNTER_BIT8  = (0x2), /* Timeout counter is 8 bits (up to 256 counts) */
    RIICHS_COUNTER_BIT6  = (0x3)  /* Timeout counter is 6 bits (up to 64 counts) */
} riichs_counter_bit_t;

/* Select enable or disable the internal counter of the timeout function to count up while the */
/* SCL line is held LOW when the timeout function is enabled. */
typedef enum
{
    RIICHS_L_COUNT_DISABLE = (0x0), /* Count is disabled while the SCLHS0 line is low */
    RIICHS_L_COUNT_ENABLE  = (0x1)  /* Count is enabled while the SCLHS0 line is low. */
} riichs_low_count_t;

/* Select enable or disable the internal counter of the timeout function to count up while the */
/* SCL line is held HIGH when the timeout function is enabled. */
typedef enum
{
    RIICHS_H_COUNT_DISABLE = (0x0), /* Count is disabled while the SCLHS0 line is high. */
    RIICHS_H_COUNT_ENABLE  = (0x1)  /* Count is enabled while the SCLHS0 line is high. */
} riichs_high_count_t;

/* Timeout Detection Mode Select */
typedef enum
{
    RIICHS_TIMEOUT_MODE_ALL  = (0x0), /* Time-out function is enabled in three conditions. */
    RIICHS_TIMEOUT_MODE_BUSY = (0x1), /* Time-out function is enabled during ICBSR.BFREE flag = 0. */
    RIICHS_TIMEOUT_MODE_FREE = (0x2)  /* Time-out function is enabled during ICBSR.BFREE flag = 1. */
} riichs_time_mode_t;

/*----------------------------------------------------------------------------*/
/*   Define riichs information structure type.                                */
/*----------------------------------------------------------------------------*/
/* ---- Callback function type. ---- */
typedef void (*riichs_callback) (void); /* Callback function type */

/* ---- IIC Information structure type. ---- */
typedef volatile struct
{
    uint8_t              rsv2; /* reserved */
    uint8_t              rsv1; /* reserved */
    riichs_ch_dev_status_t dev_sts; /* Device status flag */
    uint8_t              ch_no; /* Channel No. */
    riichs_callback        callbackfunc; /* Callback function */
    uint32_t             cnt2nd; /* 2nd Data Counter */
    uint32_t             cnt1st; /* 1st Data Counter */
    uint8_t            * p_data2nd; /* Pointer for 2nd Data buffer */
    uint8_t            * p_data1st; /* Pointer for 1st Data buffer */
    uint8_t            * p_slv_adr; /* Pointer for Slave address buffer */
    double               scl_up_time; /* Rise time of SCLn Line */
    double               scl_down_time; /* Fall time of SCLn Line */
    double               fs_scl_up_time; /* Rise time of SCLn Line before transition to Hs mode */
    double               fs_scl_down_time; /* Fall time of SCLn Line before transition to Hs mode */
    uint32_t             speed_kbps; /* RIICHS bps(kbps) */
    uint32_t             fs_speed_kbps; /* RIICHS bps(kbps) before transition to Hs mode */
    uint32_t             bus_check_counter; /* software bus busy check counter */
    uint32_t             bus_free_time; /* software bus free counter */
    uint16_t             slave_addr0; /* Slave Address 0 Register (SAR0) */
    uint16_t             slave_addr1; /* Slave Address 1 Register (SAR1) */
    uint16_t             slave_addr2; /* Slave Address 2 Register (SAR2) */
    riichs_addr_format_t slave_addr0_format; /* slave address 0 format */
    riichs_addr_format_t slave_addr1_format; /* slave address 1 format */
    riichs_addr_format_t slave_addr2_format; /* slave address 2 format */
    riichs_gca_t         gca_enable; /* General call address detection */
    riichs_priority_t    rxi_priority; /* The priority level of the RXI */
    riichs_priority_t    txi_priority; /* The priority level of the TXI */
    riichs_priority_t    eei_priority; /* The priority level of the EEI, not lower than the level of TXI and RXI */
    riichs_priority_t    tei_priority; /* The priority level of the TEI, not lower than the level of TXI and RXI */
    riichs_master_arb_t  master_arb; /* Master Arbitration-Lost Detection function */
    riichs_filter_t      filter_stage; /* using digital noise filter stage */
    riichs_timeout_t     timeout_enable; /* Timeout function */
    riichs_nack_detc_t   nack_detc_enable; /* NACK Detection */
    riichs_arb_lost_t    arb_lost_enable; /* Arbitration Lost */
    riichs_counter_bit_t counter_bit; /* bit for the timeout detection time */
    riichs_low_count_t   l_count; /* SCL line is held LOW when the timeout function is enabled */
    riichs_high_count_t  h_count; /* SCL line is held HIGH when the timeout function is enabled */
    riichs_time_mode_t   timeout_mode; /* Timeout Detection Mode Select */
} riichs_info_t;

/*----------------------------------------------------------------------------*/
/*   Define riichs status structure type.                                     */
/*----------------------------------------------------------------------------*/
typedef union
{
    uint32_t LONG;
    R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_22
    (
        uint32_t rsv :11, /* reserve */
        uint32_t AAS2 :1, /* Slave2 address detection flag */
        uint32_t AAS1 :1, /* Slave1 address detection flag */
        uint32_t AAS0 :1, /* Slave0 address detection flag */
        uint32_t GCA :1, /* General call address detection flag */
        uint32_t DID :1, /* DeviceID address detection flag */
        uint32_t HOA :1, /* Host address detection flag */
        uint32_t MST :1, /* Master mode / Slave mode flag */
        uint32_t TMO :1, /* Time out flag */
        uint32_t AL :1, /* Arbitration lost detection flag */
        uint32_t SP :1, /* Stop condition detection flag */
        uint32_t ST :1, /* Start condition detection flag */
        uint32_t RBUF :1, /* Receive buffer status flag */
        uint32_t SBUF :1, /* Send buffer status flag */
        uint32_t SCLO :1, /* SCL pin output control status */
        uint32_t SDAO :1, /* SDA pin output control status */
        uint32_t SCLI :1, /* SCL pin level */
        uint32_t SDAI :1, /* SDA pin level */
        uint32_t NACK :1, /* NACK detection flag */
        uint32_t TRS :1, /* Send mode / Receive mode flag */
        uint32_t BSY :1, /* Bus status flag */
        uint32_t HSMC :1 /* Hs-mode Master Code Detection flag */
    ) BIT;
} riichs_mcu_status_t;

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/
/* ---- Channel status ---- */
extern riichs_ch_dev_status_t g_riichs_ChStatus[];

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
/* ---- RIICHS Driver API functions ---- */
riichs_return_t R_RIICHS_Open (riichs_info_t *);
riichs_return_t R_RIICHS_MasterSend (riichs_info_t *);
riichs_return_t R_RIICHS_MasterReceive (riichs_info_t *);
riichs_return_t R_RIICHS_SlaveTransfer (riichs_info_t *);
riichs_return_t R_RIICHS_GetStatus (riichs_info_t *, riichs_mcu_status_t *);
riichs_return_t R_RIICHS_Control (riichs_info_t *, uint8_t ctrl_ptn);
riichs_return_t R_RIICHS_Close (riichs_info_t *);
uint32_t R_RIICHS_GetVersion (void);

R_BSP_PRAGMA_PACKOPTION
#endif /* RIICHS_IF_H */

