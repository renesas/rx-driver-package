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
* Copyright (C) 2018(2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_flash_dm_rx_if.c
* Version      : 2.01
* Description  : DATFRX interface source file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*              : 25.01.2019 2.01     English PDF added, Fixed blank check processing and Busy check procedure
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_flash_dm_rx_if.h"
#include "r_flash_dm_private.h"

#if(FLASH_TYPE == FLASH_TYPE_1)
#include "r_dispatch_1.h"
#else
#include "r_dispatch_2.h"
#endif

/************************************************************************************************
Macro definitions
*************************************************************************************************/

/************************************************************************************************
Local Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/

/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
/* narrow APIs*/
#if(FLASH_TYPE == FLASH_TYPE_1)
static e_flash_dm_status_t r_flash_dm_d1_init(void);
static e_flash_dm_status_t r_flash_dm_d1_read(st_flash_dm_info_t * p_flash_dm_info);
static e_flash_dm_status_t r_flash_dm_d1_write(st_flash_dm_info_t * p_flash_dm_info);
static e_flash_dm_status_t r_flash_dm_d1_erase(void);
static e_flash_dm_status_t r_flash_dm_d1_control(e_flash_dm_cmd_t cmd, uint32_t* pcfg);
static e_flash_dm_status_t r_flash_dm_d1_format(void);
static e_flash_dm_status_t r_flash_dm_1_get_writable_size(uint32_t* pcfg);

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
static e_flash_dm_status_t r_flash_dm_cfprotect(uint32_t protect);
static e_flash_dm_status_t r_flash_dm_cfromcache(uint8_t rom_cache);

#endif /* FLASH_CFG_CODE_FLASH_ENABLE */

static st_flash_dispatch_1_hndl_t* (g_flash_dm_handle);

#else /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
static e_flash_dm_status_t r_flash_dm_d2_init(void);
static e_flash_dm_status_t r_flash_dm_d2_read(st_flash_dm_info_t * p_flash_dm_info);
static e_flash_dm_status_t r_flash_dm_d2_write(st_flash_dm_info_t * p_flash_dm_info);
static e_flash_dm_status_t r_flash_dm_d2_erase(void);
static e_flash_dm_status_t r_flash_dm_d2_control(e_flash_dm_cmd_t cmd, uint32_t* pcfg);
static e_flash_dm_status_t r_flash_dm_d2_format(void);
static e_flash_dm_status_t r_flash_dm_2_get_writable_size(uint32_t*);

static st_flash_dispatch_2_hndl_t* (g_flash_dm_handle);
#endif/* FLASH_TYPE */

/* common APIs */
static e_flash_dm_status_t r_flash_dm_get_writable_size(uint32_t*);
static e_flash_dm_status_t r_flash_dm_getactivity(uint32_t*);
static e_flash_dm_status_t r_flash_dm_getdatasize(uint32_t*);
static e_flash_dm_status_t r_flash_dm_getdatanum(uint32_t*);
static uint8_t r_flash_dm_drvif_close(void);

#if(FLASH_TYPE == FLASH_TYPE_2)
extern void r_flash_dm_disable_rw(void);
extern void r_flash_dm_enable_rw(void);
#endif

/* user callback */
p_flash_dm_callback r_flash_dm_callbackfunc;

/***************************************************************************************************
* Function Name: R_FLASH_DM_Open
* Description  : Initializes the DATFRX work memory.
* Arguments    : uint32_t * p_flash_dm_work ; Work memory
*              : p_flash_dm_callback func   ; Callback function for end processing
*              :                            ; Processing are format, write, erase, reclaim and error.
* Return Value : FLASH_DM_SUCCESS           ; Successful operation.
*              : FLASH_DM_ERR_ARGUMENT      ; Failed operation of parameter.
*              : FLASH_DM_ERR_OPEN          ; Failed operation of driver interface/
*--------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, must place the source code to the ROM.
*              : (Prohibit the placement to RAM)
***************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Open(uint32_t* p_flash_dm_work, p_flash_dm_callback func)
{
    uint32_t * p_ptr = 0;
    uint32_t i = 0;
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    /* Check code flush and data flag setting. */
#if FLASH_CFG_DATA_FLASH_BGO == 0
#error "ERROR in r_flash_rx_config.h configuration. Must ENABLE FLASH_CFG_DATA_FLASH_BGO."
#endif

    /*  Initialize the work memory. */
    if (0 == (uint32_t)p_flash_dm_work)
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    /* Open driver interface. */
    else if (!r_flash_dm_drvif_open())
    {
        ret = FLASH_DM_ERR_OPEN;
    }
    else
    {
#if(FLASH_TYPE == FLASH_TYPE_1)
        /* cast from uint32_t* to st_flash_dispatch_1_hndl_t */
        g_flash_dm_handle = (st_flash_dispatch_1_hndl_t *)p_flash_dm_work;
#else
        /* cast from uint32_t* to st_flash_dispatch_2_hndl_t* */
        g_flash_dm_handle = (st_flash_dispatch_2_hndl_t *)p_flash_dm_work;
#endif

        /* ---- Clear the work memory to '0'. --- */
        p_ptr = (uint32_t *)g_flash_dm_handle;
#if(FLASH_TYPE == FLASH_TYPE_1)
        /* WAIT_LOOP */
        for (i = (sizeof(st_flash_dispatch_1_hndl_t) / sizeof(uint32_t)); i > 0; i--)
#else
        /* WAIT_LOOP */
        for (i = (sizeof(st_flash_dispatch_2_hndl_t) / sizeof(uint32_t)); i > 0; i--)
#endif
        {
            *p_ptr++ = 0;
        }

        /* Register the callback function. */
        r_flash_dm_callbackfunc = 0;
        if (0 != func)
        {
            r_flash_dm_callbackfunc = func;
        }

        /* Pass address to dm */
        r_flash_dm_set_dm((uint32_t)g_flash_dm_handle);

        /* Pass address to dispatch */
        r_flash_dm_set_dispatch((uint32_t)g_flash_dm_handle);

#if(FLASH_TYPE == FLASH_TYPE_2)
        /* enable read/write */
        r_flash_dm_enable_rw();
#endif
    }
    return ret;
} /* End of function R_FLASH_DM_Open() */

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)
#pragma section FRAM
#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1) */

/************************************************************************************************
* Function Name: R_FLASH_DM_Close
* Description  : Finishes the DATFRX.
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS       ; Successful operation
*              : FLASH_DM_ERR_CLOSE     ; Failed operation of Close
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Close(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    /* Check Argument. */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_CLOSE;
    }
    /* Release the FLASH handle. */
    else if (!r_flash_dm_drvif_close())
    {
        ret = FLASH_DM_ERR_CLOSE;
    }
    else
    {
        /* Release the FLASH DM handle. */
        g_flash_dm_handle = 0;

#if(FLASH_TYPE == FLASH_TYPE_2)
        /* disable read/write */
        r_flash_dm_disable_rw();
#endif
    }

    return ret;
} /* End of function R_FLASH_DM_Close() */

