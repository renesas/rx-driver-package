/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : r_sdc_sdmem_rx_lib.c
 *
 ***************************************************************************/

#include "cri_aero_config_ais.h"

#if CRI_AERO_CONFIG_AIS_USE_SD

#include "r_sdc_sdmem_rx_lib.h"
#include "platform.h"
#include "r_sdhi_rx_pinset.h"
#include "r_sdc_sd_rx_if.h"
#include "r_sdc_sd_rx_config.h"
#include "r_sdc_sdmem_rx_pin_config.h"
#include "r_sdc_sdmem_rx_pin.h"
#include "r_cmt_rx_if.h"
#ifdef SDC_SD_CFG_LONGQ_ENABLE
#include "r_longq_if.h"
#endif  

#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
#include "r_dmaca_rx_if.h"
#endif 

#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
#include "r_dtc_rx_if.h"
#endif 


#ifdef SDC_SD_CFG_LONGQ_ENABLE
#define SDC_SD_USER_LONGQ_MAX           (8)                         
#define SDC_SD_USER_LONGQ_BUFSIZE       (SDC_SD_USER_LONGQ_MAX * 4) 
#define SDC_SD_USER_LONGQ_IGN_OVERFLOW  (1)                         
#define USER_DRIVER_ID                  (1)
#define USER_LOG_MAX                    (63)
#define USER_LOG_ADR_MAX                (0x00001fff)
#endif  



#define SDC_SD_TEST_WRITE_OP            (0)
#define SDC_SD_TEST_READ_OP             (1)



static uint32_t g_sdc_sd_card_no;
static uint32_t g_cmt_channel;
static uint32_t g_sdc_sd_work[256 / sizeof(uint32_t)];
static sdc_sd_cfg_t s_sdc_sd_config;
static uint32_t s_reg_buff;


#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
volatile uint32_t g_transfer_data[4];
#endif  

#ifdef SDC_SD_CFG_LONGQ_ENABLE
uint32_t            g_sdc_sd_user_longq_buf[SDC_SD_USER_LONGQ_BUFSIZE]; 
static longq_hdl_t  p_sdc_sd_user_long_que;                             
#endif  

static void trap(void);
static void r_cmt_open(void);
static void r_cmt_close(void);
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
static void r_dmaca_open(void);
static void r_dmaca_enable(sdc_sd_access_t * p_sdc_sd_access, uint8_t op_mode, uint32_t reg_buff);
static void r_dmaca_close(void);
static void r_dmaca_disable(void);
static void r_dmaca_set_recv(dmaca_transfer_data_cfg_t *p_data_cfg, sdc_sd_access_t *sdc_sd_access, uint32_t reg_buff,
    dmaca_activation_source_t act_source);
static void r_dmaca_set_send(dmaca_transfer_data_cfg_t *p_data_cfg, sdc_sd_access_t *sdc_sd_access, uint32_t reg_buff,
    dmaca_activation_source_t act_source);
#endif  
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
static void r_dtc_open(void);
static void r_dtc_enable(sdc_sd_access_t * p_sdc_sd_access, uint8_t op_mode, uint32_t * p_reg_buff);
static void r_dtc_disable(void);
static void r_dtc_set_recv(dtc_transfer_data_cfg_t  *p_data_cfg, sdc_sd_access_t *p_sdc_sd_access, uint32_t *p_reg_buff);
static void r_dtc_set_send(dtc_transfer_data_cfg_t  *p_data_cfg, sdc_sd_access_t *p_sdc_sd_access, uint32_t *p_reg_buff);
static void r_dtc_close(void);
#endif  
#ifdef SDC_SD_CFG_LONGQ_ENABLE
static void r_longq_open(void);
static void r_longq_close(void);
#endif  

sdc_sd_status_t r_sdc_sd_callback(int32_t channel)
{
    return SDC_SD_SUCCESS;
} 

static int32_t r_sdc_sdmem_rx_lib_cd=0;

