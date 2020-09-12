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
 * Copyright (C) 2014(2020) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_pmsc_driver.c
 * Description  : USB Host and Peripheral Interrupt code
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 26.12.2014 1.10 RX71M is added
 *         : 30.09.2015 1.11 RX63N/RX631 is added.
 *         : 30.09.2016 1.20 RX65N/RX651 is added.
 *         : 30.09.2017 1.22 Change function name "usb_pmsc_atapi_command_execute()"->"pmsc_atapi_command_processing()"
 *                           "usb_pmsc_atapi_analyze_cbwcb()"->"pmsc_atapi_analyze_cbwcb()"
 *                           "usb_pmsc_usb_execute()"->"usb_pmsc_data_transfer()"
 *                           Change Error macro.
 *                           Fix "usb_pmsc_check_cbw()","usb_pmsc_data_transfer()","usb_pmsc_err_csw_ng()",
 *                           "usb_pmsc_err_phase_err()".
 *                           Add "usb_pmsc_err_csw_ok()".
 *         : 31.03.2018 1.23 Supporting Smart Configurator
 *         : 31.05.2019 1.26 Added support for GNUC and ICCRX.
 *         : 01.03.2020 1.30 RX72N/RX66N is added and uITRON is supported.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include <string.h>

#include "r_usb_basic_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"

#include "r_usb_patapi.h"
#include "r_usb_pmsc.h"
#include "r_usb_pmsc_if.h"

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
#include "r_rtos_abstract.h"
#include "r_usb_cstd_rtos.h"
#endif /* (BSP_CFG_RTOS_USED != 0) */

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/
static void     usb_pmsc_transfer_start (uint16_t pipe, uint32_t size, uint8_t *table);
static void     usb_pmsc_check_cbw (void);
static uint8_t  usb_pmsc_check_case13 (uint32_t ul_size, uint8_t *uc_case);
static uint8_t  usb_pmsc_transfer_matrix (uint8_t uc_pmsc_case);
static void     usb_pmsc_setcsw (uint8_t ar_resp);
static void     usb_pmsc_data_transfer (usb_utr_t *mess, uint16_t data1, uint16_t data2);
static void     usb_pmsc_trans_result (usb_utr_t *mess, uint16_t data1, uint16_t data2);
static void     usb_pmsc_err_csw_ok (usb_utr_t *ptr, uint16_t data1, uint16_t data2);
static void     usb_pmsc_err_csw_ng (usb_utr_t *ptr, uint16_t data1, uint16_t data2);
static void     usb_pmsc_err_phase_err (usb_utr_t *ptr, uint16_t data1, uint16_t data2);
static void     usb_pmsc_csw_transfer(uint8_t csw_status);

/* Mass Storage Device Class CBW */
static usb_msc_cbw_t    g_usb_pmsc_cbw;

/* Mass Storage Device Class CSW */
static usb_msc_csw_t    g_usb_pmsc_csw;

/* PMSC UTR */
static usb_utr_t        g_usb_pmsc_utr;

/* Mass Storage Device Class sequence */
static uint8_t          g_usb_pmsc_seq;

#if (BSP_CFG_RTOS_USED == 0)        /* Non-OS */
/* Transfer Complete Flag */
static uint8_t          g_usb_pmsctrans_complete;
#endif  /* (BSP_CFG_RTOS_USED == 0) */

/* Transfer Status */
static uint16_t         g_usb_pmscstatus;

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
/* Mass Storage Device Class Transfer Length(CBW) */
uint32_t        g_usb_pmsc_dtl;

/* Mass Storage Device Class */
usb_pmsc_cbm_t  g_usb_pmsc_message;


/* Pipe Information Table (or "endpoint table"). */

/***********************************************************************************************************************
 Renesas Abstracted Peripheral Mass Storage Class Driver API functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Function Name   : usb_pmsc_task
 Description     : Peripheral Mass Storage Class main task
 Arguments       : none
 Return value    : none
 ***********************************************************************************************************************/