/************************************************************************************************
* Function Name: R_FLASH_DM_GetVersion
* Description  : Returns the version of DATFRX.
*                The version number is encoded such that the top two bytes are
*                the major version number and the bottom two bytes are the minor
*                version number.
* Arguments    : None
* Return Value : Version number     ; Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t R_FLASH_DM_GetVersion(void)
{
    uint32_t version = (FLASH_DM_VERSION_MAJOR << 16) | FLASH_DM_VERSION_MINOR;

    return version;
} /* End of function R_FLASH_DM_GetVersion() */

/************************************************************************************************
* Function Name: R_FLASH_DM_Init
* Description  : Initializes the device.
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS               ; Successful operation.(Type1)
*              : FLASH_DM_ADVANCE               ; Successful operation, call advanced function.(Type2,3,4)
*              : FLASH_DM_SUCCESS_REQUEST_ERASE ; Successful operation, request erase.
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*              : FLASH_DM_ERR_REQUEST_FORMAT    ; Failed operation and request format.(Type1)
*              : FLASH_DM_ERR_REQUEST_INIT      : Failed operation and request initialization.
*              : FLASH_DM_ERR_INIT              ; Failed operation of Initialize.
*------------------------------------------------------------------------------------------------
* Notes        : Always, Place the source code to the ROM.
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Init(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
#if(FLASH_TYPE == FLASH_TYPE_1)
    ret = r_flash_dm_d1_init();
#else /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
    ret = r_flash_dm_d2_init();
#endif /* FLASH_TYPE */
    return ret;
} /* End of function R_FLASH_DM_Init() */

/************************************************************************************************
* Function Name: R_FLASH_DM_InitAdvance
* Description  : Initializes the device.
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS               ; Successful operation.
*              : FLASH_DM_ADVANCE               ; Successful operation, please call R_FLASH_InitAdvance().
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*              : FLASH_DM_ERR_REQUEST_FORMAT    ; Failed operation and format initialization.
*              : FLASH_DM_ERR_REQUEST_INIT      ; Failed operation and request initialization.
*              : FLASH_DM_ERR_INIT              ; Failed operation of Initialize.
*------------------------------------------------------------------------------------------------
* Notes        : Always, Place the source code to the ROM.
*************************************************************************************************/
#if((FLASH_TYPE == FLASH_TYPE_2) || (FLASH_TYPE == FLASH_TYPE_3) || (FLASH_TYPE == FLASH_TYPE_4))
e_flash_dm_status_t R_FLASH_DM_InitAdvance(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    /* ==== Handle Check ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_INIT;
    }
    /* ==== Check busy. ==== */
    else if (g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        g_flash_dm_handle->activity = FLASH_DM_ACT_INITIALIZING;
        ret = r_flash_dm_1st_check(FLASH_DM_INIT_1ST_CHECK);

        /* If the DM is performing or the activity is not idle, do not initialize. */
        if(FLASH_DM_SUCCESS == ret)
        {
            /* Check status. */
            g_flash_dm_handle->initialized = 0u;
            ret = r_flash_dm_init_set2();
        }
    }
    g_flash_dm_handle->activity = FLASH_DM_ACT_IDLE;
    g_flash_dm_handle->api_call = tmp;
    return ret;
}/* End of function R_FLASH_DM_InitAdvance() */
#endif/* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */

/************************************************************************************************
* Function Name: R_FLASH_DM_Format
* Description  : Formats the device.
* Arguments    : none
* Return Value : FLASH_DM_ACCEPT            ; Successful operation reception.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Format(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
#if(FLASH_TYPE == FLASH_TYPE_1)
    ret = r_flash_dm_d1_format();
#else /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
    ret = r_flash_dm_d2_format();
#endif /* FLASH_TYPE */
    return ret;
} /* End of function R_FLASH_DM_Format() */

/************************************************************************************************
* Function Name: R_FLASH_DM_Read
* Description  : Reads data from the device.
* Arguments    : st_flash_dm_info_t * p_flash_dm_info
*              :    uint8_t     data_no         ; The data no.
*              :    uint8_t   * p_data          ; Pointer to read data buffer
* Return Value : FLASH_DM_SUCCESS               ; Successful operation.
*              : FLASH_DM_ERR_ARGUMENT          ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*              : FLASH_DM_ERR_DATA_NOT_PRESENT  ; Failed operation of the data no. to support.(TYPE1)
*              : FLASH_DM_ERR_REQUEST_INIT      ; Failed operation and request initialization.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Read(st_flash_dm_info_t * p_flash_dm_info)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
#if(FLASH_TYPE == FLASH_TYPE_1)
    ret = r_flash_dm_d1_read(p_flash_dm_info);
#else /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
    ret = r_flash_dm_d2_read(p_flash_dm_info);
#endif /* FLASH_TYPE */
    return ret;
} /* End of function R_FLASH_DM_Read() */

/************************************************************************************************
* Function Name: R_FLASH_DM_Write
* Description  : Writes data to the device.
* Arguments    : st_flash_dm_info_t * p_flash_dm_info
*              :    uint8_t         data_no     ; The data no.
*              :    uint8_t       * p_data      ; Pointer to write data buffer
* Return Value : FLASH_DM_ACCEPT                ; Successful operation reception.
*              : FLASH_DM_ERR_REQUEST_INIT      ; Failed operation and request initialization.
*              : FLASH_DM_ERR_ARGUMENT          ; Failed operation of parameter.
*              : FLASH_DM_ERR_REQUEST_ERASE     ; Failed operation and request erase.(TYPE,2,3,4)
*              : FLASH_DM_ERR_REQUEST_RECLAIM   ; Failed operation and request reclaim.(TYPE1)
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Write(st_flash_dm_info_t * p_flash_dm_info)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
#if(FLASH_TYPE == FLASH_TYPE_1)
    ret = r_flash_dm_d1_write(p_flash_dm_info);
#else /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
    ret = r_flash_dm_d2_write(p_flash_dm_info);
#endif /* FLASH_TYPE */
    return ret;
} /* End of function R_FLASH_DM_Write() */

/************************************************************************************************
* Function Name: R_FLASH_DM_Erase
* Description  : Erases a garbage block.
* Arguments    : None
* Return Value : FLASH_DM_ACCEPT            ; Successful operation reception.
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization.
*              : FLASH_DM_NO_INVALID_BLOCK  ; Successful operation no invalid block.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*              : FLASH_DM_ERR_ERASE         ; Failed erase.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Erase(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
#if(FLASH_TYPE == FLASH_TYPE_1)
    ret = r_flash_dm_d1_erase();
#else /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
    ret = r_flash_dm_d2_erase();
#endif /* FLASH_TYPE */
    return ret;
} /* End of function R_FLASH_DM_Erase() */

