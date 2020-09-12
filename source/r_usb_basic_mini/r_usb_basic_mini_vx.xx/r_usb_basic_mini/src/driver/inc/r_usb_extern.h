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
 ******************************************************************************/
/*******************************************************************************
 * File Name    : r_usb_extern.h
 * Description  : USB common extern header
 ******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*           : 30.06.2019 1.12    RX23W is added.
*******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
 
/*******************************************************************************
 Macro definitions
 ******************************************************************************/
#ifndef R_USB_EXTERN_H
#define R_USB_EXTERN_H


/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables
 ******************************************************************************/
/* r_usb_usbif_api.c */
extern uint16_t         g_usb_cstd_usb_mode;                         /* USB mode HOST/PERI */
extern uint32_t         g_usb_read_request_size[USB_MAXPIPE_NUM + 1];
extern uint16_t         g_usb_cstd_open_class;
extern usb_event_t      g_usb_cstd_event;


/* r_usb_cdataio.c */
extern usb_pipe_table_t  g_usb_cstd_pipe_tbl[USB_MAXPIPE_NUM+1];
extern uint16_t     g_usb_cstd_bemp_skip[USB_MAX_PIPE_NO + 1u];

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
/* r_usb_cdataio.c */
extern usb_putr_t       *gp_usb_pstd_pipe[USB_MAX_PIPE_NO + 1u];
extern uint32_t          g_usb_pstd_data_cnt[USB_MAX_PIPE_NO + 1u]; /* PIPEn Buffer counter */
extern uint8_t          *gp_usb_pstd_data[USB_MAX_PIPE_NO + 1u];    /* PIPEn Buffer pointer(8bit) */

/* r_usb_pdriver.c */
extern uint16_t     g_usb_pstd_is_stall_pipe[USB_MAX_PIPE_NO + 1u]; /* Stall Pipe info */
extern usb_pcb_t    g_usb_pstd_stall_cb;      /* Stall Callback function */
extern uint16_t     g_usb_pstd_config_no;     /* Configuration Number */
extern uint16_t     g_usb_pstd_alt_no[];      /* Alternate */
extern uint16_t     g_usb_pstd_remote_wakeup; /* Remote Wake up Enable Flag */
extern uint16_t     g_usb_pstd_remote_wakeup_state;                /* Result for Remote wake up */
extern uint16_t     g_usb_pstd_eptbl_index[2][USB_MAX_EP_NO + 1u]; /* Index of Endpoint Information table */
extern uint16_t     g_usb_pstd_req_type;     /* Request type */
extern uint16_t     g_usb_pstd_req_value;    /* Value */
extern uint16_t     g_usb_pstd_req_index;    /* Index */
extern uint16_t     g_usb_pstd_req_length;   /* Length */
extern usb_pcdreg_t g_usb_pstd_driver;       /* Driver registration */
extern usb_setup_t  g_usb_pstd_req_reg;      /* Request variable */
extern usb_int_t    g_usb_pstd_usb_int;
extern uint16_t     g_usb_pstd_pipe0_request;
extern uint16_t     g_usb_pstd_std_request;
extern uint16_t     g_usb_pstd_is_connected; /* Peri CDC application enable */


/* r_usb_usbif_api.c */
extern usb_putr_t       g_usb_pstd_data[USB_MAXPIPE_NUM + 1];

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */

