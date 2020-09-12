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
 * Copyright (C) 2014(2019) Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/*******************************************************************************
 * File Name    : r_usb_typedef.h
 * Description  : User macro define file
 *                This file is the macro-definition header file which a user can operate.
 ******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*******************************************************************************/

#ifndef R_USB_TYPEDEF_H
#define R_USB_TYPEDEF_H


/* Condition compilation by the difference of the operating system */
    #ifndef NULL
      #define   NULL    (0u)
    #endif  /* NULL */

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef void*   VP;         /* Pointer to variable      */
typedef long    TMO;        /* Time out                 */
typedef long    VP_INT;     /* Integer data             */

/*----------- msghead -----------*/
typedef struct
{
    VP msghead; /* Message header */
} t_msg_t;

typedef t_msg_t         usb_msg_t;
typedef uint16_t        usb_er_t;           /*typedef ER              usb_er_t; */
typedef TMO             usb_tm_t;
typedef VP              usb_mh_t;
typedef VP_INT          usb_vp_int_t;

typedef struct
{
    usb_msg_t   *p_address;                   /* Waiting message */
    uint16_t     counter;                     /* Wait counter */
} usb_waitinfo_t;

typedef struct
{
    usb_msg_t   *p_address;                   /* Queuing as for some message. */
    uint8_t      task_id;                     /* Wait task id */
} usb_waitque_t;

typedef uint8_t             usb_pipe_no_t;
typedef uint8_t             usb_port_t;
typedef uint8_t             usb_addr_t;
typedef uint32_t            usb_leng_t;
typedef uint8_t             usb_strct_t;

typedef struct  usb_utr     usb_putr_t;
typedef struct  usb_sutr_t  usb_hutr_t;
typedef void    (*usb_cbinfo_t)(uint16_t, uint16_t);
typedef void    (*usb_cbcheck_t)(uint8_t**);
typedef void    (*usb_hcb_t)(usb_hutr_t*);
typedef void    (*usb_pcb_t)(struct usb_utr *, uint16_t, uint16_t);
typedef void    (*mgrfunc_t)(void);

/* Class request processing function type. */
typedef void (*usb_cb_trn_t) (usb_setup_t* preq, uint16_t ctsq);

typedef struct usb_sutr_t
{
    usb_strct_t     msginfo;        /* Message Info for F/W */
    usb_strct_t     pipenum;        /* Pipe number */
    uint16_t        status;         /* status */
    usb_strct_t     flag;           /* not used */
    usb_hcb_t       complete;       /* Callback function address */
    uint8_t         *p_tranadr;     /* Transfer data start address */
    uint16_t        *setup;         /* Setup packet(for control only) */
    usb_leng_t      tranlen;        /* Transfer data length */
    uint16_t        dummy;          /* not used */
} usb_sutr_t;

typedef struct usb_utr
{
    uint16_t    msginfo;        /* Message Info for F/W */
    uint16_t    keyword;        /* Root port / Device address / Pipe number */
    uint16_t    result;         /* Result */
    usb_pcb_t   complete;       /* Call Back Function Info */
    void        *p_tranadr;     /* Transfer data Start address */
    uint32_t    tranlen;        /* Transfer data length */
    uint16_t    *p_setup;       /* Setup packet(for control only) */
    uint16_t    status;         /* Status */
    uint16_t    pipectr;        /* Pipe control register */
} usb_utr_t;

typedef struct
{
    union {
        struct {                    /* Characteristics of request */
#if USB_CFG_ENDIAN == USB_CFG_BIG
            /* Big-endian */
            uint8_t b_request;       /* Characteristics of request */
            uint8_t bm_request_type;      /* Specific request */
#else /* USB_CFG_ENDIAN == USB_CFG_BIG */
            /* Little-endian */
            uint8_t bm_request_type;      /* Characteristics of request */
            uint8_t b_request;           /* Specific request */
#endif /* USB_CFG_ENDIAN == USB_CFG_BIG */
        } byte;
        uint16_t    w_request;       /* Control transfer request */
    } word;
    uint16_t        w_value;         /* Control transfer value */
    uint16_t        w_index;         /* Control transfer index */
    uint16_t        w_length;        /* Control transfer length */
    uint16_t        address;
} usb_hcdrequest_t;

typedef struct
{
    uint8_t      *p_devicetbl;      /* Device descriptor Table address */
    uint8_t      *p_configtbl;      /* Configuration descriptor Table address */
    uint8_t      **pp_stringtbl;     /* String descriptor Table address */
    usb_pcb_t     devdefault;       /* Device default */
    usb_pcb_t     devconfig;        /* Device configured */
    usb_pcb_t     devdetach;        /* Device detach */
    usb_pcb_t     devsuspend;       /* Device suspend */
    usb_pcb_t     devresume;        /* Device resume */
    usb_pcb_t     interface;        /* Interface changed */
    usb_cb_trn_t ctrltrans;         /* Control Transfer */
    uint8_t      num_string;        /* Num entry String Descriptor */
} usb_pcdreg_t;