/************************************************************************************************
* Function Name: R_FLASH_DM_Control
* Description  : Define various functions.
* Arguments    : e_flash_dm_cmd_t   cmd     ; Control command.
*              : uint32_t*          pcfg    ; Work memory.
* Return Value : FLASH_DM_SUCCESS           ; Successful operation.
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization.
*              : FLASH_DM_ERR_ARGUMENT      ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*              : FLASH_DM_ERR               ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Control(e_flash_dm_cmd_t cmd, uint32_t* pcfg)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
#if(FLASH_TYPE == FLASH_TYPE_1)
    ret = r_flash_dm_d1_control(cmd, pcfg);
#else /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
    ret = r_flash_dm_d2_control(cmd, pcfg);
#endif /* FLASH_TYPE */
    return ret;
} /* End of function R_FLASH_DM_Control() */

/************************************************************************************************
* Function Name: r_flash_dm_get_writable_size
* Description  : Acquires the size of free space
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS               ; Successful operation, but no free area.
*              : FLASH_DM_ERR_REQUEST_INIT      ; Failed operation and request initialization.(TYPE1)
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*              : FLASH_DM_ERR_ARGUMENT          ; Failed operation of parameter.(TYPE1)
*              : FLASH_DM_ERR                   ; Failed operation.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_get_writable_size(uint32_t* pcfg)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
#if(FLASH_TYPE == FLASH_TYPE_1)
    ret = r_flash_dm_1_get_writable_size(pcfg);
#else /* FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4 */
    ret = r_flash_dm_2_get_writable_size(pcfg);
#endif /* FLASH_TYPE */
    return ret;
} /* End of function r_flash_dm_get_writable_size() */


#if(FLASH_TYPE == FLASH_TYPE_1)
/************************************************************************************************
* Function Name: R_FLASH_DM_Reclaim
* Description  : Switches active block.
* Arguments    : None
* Return Value : FLASH_DM_ACCEPT            ; Successful operation reception.
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization.
*              : FLASH_DM_ERR_REQUEST_ERASE ; Failed operation and request erase.
*              : FLASH_DM_ERR_CANT_RECLAIM  ; Failed operation and cannot reclaim.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t R_FLASH_DM_Reclaim(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;
    uint8_t uc_tmp = 0;

    /* ==== Check Argument. ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_RECLAIM;
    }
    /* ==== Check busy. ==== */
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    /* ==== Detect the erase block. ==== */
    else if (0 != g_flash_dm_handle->p_reclaim )
    {
        ret = FLASH_DM_ERR_REQUEST_ERASE;
    }
    else
    {
        /* ==== Check status. ==== */
        ret = r_flash_dm_1st_check(FLASH_DM_RECLAIM_1ST_CHECK);
        if (FLASH_DM_ERR_BUSY == ret)
        {
            ret = FLASH_DM_ERR_BUSY;
        }
        else if(FLASH_DM_ERR_REQUEST_INIT == ret)
        {
            ret = FLASH_DM_ERR_REQUEST_INIT;
        }
        else
        {
            /* ==== Status Check. ==== */
            g_flash_dm_handle->p_new_active = r_flash_dm_drvif_get_block_info(g_flash_dm_handle->p_active->next);
            if (FLASH_DM_BLOCK_STATUS_READY != g_flash_dm_handle->p_new_active->stat)
            {
                ret = FLASH_DM_ERR_CANT_RECLAIM;
            }
            else
            {
                /* ==== Check if this module is processing callback. ==== */
                if(g_flash_dm_handle->flash_state == DATF_FLASH_CALL)
                {
                    ret = FLASH_DM_ERR_BUSY;
                    /* ==== Start to reclaim. ==== */
                }
                else
                {
                    /* cast from uint8_t to uint8_t */
                    uc_tmp = (g_flash_dm_handle->activity | FLASH_DM_ACT_RECLAIMING);
                    g_flash_dm_handle->activity = uc_tmp;
                    ret = r_flash_dm_dispatch(FLASH_DM_USER_RECLAIM_SETUP);
                }
            }
        }
    }

    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function R_FLASH_DM_Reclaim() */
#endif /* FLASH_TYPE */