#if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST
/* r_usb_hstdfunction.c */
extern usb_pipe_no_t    g_usb_hstd_current_pipe;               /* Pipe number */
extern usb_hutr_t      *gp_usb_hstd_pipe[];                    /* Message pipe */
extern usb_leng_t       g_usb_hstd_data_cnt[];                 /* PIPEn Buffer counter */
extern uint8_t         *gp_usb_hstd_data_ptr[];                /* PIPEn Buffer pointer(8bit) */
extern usb_intinfo_t    g_usb_hstd_int_msg[];
extern uint8_t          g_usb_hstd_int_msg_cnt;
extern usb_addr_t       g_usb_hcd_devaddr;                     /* Device address */
extern uint8_t          g_usb_hcd_ctsq;                        /* Control transfer stage management */
extern usb_port_t       g_usb_hstd_hcd_port;
extern usb_hcdreg_t     g_usb_hcd_driver;                      /* Device driver (registration) */
extern usb_hcddevinf_t  g_usb_hcd_dev_info;                    /* device address, status, config, speed, */
extern uint8_t          g_usb_hcd_remote_wakeup;
extern usb_addr_t       g_usb_mgr_devaddr;                     /* Device address */
extern uint8_t          g_usb_mgr_seq;
extern uint8_t          g_usb_mgr_seq_mode;
extern usb_tskinfo_t   *gp_usb_hstd_hcd_message;               /* Hcd Task receive message */
extern uint16_t         g_usb_hstd_hcd_dcp_mxps[USB_MAXDEVADDR+1u];    /* DEVSEL & DCPMAXP (Multiple device) */
/* r_usb_hstdfunction.c scheduler */
extern usb_msg_t       *g_usb_hstd_msg_tbl_sch[][USB_TABLEMAX];
extern uint8_t          g_usb_hstd_read_sch[];
extern uint8_t          g_usb_hstd_write_sch[];
extern usb_tskinfo_t    g_usb_hstd_block[];
extern usb_waitinfo_t   g_usb_hstd_wait_msg_sch[];
extern usb_waitque_t    g_usb_hstd_wait_que_sch[];
extern usb_msg_t       *gp_usb_hstd_next_addr_sch;
extern uint8_t          g_usb_hstd_next_id_sch;                /* Priority Counter */
extern uint8_t          g_usb_next_read_ptr_sch;               /* Priority Table Reading pointer */

/* r_usb_hdriver.c */
extern usb_port_t       g_usb_mgr_port;
extern uint8_t          g_usb_hstd_devspd;                     /* Reset handshake result */
extern uint8_t          g_usb_hstd_cfg_descriptor[USB_CONFIGSIZE];
extern usb_hutr_t       g_usb_mgr_ctrl_msg;
extern usb_tskinfo_t   *gp_usb_mgr_msg;                        /* Mgr Task receive message */
extern uint16_t         g_usb_enum_seq;                        /* Enumeration Sequence */
extern usb_hcdrequest_t g_usb_hstd_request;                    /* Control Transfer Request field */

/* r_usb_usbif_api.c */
extern uint16_t         g_usb_hstd_is_susp_resm;
extern usb_hutr_t       g_usb_hstd_data[USB_MAXPIPE_NUM + 1];

extern uint16_t         g_usb_hstd_pipe_request[USB_MAX_PIPE_NO + 1u];


#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

#if USB_CFG_BC == USB_CFG_ENABLE
#if (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI
/* r_usb_pbc.c */
extern uint16_t        g_usb_pstd_bc_detect;
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/* r_usb_hbc.c */
extern usb_bc_status_t  g_usb_hstd_bc;
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */


#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
extern uint16_t         g_usb_hstd_response_counter;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

#if defined(USB_CFG_HMSC_USE)
extern uint8_t          g_usb_hmsc_sub_class;
#endif /* defined(USB_CFG_HMSC_USE) */

#if defined(USB_CFG_HHID_USE)
extern uint8_t         *gp_usb_hstd_interface_tbl;
#endif /* defined(USB_CFG_HMSC_USE) */

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
/* r_usb_rx_mcu.c */
extern usb_err_t usb_module_start (void);
extern usb_err_t usb_module_stop (void);
extern void      usb_cpu_delay_xms (uint16_t time);
extern void      usb_cpu_delay_1us (uint16_t time);
extern void      usb_cpu_usbint_init (void);
extern uint16_t  usb_chattering (uint16_t *p_syssts);
extern void      usb_cpu_int_enable(void);
extern void      usb_cpu_int_disable(void);

/* r_usb_dmac.h */
extern void      usb_cstd_dma_stop(uint16_t fifo_mode);

