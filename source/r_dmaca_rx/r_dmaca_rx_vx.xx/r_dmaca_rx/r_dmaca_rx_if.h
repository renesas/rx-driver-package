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
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_dmaca_rx_if.h
* Description  : Functions for DMACA driver
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.06.2014 1.00    Initial revision
*         : 27.08.2014 1.01    Not change program due to updating application note only.
*         : 30.01.2015 1.02    Added RX71M.
*         : 13.04.2015 1.03    Added RX231 and RX230.
*         : 30.09.2016 1.04    Added RX65N.
*         :                    Added transfer_count and vlock_size to dmaca_stat_t.
*         : 31.07.2017 1.05    Supported RX65N-2MB.
*         : 28.09.2018 1.10    Supported RX66T.
*         : 01.02.2019 1.20    Supported RX72T, RX65N-64pin.
*         : 20.05.2019 2.00    Added support for GNUC and ICCRX.
*         : 28.06.2019 2.10    Supported RX23W.
*         : 15.08.2019 2.20    Supported RX72M.
*         : 30.12.2019 2.30    Supported RX66N, RX72N.
*******************************************************************************/
#ifndef DMACA_RX_IF_H
#define DMACA_RX_IF_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
/* Configuration for this package. */
#include "r_dmaca_rx_config.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API */
#define DMACA_VERSION_MAJOR  (2)
#define DMACA_VERSION_MINOR  (30)

/* DMAC activation is disabled. */
#define DMACA_ACTIVE_DISABLE              (0x00u)
/* DMAC activation is enabled. */
#define DMACA_ACTIVE_ENABLE               (0x01u)

/* DMA transfer by software is not requested. */
#define DMACA_SOFT_REQ_DISABLE            (0x00u)
/* DMA transfer by software is requested and request flag is cleared automatically. */
#define DMACA_SOFT_REQ_WITH_AUTO_CLR_REQ  (0x01u)
/* DMA transfer by software is requested and request flag is not cleared. */
#define DMACA_SOFT_REQ_NOT_CLR_REQ        (0x11u)

/* Transfer Request Source (DMTMD.DCTG[1:0]) bit mask */
#define DMACA_REQ_BIT_MASK                (0x03u)

/* DMA Software Start Bit Auto Clear flag (DMREQ.CLRS) bit mask */
#define DMACA_CLRS_BIT_MASK               (0x10u)
/* DMA Software Start flag (DMREQ.SWREQ) bit mask */
#define DMACA_SWREQ_BIT_MASK              (0x01u)

/* DMA Active Flag (DMSTS.ACT) bit mask */
#define DMACA_ACT_BIT_MASK                (0x80u)
/* Transfer End Interrupt Flag (DMSTS.DTIF) bit mask */
#define DMACA_DTIF_BIT_MASK               (0x10u)
/* Transfer Escape End Interrupt Flag (DMSTS.ESIF) bit mask */
#define DMACA_ESIF_BIT_MASK               (0x01u)

/* Definition of DMAC channel */
#define DMACA_CH0                         (0)
#define DMACA_CH1                         (1)
#define DMACA_CH2                         (2)
#define DMACA_CH3                         (3)
#define DMACA_CH4                         (4)
#define DMACA_CH5                         (5)
#define DMACA_CH6                         (6)
#define DMACA_CH7                         (7)

/*******************************************************************************
Typedef definitions
*******************************************************************************/
/* DMTMD.MD[1:0](b15,b14) : Configurable options for Transfer Mode */
typedef enum e_dmaca_transfer_mode
{
    /* = (0 << 14) : Normal transfer */
    DMACA_TRANSFER_MODE_NORMAL = (0),

    /* Repeat transfer */
    DMACA_TRANSFER_MODE_REPEAT = (1 << 14),

    /* Block transfer */
    DMACA_TRANSFER_MODE_BLOCK  = (2 << 14)
} dmaca_transfer_mode_t;

/* DMTMD.DTS[1:0](b13,b12) : Configurable options for Side as repeat area */
typedef enum e_dmaca_repeat_block_side
{
    /* = (0 << 12) : The destination is specified as the repeat area or block area. */
    DMACA_REPEAT_BLOCK_DESTINATION = (0),

    /* The destination is specified as the repeat area or block area. */
    DMACA_REPEAT_BLOCK_SOURCE      = (1 << 12),

    /* The repeat area or block area is not specified. */
    DMACA_REPEAT_BLOCK_DISABLE     = (2 << 12)
} dmaca_repeat_block_side_t;

