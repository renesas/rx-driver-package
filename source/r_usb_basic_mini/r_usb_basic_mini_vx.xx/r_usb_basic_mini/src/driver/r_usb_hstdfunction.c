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
* File Name    : r_usb_hstdfunction.c
* Description  : This is the USB basic firmware library function code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 29.12.2015 1.02    Minor Update.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*           : 30.06.2019 1.12    RX23W is added.
*******************************************************************************/


/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_reg_access.h"            /* Definition of the USB register access macro */
#include "r_usb_extern.h"

#if defined(USB_CFG_HCDC_USE)
#include "r_usb_hcdc.h"
#endif /* defined(USB_CFG_HCDC_USE) */

#if defined(USB_CFG_HHID_USE)
#include "r_usb_hhid_mini_if.h"
#endif /* defined(USB_CFG_HHID_USE) */

#if defined(USB_CFG_HMSC_USE)
#include "r_usb_hmsc_mini_if.h"
#include "r_usb_hmsc.h"
#endif /* defined(USB_CFG_HMSC_USE) */

#include "r_usb_dmac.h"

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

#define CLSDATASIZE     (512)
#define USB_PET_VID     (0x1a0a)    /* USB-PET Vendor ID  */
#define USB_PET_PID     (0x0200)    /* USB-PET Product ID (Use Embedded Host Test) */


/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/* variables */
USB_STATIC  usb_hcdrequest_t    gs_usb_hstd_mgr_request;
USB_STATIC  uint8_t             gs_usb_hstd_mgr_dummy_data[CLSDATASIZE];
USB_STATIC  uint8_t             gs_usb_hstd_device_descriptor[USB_DEVICESIZE];
USB_STATIC  uint8_t             gs_usb_hstd_string_descriptor[USB_STRING_SIZE];
USB_STATIC  uint8_t             *gsp_usb_hstd_device_desc_tbl;
USB_STATIC  uint8_t             *gsp_usb_hstd_config_desc_tbl;
USB_STATIC  usb_hutr_t          gs_usb_hstd_string_control;        /* Request for HID to USB Request */

USB_STATIC  const uint16_t gs_usb_device_tpl[] =
{
    /* Number of tpl table */
    USB_CFG_TPLCNT,
    /* Reserved */
    0,
    /* Vendor ID, Product ID : no-check */
    USB_CFG_TPL,
};


/* functions */
/* HCD task / MGR task */
USB_STATIC  usb_er_t            usb_hstd_set_asddress(void);
USB_STATIC  void                usb_hstd_dummy_request(void);
/* USB Host control driver libraries */
USB_STATIC  uint16_t            usb_hstd_get_devaddx_register(void);
/* standard request functions */
USB_STATIC  usb_er_t            usb_hstd_do_command(uint8_t* tranadr);
/* HCD to MGR, APL(class driver) to MGR, MGR to HCD, HCD to MGR command control function */
USB_STATIC  void                usb_hstd_transfer_result(usb_hutr_t *utrmsg);
USB_STATIC  void                usb_hstd_setup_clear_status(void);
USB_STATIC  void                usb_hstd_setup_enable(void);
USB_STATIC  uint8_t             usb_cstd_buf_to_cfifo(uint16_t useport);
USB_STATIC  uint8_t             usb_hstd_cfifo_to_buf(void);

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
USB_STATIC uint8_t usb_hstd_get_ifclass( uint8_t class );
#endif

USB_STATIC void usb_hstd_get_string_desc(usb_addr_t addr, uint8_t string, usb_hcb_t complete);
USB_STATIC void usb_hstd_specified_path(void);
USB_STATIC uint8_t usb_hstd_check_open_class (uint8_t class);

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
/* variables */
/*Interrupt global variables */
usb_intinfo_t   g_usb_hstd_int_msg[USB_INTMSGMAX];         /* Interrupt message */
uint8_t         g_usb_hstd_int_msg_cnt;                         /* Interrupt message count */
usb_hutr_t      *gp_usb_hstd_pipe[USB_MAX_PIPE_NO + 1u];      /* Message pipe */
uint8_t         *gp_usb_hstd_data_ptr[USB_MAX_PIPE_NO + 1u];    /* PIPEn Buffer pointer(8bit) */
usb_leng_t      g_usb_hstd_data_cnt[USB_MAX_PIPE_NO + 1u];     /* PIPEn Buffer counter */
usb_leng_t      g_usb_hstd_data_cnt_pipe0;
usb_pipe_no_t   g_usb_hstd_current_pipe;                          /* Pipe number */
/* Scheduler global variables */
usb_tskinfo_t   g_usb_hstd_block[USB_BLKMAX];
usb_msg_t       *g_usb_hstd_msg_tbl_sch[USB_IDMAX][USB_TABLEMAX];
uint8_t         g_usb_hstd_read_sch[USB_IDMAX];
uint8_t         g_usb_hstd_write_sch[USB_IDMAX];
/* Scheduler wait message */
usb_waitinfo_t  g_usb_hstd_wait_msg_sch[USB_IDMAX];
usb_waitque_t   g_usb_hstd_wait_que_sch[USB_TABLEMAX];           /* Queuing as for some messages. */
/* Scheduler global flags */
usb_msg_t       *gp_usb_hstd_next_addr_sch;
uint8_t         g_usb_hstd_next_id_sch;        /* Priority Counter */
uint8_t         g_usb_next_read_ptr_sch;       /* Priority Table Reading pointer */
uint8_t         *gp_usb_hstd_interface_tbl;

/******************************************************************************
Function Name   : usb_cstd_init_library
Description     : Initialized global variables
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_init_library(void)
{
    uint8_t i;

    /* Initialize priority table pointer and priority table */
    /* WAIT_LOOP */
    for (i = 0; USB_IDMAX != i; i++)
    {
        g_usb_hstd_read_sch[i]    = 0;
        g_usb_hstd_write_sch[i]   = 0;
        g_usb_hstd_wait_msg_sch[i].counter = 0;
    }
    /* Initialize wait table */
    /* WAIT_LOOP */
    for (i = 0; USB_TABLEMAX != i;i ++)
    {
        g_usb_hstd_wait_que_sch[i].task_id    = USB_IDCLR;
    }

    /* Initialize block table */
    /* WAIT_LOOP */
    for (i = 0; USB_BLKMAX != i; i++)
    {
        g_usb_hstd_block[i].flag  = USB_IDCLR;
    }

    /* Initialize pipe transfer data */
    /* WAIT_LOOP */
    for (i = USB_PIPE0; i <= USB_MAX_PIPE_NO; i++ )
    {
        gp_usb_hstd_pipe[i]        = (usb_hutr_t*)USB_NULL;
    }

    /* Initialize interrupt message */
    /* WAIT_LOOP */
    for (i = 0; i < USB_INTMSGMAX; i++ )
    {
        g_usb_hstd_int_msg[i].msginfo = USB_INTERRUPT;
    }

    g_usb_hstd_int_msg_cnt  = 0;
    g_usb_hstd_current_pipe   = USB_PIPE0;
}   /* End of function usb_cstd_init_library() */


/******************************************************************************
Function Name   : usb_hstd_buf_to_fifo
Description     : Buffer to FIFO data write
Arguments       : uint16_t useport       : FIFO Access mode
Return value    : none
******************************************************************************/
void usb_hstd_buf_to_fifo(uint16_t useport)
{
    uint8_t result;

    /* Disable Ready Interrupt */
    hw_usb_clear_brdyenb(g_usb_hstd_current_pipe);

    result = usb_cstd_buf_to_cfifo(useport);

    /* Buffer to CFIFO write */
    switch (result)
    {
        case USB_WRITING:           /* Continue of data write */
            /* Enable BRDY/NRDY Interrupt */
            hw_usb_set_brdyenb(g_usb_hstd_current_pipe);
            usb_cstd_nrdy_enable(g_usb_hstd_current_pipe);
        break;
        case USB_WRITEEND:          /* End of data write */
        /* continue */
        case USB_WRITESHRT:         /* End of data write */
            /* Enable BEMP/NRDY Interrupt */
            hw_usb_set_bempenb(g_usb_hstd_current_pipe);
            usb_cstd_nrdy_enable(g_usb_hstd_current_pipe);
        break;
        case USB_FIFOERROR:         /* FIFO access error */
        /* continue */
        default:
            usb_hstd_forced_termination(USB_DATA_ERR);
        break;
    }

}   /* End of function usb_hstd_buf_to_fifo() */


/******************************************************************************
Function Name   : usb_cstd_buf_to_cfifo
Description     : Buffer to CFIFO data write
Arguments       : uint16_t useport       : FIFO Access mode
Return value    : uint16_t end_flag
******************************************************************************/
USB_STATIC uint8_t usb_cstd_buf_to_cfifo(uint16_t useport)
{
    uint16_t       count;
    uint16_t       even;
    uint16_t       buffer;
    uint16_t       mxps;
    uint8_t        end_flag;
    usb_pipe_no_t  pipe;
    uint16_t       *p_fifo_port_16bit;
    uint8_t        *p_fifo_port_8bit;

    pipe    = g_usb_hstd_current_pipe;

    switch (useport)
    {
        case USB_D0USE:
            p_fifo_port_16bit = (uint16_t*)&USB0.D0FIFO.WORD;
            p_fifo_port_8bit  = (uint8_t*)&USB0.D0FIFO.BYTE.L;
        break;

        case USB_D1USE:
            p_fifo_port_16bit = (uint16_t*)&USB0.D1FIFO.WORD;
            p_fifo_port_8bit  = (uint8_t*)&USB0.D1FIFO.BYTE.L;
        break;

        case USB_CUSE:
        default:
            /* Changes FIFO port by the pipe. */
            if (USB_PIPE0 == pipe)
            {
                buffer = usb_cstd_is_set_frdy(g_usb_hstd_current_pipe, USB_CUSE, USB_ISEL);
            }
            else
            {
                buffer = usb_cstd_is_set_frdy(g_usb_hstd_current_pipe, USB_CUSE, USB_NULL);
            }
            /* Check error */
            if (USB_FIFOERROR == buffer)
            {
                return (USB_FIFOERROR);
            }
            p_fifo_port_16bit = (uint16_t*)&USB0.CFIFO.WORD;
            p_fifo_port_8bit  = (uint8_t*)&USB0.CFIFO.BYTE.L;
        break;
    }

    mxps = usb_cstd_get_maxpacket_size(pipe);

    /* Write continues */
    end_flag    = USB_WRITING;
    count       = mxps;

    /* Data size check */
    if (g_usb_hstd_data_cnt[pipe] <= (usb_leng_t)mxps)
    {
        count       = (uint16_t)g_usb_hstd_data_cnt[pipe];
        end_flag    = USB_WRITEEND;
        if ((0 == count) || (count != mxps))
        {
            /* Null Packet is end of write */
            /* Short Packet is end of write */
            end_flag    = USB_WRITESHRT;
        }
    }

    /* FIFO write */
    /* WAIT_LOOP */
    for (even = count/2; (0 != even); --even )
    {
        /* 16bit access */
        *p_fifo_port_16bit = *((uint16_t*)gp_usb_hstd_data_ptr[pipe]);
        gp_usb_hstd_data_ptr[pipe] += 2;
    }
    if ((count % 2) == 1)
    {
        /* odd number data access */
        hw_usb_set_mbw(useport, USB_MBW_8);
        *p_fifo_port_8bit = *((uint8_t*)gp_usb_hstd_data_ptr[pipe]);
        gp_usb_hstd_data_ptr[pipe]++;
        hw_usb_set_mbw(useport, USB_MBW_16);
    }

    /* Check data count to remain */
    if (g_usb_hstd_data_cnt[pipe] < (usb_leng_t)mxps)
    {
        /* Short Packet */
        g_usb_hstd_data_cnt[pipe] = 0;
        hw_usb_set_bval(useport);
    }
    else
    {
        /* Total data count - count */
        g_usb_hstd_data_cnt[pipe] -= count;
    }
    return end_flag;

}   /* End of function usb_cstd_buf_to_cfifo() */