#if (BSP_CFG_RTOS_USED == 4)        /* Renesas RI600V4 & RI600PX */
void usb_pmsc_task (VP_INT b)
#else  /* (BSP_CFG_RTOS_USED == 4) */
void usb_pmsc_task(void)
#endif /* (BSP_CFG_RTOS_USED == 4) */
{
#if (BSP_CFG_RTOS_USED == 0)        /* Non-OS */
    static uint8_t  is_init = USB_NO;

    if (USB_NO == is_init)
    {
        usb_pmsc_init ();
        pmsc_atapi_init ();

        is_init = USB_YES;
    }

    if (USB_FALSE == g_usb_pmsctrans_complete)
    {
        return;
    }
    switch (g_usb_pmsc_seq)
    {
        /* Normal  pmsc_seq Conditions */
        case USB_PMSC_CBWRCV:

            switch (g_usb_pmscstatus)
            {
                /* Normal Status Condotion */
                case USB_DATA_OK:

                    usb_pmsc_check_cbw();

                break;

                /* Abnormal Status Conditions */
                case USB_DATA_SHT:
                case USB_DATA_OVR:

                    USB_PRINTF0("### CBW Size Error \n");

                break;

                case USB_DATA_STOP:

                    USB_PRINTF0("### USB_PMSC_CBWRCV detach error\n");

                break;

                default:

                    USB_PRINTF0("### USB_PMSC_CBWRCV error\n");

                break;
            }

        break;

        case USB_PMSC_DATARCV:

            switch (g_usb_pmscstatus)
            {
                /* Normal Status Condotion */
                case USB_DATA_OK:
                case USB_DATA_SHT:
                case USB_DATA_OVR:

                    pmsc_atapi_command_processing(g_usb_pmsc_cbw.cbwcb, g_usb_pmscstatus, &usb_pmsc_data_transfer);

                break;

                /* Abnormal Stauts Conditions */
                case USB_DATA_STOP:

                    USB_PRINTF0("### USB_PMSC_DATARCV detach error\n");

                break;

                default:

                    USB_PRINTF0("### USB_PMSC_DATARCV error\n");

                break;
            }

        break;

        case USB_PMSC_DATASND:

            switch (g_usb_pmscstatus)
            {
                /* Normal Status Condotion */
                case USB_DATA_NONE:

                    pmsc_atapi_command_processing(g_usb_pmsc_cbw.cbwcb, g_usb_pmscstatus, &usb_pmsc_data_transfer);

                break;

                /* Abnormal Status Conditions */
                case USB_DATA_STOP:

                    USB_PRINTF0("### USB_PMSC_DATASND detach error\n");

                break;

                default :

                    USB_PRINTF0("### USB_PMSC_DATASND error\n");

                break;
            }

        break;

        case USB_PMSC_CSWSND :

            switch (g_usb_pmscstatus)
            {
                /* Normal  Status Condotion */
                case USB_DATA_NONE:

                    usb_pmsc_receive_cbw();

                break;

                /* Abnormal Status Conditions */
                case USB_DATA_STOP:

                    USB_PRINTF0("### USB_PMSC_CSWSND detach error\n");

                break;

                default:

                    USB_PRINTF0("### USB_PMSC_CSWSND error\n");

                break;
            }

        break;

        /* Abnormal pmsc_seq Conditions */
        default:

            USB_PRINTF1("### pmsc Sequence error:%x\n", g_usb_pmsc_seq);

        break;
    }

    g_usb_pmsctrans_complete = USB_FALSE;
#endif /*(BSP_CFG_RTOS_USED == 0)*/

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
    static uint8_t  is_init = USB_NO;
    rtos_err_t  err;
    usb_int_t   *p_mess;

    if (USB_NO == is_init)
    {
        usb_pmsc_init ();
        pmsc_atapi_init ();

        is_init = USB_YES;
    }

    /* WAIT_LOOP */
    while(1)
    {
        err = rtos_receive_mailbox (&g_rtos_usb_pmsc_mbx_id, (void **)&p_mess, (rtos_time_t)1000);
        if (RTOS_SUCCESS == err)
        {
            switch (g_usb_pmsc_seq)
            {
                /* Normal  pmsc_seq Conditions */
                case USB_PMSC_CBWRCV:
                    switch (g_usb_pmscstatus)
                    {
                        /* Normal Status Condotion */
                        case USB_DATA_OK:
                            usb_pmsc_check_cbw();
                        break;
                        /* Abnormal Status Conditions */
                        case USB_DATA_SHT:
                        case USB_DATA_OVR:
                            USB_PRINTF0("### CBW Size Error \n");
                        break;
                        case USB_DATA_STOP:
                            USB_PRINTF0("### USB_PMSC_CBWRCV detach error\n");
                        break;
                        default:
                            USB_PRINTF0("### USB_PMSC_CBWRCV error\n");
                        break;
                    }
                break;

                case USB_PMSC_DATARCV:
                    switch (g_usb_pmscstatus)
                    {
                        /* Normal Status Condotion */
                        case USB_DATA_OK:
                        case USB_DATA_SHT:
                        case USB_DATA_OVR:
                            pmsc_atapi_command_processing(g_usb_pmsc_cbw.cbwcb, g_usb_pmscstatus, &usb_pmsc_data_transfer);
                        break;
                        /* Abnormal Stauts Conditions */
                        case USB_DATA_STOP:
                            USB_PRINTF0("### USB_PMSC_DATARCV detach error\n");
                        break;
                        default:
                            USB_PRINTF0("### USB_PMSC_DATARCV error\n");
                        break;
                    }
                break;

                case USB_PMSC_DATASND:
                    switch (g_usb_pmscstatus)
                    {
                        /* Normal Status Condotion */
                        case USB_DATA_NONE:
                            pmsc_atapi_command_processing(g_usb_pmsc_cbw.cbwcb, g_usb_pmscstatus, &usb_pmsc_data_transfer);
                        break;
                        /* Abnormal Status Conditions */
                        case USB_DATA_STOP:
                            USB_PRINTF0("### USB_PMSC_DATASND detach error\n");
                        break;
                        default :
                            USB_PRINTF0("### USB_PMSC_DATASND error\n");
                        break;
                    }
                break;

                case USB_PMSC_CSWSND :
                    switch (g_usb_pmscstatus)
                    {
                        /* Normal  Status Condotion */
                        case USB_DATA_NONE:
                            usb_pmsc_receive_cbw();
                        break;
                        /* Abnormal Status Conditions */
                        case USB_DATA_STOP:
                            USB_PRINTF0("### USB_PMSC_CSWSND detach error\n");
                        break;
                        default:
                            USB_PRINTF0("### USB_PMSC_CSWSND error\n");
                        break;
                    }
                break;

                /* Abnormal pmsc_seq Conditions */
                default:
                    USB_PRINTF1("### pmsc Sequence error:%x\n", g_usb_pmsc_seq);
                break;
            }
        }
    }
#endif /*(BSP_CFG_RTOS_USED != 0)*/

} /* End of function usb_pmsc_task() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_check_cbw
 Description     : Check CBW
 Arguments       : none
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_check_cbw (void)
{
    uint8_t     result;
    uint8_t     uc_case = USB_MSC_CASE_ERR;
    uint8_t     uc_pmsc_case = USB_MSC_CASE_ERR;

    result = USB_PMSC_CHECK;

    /*  CBW packet valiable check    */
    if (USB_MSC_CBW_SIGNATURE != g_usb_pmsc_cbw.dcbw_signature)
    {
        result = USB_PMSC_SIG_ERR;
    }

    /*  Check reserved bit   */
    if (0 != g_usb_pmsc_cbw.bmcbw_flags.BIT.reserved7)
    {
        result = USB_PMSC_IN_DATA_STALL_CSW_NG;
    }

    /*  Check reserved bit   */
    if (0 != g_usb_pmsc_cbw.bcbwlun.BIT.reserved4)
    {
        result = USB_PMSC_IN_DATA_STALL_CSW_NG;
    }

    /*  Check reserved bit   */
    if (0 != g_usb_pmsc_cbw.bcbwcb_length.BIT.reserved3)
    {
        result = USB_PMSC_IN_DATA_STALL_CSW_NG;
    }

    /*  Check Logical Unit Number   */
    if (g_usb_pmsc_cbw.bcbwlun.BIT.bcbwlun > 0)
    {
        result = USB_PMSC_IN_DATA_STALL_CSW_NG;
    }

    /*  Check Command Block Length  */
    if (g_usb_pmsc_cbw.bcbwcb_length.BIT.bcbwcb_length > USB_MSC_CBWCB_LENGTH)
    {
        result = USB_PMSC_IN_DATA_STALL_CSW_NG;
    }

    if (result == USB_PMSC_CHECK)
    {
        /*  Set data length */
        g_usb_pmsc_dtl = (uint32_t) g_usb_pmsc_cbw.dcbw_dtl_hi << 24;
        g_usb_pmsc_dtl |= ((uint32_t) g_usb_pmsc_cbw.dcbw_dtl_mh << 16);
        g_usb_pmsc_dtl |= ((uint32_t) g_usb_pmsc_cbw.dcbw_dtl_ml << 8);
        g_usb_pmsc_dtl |= (uint32_t) g_usb_pmsc_cbw.dcbw_dtl_lo;

        /* Analyze CBWCB Command */
        pmsc_atapi_analyze_cbwcb(g_usb_pmsc_cbw.cbwcb);

        /* Check Analyze_CBWCB result */
        switch (g_usb_pmsc_message.ar_rst)
        {
            case USB_ATAPI_NO_DATA:     /* No data commnad */

                uc_case = USB_MSC_DNXX;

            break;

            case USB_ATAPI_SND_DATAS:   /* Send data command */

                uc_case = USB_MSC_DIXX;

            break;

            case USB_ATAPI_RCV_DATAS:   /* Receive data command */

                uc_case = USB_MSC_DOXX;

            break;

            case USB_ATAPI_NOT_SUPPORT: /* Not Support Command */

                if (0ul == g_usb_pmsc_dtl)
                {
                    result = USB_PMSC_NO_DATA_CSW_NG;   /* No STALL Pipe */
                }
                else
                {
                    if (1 == g_usb_pmsc_cbw.bmcbw_flags.BIT.cbw_dir)
                    {
                        result = USB_PMSC_IN_DATA_STALL_CSW_NG;   /* IN Pipe Stall */
                    }
                    else
                    {
                        result = USB_PMSC_OUT_DATA_STALL_CSW_NG;   /* OUT Pipe Stall */
                    }
                }

            break;

            default:

                USB_PRINTF0("### Unexpcted Command Check Error\n");
                result = USB_PMSC_IN_DATA_STALL_CSW_NG;

            break;
        }
    }
    if (result == USB_PMSC_CHECK)
    {
        /* Check 13 case */
        uc_pmsc_case = usb_pmsc_check_case13(g_usb_pmsc_message.ul_size, &uc_case);

        /* Decide response according to 13 case */
        result = usb_pmsc_transfer_matrix(uc_pmsc_case);
    }

    switch (result)
    {
        case USB_PMSC_DATASND:
        case USB_PMSC_DATARCV:
        case USB_PMSC_CSWSND:

            g_usb_pmsc_seq = result;
            pmsc_atapi_command_processing(g_usb_pmsc_cbw.cbwcb, g_usb_pmscstatus, &usb_pmsc_data_transfer);

        break;

        case USB_PMSC_SIG_ERR:

            USB_PRINTF0("### ERROR0 \n");

            g_usb_pmsc_dtl = (uint32_t) g_usb_pmsc_cbw.dcbw_dtl_hi << 24;
            g_usb_pmsc_dtl |= ((uint32_t) g_usb_pmsc_cbw.dcbw_dtl_mh << 16);
            g_usb_pmsc_dtl |= ((uint32_t) g_usb_pmsc_cbw.dcbw_dtl_ml << 8);
            g_usb_pmsc_dtl |= (uint32_t) g_usb_pmsc_cbw.dcbw_dtl_lo;

            if (0ul == g_usb_pmsc_dtl)
            {
                usb_pmsc_csw_transfer(USB_MSC_CSW_NG);
            }
            else
            {
                if (1 == g_usb_pmsc_cbw.bmcbw_flags.BIT.cbw_dir)
                {
                    /* IN Stall & CSW(NG) transfer*/
                    usb_pstd_change_device_state(USB_DO_STALL, USB_CFG_PMSC_BULK_IN, &usb_pmsc_err_csw_ng);
                }
                else
                {
                    /* OUT Stall & CSW(NG) transfer */
                    usb_pstd_change_device_state(USB_DO_STALL, USB_CFG_PMSC_BULK_OUT, &usb_pmsc_err_csw_ng);
                }
            }

        break;

        case USB_PMSC_IN_DATA_STALL_CSW_NG:

            USB_PRINTF0("### ERROR1 \n");

            /* IN Stall & CSW(NG) transfer*/
            usb_pstd_change_device_state(USB_DO_STALL, USB_CFG_PMSC_BULK_IN, &usb_pmsc_err_csw_ng);

        break;

        case USB_PMSC_OUT_DATA_STALL_CSW_NG:

            USB_PRINTF0("### ERROR2 \n");

            /* OUT Stall & CSW(NG) transfer */
            usb_pstd_change_device_state(USB_DO_STALL, USB_CFG_PMSC_BULK_OUT, &usb_pmsc_err_csw_ng);

        break;

        case USB_PMSC_IN_DATA_STALL_CSW_ERR:

            USB_PRINTF0("### ERROR3 \n");
            g_usb_pmsc_dtl = 0x00ul;
            usb_pstd_change_device_state(USB_DO_STALL, USB_CFG_PMSC_BULK_IN, (usb_cb_t)&usb_pmsc_err_phase_err);

        break;

        case USB_PMSC_OUT_DATA_STALL_CSW_ERR:

            USB_PRINTF0("### ERROR4 \n");
            g_usb_pmsc_dtl = 0x00ul;
            usb_pstd_change_device_state(USB_DO_STALL, USB_CFG_PMSC_BULK_OUT, (usb_cb_t)&usb_pmsc_err_phase_err);

        break;

        case USB_PMSC_NO_DATA_CSW_NG:

            USB_PRINTF0("### ERROR5 \n");

            /* CSW(NG) transfer */
            usb_pmsc_csw_transfer(USB_MSC_CSW_NG);

        break;

        default :
        break;
    }
} /* End of function usb_pmsc_check_cbw() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_check_case13
 Description     : 13 case check
 Arguments       : uint32_t     ul_size      : size
                 : uint8_t      *uc_case     : case
 Return value    : uint8_t                   : result
 ***********************************************************************************************************************/