/* r_usb_cdataio.c */
extern uint8_t   usb_cstd_get_usepipe (usb_ctrl_t *p_ctrl, uint8_t dir);
extern usb_er_t  usb_cstd_data_read (usb_ctrl_t *p_ctrl, uint8_t  *p_buf, uint32_t size);
extern usb_er_t  usb_cstd_ctrl_receive (usb_ctrl_t *p_ctrl, uint8_t  *p_buf, uint32_t size);
extern usb_er_t  usb_cstd_data_write (usb_ctrl_t *p_ctrl, uint8_t  *p_buf, uint32_t size);
extern usb_er_t  usb_cstd_ctrl_send (usb_ctrl_t *p_ctrl, uint8_t  *p_buf, uint32_t size);
extern usb_er_t  usb_cstd_data_stop (usb_ctrl_t *p_ctrl, uint16_t type);
extern usb_er_t  usb_cstd_ctrl_stop (void);
extern void      usb_cstd_debug_hook (uint16_t error_code);
extern void      usb_cstd_pipe_reg_set (void);

/* r_usb_creg_abs.c */
extern uint16_t  usb_cstd_is_set_frdy (uint16_t pipe, uint16_t fifosel, uint16_t isel);
extern void      usb_cstd_chg_curpipe (uint16_t pipe, uint16_t fifosel, uint16_t isel);
extern void      usb_cstd_set_transaction (uint16_t pipe, uint16_t trncnt);
extern void      usb_cstd_clr_transaction (uint16_t pipe);
extern void      usb_cstd_set_pipe_config (uint16_t pipe);
extern void      usb_cstd_clr_pipe_config (uint16_t pipeno);
extern uint16_t  usb_cstd_get_maxpacket_size (uint16_t pipe);

/* r_usb_clibusbip.c */
extern void      usb_cstd_nrdy_enable (uint16_t pipe);
extern uint16_t  usb_cstd_port_speed (void);
extern uint16_t  usb_cstd_get_pipe_dir (uint16_t pipe);
extern uint16_t  usb_cstd_get_pipe_type (uint16_t pipe);
extern void      usb_cstd_do_aclrm (uint16_t pipe);
extern void      usb_cstd_set_buf (uint16_t pipe);
extern void      usb_cstd_clr_stall (uint16_t pipe);
extern void      usb_cstd_usb_task (void);
extern void      usb_cstd_set_event (usb_status_t event, usb_ctrl_t *p_ctrl);
extern void      usb_cstd_set_nak (uint16_t pipe);
extern void      usb_cstd_pipe_table_clear (void);
extern void      usb_cstd_pipe_reg_clear (void);
extern uint16_t  usb_cstd_pipe_to_fport (uint16_t pipe);
extern uint16_t  usb_cstd_get_pid (uint16_t pipe);


#if defined(USB_CFG_HMSC_USE)
extern uint16_t         usb_hmsc_pipe_info(uint8_t *table, uint16_t length);
extern void             usb_hmsc_process (usb_tskinfo_t *mess);
#endif /* defined(USB_CFG_HMSC_USE) */




#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

/* r_usb_plibusbip.c */
extern void      usb_pstd_send_start (uint16_t pipe);
extern void      usb_pstd_buf_to_fifo (uint16_t pipe, uint16_t  useport);
extern uint16_t  usb_pstd_write_data (uint16_t pipe, uint16_t pipemode);
extern void      usb_pstd_receive_start (uint16_t pipe);
extern void      usb_pstd_fifo_to_buf (uint16_t pipe, uint16_t  useport);
extern uint16_t  usb_pstd_read_data (uint16_t pipe, uint16_t pipemode);
extern void      usb_pstd_data_end (uint16_t pipe, uint16_t status);
extern uint8_t   usb_pstd_set_pipe_table (uint8_t  *p_descriptor);
extern void      usb_pstd_set_pipe_reg (void);
extern uint8_t   usb_pstd_get_pipe_no (uint8_t type, uint8_t dir);
extern uint16_t  usb_pstd_epadr_to_pipe (uint16_t dir_ep);
extern void      usb_pstd_nrdy_pipe_process(uint16_t bitsts);
extern void      usb_pstd_bemp_pipe_process(uint16_t bitsts);
extern void      usb_pstd_brdy_pipe_process(uint16_t bitsts);