/******************************************************************************
Function Name   : usb_hstd_fifo_to_buf
Description     : FIFO to Buffer data read
                : CFIFO     : Operation on the conditions of CNTMD=0 is possible
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_fifo_to_buf(void)
{
    /* CFIFO to buffer read */
    switch (usb_hstd_cfifo_to_buf())
    {
        case USB_READING:
            /* Continue of data read (no processing) */
        break;
        case USB_READEND:
            /* End of data read */
            usb_hstd_data_end(USB_DATA_OK);
        break;
        case USB_READSHRT:
            /* End of data read */
            usb_hstd_data_end(USB_DATA_SHT);
        break;
        case USB_READOVER:
            /* Buffer over */
            usb_hstd_forced_termination(USB_DATA_OVR);
        break;
        case USB_FIFOERROR:
        /* continue */
        default:
            usb_hstd_forced_termination(USB_DATA_ERR);
        break;
    }
}   /* End of function usb_hstd_fifo_to_buf() */


/******************************************************************************
Function Name   : usb_hstd_cfifo_to_buf
Description     : CFIFO to Buffer data read
                : Operation on the conditions of DBLB=0/CNTMD=0 is possible.
Arguments       : none
Return value    : uint16_t end_flag
******************************************************************************/
USB_STATIC uint8_t usb_hstd_cfifo_to_buf(void)
{
    uint16_t       count;
    uint16_t       even;
    uint16_t       buffer;
    uint16_t       mxps;
    uint16_t       dtln;
    uint8_t        end_flag;
    usb_pipe_no_t  pipe;

    pipe    = g_usb_hstd_current_pipe;

    /* Changes FIFO port by the pipe. */
    buffer = usb_cstd_is_set_frdy(pipe, USB_CUSE, USB_NULL);
    if (USB_FIFOERROR == buffer)
    {
        /* FIFO access error */
        return (USB_FIFOERROR);
    }

    dtln = (uint16_t)(buffer & USB_DTLN);
    /* Max Packet Size */
    mxps = usb_cstd_get_maxpacket_size(pipe);

    if (g_usb_hstd_data_cnt[pipe] < dtln )                 /* Buffer Over ? */
    {
        end_flag = USB_READOVER;
        count = (uint16_t)g_usb_hstd_data_cnt[pipe];
        g_usb_hstd_data_cnt[pipe] = dtln;
    }
    else if (g_usb_hstd_data_cnt[pipe] == dtln )           /* Just Receive Size */
    {
        count = dtln;
        end_flag = USB_READEND;
    }
    else                                                /* Continues Receive data */
    {
        count = dtln;
        end_flag = USB_READING;
        if ( (0 == count) || (count != mxps) )
        {
            /* Null Packet is end of read */
            /* Short Packet is end of read */
            end_flag = USB_READSHRT;
        }
    }
    if (USB_READING != end_flag)
    {
        usb_cstd_set_nak(g_usb_hstd_current_pipe);
    }

    if (0 == dtln)
    {
        /* 0 length packet */
        hw_usb_set_bclr(USB_CUSE);
    }
    else
    {
        /* WAIT_LOOP */
        for (even = count/2; (0 != even); --even )
        {
            buffer = USB0.CFIFO.WORD;
            *(uint16_t*)gp_usb_hstd_data_ptr[pipe]  =(uint16_t)buffer;
            gp_usb_hstd_data_ptr[pipe] += 2;
        }

        if (1 == (count % 2))
        {
            buffer = USB0.CFIFO.WORD;
#if USB_CFG_ENDIAN == USB_CFG_BIG
            buffer >>= 8;
#else /* USB_CFG_ENDIAN == USB_CFG_BIG */
            buffer &= 0xFF;
#endif /* USB_CFG_ENDIAN == USB_CFG_BIG */
            *gp_usb_hstd_data_ptr[pipe] =(uint8_t)buffer;
            gp_usb_hstd_data_ptr[pipe]++;
        }
    }

    g_usb_hstd_data_cnt[pipe] -= count;
    return (end_flag);
}   /* End of function usb_hstd_cfifo_to_buf() */


/******************************************************************************
Function Name   : usb_hstd_devaddr_to_port
Description     : Get device port from device address
Arguments       : none
Return value    : usb_port_t                  : root port
Note            : Use also to a connection check is possible
******************************************************************************/
usb_port_t usb_hstd_devaddr_to_port(void)
{
    usb_port_t  port;

    port    = (usb_port_t)(g_usb_mgr_devaddr - USB_DEVICEADDR);
    if (0 == port)
    {
        return port;
    }
    return USB_NODEVICE;
}   /* End of function usb_hstd_devaddr_to_port() */

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
/******************************************************************************
Function Name   : usb_hstd_get_ifclass
Description     : 
Arguments       : none
Return value    : uint8_t                  : class
Note            : 
******************************************************************************/
USB_STATIC uint8_t usb_hstd_get_ifclass( uint8_t class )
{
    uint16_t    len;
    uint16_t    index;
    
    len = g_usb_hstd_cfg_descriptor[2] | ( g_usb_hstd_cfg_descriptor[3] << 8 );
    index = g_usb_hstd_cfg_descriptor[0];
    /* WAIT_LOOP */
    while (index < len )
    {
        if (0 == g_usb_hstd_cfg_descriptor[index])
        {
            return USB_NO;
        }

        if (USB_DT_INTERFACE == g_usb_hstd_cfg_descriptor[index+1])
        {
            if (g_usb_hstd_cfg_descriptor[index+5] == class )
            {
                return USB_YES;
            }
        }
        index += g_usb_hstd_cfg_descriptor[index];
    }
    return USB_NO;
} /* End of function usb_hstd_get_ifclass() */
#endif /* end USB_CFG_COMPLIANCE == USB_CFG_ENABLE */


