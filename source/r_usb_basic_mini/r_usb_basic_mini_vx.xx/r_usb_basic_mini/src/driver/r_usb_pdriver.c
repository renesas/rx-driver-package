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
 * Copyright (C) 2015(2019) Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * File Name    : r_usb_pdriver.c
 * Description  : USB Peripheral driver code.
 ******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
#include "r_usb_dmac.h"
#endif  /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
******************************************************************************/

static void usb_pstd_interrupt_process (uint16_t type, uint16_t status);


/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
uint16_t g_usb_pstd_is_stall_pipe[USB_MAX_PIPE_NO + 1u];   /* Stall Pipe info */
usb_pcb_t g_usb_pstd_stall_cb;                          /* Stall Callback function */
uint16_t g_usb_pstd_config_no = 0;                      /* Current configuration number */
uint16_t g_usb_pstd_alt_no[USB_ALT_NO];                 /* Alternate number */
uint16_t g_usb_pstd_remote_wakeup = USB_FALSE;          /* Remote wake up enable flag */
uint16_t g_usb_pstd_remote_wakeup_state;                /* Result for Remote wake up */

uint16_t g_usb_pstd_req_type;                           /* Request type */
uint16_t g_usb_pstd_req_value;                          /* Value */
uint16_t g_usb_pstd_req_index;                          /* Index */
uint16_t g_usb_pstd_req_length;                         /* Length */

uint16_t g_usb_pstd_pipe0_request;
uint16_t g_usb_pstd_std_request;
uint16_t g_usb_pstd_is_connected;                       /* Status for USB connect. */

/* Driver registration */
usb_pcdreg_t g_usb_pstd_driver;
usb_setup_t g_usb_pstd_req_reg;                         /* Device Request - Request structure */


/******************************************************************************
 Function Name   : usb_pstd_interrupt_process
 Description     : Analyze the USB Peripheral interrupt event and execute the
                 : appropriate process.
 Arguments       : uint16_t type      : Interrupt type.
                 : uint16_t status    : BRDYSTS register & BRDYENB register.
 Return value    : none
 ******************************************************************************/
static void usb_pstd_interrupt_process (uint16_t type, uint16_t status)
{
    uint16_t stginfo;

    /* check interrupt status */
    switch (type)
    {
        /* BRDY, BEMP, NRDY */
        case USB_INT_BRDY :
            usb_pstd_brdy_pipe(status);
        break;
        case USB_INT_BEMP :
            usb_pstd_bemp_pipe(status);
        break;
        case USB_INT_NRDY :
            usb_pstd_nrdy_pipe(status);
        break;

            /* Resume */
        case USB_INT_RESM :
            USB_PRINTF0("RESUME int peri\n");

            /* Callback */
            if (USB_NULL != g_usb_pstd_driver.devresume)
            {
                (*g_usb_pstd_driver.devresume)(USB_NULL, USB_NO_ARG, USB_NULL);
            }
            usb_pstd_resume_process();
        break;

            /* VBUS */
        case USB_INT_VBINT :
            if (USB_ATTACH == usb_pstd_chk_vbsts())
            {
                USB_PRINTF0("VBUS int attach\n");
                usb_pstd_attach_process();    /* USB attach */
            }
            else
            {
                USB_PRINTF0("VBUS int detach\n");
                usb_pstd_detach_process();    /* USB detach */
            }
        break;

            /* SOF */
        case USB_INT_SOFR :

            /* User program */
        break;

            /* DVST */
        case USB_INT_DVST :
            switch ((uint16_t) (status & USB_DVSQ))
            {
                /* Power state  */
                case USB_DS_POWR :
                break;

                    /* Default state  */
                case USB_DS_DFLT :
                    USB_PRINTF0("USB-reset int peri\n");
                    usb_pstd_bus_reset();
                break;

                    /* Address state  */
                case USB_DS_ADDS :
                break;

                    /* Configured state  */
                case USB_DS_CNFG :
                    USB_PRINTF0("Device configuration int peri\n");
                break;

                    /* Power suspend state */
                case USB_DS_SPD_POWR :

                    /* Continue */
                    /* Default suspend state */
                case USB_DS_SPD_DFLT :

                    /* Continue */
                    /* Address suspend state */
                case USB_DS_SPD_ADDR :

                    /* Continue */
                    /* Configured Suspend state */
                case USB_DS_SPD_CNFG :
                    USB_PRINTF0("SUSPEND int peri\n");
                    usb_pstd_suspend_process();
                break;

                    /* Error */
                default :
                break;
            }
        break;

            /* CTRT */
        case USB_INT_CTRT :
            stginfo = (uint16_t) (status & USB_CTSQ);
            if (USB_CS_IDST == stginfo)
            {
                /* Non process */
            }
            else
            {
                if (((USB_CS_RDDS == stginfo) || (USB_CS_WRDS == stginfo)) || (USB_CS_WRND == stginfo))
                {
                    /* Save request register */
                    usb_pstd_save_request();
                }
            }

            if ((g_usb_pstd_req_type & USB_BMREQUESTTYPETYPE) == USB_STANDARD)
            {
                /* Switch on the control transfer stage (CTSQ). */
                switch (stginfo)
                {
                    /* Idle or setup stage */
                    case USB_CS_IDST :
                        g_usb_pstd_pipe0_request = USB_OFF;
                        g_usb_pstd_std_request = USB_NO;
                        usb_pstd_stand_req0();
                    break;

                    /* Control read data stage */
                    case USB_CS_RDDS :
                        usb_pstd_stand_req1();
                    break;

                    /* Control write data stage */
                    case USB_CS_WRDS :
                        usb_pstd_stand_req2();
                    break;

                    /* Status stage of a control write where there is no data stage. */
                    case USB_CS_WRND :
                        usb_pstd_stand_req3();
                    break;

                    /* Control read status stage */
                    case USB_CS_RDSS :
                        usb_pstd_stand_req4();
                    break;

                    /* Control write status stage */
                    case USB_CS_WRSS :
                        usb_pstd_stand_req5();
                    break;

                    /* Control sequence error */
                    case USB_CS_SQER :
                        usb_pstd_ctrl_end((uint16_t) USB_DATA_ERR);
                    break;

                    /* Illegal */
                    default :
                        usb_pstd_ctrl_end((uint16_t) USB_DATA_ERR);
                    break;
                }
            }
            else
            {
                /* Vender Specific */
                g_usb_pstd_req_reg.type = g_usb_pstd_req_type;
                g_usb_pstd_req_reg.value = g_usb_pstd_req_value;
                g_usb_pstd_req_reg.index = g_usb_pstd_req_index;
                g_usb_pstd_req_reg.length = g_usb_pstd_req_length;

                /* Callback */
                if (USB_NULL != g_usb_pstd_driver.ctrltrans)
                {
                    (*g_usb_pstd_driver.ctrltrans)((usb_setup_t *) &g_usb_pstd_req_reg, stginfo);
                }
            }
        break;

            /* Error */
        case USB_INT_UNKNOWN :
            USB_PRINTF0("pINT_UNKNOWN\n");
        break;
        default :
        break;
    }
} /* End of function usb_pstd_interrupt_process */