static uint8_t usb_pmsc_check_case13 (uint32_t ul_size, uint8_t *uc_case)
{
    uint8_t     result;

    if (0ul == g_usb_pmsc_dtl)
    {
        (*uc_case) |= (uint8_t)USB_MSC_XXHN;    /* Host No Data */
    }
    else if (0 != g_usb_pmsc_cbw.bmcbw_flags.BIT.cbw_dir)
    {
        (*uc_case) |= (uint8_t)USB_MSC_XXHI;    /* Host Recieved(IN) Data */
    }
    else
    {
        (*uc_case) |= (uint8_t)USB_MSC_XXHO;    /* Host Send(OUT) Data */
    }

    /* 13cases */
    switch (*uc_case)
    {
        case USB_MSC_DNHN:  /* Device No Data */

            result = USB_MSC_CASE01;

        break;

        case USB_MSC_DNHI:  /* Device No Data & Host Recieved(IN) Data */

            result = USB_MSC_CASE04;

        break;

        case USB_MSC_DNHO:  /* Device No Data & Host Send(OUT) Data */

            result = USB_MSC_CASE09;

        break;

        case USB_MSC_DIHN:  /* Device Send(IN) Data & Host No Data */

            result = USB_MSC_CASE02;

        break;

        case USB_MSC_DIHI:  /* Device Send(IN) Data & Host Recieved(IN) Data */

            if (g_usb_pmsc_dtl > ul_size)
            {
                result = USB_MSC_CASE05;
            }
            else
            {
                if (g_usb_pmsc_dtl == ul_size)
                {
                    result = USB_MSC_CASE06;
                }
                else
                {
                    result = USB_MSC_CASE07;
                }
            }

        break;

        case USB_MSC_DIHO:  /* Device Send(IN) Data & Host Send(OUT) Data */

            result = USB_MSC_CASE10;

        break;

        case USB_MSC_DOHN:  /* Device Recieved(OUT) Data & Host No Data */

            result = USB_MSC_CASE03;

        break;

        case USB_MSC_DOHI:  /* Device Recieved(OUT) Data & Host Recieved(IN) Data */

            result = USB_MSC_CASE08;

        break;

        case USB_MSC_DOHO:  /* Device Recieved(OUT) Data & Host Send(OUT) Data */

            if (g_usb_pmsc_dtl > ul_size)
            {
                result = USB_MSC_CASE11;
            }
            else
            {
                if (g_usb_pmsc_dtl == ul_size)
                {
                    result = USB_MSC_CASE12;
                }
                else
                {
                    result = USB_MSC_CASE13;
                }
            }
        break;

        default :

            USB_PRINTF0("### (Error) Not Found 13 Case \n");
            result = USB_MSC_CASE_ERR;

        break;
    }

    return result;
} /* End of function usb_pmsc_check_case13() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_transfer_matrix
 Description     : Decide response according to 13 case
 Arguments       : uint8_t      uc_case      : case
 Return value    : uint8_t                   : result
 ***********************************************************************************************************************/