bool r_sdc_sdmem_rx_lib_get_ditection()
{
	return r_sdc_sdmem_rx_lib_cd;
}

sdc_sd_status_t r_sdc_sd_cd_callback(int32_t cd)
{
	r_sdc_sdmem_rx_lib_cd=cd;
   if (R_SDC_SD_CdInt(g_sdc_sd_card_no, SDC_SD_CD_INT_ENABLE, r_sdc_sd_cd_callback) != SDC_SD_SUCCESS)
    {
        trap();
    }
    return SDC_SD_SUCCESS;
} 

void r_cmt_callback(void * pdata)
{
    uint32_t * channel = (uint32_t *)pdata;

    if (*channel == g_cmt_channel)
    {
        R_SDC_SD_1msInterval();
    }
} 

#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
void r_dmaca_callback(void)
{
    volatile dmaca_return_t    ret_dmaca = DMACA_SUCCESS;
    dmaca_stat_t               p_stat_dmaca;

    ret_dmaca = R_DMACA_Control(DMACA_CH0, DMACA_CMD_STATUS_GET, (dmaca_stat_t*)&p_stat_dmaca);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        return;
    }

    if (true == (p_stat_dmaca.dtif_stat))
    {
        ret_dmaca = R_DMACA_Control(DMACA_CH0, DMACA_CMD_DTIF_STATUS_CLR, (dmaca_stat_t*)&p_stat_dmaca);
        R_SDC_SD_SetDmacDtcTransFlg(g_sdc_sd_card_no, SDC_SD_SET_TRANS_STOP);
    }

    if (true == (p_stat_dmaca.esif_stat))
    {
        ret_dmaca = R_DMACA_Control(DMACA_CH0, DMACA_CMD_ESIF_STATUS_CLR, (dmaca_stat_t*)&p_stat_dmaca);
    }

    return;

} 
#endif 

void trap(void)
{
    sdc_sd_status_t err_code;

    err_code = R_SDC_SD_GetErrCode(g_sdc_sd_card_no);

    R_SDC_SD_Log(0x00000001, 0x0000003f, 0x0001ffff);

    while (err_code);
} 

static void r_cmt_open(void)
{
    R_CMT_CreatePeriodic(1000, &r_cmt_callback, &g_cmt_channel);    
} 

static void r_cmt_close(void)
{
    bool ret = true;

    ret = R_CMT_Stop(g_cmt_channel);
    if (true != ret)
    {
        trap();
    }
} 

#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
static void r_dmaca_open(void)
{
    dmaca_return_t  ret_dmaca = DMACA_SUCCESS;

    R_DMACA_Init();

    ret_dmaca = R_DMACA_Open(DMACA_CH0);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        trap();
    }

    ret_dmaca = R_DMACA_Int_Callback(DMACA_CH0, r_dmaca_callback);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        trap();
    }

    ret_dmaca = R_DMACA_Int_Enable(DMACA_CH0, 10);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        trap();
    }
} 

static void r_dmaca_enable(sdc_sd_access_t * p_sdc_sd_access, uint8_t op_mode, uint32_t reg_buff)
{
    dmaca_return_t  ret_dmaca = DMACA_SUCCESS;
    dmaca_transfer_data_cfg_t   p_data_cfg_dmac;
    dmaca_stat_t                p_stat_dmaca;

    if (SDC_SD_TEST_WRITE_OP == op_mode)
    {
        r_dmaca_set_send(&p_data_cfg_dmac, p_sdc_sd_access, reg_buff, IR_SDHI_SBFAI);
    }
    else
    {
        r_dmaca_set_recv(&p_data_cfg_dmac, p_sdc_sd_access, reg_buff, IR_SDHI_SBFAI);
    }

    ret_dmaca = R_DMACA_Create(DMACA_CH0, (dmaca_transfer_data_cfg_t *)&p_data_cfg_dmac);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        trap();
    }

    ret_dmaca = R_DMACA_Control(DMACA_CH0, DMACA_CMD_ENABLE, (dmaca_stat_t*)&p_stat_dmaca);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        trap();
    }
} 