/******************************************************************************
Function Name   : usb_hstd_enumeration
Description     : USB Enumeration
Argument        : none
Return          : uint16_t                  : enumeration status
******************************************************************************/
uint8_t usb_hstd_enumeration(void)
{
    usb_hcdreg_t    *p_driver;
    uint8_t         *p_table[3];
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    uint16_t vendor_id;
    uint16_t product_id;
    usb_compliance_t disp_param;
    uint8_t         ifclass;
#endif /* end USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    switch (g_usb_mgr_seq)
    {
        case USB_2:                 /* Receive Device Descriptor */
            /* Set device speed */
            g_usb_hstd_hcd_port    = g_usb_mgr_port;
            g_usb_hcd_devaddr      = g_usb_mgr_devaddr;
            usb_hstd_set_devaddx_register(g_usb_hstd_devspd);
            g_usb_hstd_hcd_dcp_mxps[g_usb_mgr_devaddr]
                = (uint16_t)((uint16_t)(gs_usb_hstd_device_descriptor[USB_DEV_B_MAX_PACKET_SIZE_0] & USB_MAXPFIELD) |
                             (USB_ADDR2DEVSEL(g_usb_mgr_devaddr)));
            usb_hstd_set_asddress();
            g_usb_hcd_dev_info.state    = USB_STS_ADDRESS;
        break;
        case USB_3:             /* Set Address */
        /* continue */
        case USB_4:             /* Receive Device Descriptor(18) */
        /* continue */
        case USB_5:             /* Receive Configuration Descriptor(9) */
            if (usb_hstd_get_descriptor(g_usb_mgr_seq) != USB_OK)
            {
                return (USB_NONDEVICE);
            }
        break;
        case USB_6:             /* Receive Configuration Descriptor(xx) */
            /* Device enumeration function */
            p_driver = &g_usb_hcd_driver;

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            vendor_id = (uint16_t)(gs_usb_hstd_device_descriptor[USB_DEV_ID_VENDOR_L]
                    + ((uint16_t)gs_usb_hstd_device_descriptor[USB_DEV_ID_VENDOR_H] << 8));
            product_id = (uint16_t)(gs_usb_hstd_device_descriptor[USB_DEV_ID_PRODUCT_L]
                    + ((uint16_t)gs_usb_hstd_device_descriptor[USB_DEV_ID_PRODUCT_H] << 8));

            /* Check PET connect */
            if ((USB_PET_VID == vendor_id) && (USB_PET_PID == product_id))
            {
                usb_hstd_set_configuration();
                g_usb_mgr_seq = USB_7;
                return (USB_DEVICEENUMERATION);
            }
            else
            {
                ifclass = usb_hstd_get_ifclass( p_driver->ifclass );
            }

            if ((USB_STS_DETACH == p_driver->devstate) && ( USB_YES == ifclass ) )
            {
#else /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
            if (USB_STS_DETACH == p_driver->devstate)
            {
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
                /* To judge the detach while the class driver is operating, the pre-register is executed. */
                p_driver->rootport    = g_usb_mgr_port;
                p_driver->devaddr     = g_usb_mgr_devaddr;
                p_table[0]    = gs_usb_hstd_device_descriptor;
                p_table[1]    = g_usb_hstd_cfg_descriptor;
                p_table[2]    = (uint8_t*)&g_usb_mgr_devaddr;

                g_usb_hcd_dev_info.port     = g_usb_mgr_port;
                g_usb_hcd_dev_info.speed    = g_usb_hstd_devspd;

                (*p_driver->classcheck)(p_table);
                return (USB_DEVICEENUMERATION);
                /* In this function, check device class of enumeration flow move to class */
                /* "usb_hstd_return_enu_mgr()" is used to return */
            }
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            ifclass = usb_hstd_get_ifclass( USB_IFCLS_HUB );
            if (USB_YES == ifclass )
            {
                disp_param.status = USB_CT_HUB;
                disp_param.pid    = product_id;
                disp_param.vid    = vendor_id;
            }
            else
            {
                disp_param.status = USB_CT_NOTTPL;
                disp_param.pid    = product_id;
                disp_param.vid    = vendor_id;
            }
            usb_compliance_disp ((void *)&disp_param);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
            usb_hstd_return_enu_mgr(USB_ERROR);
            usb_hstd_connect_err_event_set();           /* Set event USB_STS_NOT_SUPPORT */
        break;
        case USB_8:             /* Set Configuration */
            /* Device enumeration function */
            p_driver = &g_usb_hcd_driver;
            if (g_usb_mgr_devaddr == p_driver->devaddr )
            {
                g_usb_hcd_dev_info.port     = g_usb_mgr_port;
                g_usb_hcd_dev_info.state    = USB_STS_CONFIGURED;
                g_usb_hcd_dev_info.config   = g_usb_hstd_cfg_descriptor[USB_DEV_B_CONFIGURATION_VALUE];
                g_usb_hcd_dev_info.speed    = g_usb_hstd_devspd;
                /* Device state */
                p_driver->devstate                        = USB_STS_CONFIGURED;
                (*p_driver->statediagram)(p_driver->devaddr, USB_STS_CONFIGURED);
                return (USB_COMPLETEPIPESET);
            }
            return (USB_COMPLETEPIPESET);
        break;
        default:
            usb_hstd_dummy_request();
        break;
    }
    return (USB_DEVICEENUMERATION);
}   /* End of function usb_hstd_enumeration() */


/******************************************************************************
Function Name   : usb_hstd_get_descriptor
Description     : Set GetDescriptor
Argument        : uint16_t cnt_value        : Enumeration sequence
Return          : usb_er_t
******************************************************************************/
usb_er_t usb_hstd_get_descriptor(uint16_t cnt_value)
{
    uint8_t     *p_data_table;

    gs_usb_hstd_mgr_request.word.byte.bm_request_type  = USB_REQUEST_TYPE(USB_DEV_TO_HOST,USB_STANDARD,USB_DEVICE);
    gs_usb_hstd_mgr_request.word.byte.b_request        = USB_REQUEST_GET_DESCRIPTOR;
    gs_usb_hstd_mgr_request.address                    = (uint16_t)g_usb_mgr_devaddr;
    switch (cnt_value)
    {
        case 0:
            gs_usb_hstd_mgr_request.address        = (uint16_t)0;
        /* continue */
        case 1:
        /* continue */
        case 2:
        /* continue */
        case 3:
        /* continue */
        case 6:
            gs_usb_hstd_mgr_request.w_value     = USB_DEV_DESCRIPTOR;
            gs_usb_hstd_mgr_request.w_index     = 0x0000;
            gs_usb_hstd_mgr_request.w_length    = USB_DEVICESIZE;
            p_data_table                  = (uint8_t*)&gs_usb_hstd_device_descriptor;
        break;
        case 4:
            gs_usb_hstd_mgr_request.w_value     = USB_CONF_DESCRIPTOR;
            gs_usb_hstd_mgr_request.w_index     = 0x0000;
            gs_usb_hstd_mgr_request.w_length    = 0x0009;
            p_data_table                  = (uint8_t*)&g_usb_hstd_cfg_descriptor;
        break;
        case 5:
            gs_usb_hstd_mgr_request.w_value     = USB_CONF_DESCRIPTOR;
            gs_usb_hstd_mgr_request.w_index     = 0x0000;
            gs_usb_hstd_mgr_request.w_length  
                                    = (uint16_t)(((uint16_t)g_usb_hstd_cfg_descriptor[USB_DEV_W_TOTAL_LENGTH_H] << 8)
                                       + (uint16_t)g_usb_hstd_cfg_descriptor[USB_DEV_W_TOTAL_LENGTH_L]);
            if (gs_usb_hstd_mgr_request.w_length > USB_CONFIGSIZE )
            {
                gs_usb_hstd_mgr_request.w_length = USB_CONFIGSIZE;
            }
            p_data_table                  = (uint8_t*)&g_usb_hstd_cfg_descriptor;
        break;
        default:
            return USB_ERROR;
        break;
    }

    return usb_hstd_do_command(p_data_table);
}   /* End of function usb_hstd_get_descriptor() */

/******************************************************************************
Function Name   : usb_hstd_set_asddress
Description     : Set SetAddress
Argument        : none
Return          : usb_er_t
******************************************************************************/
USB_STATIC usb_er_t usb_hstd_set_asddress(void)
{
    gs_usb_hstd_mgr_request.word.byte.bm_request_type  = USB_REQUEST_TYPE(USB_HOST_TO_DEV,USB_STANDARD,USB_DEVICE);
    gs_usb_hstd_mgr_request.word.byte.b_request        = USB_REQUEST_SET_ADDRESS;
    gs_usb_hstd_mgr_request.w_value                    = (uint16_t)g_usb_mgr_devaddr;
    gs_usb_hstd_mgr_request.w_index                    = 0x0000;
    gs_usb_hstd_mgr_request.w_length                   = 0x0000;
    gs_usb_hstd_mgr_request.address                    = 0;
    return usb_hstd_do_command((uint8_t*)&gs_usb_hstd_mgr_dummy_data);
}   /* End of function usb_hstd_set_asddress() */

/******************************************************************************
Function Name   : usb_hstd_set_configuration
Description     : Set SetConfiguration
Argument        : none
Return          : usb_er_t
******************************************************************************/
usb_er_t usb_hstd_set_configuration(void)
{
    gs_usb_hstd_mgr_request.word.byte.bm_request_type  = USB_REQUEST_TYPE(USB_HOST_TO_DEV,USB_STANDARD,USB_DEVICE);
    gs_usb_hstd_mgr_request.word.byte.b_request        = USB_REQUEST_SET_CONFIGURATION;
    gs_usb_hstd_mgr_request.w_value         = (uint16_t)(g_usb_hstd_cfg_descriptor[USB_DEV_B_CONFIGURATION_VALUE]);
    gs_usb_hstd_mgr_request.w_index                    = 0x0000;
    gs_usb_hstd_mgr_request.w_length                   = 0x0000;
    gs_usb_hstd_mgr_request.address                    = (uint16_t)g_usb_mgr_devaddr;
    return usb_hstd_do_command((uint8_t*)&gs_usb_hstd_mgr_dummy_data);
}   /* End of function usb_hstd_set_configuration() */

/******************************************************************************
Function Name   : usb_hstd_dummy_request
Description     : Set Dummy Request
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_dummy_request(void)
{
}   /* End of function usb_hstd_dummy_request() */


/******************************************************************************
Function Name   : usb_hstd_clr_device_info
Description     : Device information clear
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_clr_device_info(void)
{
    g_usb_hcd_dev_info.port     = USB_NOPORT;
    g_usb_hcd_dev_info.state    = USB_STS_DETACH;
    g_usb_hcd_dev_info.config   = 0;
    g_usb_hcd_dev_info.speed    = USB_NOCONNECT;
}   /* End of function usb_hstd_clr_device_info() */


/******************************************************************************
Function Name   : usb_hstd_update_devinfo
Description     : 
Argument        : state
Return          : none
******************************************************************************/
void usb_hstd_update_devinfo(uint8_t state)
{
    usb_hstd_update_devstate(state);
    g_usb_hcd_dev_info.state    = state;
}   /* End of function usb_hstd_update_devinfo() */


/******************************************************************************
Function Name   : usb_hstd_update_devstate
Description     : 
Argument        : state
Return          : none
******************************************************************************/
/* The state of the device of the registered driver is updated.  */
void usb_hstd_update_devstate(uint8_t state)
{
    usb_hcdreg_t    *p_driver;

    p_driver  = &g_usb_hcd_driver;
    if (g_usb_mgr_devaddr == p_driver->devaddr)
    {
        p_driver->devstate    = state;
    }
}   /* End of function usb_hstd_update_devstate() */


/******************************************************************************
Function Name   : usb_hstd_control_write_start
Description     : Control write start
Argument        : none
Return          : uint16_t                  : USB_WRITEEND / USB_WRITING
                :                           : USB_WRITESHRT / USB_FIFOERROR
******************************************************************************/
uint16_t usb_hstd_control_write_start(void)
{
    uint16_t    end_flag;

    /* PID=NAK & clear STALL */
    g_usb_hstd_current_pipe = USB_PIPE0;
    usb_cstd_clr_stall(g_usb_hstd_current_pipe);

    /* DCP Configuration Register  (0x5C) */
    hw_usb_write_dcpcfg(USB_DIRFIELD);
    hw_usb_set_sqset(USB_PIPE0);

    hw_usb_clear_status_bemp(USB_PIPE0);

    /* Host Control sequence */
    end_flag = usb_cstd_buf_to_cfifo(USB_CUSE);

    switch (end_flag)
    {
        case USB_WRITEEND:              /* End of data write (not null) */
        case USB_WRITESHRT:             /* End of data write */
            /* Next stage is Control write status stage */
            g_usb_hcd_ctsq = USB_STATUSWR;
            hw_usb_set_bempenb(g_usb_hstd_current_pipe);
            usb_cstd_nrdy_enable(g_usb_hstd_current_pipe);
            usb_cstd_set_buf(g_usb_hstd_current_pipe);
        break;
        case USB_WRITING:               /* Continuance of data write */
            /* Next stage is Control write data stage */
            g_usb_hcd_ctsq = USB_DATAWR;
            hw_usb_set_bempenb(g_usb_hstd_current_pipe);
            usb_cstd_nrdy_enable(g_usb_hstd_current_pipe);
            usb_cstd_set_buf(g_usb_hstd_current_pipe);
        break;
        /* FIFO access error */
        case USB_FIFOERROR:
        /* continue */
        default:
            /* no processing */
        break;
    }
    return (end_flag);
}   /* End of function usb_hstd_control_write_start() */

/******************************************************************************
Function Name   : usb_hstd_control_read_start
Description     : Control read start
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_control_read_start(void)
{
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    g_usb_hstd_response_counter = 0;
    hw_usb_clear_sts_sofr();
    hw_usb_set_intenb(USB_SOFE);
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    /* PID=NAK & clear STALL */
    g_usb_hstd_current_pipe = USB_PIPE0;
    usb_cstd_clr_stall(g_usb_hstd_current_pipe);
    hw_usb_write_dcpcfg(USB_SHTNAKFIELD);
    hw_usb_hwrite_dcpctr(USB_SQSET); /* SQSET=1, PID=NAK */

    /* Next stage is Control read data stage */
    g_usb_hcd_ctsq = USB_DATARD;

    /* Interrupt enable */
    hw_usb_set_brdyenb(g_usb_hstd_current_pipe);
    usb_cstd_nrdy_enable(g_usb_hstd_current_pipe);
    usb_cstd_set_buf(g_usb_hstd_current_pipe);
}   /* End of function usb_hstd_control_read_start() */


