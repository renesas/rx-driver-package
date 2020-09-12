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
* File Name    : r_dispatch_2.c
* Version      : 2.01
* Description  : DATFRX interface source file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*              : 25.01.2019 2.01     English PDF added, Fixed blank check processing and Busy check procedure
*************************************************************************************************/

/***********************************************************************************************
Includes   <System Includes> , "Project Includes"
************************************************************************************************/
#include "r_dispatch_2.h"

#if(FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)
/***********************************************************************************************
Macro definitions
************************************************************************************************/
#define DATF_JUMP(PC)           pc = PC
#define DATF_UNWIND()           --g_flash_dispatch_2_hndl->sp
#define DATF_RETURN()           DATF_UNWIND(); \
    DATF_JUMP(g_flash_dispatch_2_hndl->stack[g_flash_dispatch_2_hndl->sp])
#define DATF_AGAIN()            ++g_flash_dispatch_2_hndl->sp
#define DATF_LINK(PC)           g_flash_dispatch_2_hndl->stack[g_flash_dispatch_2_hndl->sp] = PC; \
    ++g_flash_dispatch_2_hndl->sp
#define DATF_TRY(CODE)          DATF_LINK(CODE); DATF_LINK(DATF_CATCH_ERROR)
#define    DATF_EXIT_DISPATCH() return ret
#define    DATF_EXIT(X)         return X
#define assert(TEST)

/************************************************************************************************
Local Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
/* driver's environment, visible from user system when datf_internal.h is included */

/***********************************************************************************************
Private global variables and functions
************************************************************************************************/
st_flash_dispatch_2_hndl_t* (g_flash_dispatch_2_hndl);
static st_flash_dispatch_2_hndl_blk_info_t g_flash_dispatch_2_handle_blk_info[FLASH_DM_CFG_DF_BLOCK_NUM];
static u_flash_dispatch_2_hndl_block_t* (g_flash_dispatch_2_handle_blocks);
static uint16_t    gs_max_serial_no_blk;
const  uint32_t    gcDatf_block_flg_data = 0x00000000;