static uint8_t usb_pmsc_transfer_matrix (uint8_t uc_case)
{
    uint8_t     result;

    switch (uc_case)
    {
        case USB_MSC_CASE01:    /* CSW transfer */

            result = USB_PMSC_CSWSND;   /* Transfer Command Status Wrapper */

        break;

        case USB_MSC_CASE05:
        case USB_MSC_CASE06:    /* Data Transfer(IN) */

            result = USB_PMSC_DATASND;  /* Data Transport */

        break;

        case USB_MSC_CASE12:    /* Data Transfer(OUT) */

            result = USB_PMSC_DATARCV;  /* Data Transport */

        break;

        case USB_MSC_CASE04:    /* CSW transfer */

            result = USB_PMSC_IN_DATA_STALL_CSW_NG;   /* Data Transfer(IN) */

        break;

        case USB_MSC_CASE09:    /* CSW transfer */
        case USB_MSC_CASE11:    /* Data Transfer(OUT) */

            result = USB_PMSC_OUT_DATA_STALL_CSW_NG;

        break;

        case USB_MSC_CASE02:
        case USB_MSC_CASE03:
        case USB_MSC_CASE07:
        case USB_MSC_CASE08:

            result = USB_PMSC_IN_DATA_STALL_CSW_ERR;

        break;

        case USB_MSC_CASE10:
        case USB_MSC_CASE13:

            result = USB_PMSC_OUT_DATA_STALL_CSW_ERR;

        break;

        default:

            g_usb_pmsc_dtl = 0x00ul;
            result = USB_PMSC_OUT_DATA_STALL_CSW_ERR;   /* Internal Device Error */
            USB_PRINTF0("### (Error) Not CASE1-13 \n");

        break;
    }
    return result;
} /* End of function usb_pmsc_transfer_matrix() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_csw_transfer
 Description     : Create CSW and Transfer
 Arguments       : uint8_t      csw_status      : status(USB_MSC_CSW_OK/USB_MSC_CSW_NG/USB_MSC_CSW_PHASE_ERR)
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_csw_transfer(uint8_t csw_status) {
    usb_pmsc_setcsw(csw_status);
    usb_pmsc_transfer_start(USB_CFG_PMSC_BULK_IN, USB_MSC_CSW_LENGTH, (uint8_t*)&g_usb_pmsc_csw);
    g_usb_pmsc_seq = USB_PMSC_CSWSND;
} /* End of function usb_pmsc_csw_transfer() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_setcsw
 Description     : Create CSW
 Arguments       : uint8_t      ar_resp      : status
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_setcsw (uint8_t ar_resp)
{
    /* Set CSW parameter */
    g_usb_pmsc_csw.dcsw_signature       = USB_MSC_CSW_SIGNATURE;
    g_usb_pmsc_csw.dcsw_tag             = g_usb_pmsc_cbw.dcbw_tag;
    g_usb_pmsc_csw.dcsw_data_residue_lo = (uint8_t)g_usb_pmsc_dtl;
    g_usb_pmsc_csw.dcsw_data_residue_ml = (uint8_t)(g_usb_pmsc_dtl >> 8);
    g_usb_pmsc_csw.dcsw_data_residue_mh = (uint8_t)(g_usb_pmsc_dtl >> 16);
    g_usb_pmsc_csw.dcsw_data_residue_hi = (uint8_t)(g_usb_pmsc_dtl >> 24);
    g_usb_pmsc_csw.bcsw_status = ar_resp;
} /* End of function usb_pmsc_setcsw() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_data_transfer
 Description     : Next treatment after ATAPI Command Execute
 Arguments       : usb_utr_t    *ptr         : Pointer to usb_utr_t structure
                 : uint16_t     data1        : Not used
                 : uint16_t     data2        : Not used
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_data_transfer (usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
    switch (mess->status)
    {
        case USB_PMSC_CMD_COMPLETE:     /* Command Execute -> Pass! */

            /* CSW(OK) transfer */
            usb_pmsc_csw_transfer(USB_MSC_CSW_OK);

        break;

        case USB_PMSC_CMD_SHT_COMPLETE: /* Command Execute -> Pass! */

            /* CSW(OK) transfer */
            usb_pstd_change_device_state(USB_DO_STALL, USB_CFG_PMSC_BULK_IN, &usb_pmsc_err_csw_ok);

        break;

        case USB_PMSC_CMD_FAILED:       /* Command Execute -> Fail */

            if (0ul != g_usb_pmsc_dtl)

            { /* Zero or short packet */
                usb_pstd_change_device_state(USB_DO_STALL, USB_CFG_PMSC_BULK_IN, &usb_pmsc_err_csw_ng);
            }
            else
            {
                /* CSW(NG) transfer */
                usb_pmsc_csw_transfer(USB_MSC_CSW_NG);
            }

        break;

        case USB_PMSC_CMD_CONTINUE:     /* Set Send /Recieve Data parameters */

            switch (g_usb_pmsc_seq)
            {
                case USB_PMSC_DATARCV:  /* OUT Data */
                    /* Data receive start */
                    usb_pmsc_transfer_start(USB_CFG_PMSC_BULK_OUT, mess->tranlen, (uint8_t*)mess->p_tranadr);

                break;

                case USB_PMSC_DATASND: /* IN Data */
                    /* Data transfer start */
                    usb_pmsc_transfer_start(USB_CFG_PMSC_BULK_IN, mess->tranlen, (uint8_t*)mess->p_tranadr);

                break;

                default:

                    USB_PRINTF0("### usb_pmsc_data_transfer PMSC error:\n");

                break;
            }

        break;

        case USB_PMSC_CMD_ERROR:

            USB_PRINTF2("### usb_pmsc_data_transfer CMD error1:%d, %x\n", g_usb_pmsc_seq,mess->status);
            USB_PRINTF1("### g_usb_pmsc_message:%08x\n", mess);
            USB_PRINTF2("### *tranadr:%08x, tranlen:%d\n", &mess->p_tranadr, mess->tranlen);
            USB_PRINTF0("### error\n");

        break;

        default:

            USB_PRINTF2("### usb_pmsc_data_transfer CMD error2:%d, %x\n", g_usb_pmsc_seq,mess->status);
            USB_PRINTF1("### g_usb_pmsc_message:%08x\n",mess);
            USB_PRINTF2("### *tranadr:%08x, tranlen:%d\n", &mess->p_tranadr, mess->tranlen);
            USB_PRINTF0("### error\n");

        break;
    }
} /* End of function usb_pmsc_data_transfer() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_err_csw_ok
 Description     : Send Stall Callback CSW(OK)
 Arguments       : usb_utr_t    *ptr         : Pointer to usb_utr_t structure
                 : uint16_t     data1        : Not used
                 : uint16_t     data2        : Not used
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_err_csw_ok(usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    /* CSW(NG) transfer */
    usb_pmsc_csw_transfer(USB_MSC_CSW_OK);
} /* End of function usb_pmsc_err_csw_ok() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_err_csw_ng
 Description     : Send Stall Callback CSW(NG)
 Arguments       : usb_utr_t    *ptr         : Pointer to usb_utr_t structure
                 : uint16_t     data1        : Not used
                 : uint16_t     data2        : Not used
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_err_csw_ng (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    /* CSW(NG) transfer */
    usb_pmsc_csw_transfer(USB_MSC_CSW_NG);
} /* End of function usb_pmsc_err_csw_ng() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_err_phase_err
 Description     : Send Stall Callback CSW(ERROR)
 Arguments       : usb_utr_t    *ptr         : Pointer to usb_utr_t structure
                 : uint16_t     data1        : Not used
                 : uint16_t     data2        : Not used
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_err_phase_err (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    /* CSW(ERROR) transfer */
    usb_pmsc_csw_transfer(USB_MSC_CSW_PHASE_ERR);
} /* End of function usb_pmsc_err_phase_err() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_receive_cbw
 Description     : Ready to receive CBW
 Arguments       : none
 Return value    : none
 ***********************************************************************************************************************/
