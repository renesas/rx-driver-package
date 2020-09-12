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
* File Name    : r_dispatch_1.c
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
#include "r_dispatch_1.h"

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#if(FLASH_TYPE == FLASH_TYPE_1)

#define FLASH_DM_JUMP(PC)   (pc = (PC))
#define FLASH_DM_UNWIND()   (--g_flash_dispatch_1_hndl->sp)
#define FLASH_DM_AGAIN()    (++g_flash_dispatch_1_hndl->sp)
#define FLASH_DM_RETURN()   FLASH_DM_UNWIND(); \
    sp = g_flash_dispatch_1_hndl->sp; \
    FLASH_DM_JUMP(g_flash_dispatch_1_hndl->stack[sp])
#define FLASH_DM_EXIT_DISPATCH(X)      return (X)
#define FLASH_DM_OVERHEAD_LOOP_COUNT   (4)
#define FLASH_DM_CPU_CYCLES_PER_LOOP   (4)

/************************************************************************************************
Local Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/

/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
st_flash_dispatch_1_hndl_t* (g_flash_dispatch_1_hndl);
static uint8_t           r_flash_dm_equal_p(const flash_dm_t * p_a, const flash_dm_t * p_b, uint16_t size);
static void              r_flash_dm_memclr(void * p_dest, uint16_t size);
static void              r_flash_dm_link(e_flash_dm_driver_status_t pc);
static void              r_flash_dm_try(e_flash_dm_driver_status_t pc);
#if defined(FLASH_DM_CHECK_REJECTED_BLOCK)
static void              r_flash_dm_reject_block(uint8_t block_num);
#endif /* defined(FLASH_DM_CHECK_REJECTED_BLOCK) */

/* common variables */
/* Physical addresses of data */
#if (FLASH_CFG_CODE_FLASH_ENABLE == 0)
flash_dm_ptr_t gs_flash_dm_data_addr[FLASH_DM_CFG_DF_DATA_NUM];
#elif
flash_dm_ptr_t gs_flash_dm_data_addr[FLASH_DM_CFG_CF_DATA_NUM];
#endif

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)
#pragma section FRAM
#endif

/************************************************************************************************
* Function Name: r_flash_dm_advance
* Description  : Continues process or acquires the status or driver.
* Arguments    : None
* Return Value : FLASH_DM_ACCEPT                ; Successful operation reception
*                FLASH_DM_FINISH_FORMAT         ; Format completion
*                FLASH_DM_FINISH_WRITE          ; Write completion
*                FLASH_DM_FINISH_ERASE          ; Erase completion
*                FLASH_DM_FINISH_RECLAIM        ; Reclaim completion
*                FLASH_DM_ERR_REQUEST_INIT      ; Error request initialize
*                FLASH_DM_ERR_FORMAT            ; Error format
*                FLASH_DM_ERR_WRITE             ; Error write
*                FLASH_DM_ERR_ERASE             ; Error erase
*                FLASH_DM_ERR_RECLAIM           ; Error reclaim
*                FLASH_DM_ERR_BUSY              ; Busy
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_advance(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    ret = r_flash_dm_1st_check(FLASH_DM_ADVANCE_1ST_CHECK);
    if(ret == FLASH_DM_SUCCESS)
    {
        ret = r_flash_dm_dispatch((e_flash_dm_driver_status_t)FLASH_DM_RESTART);
    }
    else
    {
        ret = FLASH_DM_ERR_REQUEST_INIT;
    }

    return ret;
} /* End of function r_flash_dm_advance() */

/* #define FLASH_DM_CFG_USE_OTHER_MANAGE */
#ifdef FLASH_DM_CFG_USE_OTHER_MANAGE
/************************************************************************************************
* Function Name: R_FLASH_DM_GetActivity
* Description  : Gets the activity of DATFRX.
* Arguments    : None
* Return Value : The activity       ; Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
uint8_t R_FLASH_DM_GetActivity(void)
{
    return g_flash_dispatch_1_hndl->activity;
} /* End of function R_FLASH_DM_GetActivity() */

/************************************************************************************************
* Function Name: R_FLASH_DM_FAPI_Open
* Description  : Refer to the Flash API.
* Arguments    : Refer to the Flash API.
* Return Value : true                   ; Successful operation
*              : false                  ; Error or DATFRX is active.
*------------------------------------------------------------------------------------------------
* Notes        :  If control the code flash, must place the source code to the ROM.
*              : (Prohibit the placement to RAM)
*************************************************************************************************/
bool R_FLASH_DM_FAPI_Open(void)
{
    if (FLASH_DM_ACT_IDLE == g_flash_dispatch_1_hndl->activity)
    {
        if (R_FLASH_Open() != FLASH_SUCCESS)
        {
            return false;
        }
        else
        {
            /* Do Nothing */
        }
        return true;
    }
    return false;
}

/************************************************************************************************
* Function Name: R_FLASH_DM_FAPI_Write
* Description  : Refer to the Flash API.
* Arguments    : Refer to the Flash API.
* Return Value : true                   ; Successful operation
*              : false                  ; Error or DATFRX is active.
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
bool R_FLASH_DM_FAPI_Write(uint32_t src_address, uint32_t dest_address, uint32_t num_bytes)
{
    if (FLASH_DM_ACT_IDLE == g_flash_dispatch_1_hndl->activity)
    {
        if (R_FLASH_Write(src_address, dest_address,  num_bytes) != FLASH_SUCCESS)
        {
            return false;
        }
        return true;
    }
    return false;
}

/************************************************************************************************
* Function Name: R_FLASH_DM_FAPI_Erase
* Description  : Refer to the Flash API.
* Arguments    : Refer to the Flash API.
* Return Value : true                   ; Successful operation
*              : false                  ; Error or DATFRX is active.
*************************************************************************************************/
bool R_FLASH_DM_FAPI_Erase(flash_block_address_t block_start_address, uint32_t num_blocks)
{
    if (FLASH_DM_ACT_IDLE == g_flash_dispatch_1_hndl->activity)
    {
        if (R_FLASH_Erase(block_start_address, num_blocks) != FLASH_SUCCESS)
        {
            return false;
        }
        return true;
    }
    return false;
}

/************************************************************************************************
* Function Name: R_FLASH_DM_FAPI_BlankCheck
* Description  : Refer to the Flash API.
* Arguments    : Refer to the Flash API.
* Return Value : true                   ; Successful operation
*              : false                  ; Error or DATFRX is active.
*************************************************************************************************/
bool R_FLASH_DM_FAPI_BlankCheck(uint32_t address, uint32_t num_bytes, flash_res_t *result)
{
    if (FLASH_DM_ACT_IDLE == g_flash_dispatch_1_hndl->activity)
    {
        if (R_FLASH_BlankCheck(address, num_bytes, result) != FLASH_SUCCESS)
        {
            return false;
        }
        return true;
    }
    return false;
}

/************************************************************************************************
* Function Name: R_FLASH_DM_FAPI_Control
* Description  : Refer to the Flash API.
* Arguments    : Refer to the Flash API.
* Return Value : true                   ; Successful operation
*              : false                  ; Error or DATFRX is active.
*************************************************************************************************/
bool R_FLASH_DM_FAPI_Control(flash_cmd_t cmd, void *pcfg)
{
    if (FLASH_DM_ACT_IDLE == g_flash_dispatch_1_hndl->activity)
    {
        if (R_FLASH_DM_Control(cmd, pcfg) != FLASH_SUCCESS)
        {
            return false;
        }
        return true;
    }
    return false;
}
#endif /* FLASH_DM_CFG_USE_OTHER_MANAGE */

/************************************************************************************************
* Function Name: r_flash_dm_memcopy
* Description  : Memory copy
* Arguments    : void         * p_dest  ; Pointer of the start address of destination
*              : const void   * p_src   ; Pointer of the start address of source
*              : uint16_t       size    ; Copy size
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
void r_flash_dm_memcopy(void * p_dest, const void * p_src, uint16_t size)
{
    uint8_t * p_dest_tmp = p_dest;
    const uint8_t * p_src_tmp  = p_src;

    /* WAIT_LOOP */
    do
    {
        *p_dest_tmp++ = *p_src_tmp++;
        size--;
    }
    while (0 != size);
} /* End of function r_flash_dm_memcopy() */