/******************************************************************************
 Function Name   : usb_pstd_pcd_task
 Description     : The Peripheral Control Driver(PCD) task.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_pcd_task (void)
{
    if (g_usb_pstd_usb_int.wp != g_usb_pstd_usb_int.rp)
    {
        /* Pop Interrupt info */
        usb_pstd_interrupt_process(g_usb_pstd_usb_int.buf[g_usb_pstd_usb_int.rp].type,
                g_usb_pstd_usb_int.buf[g_usb_pstd_usb_int.rp].status);

        /* Read CountUp */
        g_usb_pstd_usb_int.rp = ((g_usb_pstd_usb_int.rp + 1) % USB_INT_BUFSIZE);
    }
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    usb_cstd_dma_driver();           /* USB DMA driver */
#endif  /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */
} /* End of function usb_pstd_pcd_task */

/******************************************************************************
 Function Name   : usb_pstd_set_submitutr
 Description     : USB Peripheral Submit utr.
 Arguments       : usb_putr_t *p_utrmsg    : Pointer to usb_putr_t structure
 Return value    : usb_er_t
 ******************************************************************************/
usb_er_t usb_pstd_set_submitutr (usb_putr_t * p_utrmsg)
{
    uint16_t pipenum;

    pipenum = p_utrmsg->keyword;
    gp_usb_pstd_pipe[pipenum] = p_utrmsg;

    /* Check state (Configured) */
    if (USB_TRUE == usb_pstd_chk_configured())
    {
        /* Data transfer */
        if (USB_DIR_P_OUT == usb_cstd_get_pipe_dir(pipenum))
        {
            usb_pstd_receive_start(pipenum);    /* Out transfer */
        }
        else
        {
            /* In transfer */
            usb_pstd_send_start(pipenum);
        }
    }
    else
    {
        /* Transfer stop */
        usb_pstd_forced_termination(pipenum, (uint16_t) USB_DATA_ERR);
    }
    return USB_OK;
} /* End of function usb_pstd_set_submitutr */

/******************************************************************************
 Function Name   : usb_pstd_clr_alt
 Description     : Zero-clear the alternate table (buffer).
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_clr_alt (void)
{
    uint16_t i;

    /* WAIT_LOOP */
    for (i = 0; i < USB_ALT_NO; ++i)
    {
        /* Alternate table clear */
        g_usb_pstd_alt_no[i] = 0;
    }
} /* End of function usb_pstd_clr_alt */