static void r_dmaca_set_recv(dmaca_transfer_data_cfg_t *p_data_cfg, sdc_sd_access_t *sdc_sd_access, uint32_t reg_buff,
    dmaca_activation_source_t act_source)
{
    p_data_cfg->transfer_mode = DMACA_TRANSFER_MODE_BLOCK;         
    p_data_cfg->repeat_block_side = DMACA_REPEAT_BLOCK_SOURCE;
    p_data_cfg->data_size = DMACA_DATA_SIZE_LWORD;             
    p_data_cfg->act_source = act_source;
    p_data_cfg->request_source = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    p_data_cfg->dtie_request = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    p_data_cfg->esie_request = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    p_data_cfg->rptie_request = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    p_data_cfg->sarie_request = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_data_cfg->darie_request = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_data_cfg->src_addr_mode = DMACA_SRC_ADDR_FIXED;              
    p_data_cfg->des_addr_mode = DMACA_DES_ADDR_INCR;
    p_data_cfg->src_addr_repeat_area = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    p_data_cfg->des_addr_repeat_area = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    p_data_cfg->interrupt_sel = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    p_data_cfg->p_src_addr = (void *)reg_buff;                  
    p_data_cfg->p_des_addr = (void *)sdc_sd_access->p_buff;     
    p_data_cfg->transfer_count = sdc_sd_access->cnt;                
    p_data_cfg->block_size = (512 / sizeof(uint32_t));
} 

static void r_dmaca_set_send(dmaca_transfer_data_cfg_t *p_data_cfg, sdc_sd_access_t *sdc_sd_access, uint32_t reg_buff,
    dmaca_activation_source_t act_source)
{
    p_data_cfg->transfer_mode = DMACA_TRANSFER_MODE_BLOCK;         
    p_data_cfg->repeat_block_side = DMACA_REPEAT_BLOCK_DESTINATION;
    p_data_cfg->data_size = DMACA_DATA_SIZE_LWORD;             
    p_data_cfg->act_source = act_source;
    p_data_cfg->request_source = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    p_data_cfg->dtie_request = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    p_data_cfg->esie_request = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    p_data_cfg->rptie_request = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    p_data_cfg->sarie_request = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_data_cfg->darie_request = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_data_cfg->src_addr_mode = DMACA_SRC_ADDR_INCR;               
    p_data_cfg->des_addr_mode = DMACA_DES_ADDR_FIXED;              
    p_data_cfg->src_addr_repeat_area = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    p_data_cfg->des_addr_repeat_area = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    p_data_cfg->interrupt_sel = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    p_data_cfg->p_src_addr = (void *)sdc_sd_access->p_buff;     
    p_data_cfg->p_des_addr = (void *)reg_buff;                  
    p_data_cfg->transfer_count = sdc_sd_access->cnt;                
    p_data_cfg->block_size = (512 / sizeof(uint32_t));
} 

static void r_dmaca_disable(void)
{
    dmaca_return_t  ret_dmaca = DMACA_SUCCESS;
    dmaca_stat_t                p_stat_dmaca;

    ret_dmaca = R_DMACA_Control(DMACA_CH0, DMACA_CMD_DISABLE, (dmaca_stat_t*)&p_stat_dmaca);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        trap();
    }
} 

static void r_dmaca_close(void)
{
    dmaca_return_t  ret_dmaca = DMACA_SUCCESS;

    ret_dmaca = R_DMACA_Close(DMACA_CH0);
    if (DMACA_SUCCESS < ret_dmaca)
    {
        trap();
    }
} 
#endif  

#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
static void r_dtc_open(void)
{
    dtc_err_t   ret_dtc = DTC_SUCCESS;

    ret_dtc = R_DTC_Open();
    if (DTC_SUCCESS != ret_dtc)
    {
        trap();
    }
} 

