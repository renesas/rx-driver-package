/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : wrap_sci.c
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "wrap_sci.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
extern volatile uint32_t g_spp_tx_data_empty;
st_sci_tbl_t g_sci_tbl;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/* SCI configurations */
static const st_sci_conf_t s_sci_cfg[] =
{
#if SCI_CFG_CH0_INCLUDED == 1
    {SCI_CH0 , R_SCI_PinSet_SCI0  ,SCI_CFG_CH0_TX_BUFSIZ  ,SCI_CFG_CH0_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH1_INCLUDED == 1
    {SCI_CH1 , R_SCI_PinSet_SCI1  ,SCI_CFG_CH1_TX_BUFSIZ  ,SCI_CFG_CH1_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH2_INCLUDED == 1
    {SCI_CH2 , R_SCI_PinSet_SCI2  ,SCI_CFG_CH2_TX_BUFSIZ  ,SCI_CFG_CH2_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH3_INCLUDED == 1
    {SCI_CH3 , R_SCI_PinSet_SCI3  ,SCI_CFG_CH3_TX_BUFSIZ  ,SCI_CFG_CH3_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH4_INCLUDED == 1
    {SCI_CH4 , R_SCI_PinSet_SCI4  ,SCI_CFG_CH4_TX_BUFSIZ  ,SCI_CFG_CH4_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH5_INCLUDED == 1
    {SCI_CH5 , R_SCI_PinSet_SCI5  ,SCI_CFG_CH5_TX_BUFSIZ  ,SCI_CFG_CH5_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH6_INCLUDED == 1
    {SCI_CH6 , R_SCI_PinSet_SCI6  ,SCI_CFG_CH6_TX_BUFSIZ  ,SCI_CFG_CH6_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH7_INCLUDED == 1
    {SCI_CH7 , R_SCI_PinSet_SCI7  ,SCI_CFG_CH7_TX_BUFSIZ  ,SCI_CFG_CH7_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH8_INCLUDED == 1
    {SCI_CH8 , R_SCI_PinSet_SCI8  ,SCI_CFG_CH8_TX_BUFSIZ  ,SCI_CFG_CH8_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH9_INCLUDED == 1
    {SCI_CH9 , R_SCI_PinSet_SCI9  ,SCI_CFG_CH9_TX_BUFSIZ  ,SCI_CFG_CH9_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH10_INCLUDED == 1
    {SCI_CH10, R_SCI_PinSet_SCI10 ,SCI_CFG_CH10_TX_BUFSIZ ,SCI_CFG_CH10_RX_BUFSIZ },
#endif
#if SCI_CFG_CH11_INCLUDED == 1
    {SCI_CH11, R_SCI_PinSet_SCI11 ,SCI_CFG_CH11_TX_BUFSIZ ,SCI_CFG_CH11_RX_BUFSIZ },
#endif
#if SCI_CFG_CH12_INCLUDED == 1
    {SCI_CH12, R_SCI_PinSet_SCI12 ,SCI_CFG_CH12_TX_BUFSIZ ,SCI_CFG_CH12_RX_BUFSIZ },
#endif
    {SCI_NUM_CH, NULL ,0 ,0 }
};


/**********************************************************************************************************************
 * Function Name: get_port_config
 * Description  : get UART port configuration.
 * Arguments    : ch
 * Return Value : pointer of st_sci_conf_t
 *********************************************************************************************************************/
static st_sci_conf_t * get_port_config(sci_ch_t ch)
{
    uint16_t i;
    st_sci_conf_t * p_tbl = NULL;

    /* Set table pointer */
    for (i = 0;; i++ )
    {
        if (SCI_NUM_CH == s_sci_cfg[i].ch)
        {
            break;
        }
        if (ch == s_sci_cfg[i].ch)
        {
            p_tbl = (st_sci_conf_t *)&s_sci_cfg[i];
            break;
        }
    }
    return p_tbl;
}
/**********************************************************************************************************************
 * End of get_port_config
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: wrap_sci_open
 * Description  : Open SCI channel.
 * Arguments    : ch
 *                p_cb
 * Return Value :
 *********************************************************************************************************************/
uint8_t wrap_sci_open(sci_ch_t ch, void (* const p_cb)(void *p_args))
{
    st_sci_tbl_t * p_sci = &g_sci_tbl;
    st_sci_conf_t * p_cfg = get_port_config(ch);

    if (NULL == p_cfg)
    {
        return 1;
    }

    /* Port settings */
    p_cfg->func();

    memset(&p_sci->sci_hdl , 0, sizeof(sci_hdl_t));
    p_sci->sci_config.async.baud_rate    = 115200;
    p_sci->sci_config.async.clk_src      = SCI_CLK_INT;
    p_sci->sci_config.async.data_size    = SCI_DATA_8BIT;
    p_sci->sci_config.async.parity_en    = SCI_PARITY_OFF;
    p_sci->sci_config.async.parity_type  = SCI_EVEN_PARITY;
    p_sci->sci_config.async.stop_bits    = SCI_STOPBITS_1;
    p_sci->sci_config.async.int_priority = 4;
    if (SCI_SUCCESS != R_SCI_Open(p_cfg->ch, SCI_MODE_ASYNC, &p_sci->sci_config, p_cb, &p_sci->sci_hdl))
    {
        return 1;
    }
    return SCI_SUCCESS;
}
/**********************************************************************************************************************
 * End of wrap_sci_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: wrap_sci_send
 * Description  : Send data on SCI channel.
 * Arguments    : p_src
 *                length
 * Return Value : same as R_SCI_Send()
 *********************************************************************************************************************/
sci_err_t wrap_sci_send(uint8_t *p_src, uint16_t const length)
{
    sci_err_t err;

    g_sci_tbl.tx_end_flag = 0;
    g_spp_tx_data_empty = 0;
    err = R_SCI_Send(g_sci_tbl.sci_hdl, p_src, length);
    return err;
}
/**********************************************************************************************************************
 * End of wrap_sci_send
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: wrap_sci_recv
 * Description  : receive data from SCI channel.
 * Arguments    : p_dst
 *                length
 * Return Value : same as R_SCI_Receive()
 *********************************************************************************************************************/
sci_err_t wrap_sci_recv(uint8_t * p_dst, uint16_t const length)
{
    return R_SCI_Receive(g_sci_tbl.sci_hdl, p_dst, length);
}
/**********************************************************************************************************************
 * End of wrap_sci_recv
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: wrap_sci_close
 * Description  : close SCI channel.
 * Arguments    : none
 * Return Value : same as R_SCI_Close()
 *********************************************************************************************************************/
sci_err_t wrap_sci_close(void)
{
    return R_SCI_Close(g_sci_tbl.sci_hdl);
}
/**********************************************************************************************************************
 * End of wrap_sci_close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: wrap_sci_ctrl
 * Description  : configure SCI channel.
 * Arguments    : cmd
 *                p_args
 * Return Value : same as R_SCI_Control()
 *********************************************************************************************************************/
sci_err_t wrap_sci_ctrl(sci_cmd_t const cmd, void * p_args)
{
    return R_SCI_Control(g_sci_tbl.sci_hdl, cmd, p_args);
}
/**********************************************************************************************************************
 * End of wrap_sci_ctrl
 *********************************************************************************************************************/