/* DMTMD.SZ[1:0](b9,b8) : Configurable options for Transfer Data Size */
typedef enum e_dmaca_data_size
{
    /* = (0 << 8) : 8-bit (byte) data */
    DMACA_DATA_SIZE_BYTE  = (0),

    /* 16-bit (word) data*/
    DMACA_DATA_SIZE_WORD  = (1 << 8),

    /* 32-bit (long word) data*/
    DMACA_DATA_SIZE_LWORD = (2 << 8)
} dmaca_data_size_t;

/* DMTMD.DCTG[1:0](b1,b0)) : Configurable options for Transfer Request Source */
typedef enum e_dmaca_request_source
{
    /* Interrupts from Software */
    DMACA_TRANSFER_REQUEST_SOFTWARE   = (0),

    /* Interrupts from peripheral modules or external interrupt input pins */
    DMACA_TRANSFER_REQUEST_PERIPHERAL = (1)
} dmaca_request_source_t;

/* DMINT.DTIE(b4) : Configurable options for Transfer End Interrupt Enable */
typedef enum e_dmaca_dti
{
    /* =(0 << 4) :  Disables the transfer end interrupt request. */
    DMACA_TRANSFER_END_INTERRUPT_DISABLE = (0),

    /* Enables the transfer end interrupt request. */
    DMACA_TRANSFER_END_INTERRUPT_ENABLE  = (1 << 4)
} dmaca_dti_t;

/* DMINT.ESIE(b3) : Configurable options for Transfer Escape End Interrupt Enable */
typedef enum e_dmaca_esi
{
    /*  =(0 << 3) : Disables the transfer escape end interrupt request. */
    DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE = (0),

    /* Enables the transfer escape end interrupt request. */
    DMACA_TRANSFER_ESCAPE_END_INTERRUPT_ENABLE  = (1 << 3)
} dmaca_esi_t;

/* DMINT.RPTIE(b2) : Configurable options for Repeat Size End Interrupt Enable */
typedef enum e_dmaca_rpti
{
    /*  =(0 << 2) : Disables the repeat size end interrupt request. */
    DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE = (0),

    /* Enables the repeat size end interrupt request. */
    DMACA_REPEAT_SIZE_END_INTERRUPT_ENABLE  = (1 << 2)
} dmaca_rpti_t;

/* DMINT.SARIE(b1) : Configurable options for Source Address Extended Repeat Area Overflow Interrupt Enable */
typedef enum e_dmaca_sari
{
    /* =(0 << 1) : Disables an interrupt request for an extended repeat area overflow on the source address. */
    DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE = (0),

    /* Enables an interrupt request for an extended repeat area overflow on the source address. */
    DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_ENABLE  = (1 << 1)
} dmaca_sari_t;

/* DMINT.DARIE(b0) : Configurable options for Destination Address Extended Repeat Area Overflow Interrupt Enable */
typedef enum e_dmaca_dari
{
    /* Disables an interrupt request for an extended repeat area overflow on the destination address. */
    DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE = (0),

    /* Enables an interrupt request for an extended repeat area overflow on the destination address. */
    DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_ENABLE  = (1)
} dmaca_dari_t;

/* DMAMD.SM[1:0](b15,b14) : Configurable options for Source Address Update Mode */
typedef enum e_dmaca_src_addr_mode
{
    /* = (0 << 14) : Source address is fixed. */
    DMACA_SRC_ADDR_FIXED  = (0),

    /* Offset addition */
    DMACA_SRC_ADDR_OFFSET = (1 << 14),

    /* Source address is incremented after each transfer. */
    DMACA_SRC_ADDR_INCR   = (2 << 14),

    /* Source address is decremented after each transfer. */
    DMACA_SRC_ADDR_DECR   = (3 << 14)
} dmaca_src_addr_mode_t;

/* DMAMD.SARA[4:0](b12,b8) : Configurable options for Source Address Extended Repeat Area */
typedef enum e_dmaca_src_addr_repeat_area
{
    DMACA_SRC_ADDR_EXT_REP_AREA_NONE   = (0),                  /* = (0 << 8): Not specified */
    DMACA_SRC_ADDR_EXT_REP_AREA_2B    = (1 << 8),              /*    2 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_4B    = (2 << 8),              /*    4 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_8B    = (3 << 8),              /*    8 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_16B   = (4 << 8),              /*   16 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_32B   = (5 << 8),              /*   32 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_64B   = (6 << 8),              /*   64 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_128B  = (7 << 8),              /*  128 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_256B  = (8 << 8),              /*  256 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_512B  = (9 << 8),              /*  512 bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_1KB   = (10 << 8),             /*   1K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_2KB   = (11 << 8),             /*   2K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_4KB   = (12 << 8),             /*   4K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_8KB   = (13 << 8),             /*   8K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_16KB  = (14 << 8),             /*  16K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_32KB  = (15 << 8),             /*  32K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_64KB  = (16 << 8),             /*  64K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_128KB = (17 << 8),             /* 128K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_256KB = (18 << 8),             /* 256K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_512KB = (19 << 8),             /* 512K bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_1MB   = (20 << 8),             /*   1M bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_2MB   = (21 << 8),             /*   2M bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_4MB   = (22 << 8),             /*   4M bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_8MB   = (23 << 8),             /*   8M bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_16MB  = (24 << 8),             /*  16M bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_32MB  = (25 << 8),             /*  32M bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_64MB  = (26 << 8),             /*  64M bytes specified as extended repeat area */
    DMACA_SRC_ADDR_EXT_REP_AREA_128MB = (27 << 8)              /* 128M bytes specified as extended repeat area */
} dmaca_src_addr_repeat_area_t;