/* r_usb_preg_abs.c */
extern uint8_t  *usb_pstd_write_fifo (uint16_t count, uint16_t pipemode, uint8_t  * p_buff);
extern uint8_t  *usb_pstd_read_fifo (uint16_t count, uint16_t pipemode, uint8_t  * p_buff);
extern void      usb_pstd_forced_termination (uint16_t pipe, uint16_t status);
extern void      usb_pstd_interrupt_clock (void);
extern void      usb_pstd_self_clock (void);
extern void      usb_pstd_stop_clock (void);

/* r_usb_pinthandler_usbip0.c */
extern void      usb_pstd_usb_handler (void);

/* r_usb_pdriver.c */
extern void      usb_pstd_pcd_task (void);
extern usb_er_t  usb_pstd_set_submitutr (usb_putr_t * p_utrmsg);
extern void      usb_pstd_clr_alt (void);
extern void      usb_pstd_clr_mem (void);
extern void      usb_pstd_set_config_no (uint16_t Value);
extern void      usb_pstd_clr_eptbl_index (void);
extern uint16_t  usb_pstd_get_interface_no (void);
extern uint16_t  usb_pstd_get_alternate_no (uint16_t int_num);
extern void      usb_pstd_set_eptbl_index (uint16_t int_num, uint16_t alt_num);
extern uint16_t  usb_pstd_chk_remote (void);
extern uint8_t   usb_pstd_get_current_power (void);
extern usb_er_t  usb_pstd_transfer_start (usb_putr_t *p_utr);
extern usb_er_t  usb_pstd_transfer_end (uint16_t pipe);
extern void      usb_pstd_change_device_state (uint16_t state, uint16_t keyword, usb_pcb_t complete);
extern void      usb_pstd_driver_registration (usb_pcdreg_t *p_callback);
extern void      usb_pstd_driver_release (void);
extern void      usb_pstd_dummy_function (usb_putr_t *p_utr, uint16_t data1, uint16_t data2);
extern void      usb_pstd_dummy_trn (usb_setup_t * p_req, uint16_t ctsq);

/* r_usb_pcontrolrw.c */
extern uint16_t  usb_pstd_ctrl_read (uint32_t bsize, uint8_t  *p_table);
extern void      usb_pstd_ctrl_write (uint32_t bsize, uint8_t  *p_table);
extern void      usb_pstd_ctrl_end (uint16_t status);

/* r_usb_pintfifo.c */
extern void      usb_pstd_brdy_pipe (uint16_t bitsts);
extern void      usb_pstd_nrdy_pipe (uint16_t bitsts);
extern void      usb_pstd_bemp_pipe (uint16_t bitsts);

/* r_usb_pstdfunction.c */
extern void      usb_pstd_busreset_function (void);
extern void      usb_pstd_suspend_function (void);
extern void      usb_pstd_driver_init(usb_cfg_t *p_cfg);
extern uint8_t   usb_pstd_get_open_class (void);

/* r_usb_preg_abs.c */
extern void      usb_pstd_interrupt_handler (uint16_t * p_type, uint16_t * p_status);
extern void      usb_pstd_save_request (void);
extern uint16_t  usb_pstd_chk_configured (void);
extern void      usb_pstd_bus_reset (void);
extern void      usb_pstd_remote_wakeup (void);
extern void      usb_pstd_attach_process (void);
extern void      usb_pstd_detach_process (void);
extern void      usb_pstd_suspend_process (void);
extern void      usb_pstd_resume_process (void);
extern uint16_t  usb_pstd_chk_vbsts (void);
extern void      usb_pstd_set_stall (uint16_t pipe);
extern void      usb_pstd_set_stall_pipe0 (void);

/* r_usb_pstdrequest.c */
extern void      usb_pstd_stand_req0 (void);
extern void      usb_pstd_stand_req1 (void);
extern void      usb_pstd_stand_req2 (void);
extern void      usb_pstd_stand_req3 (void);
extern void      usb_pstd_stand_req4 (void);
extern void      usb_pstd_stand_req5 (void);
extern void      usb_pstd_set_feature_function (void);