/******************************************************************************
Function Name   : usb_hstd_status_start
Description     : Control Read/Write Status
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_status_start(void)
{
    uint16_t    end_flag;

    /* Interrupt Disable */
    g_usb_hstd_current_pipe = USB_PIPE0;
    hw_usb_clear_bempenb(g_usb_hstd_current_pipe);
    hw_usb_clear_brdyenb(g_usb_hstd_current_pipe);
    gp_usb_hstd_pipe[USB_PIPE0]->tranlen = g_usb_hstd_data_cnt[USB_PIPE0];
    g_usb_hstd_data_cnt_pipe0 = g_usb_hstd_data_cnt[USB_PIPE0];
    g_usb_hstd_data_cnt[USB_PIPE0] = 0;
    gp_usb_hstd_data_ptr[USB_PIPE0] = (uint8_t*)&gs_usb_hstd_mgr_dummy_data;

    /* Branch by the Control transfer stage management */
    switch (g_usb_hcd_ctsq)
    {
        case USB_DATARD:                    /* Control Read Data */
            /* Control read status, Control write start */
            g_usb_hcd_ctsq = USB_DATARD;
            end_flag = usb_hstd_control_write_start();

            if (USB_FIFOERROR == end_flag)
            {
                /* Control Read/Write End */
                usb_hstd_control_end(USB_DATA_ERR);
            }
            else
            {
                /* Next stage is Control read status stage */
                g_usb_hcd_ctsq = USB_STATUSRD;
            }
        break;
        case USB_STATUSWR:                  /* Control Write Data */
            /* continue */
        case USB_SETUPNDC:                  /* NoData Control */
            /* Next stage is Control write status stage */
            usb_hstd_control_read_start();
            g_usb_hcd_ctsq = USB_STATUSWR;
        break;
        default:
            return;
        break;
    }

}   /* End of function usb_hstd_status_start() */


/******************************************************************************
Function Name   : usb_hstd_control_end
Description     : Control Read/Write End
Argument        : usb_strct_t status           : Transfer status
Return          : none
******************************************************************************/
void usb_hstd_control_end(usb_strct_t status)
{
    usb_hutr_t       *p_utr;

    /* Evacuation pointer */
    p_utr     = gp_usb_hstd_pipe[USB_PIPE0];
    gp_usb_hstd_pipe[USB_PIPE0]    = (usb_hutr_t*)USB_NULL;

    /* Interrupt Disable */
    g_usb_hstd_current_pipe = USB_PIPE0;
    hw_usb_clear_bempenb(g_usb_hstd_current_pipe);
    hw_usb_clear_brdyenb(g_usb_hstd_current_pipe);
    hw_usb_clear_nrdyenb(g_usb_hstd_current_pipe);

    /* PID=NAK & clear STALL */
    usb_cstd_clr_stall(g_usb_hstd_current_pipe);
    hw_usb_hwrite_dcpctr((uint16_t) (USB_SUREQCLR | USB_SQCLR));

    /* CFIFO buffer clear */
    usb_cstd_chg_curpipe(USB_PIPE0, USB_CUSE, USB_NULL);

    hw_usb_set_bclr(USB_CUSE); /* Clear BVAL */
    usb_cstd_chg_curpipe(USB_PIPE0, USB_CUSE, USB_ISEL);

    hw_usb_set_bclr(USB_CUSE); /* Clear BVAL */

    /* Next stage is idle */
    g_usb_hcd_ctsq = USB_IDLEST;

    /* Call Back */
    if ((USB_NULL != p_utr) && (USB_DATA_TMO != status))
    {
        /* Transfer information set */
        p_utr->status     = (usb_strct_t)status;
        if (USB_NULL != p_utr->complete)
        {
            (*p_utr->complete)(p_utr);
        }
    }

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    hw_usb_clear_enb_sofe();
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

}   /* End of function usb_hstd_control_end() */


/******************************************************************************
Function Name   : usb_hstd_setup_start
Description     : Setup start
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_setup_start(void)
{
    uint16_t        dir;
    uint16_t        setup_req;
    uint16_t        setup_val;
    uint16_t        setup_indx;
    uint16_t        setup_leng;
    uint16_t        *p_setup;

    g_usb_hstd_current_pipe = USB_PIPE0;
    p_setup     = gp_usb_hstd_pipe[USB_PIPE0]->setup;

    /* Set Setup Request data */
    setup_req   = *p_setup++;
    setup_val   = *p_setup++;
    setup_indx  = *p_setup++;
    setup_leng  = *p_setup++;

    /* Max Packet Size + Device Number select */
    hw_usb_write_dcpmxps(g_usb_hstd_hcd_dcp_mxps[*p_setup]);

    /* Transfer Length check */

    if (gp_usb_hstd_pipe[USB_PIPE0]->tranlen < setup_leng )
    {
        setup_leng = (uint16_t)gp_usb_hstd_pipe[USB_PIPE0]->tranlen;
    }

    if (setup_leng < gp_usb_hstd_pipe[USB_PIPE0]->tranlen )
    {
        gp_usb_hstd_pipe[USB_PIPE0]->tranlen = (usb_leng_t)setup_leng;
    }

    g_usb_hstd_data_cnt[USB_PIPE0] = (usb_leng_t)gp_usb_hstd_pipe[USB_PIPE0]->tranlen;
    gp_usb_hstd_data_ptr[USB_PIPE0] = (uint8_t*)gp_usb_hstd_pipe[USB_PIPE0]->p_tranadr;


    /* Control sequence setting */
    dir = (uint16_t)(setup_req & USB_BMREQUESTTYPEDIR);
    /* Check wLength field */
    if (0 == setup_leng)
    {
        /* Check direction field */
        if (0 == dir)
        {
            /* Next stage is NoData control status stage */
            g_usb_hcd_ctsq = USB_SETUPNDC;
        }
        else
        {
            /* Error */
            g_usb_hcd_ctsq = USB_IDLEST;
        }
    }
    else
    {
        /* Check direction field */
        if (0 == dir)
        {
                /* Next stage is Control Write data stage */
                g_usb_hcd_ctsq = USB_SETUPWR;
        }
        else
        {
                /* Next stage is Control read data stage */
                g_usb_hcd_ctsq = USB_SETUPRD;
        }
    }

    /* Control transfer idle stage ? */
    if (USB_IDLEST == g_usb_hcd_ctsq)
    {
        /* Control Read/Write End */
        usb_hstd_control_end(USB_DATA_ERR);
    }
    else
    {
        /* SETUP request set */
        hw_usb_hwrite_usbreq(setup_req);
        hw_usb_hset_usbval(setup_val);
        hw_usb_hset_usbindx(setup_indx);
        hw_usb_hset_usbleng(setup_leng);
        /* Interrupt enable */
        usb_hstd_setup_enable();
    }

}   /* End of function usb_hstd_setup_start() */


/******************************************************************************
Function Name   : usb_hstd_brdy_pipe
Description     : INTR interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_brdy_pipe(void)
{
    uint16_t    bitsts;

    bitsts  = gp_usb_hstd_hcd_message->status;         /* p->bitsts; */

    /* When operating by the host function, usb_hstd_brdy_pipe_process() is executed without fail because */
    /* only one BRDY message is issued even when the demand of PIPE0 and PIPEx has been generated at the same time. */
    if ((bitsts & USB_BRDY0) == USB_BRDY0 )
    {
        /* Branch  by the Control transfer stage management */
        switch (g_usb_hcd_ctsq)
        {
            case USB_DATARD:                    /* Data stage of Control read transfer */
                g_usb_hstd_current_pipe = USB_PIPE0;
                switch (usb_hstd_cfifo_to_buf())
                {
                    case USB_READEND:           /* End of data read */
                    /* continue */
                    case USB_READSHRT:          /* End of data read */
                        usb_hstd_status_start();
                    break;
                    case USB_READING:           /* Continuance of data read */
                    break;
                    case USB_READOVER:          /* bufer over */
                        /* Control Read/Write End */
                        usb_hstd_control_end(USB_DATA_OVR);
                    break;
                    case USB_FIFOERROR:         /* FIFO access error */
                        /* Control Read/Write End */
                        usb_hstd_control_end(USB_DATA_ERR);
                    break;
                    default:
                        /* no processing */
                    break;
                }
            break;
            case USB_STATUSWR:                  /* Status stage of Control write (NoData control) transfer */
                /* Control Read/Write End */
                usb_hstd_control_end(USB_CTRL_END);
            break;
            default:
                /* no processing */
            break;
        }
    }
    usb_hstd_brdy_pipe_process(bitsts);

}   /* End of function usb_hstd_brdy_pipe() */


/******************************************************************************
Function Name   : usb_hstd_nrdy_pipe
Description     : INTN interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_nrdy_pipe(void)
{
    uint16_t        buffer;
    uint16_t    bitsts;

    bitsts  = gp_usb_hstd_hcd_message->status;         /* p->bitsts; */

    /* When operating by the host function, usb_hstd_nrdy_pipe_process() is executed without fail because */
    /* only one NRDY message is issued even when the demand of PIPE0 and PIPEx has been generated at the same time. */
    if ((bitsts & USB_NRDY0) == USB_NRDY0 )
    {
        /* Get Pipe PID from pipe number */
        g_usb_hstd_current_pipe = USB_PIPE0;
        buffer = usb_cstd_get_pid(g_usb_hstd_current_pipe);
        /* STALL ? */
        if ((buffer & USB_PID_STALL) == USB_PID_STALL )
        {
            /* PIPE0 STALL callback */
            usb_hstd_control_end(USB_DATA_STALL);
        }
        else
        {
            /* PIPE0 detach callback */
            usb_hstd_control_end(USB_DATA_DTCH);
        }
    }
    usb_hstd_nrdy_pipe_process(bitsts);

}   /* End of function usb_hstd_nrdy_pipe() */