/************************************************************************************************
* Function Name: r_flash_dm_dispatch
* Description  : Control DATFRX.
* Arguments    : int8_t pc              ; Program counter of Starting  process
* Return values: FLASH_DM_ACCEPT        ; Successful operation reception
*              : FLASH_DM_FINISH_FORMAT ; Successful operation and finish format
*              : FLASH_DM_FINISH_WRITE  ; Successful operation and finish write
*              : FLASH_DM_FINISH_ERASE  ; Successful operation and finish erase
*              : FLASH_DM_ERR_FORMAT    ; Failed operation of format
*              : FLASH_DM_ERR_WRITE     ; Failed operation of write
*              : FLASH_DM_ERR_ERASE     ; Failed operation of erase
*              : FLASH_DM_ERR_REQUEST_INIT ; Failed operation and request initialization
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_dispatch(uint8_t pc)
{
    uint8_t datf_command_id = 0;
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;
    uint16_t data_size = 0;
    uint16_t next_block_no = 0;
    flash_res_t result = FLASH_RES_BLANK;

    /* WAIT_LOOP */
    for (;;)
    {
        switch (pc)
        {
            case DATF_RESTART:
            {
                if (g_flash_dispatch_2_hndl->sp > 0)
                {
                    g_flash_dispatch_2_hndl->sp--;
                }
                DATF_JUMP(g_flash_dispatch_2_hndl->stack[g_flash_dispatch_2_hndl->sp]);
            }
            break;

            case DATF_IDLE_NORMAL:
            {
                DATF_EXIT(FLASH_DM_SUCCESS);
            }
            break;

            case DATF_TERMINATE:
            {
                g_flash_dispatch_2_hndl->sp = 0;
                g_flash_dispatch_2_hndl->stack[0] = DATF_IDLE_NORMAL;
                g_flash_dispatch_2_hndl->flash_state = DATF_FLASH_IDLE;
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_2_hndl->activity = FLASH_DM_ACT_IDLE;
                if(0 != r_flash_dm_callbackfunc)
                {
                    /* cast from uint16_t to uint32_t* */
                    r_flash_dm_callbackfunc((void*)g_flash_dispatch_2_hndl->error_code);
                }
                DATF_EXIT_DISPATCH();
            }
            break;

            case DATF_RAISE:
            {
                /* WAIT_LOOP */
                while (g_flash_dispatch_2_hndl->stack[g_flash_dispatch_2_hndl->sp] != DATF_CATCH_ERROR)
                {
                    g_flash_dispatch_2_hndl->sp--;
                }
                DATF_RETURN();
            }
            break;

            case DATF_ERASE_CMD_SETTING:
            {
                datf_command_id = DATF_ERASE_CMD;
                /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                g_flash_dispatch_2_hndl->command_addr \
                    = (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no];
                DATF_JUMP(DATF_COMMAND);
            }
            break;

            case DATF_PROGRAM_DATA_CMD_SETTING:
            {
                g_flash_dispatch_2_hndl->program_counter = \
                    ((g_flash_dispatch_2_hndl->program_counter + BLANK_ROUND_NUM) / FLASH_DF_MIN_PGM_SIZE) \
                    * FLASH_DF_MIN_PGM_SIZE;
                datf_command_id = DATF_PROGRAM_CMD;
                DATF_LINK(DATF_PROGRAM_CHECK);
                DATF_JUMP(DATF_COMMAND);
            }
            break;

            case DATF_BLANK_CMD_SETTING:
            {
                datf_command_id = DATF_BLANK_CHECK_CMD;
                DATF_LINK(DATF_BLANK_CHECK);
                DATF_JUMP(DATF_COMMAND);
            }
            break;

            case DATF_COMMAND:
            {
                /* cast from uint8_t to e_dm_cmd_t */
                ret = r_flash_dm_command((e_dm_cmd_t)datf_command_id, &result);
                DATF_EXIT_DISPATCH();
            }
            break;

            case DATF_PROGRAM_CHECK:
#if(FLASH_TYPE == FLASH_TYPE_2)
                if(*(volatile uint16_t *)g_flash_dispatch_2_hndl->command_addr != *g_flash_dispatch_2_hndl->program_data)
#else
                if(*(volatile uint32_t *)g_flash_dispatch_2_hndl->command_addr != *g_flash_dispatch_2_hndl->program_data)
#endif
                {
                    DATF_JUMP(DATF_ERROR_PROCESS);
                }
                else
                {
                    DATF_JUMP(DATF_PROGRAM_NEXT_SETTING);
                }
                break;

            case DATF_ERROR_PROCESS:
            {
                /* cast from uint8_t to uint8_t */
                if(g_flash_dispatch_2_hndl->activity == FLASH_DM_ACT_FORMATTING)
                {
                    DATF_RETURN();
                }
                else
                {
                    DATF_JUMP(DATF_RAISE);
                }
            }
            break;

            case DATF_BLANK_CHECK:
            {
                if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_BLANK)
                {
                    DATF_RETURN();
                }
                else
                {
                    /* cast from uint32_t to uint32_t */
                    if(g_flash_dispatch_2_hndl->command_addr == (uint32_t) \
                            &g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.erase_start)
                    {
                        DATF_UNWIND();
                        DATF_JUMP(DATF_PROGRAM_NEXT_SETTING);
                    }
                    else
                    {
                        DATF_JUMP(DATF_RAISE);
                    }
                }
            }
            break;

            case DATF_PROGRAM_NEXT_SETTING:
                DATF_RETURN();
                break;

            case DATF_USER_FORMAT:
            {
                /* cast from uint32_t to u_flash_dispatch_2_hndl_block_t* */
                g_flash_dispatch_2_handle_blocks = (u_flash_dispatch_2_hndl_block_t *)FLASH_DF_BLOCK_0;
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_2_hndl->initialized = FLASH_DM_FORMATTING_ID;
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_2_hndl->activity = FLASH_DM_ACT_FORMATTING;
                g_flash_dispatch_2_hndl->sp = 0;
                g_flash_dispatch_2_hndl->block_no = 0;
                g_flash_dispatch_2_hndl->invalid_block_num = 0;
                g_flash_dispatch_2_hndl->erase_block_num = 0;

                DATF_TRY(DATF_FORMAT_FAILURE);
                DATF_LINK(DATF_FORMAT_FINALIZE);
                DATF_JUMP(DATF_FORMAT_SETTING);
            }
            break;

            case DATF_FORMAT_SETTING:
            {
                if(g_flash_dispatch_2_hndl->block_no < (FLASH_DM_CFG_DF_BLOCK_NUM - 1))
                {
                    DATF_LINK(DATF_FORMAT_NEXT_BLK_SETTING);
                    DATF_LINK(DATF_PROGRAM_ERASE_END_FLAG);
                }
                DATF_LINK(DATF_ERASE_CMD_SETTING);
                DATF_JUMP(DATF_PROGRAM_ERASE_START_FLAG);
            }
            break;

            case DATF_FORMAT_NEXT_BLK_SETTING:
            {
                g_flash_dispatch_2_hndl->block_no++;
                DATF_JUMP(DATF_FORMAT_SETTING);
            }
            break;

            case DATF_FORMAT_FINALIZE:
            {
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_2_hndl->initialized = FLASH_DM_FORMATTING_ID;
                g_flash_dispatch_2_hndl->error_code = FLASH_DM_FINISH_FORMAT;
                DATF_JUMP(DATF_TERMINATE);
            }
            break;

            case DATF_FORMAT_FAILURE:
            {
                g_flash_dispatch_2_hndl->initialized = 0;
                g_flash_dispatch_2_hndl->error_code = FLASH_DM_ERR_FORMAT;
                DATF_JUMP(DATF_TERMINATE);
            }
            break;

            case DATF_USER_WRITE:
            {
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_2_hndl->activity = FLASH_DM_ACT_WRITING;
                DATF_TRY(DATF_WRITE_FAILURE);
                DATF_LINK(DATF_WRITE_FINALIZE);
                /* cast from uint16_t to uint16_t */
                g_flash_dispatch_2_hndl->block_num \
                    = (uint16_t)(((((gc_dm_data_size[g_flash_dispatch_2_hndl->data_no] \
                                     + DATF_F2_DATASIZE) - DATF_F1_DATASIZE) - 1) \
                                  / DATF_F2_DATASIZE) + 1);
                g_flash_dispatch_2_hndl->writeblock_cnt = 1;

                if(g_flash_dispatch_2_hndl->block_num == 1)
                {
                    g_flash_dispatch_2_hndl->b.bf.data_type = 8;
                    data_size = gc_dm_data_size[g_flash_dispatch_2_hndl->data_no];
                    g_flash_dispatch_2_hndl->program_counter = DATF_F1_INFO_SIZE + data_size;
                    DATF_JUMP(DATF_FORMAT1_PROGRAM_SETUP);
                }
                else
                {
                    g_flash_dispatch_2_hndl->b.bf.data_type = 4;
                    data_size = (gc_dm_data_size[g_flash_dispatch_2_hndl->data_no] \
                                 - DATF_F1_DATASIZE) % DATF_F2_DATASIZE;
                    if(data_size == 0)
                    {
                        data_size = DATF_F2_DATASIZE;
                    }
                    g_flash_dispatch_2_hndl->program_counter = DATF_F2_INFO_SIZE + data_size;
                    DATF_JUMP(DATF_FORMAT2_PROGRAM_SETUP);
                }
                g_flash_dispatch_2_hndl->b.bf.chain = 0xffff;
                g_flash_dispatch_2_hndl->user_data += \
                                                      (gc_dm_data_size[g_flash_dispatch_2_hndl->data_no] \
                                                       - data_size);
                g_flash_dispatch_2_hndl->block_no = g_flash_dispatch_2_hndl->erase_chain_first;
                /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                g_flash_dispatch_2_hndl->command_addr = (uint32_t) \
                                                        &g_flash_dispatch_2_handle_blocks \
                                                        [g_flash_dispatch_2_hndl->block_no];
                /* cast from uint32_t* to uint8_t* */
                (uint8_t *)g_flash_dispatch_2_hndl->program_data  = &(g_flash_dispatch_2_hndl->b.raw[0]);
            }
            break;

            case DATF_FORMAT1_PROGRAM_SETUP:
            {
                g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no = g_flash_dispatch_2_hndl->data_no;
                ++g_flash_dispatch_2_hndl->max_serial_no;
                g_flash_dispatch_2_hndl->b.bf.ft.f1.ser_no = (g_flash_dispatch_2_hndl->max_serial_no);
                r_flash_dm_data_set(&(g_flash_dispatch_2_hndl->b.bf.ft.f1.data[0]), \
                                    g_flash_dispatch_2_hndl->user_data, \
                                    g_flash_dispatch_2_hndl->program_counter);
                /* cast from uint16_t to uint16_t */
                g_flash_dispatch_2_hndl->b.bf.ft.f1.crc_ccitt \
                    = (uint16_t)(~r_datf_crc16( &((uint8_t)g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no) \
                                                , DATF_F1_CRC_CALC_SIZE ));
                DATF_LINK(DATF_PROGRAM_WRITE_END_FLAG);
                DATF_JUMP(DATF_PROGRAM_DATA_CMD_SETTING);
            }
            break;

            case DATF_FORMAT2_PROGRAM_SETUP:
            {
                r_flash_dm_data_set(&(g_flash_dispatch_2_hndl->b.bf.ft.f2.data[0]) \
                                    , g_flash_dispatch_2_hndl->user_data \
                                    , g_flash_dispatch_2_hndl->program_counter);
                DATF_LINK(DATF_PROGRAM_NEXT_BLOCK);
                DATF_JUMP(DATF_PROGRAM_DATA_CMD_SETTING);
            }
            break;

            case DATF_PROGRAM_WRITE_END_FLAG:
            {
                /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                g_flash_dispatch_2_hndl->command_addr = (uint32_t) \
                                                        &g_flash_dispatch_2_handle_blocks \
                                                        [g_flash_dispatch_2_hndl->block_no].bf.ft.f1.write_end;
                DATF_LINK(DATF_PROGRAM_DATA_CMD_SETTING);
                DATF_JUMP(DATF_BLOCK_FLAG_PROGRAM_SETTING);
            }
            break;

            case DATF_PROGRAM_NEXT_BLOCK:
            {
                g_flash_dispatch_2_hndl->block_num--;
                g_flash_dispatch_2_hndl->writeblock_cnt++;
                next_block_no = g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain;
                g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain \
                    = g_flash_dispatch_2_hndl->b.bf.chain;
                g_flash_dispatch_2_hndl->b.bf.chain = g_flash_dispatch_2_hndl->block_no;
                g_flash_dispatch_2_hndl->block_no = next_block_no;
                /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                g_flash_dispatch_2_hndl->command_addr \
                    = (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no];
                /* cast from uint32_t* to uint8_t */
                (uint8_t *)g_flash_dispatch_2_hndl->program_data  = &(g_flash_dispatch_2_hndl->b.raw[0]);

                if(g_flash_dispatch_2_hndl->block_num == 1)
                {
                    g_flash_dispatch_2_hndl->b.bf.data_type = 1;
                    data_size = DATF_F1_DATASIZE;
                    g_flash_dispatch_2_hndl->user_data -= DATF_F1_DATASIZE;
                    g_flash_dispatch_2_hndl->program_counter = DATF_F1_INFO_SIZE + data_size;
                    DATF_JUMP(DATF_FORMAT1_PROGRAM_SETUP);
                }
                else
                {
                    g_flash_dispatch_2_hndl->b.bf.data_type = 2;
                    data_size = DATF_F2_DATASIZE;
                    g_flash_dispatch_2_hndl->user_data -= DATF_F2_DATASIZE;
                    g_flash_dispatch_2_hndl->program_counter = DATF_F2_INFO_SIZE + data_size;
                    DATF_JUMP(DATF_FORMAT2_PROGRAM_SETUP);
                }
            }
            break;

            case DATF_WRITE_FINALIZE:
            {
                r_flash_dm_info_update_w_pass();
                g_flash_dispatch_2_hndl->error_code = FLASH_DM_FINISH_WRITE;
                DATF_JUMP(DATF_TERMINATE);
            }
            break;

            case DATF_WRITE_FAILURE:
            {
                r_flash_dm_info_update_w_error();
                g_flash_dispatch_2_hndl->error_code = FLASH_DM_ERR_WRITE;
                DATF_JUMP(DATF_TERMINATE);
            }
            break;

            case DATF_USER_ERASE:
            {
                /* cast from uint8_t to uint8_t */
                g_flash_dispatch_2_hndl->activity = FLASH_DM_ACT_ERASING;
                DATF_TRY(DATF_ERASE_FAILURE);
                DATF_LINK(DATF_ERASE_FINALIZE);
                g_flash_dispatch_2_hndl->block_no = g_flash_dispatch_2_hndl->invalid_chain_first;
                DATF_LINK(DATF_PROGRAM_ERASE_END_FLAG);
                DATF_LINK(DATF_ERASE_CMD_SETTING);
                DATF_JUMP(DATF_PROGRAM_ERASE_START_FLAG);
            }
            break;

            case DATF_PROGRAM_ERASE_START_FLAG:
            {
                /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                g_flash_dispatch_2_hndl->command_addr \
                    = (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.erase_start;
                DATF_LINK(DATF_PROGRAM_DATA_CMD_SETTING);
                DATF_LINK(DATF_BLANK_CMD_SETTING);
                DATF_JUMP(DATF_BLOCK_FLAG_PROGRAM_SETTING);
            }
            break;

            case DATF_PROGRAM_ERASE_END_FLAG:
            {
                /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                g_flash_dispatch_2_hndl->command_addr \
                    = (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.erase_end;
                DATF_LINK(DATF_PROGRAM_DATA_CMD_SETTING);
                DATF_JUMP(DATF_BLOCK_FLAG_PROGRAM_SETTING);
            }
            break;

            case DATF_BLOCK_FLAG_PROGRAM_SETTING:
            {
#if(FLASH_TYPE == FLASH_TYPE_2)
                g_flash_dispatch_2_hndl->program_data = &(uint16_t)(gcDatf_block_flg_data);
#else
                g_flash_dispatch_2_hndl->program_data = &(uint32_t)(gcDatf_block_flg_data);
#endif
                g_flash_dispatch_2_hndl->program_counter = FLASH_DF_MIN_PGM_SIZE;
                DATF_RETURN();
            }
            break;

            case DATF_ERASE_FINALIZE:
            {
                g_flash_dispatch_2_hndl->erase_chain_last = g_flash_dispatch_2_hndl->invalid_chain_first;
                g_flash_dispatch_2_hndl->invalid_chain_first \
                    = g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->invalid_chain_first].chain;
                g_flash_dispatch_2_hndl->erase_block_num ++;
                g_flash_dispatch_2_hndl->invalid_block_num --;
                g_flash_dispatch_2_hndl->error_code = FLASH_DM_FINISH_ERASE;
                DATF_JUMP(DATF_TERMINATE);
            }
            break;

            case DATF_ERASE_FAILURE:
            {
                g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->invalid_chain_last].chain \
                    = g_flash_dispatch_2_hndl->invalid_chain_first;
                g_flash_dispatch_2_hndl->invalid_chain_last = g_flash_dispatch_2_hndl->invalid_chain_first;
                g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->erase_chain_last].chain \
                    = g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->invalid_chain_first].chain;
                g_flash_dispatch_2_hndl->invalid_chain_first \
                    = g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->invalid_chain_first].chain;
                g_flash_dispatch_2_hndl->error_code = FLASH_DM_ERR_ERASE;
                DATF_JUMP(DATF_TERMINATE);
            }
            break;

            default:
            {
                g_flash_dispatch_2_hndl->error_code = FLASH_DM_ERR_REQUEST_INIT;
                DATF_JUMP(DATF_TERMINATE);
            }
            break;
    }
    }

    ret = g_flash_dispatch_2_hndl->error_code;
    return ret;
}