/* peri_processing.c */
extern void      usb_pstd_registration(usb_cfg_t *p_cfg);
extern void      usb_pstd_devdefault (usb_putr_t *p_utr, uint16_t mode, uint16_t data2);
extern uint16_t  usb_pstd_pipe_info (uint8_t  *p_table, uint16_t length);
extern void      usb_pstd_configured (usb_putr_t *p_utr, uint16_t data1, uint16_t data2);
extern void      usb_pstd_detach (usb_putr_t *p_utr, uint16_t data1, uint16_t data2);
extern void      usb_pstd_suspended(usb_putr_t *p_utr, uint16_t data1, uint16_t data2);
extern void      usb_pstd_resume(usb_putr_t *p_utr, uint16_t data1, uint16_t data2);
extern void      usb_pstd_interface(usb_putr_t *p_utr, uint16_t data1, uint16_t data2);
extern void      usb_pstd_class_request(usb_setup_t *p_req, uint16_t ctsq);
extern void      usb_pstd_class_request_ioss (usb_setup_t *p_req);
extern void      usb_pstd_class_request_rwds (usb_setup_t * p_req);
extern void      usb_pstd_class_request_wds (usb_setup_t * p_req);
extern void      usb_pstd_other_request (usb_setup_t *p_req);
extern void      usb_pstd_class_request_wnss (usb_setup_t *p_req);
extern void      usb_pstd_class_request_rss (usb_setup_t *p_req);
extern void      usb_pstd_class_request_wss (usb_setup_t *p_req);




#if USB_CFG_BC == USB_CFG_ENABLE
/* r_usb_hbc.c */
extern    void      usb_pstd_bc_detect_process(void);
#endif /* USB_CFG_BC == USB_CFG_ENABLE */



#endif/* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */

#if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST

/* r_usb_hreq_abs.c */
extern void             usb_hstd_check_interrupt_source(usb_strct_t *p_inttype, uint16_t *p_bitsts);
extern void             usb_hstd_forced_termination(uint16_t status);
extern void             usb_hstd_interrupt_handler(void);

/* r_usb_hsignal.c */
extern void             usb_hstd_detach_control(void);
extern void             usb_hstd_attach_control(void);
extern void             usb_hstd_int_disable(void);
extern void             usb_hstd_interrupt_process(void);
extern void             usb_hstd_vbus_control_on(void);
extern void             usb_hstd_vbus_control_off(void);
extern void             usb_hstd_suspend_process(void);
extern usb_strct_t      usb_hstd_port_speed(void);
extern void             usb_hstd_set_hw_function(void);

/* r_usb_hstdfunction.c */
extern uint8_t          usb_hstd_check_device_address(void);
extern usb_addr_t       usb_hstd_pipe_to_addr(void);
extern uint8_t          usb_hstd_enumeration(void);
extern void             usb_hstd_update_devinfo(uint8_t state);
extern void             usb_hstd_update_devstate(uint8_t state);
extern usb_port_t       usb_hstd_devaddr_to_port(void);
extern usb_er_t         usb_hstd_get_descriptor(uint16_t CntValue);
extern usb_er_t         usb_hstd_set_configuration(void);
extern void             usb_hclass_request_complete (usb_hutr_t *p_mess);
extern void             usb_hstd_sw_reset(void);
extern void             usb_hstd_class_check(uint8_t **pp_table);
extern void             usb_hstd_class_enumeration(usb_tskinfo_t *p_mess);
extern void             usb_hstd_hw_start(void);
extern void             usb_hstd_status_notification(usb_strct_t msginfo, usb_strct_t keyword);
extern void             usb_hstd_setup_disable(void);
extern void             usb_hstd_port_enable(void);