void usb_pmsc_receive_cbw(void)
{
    g_usb_pmsc_dtl = 0x00ul;
    usb_pmsc_transfer_start(USB_CFG_PMSC_BULK_OUT, USB_MSC_CBWLENGTH, (uint8_t*) &g_usb_pmsc_cbw);
    g_usb_pmsc_seq = USB_PMSC_CBWRCV;
} /* End of function usb_pmsc_receive_cbw() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_transfer_start
 Description     : Tranfer data from USB
 Arguments       : uint16_t     pipe         : Pipe No
                 : uint32_t     size         : Data Size
                 : uint8_t      *table       : Data address
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_transfer_start (uint16_t pipe, uint32_t size, uint8_t *table)
{
    /* PIPE Transfer set */
    g_usb_pmsc_utr.keyword      = pipe;
    g_usb_pmsc_utr.p_tranadr    = table;
    g_usb_pmsc_utr.tranlen      = size;
    g_usb_pmsc_utr.complete     = (usb_cb_t)usb_pmsc_trans_result;

#if (BSP_CFG_RTOS_USED == 0)        /* Non-OS */
    g_usb_pmsctrans_complete = USB_FALSE;
#endif /*(BSP_CFG_RTOS_USED == 0)*/

    usb_pstd_transfer_start(&g_usb_pmsc_utr);
} /* End of function usb_pmsc_transfer_start() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_trans_result
 Description     : Transfer CallBack Function
 Arguments       : usb_utr_t    *mess        : Pointer to usb_utr_t structure.
                 : uint16_t     data1        : Not used
                 : uint16_t     data2        : Not used
 Return value    : none
 ***********************************************************************************************************************/