/************************************************************************************************
* Function Name: r_flash_dm_info_update_w_pass
* Description  : Update block informations when IDLE process is pass.
* Arguments    : void
* Return values: void
*------------------------------------------------------------------------------------------------
* Notes        : Update informations of block chain.
*              : Update numbers of DATF_STATUS_ERASE block
*              : Update numbers of invalid block
*************************************************************************************************/
void r_flash_dm_info_update_w_pass(void)
{
    uint8_t data_type = 0;
    uint16_t old_data_block_no = 0;

    g_flash_dispatch_2_hndl->erase_block_num -= g_flash_dispatch_2_hndl->writeblock_cnt;

    if(g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no] != 0xFFFF)
    {
        old_data_block_no = g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no];
        g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->invalid_chain_last].chain = old_data_block_no;
        /* WAIT_LOOP */
        for(;;)
        {
            g_flash_dispatch_2_hndl->invalid_block_num++;
            data_type = g_flash_dispatch_2_handle_blocks[old_data_block_no].bf.data_type;
            if((data_type == 4) || (data_type == 8))
            {
                g_flash_dispatch_2_hndl->invalid_chain_last = old_data_block_no;
                break;
            }
            old_data_block_no = g_flash_dispatch_2_handle_blk_info[old_data_block_no].chain;
        }
    }
    g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no] = g_flash_dispatch_2_hndl->block_no;
    g_flash_dispatch_2_hndl->erase_chain_first \
        = g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain;

    g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain = g_flash_dispatch_2_hndl->b.bf.chain;
}