/******************************************************************************
 Function Name   : usb_pstd_clr_mem
 Description     : Initialize global variables defined for peripheral mode.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_clr_mem (void)
{
    g_usb_pstd_config_no = 0;               /* Configuration number */
    g_usb_pstd_remote_wakeup = USB_FALSE;    /* Remote wake up enable flag */
    usb_pstd_clr_alt();                      /* Alternate setting clear */
} /* End of function usb_pstd_clr_mem */

/******************************************************************************
 Function Name   : usb_pstd_set_config_no
 Description     : Set specified configuration number.
 Arguments       : uint16_t value    : Configuration number
 Return value    : none
 ******************************************************************************/
void usb_pstd_set_config_no (uint16_t value)
{
    g_usb_pstd_config_no = value;    /* Set configuration number */
    usb_pstd_clr_alt();               /* Alternate setting clear */
} /* End of function usb_pstd_set_config_no */

/******************************************************************************
 Function Name   : usb_pstd_get_interface_no
 Description     : Get interface number
 Arguments       : uint16_t con_num : Configuration Number
 Return value    : uint16_t         : Number of this interface(bNumInterfaces)
 ******************************************************************************/
uint16_t usb_pstd_get_interface_no (void)
{
    uint16_t num_if = 0;

    /* Get NumInterfaces.        4:bNumInterfaces */
    num_if = *(uint8_t *) (g_usb_pstd_driver.p_configtbl + USB_DEV_B_NUM_INTERFACES);

    return num_if;
} /* End of function usb_pstd_get_interface_no */

/******************************************************************************
 Function Name   : usb_pstd_get_alternate_no
 Description     : Get Alternate Setting Number
 Arguments       : uint16_t con_num : Configuration Number
                 : uint16_t int_num : Interface Number
 Return value    : uint16_t         : Value used to select this alternate (bAlternateSetting)
 ******************************************************************************/
uint16_t usb_pstd_get_alternate_no (uint16_t int_num)
{
    uint16_t  i;
    uint16_t  alt_num = 0;
    uint8_t * p_utr;
    uint16_t  length;

    p_utr = g_usb_pstd_driver.p_configtbl;
    i = p_utr[0];

    /* Interface descriptor[0] */
    p_utr = (uint8_t *) ((uint32_t) p_utr + p_utr[0]);
    length = (uint16_t) (*(uint8_t *) ((uint32_t) g_usb_pstd_driver.p_configtbl + (uint16_t) 2u));
    length |= (uint16_t) ((uint16_t) (*(uint8_t *) ((uint32_t) g_usb_pstd_driver.p_configtbl + (uint16_t) 3u)) << 8u);

    /* Search descriptor table size */
    /* WAIT_LOOP */
    for (; i < length;)
    {
        /* Descriptor type ? */
        switch (p_utr[1])
        {
            /* Interface */
            case USB_DT_INTERFACE :
                if (int_num == p_utr[2])
                {
                    /* Alternate number count */
                    alt_num = (uint16_t) p_utr[3];
                }
                i += p_utr[0];

                /* Interface descriptor[0] */
                p_utr = (uint8_t *) ((uint32_t) p_utr + p_utr[0]);
            break;

                /* Device */
            case USB_DT_DEVICE :

                /* Continue */
                /* Configuration */
            case USB_DT_CONFIGURATION :

                /* Continue */
                /* String */
            case USB_DT_STRING :

                /* Continue */
                /* EndPoint */
            case USB_DT_ENDPOINT :

                /* Continue */
                /* Class, Vendor, else */
            default :
                i += p_utr[0];

                /* Interface descriptor[0] */
                p_utr = (uint8_t *) ((uint32_t) p_utr + p_utr[0]);
            break;
        }
    }
    return alt_num;
} /* End of function usb_pstd_get_alternate_no */

/******************************************************************************
 Function Name   : usb_pstd_set_eptbl_index
 Description     : Set endpoint index in table (buffer) region based on config-
                 : uration descriptor. In other words, set which endpoints to
                 : use based on specified configuration,
 Arguments       : uint16_t con_num : Configuration Number.
                 : uint16_t int_num : Interface Number.
                 : uint16_t alt_num : Alternate Setting.
 Return value    : none
 ******************************************************************************/