static void usb_pmsc_trans_result (usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
#if (BSP_CFG_RTOS_USED == 0)        /* Non-OS */
    g_usb_pmsctrans_complete = USB_TRUE;
#else  /*(BSP_CFG_RTOS_USED == 0)*/
    rtos_send_mailbox (&g_rtos_usb_pmsc_mbx_id, (void *)mess);

#endif /*(BSP_CFG_RTOS_USED == 0)*/
    g_usb_pmscstatus = mess->status;
} /* End of function usb_pmsc_trans_result() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_mass_strage_reset
 Description     : Process Mass storage reset request
 Arguments       : uint16_t     value        : value
                 : uint16_t     index        : index
                 : uint16_t     length       : length
 Return value    : none
 ***********************************************************************************************************************/
void usb_pmsc_mass_strage_reset(uint16_t value, uint16_t index, uint16_t length)
{
    if ((0 == value) && (0 == length))
    {
        USB_PRINTF0("### Mass Storage Reset Request \n");
        usb_cstd_set_buf(USB_NULL, (uint16_t) USB_PIPE0);

        /* Please add the processing with the system. */
        usb_pmsc_receive_cbw();
        usb_pstd_ctrl_end(USB_CTRL_END);
    }
    else
    {
        /* Set Stall */
        usb_pstd_set_stall_pipe0(); /* Req Error */
    }
} /* End of function usb_pmsc_mass_strage_reset() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_get_max_lun
 Description     : Process Get max LUN request
 Arguments       : uint16_t     value        : value
                 : uint16_t     index        : index
                 : uint16_t     length       : length
 Return value    : none
 ***********************************************************************************************************************/