/************************************************************************************************
* Function Name: r_flash_dm_info_update_w_error
* Description  : Update block informations when IDLE error is occurred.
* Arguments    : void
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : Update informations of block chain
 *             : Update numbers of DATF_STATUS_ERASE block
 *             : Update numbers of invalid block
*************************************************************************************************/
void r_flash_dm_info_update_w_error(void)
{
    g_flash_dispatch_2_hndl->erase_block_num -= g_flash_dispatch_2_hndl->writeblock_cnt;
    g_flash_dispatch_2_hndl->invalid_block_num += g_flash_dispatch_2_hndl->writeblock_cnt;

    g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->invalid_chain_last].chain \
        = g_flash_dispatch_2_hndl->block_no;
    g_flash_dispatch_2_hndl->invalid_chain_last = g_flash_dispatch_2_hndl->erase_chain_first;
    g_flash_dispatch_2_hndl->erase_chain_first \
        = g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain;
    g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain \
        = g_flash_dispatch_2_hndl->b.bf.chain;

}

/************************************************************************************************
* Function Name: r_flash_dm_data_set
* Description  : setting of user data
* Arguments    : uint8_t *p_Dest        : Pointer of destination address
 *             : const uint8_t *pData   : Pointer of source address
 *             : uint16_t data_size : Data size of use data
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : setting of user data to the destination address.
*************************************************************************************************/
void r_flash_dm_data_set(uint8_t *p_Dest, const uint8_t *pData, uint16_t data_size)
{
    uint16_t data_set_size = 0;

    if(g_flash_dispatch_2_hndl->block_num == 1)
    {
        data_set_size = DATF_F1_DATASIZE;
    }
    else
    {
        data_set_size = DATF_F2_DATASIZE;
    }

    /* WAIT_LOOP */
    do
    {
        if(data_size > 0)
        {
            *p_Dest++ = *pData++;
            data_size--;
        }
        else
        {
            /* cast from uint8_t to uint8_t */
            *p_Dest = (uint8_t)0xff;
            break;
        }
    }
    while((--data_set_size) > 0);
}

/************************************************************************************************
* Function Name: r_flash_dm_drvif_read
* Description  : Reads data from the device.
* Arguments    : st_flash_dm_info_t * p_flash_dm_info
*              :    uint8_t     data_no         ; The data no.
*              :    uint8_t   * p_data          ; Pointer to read data buffer
* Return Value : none
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
void r_flash_dm_drvif_read(st_flash_dm_info_t * p_flash_dm_info)
{
    uint8_t *flash_data_ptr = 0;
    uint16_t data_size = 0;
    uint16_t cnt = 0;
    uint16_t block_no = 0;

    data_size = gc_dm_data_size[p_flash_dm_info->data_no];
    if (data_size > DATF_F1_DATASIZE)
    {
        cnt = DATF_F1_DATASIZE;
    }
    else
    {
        cnt = data_size;
    }

    block_no = g_flash_dispatch_2_hndl->data_info[p_flash_dm_info->data_no];
    flash_data_ptr = &g_flash_dispatch_2_handle_blocks[block_no].bf.ft.f1.data[0];

    data_size -= cnt;

    /* WAIT_LOOP */
    while((cnt--) > 0)
    {
        *p_flash_dm_info->p_data++ = *flash_data_ptr++ ;
    }

    /* WAIT_LOOP */
    while(data_size > 0)
    {
        if (data_size > DATF_F2_DATASIZE)
        {
            cnt =   DATF_F2_DATASIZE;
        }
        else
        {
            cnt =   data_size;
        }
        block_no = g_flash_dispatch_2_handle_blk_info[block_no].chain;
        flash_data_ptr = &g_flash_dispatch_2_handle_blocks[block_no].bf.ft.f2.data[0];

        data_size -= cnt;

        /* WAIT_LOOP */
        while((cnt--) > 0)
        {
            *p_flash_dm_info->p_data++ = *flash_data_ptr++ ;
        }
    }
}

