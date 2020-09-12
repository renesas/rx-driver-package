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
 * Copyright (C) 2013(2019) Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
* File Name    : r_usb_hscheduler.c
* Description  : This is the USB basic firmware library API function code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"

#include "r_usb_dmac.h"


#if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST
/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
USB_STATIC void usb_hstd_update_wait_queue(void);
USB_STATIC void usb_hstd_wait_scheduler(void);


/******************************************************************************
Function Name   : usb_hstd_update_wait_queue
Description     : Wait queue update
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_update_wait_queue(void)
{
    uint8_t     i;
    uint8_t     j;

    /* WAIT_LOOP */
    for (i = 0, j = 1; j < USB_TABLEMAX; i++, j++)
    {
        if (USB_IDCLR == g_usb_hstd_wait_que_sch[i].task_id)
        {
            if (USB_IDCLR != g_usb_hstd_wait_que_sch[j].task_id)
            {
                g_usb_hstd_wait_que_sch[i].p_address   = g_usb_hstd_wait_que_sch[j].p_address;
                g_usb_hstd_wait_que_sch[i].task_id    = g_usb_hstd_wait_que_sch[j].task_id;
                g_usb_hstd_wait_que_sch[j].task_id    = USB_IDCLR;
            }
            else
            {
                i--;
            }
        }
    }
}   /* End of function usb_hstd_update_wait_queue() */


/******************************************************************************
Function Name   : usb_hstd_wait_scheduler
Description     : Wait request sceduling
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_wait_scheduler(void)
{
    uint8_t     i;
    uint8_t     id;

    /* WAIT_LOOP */
    for (id = 0; id < USB_IDMAX; id++ )
    {
        if (0 != g_usb_hstd_wait_msg_sch[id].counter)
        {
            g_usb_hstd_wait_msg_sch[id].counter--;
            if (0 == g_usb_hstd_wait_msg_sch[id].counter)
            {
                if (USB_OK != usb_hstd_snd_msg(id, g_usb_hstd_wait_msg_sch[id].p_address))
                {
                    /* Message is carrying over next time because it was not delayed. */
                    g_usb_hstd_wait_msg_sch[id].counter = 1;
                }
                else
                {
                    /* Because the message was able to be preserved in the priority table, */
                    /* the wait queue table is updated. */
                    /* WAIT_LOOP */
                    for (i = 0; i < USB_TABLEMAX; i++)
                    {
                        /* The same ID that's in the wait queue table moves to the priority table. */
                        if (g_usb_hstd_wait_que_sch[i].task_id == id)
                        {
                            if (USB_OK != usb_hstd_snd_msg(id, g_usb_hstd_wait_que_sch[i].p_address))
                            {
                                usb_hstd_wai_msg( id, g_usb_hstd_wait_que_sch[i].p_address, USB_1 );
                            }
                            g_usb_hstd_wait_que_sch[i].task_id    = USB_IDCLR;
                        }
                    }
                    usb_hstd_update_wait_queue();
                }
            }
        }
    }
}   /* End of function usb_hstd_wait_scheduler() */


/******************************************************************************
Function Name   : usb_hstd_scheduler
Description     : Task scheduler
Argument        : none
Return          : uint8_t USB_FLGSET    Task that can be operated. 
                : uint8_t USB_FLGCLR    No task that can be operated. 
******************************************************************************/
uint8_t usb_hstd_scheduler(void)
{
    uint8_t tsk_pri;        /* Priority Counter */
    uint8_t rd_ptr;         /* Priority Table Reading pointer */

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    usb_cstd_dma_driver();
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

    /* wait msg */
    usb_hstd_wait_scheduler();

    /* Priority table search */
    /* WAIT_LOOP */
    for ( tsk_pri = 0; tsk_pri < USB_IDMAX; tsk_pri++)
    {
        rd_ptr  = g_usb_hstd_read_sch[tsk_pri];
        if (rd_ptr != g_usb_hstd_write_sch[tsk_pri])
        {
            /* Next message read */
            rd_ptr++;
            if (rd_ptr >= USB_TABLEMAX)
            {
                rd_ptr = 0;
            }
            /* Set practice message */
            gp_usb_hstd_next_addr_sch       = g_usb_hstd_msg_tbl_sch[tsk_pri][rd_ptr];
            g_usb_hstd_next_id_sch         = tsk_pri;
            g_usb_next_read_ptr_sch        = rd_ptr;
            return  USB_FLGSET;
        }
    }
    /* Task is not executing */
    return  USB_FLGCLR;
}/* End of function usb_hstd_scheduler() */