/******************************************************************************
Function Name   : usb_hstd_bemp_pipe
Description     : BEMP interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_bemp_pipe(void)
{
    uint16_t    buffer;
    uint16_t    bitsts;

    bitsts  = gp_usb_hstd_hcd_message->status;         /* p->bitsts; */

    /* When operating by the host function, usb_hstd_bemp_pipe_process() is executed without fail because */
    /* only one BEMP message is issued even when the demand of PIPE0 and PIPEx has been generated at the same time. */
    if ((bitsts & USB_BEMP0) == USB_BEMP0 )
    {
        /* Get Pipe PID from pipe number */
        g_usb_hstd_current_pipe = USB_PIPE0;
        buffer = usb_cstd_get_pid(g_usb_hstd_current_pipe);
        /* MAX packet size error ? */
        if ((buffer & USB_PID_STALL) == USB_PID_STALL )
        {
            usb_hstd_control_end(USB_DATA_STALL);
        } 
        else
        {
            /* Branch by the Control transfer stage management */
            switch (g_usb_hcd_ctsq)
            {
                case USB_DATAWR:                    /* Continuance of data stage (Control write) */
                    /* Buffer to CFIFO data write */
                    buffer = usb_cstd_buf_to_cfifo(USB_CUSE);
                    switch (buffer)
                    {
                        case USB_WRITEEND:          /* End of data write (not null) */
                        case USB_WRITESHRT:         /* End of data write */
                            /* Next stage is Control write status stage */
                            g_usb_hcd_ctsq = USB_STATUSWR;
                            hw_usb_set_bempenb(g_usb_hstd_current_pipe);
                            usb_cstd_nrdy_enable(g_usb_hstd_current_pipe);
                        break;
                        case USB_WRITING:           /* Continue of data write */
                            hw_usb_set_bempenb(g_usb_hstd_current_pipe);
                            usb_cstd_nrdy_enable(g_usb_hstd_current_pipe);
                        break;
                        case USB_FIFOERROR:         /* FIFO access error */
                            /* Control Read/Write End */
                            usb_hstd_control_end(USB_DATA_ERR);
                        break;
                        default:
                            /* no processing */
                        break;
                    }
                break;
                case USB_STATUSWR:                  /* End of data stage (Control write) */
                    usb_hstd_status_start();
                break;
                case USB_STATUSRD:                  /* Status stage of Control read transfer */
                    /* Control Read/Write End */
                    usb_hstd_control_end(USB_CTRL_END);
                break;
                default:
                    /* no processing */
                break;
            }
        }
    }
    usb_hstd_bemp_pipe_process(bitsts);

}   /* End of function usb_hstd_bemp_pipe() */



/******************************************************************************
Function Name   : usb_hstd_devaddr_to_speed
Description     : Get device speed from device address
Arguments       : none
Return value    : uint8_t                  : device speed
Note            : Use also to a connection check is possible
******************************************************************************/
uint8_t usb_hstd_devaddr_to_speed(void)
{
    uint16_t    buffer;

    /* Get device address configuration register from device address */
    buffer = usb_hstd_get_devaddx_register();
    if (USB_ERROR != buffer)
    {
        buffer = (uint16_t)(buffer & USB_USBSPD);
        if (USB_FULLSPEED == buffer)
        {
            return  (USB_FSCONNECT);
        }
        else if (USB_LOWSPEED == buffer)
        {
            return  (USB_LSCONNECT);
        }
        else
        {

        }
    }
    return USB_NOCONNECT;
}   /* End of function usb_hstd_devaddr_to_speed() */


/******************************************************************************
Function Name   : usb_hstd_check_device_address
Description     : Check device connected from device address & root port
Arguments       : none
Return value    : uint8_t                  : YES/NO
******************************************************************************/
uint8_t usb_hstd_check_device_address(void)
{
    uint16_t    buffer;

    /* Get device address configuration register from device address */
    buffer = usb_hstd_get_devaddx_register();
    if (USB_ERROR != buffer)
    {
        buffer = (uint16_t)(buffer & USB_USBSPD);
        if (USB_FULLSPEED == buffer)
        {
            return  (USB_FSCONNECT);
        }
        else if (USB_LOWSPEED == buffer)
        {
            return  (USB_LSCONNECT);
        }
        else
        {

        }
    }
    return USB_NOCONNECT;
}   /* End of function usb_hstd_check_device_address() */


/******************************************************************************
Function Name   : usb_hstd_get_devaddx_register
Description     : Get device address configuration register from device address
Arguments       : none
Return value    : uint16_t                  : configuration register
******************************************************************************/
USB_STATIC uint16_t usb_hstd_get_devaddx_register(void)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;
    uint16_t return_value;

    if (g_usb_hcd_devaddr > USB_MAXDEVADDR)
    {
        return_value = USB_ERROR;
    }
    else
    {
        p_reg = (uint16_t *) &(USB0.DEVADD0) + g_usb_hcd_devaddr;
        return_value = *p_reg;
    }

    return return_value;
}   /* End of function usb_hstd_get_devaddx_register() */


/******************************************************************************
Function Name   : usb_hstd_set_devaddx_register
Description     : Set device speed
Arguments       : uint8_t    usbspd     : device speed
                : usb_port_t port       : message keyword
Return value    : none
******************************************************************************/
void usb_hstd_set_devaddx_register(uint8_t usbspd)
{
    R_BSP_VOLATILE_EVENACCESS uint16_t *p_reg;
    uint16_t    data;

    if (USB_FSCONNECT == usbspd)
    {
        data    = (uint16_t)(USB_FULLSPEED);
    }
    else if (USB_LSCONNECT == usbspd)
    {
        data    = (uint16_t)(USB_LOWSPEED);
    }
    else                                      /* else if (usbspd == USB_NOCONNECT) */
    {
        data    = (uint16_t)(USB_NOTUSED);    /* RTPT is set to port0 when unused. */
    }

    p_reg = (uint16_t *) &(USB0.DEVADD0) + g_usb_hcd_devaddr;

    (*p_reg) &= (~USB_USBSPD);
    (*p_reg) |= data;


}   /* End of function usb_hstd_set_devaddx_register() */


/******************************************************************************
Function Name   : usb_hstd_check_attach
Description     : Check Atacch
Arguments       : none
Return value    : uint8_t                  : connection status(ATTACH/DETACH)
Note            : Please change for your SYSTEM
******************************************************************************/
uint8_t usb_hstd_check_attach(void)
{
    uint16_t    buf[3];

    usb_hstd_read_lnst((uint16_t*)&buf);

    if (USB_UNDECID == (uint16_t)(buf[1] & USB_RHST))      /* RHST is at non-connection */
    {
        if ((buf[0] & USB_LNST) == USB_FS_JSTS )            /* High/Full speed device */
        {
            return USB_LNST_ATTACH;
        }
        else if ((buf[0] & USB_LNST) == USB_LS_JSTS )       /* Low speed device */
        {
            return USB_LNST_ATTACH;
        }
        else if ((buf[0] & USB_LNST) == USB_SE0 )
        {
        }
        else 
        {
             /* Do Nothing */
        }
        return USB_LNST_DETACH;
    }
    return USB_RHST_ATTACH;
}   /* End of function usb_hstd_check_attach() */


/******************************************************************************
Function Name   : usb_hstd_set_feature
Description     : Set SetFeature
Arguments       : uint16_t epnum           : endpoint number
Return          : usb_er_t
******************************************************************************/
usb_er_t usb_hstd_set_feature(uint16_t epnum)
{
    if (0xFF == epnum)
    {
        /* SetFeature(Device) */
        gs_usb_hstd_mgr_request.word.byte.bm_request_type = USB_REQUEST_TYPE(USB_HOST_TO_DEV,USB_STANDARD,USB_DEVICE);
        gs_usb_hstd_mgr_request.w_value                   = USB_DEV_REMOTE_WAKEUP;
        gs_usb_hstd_mgr_request.w_index                   = (uint16_t)0x0000;
    }
    else
    {
        /* SetFeature(endpoint) */
        gs_usb_hstd_mgr_request.word.byte.bm_request_type 
                                                       = USB_REQUEST_TYPE(USB_HOST_TO_DEV,USB_STANDARD,USB_ENDPOINT);
        gs_usb_hstd_mgr_request.w_value                   = USB_ENDPOINT_HALT;
        gs_usb_hstd_mgr_request.w_index                   = epnum;
    }
    gs_usb_hstd_mgr_request.word.byte.b_request           = USB_REQUEST_SET_FEATURE;
    gs_usb_hstd_mgr_request.w_length                      = (uint16_t)0x0000;
    gs_usb_hstd_mgr_request.address                       = (uint16_t)g_usb_mgr_devaddr;
    return usb_hstd_do_command((uint8_t*)&gs_usb_hstd_mgr_dummy_data);
}   /* End of function usb_hstd_set_feature() */


/******************************************************************************
Function Name   : usb_hstd_clear_feature
Description     : Set ClearFeature
Arguments       : uint16_t epnum           : endpoint number
Return          : usb_er_t
******************************************************************************/
usb_er_t usb_hstd_clear_feature(uint16_t epnum)
{
    if (0xFF == epnum)
    {
        /* ClearFeature(Device) */
        gs_usb_hstd_mgr_request.word.byte.bm_request_type = USB_REQUEST_TYPE(USB_HOST_TO_DEV,USB_STANDARD,USB_DEVICE);
        gs_usb_hstd_mgr_request.w_value                   = USB_DEV_REMOTE_WAKEUP;
        gs_usb_hstd_mgr_request.w_index                   = (uint16_t)0x0000;
    }
    else
    {
        /* ClearFeature(endpoint) */
        gs_usb_hstd_mgr_request.word.byte.bm_request_type 
                                                       = USB_REQUEST_TYPE(USB_HOST_TO_DEV,USB_STANDARD,USB_ENDPOINT);
        gs_usb_hstd_mgr_request.w_value                   = USB_ENDPOINT_HALT;
        gs_usb_hstd_mgr_request.w_index                   = epnum;
    }
    gs_usb_hstd_mgr_request.word.byte.b_request           = USB_REQUEST_CLEAR_FEATURE;
    gs_usb_hstd_mgr_request.w_length                      = (uint16_t)0x0000;
    gs_usb_hstd_mgr_request.address                       = (uint16_t)g_usb_mgr_devaddr;
    return usb_hstd_do_command((uint8_t*)&gs_usb_hstd_mgr_dummy_data);
}   /* End of function usb_hstd_clear_feature() */


/******************************************************************************
Function Name   : usb_hstd_do_command
Description     : command submit
Arguments       : uint8_t  *data_table      : Transfer data
Return          : usb_er_t
******************************************************************************/
USB_STATIC usb_er_t usb_hstd_do_command(uint8_t *data_table)
{
    g_usb_mgr_ctrl_msg.p_tranadr   = (void*)data_table;
    g_usb_mgr_ctrl_msg.complete    = (usb_hcb_t)&usb_hstd_transfer_result;
    g_usb_mgr_ctrl_msg.tranlen     = (usb_leng_t)gs_usb_hstd_mgr_request.w_length;
    g_usb_mgr_ctrl_msg.pipenum     = USB_PIPE0;
    g_usb_mgr_ctrl_msg.setup       = (void*)&gs_usb_hstd_mgr_request;
    return usb_hstd_transfer_start(&g_usb_mgr_ctrl_msg);
}   /* End of function usb_hstd_do_command() */