/************************************************************************************************
* Function Name: r_flash_dm_init_set
* Description  : Driver initialization 1
* Arguments    : void
* Return Value : FLASH_DM_ERR_BUSY
 *             : FLASH_DM_ERR_REQUEST_INIT
 *             : FLASH_DM_ADVANCE
*------------------------------------------------------------------------------------------------
* Notes        :  Initializes data flash driver
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_init_set(void)
{
    uint32_t    block_addr = 0;
    uint16_t    crc_generated = 0;
    uint8_t     data_type = 0;
    /* cast from uint8_t and uint16_t to uint8_t and uint16_t */
    st_flash_dispatch_2_hndl_blk_info_t blk_info_init = {(uint8_t)0, (uint16_t)0};
    e_flash_dm_status_t ret = FLASH_DM_ADVANCE;
    g_flash_dispatch_2_hndl->sp = 0;
    g_flash_dispatch_2_hndl->stack[0] = DATF_IDLE_NORMAL;
    gs_max_serial_no_blk = 0;
    /* initialize block information */
    /* cast from uint16_t to uint16_t */
    g_flash_dispatch_2_hndl->erase_block_num = (uint16_t)0;
    /* cast from uint16_t to uint16_t */
    g_flash_dispatch_2_hndl->invalid_block_num = (uint16_t)0;
    /* cast from uint16_t to uint16_t */
    g_flash_dispatch_2_hndl->writeblock_cnt = (uint16_t)0;
    /* cast from uint32_t to uint32_t */
    g_flash_dispatch_2_hndl->max_serial_no = (uint32_t)0;
    /* block base address setting */
    g_flash_dispatch_2_handle_blocks = (u_flash_dispatch_2_hndl_block_t *)FLASH_DF_BLOCK_0;
    /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
    g_flash_dispatch_2_hndl->command_addr = (uint32_t)g_flash_dispatch_2_handle_blocks;

    /* initialize table */
    /* WAIT_LOOP */
    for(g_flash_dispatch_2_hndl->data_no = 0; \
            g_flash_dispatch_2_hndl->data_no < FLASH_DM_CFG_DF_DATA_NUM; \
            g_flash_dispatch_2_hndl->data_no++)
    {
        g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no]        = 0xFFFF;
    }
    g_flash_dispatch_2_hndl->block_no = 0;
    g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no]   = blk_info_init;
    /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
    block_addr = (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no];
    g_flash_dispatch_2_hndl->command_addr = block_addr;
    /* cast from e_dm_cmd_t to e_dm_cmd_t */
    ret = r_flash_dm_command((e_dm_cmd_t)DATF_BLANK_CHECK_CMD, NULL);
    if(ret == FLASH_DM_ACCEPT)
    {
        ret = FLASH_DM_ADVANCE;
        g_flash_dispatch_2_hndl->command_addr += FLASH_DF_MIN_PGM_SIZE;
        if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
        {
            ret = FLASH_DM_ERR_REQUEST_INIT;
        }
        else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_BLANK)
        {
            r_flash_dm_erase_block_blank_chk(block_addr);
            if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
            {
                ret = FLASH_DM_ERR_REQUEST_INIT;
            }
            else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_PASS)
            {
                g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type = DATF_ERASE_BLOCK;
            }
            else
            {
                /* Do Nothing */
            }
        }
        else
        {
            /* cast from uint32_t to const void* */
            memcpy(&g_flash_dispatch_2_hndl->b, (const void *)block_addr, FLASH_DF_BLOCK_SIZE);
            data_type = g_flash_dispatch_2_hndl->b.bf.data_type;
            r_flash_dm_valid_block_blank_chk(data_type, block_addr);
            if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
            {
                ret = FLASH_DM_ERR_REQUEST_INIT;
            }
            else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_PASS)
            {
                /* data_type = 1 or 8 */
                /* chain check  */
                if(((data_type == 1) && (g_flash_dispatch_2_hndl->b.bf.chain < FLASH_DM_CFG_DF_BLOCK_NUM))
                        || ((data_type == 8) && (g_flash_dispatch_2_hndl->b.bf.chain == 0xFFFF)))
                {
                    /* data_no check  */
                    if(g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no < FLASH_DM_CFG_DF_DATA_NUM)
                    {
                        /* crc check  */
                        crc_generated = \
                                        (uint16_t)(~r_datf_crc16 \
                                                   ( &((uint8_t)g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no) \
                                                     , DATF_F1_CRC_CALC_SIZE ));
                        if(crc_generated == g_flash_dispatch_2_hndl->b.bf.ft.f1.crc_ccitt)
                        {
                            /* data existence check */
                            if(g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no] \
                                    != 0xFFFF)
                            {
                                /* ser_No check */
                                if(g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->\
                                                                    data_info[g_flash_dispatch_2_hndl-> \
                                                                            b.bf.ft.f1.data_no]].bf.ft.f1.ser_no < \
                                        (g_flash_dispatch_2_hndl->b.bf.ft.f1.ser_no))
                                {
                                    g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->\
                                                                       data_info[g_flash_dispatch_2_hndl-> \
                                                                               b.bf.ft.f1.data_no]].block_type \
                                        = DATF_INVALID_BLOCK;
                                    r_flash_dm_info_set(&gs_max_serial_no_blk);
                                }
                            }
                            else
                            {
                                r_flash_dm_info_set(&gs_max_serial_no_blk);
                            }
                        }
                    }
                }
            }
            else
            {
                /* Do Nothing */
            }
        }
        /* cast from uint8_t to uint8_t */
        g_flash_dispatch_2_hndl->activity = FLASH_DM_ACT_IDLE;
        g_flash_dispatch_2_hndl->block_no++;
    }
    else
    {
        ret = FLASH_DM_ERR_BUSY;
    }
    return ret;
}