static void r_dtc_enable(sdc_sd_access_t * p_sdc_sd_access, uint8_t op_mode, uint32_t * p_reg_buff)
{
    dtc_activation_source_t     act_source;
    dtc_transfer_data_cfg_t     p_data_cfg;
    uint32_t                    chain_transfer_nr;
    dtc_cmd_arg_t               p_args;
    dtc_stat_t                  p_stat;
    dtc_err_t                   ret_dtc = DTC_SUCCESS;

    act_source = DTCE_SDHI_SBFAI;

    if (SDC_SD_TEST_WRITE_OP == op_mode)
    {
        r_dtc_set_send(&p_data_cfg, p_sdc_sd_access, p_reg_buff);
    }
    else
    {
        r_dtc_set_recv(&p_data_cfg, p_sdc_sd_access, p_reg_buff);
    }

    chain_transfer_nr = 0;

    ret_dtc = R_DTC_Create(act_source, (dtc_transfer_data_t *)&g_transfer_data[0],
        (dtc_transfer_data_cfg_t *)&p_data_cfg, chain_transfer_nr);
    if (DTC_SUCCESS != ret_dtc)
    {
        trap();
    }

    p_args.act_src = DTCE_SDHI_SBFAI;
    p_args.chain_transfer_nr = 0;

    ret_dtc = R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, (void *)&p_stat, (void *)&p_args);
    if (DTC_SUCCESS != ret_dtc)
    {
        trap();
    }
    ret_dtc = R_DTC_Control(DTC_CMD_DTC_START, (void *)&p_stat, (void *)&p_args);
    if (DTC_SUCCESS != ret_dtc)
    {
        trap();
    }
} 

static void r_dtc_set_recv(dtc_transfer_data_cfg_t  *p_data_cfg, sdc_sd_access_t *p_sdc_sd_access, uint32_t *p_reg_buff)
{
    p_data_cfg->transfer_mode = DTC_TRANSFER_MODE_BLOCK;           
    p_data_cfg->data_size = DTC_DATA_SIZE_LWORD;               
    p_data_cfg->src_addr_mode = DTC_SRC_ADDR_FIXED;                
    p_data_cfg->chain_transfer_enable = DTC_CHAIN_TRANSFER_DISABLE;        
    p_data_cfg->chain_transfer_mode = DTC_CHAIN_TRANSFER_CONTINUOUSLY;   
    p_data_cfg->response_interrupt = DTC_INTERRUPT_AFTER_ALL_COMPLETE;  
    p_data_cfg->repeat_block_side = DTC_REPEAT_BLOCK_SOURCE;
    p_data_cfg->dest_addr_mode = DTC_DES_ADDR_INCR;                 
    p_data_cfg->source_addr = (uint32_t)(*p_reg_buff);           
    p_data_cfg->dest_addr = (uint32_t)p_sdc_sd_access->p_buff; 
    p_data_cfg->transfer_count = p_sdc_sd_access->cnt;              
    p_data_cfg->block_size = (512 / sizeof(uint32_t));
#if defined(BSP_MCU_RX65N)
    p_data_cfg->writeback_disable = DTC_WRITEBACK_ENABLE;
    p_data_cfg->sequence_end = DTC_SEQUENCE_TRANSFER_CONTINUE;
    p_data_cfg->refer_index_table_enable = DTC_REFER_INDEX_TABLE_DISABLE;
    p_data_cfg->disp_add_enable = DTC_SRC_ADDR_DISP_ADD_DISABLE;
#endif 
} 