void usb_pstd_set_eptbl_index (uint16_t int_num, uint16_t alt_num)
{
    uint8_t * p_utr;
    uint16_t i;
    uint16_t j;
    uint16_t length;
    uint16_t start;
    uint16_t numbers;
    uint16_t ep;

    /* Configuration descriptor */
    p_utr = g_usb_pstd_driver.p_configtbl;
    i = *p_utr;
    length = (uint16_t) (*(uint8_t *) ((uint32_t) p_utr + (uint32_t) 3u));
    length = (uint16_t) (length << 8);
    length += (uint16_t) (*(uint8_t *) ((uint32_t) p_utr + (uint32_t) 2u));
    p_utr = (uint8_t *) ((uint32_t) p_utr + (*p_utr));
    start = 0;
    numbers = 0;
    j = 0;

    /* WAIT_LOOP */
    for (; i < length;)
    {
        /* Descriptor type ? */
        switch (*(uint8_t *) ((uint32_t) p_utr + (uint32_t) 1u))
        {
            /* Interface */
            case USB_DT_INTERFACE :
                if (((*(uint8_t *) ((uint32_t) p_utr + (uint32_t) 2u)) == int_num)
                        && ((*(uint8_t *) ((uint32_t) p_utr + (uint32_t) 3u)) == alt_num))
                {
                    numbers = *(uint8_t *) ((uint32_t) p_utr + (uint32_t) 4u);
                }
                else
                {
                    start += (*(uint8_t *) ((uint32_t) p_utr + (uint32_t) 4u));
                }
                i += (*p_utr);
                p_utr = (uint8_t *) ((uint32_t) p_utr + (*p_utr));
            break;

                /* Endpoint */
            case USB_DT_ENDPOINT :
                if (j < numbers)
                {
                    ep = (uint16_t) *(uint8_t *) ((uint32_t) p_utr + (uint32_t) 2u);
                    ep &= (uint16_t) 0x0f;
                    ++j;
                }
                i += (*p_utr);
                p_utr = (uint8_t *) ((uint32_t) p_utr + (*p_utr));
            break;

                /* Device */
            case USB_DT_DEVICE :

                /* Continue */
                /* Configuration */
            case USB_DT_CONFIGURATION :

                /* Continue */
                /* String */
            case USB_DT_STRING :

                /* Continue */
                /* Class, Vendor, else */
            default :
                i += (*p_utr);
                p_utr = (uint8_t *) ((uint32_t) p_utr + (*p_utr));
            break;
        }
    }
} /* End of function usb_pstd_set_eptbl_index */

/******************************************************************************
 Function Name   : usb_pstd_chk_remote
 Description     : Check if the RemoteWakeUp bit for the configuration descriptor is set.
 Arguments       : none
 Return value    : uint16_t : remote wakeup status (TRUE/FALSE).
 ******************************************************************************/
uint16_t usb_pstd_chk_remote (void)
{
    uint8_t atr;

    if (0 == g_usb_pstd_config_no)
    {
        return USB_FALSE;
    }

    /* Get Configuration Descriptor - bmAttributes */
    atr = *(uint8_t *) ((uint32_t) g_usb_pstd_driver.p_configtbl + (uint32_t) 7u);

    /* Remote WakeUp check(= D5) */
    if ((atr & USB_CF_RWUPON) == USB_CF_RWUPON)
    {
        return USB_TRUE;
    }
    return USB_FALSE;
} /* End of function usb_pstd_chk_remote */

/******************************************************************************
 Function Name   : usb_pstd_get_current_power
 Description     : Find out how the peripheral is powered by looking at the con-
                 : figuration descriptor.
 Arguments       : none
 Return value    : uint8_t : Current power means; self-powered or bus-powered
                 : (GS_SELFPOWERD/GS_BUSPOWERD).
 ******************************************************************************/
uint8_t usb_pstd_get_current_power (void)
{
    /*
     * Please answer the currently power of your system.
     */

    uint8_t tmp;
    uint8_t currentpower;

    /* Standard configuration descriptor */
    tmp = *(uint8_t *) ((uint32_t) g_usb_pstd_driver.p_configtbl + (uint32_t) 7u);
    if ((tmp & USB_CF_SELFP) == USB_CF_SELFP)
    {
        /* Self Powered */
        currentpower = USB_GS_SELFPOWERD;
    }
    else
    {
        /* Bus Powered */
        currentpower = USB_GS_BUSPOWERD;
    }

    /* Check currently powered */

    return currentpower;
} /* End of function usb_pstd_get_current_power */

/******************************************************************************
 Function Name   : usb_pstd_fifo_to_buf
 Description     : Request readout from USB FIFO to buffer and process depending
                 : on status; read complete or reading.
 Arguments       : uint16_t pipe     : Pipe no.
                 : uint16_t useport  : FIFO select(USB_CUSE,USB_DMA0,....)
 Return value    : none
 ******************************************************************************/