/************************************************************************************************
* Function Name: r_flash_dm_init_set2
* Description  : Driver initialization 2
* Arguments    : void
* Return Value : FLASH_DM_ERR_REQUEST_INIT
 *             : FLASH_DM_ERR_REQUEST_FORMAT
 *             : FLASH_DM_SUCCESS
 *             : FLASH_DM_ADVANCE
*------------------------------------------------------------------------------------------------
* Notes        : Initializes data flash driver
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_init_set2(void)
{
    uint32_t    block_addr = 0;
    uint16_t    crc_generated = 0;
    uint8_t     data_type = 0;
    /* cast from uint8_t and uint16_t to uint8_t and uint16_t */
    st_flash_dispatch_2_hndl_blk_info_t blk_info_init = {(uint8_t)0, (uint16_t)0};
    e_flash_dm_status_t ret = FLASH_DM_ADVANCE;
    uint16_t    erase_chain_block = 0;
    uint16_t    invalid_chain_block = 0;

    /* ==== When block_no is smaller than FLASh_DM_CFG_DF_BLOCK_NUM ====*/
    if (g_flash_dispatch_2_hndl->block_no < FLASH_DM_CFG_DF_BLOCK_NUM)
    {
        g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no]   = blk_info_init;
        /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
        block_addr = (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no];
        g_flash_dispatch_2_hndl->command_addr = block_addr;
        /* cast from e_dm_cmd_t to e_dm_cmd_t */
        ret = r_flash_dm_command((e_dm_cmd_t)DATF_BLANK_CHECK_CMD, NULL);
        if(ret == FLASH_DM_ACCEPT)
        {
            ret = FLASH_DM_ADVANCE;
            g_flash_dispatch_2_hndl->command_addr += FLASH_DF_MIN_PGM_SIZE;
            if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
            {
                ret = FLASH_DM_ERR_REQUEST_INIT;
            }
            else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_BLANK)
            {
                r_flash_dm_erase_block_blank_chk(block_addr);
                if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
                {
                    ret = FLASH_DM_ERR_REQUEST_INIT;
                }
                else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_PASS)
                {
                    if(g_flash_dispatch_2_hndl->block_no < (FLASH_DM_CFG_DF_BLOCK_NUM - 1))
                    {
                        /* normal pattern */
                        g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type \
                            = DATF_ERASE_BLOCK;
                    }
                    else if(g_flash_dispatch_2_hndl->block_no >= (FLASH_DM_CFG_DF_BLOCK_NUM - 1))
                    {
                        g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type \
                            = DATF_ERASE_BLOCK;
                        /* considered after format */
                        /* WAIT_LOOP */
                        for(g_flash_dispatch_2_hndl->data_no = 0; \
                                g_flash_dispatch_2_hndl->data_no < FLASH_DM_CFG_DF_DATA_NUM; \
                                g_flash_dispatch_2_hndl->data_no++)
                        {
                            if(g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no] != 0xFFFF)
                            {
                                break;
                            }
                            else if(g_flash_dispatch_2_hndl->data_no == (FLASH_DM_CFG_DF_DATA_NUM - 1))
                            {
                                g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type \
                                    = DATF_INVALID_BLOCK;
                            }
                            else
                            {
                                /* Do Nothing */
                            }
                        }
                    }
                    else
                    {
                        /* Do Nothing */
                    }
                }
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                /* cast from uint32_t to const void* */
                memcpy(&g_flash_dispatch_2_hndl->b, (const void *)block_addr, FLASH_DF_BLOCK_SIZE);
                data_type = g_flash_dispatch_2_hndl->b.bf.data_type;
                r_flash_dm_valid_block_blank_chk(data_type, block_addr);
                if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
                {
                    ret = FLASH_DM_ERR_REQUEST_INIT;
                }
                else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_PASS)
                {
                    /* data_type = 1 or 8 */
                    /* chain check  */
                    if(((data_type == 1) && (g_flash_dispatch_2_hndl->b.bf.chain < FLASH_DM_CFG_DF_BLOCK_NUM))
                            || ((data_type == 8) && (g_flash_dispatch_2_hndl->b.bf.chain == 0xFFFF)))
                    {

                        /* data_no check  */
                        if(g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no < FLASH_DM_CFG_DF_DATA_NUM)
                        {
                            /* crc check  */
                            crc_generated \
                                = (uint16_t)(~r_datf_crc16( &((uint8_t)g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no), \
                                                            DATF_F1_CRC_CALC_SIZE ));
                            if(crc_generated == g_flash_dispatch_2_hndl->b.bf.ft.f1.crc_ccitt)
                            {
                                /* data existence check */
                                if(g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no] \
                                        != 0xFFFF)
                                {
                                    /* ser_No check */
                                    if(g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->\
                                                                        data_info[g_flash_dispatch_2_hndl-> \
                                                                                b.bf.ft.f1.data_no]].bf.ft.f1.ser_no \
                                            < (g_flash_dispatch_2_hndl->b.bf.ft.f1.ser_no))
                                    {
                                        g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl-> \
                                                                           data_info[g_flash_dispatch_2_hndl-> \
                                                                                   b.bf.ft.f1.data_no]].block_type \
                                            = DATF_INVALID_BLOCK;
                                        r_flash_dm_info_set(&gs_max_serial_no_blk);
                                    }
                                }
                                else
                                {
                                    r_flash_dm_info_set(&gs_max_serial_no_blk);
                                }
                            }
                        }
                    }
                }
                else
                {
                    /* Do Nothing */
                }
            }
            g_flash_dispatch_2_hndl->block_no++;
        }
        else
        {
            return FLASH_DM_ERR_REQUEST_INIT;
        }
    }

    /* ==== When block_no is larger than FLASH_DM_CFG_DF_BLOCK_NUM ====*/
    if (g_flash_dispatch_2_hndl->block_no >= FLASH_DM_CFG_DF_BLOCK_NUM)
    {
        /* valid data block chain setting to block information table  */
        /* WAIT_LOOP */
        for(g_flash_dispatch_2_hndl->data_no = 0; \
                g_flash_dispatch_2_hndl->data_no < FLASH_DM_CFG_DF_DATA_NUM; \
                g_flash_dispatch_2_hndl->data_no++)
        {
            if(g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no] != 0xFFFF)
            {
                g_flash_dispatch_2_hndl->block_no \
                    = g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no];

                /* WAIT_LOOP */
                for(;;)
                {
                    g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type \
                        = DATF_VALID_BLOCK;
                    data_type = g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.data_type;
                    if((data_type == 2) || (data_type == 4))
                    {
                        g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain \
                            = g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.chain;
                    }
                    if(
                        (data_type == 1) || \
                        ((data_type == 2) && \
                         (g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain \
                          < FLASH_DM_CFG_DF_BLOCK_NUM))
                    )
                    {
                        g_flash_dispatch_2_hndl->block_no \
                            = g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain;
                    }
                    else if(
                        ((data_type == 4) \
                         && (g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain \
                             == 0xFFFF)) \
                        || (data_type == 8)
                    )
                    {
                        break;
                    }
                    else
                    {
                        g_flash_dispatch_2_hndl->block_no \
                            = g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no];
                        /* WAIT_LOOP */
                        for(;;)
                        {
                            g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type \
                                = DATF_INVALID_BLOCK;
                            data_type \
                                = g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.data_type;
                            if(
                                (data_type == 1) \
                                || ((data_type == 2) \
                                    && (g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain \
                                        < FLASH_DM_CFG_DF_BLOCK_NUM))
                            )
                            {
                                g_flash_dispatch_2_hndl->block_no \
                                    = g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain;
                            }
                            else
                            {
                                /*  not exist. */
                                g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->data_no] = 0xFFFF;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
        /* DATF_STATUS_ERASE block chain and invalid block chain setting to block information table  */
        /* WAIT_LOOP */
        for(g_flash_dispatch_2_hndl->block_no = gs_max_serial_no_blk; \
                g_flash_dispatch_2_hndl->block_no < FLASH_DM_CFG_DF_BLOCK_NUM; \
                g_flash_dispatch_2_hndl->block_no++)
        {
            r_flash_dm_init_making_chain(&erase_chain_block, &invalid_chain_block);
        }

        for(g_flash_dispatch_2_hndl->block_no = 0; \
                g_flash_dispatch_2_hndl->block_no < gs_max_serial_no_blk; \
                g_flash_dispatch_2_hndl->block_no++)
        {
            r_flash_dm_init_making_chain(&erase_chain_block, &invalid_chain_block);
        }

        if((g_flash_dispatch_2_hndl->erase_block_num < 1) || (g_flash_dispatch_2_hndl->invalid_block_num < 1))
        {
            ret = FLASH_DM_ERR_REQUEST_FORMAT;
        }
        else
        {
            g_flash_dispatch_2_hndl->erase_chain_last = erase_chain_block;
            g_flash_dispatch_2_hndl->invalid_chain_last = invalid_chain_block;
            g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->erase_chain_last].chain \
                = g_flash_dispatch_2_hndl->invalid_chain_first;
            /* cast from uint8_t to uint8_t */
            g_flash_dispatch_2_hndl->initialized = FLASH_DM_INITIALIZED_ID;
            ret = FLASH_DM_SUCCESS;
        }
    }
    return ret;
}

/************************************************************************************************
* Function Name: r_flash_dm_init_making_chain
* Description  : Set block control data to the block information table
* Arguments    : uint16_t *erase_chain_block    : Chained block as erased block
 *             : uint16_t *invalid_chain_block  : Chained block as garbage block
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : setting of block information table
*************************************************************************************************/
void r_flash_dm_init_making_chain(uint16_t *erase_chain_block, uint16_t *invalid_chain_block)
{
    if(g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type == DATF_ERASE_BLOCK)
    {
        if((++g_flash_dispatch_2_hndl->erase_block_num) == 1)
        {
            g_flash_dispatch_2_hndl->erase_chain_first = g_flash_dispatch_2_hndl->block_no;
        }
        else
        {
            g_flash_dispatch_2_handle_blk_info[*erase_chain_block].chain = g_flash_dispatch_2_hndl->block_no;
        }
        *erase_chain_block = g_flash_dispatch_2_hndl->block_no;
    }
    else
    {
        if(g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type == DATF_INVALID_BLOCK)
        {
            if((++g_flash_dispatch_2_hndl->invalid_block_num) == 1)
            {
                g_flash_dispatch_2_hndl->invalid_chain_first = g_flash_dispatch_2_hndl->block_no;
            }
            else
            {
                g_flash_dispatch_2_handle_blk_info[*invalid_chain_block].chain = g_flash_dispatch_2_hndl->block_no;
            }
            *invalid_chain_block = g_flash_dispatch_2_hndl->block_no;
        }
    }
}