static void r_dtc_set_send(dtc_transfer_data_cfg_t  *p_data_cfg, sdc_sd_access_t *p_sdc_sd_access, uint32_t *p_reg_buff)
{
    p_data_cfg->transfer_mode = DTC_TRANSFER_MODE_BLOCK;            
    p_data_cfg->data_size = DTC_DATA_SIZE_LWORD;                
    p_data_cfg->src_addr_mode = DTC_SRC_ADDR_INCR;                  
    p_data_cfg->chain_transfer_enable = DTC_CHAIN_TRANSFER_DISABLE;         
    p_data_cfg->chain_transfer_mode = DTC_CHAIN_TRANSFER_CONTINUOUSLY;    
    p_data_cfg->response_interrupt = DTC_INTERRUPT_AFTER_ALL_COMPLETE;   
    p_data_cfg->repeat_block_side = DTC_REPEAT_BLOCK_DESTINATION;
    p_data_cfg->dest_addr_mode = DTC_DES_ADDR_FIXED;                 
    p_data_cfg->source_addr = (uint32_t)p_sdc_sd_access->p_buff;  
    p_data_cfg->dest_addr = (uint32_t)(*p_reg_buff);            
    p_data_cfg->transfer_count = p_sdc_sd_access->cnt;               
    p_data_cfg->block_size = (512 / sizeof(uint32_t));
#if defined(BSP_MCU_RX65N)
    p_data_cfg->writeback_disable = DTC_WRITEBACK_ENABLE;
    p_data_cfg->sequence_end = DTC_SEQUENCE_TRANSFER_CONTINUE;
    p_data_cfg->refer_index_table_enable = DTC_REFER_INDEX_TABLE_DISABLE;
    p_data_cfg->disp_add_enable = DTC_SRC_ADDR_DISP_ADD_DISABLE;
#endif 
} 

static void r_dtc_disable(void)
{
    dtc_cmd_arg_t   p_args;
    dtc_stat_t      p_stat;
    dtc_err_t       ret_dtc = DTC_SUCCESS;

    p_args.act_src = DTCE_SDHI_SBFAI;
    p_args.chain_transfer_nr = 0;
    ret_dtc = R_DTC_Control(DTC_CMD_DTC_STOP, (void *)&p_stat, (void *)&p_args);
    if (DTC_SUCCESS != ret_dtc)
    {
        trap();
    }
    ret_dtc = R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, (void *)&p_stat, (void *)&p_args);
    if (DTC_SUCCESS != ret_dtc)
    {
        trap();
    }
} 

static void r_dtc_close(void)
{
    dtc_err_t   ret_dtc = DTC_SUCCESS;

    ret_dtc = R_DTC_Close();
    if (DTC_SUCCESS < ret_dtc)
    {
        trap();
    }
} 
#endif 

#ifdef SDC_SD_CFG_LONGQ_ENABLE
static void r_longq_open(void)
{
    longq_err_t     err = LONGQ_SUCCESS;
    uint32_t        user_long_que = 0;

    err = R_LONGQ_Open(g_sdc_sd_user_longq_buf, SDC_SD_USER_LONGQ_BUFSIZE, SDC_SD_USER_LONGQ_IGN_OVERFLOW,
        &p_sdc_sd_user_long_que);
    if (LONGQ_SUCCESS != err)
    {
        trap();
    }
    user_long_que = (uint32_t)p_sdc_sd_user_long_que;
    if (R_SDC_SD_SetLogHdlAddress(user_long_que) != SDC_SD_SUCCESS)
    {
        trap();
    }
} 

static void r_longq_close(void)
{
    longq_err_t     err = LONGQ_SUCCESS;

    R_SDC_SD_Log(USER_DRIVER_ID, USER_LOG_MAX, USER_LOG_ADR_MAX);
    err = R_LONGQ_Close(p_sdc_sd_user_long_que);
    if (LONGQ_SUCCESS != err)
    {
        trap();
    }
} 
#endif 