/******************************************************************************
Function Name   : usb_hstd_pipe_to_epadr
Description     : pipe number to endpoint address
Arguments       : none
Return value    : uint8_t               : Endpoint Number + Direction
******************************************************************************/
uint8_t usb_hstd_pipe_to_epadr(void)
{

    uint16_t    buffer;
    uint8_t     direp = 0;

    if (USB_PIPE0 == g_usb_hstd_current_pipe)
    {
        buffer = hw_usb_read_dcpcfg();
    }
    else
    {
        /* Pipe select */
        buffer = hw_usb_read_pipecfg();
        direp = (uint8_t)(buffer & USB_EPNUM);
    }

    direp   |= (uint8_t)( ((buffer & USB_DIR)^USB_DIR) << 3);

    return direp;

}   /* End of function usb_hstd_pipe_to_epadr() */


/******************************************************************************
Function Name   : usb_hstd_pipe_to_hcddevaddr
Description     : Get device address from pipe number
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_pipe_to_hcddevaddr(void)
{
    uint16_t    buffer;

    if (USB_PIPE0 == g_usb_hstd_current_pipe)
    {
        buffer = hw_usb_read_dcpmaxp();
    }
    else
    {
        /* Pipe select */
        hw_usb_write_pipesel(g_usb_hstd_current_pipe);
        buffer = hw_usb_read_pipemaxp();
    }

    /* Device address */
    g_usb_hcd_devaddr    = USB_DEVSEL2ADDR(buffer & USB_DEVSEL);
}   /* End of function usb_hstd_pipe_to_hcddevaddr() */


/******************************************************************************
Function Name   : usb_hstd_pipe_to_addr
Description     : Get device address from pipe number
Arguments       : none
Return value    : usb_addr_t                    : device address
******************************************************************************/
usb_addr_t usb_hstd_pipe_to_addr(void)
{
    uint16_t    buffer;

    if (USB_PIPE0 == g_usb_hstd_current_pipe)
    {
        buffer = hw_usb_read_dcpmaxp();
    }
    else
    {
        /* Pipe select */
        hw_usb_write_pipesel(g_usb_hstd_current_pipe);
        buffer = hw_usb_read_pipemaxp();
    }

    /* Device address */
    return  USB_DEVSEL2ADDR(buffer & USB_DEVSEL);
}   /* End of function usb_hstd_pipe_to_addr() */


/******************************************************************************
Function Name   : usb_hstd_status_notification
Description     : Notification status
Argument        : uint16_t msginfo       : message info
                : uint8_t keyword        : message keyword
Return          : none
******************************************************************************/
void usb_hstd_status_notification(usb_strct_t msginfo, usb_strct_t keyword)
{
    if (USB_GET_SND(USB_MGR_MBX, msginfo, &usb_hstd_dummy_function, keyword) != USB_OK)
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_HOST | USB_DEBUG_HOOK_CODE6);
    }
}   /* End of function usb_hstd_status_notification() */


/******************************************************************************
Function Name   : usb_hstd_status_change
Description     : Notification status change
Argument        : uint16_t msginfo       : message info
                : uint8_t keyword        : message keyword
Return          : usb_er_t
******************************************************************************/
usb_er_t usb_hstd_status_change(usb_strct_t msginfo, usb_strct_t keyword)
{
    gp_usb_mgr_msg->msginfo   = msginfo;
    gp_usb_mgr_msg->keyword   = keyword;
    return  USB_SND_MSG(USB_HCD_MBX, (usb_msg_t*)gp_usb_mgr_msg);
}   /* End of function usb_hstd_status_change() */


/******************************************************************************
Function Name   : usb_hstd_status_result
Description     : Notification status result
Argument        : usb_strct_t status
Return          : none
******************************************************************************/
void usb_hstd_status_result(usb_strct_t status)
{
    g_usb_mgr_seq++;
    gp_usb_hstd_hcd_message->msginfo   = USB_MGR_CONTINUE;
    gp_usb_hstd_hcd_message->keyword   = g_usb_hstd_hcd_port;
    gp_usb_hstd_hcd_message->status    = status;
    USB_SND_MSG(USB_MGR_MBX, (usb_msg_t*)gp_usb_hstd_hcd_message);
}   /* End of function usb_hstd_status_result() */


/******************************************************************************
Function Name   : usb_hstd_transfer_result
Description     : Notification transfer result
Argument        : usb_hutr_t *utrmsg      : message
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_transfer_result(usb_hutr_t *p_utrmsg)
{
    g_usb_mgr_seq++;
    p_utrmsg->msginfo = USB_MGR_CONTINUE;
    USB_SND_MSG(USB_MGR_MBX, (usb_msg_t*)p_utrmsg);
}   /* End of function usb_hstd_transfer_result() */


/******************************************************************************
Function Name   : usb_hstd_transferend_check
Description     : Transfer End Check
Argument        : usb_strct_t status
Return          : none
******************************************************************************/
void usb_hstd_transferend_check(usb_strct_t status)
{
    if (USB_PIPE0 == g_usb_hstd_current_pipe)
    {
        /* Control Read/Write End */
        usb_hstd_control_end(status);
    }
    else
    {
        /* Transfer stop */
        usb_hstd_forced_termination(status);
    }
    R_USB_REL_BLK(gp_usb_hstd_hcd_message->flag);
}   /* End of function usb_hstd_transferend_check() */


/******************************************************************************
Function Name   : usb_hstd_driver_reg_init
Description     : driver initialized
Argument        : none
Return          : none
******************************************************************************/
void    usb_hstd_driver_reg_init(void)
{
    g_usb_hcd_driver.rootport   = USB_NOPORT;
    g_usb_hcd_driver.devaddr    = USB_NODEVICE;
    g_usb_hcd_driver.devstate   = USB_STS_DETACH;
    g_usb_hcd_driver.ifclass    = USB_IFCLS_NOT;
}   /* End of function usb_hstd_driver_reg_init() */


/******************************************************************************
Function Name   : usb_hstd_setup_clear_status
Description     : Clear setup interrupt status
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hstd_setup_clear_status(void)
{
    hw_usb_hclear_sts_sign();
    hw_usb_hclear_sts_sack();
}   /* End of function usb_hstd_setup_clear_status() */

/******************************************************************************
Function Name   : usb_hstd_setup_enable
Description     : Enable setup interrupt
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hstd_setup_enable(void)
{
    usb_hstd_setup_clear_status();
    hw_usb_hset_enb_signe();
    hw_usb_hset_enb_sacke();
    hw_usb_hset_sureq();
}   /* End of function usb_hstd_setup_enable() */

/******************************************************************************
Function Name   : usb_hstd_setup_disable
Description     : Disable setup interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_setup_disable(void)
{
    usb_hstd_setup_clear_status();
    USB0.INTENB1.WORD &= (uint16_t) ~(USB_SIGNE | USB_SACKE);
}   /* End of function usb_hstd_setup_disable() */


/******************************************************************************
Function Name   : usb_hstd_port_enable
Description     : Check connection
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_port_enable(void)
{
    usb_hstd_vbus_control_on();
#if USB_CFG_BC == USB_CFG_DISABLE
    usb_cpu_delay_xms((uint16_t)100);
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */

    if (usb_hstd_check_attach() == USB_LNST_ATTACH )
    {
        usb_hstd_attach_control();
    }
    else
    {
        usb_hstd_detach_control();
    }
}   /* End of function usb_hstd_port_enable() */

/******************************************************************************
 Function Name   : usb_hclass_request_complete
 Description     : Class request transfer complete
 Arguments       : usb_hutr_t *mess  : Pointer to usb_hutr_t structure.
                 : uint16_t  data1  : Not used.
                 : uint16_t  data2  : Not used.
 Return          : none
 ******************************************************************************/
void usb_hclass_request_complete (usb_hutr_t *p_mess)
{
    usb_ctrl_t ctrl;

    if (USB_CTRL_END == p_mess->status) /* Check usb receive status */
    {
        ctrl.status = USB_ACK;
    }
    else if (USB_DATA_STALL == p_mess->status) /* Check usb receive status */
    {
        ctrl.status = USB_STALL;
    }
    else
    {
        ctrl.status = USB_ERROR;
    }
    ctrl.type = USB_REQUEST;
    ctrl.setup.type = p_mess->setup[0];
    ctrl.pipe = USB_PIPE0;
    ctrl.setup.value = p_mess->setup[1];
    ctrl.setup.index = p_mess->setup[2];
    ctrl.setup.length = p_mess->setup[3];
    ctrl.size = ctrl.setup.length - g_usb_hstd_data_cnt_pipe0;
    usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl); /* Set Event()  */
} /* End of function usb_hclass_request_complete */


/******************************************************************************
 Function Name   : usb_hstd_suspend_complete
 Description     : usb_hstd_change_device_state callback (Suspend)
 Arguments       : uint16_t  data1  : Not used.
                 : uint16_t  data2  : Not used.
 Return value    : none
 ******************************************************************************/
void usb_hstd_suspend_complete (uint16_t data1, uint16_t data2)
{
    usb_ctrl_t  ctrl;

    g_usb_hstd_is_susp_resm &= (~(1 << USB_STS_SUSPEND));
    usb_cstd_set_event(USB_STS_SUSPEND, &ctrl);
} /* End of function usb_hstd_suspend_complete */

/******************************************************************************
 Function Name   : usb_hstd_resume_complete
 Description     : usb_hstd_change_device_state callback (Resume)
 Arguments       : uint16_t  data1  : Not used.
                 : uint16_t  data2  : Not used.
 Return value    : none
 ******************************************************************************/
void usb_hstd_resume_complete (uint16_t data1, uint16_t data2)
{
    usb_ctrl_t  ctrl;

    g_usb_hstd_is_susp_resm &= (~(1 << USB_STS_RESUME));
    usb_cstd_set_event(USB_STS_RESUME, &ctrl);
}/* End of function usb_hstd_resume_complete */


/******************************************************************************
 Function Name   : usb_hstd_read_complete
 Description     : CallBack Function
 Arguments       : usb_hutr_t    *mess   : Pointer to usb_hutr_t structure
 Return          : none
 ******************************************************************************/