/* ---- DMAMD.DM[1:0](b7,b6) : Configurable options for Destination Address Update Mode ---- */
typedef enum e_dmaca_des_addr_mode
{
    /* = (0 << 6) : Destination address is fixed. */
    DMACA_DES_ADDR_FIXED =  (0),

    /* Offset addition */
    DMACA_DES_ADDR_OFFSET = (1 << 6),

    /* Destination address is incremented after each transfer. */
    DMACA_DES_ADDR_INCR  =  (2 << 6),

    /* Destination address is decremented after each transfer. */
    DMACA_DES_ADDR_DECR  =  (3 << 6)
} dmaca_des_addr_mode_t;

/* DMAMD.DARA[4:0](b4,b0) : Configurable options for Destination Address Extended Repeat Area */
typedef enum e_dmaca_des_addr_repeat_area
{
    DMACA_DES_ADDR_EXT_REP_AREA_NONE,                         /* Not specified */
    DMACA_DES_ADDR_EXT_REP_AREA_2B,                           /*    2 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_4B,                           /*    4 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_8B,                           /*    8 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_16B,                          /*   16 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_32B,                          /*   32 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_64B,                          /*   64 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_128,                          /*  128 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_256B,                         /*  256 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_512B,                         /*  512 bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_1KB,                          /*   1K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_2KB,                          /*   2K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_4KB,                          /*   4K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_8KB,                          /*   8K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_16KB,                         /*  16K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_32KB,                         /*  32K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_64KB,                         /*  64K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_128KB,                        /* 128K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_256KB,                        /* 256K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_512KB,                        /* 512K bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_1MB,                          /*   1M bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_2MB,                          /*   2M bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_4MB,                          /*   4M bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_8MB,                          /*   8M bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_16MB,                         /*  16M bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_32MB,                         /*  32M bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_64MB,                         /*  64M bytes specified as extended repeat area */
    DMACA_DES_ADDR_EXT_REP_AREA_128MB                         /* 128M bytes specified as extended repeat area */
} dmaca_des_addr_repeat_area_t;

/* DMCSL.DISEL(b0) : Configurable options for Interrupt Select */
typedef enum e_dmaca_interrupt_select
{
    /* At the beginning of transfer, clears the interrupt flag of the activation source to 0. */
    DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER  = (0),

    /* At the end of transfer, the interrupt flag of the activation source issues an interrupt to the CPU. */
    DMACA_ISSUES_INTERRUPT_TO_CPU_END_OF_TRANSFER = (1)
} dmaca_interrupt_select_t;

/* Enumerate list that can be selected as DMAC activation source */
/* enum enum_ir is included from iodefine.h.  */
typedef enum enum_ir dmaca_activation_source_t;

typedef enum e_dmaca_command
{
    DMACA_CMD_ENABLE = 0,                                   /* Enables DMA transfer. */
    DMACA_CMD_ALL_ENABLE,                                   /* Enables DMAC activation. */
    DMACA_CMD_RESUME,                                       /* Resumes DMA transfer. */
    DMACA_CMD_DISABLE,                                      /* Enables DMA transfer. */
    DMACA_CMD_ALL_DISABLE,                                  /* Disables DMAC activation. */
    DMACA_CMD_SOFT_REQ_WITH_AUTO_CLR_REQ,                   /* SWREQ bit is cleared automatically after DMA transfer. */
    DMACA_CMD_SOFT_REQ_NOT_CLR_REQ,                         /* SWREQ bit is not cleared after DMA transfer. */
    DMACA_CMD_SOFT_REQ_CLR,                                 /* Clears DMACA Software request flag. */
    DMACA_CMD_STATUS_GET,                                   /* Gets the current status of DMACA. */
    DMACA_CMD_ESIF_STATUS_CLR,                              /* Clears Transfer Escape End Interrupt Flag. */
    DMACA_CMD_DTIF_STATUS_CLR                               /* Clears Transfer Interrupt Flag. */
} dmaca_command_t;