/************************************************************************************************
* Function Name: r_flash_dm_info_set
* Description  : Set block control data to the block information table
* Arguments    : uint16_t *max_serial_no_blk ; block No. set Max serial No.
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : setting of block information table
*************************************************************************************************/
void r_flash_dm_info_set(uint16_t *max_serial_no_blk)
{
    g_flash_dispatch_2_hndl->data_info[g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no] \
        = g_flash_dispatch_2_hndl->block_no;
    g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].block_type = DATF_VALID_BLOCK;
    g_flash_dispatch_2_handle_blk_info[g_flash_dispatch_2_hndl->block_no].chain = g_flash_dispatch_2_hndl->b.bf.chain;

    /* max serial No. set  */
    if((g_flash_dispatch_2_hndl->max_serial_no) < (g_flash_dispatch_2_hndl->b.bf.ft.f1.ser_no))
    {
        g_flash_dispatch_2_hndl->max_serial_no = (g_flash_dispatch_2_hndl->b.bf.ft.f1.ser_no);
        *max_serial_no_blk = g_flash_dispatch_2_hndl->block_no;
    }
}

/************************************************************************************************
* Function Name: r_flash_dm_erase_block_blank_chk
* Description  : Confirm if a block is blank block
* Arguments    : uint32_t block_addr            ;block address
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : Blank block check
*************************************************************************************************/
void r_flash_dm_erase_block_blank_chk(uint32_t block_addr)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    /* WAIT_LOOP */
    for( ; g_flash_dispatch_2_hndl->command_addr < (block_addr + FLASH_DF_BLOCK_SIZE); \
            g_flash_dispatch_2_hndl->command_addr += FLASH_DF_MIN_PGM_SIZE)
    {
        /* cast from e_dm_cmd_t to e_dm_cmd_t */
        ret = r_flash_dm_command((e_dm_cmd_t)DATF_BLANK_CHECK_CMD, NULL);
        if(ret == FLASH_DM_ACCEPT)
        {
            if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
            {
                return;
            }
            else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_NONBLANK)
            {
                /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                if(g_flash_dispatch_2_hndl->command_addr \
                        != (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.erase_end)
                {
                    g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_FAIL;
                    return;
                }
            }
            else
            {
                /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                if(g_flash_dispatch_2_hndl->command_addr \
                        == (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.erase_end)
                {
                    g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_FAIL;
                    return;
                }
            }
        }
        else
        {
            g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_CMD_ERROR;
            return;
        }
    }
    g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_PASS;
    return;
}

/************************************************************************************************
* Function Name: r_flash_dm_valid_block_blank_chk
* Description  : Confirm if a block is non blank block
* Arguments    : uint8_t data_type                ; data type
 *             : uint32_t block_addr            ; block address
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : Non blank block check
*************************************************************************************************/
void r_flash_dm_valid_block_blank_chk(uint8_t data_type, uint32_t block_addr)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    uint16_t command_counter = 0;

    if((data_type == 1) || (data_type == 8))
    {
        if(data_type == 1)
        {
            command_counter = DATF_F1_DATASIZE + DATF_F1_INFO_SIZE;
        }
        else
        {
            command_counter = gc_dm_data_size[g_flash_dispatch_2_hndl->b.bf.ft.f1.data_no] + DATF_F1_INFO_SIZE;
            command_counter = ((command_counter + BLANK_ROUND_NUM) / FLASH_DF_MIN_PGM_SIZE) * FLASH_DF_MIN_PGM_SIZE;
        }

        /* WAIT_LOOP */
        for( ; g_flash_dispatch_2_hndl->command_addr < (block_addr + command_counter); \
                g_flash_dispatch_2_hndl->command_addr += FLASH_DF_MIN_PGM_SIZE)
        {
            /* cast from e_dm_cmd_t to e_dm_cmd_t */
            ret = r_flash_dm_command((e_dm_cmd_t)DATF_BLANK_CHECK_CMD, NULL);
            if(ret == FLASH_DM_ACCEPT)
            {
                if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
                {
                    return;
                }
                else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_BLANK)
                {
                    g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_FAIL;
                    return;
                }
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_CMD_ERROR;
                return;
            }
        }
        /* WAIT_LOOP */
        for(g_flash_dispatch_2_hndl->command_addr = (block_addr + command_counter);
                g_flash_dispatch_2_hndl->command_addr < ((block_addr + DATF_F1_DATASIZE) + DATF_F1_INFO_SIZE);
                g_flash_dispatch_2_hndl->command_addr += FLASH_DF_MIN_PGM_SIZE)
        {
            /* cast from e_dm_cmd_t to e_dm_cmd_t */
            ret = r_flash_dm_command((e_dm_cmd_t)DATF_BLANK_CHECK_CMD, NULL);
            if(ret == FLASH_DM_ACCEPT)
            {
                if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
                {
                    return;
                }
                else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_NONBLANK)
                {
                    g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_FAIL;
                    return;
                }
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_CMD_ERROR;
                return;
            }
        }
        /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
        /* WAIT_LOOP */
        for(g_flash_dispatch_2_hndl->command_addr \
                = (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl->block_no].bf.ft.f1.write_end;
                g_flash_dispatch_2_hndl->command_addr < (block_addr + FLASH_DF_BLOCK_SIZE);
                g_flash_dispatch_2_hndl->command_addr += FLASH_DF_MIN_PGM_SIZE)
        {

            /* cast from e_dm_cmd_t to e_dm_cmd_t */
            ret = r_flash_dm_command((e_dm_cmd_t)DATF_BLANK_CHECK_CMD, NULL);
            if(ret == FLASH_DM_ACCEPT)
            {
                if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_CMD_ERROR)
                {
                    return;
                }
                else if(g_flash_dispatch_2_hndl->blankcheck_result == DATF_BLANKCHECK_BLANK)
                {
                    /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                    if(g_flash_dispatch_2_hndl->command_addr \
                            != (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl-> \
                                    block_no].bf.erase_start)
                    {
                        g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_FAIL;
                        return;
                    }
                }
                else
                {
                    /* cast from u_flash_dispatch_2_hndl_block_t* to uint32_t */
                    if(g_flash_dispatch_2_hndl->command_addr \
                            == (uint32_t)&g_flash_dispatch_2_handle_blocks[g_flash_dispatch_2_hndl-> \
                                    block_no].bf.erase_start)
                    {
                        g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_FAIL;
                        return;
                    }
                }
            }
            else
            {
                g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_CMD_ERROR;
                return;
            }
        }
        g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_PASS;
        return;
    }
    else
    {
        g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_FAIL;
        return;
    }
}