void usb_pstd_fifo_to_buf(uint16_t pipe, uint16_t useport)
{
    uint16_t end_flag;

    end_flag = USB_ERROR;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    end_flag = usb_pstd_read_data(pipe, useport);

    /* Check FIFO access sequence */
    switch (end_flag)
    {
        case USB_READING:

        /* Continue of data read */
        break;

        case USB_READEND:

        /* End of data read */
        usb_pstd_data_end(pipe, (uint16_t)USB_DATA_OK);
        break;

        case USB_READSHRT:

        /* End of data read */
        usb_pstd_data_end(pipe, (uint16_t)USB_DATA_SHT);
        break;

        case USB_READOVER:

        /* Buffer over */
        USB_PRINTF1("### Receive data over PIPE%d\n", pipe);
        usb_pstd_forced_termination(pipe, (uint16_t)USB_DATA_OVR);
        break;

        case USB_FIFOERROR:

        /* FIFO access error */
        USB_PRINTF0("### FIFO access error \n");
        usb_pstd_forced_termination(pipe, (uint16_t)USB_DATA_ERR);
        break;

        default:
        usb_pstd_forced_termination(pipe, (uint16_t)USB_DATA_ERR);
        break;
    }
} /* End of function usb_pstd_fifo_to_buf */

/******************************************************************************
 Function Name   : usb_pstd_buf_to_fifo
 Description     : Set USB registers as required to write from data buffer to USB
 : FIFO, to have USB FIFO to write data to bus.
 Arguments       : uint16_t pipe     : Pipe no.
                 : uint16_t useport  : Port no.
 Return value    : none
 ******************************************************************************/
void usb_pstd_buf_to_fifo(uint16_t pipe, uint16_t useport)
{
    uint16_t end_flag;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Disable Ready Interrupt */
    hw_usb_clear_brdyenb(pipe);

    end_flag = usb_pstd_write_data(pipe, useport);

    /* Check FIFO access sequence */
    switch (end_flag)
    {
        case USB_WRITING:
            /* Continue of data write */
            /* Enable Ready Interrupt */
            hw_usb_set_brdyenb(pipe);

            /* Enable Not Ready Interrupt */
            usb_cstd_nrdy_enable(pipe);
        break;

        case USB_WRITEEND:
            /* End of data write */
            /* continue */
        case USB_WRITESHRT:
            /* End of data write */
            /* Enable Empty Interrupt */
            hw_usb_set_bempenb(pipe);

            /* Enable Not Ready Interrupt */
            usb_cstd_nrdy_enable(pipe);
        break;

        case USB_FIFOERROR:
            /* FIFO access error */
            USB_PRINTF0("### FIFO access error \n");
            usb_pstd_forced_termination(pipe, (uint16_t)USB_DATA_ERR);
        break;

        default:
            usb_pstd_forced_termination(pipe, (uint16_t)USB_DATA_ERR);
        break;
    }
} /* End of function usb_pstd_buf_to_fifo */

/******************************************************************************
 Function Name   : usb_pstd_transfer_start
 Description     : Transfer the data of each pipe
                 : Request PCD to transfer data, and the PCD transfers the data
                 : based on the transfer information stored in ptr
 Arguments       : usb_putr_t *ptr : Pointer to usb_putr_t structure.
 Return value    : usb_er_t       : Error info.
 ******************************************************************************/
usb_er_t usb_pstd_transfer_start(usb_putr_t * p_utr)
{
    usb_er_t err;
    uint16_t pipenum;

    pipenum = p_utr->keyword;
    if (USB_NULL != gp_usb_pstd_pipe[pipenum])
    {
        /* Get PIPE TYPE */
        if (usb_cstd_get_pipe_type(pipenum) != USB_TYPFIELD_ISO)
        {
            USB_PRINTF1("### usb_pstd_transfer_start overlaps %d\n", pipenum);
            return USB_QOVR;
        }
    }

    /* Check state (Configured) */
    if (USB_TRUE != usb_pstd_chk_configured())
    {
        USB_PRINTF0("### usb_pstd_transfer_start not configured\n");
        return USB_ERROR;
    }

    if (USB_PIPE0 == pipenum)
    {
        USB_PRINTF0("### usb_pstd_transfer_start PIPE0 is not support\n");
        return USB_ERROR;
    }

    err = usb_pstd_set_submitutr(p_utr);
    return err;
} /* End of function usb_pstd_transfer_start */

/******************************************************************************
 Function Name   : usb_pstd_transfer_end
 Description     : Force termination of data transfer of specified pipe. Request
                 : PCD to force termination data transfer, 
                 : and the PCD forced-terminates data transfer.
 Arguments       : uint16_t pipe     : Pipe number.
 Return value    : usb_er_t          : Error info.
 ******************************************************************************/
usb_er_t usb_pstd_transfer_end(uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_ERROR; /* Error */
    }

    if (USB_NULL == gp_usb_pstd_pipe[pipe])
    {
        USB_PRINTF0("### usb_pstd_transfer_end overlaps\n");
        return USB_ERROR;
    }
    else
    {
        usb_pstd_forced_termination(pipe, (uint16_t)USB_DATA_STOP);
    }

    return USB_OK;
} /* End of function usb_pstd_transfer_end */