typedef enum e_dmaca_return
{
    DMACA_SUCCESS_OTHER_CH_BUSY = 0,                        /* Other DMAC channels are locked, */

                                                            /*  so that cannot set to module stop state. */
    DMACA_SUCCESS_DTC_BUSY,                                 /* DTC is locked so that cannot set to module stop state. */
    DMACA_SUCCESS,

    DMACA_ERR_INVALID_CH,                                   /* Channel is invalid. */
    DMACA_ERR_INVALID_ARG,                                  /* Parameters are invalid. */
    DMACA_ERR_INVALID_HANDLER_ADDR,                         /* Invalid function address is set, */

                                                            /*  and any previous function has been unregistered. */
    DMACA_ERR_INVALID_COMMAND,                              /* Command is invalid. */
    DMACA_ERR_NULL_PTR,                                     /* Argument pointers are NULL. */
    DMACA_ERR_BUSY,                                         /* Resource has been locked by other process. */
    DMACA_ERR_SOFTWARE_REQUESTED,                           /* DMA transfer request by software has been */

                                                            /*  generated already, so that cannot execute command. */
    DMACA_ERR_SOFTWARE_REQUEST_DISABLED,                    /* Transfer Request Source is not Software. */
    DMACA_ERR_INTERNAL                                      /* DMACA Driver internal error */
 } dmaca_return_t;

typedef struct st_dmaca_stat
{
   bool  soft_req_stat;                                     /* Software Request Status */
   bool  esif_stat;                                         /* Transfer Escape End Interrupt Status */
   bool  dtif_stat;                                         /* Transfer End Interrupt Status */
   bool  act_stat;                                          /* Active Flag of DMAC */
   uint32_t transfer_count;                                 /* Transfer Count */
} dmaca_stat_t;

/* Transfer data configuration */
typedef struct st_dmaca_transfer_data_cfg
{
    dmaca_transfer_mode_t           transfer_mode;          /* Transfer Mode */
    dmaca_repeat_block_side_t       repeat_block_side;      /* Repeat Area in Repeat or Block Transfer Mode */
    dmaca_data_size_t               data_size;              /* Transfer Data Size */
    dmaca_activation_source_t       act_source;             /* Activation Source */
    dmaca_request_source_t          request_source;         /* Transfer Request Source */
    dmaca_dti_t                     dtie_request;           /* Transfer End Interrupt Request */
    dmaca_esi_t                     esie_request;           /* Transfer Escape End Interrupt Request */
    dmaca_rpti_t                    rptie_request;          /* Repeat Size End Interrupt Request */
    dmaca_sari_t                    sarie_request;          /* Source Address Extended Repeat Area */

                                                            /* Overflow Interrupt Request */
    dmaca_dari_t                    darie_request;          /* Destination Address Extended Repeat Area */

                                                            /* Overflow Interrupt Request */
    dmaca_src_addr_mode_t           src_addr_mode;          /* Address Mode of Source */
    dmaca_src_addr_repeat_area_t    src_addr_repeat_area;   /* Source Address Extended Repeat Area */
    dmaca_des_addr_mode_t           des_addr_mode;          /* Address Mode of Destination */
    dmaca_des_addr_repeat_area_t    des_addr_repeat_area;   /* Destination Address Extended Repeat Area */
    uint32_t                        offset_value;           /* Offset value for DMA Offset Register (DMOFR) */
    dmaca_interrupt_select_t        interrupt_sel;          /* Configurable Options for Interrupt Select */
    void                            *p_src_addr;            /* Start Address of Source */
    void                            *p_des_addr;            /* Start Address of Destination */
    uint32_t                        transfer_count;         /* Transfer Count */
    uint16_t                        block_size;             /* Repeat Size or Block Size */
    uint8_t  rsv[2];
} dmaca_transfer_data_cfg_t;

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
dmaca_return_t R_DMACA_Open(uint8_t channel);
dmaca_return_t R_DMACA_Create(uint8_t channel, dmaca_transfer_data_cfg_t *p_data_cfg);
dmaca_return_t R_DMACA_Close(uint8_t channel);
dmaca_return_t R_DMACA_Control(uint8_t channel, dmaca_command_t  command, dmaca_stat_t *p_stat);
dmaca_return_t R_DMACA_Int_Enable(uint8_t channel, uint8_t  priority);
dmaca_return_t R_DMACA_Int_Disable(uint8_t channel);
uint32_t       R_DMACA_GetVersion(void);
void           R_DMACA_Init(void);
dmaca_return_t R_DMACA_Int_Callback(uint8_t channel, void *callback);

#endif /* DMACA_RX_IF_H */

/* End of File */