bool r_sdc_sdmem_rx_lib_detect()
{
	uint32_t              channel = 0;
	bool ret = false;
	r_cmt_open();
	r_dmaca_open();
	g_sdc_sd_card_no = SDC_SD_CARD_NO0;
	channel = SDHI_CH0;
	if (r_sdc_sdmem_power_Initialize(g_sdc_sd_card_no) != SDC_SD_SUCCESS)
	{
		return false;
	}
	R_SDHI_PinSetInit();
	if (R_SDC_SD_Open(g_sdc_sd_card_no, channel, &g_sdc_sd_work) != SDC_SD_SUCCESS)
	{
		return false;
	}
	if (R_SDC_SD_CdInt(g_sdc_sd_card_no, SDC_SD_CD_INT_DISABLE, 0) != SDC_SD_SUCCESS)
	{
		return false;
	}

	for(int i=0;i<200;i++)
	{
		if(R_SDC_SD_GetCardDetection(g_sdc_sd_card_no) == SDC_SD_SUCCESS)
		{
			ret=true;
			break;
		}
		r_sdc_sdmem_softwaredelay(5, SDC_SD_DELAY_MILLISECS);
    }

	if (R_SDC_SD_Close(g_sdc_sd_card_no) != SDC_SD_SUCCESS)
	{
		return false;
	}
	R_SDHI_PinSetEnd();
	r_cmt_close();
	r_dmaca_close();

	return ret;
}


bool r_sdc_sdmem_rx_lib_Initialize()
{
    sdc_sd_card_reg_t     sdhi_CardReg;
    sdc_sd_card_status_t  sdhi_CardStatus;
    uint32_t              channel = 0;

    r_cmt_open();
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
    r_dmaca_open();
#endif 
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
    r_dtc_open();
#endif 
#ifdef SDC_SD_CFG_LONGQ_ENABLE
    r_longq_open();
#endif 

    g_sdc_sd_card_no = SDC_SD_CARD_NO0;
    channel = SDHI_CH0;

    if (r_sdc_sdmem_power_Initialize(g_sdc_sd_card_no) != SDC_SD_SUCCESS)
    {
        return false;
    }

    R_SDHI_PinSetInit();

    if (R_SDC_SD_Open(g_sdc_sd_card_no, channel, &g_sdc_sd_work) != SDC_SD_SUCCESS)
    {
        return false;
    }

#if (SDHI_CFG_CH0_CD_ACTIVE)
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_HWINT)
    if (R_SDC_SD_CdInt(g_sdc_sd_card_no, SDC_SD_CD_INT_ENABLE, r_sdc_sd_cd_callback) != SDC_SD_SUCCESS)
    {
        return false;
    }
#endif 

    for(int i=0;i<100;i++){
		if(R_SDC_SD_GetCardDetection(g_sdc_sd_card_no) == SDC_SD_SUCCESS)
		{
		    r_sdc_sdmem_rx_lib_cd=1;
			break;
		}
	    if (true != r_sdc_sdmem_softwaredelay(5, SDC_SD_DELAY_MILLISECS))
		{
	    	return false;
		}
    }

#endif 

    if (R_SDC_SD_IntCallback(g_sdc_sd_card_no, r_sdc_sd_callback) != SDC_SD_SUCCESS)
    {
        return false;
    }

    if (r_sdc_sdmem_power_on(g_sdc_sd_card_no) != SDC_SD_SUCCESS)
    {
        return false;
    }

    R_SDHI_PinSetTransfer();

    s_sdc_sd_config.mode = SDC_SD_CFG_DRIVER_MODE;
    s_sdc_sd_config.voltage = SDC_SD_VOLT_3_3;
    if (R_SDC_SD_Initialize(g_sdc_sd_card_no, &s_sdc_sd_config, SDC_SD_MODE_MEM) != SDC_SD_SUCCESS)
    {
        return false;
    }

    if (R_SDC_SD_GetCardInfo(g_sdc_sd_card_no, &sdhi_CardReg) != SDC_SD_SUCCESS)
    {
        return false;
    }

    if (R_SDC_SD_GetCardStatus(g_sdc_sd_card_no, &sdhi_CardStatus) != SDC_SD_SUCCESS)
    {
        return false;
    }


    s_reg_buff = 0;
    if (R_SDC_SD_GetBuffRegAddress(g_sdc_sd_card_no, &s_reg_buff) != SDC_SD_SUCCESS)
    {
        return false;
    }

    return true;
}