/******************************************************************************
Function Name   : usb_hstd_rec_msg
Description     : Receive Message
Argument        : uint8_t   id              : ID Number
                : usb_msg_t **pp_mess       : Message Pointer
Return          : uint16_t                  : USB_OK / USB_ERROR
******************************************************************************/
usb_er_t usb_hstd_rec_msg( uint8_t id, usb_msg_t** pp_mess )
{
    if (id == g_usb_hstd_next_id_sch)
    {
        /* Task is executing */
        *pp_mess                           = gp_usb_hstd_next_addr_sch;
        g_usb_hstd_read_sch[g_usb_hstd_next_id_sch]  = g_usb_next_read_ptr_sch;
        return  USB_OK;
    }
    /* Task is not executing */
    return  USB_ERROR;
}   /* End of function usb_hstd_rec_msg() */


/******************************************************************************
Function Name   : usb_hstd_snd_msg
Description     : Send Message
Argument        : uint8_t   id              : ID Number
                : usb_msg_t *mess           : Message Pointer
Return          : usb_er_t                  : USB_OK / USB_ERROR
******************************************************************************/
usb_er_t usb_hstd_snd_msg( uint8_t id, usb_msg_t* p_mess )
{
    usb_er_t    status;

    if (0 == g_usb_hstd_wait_msg_sch[id].counter)
    {
        /* The message is preserved in the priority table while not waiting. */
        usb_cpu_int_disable();                      /* USB interrupt disable */
        status  = usb_hstd_isnd_msg(id,p_mess);      /* Message send */
        usb_cpu_int_enable();                       /* USB interrupt enable */
    }
    else
    {
        /* The message is preserved in the wait queue table because it is waiting. */
        status = usb_hstd_wai_msg( id, p_mess, USB_1 );
    }
    return status;
}   /* End of function usb_hstd_snd_msg() */


/******************************************************************************
Function Name   : usb_hstd_isnd_msg
Description     : Send Message(for interrupt)
Argument        : uint8_t   id              : ID Number
                : usb_msg_t *mess           : Message Pointer
Return          : usb_er_t                  : USB_OK / USB_ERROR
******************************************************************************/
usb_er_t usb_hstd_isnd_msg( uint8_t id, usb_msg_t* p_mess )
{
    uint8_t wr_ptr;                                     /* Priority table write pointer */

    if (id < USB_IDMAX)
    {
        /* Read priority and table pointer */
        wr_ptr  = g_usb_hstd_write_sch[id];
        wr_ptr++;                                       /* Renewal write pointer */
        if (wr_ptr >= USB_TABLEMAX)
        {
            wr_ptr = 0;
        }
        if (wr_ptr != g_usb_hstd_read_sch[id])                /* Check pointer */
        {
            /* Save message & Set priorty table */
            g_usb_hstd_msg_tbl_sch[id][wr_ptr]     = p_mess;
            g_usb_hstd_write_sch[id]               = wr_ptr;
            return  USB_OK;
        }
        /* Error of Sucedule Table Full !!  */
    }
    /* Error of argument !!  */
    return  USB_ERROR;
}   /* End of function usb_hstd_isnd_msg() */