/******************************************************************************
 Function Name   : usb_pstd_change_device_state
 Description     : Change USB Device to the status specified by argument
 Arguments       : uint16_t state            : New device status.
                 : uint16_t keyword          : Pipe number etc.
                 : usb_pcb_t complete         : Callback function.
 Return value    : none
 ******************************************************************************/
void usb_pstd_change_device_state(uint16_t state, uint16_t keyword, usb_pcb_t complete)
{
    uint16_t pipenum;

    pipenum = keyword;
    switch (state)
    {
        case USB_DO_STALL:
            usb_pstd_set_stall(pipenum);
            g_usb_pstd_is_stall_pipe[pipenum] = USB_TRUE;
            g_usb_pstd_stall_cb = complete;
        break;

        case USB_DO_REMOTEWAKEUP:
            usb_pstd_remote_wakeup();
        break;

        case USB_MSG_PCD_DP_ENABLE:
            hw_usb_pset_dprpu();
        break;

        case USB_MSG_PCD_DP_DISABLE:
            hw_usb_pclear_dprpu();
        break;
        
        default:
        break;
    }
} /* End of function usb_pstd_change_device_state */

/******************************************************************************
 Function Name   : usb_pstd_driver_registration
 Description     : Register pipe information table, descriptor information table,
                 : callback function, etc. This info is specified by the data in
                 : the structure usb_pcdreg_t.
 Arguments       : usb_pcdreg_t *registinfo     : Class driver structure.
 Return value    : none
 ******************************************************************************/
void usb_pstd_driver_registration(usb_pcdreg_t * p_registinfo)
{
    usb_pcdreg_t *p_driver;

    p_driver = &g_usb_pstd_driver;

    p_driver->p_devicetbl  = p_registinfo->p_devicetbl;  /* Device descriptor table address */
    p_driver->p_configtbl  = p_registinfo->p_configtbl;  /* Configuration descriptor table address */
    p_driver->pp_stringtbl = p_registinfo->pp_stringtbl;  /* String descriptor table address */
    p_driver->devdefault   = p_registinfo->devdefault;   /* Device default */
    p_driver->devconfig    = p_registinfo->devconfig;    /* Device configured */
    p_driver->devdetach    = p_registinfo->devdetach;    /* Device detach */
    p_driver->devsuspend   = p_registinfo->devsuspend;   /* Device suspend */
    p_driver->devresume    = p_registinfo->devresume;    /* Device resume */
    p_driver->interface    = p_registinfo->interface;    /* Interfaced change */
    p_driver->ctrltrans    = p_registinfo->ctrltrans;    /* Control transfer */
    p_driver->num_string   = p_registinfo->num_string;   /* String descriptor number */
} /* End of function usb_pstd_driver_registration */

/******************************************************************************
 Function Name   : usb_pstd_driver_release
 Description     : Clear the information registered in the structure usb_pcdreg_t.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_driver_release(void)
{
    usb_pcdreg_t *p_driver;

    p_driver = &g_usb_pstd_driver;
    p_driver->p_devicetbl  = (uint8_t*)0u;              /* Device descriptor table address */
    p_driver->p_configtbl  = (uint8_t*)0u;              /* Configuration descriptor table address */
    p_driver->pp_stringtbl = (uint8_t**)0u;             /* String descriptor table address */
    p_driver->devdefault   = &usb_pstd_dummy_function;  /* Device default */
    p_driver->devconfig    = &usb_pstd_dummy_function;  /* Device configured */
    p_driver->devdetach    = &usb_pstd_dummy_function;  /* Device detach */
    p_driver->devsuspend   = &usb_pstd_dummy_function;  /* Device suspend */
    p_driver->devresume    = &usb_pstd_dummy_function;  /* Device resume */
    p_driver->interface    = &usb_pstd_dummy_function;  /* Interfaced change */
    p_driver->ctrltrans    = &usb_pstd_dummy_trn;       /* Control transfer */

}/* End of function usb_pstd_driver_release() */

/******************************************************************************
 Function Name   : usb_pstd_dummy_function
 Description     : dummy function
 Arguments       : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return value    : none
 ******************************************************************************/
void usb_pstd_dummy_function(usb_putr_t *p_utr,uint16_t data1, uint16_t data2)
{
    /* Dummy function */
} /* End of function usb_pstd_dummy_function */

/******************************************************************************
 Function Name   : usb_pstd_dummy_trn
 Description     : Class request processing for dummy
 Arguments       : usb_setup_t *preq  ; Class request information
 : uint16_t ctsq        ; Control Stage
 Return value    : none
 ******************************************************************************/