extern void             usb_hstd_class_task (void);
extern void             usb_hstd_read_complete (usb_hutr_t *p_mess);
extern void             usb_hstd_write_complete (usb_hutr_t *p_mess);
extern void             usb_hstd_suspend_complete (uint16_t data1, uint16_t data2);
extern void             usb_hstd_resume_complete (uint16_t data1, uint16_t data2);
extern void             usb_hstd_dummy_function(usb_hutr_t *p_utr, uint16_t data1, uint16_t data2);
extern void             usb_hstd_driver_reg_init(void);  /* Initialized g_usb_HcdDriver */
extern void             usb_hstd_clr_device_info(void);     /* Initialized g_usb_HcdDevInfo */
extern void             usb_hstd_pipe_to_hcddevaddr(void);
extern void             usb_hstd_connect_err_event_set(void);
extern void             usb_hstd_set_devaddx_register(uint8_t usbspd);
extern void             usb_hstd_buf_to_fifo(uint16_t useport);
extern void             usb_hstd_fifo_to_buf(void);
extern void             usb_hstd_control_read_start(void);
extern uint16_t         usb_hstd_control_write_start(void);
extern void             usb_hstd_status_start(void);
extern void             usb_hstd_setup_start(void);
extern void             usb_hstd_control_end(usb_strct_t status);
extern void             usb_cstd_init_library(void);
extern uint8_t          usb_hstd_devaddr_to_speed(void);
extern uint8_t          usb_hstd_check_attach(void);


/* r_usb_creg_abs.c */
extern void             usb_hstd_bus_reset(void);
extern void             usb_hstd_detach_process(void);
extern void             usb_hstd_attach_process(void);
extern void             usb_hstd_resume_process(void);
extern void             usb_hstd_read_lnst(uint16_t *p_buf);
extern void             usb_hstd_bemp_pipe(void);
extern void             usb_hstd_nrdy_pipe(void);
extern void             usb_hstd_brdy_pipe(void);
extern usb_er_t         usb_hstd_set_feature(uint16_t epnum);
extern usb_er_t         usb_hstd_clear_feature(uint16_t epnum);
extern usb_er_t         usb_hstd_status_change(usb_strct_t msginfo, usb_strct_t keyword);
extern void             usb_hstd_status_result(usb_strct_t status);
extern void             usb_hstd_transferend_check(usb_strct_t status);
extern uint8_t          usb_hstd_pipe_to_epadr(void);

/* r_usb_hlibusbip.c */
extern void             usb_hstd_send_start(void);
extern void             usb_hstd_receive_start(void);
extern uint8_t          usb_cstd_pipe_table_set (uint16_t usb_class, uint8_t *p_descriptor);
extern void             usb_hstd_data_end(uint16_t status);
extern void             usb_hstd_bchg_enable(void);
extern void             usb_hstd_bchg_disable(void);
extern void             usb_hstd_set_uact(void);
extern void             usb_hstd_ovrcr_enable(void);
extern void             usb_hstd_ovrcr_disable(void);
extern void             usb_hstd_attch_clear_status(void);
extern void             usb_hstd_detch_clear_status(void);
extern void             usb_hstd_attch_disable(void);
extern void             usb_hstd_detach_enable(void);
extern void             usb_hstd_detach_disable(void);
extern void             usb_hstd_bus_int_disable(void);
extern void             usb_hstd_host_registration (void);
extern void             usb_hstd_class_driver_start (void);
extern uint8_t          usb_hstd_get_pipe_no (uint16_t usb_class, uint8_t type, uint8_t dir);
extern uint16_t         usb_hstd_get_pipe_peri_value (uint8_t binterval);
extern void             usb_hstd_brdy_pipe_process(uint16_t bitsts);
extern void             usb_hstd_nrdy_pipe_process(uint16_t bitsts);
extern void             usb_hstd_bemp_pipe_process(uint16_t bitsts);


/* r_usb_hdriver.c */
extern void             usb_hstd_hcd_task(void);
extern void             usb_hstd_mgr_task(void);

/* r_usb_usbif_api.c  */
extern void             usb_hstd_driver_init(void);

/* r_usb_hdriverapi.c */
extern void        usb_hstd_driver_registration(usb_hcdreg_t *p_registinfo);
extern void        usb_hstd_driver_release(uint8_t devclass);
extern usb_er_t    usb_hstd_transfer_start(usb_hutr_t *p_utr_table);
extern usb_er_t    usb_hstd_change_device_state(usb_cbinfo_t complete, usb_strct_t msginfo, usb_strct_t keyword);
extern void        usb_hstd_return_enu_mgr(uint16_t cls_result);