typedef struct
{
    usb_port_t      rootport;       /* Root port */
    usb_addr_t      devaddr;        /* Device address */
    uint8_t         devstate;       /* Device state */
    uint8_t         ifclass;        /* Interface Class */
    usb_cbcheck_t   classcheck;     /* Driver check */
    usb_cbinfo_t    statediagram;   /* Device status */
} usb_hcdreg_t;

typedef struct
{
    usb_strct_t     msginfo;        /* Message Info for F/W */
    usb_strct_t     inttype;        /* Interrupt type */
    uint16_t        bitsts;         /* Fifo buffer pipe status */
    usb_strct_t     dummy;          /* not used */
} usb_intinfo_t;

typedef struct
{
    usb_strct_t     msginfo;        /* Message Info for F/W */
    usb_strct_t     keyword;        /* Rootport / Device address / Pipe number */
    uint16_t        status;         /* status */
    usb_strct_t     flag;           /* Message enable/disable */
    usb_cbinfo_t    complete;       /* Device status */
} usb_tskinfo_t;

typedef struct
{
    usb_addr_t      port;           /* root port */
    uint8_t         state;          /* Device state */
    uint8_t         config;         /* Configuration number */
    uint8_t         speed;          /* Device speed */
} usb_hcddevinf_t;

typedef struct
{
    uint8_t         dcpmode;        /* DCP Mode Flag */
    uint8_t         state;          /* BC State */
    uint8_t         pd_detect;      /* PD Detect Flag */
} usb_bc_status_t;

typedef struct usb_ctrl_trans
{
    usb_setup_t setup;              /* Request command */
    uint16_t    address;            /* Device address setting */
} usb_ctrl_trans_t;

typedef struct
{
    uint16_t    type;
    uint16_t    status;
} usb_int_info_t;

typedef struct
{
    usb_int_info_t  buf[USB_INT_BUFSIZE];   /* Interrupt Info */
    uint8_t         wp;             /* Write pointer */
    uint8_t         rp;             /* Read pointer */
} usb_int_t;

typedef struct
{
    uint16_t        fifo_type[USB_INT_BUFSIZE]; /* Use FIFO type(USB_CUSE/USB_D0USE/USB_D1USE...) */
    uint8_t         wp;             /* Write pointer */
    uint8_t         rp;             /* Read pointer */
} usb_dma_int_t;

typedef struct{
    union {
        uint16_t    w;
        struct {
#if USB_CFG_ENDIAN == USB_CFG_BIG
            /* Big-endian */
            uint8_t up;
            uint8_t dn;
#else /* USB_CFG_ENDIAN == USB_CFG_BIG */
            /* Little-endian */
            uint8_t dn;
            uint8_t up;
#endif /* USB_CFG_ENDIAN == USB_CFG_BIG */
        } byte;
    } word;
} status_t;

R_BSP_PRAGMA_UNPACK
typedef struct{
    union{
        uint8_t b;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_8(
            uint8_t brdy:1,
            uint8_t nrdy:1,
            uint8_t bemp:1,
            uint8_t ctrt:1,
            uint8_t dvst:1,
            uint8_t sofr:1,
            uint8_t resm:1,
            uint8_t vbint:1
        ) bit;
    } byte;
} intbit_t;
R_BSP_PRAGMA_PACKOPTION

R_BSP_PRAGMA_UNPACK
typedef struct{
    union{
        uint16_t    w;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_12(
            uint16_t pddetint:1,
            uint16_t :3,
            uint16_t sack:1,
            uint16_t sign:1,
            uint16_t eoferr:1,
            uint16_t :1,
            uint16_t :3,
            uint16_t attch:1,
            uint16_t dtch:1,
            uint16_t :1,
            uint16_t bchg:1,
            uint16_t ovrcr:1
        ) bit;
    } word;
} intbit2_t;
R_BSP_PRAGMA_PACKOPTION

typedef struct{
    union {
        uint16_t    w;
        struct {
#if USB_CFG_ENDIAN == USB_CFG_BIG
            /* Big-endian */
            uint8_t up;
            uint8_t dn;
#else /* USB_CFG_ENDIAN == USB_CFG_BIG */
            /* Little-endian */
            uint8_t dn;
            uint8_t up;
#endif /* USB_CFG_ENDIAN == USB_CFG_BIG */
        } byte;
    } word;
} totallen_t;

typedef struct usb_event
{
    uint8_t         write_pointer;       /* Write pointer */
    uint8_t         read_pointer;        /* Read pointer */
    usb_status_t    code[USB_EVENT_MAX]; /* Event code */
    usb_ctrl_t      ctrl[USB_EVENT_MAX]; /* Control Information */
} usb_event_t;

typedef struct usb_pipe_table
{
    uint16_t    use_flag;
    uint16_t    pipe_cfg;
    uint16_t    pipe_maxp;
    uint16_t    pipe_peri;
} usb_pipe_table_t;

/******************************************************************************
 Constant macro definitions
******************************************************************************/


#endif /* R_USB_TYPEDEF_H */
/******************************************************************************
End of file
******************************************************************************/