/******************************************************************************
Function Name   : usb_hstd_wai_msg
Description     : Wait Message
Argument        : uint8_t   id              : ID Number
                : usb_msg_t *mess           : Message pointer
                : uint16_t  count           : Timeout Counter
Return          : usb_er_t                  : USB_OK / USB_ERROR
******************************************************************************/
usb_er_t usb_hstd_wai_msg( uint8_t id, usb_msg_t* p_mess, uint16_t count )
{
    uint8_t     i;

    if (id < USB_IDMAX)
    {
        if (0 != count)
        {
            /* Set delay timer */
            if (0 == g_usb_hstd_wait_msg_sch[id].counter)
            {
                /* The counter is set and the message is preserved temporarily while not waiting. */
                g_usb_hstd_wait_msg_sch[id].p_address  = p_mess;
                g_usb_hstd_wait_msg_sch[id].counter    = count;
                return  USB_OK;
            }
            else
            {
                /* The message is preserved in the wait queue table because it is waiting(Time is forgotten). */
                /* WAIT_LOOP */
                for (i = 0; i < USB_TABLEMAX; i++)
                {
                    if (USB_IDCLR == g_usb_hstd_wait_que_sch[i].task_id)
                    {
                        g_usb_hstd_wait_que_sch[i].task_id    = id;
                        g_usb_hstd_wait_que_sch[i].p_address  = p_mess;
                        return  USB_OK;
                    }
                }
            }
        }
        else
        {
            /* Clear delay timer */
            g_usb_hstd_wait_msg_sch[id].counter    = 0;
            /* WAIT_LOOP */
            for (i = 0; i < USB_TABLEMAX; i++)
            {
                if (g_usb_hstd_wait_que_sch[i].task_id == id)
                {
                    g_usb_hstd_wait_que_sch[i].task_id    = USB_IDCLR;
                }
            }
            usb_hstd_update_wait_queue();
            return  USB_OK;
        }
    }
    /* Error of argument !!  */
    return  USB_ERROR;
}   /* End of function usb_hstd_wai_msg() */


/******************************************************************************
Function Name   : usb_hstd_pget_send
Description     : Get memory block
Argument        : uint8_t  id               : ID Number
                : usb_strct_t  msginfo      : Message Info
                : usb_cbinfo_t complete     : Callback function
                : usb_strct_t  keyword      : Message Keyword
Return          : usb_er_t                  : USB_OK / USB_ERROR
******************************************************************************/
usb_er_t usb_hstd_pget_send(uint8_t id, usb_strct_t msginfo, usb_cbinfo_t complete, usb_strct_t keyword)
{
    usb_tskinfo_t   *p_utr;
    uint8_t         blk_num;

    if (id < USB_IDMAX)
    {
        /* WAIT_LOOP */
        for ( blk_num = 0; blk_num < USB_BLKMAX; blk_num++)
        {
            p_utr     = &g_usb_hstd_block[blk_num];
            if (USB_IDCLR == p_utr->flag)
            {
                /* Acquire fixed-size memory block & send message */
                p_utr->keyword    = keyword;
                p_utr->msginfo    = msginfo;
                p_utr->complete   = complete;
                if (USB_SND_MSG(id, (usb_msg_t*)p_utr) == USB_OK)
                {
                    p_utr->flag   = blk_num;
                    return  USB_OK;
                }
                /* Error of Send_Message !!  */
                return  USB_ERROR;
            }
        }
        /* Error of block table full !!  */
    }
    /* Error of argument !!  */
    return  USB_ERROR;
}   /* End of function usb_hstd_pget_send() */

/******************************************************************************
Function Name   : usb_hstd_rel_blk
Description     : Release memory block
Argument        : uint8_t blk_num           : Used block index
Return          : usb_er_t                  : USB_OK / USB_ERROR
******************************************************************************/
usb_er_t usb_hstd_rel_blk( uint8_t blk_num )
{
    if ((blk_num < USB_BLKMAX) && (g_usb_hstd_block[blk_num].flag == blk_num))
    {
        g_usb_hstd_block[blk_num].flag    = USB_IDCLR;
        return  USB_OK;
    }
    /* Error of argument !!  */
    return  USB_ERROR;
}   /* End of function usb_hstd_rel_blk() */

#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
/******************************************************************************
End of file
******************************************************************************/