/************************************************************************************************
* Function Name: r_flash_dm_advance
* Description  : Continues process or acquires the status or driver.
* Arguments    : None
* Return values: FLASH_DM_ACCEPT        ; Successful operation reception
*              : FLASH_DM_FINISH_FORMAT ; Successful operation and finish format
*              : FLASH_DM_FINISH_WRITE  ; Successful operation and finish write
*              : FLASH_DM_FINISH_ERASE  ; Successful operation and finish erase
*              : FLASH_DM_ERR_FORMAT    ; Failed operation of format
*              : FLASH_DM_ERR_WRITE     ; Failed operation of write
*              : FLASH_DM_ERR_ERASE     ; Failed operation of erase
*              : FLASH_DM_ERR_REQUEST_INIT ; Failed operation and request initialization
*              : FLASH_DM_ERR_BUSY      ; Busy
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash and BGO operation, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_advance(void)
{
    e_flash_dm_status_t ret = FLASH_DM_SUCCESS;

    ret = r_flash_dm_1st_check(FLASH_DM_ADVANCE_1ST_CHECK);
    if(ret == FLASH_DM_SUCCESS)
    {
        ret = r_flash_dm_dispatch((e_dm_adv_t)DATF_RESTART);
    }
    else
    {
        ret = FLASH_DM_ERR_REQUEST_INIT;
    }

    return ret;
}/* End of function r_flash_dm_advance() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_open
* Description  : Opens the driver interface.
* Arguments    : None
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, must place the source code to the ROM.
*              : (Prohibit the placement to RAM)
*************************************************************************************************/
uint8_t r_flash_dm_drvif_open(void)
{
    if (R_FLASH_Open() != FLASH_SUCCESS)
    {
        return 0;
    }

#if(FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)

    flash_interrupt_config_t cb_func_info;
    /* Set callback function and interrupt priority
       if use BGO of the Code Flash or the Data Flash. */
    cb_func_info.pcallback    = flash_cb_function;
    cb_func_info.int_priority = FLASH_DM_CFG_FRDYI_INT_PRIORITY;
    /* cast from flash_interrupt_config_t to void* */
    if (R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)&cb_func_info) != FLASH_SUCCESS)
    {
        return 0;
    }
#endif /* (FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4) */

    return 1;
} /* End of function r_flash_dm_drvif_open() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_api_flag
* Description  : Checks API flag.
* Arguments    : st_flash_dispatch_1_hndl_t * p_hndl   ; FLASH DM information
* Return Value : 1: Unlock
*              : 0: Lock
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
uint8_t r_flash_dm_drvif_api_flag()
{
    /* cast from uint32_t to uint32_t */
    int32_t is_locked = FLASH_DM_API_CALL;

    /* ==== Check API calling ==== */
    xchg(&is_locked, &g_flash_dispatch_2_hndl->api_call);
    /* cast from uint32_t to uint32_t */
    if (FLASH_DM_API_CALL == is_locked)
    {
        return 1;   /* Unlock */
    }
    return 0;       /* Lock */
}

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
    flash_err_t ret = FLASH_SUCCESS;

    ret = R_FLASH_Control(FLASH_CMD_STATUS_GET, NULL);
    if (FLASH_SUCCESS == ret)
    {
        /* Ready */
        return 0;
    }
    else
    {
        /* Busy */
        /* Do Nothing */
    }
    /* Busy */
    return 1;
} /* End of function r_flash_dm_drvif_flash_busy() */

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

    act = g_flash_dispatch_2_hndl->activity;
    ini = g_flash_dispatch_2_hndl->initialized;

    /* WAIT_LOOP */
    do
    {
        /* ==== In the case of calling initialization or format ==== */
        if (FLASH_DM_CHECK_INITIALIZED != (criteria & FLASH_DM_CHECK_INITIALIZED))
        {
            /* Check calling format API. */
            if (FLASH_DM_FORMATTING_ID == ini)
            {
                /* cast from uint8_t to uint8_t */
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
                /* cast from uint8_t to uint8_t */
                if (((FLASH_DM_ACT_WRITING == act) \
                        || (FLASH_DM_ACT_RECLAIMING == act)) \
                        || (FLASH_DM_ACT_ERASING == act))
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
                /* cast from uint8_t to uint8_t */
                if (FLASH_DM_ACT_IDLE == act)
                {
                    break;  /* Permit the API calling. */
                }
                /* cast from uint8_t to uint8_t */
                if (((FLASH_DM_ACT_WRITING == act) \
                        || (FLASH_DM_ACT_RECLAIMING == act)) \
                        || (FLASH_DM_ACT_ERASING == act))
                {
                    break;  /* Permit the API calling. */
                }
            }

            /* Check calling format API. */
            if (FLASH_DM_FORMATTING_ID == ini)
            {
                /* cast from uint8_t to uint8_t */
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
                /* cast from uint8_t to uint8_t */
                if ((((FLASH_DM_ACT_IDLE == act) || (FLASH_DM_ACT_WRITING == act)) \
                        || (FLASH_DM_ACT_RECLAIMING == act)) || (FLASH_DM_ACT_ERASING == act))
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
                /* cast from uint8_t to uint8_t */
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
                /* cast from uint8_t to uint8_t */
                if (FLASH_DM_ACT_IDLE == act)
                {
                    /* Permit the API calling. */
                    break;
                }
                /* cast from uint8_t to uint8_t */
                if (((FLASH_DM_ACT_WRITING == act) \
                        || (FLASH_DM_ACT_RECLAIMING == act)) \
                        || (FLASH_DM_ACT_ERASING == act))
                {
                    /* The driver is in active state. */
                    ret = FLASH_DM_ERR_BUSY;
                    break;
                }
                /* cast from uint8_t to uint8_t */
                if (FLASH_DM_ACT_FORMATTING == act)
                {
                    /* The driver is in active state. */
                    ret = FLASH_DM_ERR_BUSY;
                    break;
                }
            }

            /* Check calling format API. */
            if (FLASH_DM_FORMATTING_ID == ini)
            {
                /* The driver is in active state. */
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
* Function Name: r_flash_dm_set_dispatch
* Description  : Copy the handle's address.
* Arguments    : uint32_t   handle   ; Current handle
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : Note.
*************************************************************************************************/
void r_flash_dm_set_dispatch(uint32_t handle)
{
    /* cast from uint32_t to st_flash_dispatch_2_hndl_t* */
    g_flash_dispatch_2_hndl = (st_flash_dispatch_2_hndl_t*)handle;
}

#endif /* (FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4) */

/* End of File */