#if(FLASH_TYPE == FLASH_TYPE_1)
/************************************************************************************************
* Function Name: r_flash_dm_d1_init
* Description  : Initializes the device.
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS               ; Successful operation.
*              : FLASH_DM_SUCCESS_REQUEST_ERASE ; Successful operation, request erase.
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*              : FLASH_DM_ERR_REQUEST_FORMAT    ; Failed operation and request format.
*              : FLASH_DM_ERR_REQUEST_INIT      : Failed operation and request initialization.
*              : FLASH_DM_ERR_INIT              ; Failed operation of Initialize.
*------------------------------------------------------------------------------------------------
* Notes        : Always, Place the source code to the ROM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d1_init(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    flash_dm_block_ptr_t p_block = 0;
    flash_dm_bh_ptr_t p_bh = 0;
    flash_dm_dh_ptr_t p_dh = 0;
    uint16_t i = 0;
    uint16_t j = 0;
#if defined(FLASH_DM_CHECK_REJECTED_BLOCK)
    uint16_t k = 0;
    bool ret_bool = true;
#endif /* defined(FLASH_DM_CHECK_REJECTED_BLOCK) */
    uint16_t dp = 0;
    uint8_t tmp = g_flash_dm_handle->api_call;
    uint8_t nblock_full = 0;
    uint32_t addr = 0;

    /* ==== Handle Check ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_INIT;
    }
    /* ==== Check busy. ==== */
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        g_flash_dm_handle->activity = FLASH_DM_ACT_INITIALIZING;
        /* ==== If the FLASH DM is performing or the activity is not idle, do not initialize. ==== */
        ret = r_flash_dm_1st_check(FLASH_DM_INIT_1ST_CHECK);
        if(FLASH_DM_ERR_BUSY == ret)
        {
            ret = FLASH_DM_ERR_BUSY;
        }
        else if(FLASH_DM_ERR_REQUEST_INIT == ret)
        {
            ret = FLASH_DM_ERR_REQUEST_INIT;
            /* ==== If the activity is not idle, do not initialize. ==== */
        }
        else
        {
            /* ==== Initialize the device. ==== */
            if (r_flash_dm_init_env(FLASH_DM_INITIALIZED_ID, FLASH_DM_ACT_INITIALIZING) == 0)
            {
                ret = FLASH_DM_ERR_REQUEST_INIT;
            }
            else
            {
                /* WAIT_LOOP */
                do
                {
                    /* ==== Check the block header. ==== */
                    p_block = r_flash_dm_drvif_get_block_info(0);
                    nblock_full = r_flash_dm_drvif_get_block_num();
                    /* WAIT_LOOP */
                    for (i = 0; i < nblock_full; i++, p_block++)
                    {
                        /* The block header address of the block which p_block points at. */
                        p_bh = &p_block->p_img->h.b;

                        /* ---- Check the erase completion flag. ---- */
#if defined(FLASH_DM_CHECK_REJECTED_BLOCK)
                        k = 0;
#endif /* defined(FLASH_DM_CHECK_REJECTED_BLOCK) */
                        /* WAIT_LOOP */
                        for (j = 0; j < FLASH_DM_BH_ERASE_FLAG_INDEX; j++)
                        {
                            /* cast from uint8_t to uint8_t */
                            if (FLASH_DM_BH_FLAG_AA != p_bh->erase_end[j])
                            {
                                /* The block which is targeted for erase. */
                                p_block->stat = FLASH_DM_BLOCK_STATUS_GARBAGE;
                            }
#if defined(FLASH_DM_CHECK_REJECTED_BLOCK)
                            /* Check the rejected block. */
                            if (FLASH_DM_BH_FLAG_55 == p_bh->erase_end[j])
                            {
                                k++;
                                if (FLASH_DM_BH_ERASE_FLAG_INDEX == k)
                                {
                                    r_flash_dm_reject_block(i);
                                }
                            }
#endif /* defined(FLASH_DM_CHECK_REJECTED_BLOCK) */
                        }

                        /* ---- Check the block header flags. ---- */
                        if ((FLASH_DM_BLOCK_STATUS_GARBAGE == p_block->stat) ||
                                (FLASH_DM_BLOCK_STATUS_BAD == p_block->stat))
                        {
                            /* Do nothing   */
                        }
                        /* cast from uint8_t to uint8_t */
                        else if (FLASH_DM_BH_FLAG_FF == p_bh->ready[0])
                        {
                            p_block->stat = FLASH_DM_BLOCK_STATUS_GARBAGE;
                        }
                        /* cast from uint8_t to uint8_t */
                        else if (FLASH_DM_BH_FLAG_FF != p_bh->erase_start[0])
                        {
                            r_flash_dm_drvif_overwrite_block_flag(&p_bh->erase_start[0]);
                            p_block->stat = FLASH_DM_BLOCK_STATUS_GARBAGE;
                        }
                        /* cast from uint8_t to uint8_t */
                        else if (FLASH_DM_BH_FLAG_FF == p_bh->active[0])
                        {
                            r_flash_dm_drvif_overwrite_block_flag(&p_bh->ready[0]);
                            /* The block which has been initialized */
                            p_block->stat = FLASH_DM_BLOCK_STATUS_READY;
                        }
                        /* cast from uint8_t to uint8_t */
                        else if (FLASH_DM_BH_FLAG_FF == p_bh->full[0])
                        {
                            r_flash_dm_drvif_overwrite_block_flag(&p_bh->active[0]);
                            if (0 != g_flash_dm_handle->p_active)   /* There are two active blocks. */
                            {
                                ret = FLASH_DM_ERR_REQUEST_FORMAT;
                                break;
                            }
                            else
                            {
                                g_flash_dm_handle->p_active = p_block;
                            }
                        }
                        /* cast from uint8_t to uint8_t */
                        else if (FLASH_DM_BH_FLAG_FF == p_bh->reclaim[0])
                        {
                            r_flash_dm_drvif_overwrite_block_flag(&p_bh->full[0]);
                            if (0 != g_flash_dm_handle->p_full)     /* There are two full blocks. */
                            {
                                ret = FLASH_DM_ERR_REQUEST_FORMAT;
                                break;
                            }
                            else
                            {
                                g_flash_dm_handle->p_full = p_block;
                            }
                        }
                        else
                        {
                            r_flash_dm_drvif_overwrite_block_flag(&p_bh->reclaim[0]);
                            p_block->stat = FLASH_DM_BLOCK_STATUS_RECLAIM;
                        }
                    }

                    /* ==== Check the block state. ==== */
                    if (0 == g_flash_dm_handle->p_full)
                    {
                        /* There are not the active block and the full block. */
                        if (0 == g_flash_dm_handle->p_active)
                        {
                            ret = FLASH_DM_ERR_REQUEST_FORMAT;
                        }
                    }
                    else
                    {
                        if (0 != g_flash_dm_handle->p_active)   /* There are the active block and the full block. */
                        {
                            /* Judge the power shutdown because state is reclaiming. */
                            g_flash_dm_handle->p_active->stat = FLASH_DM_BLOCK_STATUS_GARBAGE;
                        }
                        g_flash_dm_handle->p_active = g_flash_dm_handle->p_full;
                        g_flash_dm_handle->p_full = 0;
                    }

                    /* ---- Check the position of the block which has been initialized. ---- */
                    if (FLASH_DM_SUCCESS == ret)
                    {
                        p_block = r_flash_dm_drvif_get_block_info(0);
                        nblock_full = r_flash_dm_drvif_get_block_num();
                        /* WAIT_LOOP */
                        for (i = 0; i < nblock_full; i++, p_block++)
                        {
                            if (FLASH_DM_BLOCK_STATUS_READY == r_flash_dm_drvif_get_block_info(p_block->index)->stat)
                            {
                                /* Check the active block of previous the block which has been initialized. */
                                if (r_flash_dm_drvif_get_block_info(p_block->prev) != g_flash_dm_handle->p_active)
                                {
                                    /* The block which has been initialized is error. */
                                    ret = FLASH_DM_ERR_REQUEST_FORMAT;
                                    break;
                                }
                            }
                        }
                    }

                    /* Check the block error. */
                    if (FLASH_DM_SUCCESS != ret)
                    {
                        g_flash_dm_handle->initialized = 0;
                        /* cast from uint8_t to uint8_t */
                        g_flash_dm_handle->activity = FLASH_DM_ACT_IDLE;
                        break;  /* Finish to initialize. */
                    }
                    g_flash_dm_handle->p_active->stat = FLASH_DM_BLOCK_STATUS_ACTIVE;

                    /* ==== Check the garbage block. ==== */
                    if (r_flash_dm_drvif_get_block_info(g_flash_dm_handle->p_active->next)->stat !=
                            FLASH_DM_BLOCK_STATUS_READY)
                    {
                        /* If the next block of the active block is not the block which has been initialized. */
                        r_flash_dm_drvif_get_block_info(g_flash_dm_handle->p_active->next)->stat =
                            FLASH_DM_BLOCK_STATUS_GARBAGE;
                    }

                    /* ==== Make data index. ==== */
                    ret = FLASH_DM_SUCCESS;

                    /* ---- Detect the oldest reclaim block. ---- */
                    p_block = g_flash_dm_handle->p_active;
                    g_flash_dm_handle->data_top = p_block->index;
                    /* WAIT_LOOP */
                    for (i = 0; i < g_flash_dm_handle->nblock; i++)
                    {
                        if (FLASH_DM_BLOCK_STATUS_GARBAGE == p_block->stat)
                        {
                            g_flash_dm_handle->p_reclaim = p_block;
                            ret = FLASH_DM_SUCCESS_REQUEST_ERASE;
                        }
                        else if (FLASH_DM_BLOCK_STATUS_RECLAIM == p_block->stat)
                        {
                            g_flash_dm_handle->data_top = p_block->index;
                        }
                        else
                        {
                            /* Do nothing */
                        }
                        p_block = r_flash_dm_drvif_get_block_info(p_block->prev);
                    }

                    /* Pointer to the oldest reclaim block */
                    p_block = r_flash_dm_drvif_get_block_info(g_flash_dm_handle->data_top);

                    /* ---- Make data index. ---- */
                    /* WAIT_LOOP */
                    while(1)
                    {
                        p_dh = p_block->p_img->h.d;    /* Address of data header. */
                        /* Last address of the block */
                        dp = (uint16_t)((uint32_t)&p_block->p_img->raw[FLASH_DM_FLASH_BLOCK_SIZE]);
                        g_flash_dm_handle->header_index = 0;
                        /* cast from uint32_t to uint16_t */
                        for (i = 0; (uint16_t)((uint32_t)&p_dh[0]) < dp; p_dh++, i++)
                        {
                            /* Check the flag of header update start. */
                            if (FLASH_DM_DH_FLAG_FF != p_dh->header_exist[0])
                            {
                                g_flash_dm_handle->header_index = i + 1;

                                /* Check the flag of header update start. */
                                if (FLASH_DM_DH_EXIST_FLAG == p_dh->header_exist[0])
                                {
                                    /* Check the flag of header update end. */
                                    if (FLASH_DM_DH_VALIDATE_FLAG == p_dh->header_valid[0])
                                    {
                                        /* Check the valid address. */
                                        addr = r_flash_dm_drvif_get_addr(p_dh);
                                        /* cast from uint32_t to uint32_t */
                                        if ((addr & FLASH_DM_BLOCK_MAX_MASK) == (uint32_t)p_block->p_img)
                                        {
                                            dp = p_dh->addr;
                                        }

                                        /* Check the flag of data update completion. */
                                        /* cast from uint8_t to uint8_t */
                                        if ((FLASH_DM_DH_VALID_FLAG == p_dh->data_valid[0]) &&
                                                /* cast from uint8_t to uint8_t */
                                                ((((p_dh->validity[0] & FLASH_DM_DH_INVALID_FLAG) \
                                                   /* cast from uint8_t to uint8_t */
                                                   != FLASH_DM_DH_INVALID_FLAG) && \
                                                  /* cast from uint8_t to uint8_t */
                                                  ((p_dh->validity[0] & FLASH_DM_DH_VALIDITY_FLAG) \
                                                   /* cast from uint8_t to uint8_t */
                                                   == FLASH_DM_DH_VALIDITY_FLAG)) && \
                                                 (p_dh->no <= r_flash_dm_drvif_get_datanum())))
                                        {
                                            /* If the data is valid, update the index. */
                                            addr = r_flash_dm_drvif_get_addr(p_dh);
                                            /* cast from uint32_t to uint8_t */
                                            r_flash_dm_drvif_set_dataaddr(p_dh->no, (flash_dm_ptr_t)(addr));
                                        }
                                        else
                                        {
                                            /* Do nothing. */
                                        }
                                    }
                                    else
                                    {
                                        /* Do nothing. */
                                    }
                                }
                            }
                            else
                            {
                                /* Do nothing. */
                            }
                        }

                        /* Check completion to the active block. */
                        if (p_block == g_flash_dm_handle->p_active)
                        {
                            break;
                        }

                        /* Pointer to the next block. */
                        p_block = r_flash_dm_drvif_get_block_info(p_block->next);
                    }

                    /* cast from uint32_t to uint8_t */
                    g_flash_dm_handle->p_data = (flash_dm_ptr_t)dp;

                    /* ---- Finish the Initialization. ---- */
                    g_flash_dm_handle->activity = FLASH_DM_ACT_IDLE;
                    /* cast from uint8_t to uint8_t */
                    g_flash_dm_handle->initialized = FLASH_DM_INITIALIZED_ID;
                }
                while(0);
            }
        }
        g_flash_dm_handle->activity = FLASH_DM_ACT_IDLE;
    }

    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d1_init() */