void usb_hstd_read_complete (usb_hutr_t *p_mess)
{
    usb_ctrl_t ctrl;

    /* Set Receive data length */
    ctrl.size   = g_usb_read_request_size[p_mess->pipenum] - p_mess->tranlen;
    ctrl.pipe   = p_mess->pipenum;                    /* Pipe number setting */

#if defined(USB_CFG_HCDC_USE)
    if (USB_CFG_HCDC_BULK_IN == ctrl.pipe)
    {
        ctrl.type = USB_HCDC; /* CDC Data class  */
    }

    else /* USB_CFG_HCDC_INT_IN */
    {
        ctrl.type = USB_HCDCC; /* CDC Control class  */
    }
#endif

#if defined(USB_CFG_HHID_USE)
    ctrl.type    = USB_HHID;                                    /* hid Data class  */
#endif

#if defined(USB_CFG_HVND_USE)
    ctrl.type    = USB_HVND;                                    /* Host Vendor class  */
#endif

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
} /* End of function usb_hstd_read_complete() */


/******************************************************************************
 Function Name   : usb_hstd_write_complete
 Description     : CallBack Function
 Arguments       : usb_hutr_t    *mess   : Pointer to usb_hutr_t structure
 Return          : none
 ******************************************************************************/
void usb_hstd_write_complete (usb_hutr_t *p_mess)
{
    usb_ctrl_t ctrl;

    ctrl.pipe   = p_mess->pipenum;                      /* Pipe number setting */

#if defined(USB_CFG_HCDC_USE)
    ctrl.type = USB_HCDC;       /* CDC Data class  */
#endif

#if defined(USB_CFG_HHID_USE)
    ctrl.type    = USB_HHID;    /* hid Data class  */
#endif

#if defined(USB_CFG_HVND_USE)
    ctrl.type    = USB_HVND;                                    /* Host Vendor class  */
#endif

    if (USB_DATA_OK == p_mess->status)
    {
        ctrl.status = USB_SUCCESS;
    }
    else
    {
        ctrl.status = USB_ERR_NG;
    }
    usb_cstd_set_event(USB_STS_WRITE_COMPLETE, &ctrl);
} /* End of function usb_hstd_write_complete() */

/******************************************************************************
 Function Name   : usb_hstd_host_registration
 Description     : sample registration.
 Argument        : none
 Return          : none
 ******************************************************************************/
void usb_hstd_host_registration (void)
{

#if defined(USB_CFG_HCDC_USE)
    cdc_registration();                         /* Sample driver registration */

#endif /* defined(USB_CFG_HCDC_USE) */

#if defined(USB_CFG_HHID_USE)
    hid_registration();                         /* Sample driver registration */

#endif /* defined(USB_CFG_HHID_USE) */

#if defined(USB_CFG_HMSC_USE)
    msc_registration();                         /* Sample driver registration. */

#endif /* defined(USB_CFG_HMSC_USE) */

#if defined(USB_CFG_HVND_USE)
    usb_hvnd_registration();                    /* Sample driver registration */
#endif /* defined(USB_CFG_HVND_USE) */

} /* End of function usb_hstd_host_registration */

/******************************************************************************
Function Name   : usb_hstd_class_check
Description     : class check.
Arguments       : uint16_t **pp_table  : Pointer to data table.
Return value    : none
******************************************************************************/
void usb_hstd_class_check(uint8_t **pp_table)
{
    gsp_usb_hstd_device_desc_tbl  = (uint8_t*)((uint16_t*)pp_table[0]);      /* Device Descriptor Table */
    gsp_usb_hstd_config_desc_tbl  = (uint8_t*)((uint16_t*)pp_table[1]);      /* Configuration Descriptor Table */

    /* Enumeration Sequence String Descriptor #0 receive request */
    g_usb_enum_seq  = USB_SEQ_0;                            /* Initialize sequence number for enumeration */
    USB_GET_SND(USB_HCLASS_MBX, USB_MSG_HCLASS_OPEN, &usb_hstd_dummy_function, USB_NULL);
} /* End of function usb_hstd_class_check() */

/******************************************************************************
Function Name   : usb_hstd_get_string_desc
Description     : Set GetDescriptor
Arguments       : usb_addr_t addr         : device address
                : uint8_t string          : descriptor index
                : usb_hcb_t complete       : callback function
Return value    : none
******************************************************************************/
USB_STATIC void usb_hstd_get_string_desc(usb_addr_t addr, uint8_t string, usb_hcb_t complete)
{
    uint16_t        i;

    if (0 == string)
    {
        g_usb_hstd_request.w_index   = 0;
        g_usb_hstd_request.w_length  = 4;
    }
    else
    {
        g_usb_hstd_request.w_index   = (uint16_t)(gs_usb_hstd_string_descriptor[2]);
        g_usb_hstd_request.w_index   |= (uint16_t)((uint16_t)(gs_usb_hstd_string_descriptor[3]) << 8);
        g_usb_hstd_request.w_length  = (uint16_t)USB_STRING_SIZE;
    }
    g_usb_hstd_request.word.byte.bm_request_type  = USB_REQUEST_TYPE(USB_DEV_TO_HOST,USB_STANDARD,USB_DEVICE);
    g_usb_hstd_request.word.byte.b_request        = USB_REQUEST_GET_DESCRIPTOR;
    g_usb_hstd_request.w_value                    = (uint16_t)(USB_STRING_DESCRIPTOR + string);
    g_usb_hstd_request.address                    = (uint16_t)addr;

    /* WAIT_LOOP */
    for (i = 0; i < g_usb_hstd_request.w_length; i++)
    {
        gs_usb_hstd_string_descriptor[i] = (uint8_t)0xFF;
    }

    gs_usb_hstd_string_control.p_tranadr   = (void*)gs_usb_hstd_string_descriptor;
    gs_usb_hstd_string_control.complete    = complete;
    gs_usb_hstd_string_control.tranlen     = (usb_leng_t)g_usb_hstd_request.w_length;
    gs_usb_hstd_string_control.pipenum     = USB_PIPE0;
    gs_usb_hstd_string_control.setup       = (uint16_t*)&g_usb_hstd_request;

    usb_hstd_transfer_start(&gs_usb_hstd_string_control);
} /* End of function usb_hstd_get_string_desc() */

/******************************************************************************
Function Name   : usb_hstd_check_open_class
Description     : USB Device Class open check.
Arguments       : class         : Interface class
Return value    : Result for Open USB Class.(USB_YES/USB_NO)
******************************************************************************/
USB_STATIC uint8_t usb_hstd_check_open_class (uint8_t class)
{
    uint8_t ret = USB_NO;

    switch (class)
    {
        case USB_IFCLS_HID:
            if (0 != ((1 << USB_HHID) & g_usb_cstd_open_class))
            {
                ret = USB_YES;
            }
        break;

        case USB_IFCLS_CDC:
            if (0 != ((1 << USB_HCDC) & g_usb_cstd_open_class))
            {
                ret = USB_YES;
            }
        break;

        case USB_IFCLS_MAS:
            if (0 != ((1 << USB_HMSC) & g_usb_cstd_open_class))
            {
                ret = USB_YES;
            }
        break;

        case USB_IFCLS_VEN:
#if defined(USB_CFG_HCDC_USE)
#if USB_CFG_HCDC_IFCLS == USB_CFG_VEN
            if (0 != ((1 << USB_HCDC) & g_usb_cstd_open_class))
            {
                ret = USB_YES;
            }
#endif /* USB_CFG_HCDC_IFCLS == USB_CFG_VEN */

#endif /* defined(USB_CFG_HCDC_USE) */
            if (0 != ((1 << USB_HVND) & g_usb_cstd_open_class))
            {
                ret = USB_YES;
            }
        break;

        default:
        break;
    }

    return ret;
}   /* End of function usb_hstd_check_open_class() */

/******************************************************************************
Function Name   : usb_hstd_get_str_desc_cb
Description     : Callback for complete to Get String Descriptor.
Arguments       : usb_hutr_t *ptr : message
Return value    : none
******************************************************************************/
USB_STATIC void usb_hstd_get_str_desc_cb(usb_hutr_t *ptr)
{
    ptr->msginfo    = USB_MSG_HCLASS_OPEN;
    USB_SND_MSG(USB_HCLASS_MBX, (usb_msg_t*)ptr);
}   /* End of function usb_hstd_get_str_desc_cb() */


/******************************************************************************
Function Name   : usb_hstd_specified_path
Description     : Next Process Selector
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hstd_specified_path(void)
{
    USB_GET_SND(USB_HCLASS_MBX, USB_MSG_HCLASS_OPEN, &usb_hstd_dummy_function, (usb_strct_t)USB_NULL);
}   /* End of function usb_hstd_specified_path() */