/************************************************************************************************
* Function Name: r_flash_dm_memclr
* Description  : Memory clear
* Arguments    : void         * p_dest  ; Pointer of the start address of destination
*              : uint16_t       size    ; Clear size
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static void r_flash_dm_memclr(void * p_dest, uint16_t size)
{
    uint8_t * p_dest_tmp = p_dest;

    /* WAIT_LOOP */
    do
    {
        *p_dest_tmp++ = 0;
        size--;
    }
    while (0 != size);
} /* End of function r_flash_dm_memclr() */

/************************************************************************************************
* Function Name: r_flash_dm_equal_p
* Description  : Compares data.
* Arguments    : const flash_dm_t * p_a     ; Pointer of the start address of data_a
*              : const flash_dm_t * p_b     ; Pointer of the start address of data_b
*              : uint16_t           size    ; Comparison size
* Return Value : 1: Agreement
*              : 0: Disagreement
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static uint8_t r_flash_dm_equal_p(const flash_dm_t * p_a, const flash_dm_t * p_b, uint16_t size)
{
    /* WAIT_LOOP */
    while (size > 0)
    {
        if ((*p_a++) != (*p_b++))
        {
            return 0;
        }
        /* cast from uint32_t* uint16_t */
        size -= (uint16_t)FLASH_DF_MIN_PGM_SIZE;
    }
    return 1;
} /* End of function r_flash_dm_equal_p() */

/************************************************************************************************
* Function Name: r_flash_dm_init_env
* Description  : Initializes the handler and global variables.
* Arguments    : uint8_t    init_code   ; Initialization code
*              : uint8_t    act         ; Activity
* Return Value : 1: Successful operation
*              : 0: Failed operation (It is necessary to call this function again)
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
uint8_t r_flash_dm_init_env(uint8_t init_code, uint8_t act)
{
    uint16_t i = 0;
    uint8_t ret = 0;
    st_flash_dm_block_t * p_block = 0;
    uint32_t * p_dataaddr = 0;

    ret = r_flash_dm_block_init();

    /* Check the block initialization. */
    if (1 == ret)
    {
        /* ==== Clear of the latest data address. ==== */
        p_dataaddr = (uint32_t *)r_flash_dm_drvif_get_dataaddr_of_addr(0);
        /* cast from uint32_t to void* */
        r_flash_dm_memclr((void  *)p_dataaddr, sizeof(flash_dm_ptr_t) * (uint16_t)r_flash_dm_drvif_get_datanum());

        /* ==== Initialize the handler. ==== */
        r_flash_dm_memclr(g_flash_dispatch_1_hndl, sizeof(st_flash_dispatch_1_hndl_t));

        /* Get the size of data header. */
        g_flash_dispatch_1_hndl->header_write_size = (((sizeof(st_flash_dm_dh_t) - FLASH_DF_MIN_PGM_SIZE)
                /*cast from uint8_t to st_flash_dm_dh_t */
                - FLASH_DF_MIN_PGM_SIZE) - FLASH_DF_MIN_PGM_SIZE);

        /* ----Set the value except 0. ---- */
        g_flash_dispatch_1_hndl->api_call = FLASH_DM_API_CALL;
        g_flash_dispatch_1_hndl->initialized = init_code;
        g_flash_dispatch_1_hndl->activity = act;
        p_block = r_flash_dm_drvif_get_block_info(0);
        g_flash_dispatch_1_hndl->nblock = r_flash_dm_drvif_get_block_num();
        /* WAIT_LOOP */
        for (i = 0; i < g_flash_dispatch_1_hndl->nblock; i++, p_block++)
        {
            /* cast from flash_dm_ptr_t to flash_dm_block_format_ptr_t */
            p_block->p_img = (flash_dm_block_format_ptr_t)r_flash_dm_drvif_get_block_addr(i);
            /* cast from uint16_t to uint8_t */
            p_block->next  = (uint8_t)(i + 1);
            /* cast from uint16_t to uint8_t */
            p_block->prev  = (uint8_t)(i - 1);
            /* cast from uint16_t to uint8_t */
            p_block->index = (uint8_t)i;
            p_block->stat  = FLASH_DM_BLOCK_STATUS_UNKNOWN;
        }
        (--p_block)->next = 0;
        /* cast from uint8_t to uint8_t */
        r_flash_dm_drvif_get_block_info(0)->prev = (uint8_t)(g_flash_dispatch_1_hndl->nblock - 1);
    }
    return ret;
} /* End of function r_flash_dm_init_env() */

/************************************************************************************************
* Function Name: r_flash_dm_free_area
* Description  : Gets free area size.
* Arguments    : None
* Return Value : Free area size (byte)
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
int16_t r_flash_dm_free_area(void)
{
    /* cast from uint32_t to int16_t */
    return (int16_t)(((uint32_t)g_flash_dispatch_1_hndl->p_data -
                      /* cast from st_flash_dm_dh_t to uint32_t */
                      (uint32_t)&g_flash_dispatch_1_hndl-> \
                      p_active->p_img->h.d[g_flash_dispatch_1_hndl->header_index + 1]) \
                     /* cast from uint8_t to uint32_t */
                     - FLASH_DF_MIN_PGM_SIZE);
} /* End of function r_flash_dm_free_area() */

/************************************************************************************************
* Function Name: r_flash_dm_link
* Description  : Pushes the driver stack.
* Arguments    : e_flash_dm_driver_status_t   pc  ; Value of the pushing
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static void r_flash_dm_link(e_flash_dm_driver_status_t pc)
{
    /* Check the driver stack. */
    if (g_flash_dispatch_1_hndl->sp < FLASH_DM_STACKSIZE)
    {
        g_flash_dispatch_1_hndl->stack[(g_flash_dispatch_1_hndl->sp)++] = pc;
    }
    else
    {
        g_flash_dispatch_1_hndl->stack[g_flash_dispatch_1_hndl->sp] = FLASH_DM_STACK_ERROR;
    }
} /* End of function r_flash_dm_link() */

/************************************************************************************************
* Function Name: r_flash_dm_try
* Description  : Sets the driver stack.
* Arguments    : e_flash_dm_driver_status_t   pc  ; Value of the pushing
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
static void r_flash_dm_try(e_flash_dm_driver_status_t pc)
{
    r_flash_dm_link(pc);
    r_flash_dm_link(FLASH_DM_CATCH_ERROR);
} /* End of function r_flash_dm_try() */