void usb_pstd_dummy_trn(usb_setup_t * p_req, uint16_t ctsq)
{
    /* Dummy function */
} /* End of function usb_pstd_dummy_trn */

/******************************************************************************
 Function Name   : usb_pstd_registration
 Description     : Registration of peripheral Devices Class Driver
 Arguments       : usb_putr_t *p_cfg : Pointer to config structure.
 Return value    : none
 ******************************************************************************/
void usb_pstd_registration(usb_cfg_t *p_cfg)
{
    usb_pcdreg_t pdriver;

    /* pdriver registration */
    pdriver.p_devicetbl   = p_cfg->p_usb_reg->p_device;         /* Device descriptor Table address */
    pdriver.p_configtbl   = p_cfg->p_usb_reg->p_config_f;       /* Configuration descriptor Table address */
    pdriver.pp_stringtbl  = p_cfg->p_usb_reg->pp_string;         /* String descriptor Table address */
    pdriver.num_string    = p_cfg->p_usb_reg->num_string;       /* Num entry String descriptor */

    pdriver.devdefault  = &usb_pstd_devdefault;                 /* Device default */
    pdriver.devconfig   = &usb_pstd_configured;                 /* Device configuered */
    pdriver.devdetach   = &usb_pstd_detach;                     /* Device detach */
    pdriver.devsuspend  = (usb_pcb_t)&usb_pstd_suspended;       /* Device suspend */
    pdriver.devresume   = &usb_pstd_resume;                     /* Device resume */
    pdriver.interface   = &usb_pstd_interface;                  /* Interface changed */
    pdriver.ctrltrans   = (usb_cb_trn_t)&usb_pstd_class_request;/* Control Transfer */

    usb_pstd_driver_registration((usb_pcdreg_t*) &pdriver);
} /* End of function usb_pstd_registration() */

/******************************************************************************
 Function Name   : usb_pstd_devdefault
 Description     : Descriptor change
 Arguments       : usb_putr_t *p_utr       : Pointer to usb_putr_t structure.
                 : uint16_t mode        ; Speed mode
                 : uint16_t data2       ; BC Port Detect Result
 Return value    : none
 ******************************************************************************/
void usb_pstd_devdefault (usb_putr_t *p_utr, uint16_t mode, uint16_t data2)
{
    uint8_t *p_table;
    uint16_t len;
#if (defined(USB_CFG_PCDC_USE) | defined(USB_CFG_PHID_USE))
    usb_ctrl_t ctrl;
#endif

    usb_pstd_detach(p_utr, USB_NULL, USB_NULL);

    p_table = g_usb_pstd_driver.p_configtbl;

    len = (uint16_t) (*(uint8_t*) ((uint32_t) p_table + (uint32_t) 3));
    len = (uint16_t) (len << 8);
    len += (uint16_t) (*(uint8_t*) ((uint32_t) p_table + (uint32_t) 2));

    usb_cstd_pipe_table_clear();
    usb_pstd_pipe_info(p_table, len);

#if (defined(USB_CFG_PCDC_USE) | defined(USB_CFG_PHID_USE))
    usb_cstd_set_event(USB_STS_DEFAULT, &ctrl);
#endif

} /* End of function usb_pstd_devdefault() */

/******************************************************************************
 Function Name   : usb_pstd_pipe_info
 Description     : Pipe Information check and EP Table Set
 Arguments       : uint8_t *p_table  : Check Start Descriptor address
                 : uint16_t length  : Configuration Descriptor Length
 Return value    : uint16_t : USB_OK / USB_ERROR
 ******************************************************************************/
uint16_t usb_pstd_pipe_info (uint8_t *p_table, uint16_t length)
{
    uint16_t ofdsc;
    uint16_t retval = USB_ERROR;
    uint8_t         pipe_no;

    /* Check Endpoint Descriptor */
    ofdsc = p_table[0];

    /* WAIT_LOOP */
    while (ofdsc < length)
    {
        /* Endpoint Descriptor */
        if (USB_DT_ENDPOINT == p_table[ofdsc + USB_EP_B_DESCRIPTORTYPE])
        {
            /* EP Table pipe Information set */
            pipe_no = usb_pstd_set_pipe_table (&p_table[ofdsc]);
            if (USB_NULL != pipe_no)
            {
                retval = USB_OK;
            }
        }
        ofdsc += p_table[ofdsc];
    }
    return retval;
} /* End of function usb_pstd_pipe_info() */

/******************************************************************************
 Function Name   : usb_pstd_configured
 Description     : Peripheral Devices Class open function
 Arguments       : usb_putr_t    *p_utr        : Not used
                 : uint16_t     data1       : Not used
                 : uint16_t     data2       : Not used
 Return value    : none
 ******************************************************************************/