void usb_pmsc_get_max_lun(uint16_t value, uint16_t index, uint16_t length)
{
    uint8_t     uc_lun = 0;     /* Logical Unit Number */

    if (0x0000 == value)
    {
        if (USB_MSC_LUN_LENGTH == length)
        {
            USB_PRINTF0("*** GetMaxLun Request \n");
            usb_pstd_ctrl_read((uint32_t) USB_MSC_LUN_LENGTH, &uc_lun);
            usb_pstd_ctrl_end(USB_CTRL_END);
        }
        else
        {
            /* Set Stall */
            usb_pstd_set_stall_pipe0(); /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_set_stall_pipe0(); /* Req Error */
    }
} /* End of function usb_pmsc_get_max_lun() */

/***********************************************************************************************************************
 Function Name   : usb_pmsc_init
 Description     : 
 Arguments       : none
 Return value    : none
 ***********************************************************************************************************************/
void usb_pmsc_init (void)
{
    g_usb_pmsc_seq = 0;
#if (BSP_CFG_RTOS_USED == 0)        /* Non-OS */
    g_usb_pmsctrans_complete = USB_FALSE;
#endif /*(BSP_CFG_RTOS_USED == 0)*/
    g_usb_pmscstatus = 0;
    memset((void *)&g_usb_pmsc_cbw, 0, sizeof(usb_msc_cbw_t));
    memset((void *)&g_usb_pmsc_csw, 0, sizeof(usb_msc_csw_t));
    memset((void *)&g_usb_pmsc_utr, 0, sizeof(usb_utr_t));
    g_usb_pmsc_dtl = 0;
    memset((void *)&g_usb_pmsc_message, 0, sizeof(usb_pmsc_cbm_t));
} /* End of function usb_pmsc_init() */


/***********************************************************************************************************************
 End Of File
 ***********************************************************************************************************************/