/************************************************************************************************
* Function Name: r_flash_dm_dispatch
* Description  : Performs function.
* Arguments    : e_flash_dm_driver_status_t   pc  ; Value of the pushing
* Return Value : FLASH_DM_ACCEPT                ; Successful operation reception
*                FLASH_DM_FINISH_FORMAT         ; Format completion
*                FLASH_DM_FINISH_WRITE          ; Write completion
*                FLASH_DM_FINISH_ERASE          ; Erase completion
*                FLASH_DM_FINISH_RECLAIM        ; Reclaim completion
*                FLASH_DM_ERR_REQUEST_INIT      ; Error request initialize
*                FLASH_DM_ERR_FORMAT            ; Error format
*                FLASH_DM_ERR_WRITE             ; Error write
*                FLASH_DM_ERR_ERASE             ; Error erase
*                FLASH_DM_ERR_RECLAIM           ; Error reclaim
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_dispatch(e_flash_dm_driver_status_t pc)
{
    e_flash_dm_status_t b_ret = FLASH_DM_SUCCESS;
    e_flash_dm_state_t b_block_ret = FLASH_DM_FALSE;
    flash_dm_dh_ptr_t p_dh = 0;
    uint32_t addr = 0;
    uint8_t sp = 0;
    uint8_t b_cmp = 0;
    uint8_t i = 0;
    uint8_t uc_tmp = 0;

    /* WAIT_LOOP */
    while(1)
    {
        switch (pc)
        {
            /* ==== Idle state processing ==== */
            case FLASH_DM_IDLE_SETUP:       /* Prepare to idle state */
            {
                g_flash_dispatch_1_hndl->sp = 0;

                /* Check the presence for the reclaim. */
                if (0 != g_flash_dispatch_1_hndl->p_reclaim)
                {
                    g_flash_dispatch_1_hndl->stack[0] = FLASH_DM_IDLE_REQ_ERASE;
                }
                else
                {
                    g_flash_dispatch_1_hndl->stack[0] = FLASH_DM_IDLE_NORMAL;
                }
                FLASH_DM_JUMP(g_flash_dispatch_1_hndl->stack[0]);
            }
            break;

            case FLASH_DM_IDLE_NORMAL:      /* Idle state */
            {
                FLASH_DM_EXIT_DISPATCH(FLASH_DM_SUCCESS);
            }
            break;

            case FLASH_DM_IDLE_REQ_ERASE:   /* Request erase */
            {
                FLASH_DM_EXIT_DISPATCH(FLASH_DM_ERR_REQUEST_ERASE);
            }
            break;

            /* ==== Common : Driver operation processing ==== */
            case FLASH_DM_RESTART:          /* Restart dispatch */
            {
                sp = g_flash_dispatch_1_hndl->sp;
                if (sp > 0)
                {
                    sp--;
                }
                FLASH_DM_JUMP(g_flash_dispatch_1_hndl->stack[sp]);
                g_flash_dispatch_1_hndl->sp = sp;
            }
            break;

            case FLASH_DM_TERMINATE:        /* End processing */
            {
                g_flash_dispatch_1_hndl->sp = 0;
                g_flash_dispatch_1_hndl->stack[0] = FLASH_DM_IDLE_SETUP;
                g_flash_dispatch_1_hndl->flash_state = DATF_FLASH_IDLE;
                g_flash_dispatch_1_hndl->activity = FLASH_DM_ACT_IDLE;
                if (0 != r_flash_dm_callbackfunc)
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)g_flash_dispatch_1_hndl->error_code);
                }

                /* cast from e_flash_dm_status_t to e_flash_dm_status_t */
                FLASH_DM_EXIT_DISPATCH((e_flash_dm_status_t)b_ret);
            }
            break;

            case FLASH_DM_CATCH_ERROR:      /* Stack operation error */
            {
                FLASH_DM_UNWIND();
                FLASH_DM_RETURN();
            }
            break;

            case FLASH_DM_RAISE:            /* Error end processing */
            {
                /* Search FLASH_DM_CATCH_ERROR until the stack pointer (sp) becomes 1.
                   If cannot search FLASH_DM_CATCH_ERROR, judge it to be an error. */
                /* WAIT_LOOP */
                while ((g_flash_dispatch_1_hndl->stack[g_flash_dispatch_1_hndl->sp] != FLASH_DM_CATCH_ERROR) &&
                        (g_flash_dispatch_1_hndl->sp > 1))
                {
                    g_flash_dispatch_1_hndl->sp--;
                }

                if (g_flash_dispatch_1_hndl->stack[g_flash_dispatch_1_hndl->sp] != FLASH_DM_CATCH_ERROR)
                {
                    FLASH_DM_JUMP(FLASH_DM_STACK_ERROR);    /* Judge the stack pointer error. */
                }
                else
                {
                    FLASH_DM_RETURN();
                }
            }
            break;

            case FLASH_DM_STACK_ERROR:      /* Stack pointer error (Overflow or no FLASH_DM_CATCH_ERROR) */
            {
                FLASH_DM_EXIT_DISPATCH(FLASH_DM_ERR_REQUEST_INIT);  /* Request initialization */
            }
            break;

            /* ==== Common : Program processing ==== */
            case FLASH_DM_COMMAND:          /* Issue command. */
            {
                b_block_ret = r_flash_dm_block_command(g_flash_dispatch_1_hndl);

                switch (b_block_ret)
                {
                    case FLASH_DM_COMMAND_END:
                    {
                        /* When this case is always FINISH_DM_ACCEPT. */
                        b_ret = FLASH_DM_ACCEPT;
                        /* cast from e_flash_dm_status_t to e_flash_dm_status_t */
                        FLASH_DM_EXIT_DISPATCH((e_flash_dm_status_t)b_ret);
                    }
                    break;

                    case FLASH_DM_PROGRAM_END:
                    {
                        FLASH_DM_UNWIND();  /* Decrement stack pointer (FLASH_DM_PROGRAM_CHECK).  */
                        FLASH_DM_RETURN();
                    }
                    break;

                    case FLASH_DM_COMMAND_FALSE:
                    {
                        /* When this case is always FLASH_DM_ACCEPT. */
                        b_ret = FLASH_DM_ERR;
                        FLASH_DM_JUMP(FLASH_DM_RAISE);
                    }
                    break;

                    default:
                    {
                        /* Do Nothing */
                    }
                    break;
                }
            }
            break;

            case FLASH_DM_PROGRAM_CHECK:    /* Check program completion.    */
            {
                b_block_ret = r_flash_dm_block_program_check();

                switch (b_block_ret)
                {
                    case FLASH_DM_PROGRAM_ERROR:
                    {
                        FLASH_DM_JUMP(FLASH_DM_RAISE);
                    }
                    break;

                    case FLASH_DM_PROGRAM_CHECK_END:
                    {
                        r_flash_dm_link(FLASH_DM_PROGRAM_CHECK);
                        FLASH_DM_JUMP(FLASH_DM_COMMAND);
                    }
                    break;

                    default:
                    {
                        /* Do Nothing */
                    }
                    break;
                }
            }
            break;

            case FLASH_DM_ERASE_CHECK:      /* Check erase completion. */
            {
                b_block_ret = r_flash_dm_block_erase_check();

                switch (b_block_ret)
                {
                    case FLASH_DM_ERASE_ERROR:
                    {
                        FLASH_DM_JUMP(FLASH_DM_RAISE);
                    }
                    break;

                    case FLASH_DM_ERASE_END:
                    {
                        FLASH_DM_RETURN();
                    }
                    break;

                    default:
                    {
                        /* Do Nothing */
                    }
                    break;
                }
            }
            break;

            case FLASH_DM_PROGRAM_BLOCK_ERASE_FLAG:     /* Prepare to write erase completion flag. */
            {
                r_flash_dm_block_program_init(r_flash_dm_drvif_get_block_erase_flag(),
                                              /* cast from uint8_t to uint8_t */
                                              FLASH_DM_BH_ERASE_FLAG_SIZE,
                                              *r_flash_dm_drvif_get_block_erase_flag());
                r_flash_dm_link(FLASH_DM_PROGRAM_BLOCK_ERASE_FLAG_CHECK);
                r_flash_dm_link(FLASH_DM_PROGRAM_CHECK);
                FLASH_DM_JUMP(FLASH_DM_COMMAND);
            }
            break;

            case FLASH_DM_PROGRAM_BLOCK_ERASE_FLAG_CHECK:   /* Check erase completion flag. */
            {
                b_cmp = r_flash_dm_equal_p(r_flash_dm_drvif_get_block_erase_flag(),
                                           /* cast from uint32_t to uint_8 */
                                           (const flash_dm_t *)((uint32_t)r_flash_dm_block_get_program_addr() \
                                                   /* cast from uint8_t to uint8_t */
                                                   - FLASH_DM_BH_ERASE_FLAG_SIZE),
                                           /* cast from uint8_t to uint8_t */
                                           FLASH_DM_BH_ERASE_FLAG_SIZE);
                if (0 == b_cmp)
                {
                    FLASH_DM_JUMP(FLASH_DM_RAISE);  /* Failed operation */
                }
                else
                {
                    FLASH_DM_RETURN();
                }
            }
            break;

            case FLASH_DM_PROGRAM_BLOCK_FLAG:   /* Prepare to write block flag. */
            {
                r_flash_dm_block_program_init(0u,
                                              /* cast from uint8_t to uint8_t */
                                              FLASH_DM_BH_FLAG_SIZE,
                                              /* cast from uint8_t to uint16_t */
                                              FLASH_DM_BH_FLAG_00);
                r_flash_dm_link(FLASH_DM_PROGRAM_BLOCK_FLAG_CHECK);
                r_flash_dm_link(FLASH_DM_PROGRAM_CHECK);
                FLASH_DM_JUMP(FLASH_DM_COMMAND);
            }
            break;

            case FLASH_DM_PROGRAM_BLOCK_FLAG_CHECK: /* Check block flag. */
            {
                /* cast from uint32_t to flash_dm_t* */
                if ((*(flash_dm_t  *)((uint32_t)r_flash_dm_block_get_program_addr() -
                                      /* cast from uint8_t to uint8_t */
                                      FLASH_DM_BH_FLAG_SIZE)) != FLASH_DM_BH_FLAG_00)
                {
                    FLASH_DM_JUMP(FLASH_DM_RAISE);  /* Failed operation */
                }
                else
                {
                    FLASH_DM_RETURN();
                }
            }
            break;

            /* ==== Erase block processing ==== */
            case FLASH_DM_USER_ERASE_SETUP:     /* Write the erase start flag. */
            {
                b_ret = FLASH_DM_SUCCESS;
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_1_hndl->activity = FLASH_DM_ACT_ERASING;
                g_flash_dispatch_1_hndl->p_bh = &g_flash_dispatch_1_hndl->p_active->p_img->h.b;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_SUCCESS;
                /* cast from u_flash_dm_block_format_t to flash_dm_ptr_t */
                r_flash_dm_block_set_program_addr((flash_dm_ptr_t)g_flash_dispatch_1_hndl->p_reclaim->p_img);
                r_flash_dm_try(FLASH_DM_USER_ERASE_FAILURE);
                r_flash_dm_link(FLASH_DM_USER_ERASE_FINALIZE);
                /* cast from uint8_t to uint8_t */
                if ((*(flash_dm_t  *)((uint32_t)r_flash_dm_block_get_program_addr())) == FLASH_DM_BH_FLAG_FF)
                {
                    r_flash_dm_link(FLASH_DM_USER_ERASE);
                    FLASH_DM_JUMP(FLASH_DM_PROGRAM_BLOCK_FLAG);     /* Write the erase start flag. */
                }
                else
                {
                    FLASH_DM_JUMP(FLASH_DM_USER_ERASE);
                }
            }
            break;

            case FLASH_DM_USER_ERASE:   /* Block erase start processing */
            {
                /* cast from  u_flash_dm_block_format_t to flash_dm_ptr_t */
                r_flash_dm_block_set_program_addr((flash_dm_ptr_t)g_flash_dispatch_1_hndl->p_reclaim->p_img);
                r_flash_dm_block_erase_init();
                r_flash_dm_link(FLASH_DM_USER_ERASE_FINALIZE_SETUP);
                r_flash_dm_link(FLASH_DM_ERASE_CHECK);
                FLASH_DM_JUMP(FLASH_DM_COMMAND);
            }
            break;

            case FLASH_DM_USER_ERASE_FINALIZE_SETUP:    /* Write the erase completion flag */
            {
                r_flash_dm_block_set_program_addr \
                /* cast from uint8_t to flash_dm_ptr_t */
                ((flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_reclaim->p_img->h.b.erase_end[0]);
                r_flash_dm_link(FLASH_DM_PROGRAM_BLOCK_FLAG);
                FLASH_DM_JUMP(FLASH_DM_PROGRAM_BLOCK_ERASE_FLAG);   /* Write the erase completion flag. */
            }
            break;

            case FLASH_DM_USER_ERASE_FINALIZE:  /* Check block erase completion. */
            {
                /* cast from uint32_t to uint16_t* */
                if ((*(uint16_t *)((uint32_t)r_flash_dm_block_get_program_addr() -
                                   /* cast from uint8_t to uint8_t */
                                   FLASH_DM_BH_FLAG_SIZE)) != FLASH_DM_BH_FLAG_00)
                {
                    FLASH_DM_JUMP(FLASH_DM_RAISE);  /* Failed operation */
                }
                else
                {
                    g_flash_dispatch_1_hndl->p_reclaim->stat = FLASH_DM_BLOCK_STATUS_READY;
                    g_flash_dispatch_1_hndl->p_reclaim = 0;
                    b_ret = FLASH_DM_FINISH_ERASE;
                    g_flash_dispatch_1_hndl->error_code = FLASH_DM_FINISH_ERASE;
                    FLASH_DM_JUMP(FLASH_DM_TERMINATE);
                }
            }
            break;

            case FLASH_DM_USER_ERASE_FAILURE:   /* Failed operation of block erase. */
            {
                g_flash_dispatch_1_hndl->p_reclaim->stat = FLASH_DM_BLOCK_STATUS_ERROR_GARBAGE;
                b_ret = FLASH_DM_ERR_ERASE;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_ERR_ERASE;
                FLASH_DM_JUMP(FLASH_DM_TERMINATE);
            }
            break;

            /* ==== Write processing ==== */
            case FLASH_DM_USER_WRITE_SETUP:     /* Prepare to write data. */
            {
                b_ret = FLASH_DM_SUCCESS;
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_1_hndl->activity = FLASH_DM_ACT_WRITING;
                /* cast from flash_dm_ptr_t to uint16_t */
                g_flash_dispatch_1_hndl->dh.addr = ((uint16_t)g_flash_dispatch_1_hndl->p_data -
                                                    r_flash_dm_drvif_get_datasize(g_flash_dispatch_1_hndl->dh.no));
                r_flash_dm_drvif_set_addr_h(g_flash_dispatch_1_hndl);
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_SUCCESS;
                r_flash_dm_try(FLASH_DM_USER_WRITE_FAILURE);
                r_flash_dm_link(FLASH_DM_USER_WRITE_FINALIZE);
                FLASH_DM_JUMP(FLASH_DM_WRITE_SETUP);
            }
            break;

            case FLASH_DM_WRITE_SETUP:          /* Prepare to write data again. */
            {
                g_flash_dispatch_1_hndl->p_dh \
                    = &g_flash_dispatch_1_hndl->p_active->p_img->h.d[g_flash_dispatch_1_hndl->header_index];
                /* cast from uint8_t to uint8_t */
                r_flash_dm_drvif_set_dh_flag(&g_flash_dispatch_1_hndl->dh.header_exist[0], FLASH_DM_DH_EXIST_FLAG);
                /* cast from uint8_t to flash_dm_dh_ptr_t */
                r_flash_dm_block_set_program_addr((flash_dm_ptr_t)g_flash_dispatch_1_hndl->p_dh);
                /* cast from uint8_t to flash_dm_dh_ptr_t */
                r_flash_dm_block_program_init((flash_dm_ptr_t)&g_flash_dispatch_1_hndl->dh,
                                              g_flash_dispatch_1_hndl->header_write_size,
                                              g_flash_dispatch_1_hndl->dh.header_exist[0]);
                r_flash_dm_link(FLASH_DM_WRITE_HEADER_VALIDATE);
                r_flash_dm_link(FLASH_DM_PROGRAM_CHECK);
                FLASH_DM_JUMP(FLASH_DM_COMMAND);    /* Start to write the update stat flag, data id, data address. */
            }
            break;

            case FLASH_DM_WRITE_HEADER_VALIDATE:    /* Write data header update completion flag. */
            {
                p_dh = g_flash_dispatch_1_hndl->p_dh;

                /* ---- Check data header. ---- */
                if ((FLASH_DM_DH_EXIST_FLAG != p_dh->header_exist[0]) ||
                        ((p_dh->no != g_flash_dispatch_1_hndl->dh.no) ||
                         (p_dh->addr != g_flash_dispatch_1_hndl->dh.addr)))
                {
                    FLASH_DM_JUMP(FLASH_DM_RAISE);  /* Failed operation */
                }
                else
                {
                    /* cast from uint8_t to flash_dm_dh_ptr_t */
                    r_flash_dm_block_set_program_addr((flash_dm_ptr_t)&p_dh->header_valid[0]);
                    r_flash_dm_block_program_init(0,
                                                  /* cast from uint8_t to uint16_t */
                                                  FLASH_DM_DH_FLAG_SIZE,
                                                  /* cast from uint8_t to uint8_t */
                                                  FLASH_DM_DH_VALIDATE_FLAG);
                    r_flash_dm_link(FLASH_DM_WRITE_DATA);
                    r_flash_dm_link(FLASH_DM_PROGRAM_CHECK);
                    FLASH_DM_JUMP(FLASH_DM_COMMAND);
                }
            }
            break;

            case FLASH_DM_WRITE_DATA:   /* Write data */
            {

                /* ---- Check data header update completion flag. ---- */
                if (FLASH_DM_DH_VALIDATE_FLAG != g_flash_dispatch_1_hndl->p_dh->header_valid[0])
                {
                    FLASH_DM_JUMP(FLASH_DM_RAISE);  /* Failed operation */
                }
                else
                {
                    if (0 == g_flash_dispatch_1_hndl->p_user_data)
                    {
                        FLASH_DM_JUMP(FLASH_DM_WRITE_DATA_VALIDATE);    /* No data */
                    }
                    else
                    {
                        addr = r_flash_dm_drvif_get_addr(&g_flash_dispatch_1_hndl->dh);
                        /* cast from uint32_t to flash_dm_dh_ptr_t */
                        r_flash_dm_block_set_program_addr((flash_dm_ptr_t)addr);
                        /* cast from uint8_t to flash_dm_dh_ptr_t */
                        r_flash_dm_block_program_init((flash_dm_ptr_t)g_flash_dispatch_1_hndl->p_user_data,
                                                      r_flash_dm_drvif_get_datasize(g_flash_dispatch_1_hndl->dh.no),
                                                      /* cast from uint8_t to uint8_t */
                                                      FLASH_DM_DH_FLAG_00);
                        r_flash_dm_link(FLASH_DM_WRITE_DATA_VALIDATE);
                        r_flash_dm_link(FLASH_DM_PROGRAM_CHECK);
                        FLASH_DM_JUMP(FLASH_DM_COMMAND);
                        addr = r_flash_dm_drvif_get_addr(&g_flash_dispatch_1_hndl->dh);
                        /* cast from uint8_t to flash_dm_dh_ptr_t */
                        g_flash_dispatch_1_hndl->p_data = (flash_dm_ptr_t)addr;
                    }
                }
            }
            break;

            case FLASH_DM_WRITE_DATA_VALIDATE:  /* Write data update completion flag. */
            {

                /* ---- Check to write user data.  ---- */
                addr = r_flash_dm_drvif_get_addr(&g_flash_dispatch_1_hndl->dh);
                /* cast from uint8_t to uint8_t */
                b_cmp = r_flash_dm_equal_p((const flash_dm_t *)g_flash_dispatch_1_hndl->p_user_data,
                                           /* cast from uint8_t to uint8_t */
                                           (const flash_dm_t *)addr,
                                           r_flash_dm_drvif_get_datasize(g_flash_dispatch_1_hndl->dh.no));
                if (0 == b_cmp)
                {
                    FLASH_DM_JUMP(FLASH_DM_RAISE);  /* Failed operation */
                }
                else
                {
                    /* cast from uint8_t to flash_dm_dh_ptr_t */
                    r_flash_dm_block_set_program_addr((flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_dh->data_valid[0]);
                    r_flash_dm_block_program_init(0u,
                                                  /* cast from uint8_t to uint16_t */
                                                  FLASH_DM_DH_FLAG_SIZE,
                                                  /* cast from uint8_t to uint8_t */
                                                  FLASH_DM_DH_VALID_FLAG);
                    r_flash_dm_link(FLASH_DM_WRITE_VALIDATE);
                    r_flash_dm_link(FLASH_DM_PROGRAM_CHECK);
                    FLASH_DM_JUMP(FLASH_DM_COMMAND);
                }
            }
            break;

            case FLASH_DM_WRITE_VALIDATE:       /* Write valid flag. */
            {
                /* ---- Check data update completion flag. ---- */
                if (FLASH_DM_DH_VALID_FLAG != g_flash_dispatch_1_hndl->p_dh->data_valid[0])
                {
                    FLASH_DM_JUMP(FLASH_DM_RAISE);
                }
                else
                {
                    /* cast from uint8_t to flash_dm_dh_ptr_t */
                    r_flash_dm_block_set_program_addr((flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_dh->validity[0]);
                    r_flash_dm_block_program_init(0u,
                                                  /* cast from uint8_t to uint16_t */
                                                  FLASH_DM_DH_FLAG_SIZE,
                                                  /* cast from uint8_t to uint8_t */
                                                  FLASH_DM_DH_VALIDITY_FLAG);
                    r_flash_dm_link(FLASH_DM_WRITE_VALIDATE_CHECK);
                    r_flash_dm_link(FLASH_DM_PROGRAM_CHECK);
                    FLASH_DM_JUMP(FLASH_DM_COMMAND);
                }
            }
            break;

            case FLASH_DM_WRITE_VALIDATE_CHECK:     /* Check to write valid flag. */
            {
                /* cast from uint8_t to uint8_t */
                if (FLASH_DM_DH_VALIDITY_FLAG != g_flash_dispatch_1_hndl->p_dh->validity[0])
                {
                    FLASH_DM_JUMP(FLASH_DM_RAISE);
                }
                else
                {
                    FLASH_DM_JUMP(FLASH_DM_WRITE_FINALIZE);
                }
            }
            break;

            case FLASH_DM_WRITE_FINALIZE:       /* Finish to write data. */
            {
                g_flash_dispatch_1_hndl->header_index++;
                if (0 != g_flash_dispatch_1_hndl->p_user_data)
                {
                    addr = r_flash_dm_drvif_get_addr(&g_flash_dispatch_1_hndl->dh);

                    /* Update data index. */
                    r_flash_dm_drvif_set_dataaddr(g_flash_dispatch_1_hndl->dh.no, (flash_dm_ptr_t)addr);
                }
                FLASH_DM_RETURN();
            }
            break;

            case FLASH_DM_USER_WRITE_FINALIZE:  /* Finish to write data. */
            {
                /* cast from uint8_t to uint8_t */
                uc_tmp = (g_flash_dispatch_1_hndl->activity & (~FLASH_DM_ACT_WRITING));
                g_flash_dispatch_1_hndl->activity = uc_tmp;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_FINISH_WRITE;
                b_ret = FLASH_DM_FINISH_WRITE;
                FLASH_DM_JUMP(FLASH_DM_TERMINATE);
            }
            break;

            case FLASH_DM_USER_WRITE_FAILURE:   /* Failed operation of write data. */
            {
                /* cast from uint8_t to uint8_t */
                if (FLASH_DM_DH_FLAG_FF != g_flash_dispatch_1_hndl->p_dh->header_exist[0])
                {
                    g_flash_dispatch_1_hndl->header_index++;
                }
                /* cast from uint8_t to uint8_t */
                uc_tmp = (g_flash_dispatch_1_hndl->activity & (~FLASH_DM_ACT_WRITING));
                g_flash_dispatch_1_hndl->activity = uc_tmp;
                b_ret = FLASH_DM_ERR_WRITE;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_ERR_WRITE;
                FLASH_DM_JUMP(FLASH_DM_TERMINATE);
            }
            break;

            /* ==== Reclaim processing ==== */
            case FLASH_DM_USER_RECLAIM_SETUP:   /* Write the full flag. */
            {
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_1_hndl->activity = FLASH_DM_ACT_RECLAIMING;
                r_flash_dm_try(FLASH_DM_USER_RECLAIM_FAILURE);
                b_ret = FLASH_DM_SUCCESS;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_SUCCESS;
                r_flash_dm_block_set_program_addr \
                /* cast from uint8_t to flash_dm_dh_ptr_t */
                ((flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_active->p_img->h.b.full[0]);
                r_flash_dm_link(FLASH_DM_RECLAIM_CREATE_BLOCK);
                FLASH_DM_JUMP(FLASH_DM_PROGRAM_BLOCK_FLAG);     /* Write the full flag. */
            }
            break;

            case FLASH_DM_RECLAIM_CREATE_BLOCK:     /* Write the active flag. */
            {
                g_flash_dispatch_1_hndl->p_full = g_flash_dispatch_1_hndl->p_active;
                g_flash_dispatch_1_hndl->p_full->stat = FLASH_DM_BLOCK_STATUS_FULL;
                g_flash_dispatch_1_hndl->p_active = 0;
                r_flash_dm_block_set_program_addr(
                    /* cast from uint8_t to flash_dm_dh_ptr_t */
                    (flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_new_active->p_img->h.b.active[0]);
                r_flash_dm_link(FLASH_DM_RECLAIM_COPY_SETUP);
                FLASH_DM_JUMP(FLASH_DM_PROGRAM_BLOCK_FLAG);     /* Write the active flag. */
            }
            break;

            case FLASH_DM_RECLAIM_COPY_SETUP:       /* Prepare to data copy. */
            {
                g_flash_dispatch_1_hndl->p_active = g_flash_dispatch_1_hndl->p_new_active;
                g_flash_dispatch_1_hndl->p_new_active = 0;
                g_flash_dispatch_1_hndl->p_active->stat = FLASH_DM_BLOCK_STATUS_ACTIVE;
                g_flash_dispatch_1_hndl->counter = 0;
                g_flash_dispatch_1_hndl->p_reclaim \
                    = r_flash_dm_drvif_get_block_info(g_flash_dispatch_1_hndl->data_top);
                g_flash_dispatch_1_hndl->header_index = 0;
                g_flash_dispatch_1_hndl->p_data =
                    /* cast from uint32_t to uint8_t */
                    (flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_active->p_img->raw[FLASH_DM_FLASH_BLOCK_SIZE];
                r_flash_dm_link(FLASH_DM_RECLAIM_FINALIZE_SETUP);
                FLASH_DM_JUMP(FLASH_DM_COPY);
            }
            break;

            case FLASH_DM_RECLAIM_FINALIZE_SETUP:   /* Prepare to write reclaim flag. */
            {
                r_flash_dm_block_set_program_addr \
                /* cast from uint8_t to flash_dm_dh_ptr_t */
                ((flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_full->p_img->h.b.reclaim[0]);
                r_flash_dm_link(FLASH_DM_USER_RECLAIM_FINALIZE);
                FLASH_DM_JUMP(FLASH_DM_PROGRAM_BLOCK_FLAG);     /* Write the erase completion flag. */
            }
            break;

            case FLASH_DM_USER_RECLAIM_FINALIZE:    /* Finish reclaim.  */
            {
                g_flash_dispatch_1_hndl->data_top =
                    r_flash_dm_drvif_get_block_info(g_flash_dispatch_1_hndl->p_reclaim->next)->index;
                g_flash_dispatch_1_hndl->p_reclaim->stat = FLASH_DM_BLOCK_STATUS_GARBAGE;
                g_flash_dispatch_1_hndl->p_full = 0;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_FINISH_RECLAIM;
                b_ret = FLASH_DM_FINISH_RECLAIM;
                /* cast from uint8_t to uint8_t */
                uc_tmp = (g_flash_dispatch_1_hndl->activity & (~FLASH_DM_ACT_RECLAIMING));
                g_flash_dispatch_1_hndl->activity = uc_tmp;
                FLASH_DM_JUMP(FLASH_DM_TERMINATE);
            }
            break;

            case FLASH_DM_USER_RECLAIM_FAILURE:       /* Failed operation of reclaim. */
            {
                if (0 != g_flash_dispatch_1_hndl->p_full)
                {
                    if (0 != g_flash_dispatch_1_hndl->p_new_active)
                    {
                        g_flash_dispatch_1_hndl->p_new_active->stat = FLASH_DM_BLOCK_STATUS_GARBAGE;
                        g_flash_dispatch_1_hndl->p_reclaim = g_flash_dispatch_1_hndl->p_new_active;
                    }
                    else if (0 != g_flash_dispatch_1_hndl->p_active)
                    {
                        g_flash_dispatch_1_hndl->p_active->stat = FLASH_DM_BLOCK_STATUS_GARBAGE;
                        g_flash_dispatch_1_hndl->p_reclaim = g_flash_dispatch_1_hndl->p_active;
                    }
                    else
                    {
                        /* Do nothing */
                    }
                    g_flash_dispatch_1_hndl->p_active = g_flash_dispatch_1_hndl->p_full;
                    g_flash_dispatch_1_hndl->p_active->stat = FLASH_DM_BLOCK_STATUS_ACTIVE;
                }
                g_flash_dispatch_1_hndl->p_new_active = 0;
                g_flash_dispatch_1_hndl->p_full = 0;
                b_ret = FLASH_DM_ERR_RECLAIM;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_ERR_RECLAIM;
                /* cast from uint8_t to uint8_t */
                uc_tmp = (g_flash_dispatch_1_hndl->activity & (~FLASH_DM_ACT_RECLAIMING));
                g_flash_dispatch_1_hndl->activity = uc_tmp;
                FLASH_DM_JUMP(FLASH_DM_TERMINATE);
            }
            break;

            case FLASH_DM_COPY:     /* Copy data */
            {
                if (g_flash_dispatch_1_hndl->counter < r_flash_dm_drvif_get_datanum())
                {
                    g_flash_dispatch_1_hndl->dh.no = g_flash_dispatch_1_hndl->counter;
                    g_flash_dispatch_1_hndl->counter++;

                    /* Get the latest data address. */
                    g_flash_dispatch_1_hndl->p_user_data =
                        /* cast from uint8_t to uint8_t */
                        (const uint8_t *)r_flash_dm_drvif_get_dataaddr(g_flash_dispatch_1_hndl->dh.no);

                    /* The latest data check whether the reclaim block. */
                    if (((uint32_t)g_flash_dispatch_1_hndl->p_user_data & FLASH_DM_BLOCK_MAX_MASK) ==
                            /* cast from u_flash_dm_block_format_t to uint32_t */
                            (uint32_t)g_flash_dispatch_1_hndl->p_reclaim->p_img)
                    {
                        g_flash_dispatch_1_hndl->dh.addr \
                        /* cast from flash_dm_ptr_t to uint16_t */
                            = (uint16_t)g_flash_dispatch_1_hndl->p_data \
                              - r_flash_dm_drvif_get_datasize(g_flash_dispatch_1_hndl->dh.no);
                        r_flash_dm_drvif_set_addr_h(g_flash_dispatch_1_hndl);
                        r_flash_dm_link(FLASH_DM_COPY);
                        FLASH_DM_JUMP(FLASH_DM_WRITE_SETUP);
                    }
                    else
                    {
                        FLASH_DM_JUMP(FLASH_DM_COPY);
                    }
                }
                else
                {
                    g_flash_dispatch_1_hndl->counter = 0;
                    FLASH_DM_RETURN();
                }
            }
            break;

            /* ==== Format processing ==== */
            case FLASH_DM_FORMAT_SETUP:         /* Initializes the handler and global variables. */
            {
                g_flash_dispatch_1_hndl->activity = FLASH_DM_ACT_FORMATTING;
                /* cast from */
                if (0 == r_flash_dm_init_env(FLASH_DM_FORMATTING_ID, FLASH_DM_ACT_FORMATTING))
                {
                    FLASH_DM_JUMP(FLASH_DM_FORMAT_FAILURE);
                }
                else
                {
                    b_ret = FLASH_DM_SUCCESS;
                    g_flash_dispatch_1_hndl->error_code = FLASH_DM_SUCCESS;
                }

                /* ---- Set the initialization block status. ---- */
                g_flash_dispatch_1_hndl->p_new_active = r_flash_dm_drvif_get_block_info(0);

                /* Register block 0 as active block. */
                r_flash_dm_drvif_get_block_info(0)->stat = FLASH_DM_BLOCK_STATUS_ACTIVE;

                /* Register block 1 as initialization completion block. */
                r_flash_dm_drvif_get_block_info(1)->stat = FLASH_DM_BLOCK_STATUS_READY;

                /* Set the next of block 1 to the data_top. */
                g_flash_dispatch_1_hndl->data_top = r_flash_dm_drvif_get_block_info(1)->next;

                /* Register the other block as reclaim block. */
                /* WAIT_LOOP */
                for (i = 2; i < g_flash_dispatch_1_hndl->nblock; i++)
                {
                    r_flash_dm_drvif_get_block_info(i)->stat = FLASH_DM_BLOCK_STATUS_RECLAIM;
                }

                /* ---- Prepare the block format. ---- */
                /* Target for erase with block 1. */
                g_flash_dispatch_1_hndl->p_reclaim = r_flash_dm_drvif_get_block_info(1);
                /* cast from u_flash_dm_block_format_t to flash_dm_dh_ptr_t */
                r_flash_dm_block_set_program_addr((flash_dm_ptr_t)g_flash_dispatch_1_hndl->p_reclaim->p_img);
                r_flash_dm_try(FLASH_DM_FORMAT_FAILURE);
                r_flash_dm_link(FLASH_DM_FORMAT_FINALIZE);
                r_flash_dm_link(FLASH_DM_FORMAT_CREATE_BLOCK);
                /* cast from uint8_t to uint32_t */
                if ((*(flash_dm_t  *)((uint32_t)r_flash_dm_block_get_program_addr())) == FLASH_DM_BH_FLAG_FF)
                {
                    r_flash_dm_link(FLASH_DM_FORMAT_ERASE);
                    FLASH_DM_JUMP(FLASH_DM_PROGRAM_BLOCK_FLAG);     /* Write the erase start flag. */
                }
                else
                {
                    FLASH_DM_JUMP(FLASH_DM_FORMAT_ERASE);
                }
            }
            break;

            case FLASH_DM_FORMAT_ERASE:     /* Erase block. */
            {
                /* cast from u_flash_dm_block_format_t to flash_dm_ptr_t */
                r_flash_dm_block_set_program_addr((flash_dm_ptr_t)g_flash_dispatch_1_hndl->p_reclaim->p_img);
                r_flash_dm_block_erase_init();
                r_flash_dm_link(FLASH_DM_FORMAT_ERASE_FINALIZE_SETUP);
                r_flash_dm_link(FLASH_DM_ERASE_CHECK);
                FLASH_DM_JUMP(FLASH_DM_COMMAND);
            }
            break;

            case FLASH_DM_FORMAT_ERASE_FINALIZE_SETUP:  /* Prepare to finish erase. */
            {
                r_flash_dm_block_set_program_addr(
                    /* cast from uint8_t to flash_dm_ptr_t */
                    (flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_reclaim->p_img->h.b.erase_end[0]);
                r_flash_dm_link(FLASH_DM_PROGRAM_BLOCK_FLAG);   /* Write the erase completion flag. */
                FLASH_DM_JUMP(FLASH_DM_PROGRAM_BLOCK_ERASE_FLAG);
            }
            break;

            case FLASH_DM_FORMAT_CREATE_BLOCK:  /* Format each block. */
            {
                /* Set the next erase block. */
                g_flash_dispatch_1_hndl->p_reclaim \
                    = r_flash_dm_drvif_get_block_info(g_flash_dispatch_1_hndl->p_reclaim->next);
                if (g_flash_dispatch_1_hndl->p_reclaim != g_flash_dispatch_1_hndl->p_new_active)
                {
                    /* Finish all block erase */
                    r_flash_dm_link(FLASH_DM_FORMAT_CREATE_BLOCK);
                }

                if (g_flash_dispatch_1_hndl->p_reclaim->stat == FLASH_DM_BLOCK_STATUS_READY)
                {
                    /* In the case of the target block for erase is the block of initialization completion. */
                    /* Do nothing. */
                }
                else
                {
                    /* In the case of the target block for erase is the block of reclaim or active. */
                    g_flash_dispatch_1_hndl->header_index = 0;
                    g_flash_dispatch_1_hndl->p_active = g_flash_dispatch_1_hndl->p_reclaim;
                    if (g_flash_dispatch_1_hndl->p_reclaim->stat == FLASH_DM_BLOCK_STATUS_RECLAIM)
                    {
                        /* The target block for erase is the block of reclaim. */
                        r_flash_dm_link(FLASH_DM_PROGRAM_BLOCK_FLAG);   /* Write the reclaim flag. */
                        r_flash_dm_link(FLASH_DM_PROGRAM_BLOCK_FLAG);   /* Write the full flag.    */
                        r_flash_dm_link(FLASH_DM_PROGRAM_BLOCK_FLAG);   /* Write the active flag.  */
                    }
                    else
                    {
                        /* The target block for erase is the block of active. */
                        r_flash_dm_link(FLASH_DM_PROGRAM_BLOCK_FLAG);   /* Write the active flag.  */
                    }
                }
                /* cast from u_flash_dm_block_format_t to flash_dm_ptr_t */
                r_flash_dm_block_set_program_addr((flash_dm_ptr_t)g_flash_dispatch_1_hndl->p_reclaim->p_img);
                r_flash_dm_link(FLASH_DM_FORMAT_ERASE);
                FLASH_DM_JUMP(FLASH_DM_PROGRAM_BLOCK_FLAG);     /* Write the erase start flag. */
            }
            break;

            case FLASH_DM_FORMAT_FINALIZE:      /* Finish to format */
            {
                g_flash_dispatch_1_hndl->p_data =
                    /* cast from uint32_t to uint8_t */
                    (flash_dm_ptr_t)&g_flash_dispatch_1_hndl->p_active->p_img->raw[FLASH_DM_FLASH_BLOCK_SIZE];
                g_flash_dispatch_1_hndl->p_new_active = 0;
                g_flash_dispatch_1_hndl->p_reclaim = 0;
                /* cast from uint32_t to uint8_t */
                g_flash_dispatch_1_hndl->initialized = FLASH_DM_INITIALIZED_ID;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_FINISH_FORMAT;
                b_ret = FLASH_DM_FINISH_FORMAT;
                FLASH_DM_JUMP(FLASH_DM_TERMINATE);
            }
            break;

            case FLASH_DM_FORMAT_FAILURE:       /* Failed operation of format. */
            {
                g_flash_dispatch_1_hndl->initialized = 0;
                b_ret = FLASH_DM_ERR_FORMAT;
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_ERR_FORMAT;
                FLASH_DM_JUMP(FLASH_DM_TERMINATE);
            }
            break;

            /* ==== Error processing ==== */
            default:
            {
                g_flash_dispatch_1_hndl->error_code = FLASH_DM_ERR_REQUEST_INIT;
                b_ret = FLASH_DM_ERR_REQUEST_INIT;
                FLASH_DM_JUMP(FLASH_DM_TERMINATE);
            }
            break;
        }
    }

    return FLASH_DM_ERR_REQUEST_INIT;
} /* End of function r_flash_dm_dispatch() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_set_dataaddr
* Description  : Sets address of the number of data.
* Arguments    : uint8_t    data_no     ; The number of the data
*              : flash_dm_ptr_t p_addr  ; Pointer to the address
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_dm_drvif_set_dataaddr(uint8_t data_no, flash_dm_ptr_t p_addr)
{
    gs_flash_dm_data_addr[data_no] = p_addr;
} /* End of function r_flash_dm_drvif_set_dataaddr() */

/************************************************************************************************
* Function Name: r_flash_dm_1st_check
* Description  : Checks driver state.
* Arguments    : uint16_t   criteria        ; Current API
* Return Value : FLASH_DM_SUCCESS           ; Ready
*              : FLASH_DM_ERR_BUSY          ; Busy
*              : FLASH_DM_ERR_REQUEST_INIT  ; Failed operation and request initialization
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_1st_check(uint16_t criteria)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint8_t act = 0;
    uint8_t ini = 0;

    act = g_flash_dispatch_1_hndl->activity;
    ini = g_flash_dispatch_1_hndl->initialized;

    do
    {
        /* ==== In the case of calling initialization or format ==== */
        if (FLASH_DM_CHECK_INITIALIZED != (criteria & FLASH_DM_CHECK_INITIALIZED))
        {
            /* Check calling format API. */
            if (FLASH_DM_FORMATTING_ID == ini)
            {
                if (FLASH_DM_ACT_FORMATTING == act)
                {
                    ret = FLASH_DM_ERR_BUSY;    /* The driver is in active state. */
                    break;
                }
                else
                {
                    break;  /* Permit the API calling. */
                }
            }

            /* Check initialization completion */
            if (FLASH_DM_INITIALIZED_ID == ini)
            {
                if (((FLASH_DM_ACT_WRITING == act) || (FLASH_DM_ACT_RECLAIMING == act)) ||
                        (FLASH_DM_ACT_ERASING == act))
                {
                    ret = FLASH_DM_ERR_BUSY;    /* The driver is in active state. */
                    break;
                }
                else
                {
                    break;  /* Permit the API calling. */
                }
            }
            break;
        }

        /* ==== In the case of calling advance API ==== */
        if (FLASH_DM_CHECK_ADVANCE != (criteria & FLASH_DM_CHECK_ADVANCE))
        {
            /* Check initialization completion */
            if (FLASH_DM_INITIALIZED_ID == ini)
            {
                if (FLASH_DM_ACT_IDLE == act)
                {
                    break;  /* Permit the API calling. */
                }
                if (((FLASH_DM_ACT_WRITING == act) || (FLASH_DM_ACT_RECLAIMING == act)) ||
                        (FLASH_DM_ACT_ERASING == act))
                {
                    break;  /* Permit the API calling. */
                }
            }

            /* Check calling format API. */
            if (FLASH_DM_FORMATTING_ID == ini)
            {
                if (FLASH_DM_ACT_FORMATTING == act)
                {
                    break;  /* Permit the API calling. */
                }
            }
        }

        /* ==== In the case of calling read API ==== */
        if (FLASH_DM_CHECK_READ == (criteria & FLASH_DM_CHECK_READ))
        {
            /* Check initialization completion */
            if (FLASH_DM_INITIALIZED_ID == ini)
            {
                if ((((FLASH_DM_ACT_IDLE == act) || (FLASH_DM_ACT_WRITING == act)) ||
                        (FLASH_DM_ACT_RECLAIMING == act)) || (FLASH_DM_ACT_ERASING == act))
                {
                    break;  /* Permit the API calling. */
                }

                /* Check calling format API. */
                if (FLASH_DM_ACT_FORMATTING == act)
                {
                    ret = FLASH_DM_ERR_BUSY;    /* The driver is in active state. */
                    break;
                }
            }

            /* Check calling format API. */
            if (FLASH_DM_FORMATTING_ID == ini)
            {
                if (FLASH_DM_ACT_FORMATTING == act)
                {
                    ret = FLASH_DM_ERR_BUSY;    /* The driver is in active state. */
                    break;
                }
            }
        }

        /* ==== In the case of calling other API ==== */
        if ((criteria & FLASH_DM_CHECK_ACT_IDLE) != FLASH_DM_CHECK_ACT_IDLE)
        {
            /* Check initialization completion */
            if (FLASH_DM_INITIALIZED_ID == ini)
            {
                if (FLASH_DM_ACT_IDLE == act)
                {
                    break;  /* Permit the API calling. */
                }
                if (((FLASH_DM_ACT_WRITING == act) || (FLASH_DM_ACT_RECLAIMING == act)) ||
                        (FLASH_DM_ACT_ERASING == act))
                {
                    ret = FLASH_DM_ERR_BUSY;    /* The driver is in active state. */
                    break;
                }
                if (FLASH_DM_ACT_FORMATTING == act)
                {
                    ret = FLASH_DM_ERR_BUSY;    /* The driver is in active state. */
                    break;
                }
            }

            /* Check calling format API. */
            if (FLASH_DM_FORMATTING_ID == ini)
            {
                if (FLASH_DM_ACT_FORMATTING == act)
                {
                    ret = FLASH_DM_ERR_BUSY;    /* The driver is in active state. */
                    break;
                }
            }
        }
        ret = FLASH_DM_ERR_REQUEST_INIT;
        break;
    }
    while(0);

    return ret;
} /* End of function r_flash_dm_1st_check() */

/************************************************************************************************
* Function Name: r_flash_dm_delaywait
* Description  : This asm loop executes a known number (5) of CPU cycles.
*              ; If a value of '4' is passed in as an argument,
*              ; then this function would consume 20 CPU cycles before returning.
* Arguments    : unsigned long      loop_cnt            ;   The number of 'units' to delay.
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
#pragma inline_asm r_flash_dm_delaywait
static void r_flash_dm_delaywait (unsigned long loop_cnt)
{
    BRA ?+
    NOP
    ?:
    NOP
    SUB #01H, R1
    BNE ?-
} /* End of function r_flash_dm_delaywait() */

/************************************************************************************************
* Function Name: r_flash_dm_softwaredelay
* Description  : Delay the specified duration in units and return.
* Arguments    : uint32_t           delay               ;   The number of 'units' to delay.
*              : e_flash_dm_delay_units_t  units          ;   the 'base' for the units specified.
*              :                                        ;   Valid values are: FLASH_DM_DELAY_MICROSECS,
*              :                                        ;                     FLASH_DM_DELAY_MILLISECS,
*              :                                        ;                     FLASH_DM_DELAY_SECS
* Return Value : true           ;   if delay executed.
*              : false          ;   if delay/units combination resulted in overflow/underflow.
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
bool r_flash_dm_softwaredelay(uint32_t delay, e_flash_dm_delay_units_t units)
{
    uint32_t loop_cnt = 0;

    /* Calculate the number of loops, accounting for the overhead of 20 cycles (4 loops at 5 cycles/loop) */
    loop_cnt = (((uint32_t)delay * (uint32_t)BSP_ICLK_HZ) / (FLASH_DM_CPU_CYCLES_PER_LOOP * units))
               - FLASH_DM_OVERHEAD_LOOP_COUNT;

    /* Make sure the request is valid and did not result in an overflow. */
    if ((((FLASH_DM_DELAY_MILLISECS != units) && (FLASH_DM_DELAY_SECS != units)) || (0xFFFFFFFF < loop_cnt)) ||
            (0 == loop_cnt))
    {
        return (false);
    }
    /* cast from uint32_t to uint32_t */
    r_flash_dm_delaywait((uint32_t)loop_cnt);
    return (true);
} /* End of function r_flash_dm_softwaredelay() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_dataaddr
* Description  : Gets address of the number of data.
* Arguments    : uint8_t    data_no     ; The data no.
* Return Value : Pointer to address of the number of data
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_dm_ptr_t r_flash_dm_drvif_get_dataaddr(uint8_t data_no)
{
    return gs_flash_dm_data_addr[data_no];
} /* End of function r_flash_dm_drvif_get_dataaddr() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_dataaddr_of_addr
* Description  : Gets address of data address.
* Arguments    : uint8_t    data_no     ; The data no.
* Return Value : Pointer to address of data address.
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t r_flash_dm_drvif_get_dataaddr_of_addr(uint8_t data_no)
{
    /* cast from uint32_t* to uint32_t */
    return (uint32_t)&gs_flash_dm_data_addr[data_no];
} /* End of function r_flash_dm_drvif_get_dataaddr_of_addr() */

/************************************************************************************************
* Function Name: r_flash_dm_block_set_program_addr
* Description  : Sets block program address.
* Arguments    : flash_dm_ptr_t     p_addr  ; Pointer of the start address of destination
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_dm_block_set_program_addr(flash_dm_ptr_t p_addr)
{
    g_flash_dispatch_1_hndl->flash_dm_a_addr = p_addr;
} /* End of function r_flash_dm_block_set_program_addr() */

/************************************************************************************************
* Function Name: r_flash_dm_block_get_program_addr
* Description  : Gets block program address.
* Arguments    : None
* Return Value : Pointer of the start address of destination
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_dm_ptr_t r_flash_dm_block_get_program_addr(void)
{
    return g_flash_dispatch_1_hndl->flash_dm_a_addr;
} /* End of function r_flash_dm_block_get_program_addr() */


#if defined(FLASH_DM_CHECK_REJECTED_BLOCK)
/************************************************************************************************
* Function Name: r_flash_dm_reject_block
* Description  : Rejects the block.
* Arguments    : uint8_t        block_num               ;   The block no.
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static void r_flash_dm_reject_block(uint8_t block_num)
{
    st_flash_dm_block_t * p_block = 0;

    g_flash_dispatch_1_hndl->nblock--;
    p_block = r_flash_dm_drvif_get_block_info(block_num);
    p_block->stat = FLASH_DM_BLOCK_STATUS_BAD;
    r_flash_dm_drvif_get_block_info(p_block->next)->prev = p_block->prev;
    r_flash_dm_drvif_get_block_info(p_block->prev)->next = p_block->next;
    return (true);
}
#endif /* defined(FLASH_DM_CHECK_REJECTED_BLOCK) */


/************************************************************************************************
* Function Name: r_flash_dm_drvif_flash_busy
* Description  : Checks busy.
* Arguments    : None
* Return Value : 1: Busy
*              : 0: Ready or error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
uint8_t r_flash_dm_drvif_flash_busy(void)
{
#if ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)) || \
    ((FLASH_CFG_CODE_FLASH_ENABLE == 0) && (FLASH_CFG_DATA_FLASH_BGO == 1))
    flash_err_t ret = FLASH_SUCCESS;

    ret = R_FLASH_Control(FLASH_CMD_STATUS_GET, NULL);
    if (FLASH_SUCCESS == ret)
    {
        return 0;   /* Ready */
    }
    else if (FLASH_ERR_BUSY != ret)
    {
        g_flash_dm_err_flg = 1;
        return 0;   /* Error */
    }
    else
    {
        /* Busy */
    }
    return 1;   /* Busy */
#else
    return 0;   /* Ready */
#endif /* ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)) || \
          ((FLASH_CFG_CODE_FLASH_ENABLE == 0) && (FLASH_CFG_DATA_FLASH_BGO == 1)) */
} /* End of function r_flash_dm_drvif_flash_busy() */

/************************************************************************************************
* Function Name: r_flash_dm_set_dispatch
* Description  : Copy the handle's address.
* Arguments    : uint32_t   handle   ; Current handle
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : Note.
*************************************************************************************************/
void r_flash_dm_set_dispatch(uint32_t hndl)
{
    /* cast from uint32_t to st_flash_dispatch_1_hndl_t* */
    g_flash_dispatch_1_hndl = (st_flash_dispatch_1_hndl_t*)hndl;
}

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)
#pragma section
#endif

#endif /* (FLASH_TYPE == FLASH_TYPE_1) */

/* End of File */