/******************************************************************************
Function Name   : usb_hstd_class_enumeration
Description     : check class
Arguments       : usb_hutr_t *mess : message
Return value    : none
******************************************************************************/
void usb_hstd_class_enumeration(usb_tskinfo_t *p_mess)
{
    static uint8_t     s_class;
    uint16_t    retval;
    uint8_t     string;
    uint8_t     *p_desc;
    uint16_t    desc_len;
    totallen_t  length2;
    uint16_t    length1;
    uint8_t     i;
    uint8_t     open_class;
    totallen_t  vid;
    totallen_t  pid;
    uint16_t    vendor_id;
    uint16_t    product_id;
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    usb_compliance_t disp_param;
#endif /* end USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    /* Branch for Enumeration Sequence */
    switch (g_usb_enum_seq)
    {
    /* Enumeration Sequence String Descriptor #0 receive request */
        case USB_SEQ_0:
            vid.word.byte.up      =  gsp_usb_hstd_device_desc_tbl[USB_DEV_ID_VENDOR_H];
            vid.word.byte.dn      =  gsp_usb_hstd_device_desc_tbl[USB_DEV_ID_VENDOR_L];
            vendor_id = vid.word.w;
            pid.word.byte.up      =  gsp_usb_hstd_device_desc_tbl[USB_DEV_ID_PRODUCT_H];
            pid.word.byte.dn      =  gsp_usb_hstd_device_desc_tbl[USB_DEV_ID_PRODUCT_L];
            product_id = pid.word.w;

            retval = USB_ERROR;

            /* Check TPL */
            /* WAIT_LOOP */
            for (i = 0;  i < gs_usb_device_tpl[0]; i++)
            {
                if ((USB_NOVENDOR == gs_usb_device_tpl[(i * 2) + 2]) ||
                    (vendor_id == gs_usb_device_tpl[(i * 2) + 2]))
                {
                    if ((USB_NOPRODUCT == gs_usb_device_tpl[(i * 2) + 3]) ||
                        (product_id == gs_usb_device_tpl[(i * 2) + 3]))
                    {
                        retval = USB_YES;
                    }
                }
            }
            if (USB_ERROR == retval)
            {
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
                disp_param.status = USB_CT_NOTTPL;
                disp_param.pid    = product_id;
                disp_param.vid    = vendor_id;
                usb_compliance_disp ((void *)&disp_param);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
                usb_hstd_return_enu_mgr(USB_ERROR);
                break;
            }

            /* Check interface class */
            length1 = 0;
            length2.word.byte.up        =  gsp_usb_hstd_config_desc_tbl[USB_DEV_W_TOTAL_LENGTH_H];
            length2.word.byte.dn        =  gsp_usb_hstd_config_desc_tbl[USB_DEV_W_TOTAL_LENGTH_L];
            if (length2.word.w > USB_CONFIGSIZE)
            {
                length2.word.w = USB_CONFIGSIZE;
            }

            /* Search within configuration total-length */
            retval = USB_ERROR;
            /* WAIT_LOOP */
            while (length1 < length2.word.w)
            {               
                if (USB_DT_INTERFACE == gsp_usb_hstd_config_desc_tbl[length1 + USB_DEV_B_DESCRIPTOR_TYPE])
                {
                    gp_usb_hstd_interface_tbl  = (uint8_t*)&gsp_usb_hstd_config_desc_tbl[length1];
                    s_class = gsp_usb_hstd_config_desc_tbl[length1 + USB_IF_B_INTERFACECLASS];
                    open_class = usb_hstd_check_open_class(s_class);
                    if (USB_YES == open_class)
                    {
#if defined(USB_CFG_HMSC_USE)
                        if (USB_IFCLS_MAS == s_class)
                        {   /* Check MSC Sub class */
                            g_usb_hmsc_sub_class = gp_usb_hstd_interface_tbl[USB_IF_B_INTERFACESUBCLASS];
                            if ((USB_ATAPI == g_usb_hmsc_sub_class) || (USB_SCSI == g_usb_hmsc_sub_class)
                                 || (USB_ATAPI_MMC5 == g_usb_hmsc_sub_class))
                            {   /* Check Num endpoints */
                                if (gsp_usb_hstd_config_desc_tbl[length1 + USB_IF_B_NUMENDPOINTS] >= USB_TOTALEP)
                                {
                                    retval = USB_YES;
                                }
                                else
                                {
                                    retval = USB_ERROR;                 /* num endpoint error */
                                }
                            }
                            else
                            {
                                g_usb_hmsc_sub_class = USB_NULL;    /* sub class error */
                            }
                        }
                        else
                        {
                            retval = USB_YES;                           /* USB_IFCLS_CDC, USB_IFCLS_HID */
                        }

#else  /* defined(USB_CFG_HMSC_USE) */
                        retval = USB_YES;

#endif /* defined(USB_CFG_HMSC_USE) */

                        /* Get String descriptor */
                        usb_hstd_get_string_desc (USB_DEVICEADDR, 0, (usb_hcb_t)usb_hstd_get_str_desc_cb);
                        /* String Descriptor #0 Receive wait */
                        g_usb_enum_seq++;
                        break;
                    }
                }
                length1 += gsp_usb_hstd_config_desc_tbl[length1];
            }
            if (USB_ERROR == retval)
            {
                g_usb_enum_seq = USB_SEQ_0;
                usb_hstd_return_enu_mgr(USB_NO);
            }
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            else
            {
                disp_param.status = USB_CT_TPL;
                disp_param.pid    = product_id;
                disp_param.vid    = vendor_id;
                usb_compliance_disp ((void *)&disp_param);
            }

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
        break;

        /* Enumeration Sequence String Descriptor #0 Receive complete */
        case USB_SEQ_1:
            /* String descriptor check */
            if (USB_CTRL_END == p_mess->status)
            {
                string  = gsp_usb_hstd_device_desc_tbl[USB_DEV_I_PRODUCT];     /* Check iProduct */
                if (0 != string)
                {
                    /* Get String descriptor */
                    usb_hstd_get_string_desc (USB_DEVICEADDR, string, (usb_hcb_t)usb_hstd_get_str_desc_cb);
                }
                else
                {
                    usb_hstd_specified_path();                /* Skip Get String Descriptor(iProduct) */
                }
                /* StringDescriptor iProduct Receive */
                g_usb_enum_seq++;
            }
            else
            {
                g_usb_enum_seq = USB_SEQ_0;
                /* Return to MGR */
                usb_hstd_return_enu_mgr (USB_NO);
            }
        break;

        /* StringDescriptor iProduct Receive complete */
        case USB_SEQ_2:
            /* Check protocol (bInterfaceProtocol - Interface Descriptor)*/
            p_desc = gsp_usb_hstd_config_desc_tbl;
            desc_len = (uint16_t)(((uint16_t)*(p_desc + 3)) << 8);
            desc_len += (uint16_t)*(p_desc + 2);

            /* Pipe Information table set */
            if (USB_IFCLS_MAS == s_class)
            {
#if defined(USB_CFG_HMSC_USE)
                retval = usb_hmsc_pipe_info (p_desc, desc_len);

#else  /* defined(USB_CFG_HMSC_USE) */
                retval = USB_ERROR;

#endif /* defined(USB_CFG_HMSC_USE) */
            }
            else if (USB_IFCLS_CDC == s_class)
            {
#if defined(USB_CFG_HCDC_USE)
                /* Pipe Information table set */
                retval = usb_hcdc_pipe_info (p_desc, desc_len);

#else  /* defined(USB_CFG_HCDC_USE) */
                retval = USB_ERROR;

#endif /* defined(USB_CFG_HCDC_USE) */
            }
            else if (USB_IFCLS_HID == s_class)
            {
#if defined(USB_CFG_HHID_USE)
                /* pipe information table set */
                retval = usb_hhid_pipe_info(p_desc, desc_len);
#else  /* defined(USB_CFG_HHID_USE) */
                retval = USB_ERROR;

#endif /* defined(USB_CFG_HHID_USE) */
            }
            else if (USB_IFCLS_VEN == s_class)
            {

#if defined(USB_CFG_HCDC_USE)
#if USB_CFG_HCDC_IFCLS == USB_CFG_VEN
                /* Pipe Information table set */
                retval = usb_hcdc_pipe_info (p_desc, desc_len);
#else   /* USB_CFG_HCDC_IFCLS == USB_CFG_VEN */
                retval = USB_ERROR;
#endif /* USB_CFG_HCDC_IFCLS == USB_CFG_VEN */
#endif /* defined(USB_CFG_HCDC_USE) */

#if defined(USB_CFG_HVND_USE)
                /* pipe information table set */
                retval = usb_hvnd_pipe_info(p_desc, desc_len);
#endif /* defined(USB_CFG_HVND_USE) */

#if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_HCDC_USE)
                retval = USB_ERROR;
#endif  /* !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_HCDC_USE) */

            }
            else
            {
                /* None */
                retval = USB_ERROR;
            }

            if (USB_ERROR == retval)
            {
                /* Enumeration class check error */
                retval  = USB_NO;
            }
            else
            {
                /* Enumeration class check OK */
                retval  = USB_YES;
            }
            /* Enumeration Sequence Complete */
            g_usb_enum_seq  = USB_SEQ_0;
            /* return to MGR */
            usb_hstd_return_enu_mgr(retval);
        break;

        default:
            g_usb_enum_seq = USB_SEQ_0;
            /* Return to MGR */
            usb_hstd_return_enu_mgr (USB_NO);
        break;
    }
}   /* End of function usb_hstd_class_enumeration() */

/******************************************************************************
 Function Name   : usb_hstd_class_task
 Description     : Each device class task processing
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_class_task (void)
{
    usb_hutr_t   *p_mess;
    usb_er_t     err;

     err = R_USB_RCV_MSG(USB_HCLASS_MBX,(usb_msg_t**)&p_mess);
     if (USB_OK == err)
     {
        if (USB_MSG_HCLASS_OPEN == p_mess->msginfo)
        {
            usb_hstd_class_enumeration((usb_tskinfo_t *)p_mess);
        }
#if defined(USB_CFG_HMSC_USE)
        else
        {
            usb_hmsc_process ((usb_tskinfo_t *)p_mess);
        }
#endif /* defined(USB_CFG_HMSC_USE) */

        R_USB_REL_BLK(p_mess->flag);
    }
} /* End of function usb_hstd_class_task */

/******************************************************************************
 Function Name   : usb_hstd_connect_err_event_set
 Description     : Set event for "USB_STS_NOT_SUPPORT"
 Arguments       : none
 Return          : none
 ******************************************************************************/
void usb_hstd_connect_err_event_set(void)
{
    usb_ctrl_t ctrl;

    usb_cstd_set_event(USB_STS_NOT_SUPPORT, &ctrl);          /* set event()  */
} /* End of function usb_hstd_connect_err_event_set */

/******************************************************************************
Function Name   : usb_hstd_dummy_function
Description     : dummy function
Arguments       : uint16_t data1            : not use
                : uint16_t data2            : not use
Return value    : none
******************************************************************************/
void usb_hstd_dummy_function(usb_hutr_t *p_utr, uint16_t data1, uint16_t data2)
{
}   /* End of function usb_hstd_dummy_function() */

/******************************************************************************
Function Name   : usb_hstd_sw_reset
Description     : Software reset
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_sw_reset(void)
{
    hw_usb_clear_usbe ();
    hw_usb_set_usbe ();
}   /* End of function usb_hstd_sw_reset() */


/******************************************************************************
Function Name   : usb_hstd_hw_start
Description     : Start Oscillation: H/W reset
                : Reset start
                : Set bus interface register after started clock
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_hw_start(void)
{
    USB0.SYSCFG.WORD = USB_SCKE;
#if USB_CFG_ENDIAN == USB_CFG_BIG
    USB0.CFIFOSEL.WORD  = (USB_MBW_16|USB_FIFO_BIG);
    USB0.D0FIFOSEL.WORD = (USB_MBW_16|USB_FIFO_BIG);
    USB0.D1FIFOSEL.WORD = (USB_MBW_16|USB_FIFO_BIG);
#else /* USB_CFG_ENDIAN == USB_CFG_BIG */
    /* Little-endian */
    USB0.CFIFOSEL.WORD  = (USB_MBW_16);
    USB0.D0FIFOSEL.WORD = (USB_MBW_16);
    USB0.D1FIFOSEL.WORD = (USB_MBW_16);
#endif  /* USB_CFG_ENDIAN == USB_CFG_BIG */
}   /* End of function usb_hstd_hw_start() */


/******************************************************************************
 Function Name   : usb_hstd_class_driver_start
 Description     : Init host class driver task.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_class_driver_start (void)
{
    g_usb_enum_seq      = (uint16_t)USB_SEQ_0;

} /* End of function usb_hstd_class_driver_start() */

#endif /* ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST) */

/******************************************************************************
End of file
******************************************************************************/