/************************************************************************************************
* Function Name: r_flash_dm_d1_read
* Description  : Reads data from the device.
* Arguments    : st_flash_dm_info_t * p_flash_dm_info
*              :    uint8_t     data_no         ; The data no.
*              :    uint8_t   * p_data          ; Pointer to read data buffer
* Return Value : FLASH_DM_SUCCESS               ; Successful operation.
*              : FLASH_DM_ERR_ARGUMENT          ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*              : FLASH_DM_ERR_DATA_NOT_PRESENT  ; Failed operation of the data no. to support.
*              : FLASH_DM_ERR_REQUEST_INIT      ; Failed operation and request initialization.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d1_read(st_flash_dm_info_t * p_flash_dm_info)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    /* ==== Check the parameter. ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    /* ==== Check argument === */
    else if ((p_flash_dm_info->data_no >= r_flash_dm_drvif_get_datanum()) ||
             (0 == p_flash_dm_info->p_data))
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    /* ==== Check busy. ==== */
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        /* ==== Check status. ==== */
        ret = r_flash_dm_1st_check(FLASH_DM_READ_1ST_CHECK);

        if((FLASH_DM_ERR_BUSY == ret) || (FLASH_DM_ERR_REQUEST_INIT == ret))
        {
            /* Do Nothing */
        }
        else if (0 == r_flash_dm_drvif_get_dataaddr(p_flash_dm_info->data_no))
        {
            ret = FLASH_DM_ERR_DATA_NOT_PRESENT;
        }
        else
        {
            /* cast from uint8_t to void* */
            r_flash_dm_memcopy((void *)p_flash_dm_info->p_data
                               /* cast from uint8_t to void* */
                               , (void *)r_flash_dm_drvif_get_dataaddr(p_flash_dm_info->data_no)
                               , r_flash_dm_drvif_get_datasize(p_flash_dm_info->data_no));
            ret = FLASH_DM_SUCCESS;
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d1_read() */

/************************************************************************************************
* Function Name: r_flash_dm_d1_write
* Description  : Writes data to the device.
* Arguments    : st_flash_dm_info_t * p_flash_dm_info
*              :    uint8_t         data_no     ; The data no.
*              :    uint8_t       * p_data      ; Pointer to write data buffer
* Return Value : FLASH_DM_ACCEPT                ; Successful operation reception.
*              : FLASH_DM_ERR_REQUEST_INIT      ; Failed operation and request initialization.
*              : FLASH_DM_ERR_ARGUMENT          ; Failed operation of parameter.
*              : FLASH_DM_ERR_REQUEST_RECLAIM   ; Failed operation and request reclaim.(TYPE1)
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d1_write(st_flash_dm_info_t * p_flash_dm_info)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;
    uint8_t uc_tmp = 0;
    int16_t free_area = 0;

    /* ==== Check the parameter. ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    else if((p_flash_dm_info->data_no >= r_flash_dm_drvif_get_datanum()) ||
            (0 == p_flash_dm_info->p_data))
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    /* ==== Check busy. ==== */
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        /* ==== Check status. ==== */
        ret = r_flash_dm_1st_check(FLASH_DM_WRITE_1ST_CHECK);

        if((FLASH_DM_ERR_BUSY != ret) && (FLASH_DM_ERR_REQUEST_INIT != ret))
        {
            g_flash_dm_handle->p_user_data = p_flash_dm_info->p_data;
            g_flash_dm_handle->dh.no = p_flash_dm_info->data_no;

            /* ==== Check the blank area. ====*/
            if (FLASH_DM_BLOCK_STATUS_ACTIVE != g_flash_dm_handle->p_active->stat)
            {
                g_flash_dm_handle->error_code = FLASH_DM_ERR_REQUEST_RECLAIM;
                ret = FLASH_DM_ERR_REQUEST_RECLAIM;
            }
            else
            {
                free_area = r_flash_dm_free_area();
                if(free_area <= 0)
                {
                    g_flash_dm_handle->error_code = FLASH_DM_ERR_REQUEST_RECLAIM;
                    ret = FLASH_DM_ERR_REQUEST_RECLAIM;
                }
                else if ((uint16_t)free_area < r_flash_dm_drvif_get_datasize(g_flash_dm_handle->dh.no))
                {
                    g_flash_dm_handle->error_code = FLASH_DM_ERR_REQUEST_RECLAIM;
                    ret = FLASH_DM_ERR_REQUEST_RECLAIM;
                    /* === Check if this module is processing callback. === */
                }
                else if(g_flash_dm_handle->flash_state == DATF_FLASH_CALL)
                {
                    ret = FLASH_DM_ERR_BUSY;
                }
                else
                {
                    /* ==== Start to write the data. ==== */
                    uc_tmp = (g_flash_dm_handle->activity | FLASH_DM_ACT_WRITING);
                    g_flash_dm_handle->activity = uc_tmp;
                    ret = r_flash_dm_dispatch(FLASH_DM_USER_WRITE_SETUP);
                }
            }
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d1_write() */

/************************************************************************************************
* Function Name: r_flash_dm_d1_erase
* Description  : Erases a garbage block.
* Arguments    : None
* Return Value : FLASH_DM_ACCEPT            ; Successful operation reception.
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization.
*              : FLASH_DM_NO_INVALID_BLOCK  ; Successful operation no invalid block.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*              : FLASH_DM_ERR_ERASE         ; Failed erase.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d1_erase(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;
    uint8_t uc_tmp = 0;

    /* ==== Check the parameter. ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_ERASE;
    }
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        /* ==== Check status. ==== */
        ret = r_flash_dm_1st_check(FLASH_DM_ERASE_1ST_CHECK);

        if((FLASH_DM_ERR_BUSY != ret) && (FLASH_DM_ERR_REQUEST_INIT != ret))
        {
            /* ==== Detect the erase block. ==== */
            if (0 == g_flash_dm_handle->p_reclaim)
            {
                ret = FLASH_DM_NO_INVALID_BLOCK;
            }
            else if ((FLASH_DM_BLOCK_STATUS_GARBAGE != g_flash_dm_handle->p_reclaim->stat) &&
                     (FLASH_DM_BLOCK_STATUS_ERROR_GARBAGE != g_flash_dm_handle->p_reclaim->stat))
            {
                ret = FLASH_DM_NO_INVALID_BLOCK;
            }
            else if(g_flash_dm_handle->flash_state == DATF_FLASH_CALL)
                /* === Check if this module is processing callback. === */
            {
                ret = FLASH_DM_ERR_BUSY;
            }
            else
            {
                /* ==== Start to erase of the block. ==== */
                uc_tmp = (g_flash_dm_handle->activity | FLASH_DM_ACT_ERASING);
                g_flash_dm_handle->activity = uc_tmp;
                ret = r_flash_dm_dispatch(FLASH_DM_USER_ERASE_SETUP);
            }
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d1_erase() */

/************************************************************************************************
* Function Name: r_flash_dm_d1_format
* Description  : Formats the device.
* Arguments    : none
* Return Value : FLASH_DM_ACCEPT            ; Successful operation reception.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d1_format(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    /* ==== Check the parameter. ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_FORMAT;
    }
    /* ==== Check busy. ==== */
    else if (g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        /* === Check if this module is processing callback. === */
        if(g_flash_dm_handle->flash_state == DATF_FLASH_CALL)
        {
            ret = FLASH_DM_ERR_BUSY;
        }
        /* ==== Start to erase of the block. ==== */
        else
        {
            ret = r_flash_dm_dispatch(FLASH_DM_FORMAT_SETUP);
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d1_format() */

/************************************************************************************************
* Function Name: r_flash_dm_d1_control
* Description  : Define various functions.
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS           ; Successful operation and no erase block.
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization.
*              : FLASH_DM_ERR_ARGUMENT      ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*              : FLASH_DM_ERR               ; Failed operation.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d1_control(e_flash_dm_cmd_t cmd, uint32_t* pcfg)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    if ( NULL == pcfg)
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    else
    {
        switch (cmd)
        {
            case FLASH_DM_GET_WRITABLE_SIZE:
            {
                ret = r_flash_dm_get_writable_size(pcfg);
            }
            break;

            case FLASH_DM_GET_STATUS:
            {
                ret = r_flash_dm_getactivity(pcfg);
            }
            break;

            case FLASH_DM_GET_DATA_SIZE:
            {
                ret = r_flash_dm_getdatasize(pcfg);
            }
            break;

            case FLASH_DM_GET_DATA_NUM:
            {
                ret = r_flash_dm_getdatanum(pcfg);
            }
            break;

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
            case FLASH_DM_CF_PROTECT:
            {
                ret = r_flash_dm_cfprotect(pcfg);
            }
            break;

            case FLASH_DM_ROM_CACHE:
            {
                ret = r_flash_dm_cfromcache(pcfg);
            }
            break;
#endif
            default:
            {
                ret = FLASH_DM_ERR_ARGUMENT;
            }
            break;
        }
    }

    return ret;
} /* End of function r_flash_dm_d1_control() */

/************************************************************************************************
* Function Name: r_flash_dm_1_get_writable_size
* Description  : Acquires the size of free space
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS           ; Successful operation and no erase block.
*              : FLASH_DM_ERR_ARGUMENT      ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*              : FLASH_DM_ERR               ; Failed operation.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_1_get_writable_size(uint32_t* pcfg)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    int16_t writable_size;
    uint8_t tmp = g_flash_dm_handle->api_call;

    if (0 == g_flash_dm_handle)
    {
        return FLASH_DM_ERR_ARGUMENT;
    }

    if ( NULL == pcfg)
    {
        return FLASH_DM_ERR_ARGUMENT;
    }

    if (g_flash_dm_handle->activity == FLASH_DM_ACT_IDLE)
    {
        /* ==== Check status. ==== */
        ret = r_flash_dm_1st_check(FLASH_DM_WRITABLE_SIZE_1ST_CHECK);
        if (FLASH_DM_SUCCESS == ret)
        {
            /* ==== Get free area.  ==== */
            writable_size = r_flash_dm_free_area();
            if (writable_size <= 0)
            {
                ret = FLASH_DM_ERR;
            }
            else
            {
                *pcfg = (uint32_t)writable_size;
            }
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_1_get_writable_size() */

#else /* FLASH_TYPE */
/************************************************************************************************
* Function Name: r_flash_dm_d2_init
* Description  : Initializes the device.
* Arguments    : None
* Return Value : FLASH_DM_ADVANCE               ; Successful operation, call advanced function.
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*              : FLASH_DM_ERR_REQUEST_INIT      : Failed operation and request initialization.
*              : FLASH_DM_ERR_INIT              ; Failed operation of Initialize.
*------------------------------------------------------------------------------------------------
* Notes        : Always, Place the source code to the ROM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d2_init(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    /* ==== Handle Check ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_INIT;
    }
    /* ==== Check Busy. ==== */
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        g_flash_dm_handle->activity = FLASH_DM_ACT_INITIALIZING;
        /* cast from uint16_t to uint16_t */
        ret = r_flash_dm_1st_check(FLASH_DM_INIT_1ST_CHECK);

        if((FLASH_DM_ERR_BUSY != ret) && (FLASH_DM_ERR_REQUEST_INIT != ret))
        {
            g_flash_dm_handle->initialized = 0u;
            ret = r_flash_dm_init_set();
        }
    }
    g_flash_dm_handle->activity = FLASH_DM_ACT_IDLE;
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d2_init() */

/************************************************************************************************
* Function Name: r_flash_dm_d2_read
* Description  : Reads data from the device.
* Arguments    : st_flash_dm_info_t * p_flash_dm_info
*              :    uint8_t     data_no         ; The data no.
*              :    uint8_t   * p_data          ; Pointer to read data buffer
* Return Value : FLASH_DM_SUCCESS               ; Successful operation.
*              : FLASH_DM_ERR_ARGUMENT          ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*              : FLASH_DM_ERR_DATA_NOT_PRESENT  ; Failed operation of the data no. to support.
*              : FLASH_DM_ERR_REQUEST_INIT      ; Failed operation and request initialization.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d2_read(st_flash_dm_info_t * p_flash_dm_info)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    /* ==== Check the parameter. ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    /* ==== Check the data_no and p_data. ==== */
    else if ((p_flash_dm_info->data_no >= r_flash_dm_drvif_get_datanum()) ||
             (0 == p_flash_dm_info->p_data))
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    else
    {
        /* ==== Check Busy. ==== */
        if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
        {
            ret = FLASH_DM_ERR_BUSY;
        }
        else
        {
            /* ==== Check Status. ==== */
            ret = r_flash_dm_1st_check(FLASH_DM_READ_1ST_CHECK);

            if((FLASH_DM_ERR_BUSY != ret) && (FLASH_DM_ERR_REQUEST_INIT != ret))
            {
                /* ==== cast from uint8_t to uint8_t ==== */
                if(g_flash_dm_handle->initialized != FLASH_DM_INITIALIZED_ID)
                {
                    /* ==== This module checks whether initialization has been done. ==== */
                    g_flash_dm_handle->error_code = FLASH_DM_ERR_REQUEST_INIT;
                    ret = FLASH_DM_ERR_REQUEST_INIT;
                }
                else if (g_flash_dm_handle->data_info[p_flash_dm_info->data_no] == 0xFFFF)
                {
                    ret = FLASH_DM_ERR_DATA_NOT_PRESENT;
                }
                else
                {
                    /* ==== Start processing. ==== */
                    r_flash_dm_drvif_read(p_flash_dm_info);
                }
            }
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d2_read() */

/************************************************************************************************
* Function Name: r_flash_dm_d2_write
* Description  : Writes data to the device.
* Arguments    : st_flash_dm_info_t * p_flash_dm_info
*              :    uint8_t         data_no     ; The data no.
*              :    uint8_t       * p_data      ; Pointer to write data buffer.
* Return Value : FLASH_DM_ACCEPT                ; Successful operation reception.
*              : FLASH_DM_ERR_REQUEST_INIT      ; Failed operation and request initialization.
*              : FLASH_DM_ERR_ARGUMENT          ; Failed operation of parameter.
*              : FLASH_DM_ERR_REQUEST_ERASE     ; Failed operation and request erase.(TYPE,2,3,4)
*              : FLASH_DM_ERR_BUSY              ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d2_write(st_flash_dm_info_t * p_flash_dm_info)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;
    int16_t free = 0;

    /* === Check the parameter. === */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    /* === Check the data_no and p_data. === */
    else if ((p_flash_dm_info->data_no >= r_flash_dm_drvif_get_datanum()) ||
             (0 == p_flash_dm_info->p_data))
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    /* ==== Check busy. ==== */
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        /* ==== Check status. ==== */
        ret = r_flash_dm_1st_check(FLASH_DM_WRITE_1ST_CHECK);

        if((FLASH_DM_ERR_BUSY != ret) && (FLASH_DM_ERR_REQUEST_INIT != ret))
        {
            g_flash_dm_handle->data_no = p_flash_dm_info->data_no;
            g_flash_dm_handle->user_data = p_flash_dm_info->p_data;

            /* === This module checks whether initialization has been done. === */
            if (g_flash_dm_handle->initialized != FLASH_DM_INITIALIZED_ID)
            {
                g_flash_dm_handle->error_code = FLASH_DM_ERR_REQUEST_INIT;
                ret = FLASH_DM_ERR_REQUEST_INIT;
            }
            else if(g_flash_dm_handle->flash_state == DATF_FLASH_CALL)
            {
                ret = FLASH_DM_ERR_BUSY;
            }
            /* === Start processing. ===*/
            else
            {
                /* cast from uint16_t to uint16_t */
                free = (int16_t)(((g_flash_dm_handle->erase_block_num - 2) * DATF_F2_DATASIZE) + DATF_F1_DATASIZE);
                if (free < gc_dm_data_size[p_flash_dm_info->data_no])
                {
                    g_flash_dm_handle->error_code = FLASH_DM_ERR_REQUEST_ERASE;
                    ret = FLASH_DM_ERR_REQUEST_ERASE;
                }
                else
                {
                    ret = r_flash_dm_dispatch(DATF_USER_WRITE);
                }
            }
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d2_write() */

/************************************************************************************************
* Function Name: r_flash_dm_d2_erase
* Description  : Erases a garbage block.
* Arguments    : None
* Return Value : FLASH_DM_ACCEPT            ; Successful operation reception.
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization.
*              : FLASH_DM_NO_INVALID_BLOCK  ; Successful operation no invalid block.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*              : FLASH_DM_ERR_ERASE         ; Failed erase.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d2_erase(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    /* ==== Check the parameter. ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_ERASE;
    }
    /* ==== Busy Check ==== */
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        /* ==== Check status. ==== */
        ret = r_flash_dm_1st_check(FLASH_DM_ERASE_1ST_CHECK);

        if((FLASH_DM_BUSY != ret) && (FLASH_DM_ERR_REQUEST_INIT != ret))
        {
            /* ==== If the activity is not idle, do not initialize. ==== */
            if (g_flash_dm_handle->initialized != FLASH_DM_INITIALIZED_ID)
            {
                g_flash_dm_handle->error_code = FLASH_DM_ERR_REQUEST_INIT;
                ret = FLASH_DM_ERR_REQUEST_INIT;
            }
            else if (g_flash_dm_handle->invalid_block_num <= 1)
            {
                ret = FLASH_DM_NO_INVALID_BLOCK;
            }
            else if(g_flash_dm_handle->flash_state == DATF_FLASH_CALL)
            {
                ret = FLASH_DM_ERR_BUSY;
            }
            else
            {
                ret = r_flash_dm_dispatch(DATF_USER_ERASE);
            }
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d2_erase() */

/************************************************************************************************
* Function Name: r_flash_dm_d2_format
* Description  : Formats the device.
* Arguments    : none
* Return Value : FLASH_DM_ACCEPT            ; Successful operation reception.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d2_format(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    /* ==== Check the parameter. ==== */
    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_FORMAT;
    }
    /* ==== Check busy. ==== */
    else if(g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        /* === Check if this module is processing callback. === */
        if(g_flash_dm_handle->flash_state == DATF_FLASH_CALL)
        {
            ret = FLASH_DM_ERR_BUSY;
        }
        else
        {
            /* cast from uint32_t to int8_t */
            ret = r_flash_dm_dispatch(DATF_USER_FORMAT);
        }
    }
    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_d2_format() */

/************************************************************************************************
* Function Name: r_flash_dm_d2_control
* Description  : Define various functions.
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS           ; Successful operation.
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization.
*              : FLASH_DM_ERR_ARGUMENT      ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*              : FLASH_DM_ERR               ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_d2_control(e_flash_dm_cmd_t cmd, uint32_t* pcfg)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    if ( NULL == pcfg)
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    else
    {
        switch (cmd)
        {
            case FLASH_DM_GET_WRITABLE_SIZE :
            {
                ret = r_flash_dm_get_writable_size(pcfg);
            }
            break;

            case FLASH_DM_GET_STATUS :
            {
                ret = r_flash_dm_getactivity(pcfg);
            }
            break;

            case FLASH_DM_GET_DATA_SIZE :
            {
                ret = r_flash_dm_getdatasize(pcfg);
            }
            break;

            case FLASH_DM_GET_DATA_NUM :
            {
                ret = r_flash_dm_getdatanum(pcfg);
            }
            break;

            default:
            {
                ret = FLASH_DM_ERR_ARGUMENT;
            }
            break;
        }
    }

    return ret;
} /* End of function r_flash_dm_d2_control() */

/************************************************************************************************
* Function Name: r_flash_dm_2_get_writable_size
* Description  : Acquires the size of free space
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS           ; Successful operation and no erase block.
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization.
*              : FLASH_DM_ERR_ARGUMENT      ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*              : FLASH_DM_ERR               ; Failed operation.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_2_get_writable_size(uint32_t* pcfg)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    if ( NULL == pcfg)
    {
        return FLASH_DM_ERR_ARGUMENT;
    }

    if (g_flash_dm_handle->api_call == DATF_API_CALL)
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    else
    {
        g_flash_dm_handle->api_call = DATF_API_CALL;
        /* cast from uint8_t to uint8_t */
        if ((g_flash_dm_handle->activity != FLASH_DM_ACT_IDLE) || (g_flash_dm_handle->flash_state != DATF_FLASH_IDLE))
        {
            ret = FLASH_DM_ERR_BUSY;
        }
        /* cast from uint8_t to uint8_t */
        else if (g_flash_dm_handle->initialized != FLASH_DM_INITIALIZED_ID)
        {
            ret = FLASH_DM_ERR_REQUEST_INIT;
        }
        else
        {
            if(g_flash_dm_handle->erase_block_num == 1)
            {
                *pcfg = 0;
            }
            else
            {
                /* cast from uint16_t to uint16_t */
                *pcfg = (int16_t)(((g_flash_dm_handle->erase_block_num - 2) * DATF_F2_DATASIZE) + DATF_F1_DATASIZE);
            }
        }
        g_flash_dm_handle->api_call = DATF_API_IDLE;
    }
    return ret;
} /* End of function r_flash_dm_2_get_writable_size() */

#endif/* #define FLASH_TYPE */

/* common co-api area. */
/************************************************************************************************
* Function Name: r_flash_dm_drvif_close
* Description  : Close the driver interface.
* Arguments    : None
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, must place the source code to the ROM.
*              : (Prohibit the placement to RAM)
*************************************************************************************************/
static uint8_t r_flash_dm_drvif_close(void)
{
    if(R_FLASH_Close() != FLASH_SUCCESS)
    {
        return 0;
    }
    return 1;
} /* End of function r_flash_dm_drvif_close() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_datanum
* Description  : Gets the number of data.
* Arguments    : None
* Return Value : The number of data
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t r_flash_dm_drvif_get_datanum(void)
{
    /* cast from uint32_t to uint8_t */
    return FLASH_DM_CFG_DF_DATA_NUM;
} /* End of function r_flash_dm_drvif_get_datanum() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_datasize
* Description  : Gets the data size.
* Arguments    : uint8_t    data_no     ; The data no.
* Return Value : The data size of the number of the data
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint16_t r_flash_dm_drvif_get_datasize(uint32_t data_no)
{
    return gc_dm_data_size[data_no];
} /* End of function r_flash_dm_drvif_get_datasize() */

/************************************************************************************************
* Function Name: r_flash_dm_getdatasize
* Description  : Gets the data size.
* Arguments    : uint32_t    data_no     ; The data no.
* Return Value : FLASH_DM_SUCCESS           ; Successful operation and no erase block.
*              : FLASH_DM_ERR_ARGUMENT      ; Failed operation of parameter.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_getdatasize(uint32_t* data_no)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    /* ==== Check the parameter. ==== */
    if ((*data_no) >= r_flash_dm_drvif_get_datanum())
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }
    else
    {
        *data_no = r_flash_dm_drvif_get_datasize(*data_no);
    }

    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_getdatasize() */

/************************************************************************************************
* Function Name: r_flash_dm_getdatanum
* Description  : Gets the number of data.
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS           ; Successful operation and no erase block.
*              : FLASH_DM_ERR_BUSY          ; Failed operation of busy.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_getdatanum(uint32_t* data_no)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t tmp = g_flash_dm_handle->api_call;

    *data_no =  r_flash_dm_drvif_get_datanum();

    g_flash_dm_handle->api_call = tmp;
    return ret;
} /* End of function r_flash_dm_getdatanum() */

/************************************************************************************************
* Function Name: r_flash_dm_getactivity
* Description  : Gets the activity.
* Arguments    : None
* Return Value : FLASH_DM_SUCCESS           ; Successful operation and no erase block.
*              : FLASH_DM_ERR_ARGUMENT      ; Failed operation of parameter.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static e_flash_dm_status_t r_flash_dm_getactivity(uint32_t* pcfg)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    if (0 == g_flash_dm_handle)
    {
        ret = FLASH_DM_ERR_ARGUMENT;
    }

    *pcfg = g_flash_dm_handle->activity;

    return ret;

} /* End of function r_flash_dm_getactivity() */

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)
#pragma section
#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1) */

/* End of File */