/* r_usb_hscheduler.c */
extern uint8_t     usb_hstd_scheduler(void);
extern usb_er_t    usb_hstd_rec_msg( uint8_t id, usb_msg_t** pp_mess );
extern usb_er_t    usb_hstd_snd_msg( uint8_t id, usb_msg_t* p_mess );
extern usb_er_t    usb_hstd_isnd_msg( uint8_t id, usb_msg_t* p_mess );
extern usb_er_t    usb_hstd_wai_msg( uint8_t id, usb_msg_t* p_mess, uint16_t count );
extern usb_er_t    usb_hstd_pget_send( uint8_t id, usb_strct_t msginfo, usb_cbinfo_t complete, usb_strct_t keyword );
extern usb_er_t    usb_hstd_rel_blk( uint8_t blk_num );



#if USB_CFG_BC == USB_CFG_ENABLE
/* r_usb_hbc.c */

extern    void      usb_hstd_pddetint_process(void);
#endif /* USB_CFG_BC == USB_CFG_ENABLE */



#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */




#if defined(USB_CFG_HVND_USE)
extern void        usb_hvnd_registration (void);
extern uint16_t    usb_hvnd_pipe_info(uint8_t *table, uint16_t length);
#endif /* defined(USB_CFG_HVND_USE) */

#if defined(USB_CFG_HHID_USE)
extern void        hid_registration (void);
extern uint16_t    usb_hhid_pipe_info(uint8_t *table, uint16_t length);
#endif /* defined(USB_CFG_HHID_USE) */

#if defined(USB_CFG_HCDC_USE)
extern void        cdc_registration(void);                        /* Sample driver registration */
extern void        set_control_line_state(usb_ctrl_t *p_ctrl);
extern void        get_line_coding(usb_ctrl_t *p_ctrl);
extern void        usb_hcdc_device_state(uint16_t data, uint16_t state);
extern uint16_t    usb_hcdc_pipe_info(uint8_t *table, uint16_t length);
#endif /* defined(USB_CFG_HCDC_USE) */

#if defined(USB_CFG_HMSC_USE)
extern void        usb_hmsc_strg_cmd_complete(usb_tskinfo_t   *p_mess);
extern uint16_t    R_USB_HmscStrgDriveSearch (usb_hutr_t *ptr, uint16_t addr, usb_hcb_t complete);
extern void        msc_registration( void );
#endif /* defined(USB_CFG_HMSC_USE) */

#if defined(USB_CFG_PVND_USE)
extern void        usb_pstd_read_complete(usb_putr_t *p_mess, uint16_t data1, uint16_t data2);
extern void        usb_pstd_write_complete(usb_putr_t *p_mess, uint16_t data1, uint16_t data2);
#endif /* defined(USB_CFG_PVND_USE) */

#if defined(USB_CFG_PCDC_USE)
extern void        usb_pcdc_read_complete( usb_putr_t *p_mess, uint16_t data1, uint16_t data2);
extern void        usb_pcdc_write_complete( usb_putr_t *p_mess, uint16_t data1, uint16_t data2);
#endif /* defined(USB_CFG_PCDC_USE) */

#if defined(USB_CFG_PHID_USE)
extern void        usb_phid_read_complete(usb_putr_t *p_mess, uint16_t data1, uint16_t data2);
extern void        usb_phid_write_complete(usb_putr_t *p_mess, uint16_t data1, uint16_t data2);
#endif /* defined(USB_CFG_PHID_USE) */

#if defined(USB_CFG_PMSC_USE)
extern void        usb_pmsc_task (void);

#endif /* defined(USB_CFG_PMSC_USE) */

#if defined(USB_CFG_PMSC_USE)
extern void      usb_pmsc_receive_cbw (void);
extern void      usb_pmsc_get_max_lun (uint16_t value, uint16_t index, uint16_t length);
extern void      usb_pmsc_mass_strage_reset (uint16_t value, uint16_t index, uint16_t length);
#endif

#if USB_CFG_BC == USB_CFG_ENABLE
extern void       (*gp_usb_hstd_bc_func[USB_BC_STATE_MAX][USB_BC_EVENT_MAX])(void);
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */


#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
void      usb_compliance_disp (void *);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */


#endif  /* R_USB_EXTERN_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/