bool r_sdc_sdmem_rx_lib_write(const uint8_t *p_buff, uint32_t lbn,uint32_t cnt)
{
    sdc_sd_access_t sdc_sd_access;

    sdc_sd_access.p_buff = (uint8_t *)p_buff;
    sdc_sd_access.lbn = lbn;
    sdc_sd_access.cnt = cnt;
    sdc_sd_access.write_mode = SDC_SD_WRITE_OVERWRITE;

    if (s_sdc_sd_config.mode & SDC_SD_MODE_DMA)
    {
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
        r_dmaca_enable(&sdc_sd_access, SDC_SD_TEST_WRITE_OP, s_reg_buff);
#endif 
    }
    else if (s_sdc_sd_config.mode & SDC_SD_MODE_DTC)
    {
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
        r_dtc_enable(&sdc_sd_access, SDC_SD_TEST_WRITE_OP, &reg_buff);
#endif 
    }
    else
    {
    }

    if (R_SDC_SDMEM_Write(g_sdc_sd_card_no, &sdc_sd_access) != SDC_SD_SUCCESS)
    {
        R_SDC_SD_SetDmacDtcTransFlg(g_sdc_sd_card_no, SDC_SD_SET_TRANS_STOP);
        return false;
    }

    if (s_sdc_sd_config.mode & SDC_SD_MODE_DMA)
    {
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
        r_dmaca_disable();
#endif 
    }
    else if (s_sdc_sd_config.mode & SDC_SD_MODE_DTC)
    {
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
        r_dtc_disable();
#endif 
    }
    else
    {
    }

    return true;
}

bool r_sdc_sdmem_rx_lib_read(uint8_t *p_buff, uint32_t lbn, uint32_t cnt)
{
    sdc_sd_access_t sdc_sd_access;

    sdc_sd_access.p_buff = (uint8_t *)p_buff;
    sdc_sd_access.lbn = lbn;
    sdc_sd_access.cnt = cnt;
    if (s_sdc_sd_config.mode & SDC_SD_MODE_DMA)
    {
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
        r_dmaca_enable(&sdc_sd_access, SDC_SD_TEST_READ_OP, s_reg_buff);
#endif 
    }
    else if (s_sdc_sd_config.mode & SDC_SD_MODE_DTC)
    {
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
        r_dtc_enable(&sdc_sd_access, SDC_SD_TEST_READ_OP, &reg_buff);
#endif 
    }
    else
    {
    }

    if (R_SDC_SDMEM_Read(g_sdc_sd_card_no, &sdc_sd_access) != SDC_SD_SUCCESS)
    {
        R_SDC_SD_SetDmacDtcTransFlg(g_sdc_sd_card_no, SDC_SD_SET_TRANS_STOP);
        return false;
    }

    if (s_sdc_sd_config.mode & SDC_SD_MODE_DMA)
    {
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
        r_dmaca_disable();
#endif 
    }
    else if (s_sdc_sd_config.mode & SDC_SD_MODE_DTC)
    {
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
        r_dtc_disable();
#endif 
    }
    else
    {
    }

    return true;
}

bool r_sdc_sdmem_rx_lib_Finalize()
{
#if (SDHI_CFG_CH0_CD_ACTIVE)
    while (R_SDC_SD_GetCardDetection(g_sdc_sd_card_no) != SDC_SD_SUCCESS)
    {
        if (true != r_sdc_sdmem_softwaredelay(5, SDC_SD_DELAY_MILLISECS))
        {
            return false;
        }
    }
#endif 

    if (R_SDC_SD_Close(g_sdc_sd_card_no) != SDC_SD_SUCCESS)
    {
        return false;
    }

    R_SDHI_PinSetEnd();

    r_cmt_close();
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DMA) != 0)
    r_dmaca_close();
#endif 
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_DTC) != 0)
    r_dtc_close();
#endif 
#ifdef SDC_SD_CFG_LONGQ_ENABLE
    r_longq_close();
#endif 
    return true;
}

#endif