void usb_pstd_configured (usb_putr_t *p_utr, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    g_usb_pstd_is_connected = USB_TRUE;

    usb_cstd_set_event(USB_STS_CONFIGURED, &ctrl);

#if defined(USB_CFG_PMSC_USE)
    usb_pmsc_receive_cbw();
#endif
} /* End of function usb_pstd_configured() */

/******************************************************************************
 Function Name   : usb_pstd_detach
 Description     : Peripheral Devices Class close function
 Arguments       : usb_putr_t    *p_utr        : Not used
                 : uint16_t     data1       : Not used
                 : uint16_t     data2       : Not used
 Return value    : none
 ******************************************************************************/
void usb_pstd_detach (usb_putr_t *p_utr, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    if (USB_TRUE == g_usb_pstd_is_connected)
    {
        g_usb_pstd_is_connected = USB_FALSE;
        usb_cstd_set_event(USB_STS_DETACH, &ctrl);
    }
} /* End of function usb_pstd_detach() */

/******************************************************************************
 Function Name   : usb_pstd_suspended
 Description     : Peripheral Devices Class suspended function
 Arguments       : usb_putr_t    *p_utr        : Not used
                 : uint16_t     data1       : Not used
                 : uint16_t     data2       : Not used
 Return value    : none
 ******************************************************************************/
void usb_pstd_suspended(usb_putr_t *p_utr, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;
    usb_cstd_set_event(USB_STS_SUSPEND, &ctrl);
} /* End of function usb_pstd_suspended() */

/******************************************************************************
 Function Name   : usb_pstd_resume
 Description     : Peripheral Devices Class resume function
 Arguments       : usb_putr_t    *p_utr        : Not used
                 : uint16_t     data1       : Not used
                 : uint16_t     data2       : Not used
 Return value    : none
 ******************************************************************************/
void usb_pstd_resume(usb_putr_t *p_utr, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;
    
    usb_cstd_set_event(USB_STS_RESUME, &ctrl);
} /* End of function usb_pstd_resume() */

/******************************************************************************
 Function Name   : usb_pstd_interface
 Description     : Peripheral Devices Class set interface function
 Arguments       : usb_putr_t    *p_utr        : Not used
                 : uint16_t     data1       : Not used
                 : uint16_t     data2       : Not used
 Return value    : none
 ******************************************************************************/
void usb_pstd_interface(usb_putr_t *p_utr, uint16_t data1, uint16_t data2)
{
#if defined(USB_CFG_PMSC_USE)
    usb_pmsc_receive_cbw();
#else   /* defined(USB_CFG_PMSC_USE) */
    /* Non processing */
#endif  /* defined(USB_CFG_PMSC_USE) */
} /* End of function usb_pstd_interface() */

#if defined(USB_CFG_PVND_USE)
/******************************************************************************
 Function Name   : usb_pstd_read_complete
 Description     : CallBack Function
 Argument        : usb_putr_t *mess           : Transfer result message
 Return          : none
 ******************************************************************************/
void usb_pstd_read_complete (usb_putr_t *p_mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    /* Set Receive data length */
    ctrl.size = g_usb_read_request_size[p_mess->keyword] - p_mess->tranlen;
    ctrl.pipe = p_mess->keyword; /* Pipe number setting */
    ctrl.type = USB_PVND; /* Device class setting  */
    switch (p_mess->status)
    {
        case USB_DATA_OK :
            ctrl.status = USB_SUCCESS;
        break;
        case USB_DATA_SHT :
            ctrl.status = USB_ERR_SHORT;
        break;
        case USB_DATA_OVR :
            ctrl.status = USB_ERR_OVER;
        break;
        case USB_DATA_ERR :
        default :
            ctrl.status = USB_ERR_NG;
        break;
    }
    usb_cstd_set_event(USB_STS_READ_COMPLETE, &ctrl);
} /* End of function usb_pstd_read_complete() */

/******************************************************************************
 Function Name   : usb_pstd_write_complete
 Description     : CallBack Function
 Argument        : usb_putr_t *mess           : Transfer result message
 Return          : none
 ******************************************************************************/
void usb_pstd_write_complete (usb_putr_t *p_mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.pipe = p_mess->keyword; /* Pipe number setting */
    ctrl.type = USB_PVND; /* CDC Control class  */
    if (USB_DATA_NONE == p_mess->status)
    {
        ctrl.status = USB_SUCCESS;
    }
    else
    {
        ctrl.status = USB_ERR_NG;
    }
    usb_cstd_set_event(USB_STS_WRITE_COMPLETE, &ctrl);
} /* End of function usb_pstd_write_complete() */
#endif /* defined(USB_CFG_PVND_USE) */


#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/******************************************************************************
 End  Of File
 ******************************************************************************/
