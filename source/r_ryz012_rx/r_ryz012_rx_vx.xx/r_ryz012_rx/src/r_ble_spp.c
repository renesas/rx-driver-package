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
 * File Name    : r_ble_spp.c
 * Description  :
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_ble_spp.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define R_BLE_SPP_CMD_ID_SIZE           (2)
#define R_BLE_SPP_CMD_DATA_SIZE         (2)
#define R_BLE_SPP_CHECKSUM_SIZE         (2)
#define R_BLE_SPP_HEADER_SIZE           (4)
#define R_BLE_SPP_BUFF_SIZE             (255)
#define R_BLE_SPP_MAX_BUFFER_SIZE       (244)
#define R_BLE_SPP_HEADER_TRAILER_SIZE (R_BLE_SPP_CMD_ID_SIZE + R_BLE_SPP_CMD_DATA_SIZE + R_BLE_SPP_CHECKSUM_SIZE)

#define R_BLE_SPP_TX_POWER_MIN          (0x00)
#define R_BLE_SPP_TX_POWER_MAX          (0x2B)
#define R_BLE_SPP_ADEVRES_MAX_LEN       (0x001F)

/* set connection parameters boundary checks */
#define R_BLE_SPP_SCAN_INTV_MIN         (0x0004)
#define R_BLE_SPP_SCAN_INTV_MAX         (0x4000)
#define R_BLE_SPP_SCAN_WINDOW_MIN       (0x0004)
#define R_BLE_SPP_SCAN_WINDOW_MAX       (0x4000)
#define R_BLE_SPP_CONN_INTV_MIN         (0x0006)
#define R_BLE_SPP_CONN_INTV_MAX         (0x0C80)
#define R_BLE_SPP_CONN_LAT_MIN          (0x0000)
#define R_BLE_SPP_CONN_LAT_MAX          (0x01F3)
#define R_BLE_SPP_SUP_TIMEOUT_MIN       (0x000A)
#define R_BLE_SPP_SUP_TIMEOUT_MAX       (0x0C80)

/* Advertising parameter boundary checks */
#define R_BLE_SPP_ADV_INTV_MIN          (0x0020)
#define R_BLE_SPP_ADV_INTV_LEGACY_MAX   (0x4000)
#define R_BLE_SPP_ADV_INTV_EXT_MAX      (0xFFFFFF)
#define R_BLE_SPP_ADV_CHN_MAX           (0x03)

#define R_BLE_SPP_YES                   (0x1)
#define R_BLE_SPP_NO                    (0x0)
#define R_BLE_SPP_TRUE                  (1)
#define R_BLE_SPP_FALSE                 (0)

#define BYTES_TO_U16(MSB,LSB) (uint16_t)(((MSB & 0xFF) << 8) | (LSB & 0xFF))

/* endianess macro */
#define LITTLE_ENDIAN

#ifdef LITTLE_ENDIAN
#define ENDIAN_LSB16(n)     (uint8_t)(n & 0xFF)
#define ENDIAN_MSB16(n)     (uint8_t)(n >> 8)
#define ENDIAN_BYTE1_32(n)  (n & 0xFF)
#define ENDIAN_BYTE2_32(n)  ((n >> 8) & 0XFF)
#define ENDIAN_BYTE3_32(n)  ((n >> 16) & 0XFF)
#define ENDIAN_BYTE4_32(n)  (uint8_t)((n >> 24) & 0XFF)
#define CHECKSUM_LSB(n)     (uint8_t)(n & 0xFF)
#define CHECKSUM_MSB(n)     (uint8_t)((n >> 8) & 0xFF)

#else
#define ENDIAN_LSB16(n)     (n >> 8)
#define ENDIAN_MSB16(n)     (n & 0xFF)
#define ENDIAN_BYTE1_32(n)  (uint8_t)((n >> 24) & 0XFF)
#define ENDIAN_BYTE2_32(n)  ((n >> 16) & 0XFF)
#define ENDIAN_BYTE3_32(n)  ((n >> 8) & 0XFF)
#define ENDIAN_BYTE4_32(n)  (n & 0xFF)
#define CHECKSUM_LSB(n)     ((n >> 8) & 0xFF)
#define CHECKSUM_MSB(n)     (n & 0xFF)

#endif

/* command ID's and length of data */
#define R_BLE_SPP_CMD_SET_TX_POWER          (0xFF05)
#define R_BLE_SPP_LEN_SET_TX_POWER          (0x0001)

#define R_BLE_SPP_CMD_GET_RSSI              (0xFF06)
#define R_BLE_SPP_LEN_GET_RSSI              (0x0000)

#define R_BLE_SPP_CMD_SET_ENABLE_ADV        (0xFF0A)
#define R_BLE_SPP_LEN_SET_ENABLE_ADV        (0x0001)

#define R_BLE_SPP_CMD_GET_BUF_SIZE          (0xFF0C)
#define R_BLE_SPP_LEN_GET_BUF_SIZE          (0x0000)

#define R_BLE_SPP_CMD_SET_ADV_DIR_ADDR      (0xFF0E)
#define R_BLE_SPP_LEN_SET_ADV_DIR_ADDR      (0x0007)

#define R_BLE_SPP_CMD_SET_DEV_NAME          (0xFF13)
#define R_BLE_SPP_LEN_SET_DEV_NAME          (0x0012)

#define R_BLE_SPP_CMD_GET_CONN_PARAM        (0xFF14)
#define R_BLE_SPP_LEN_GET_CONN_PARAM        (0x0000)

#define R_BLE_SPP_CMD_SET_CONN_PARAM        (0xFF15)
#define R_BLE_SPP_LEN_SET_CONN_PARAM        (0x0008)

#define R_BLE_SPP_CMD_GET_CUR_STATE         (0xFF16)
#define R_BLE_SPP_LEN_GET_CUR_STATE         (0x0000)

#define R_BLE_SPP_CMD_TERMINATE_CONN        (0xFF17)
#define R_BLE_SPP_LEN_TERMINATE_CONN        (0x0000)

#define R_BLE_SPP_CMD_RESTART               (0xFF18)
#define R_BLE_SPP_LEN_RESTART               (0x0000)

#define R_BLE_SPP_CMD_SEND_NOTIFY           (0xFF1C)
#define R_BLE_SPP_LEN_SEND_NOTIFY           (0x0016)

#define R_BLE_SPP_CMD_SET_ADV_PARAM         (0xFF20)
#define R_BLE_SPP_LEN_SET_ADV_PARAM         (0x000E)

#define R_BLE_SPP_CMD_SET_DISABLE_ADV       (0xFF21)
#define R_BLE_SPP_LEN_SET_DISABLE_ADV       (0x0001)

#define R_BLE_SPP_CMD_SET_ADV_SRES          (0xFF22)
#define R_BLE_SPP_LEN_SET_ADV_SRES          (0x001F)

#define R_BLE_SPP_CMD_CONF_WHITE_LIST       (0xFF24)
#define R_BLE_SPP_LEN_CONF_WHITE_LIST       (0x0008)

#define R_BLE_SPP_CMD_SEND_INDICATION       (0xFF26)
#define R_BLE_SPP_LEN_SEND_INDICATION       (0x0016)

#define R_BLE_SPP_CMD_CONFM_SECURITY        (0xFF32)
#define R_BLE_SPP_LEN_CONFM_SECURITY        (0x0001)

#define R_BLE_SPP_CMD_SET_PAIRING_PARAMS    (0xFF35)
#define R_BLE_SPP_LEN_SET_PAIRING_PARAMS    (0x0004)

#define R_BLE_SPP_CMD_SECURITY_PASS         (0xFF36)
#define R_BLE_SPP_LEN_SECURITY_PASS         (0x0000)

#define R_BLE_SPP_CMD_MACID                 (0xFF25)
#define R_BLE_SPP_LEN_MACID                 (0x0006)

#define R_BLE_SPP_CMD_INIT                  (0xFF42)
#define R_BLE_SPP_LEN_INIT                  (0x0000)

#define R_BLE_SPP_CMD_DEVICE_READY          (0xFF46)
#define R_BLE_SPP_LEN_DEVICE_READY          (0x0000)

#define R_BLE_SPP_CMD_UNPAIR_DEVICE         (0xFF47)
#define R_BLE_SPP_LEN_UNPAIR_DEVICE         (0x0000)

#define R_BLE_SPP_CMD_ENA_DBG_EVENT         (0xFF48)
#define R_BLE_SPP_LEN_ENA_DBG_EVENT         (0x0002)

#define R_BLE_SPP_CMD_EXT_PHY_TYPE          (0xFF49)
#define R_BLE_SPP_LEN_EXT_PHY_TYPE          (0x0002)

#define R_BLE_SPP_CMD_SET_DYN_PROFILE       (0xFF70)

#define R_BLE_SPP_CMD_DYN_PROFILE_COMPLETE  (0xFF71)
#define R_BLE_SPP_LEN_DYN_PROFILE_COMPLETE  (0x0000)

#define R_BLE_SPP_CMD_GET_VERSION           (0xFF72)
#define R_BLE_SPP_LEN_GET_VERSION           (0x0000)

#define R_BLE_SPP_CMD_BLE_CONFIG            (0xFF73)
#define R_BLE_SPP_LEN_BLE_CONFIG            (0x0001)

#define R_BLE_SPP_CMD_SET_SCAN_PARAMETER    (0xFF74)
#define R_BLE_SPP_LEN_SET_SCAN_PARAMETER    (0x0007)

#define R_BLE_SPP_CMD_START_SCAN            (0xFF75)
#define R_BLE_SPP_LEN_START_SCAN            (0x0000)

#define R_BLE_SPP_CMD_STOP_SCAN             (0xFF76)
#define R_BLE_SPP_LEN_STOP_SCAN             (0x0000)

#define R_BLE_SPP_CMD_CREATE_CONN_PARAM     (0xFF77)
#define R_BLE_SPP_LEN_CREATE_CONN_PARAM     (0x0015)

#define R_BLE_SPP_CMD_DISCOVER_ALL          (0xFF79)
#define R_BLE_SPP_LEN_DISCOVER_ALL          (0x0000)

#define R_BLE_SPP_CMD_READ_HANDLE           (0xFF7A)
#define R_BLE_SPP_LEN_READ_HANDLE           (0x0002)

#define R_BLE_SPP_CMD_READ_BLOB             (0xFF7C)
#define R_BLE_SPP_LEN_READ_BLOB             (0x0002)

#define R_BLE_SPP_CMD_WRITE_CHARACTERISTIC  (0xFF7E)
#define R_BLE_SPP_LEN_WRITE_CHARACTERISTIC  (0x0018)

#define R_BLE_SPP_CMD_WRITE_CHARACTERISTIC_NO_RESPONSE  (0xFF7F)
#define R_BLE_SPP_LEN_WRITE_CHARACTERISTIC_NO_RESPONSE  (0x0018)

#define R_BLE_SPP_CMD_SET_ENABLE_RPA        (0xFF27)
#define R_BLE_SPP_LEN_SET_ENABLE_RPA        (0x0001)

#define R_BLE_SPP_CMD_SET_CONFIG_RPA        (0xFF28)
#define R_BLE_SPP_LEN_SET_CONFIG_RPA        (0x0000)

#define R_BLE_SPP_CMD_SET_OOBDATA           (0xFF2B)
#define R_BLE_SPP_LEN_SET_OOBDATA           (0x0011)

#define R_BLE_SPP_CMD_SET_ATTR              (0xFF29)
#define R_BLE_SPP_LEN_SET_ATTR              (0x0004)

#define R_BLE_SPP_CMD_GET_ATTR              (0xFF2A)
#define R_BLE_SPP_LEN_GET_ATTR              (0x0004)

#define R_BLE_SPP_MAC_BUF_SIZE              (R_BLE_SPP_LEN_MACID + 1)
#define R_BLE_SPP_ADV_SRES_DATA_BITS        (0x0003)
#define R_BLE_SPP_DEBUG_EVENT_0             (0x0000)
#define R_BLE_SPP_DEBUG_EVENT_5             (0x0005)
#define R_BLE_SPP_NOTIFY_BUF_SIZE           (R_BLE_SPP_MAX_BUFFER_SIZE +4)
#define R_BLE_SPP_HANDLE_LENGTH_SIZE        (0x0004)

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static r_ble_spp_payload_t g_p_payload;
static uint8_t g_tx_data_buf[R_BLE_SPP_BUFF_SIZE];
static uint8_t g_rx_data_buf[R_BLE_SPP_BUFF_SIZE];
static uint8_t g_payload_buf[R_BLE_SPP_BUFF_SIZE];
static r_ble_spp_cfg_t g_r_ble_spp_cfg_internal = {NULL, NULL, NULL, NULL, NULL, NULL};
static uint32_t g_event_cksum1 = 0;
static uint32_t g_event_cksum2 = 0;
static uint16_t g_event_cksum = 0;
static uint8_t g_r_ble_spp_open_complete = R_BLE_SPP_FALSE;

static uint16_t r_ble_spp_fletcher16 (uint8_t *pd, int count);
static uint16_t r_ble_spp_event_fletcher16 (uint8_t pd);
static void  r_ble_spp_memcpy (void* dst, const void* src, uint16_t cnt);
static r_ble_spp_cmd_rsp_t r_ble_spp_generate_message (uint16_t command, uint8_t *payload, uint16_t payload_length);
static uint8_t r_ble_spp_string_len (unsigned char* str);

/*********************************************************************************************************************
 * register communication related callback functions internally and opens the communication channel
 * by sending DEVICE READY command
 *
 * @param 	p_api	Contains open,read,write,close,mw_callback and context data to be used in wrapper
 *
 * @retval  R_BLE_SPP_SUCCESS	Callback registered successfully and channel is opened.
 * @retval  R_BLE_SPP_ERROR     Communication channel open/write error
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Open(r_ble_spp_cfg_t *p_api)
{
    r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_FALSE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_INVALID_OPTION;
    }

    if (NULL == p_api)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

    g_r_ble_spp_cfg_internal = *(p_api);
    retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_open((sci_ch_t)BLE_CFG_SCI_CHANNEL, rm_ble_spp_callback);
    if (R_BLE_SPP_SUCCESS != retVal)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    R_BSP_SoftwareDelay(200, BSP_DELAY_MILLISECS);

    //Generate SPP command frame
    r_ble_spp_generate_message(R_BLE_SPP_CMD_DEVICE_READY, NULL, R_BLE_SPP_LEN_DEVICE_READY);
    retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_DEVICE_READY + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
    {
        retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
    }
    else
    {
        g_r_ble_spp_open_complete = R_BLE_SPP_TRUE;
        retVal = R_BLE_SPP_SUCCESS;
    }
    return retVal;
}

/*********************************************************************************************************************
 * Dynamic profile is created and sent to the BLE module by sending CONFIG PROFILE CREATE Command
 *
 * @param   prf_info	points to the attribute table to be processed.
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Config_Profile(r_ble_spp_profile_params_t *prf_info)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint16_t len;
	uint8_t data[R_BLE_SPP_MAX_BUFFER_SIZE]= {'\0'};
	uint8_t uuidLen=0;
	uint8_t attrindex=0;
	uint16_t attr_ind=0;

	if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
	{
        retVal = R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == prf_info)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

    if((prf_info->write_flag <= 1) || (prf_info->read_flag <= 1))
	{
		data[0] = ENDIAN_LSB16(prf_info->attNum);
		data[1] = ENDIAN_MSB16(prf_info->attNum);
		data[2] = prf_info->perm;
		data[3] = prf_info->uuidLen;
		data[4] = ENDIAN_LSB16(prf_info->attrLen);
		data[5] = ENDIAN_MSB16(prf_info->attrLen);

		uuidLen = prf_info->uuidLen;
		r_ble_spp_memcpy(&data[6], prf_info->uuid,uuidLen);
		attrindex = (uint8_t)(6 + uuidLen);
		r_ble_spp_memcpy(&data[attrindex], prf_info->pAttrValue, prf_info->attrLen);
		attr_ind = (uint16_t)(attrindex + prf_info->attrLen);

		data[attr_ind ] = prf_info->write_flag;
		data[attr_ind + 1] = prf_info->read_flag;

		len = prf_info->payload_length;
		if (len > 0)
		{
			//Generate SPP command frame
			r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_DYN_PROFILE, data, len);
			retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(len + R_BLE_SPP_HEADER_TRAILER_SIZE));
			if (R_BLE_SPP_SUCCESS != retVal)
			{
				retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
			}
		}
	}
	return retVal;
}

/*********************************************************************************************************************
 * send indication to BLE module of Dynamic profile creation completion by sending
 * PROFILE COMPLETE command
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Profile_Complete(void)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_DYN_PROFILE_COMPLETE, NULL, R_BLE_SPP_LEN_DYN_PROFILE_COMPLETE);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_DYN_PROFILE_COMPLETE + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
    {
        retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
    }
	return retVal;
}

/*********************************************************************************************************************
 * Sends BLE INIT command to trigger master/slave initialization
 *
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Init(void)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }
    
    //Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_INIT, NULL,R_BLE_SPP_LEN_INIT);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_INIT + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * Sends a Public/Random MAC ID to BLE module by sending SET MAC ID command
 *
 * @param	mac_id		holds the MAC ID to be set at BLE
 * @param   p_addr_type  can select the type of MAC ID RANDOM or PUBLIC
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetMACID(r_ble_spp_peer_addrType_t p_addr_type, uint8_t *mac_id)
{

	/* any validation check based on p_addr_type */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t len = 0;
	uint8_t data[R_BLE_SPP_MAC_BUF_SIZE];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if ((NULL == mac_id) || (R_BLE_SPP_ADDR_RANDOM < p_addr_type))
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	len = 1;
	data[0] = p_addr_type;
	if (p_addr_type == R_BLE_SPP_ADDR_PUBLIC)
	{
		r_ble_spp_memcpy(&data[1], mac_id, R_BLE_SPP_LEN_MACID);
		len = (uint8_t)(len + R_BLE_SPP_LEN_MACID);
	}

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_MACID, data, len);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(len + R_BLE_SPP_HEADER_TRAILER_SIZE));
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * Send CONFIG MODE command to set the BLE module to SLAVE or MASTER
 * If the requested mode is the new mode then BLE module would restart
 *
 * @param	mode holds the mode value SLAVE/MASTER
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Config_Mode(r_ble_spp_mode_t mode)
{
	/* Cmd to Configure the BLE mode Master/Slave */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (R_BLE_SPP_SLAVE < mode)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_BLE_CONFIG, (uint8_t *)&mode , R_BLE_SPP_LEN_BLE_CONFIG);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_BLE_CONFIG + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * Multiple Advertisement parameters are sent to BLE module by sending SET ADVERTISEMENT PARAM command

 *
 * @param	p_adv_param		holds the parameters for advertising to be set at BLE
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetAdvParam(r_ble_spp_adv_param_t *p_adv_param)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_LEN_SET_ADV_PARAM];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }
    
    if (NULL == p_adv_param)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	if ((p_adv_param->adv_prop_type <=R_BLE_SPP_ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY))
	{
		if (((p_adv_param->adv_intv_min >= R_BLE_SPP_ADV_INTV_MIN)
		    && (p_adv_param->adv_intv_max <= R_BLE_SPP_ADV_INTV_LEGACY_MAX))
		    &&((p_adv_param->adv_intv_min <= p_adv_param->adv_intv_max))
		    &&((p_adv_param->adv_ch_map <= R_BLE_SPP_ADV_CHN_MAX))
			&&(p_adv_param->filter_policy <= R_BLE_SPP_ADV_ALLOW_SCAN_WLST_CONN_WLST))
		{
			data[0] = p_adv_param->adv_ch_map;
			data[1] = p_adv_param->filter_policy;
			data[2] = p_adv_param->adv_phy;
			data[3] = p_adv_param->sec_adv_phy;
			data[4] = ENDIAN_LSB16(p_adv_param->adv_prop_type);
			data[5] = ENDIAN_MSB16(p_adv_param->adv_prop_type);
			data[6] = ENDIAN_BYTE1_32(p_adv_param->adv_intv_min);
			data[7] = ENDIAN_BYTE2_32(p_adv_param->adv_intv_min); //0x00;
			data[8] = ENDIAN_BYTE3_32(p_adv_param->adv_intv_min); //0x00;
			data[9] = ENDIAN_BYTE4_32(p_adv_param->adv_intv_min); //0x00;
			data[10] = ENDIAN_BYTE1_32(p_adv_param->adv_intv_max); //0xB0;
			data[11] = ENDIAN_BYTE2_32(p_adv_param->adv_intv_max); //0x00;
			data[12] = ENDIAN_BYTE3_32(p_adv_param->adv_intv_max); //0x00;
			data[13] = ENDIAN_BYTE4_32(p_adv_param->adv_intv_max); //0x00;
			retVal = R_BLE_SPP_SUCCESS;
		}
	}
	else if ((p_adv_param->adv_prop_type >= R_BLE_SPP_ADV_EVT_PROP_LEGACY_CONNECTABLE_SCANNABLE_UNDIRECTED) &&
	        (p_adv_param->adv_prop_type <=R_BLE_SPP_ADV_EVT_PROP_EXTENDED_MASK_TX_POWER_INCLUDE))
	{
		if (((p_adv_param->adv_phy >= R_BLE_SPP_SET_PHYS_PREF_1M) && (p_adv_param->adv_phy <= R_BLE_SPP_SET_PHYS_PREF_CD))
			&& ((p_adv_param->sec_adv_phy>= R_BLE_SPP_SET_PHYS_PREF_1M) && (p_adv_param->sec_adv_phy <= R_BLE_SPP_SET_PHYS_PREF_CD))
			&& ((p_adv_param->adv_intv_min >= R_BLE_SPP_ADV_INTV_MIN) && (p_adv_param->adv_intv_max <= R_BLE_SPP_ADV_INTV_EXT_MAX))
			&&  (p_adv_param->adv_intv_min <= p_adv_param->adv_intv_max))
		{
			data[0] = p_adv_param->adv_ch_map;
			data[1] = p_adv_param->filter_policy;
			data[2] = p_adv_param->adv_phy;
			data[3] = p_adv_param->sec_adv_phy;
			data[4] = ENDIAN_LSB16(p_adv_param->adv_prop_type);
			data[5] = ENDIAN_MSB16(p_adv_param->adv_prop_type);
			data[6] = ENDIAN_BYTE1_32(p_adv_param->adv_intv_min);
			data[7] = ENDIAN_BYTE2_32(p_adv_param->adv_intv_min); //0x00;
			data[8] = ENDIAN_BYTE3_32(p_adv_param->adv_intv_min); //0x00;
			data[9] = ENDIAN_BYTE4_32(p_adv_param->adv_intv_min); //0x00;
			data[10] = ENDIAN_BYTE1_32(p_adv_param->adv_intv_max); //0xB0;
			data[11] = ENDIAN_BYTE2_32(p_adv_param->adv_intv_max); //0x00;
			data[12] = ENDIAN_BYTE3_32(p_adv_param->adv_intv_max); //0x00;
			data[13] = ENDIAN_BYTE4_32(p_adv_param->adv_intv_max); //0x00;
			retVal = R_BLE_SPP_SUCCESS;
		}
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}

	if (R_BLE_SPP_SUCCESS == retVal)
	{
		r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_ADV_PARAM,data, R_BLE_SPP_LEN_SET_ADV_PARAM);
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_ADV_PARAM + R_BLE_SPP_HEADER_TRAILER_SIZE);
		if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	return retVal;
}

/*********************************************************************************************************************
 * Sends secondary Advertising parameters to BLE module by sending SRES DATA command
 *
 * @param 	p_adv_srsp_data		contains advertisement scan response data
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetAdvSresData(r_ble_spp_adv_data_t *p_adv_srsp_data)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint16_t len = 0;
	uint8_t data[R_BLE_SPP_LEN_SET_ADV_SRES + 3];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == p_adv_srsp_data)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	if ((p_adv_srsp_data->data_type <= R_BLE_SPP_PERD_ADV_DATA_MODE) && (p_adv_srsp_data->data_length <= R_BLE_SPP_ADEVRES_MAX_LEN))
	{
		data[0] = p_adv_srsp_data->data_type;
		data[1] = ENDIAN_LSB16(p_adv_srsp_data->data_length); /* min - 0x01, max - 0x20 */
		data[2] = ENDIAN_MSB16(p_adv_srsp_data->data_length);

		r_ble_spp_memcpy(&data[3], &p_adv_srsp_data->p_data[0], p_adv_srsp_data->data_length);
		len = (uint16_t)(p_adv_srsp_data->data_length + R_BLE_SPP_ADV_SRES_DATA_BITS);

		//Generate SPP command frame
		r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_ADV_SRES,data ,(uint16_t)len);
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(len + R_BLE_SPP_HEADER_TRAILER_SIZE));
		if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	return retVal;
}

/*********************************************************************************************************************
 * Enables the advertising mode in BLE by sending ADVERTISE ENABLE command
 *
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StartAdv(void)
{
	/* This function send advertising enable command */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;
	uint8_t adv_enable = R_BLE_SPP_TRUE;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_ENABLE_ADV, &adv_enable ,R_BLE_SPP_LEN_SET_ENABLE_ADV);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_ENABLE_ADV + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * Disables the advertising mode in BLE by sending ADVERTISE DISABLE command
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StopAdv(void)
{
	/* This function send advertising disable command */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;
	uint8_t adv_enable = R_BLE_SPP_FALSE;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_DISABLE_ADV, &adv_enable, R_BLE_SPP_LEN_SET_DISABLE_ADV);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_DISABLE_ADV + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * sends user triggered DISCONNECT command to terminate the current connection
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Disconnect(void)
{
	/* This function Terminates the connection with BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_TERMINATE_CONN, NULL,R_BLE_SPP_LEN_TERMINATE_CONN);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_TERMINATE_CONN + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * Sends RESTART command to restart the BLE module
 *
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Restart(void)
{
	/* This function will issue restart module command */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_RESTART, NULL ,R_BLE_SPP_LEN_RESTART);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_RESTART + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * retrieves current RSSI value from BLE module by sending GET RSSI command
 * the retrieved rssi value shall be converted to signed for the correct value
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetRSSI(void)
{
	/* This function will issue Get RSSI value command */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_GET_RSSI, NULL , R_BLE_SPP_LEN_GET_RSSI);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_GET_RSSI + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * retrieves current state of BLE module by sending GET CURRENT STATE command
 * current state shall be IDLE/ADVERTISING/CONNECTED
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetCurrState(void)
{
	/* This function will issue Get Current Work State command */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_GET_CUR_STATE, NULL, R_BLE_SPP_LEN_GET_CUR_STATE);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_GET_CUR_STATE + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * current firmware version is retrieved from BLE module by sending GET VERSION command
 * Major, Minor and Build versions are received from BLE.
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetVersion(void)
{
	/* This function retrieves the version of the stack */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

	if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_GET_VERSION, NULL , R_BLE_SPP_LEN_GET_VERSION);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_GET_VERSION + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * Prepares and sends UNPAIR command
 *
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Unpair(void)
{
	/* This function UNPAIR the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_UNPAIR_DEVICE, NULL, R_BLE_SPP_LEN_UNPAIR_DEVICE);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_UNPAIR_DEVICE + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * User shall set the device name in the BLE by sending SET DEVICE NAME command
 *
 * @param 	dev_name	holds the device name to be set/modified at BLE
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetDevName(unsigned char *dev_name)
{
	/* This function Sets the DEVICE NAME for the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t len = 0;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == dev_name)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	len = r_ble_spp_string_len(dev_name);
	if (R_BLE_SPP_MAX_DEV_NAME_LEN < len)
	{
        return R_BLE_SPP_INVALID_PARAMETER;
	}

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_DEV_NAME, dev_name, (uint16_t)len);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(len + R_BLE_SPP_HEADER_TRAILER_SIZE));
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * send DEBUG EVENT command to BLE module
 *
 * @param	debug_event		to set the debug event type (channel Map/connection param request etc)
 * 			status			to enable or disable the debug event
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_DebugEvent(uint8_t debug_event, uint8_t status)
{
	/* This function Sends the DEBUG EVENT for the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_LEN_ENA_DBG_EVENT];
	/*	0x01- Channel Map Request Event
		0x02- Connection Parameter Request Event
		0x03- Channel Map Update Event
		0x04- Connection Parameter Update Event */
    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if ((R_BLE_SPP_YES != status) && (R_BLE_SPP_NO != status))
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

    if (R_BLE_SPP_DEBUG_EVENT_5 < debug_event)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	data[0] = debug_event;
	data[1] = status;

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_ENA_DBG_EVENT, data , R_BLE_SPP_LEN_ENA_DBG_EVENT);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_ENA_DBG_EVENT + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * Output power can be set by user by sending SET TX POWER command
 *
 * @param	tx_power	holds the power index value to be set in dBm
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetTxPower(uint8_t tx_power)
{
	/* This function set output power */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (R_BLE_SPP_TX_POWER_MAX < tx_power)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_TX_POWER, &tx_power, R_BLE_SPP_LEN_SET_TX_POWER);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_TX_POWER + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * User can get the current buffer size from BLE module by sending GET BUF SIZE command
 *
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetBufSize(void)
{
	/* This function gets the Current BUFF Size from the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_GET_BUF_SIZE, NULL , R_BLE_SPP_LEN_GET_BUF_SIZE);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_GET_BUF_SIZE + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * current connection parameters can be retrieved from BLE module
 * by sending GET CONN PARAM command
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetConnParam(void)
{
	/* This function send Get Connection parameters command */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_GET_CONN_PARAM, NULL ,  R_BLE_SPP_LEN_GET_CONN_PARAM);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_GET_CONN_PARAM + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * User can set the MAC address to BLE module
 * by sending addr_type to BLE Module for selecting public & random address
 *
 * @param p_addr is hold the Address
 * @param addr_type is used to select the type of MAC ID i.e PUBLIC/RANDOM
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetAddr(r_ble_spp_own_addrType_t addr_type, uint8_t *p_addr)
{
	/* This function sets the MAC Address for the BLE Module */
	uint8_t data[R_BLE_SPP_LEN_SET_ADV_DIR_ADDR];
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if ((NULL == p_addr) || (R_BLE_SPP_ADDR_RPA_ID_RANDOM < addr_type))
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	data[0] = addr_type;

	r_ble_spp_memcpy(&data[1], &p_addr[0], R_BLE_SPP_LEN_MACID);

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_ADV_DIR_ADDR, data, R_BLE_SPP_LEN_SET_ADV_DIR_ADDR);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_ADV_DIR_ADDR + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * User can select the Transmission speed for BLE module
 * by sending p_phy_info to BLE Module 
 *
 * @param p_phy_info will hold the type of PHY type and PHY options
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetPhy(r_ble_spp_set_phy_param_t *p_phy_info)
{
	/* This function sets Transmission Speed for the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_LEN_EXT_PHY_TYPE];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == p_phy_info)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	if (((p_phy_info->phy_type == R_BLE_SPP_SET_PHYS_PREF_1M || p_phy_info->phy_type == R_BLE_SPP_SET_PHYS_PREF_2M) &&
		(p_phy_info->phy_options == R_BLE_SPP_SET_PHYS_NO_PREF )) ||
		((p_phy_info->phy_type == R_BLE_SPP_SET_PHYS_PREF_CD) &&
		(p_phy_info->phy_options == R_BLE_SPP_SET_PHYS_S_2 || p_phy_info->phy_options == R_BLE_SPP_SET_PHYS_S_8)))
	{
		data[0] = p_phy_info->phy_type;
		data[1] = p_phy_info->phy_options;

		//Generate SPP command frame
		r_ble_spp_generate_message(R_BLE_SPP_CMD_EXT_PHY_TYPE, data , R_BLE_SPP_LEN_EXT_PHY_TYPE);
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_EXT_PHY_TYPE + R_BLE_SPP_HEADER_TRAILER_SIZE);
	    if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}
	return retVal;
}

/*********************************************************************************************************************
 * User can set the MAC address as WhiteList and various modes to BLE module
 * by sending op_code to BLE Module for selecting SET CLEAR RESET
 *
 * @param op_code is used to select the Whitelist ADD/RESET/REMOVE
 * @param p_whitelist hold the information MAC ID and type of MAC ID PUBLIC/RANDOM
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_ConfWhiteList(r_ble_spp_whiteList_optype_t op_code,r_ble_spp_whitelist_t *p_whitelist)
{
	/* This function sends the MAC Address as whitelist to the BLE Module */
	uint8_t data[R_BLE_SPP_LEN_CONF_WHITE_LIST];
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t len=0;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == p_whitelist)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	/* checks the whitelist option is SET CLEAR RESET and also MAC address is valid */
	if(((op_code == R_BLE_SPP_LIST_CLR && p_whitelist->p_addr == NULL) ||
		((op_code < R_BLE_SPP_LIST_CLR) && p_whitelist->p_addr!=NULL))&&
		((p_whitelist->addr_type == R_BLE_SPP_ADDR_PUBLIC || p_whitelist->addr_type == R_BLE_SPP_ADDR_RANDOM)))
	{
		data[0] = op_code;
		data[1] = p_whitelist->addr_type;
		len = 2;

		/* checks the MAC ADDRESS is VALID then copies the MAC ADDRESS to DATA buff */
		if (p_whitelist->p_addr != NULL)
		{
			r_ble_spp_memcpy(&data[2], &p_whitelist->p_addr[0], R_BLE_SPP_LEN_MACID);
			len = R_BLE_SPP_LEN_CONF_WHITE_LIST;

			/* the Data buff and command ID is wrapped */
			r_ble_spp_generate_message(R_BLE_SPP_CMD_CONF_WHITE_LIST, data, (uint16_t)len);
			retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(len + R_BLE_SPP_HEADER_TRAILER_SIZE));
		    if (R_BLE_SPP_SUCCESS != retVal)
			{
				retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
			}
		}
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}
	return retVal;
}

/*********************************************************************************************************************
 * User can enable the Security Functionality to the BLE module
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SecurityPasskey(void)
{
	/* This function enables the security to the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }
    
    //Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SECURITY_PASS, NULL, R_BLE_SPP_LEN_SECURITY_PASS);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SECURITY_PASS + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This Function will send the PASSKEY to the BLE module
 *
 * @param response is used to confirm numeric passkey with 1 success or 0 Failure
 * @retval  R_BLE_SPP_SUCCESS       command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_ReplyNumComp(uint8_t response)
{
	/* This function send security password to the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_LEN_CONFM_SECURITY + 4];
	int len = 0;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if ((R_BLE_SPP_YES != response) && (R_BLE_SPP_NO != response))
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

    /* password is copied to the DATA buff*/
    data[0] = 0x6A;
    data[1] = 0xE4;
    data[2] = 0x3B;
    data[3] = 0x8D;
    data[4] = response;
    len = 5;

    /* the Data buff and command ID is wrapped */
	r_ble_spp_generate_message(R_BLE_SPP_CMD_CONFM_SECURITY, data, (uint16_t)len);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(len + R_BLE_SPP_HEADER_TRAILER_SIZE));
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

//GATT functions
/*********************************************************************************************************************
 * User can send the notifications to the BLE module
 *
 * @param p_ntf_data is holds the handle and GATT value
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  invalid data to handle
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTS_Notification(r_ble_spp_gatt_hdl_value_pair_t  *p_ntf_data)
{
	/* This function send Notifications to the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_NOTIFY_BUF_SIZE] ={0};
	int len = 0;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == p_ntf_data)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	/* checks the Handler info is within range or not */
	data[0] = ENDIAN_LSB16(p_ntf_data->attr_hdl);
	data[1] = ENDIAN_MSB16(p_ntf_data->attr_hdl);
	data[2] = ENDIAN_LSB16(p_ntf_data->value.value_len);
	data[3] = ENDIAN_MSB16(p_ntf_data->value.value_len);
	len = 4;

	/* checks the data is present or not BY checking Length for sending to the BLE Module */
	if (p_ntf_data->value.value_len > 0)
	{
		r_ble_spp_memcpy(&data[4], &p_ntf_data->value.p_value[0], p_ntf_data->value.value_len);
		len += p_ntf_data->value.value_len;

		/* the Data buff and command ID is wrapped */
		r_ble_spp_generate_message(R_BLE_SPP_CMD_SEND_NOTIFY,data,(uint16_t)len);
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(len + R_BLE_SPP_HEADER_TRAILER_SIZE));
	    if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	return retVal;
}

/*********************************************************************************************************************
 * User can send the indication to the BLE module
 *
 * @param @param p_ind_data is holds the handle and GATT value
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_ERROR       Communication write error
 * @retval  R_BLE_SPP_INVALID_PARAMETER  invalid data to handle
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is NOt successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTS_Indication(r_ble_spp_gatt_hdl_value_pair_t  *p_ind_data)
{
	/* This function send Indication to the BLE Module */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_NOTIFY_BUF_SIZE];
	int len = 0;

	//Handle range 0x0000 - 0xFFFF
    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == p_ind_data)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	/* checks the Handler info is within range or not */
	data[0] = ENDIAN_LSB16(p_ind_data->attr_hdl);
	data[1] = ENDIAN_MSB16(p_ind_data->attr_hdl);
	data[2] = ENDIAN_LSB16(p_ind_data->value.value_len);
	data[3] = ENDIAN_MSB16(p_ind_data->value.value_len);
	len = 4;

	/* checks the data is present or not BY checking Length for sending to the BLE Module */
	if(p_ind_data->value.value_len > 0)
	{
		r_ble_spp_memcpy(&data[4], &p_ind_data->value.p_value[0], p_ind_data->value.value_len);
		len  += p_ind_data->value.value_len;

		/* the Data buff and command ID is wrapped */
		r_ble_spp_generate_message(R_BLE_SPP_CMD_SEND_INDICATION,data , (uint16_t)len);
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(len + R_BLE_SPP_HEADER_TRAILER_SIZE));
	    if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	return retVal;
}


//Master Functions
/*********************************************************************************************************************
 * This Function sets the scan parameters for BLE master device
 *
 * @param	scan_params				contains address type,scan interval,scan window,filter policies
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetScanParam(r_ble_spp_scan_parameter_t *scan_params)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_LEN_SET_SCAN_PARAMETER] ;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == scan_params)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	if(((scan_params->own_addr_type <= R_BLE_SPP_ADDR_RPA_ID_RANDOM))
	    && ((scan_params->scan_filter_policy <= R_BLE_SPP_SCAN_FP_ALLOW_ADV_WL_DIRECT_ADV_MACTH))
		&& ((scan_params->p_scan_param.scan_type <= R_BLE_SPP_SCAN_TYPE_ACTIVE))
		&& ((scan_params->p_scan_param.scan_interval >= R_BLE_SPP_SCAN_INTV_MIN) && (scan_params->p_scan_param.scan_interval <= R_BLE_SPP_SCAN_INTV_MAX))
		&& ((scan_params->p_scan_param.scan_window >= R_BLE_SPP_SCAN_WINDOW_MIN) && (scan_params->p_scan_param.scan_window <= R_BLE_SPP_SCAN_WINDOW_MAX)))
	{
		data[0] = scan_params->own_addr_type;
		data[1] = scan_params->scan_filter_policy;
		data[2] = scan_params->p_scan_param.scan_type;
		data[3] = ENDIAN_LSB16(scan_params->p_scan_param.scan_interval);
		data[4] = ENDIAN_MSB16(scan_params->p_scan_param.scan_interval);
		data[5] = ENDIAN_LSB16(scan_params->p_scan_param.scan_window);
		data[6] = ENDIAN_MSB16(scan_params->p_scan_param.scan_window);

		r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_SCAN_PARAMETER, data ,R_BLE_SPP_LEN_SET_SCAN_PARAMETER);
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_SCAN_PARAMETER + R_BLE_SPP_HEADER_TRAILER_SIZE);
	    if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}
	return retVal;
}

/*********************************************************************************************************************
 * connection related parameters can be set by sending SET CONN PARAM command
 * connection interval period, latency, timeout can be set
 *
 * @param	p_param		contains min/max interval, timeout, latency data
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetConnParam(r_ble_spp_conn_param_t *p_param)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_LEN_SET_CONN_PARAM] ;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == p_param)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	if (((p_param->conn_intv_min >= R_BLE_SPP_CONN_INTV_MIN) && (p_param->conn_intv_max <= R_BLE_SPP_CONN_INTV_MAX))
		&& (p_param->conn_intv_min <= p_param->conn_intv_max)
		&& (p_param->conn_latency <= R_BLE_SPP_CONN_LAT_MAX)
		&& ((p_param->sup_timout >= R_BLE_SPP_SUP_TIMEOUT_MIN) && (p_param->sup_timout <= R_BLE_SPP_SUP_TIMEOUT_MAX)))
	{
		data[0] = ENDIAN_LSB16(p_param->conn_intv_min);
		data[1] = ENDIAN_MSB16(p_param->conn_intv_min);
		data[2] = ENDIAN_LSB16(p_param->conn_intv_max);
		data[3] = ENDIAN_MSB16(p_param->conn_intv_max);
		data[4] = ENDIAN_LSB16(p_param->conn_latency);
		data[5] = ENDIAN_MSB16(p_param->conn_latency);
		data[6] = ENDIAN_LSB16(p_param->sup_timout);
		data[7] = ENDIAN_MSB16(p_param->sup_timout);

		r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_CONN_PARAM, data ,R_BLE_SPP_LEN_SET_CONN_PARAM);
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_CONN_PARAM + R_BLE_SPP_HEADER_TRAILER_SIZE);
	    if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function initiates SPP_CMD_CONNECT command and creates connection with BLE slave device
 *
 * @param	p_param		contains min/max interval,scan interval,scan window, timeout, latency data,filter policies
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Connect(r_ble_spp_create_conn_param_t *p_param)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_LEN_CREATE_CONN_PARAM];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }
	
    if (NULL == p_param)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	if ((p_param->init_filter_policy <= R_BLE_SPP_FP_ADV_WL)
		&& ((p_param->remote_bd_addr_type <= R_BLE_SPP_ADDR_RANDOM))
		&& (p_param->own_addr_type <= R_BLE_SPP_ADDR_RPA_ID_RANDOM)
		&& ((p_param->p_conn_param.scan_intv >= R_BLE_SPP_SCAN_INTV_MIN) && (p_param->p_conn_param.scan_intv <= R_BLE_SPP_SCAN_INTV_MAX))
		&& ((p_param->p_conn_param.scan_window >= R_BLE_SPP_SCAN_WINDOW_MIN) && (p_param->p_conn_param.scan_window <= R_BLE_SPP_SCAN_WINDOW_MAX))
		&& ((p_param->p_conn_param.p_param.conn_intv_min >= R_BLE_SPP_CONN_INTV_MIN) && (p_param->p_conn_param.p_param.conn_intv_max <= R_BLE_SPP_CONN_INTV_MAX))
		&& ((p_param->p_conn_param.p_param.conn_intv_min <= p_param->p_conn_param.p_param.conn_intv_max))
		&& (p_param->p_conn_param.p_param.conn_latency <= R_BLE_SPP_CONN_LAT_MAX)
		&& ((p_param->p_conn_param.p_param.sup_timout >= R_BLE_SPP_SUP_TIMEOUT_MIN) && (p_param->p_conn_param.p_param.sup_timout <= R_BLE_SPP_SUP_TIMEOUT_MAX)))
	{
		data[0] = p_param->init_filter_policy;
		r_ble_spp_memcpy(&data[1], &(p_param->remote_bd_addr[0]), R_BLE_SPP_LEN_MACID);
		data[7] = p_param->remote_bd_addr_type;
		data[8] = p_param->own_addr_type;
		data[9] = ENDIAN_LSB16(p_param->p_conn_param.scan_intv);
		data[10] = ENDIAN_MSB16(p_param->p_conn_param.scan_intv);
		data[11] = ENDIAN_LSB16(p_param->p_conn_param.scan_window);
		data[12] = ENDIAN_MSB16(p_param->p_conn_param.scan_window);
		data[13] = ENDIAN_LSB16(p_param->p_conn_param.p_param.conn_intv_min);
		data[14] = ENDIAN_MSB16(p_param->p_conn_param.p_param.conn_intv_min);
		data[15] = ENDIAN_LSB16(p_param->p_conn_param.p_param.conn_intv_max);
		data[16] = ENDIAN_MSB16(p_param->p_conn_param.p_param.conn_intv_max);
		data[17] = ENDIAN_LSB16(p_param->p_conn_param.p_param.conn_latency);
		data[18] = ENDIAN_MSB16(p_param->p_conn_param.p_param.conn_latency);
		data[19] = ENDIAN_LSB16(p_param->p_conn_param.p_param.sup_timout);
		data[20] = ENDIAN_MSB16(p_param->p_conn_param.p_param.sup_timout);

		r_ble_spp_generate_message(R_BLE_SPP_CMD_CREATE_CONN_PARAM, data ,R_BLE_SPP_LEN_CREATE_CONN_PARAM);
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_CREATE_CONN_PARAM + R_BLE_SPP_HEADER_TRAILER_SIZE);
	    if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function sends the command to start the scan for nearby BLE slave devices
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StartScan(void)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	r_ble_spp_generate_message(R_BLE_SPP_CMD_START_SCAN,NULL ,R_BLE_SPP_LEN_START_SCAN);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_START_SCAN + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function sends the command to stop the scanning
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StopScan(void)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	r_ble_spp_generate_message(R_BLE_SPP_CMD_STOP_SCAN,NULL ,R_BLE_SPP_LEN_STOP_SCAN);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_STOP_SCAN + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function sends the command to discover the BLE slave attribute services
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_DiscoverAllAttributes(void)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_DISCOVER_ALL,NULL ,R_BLE_SPP_LEN_DISCOVER_ALL);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_DISCOVER_ALL + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function initiates to read characteristics of an attribute
 *
 * @param	handle		Attribute handle
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_ReadCharacteristic(uint16_t handle)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

	//Handle value range 0x0000 - 0xFFFF
    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	r_ble_spp_generate_message(R_BLE_SPP_CMD_READ_HANDLE, (uint8_t *)&handle, R_BLE_SPP_LEN_READ_HANDLE);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_READ_HANDLE + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*************************************************************************************************************************
 * This function initiates Read blob characteristics of an attribute
 *
 * @param	handle		Attribute handle for readBlob characteristics
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 *************************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_ReadBlob(uint16_t handle)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

	//Handle value range 0x0000 - 0xFFFF
    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }
	r_ble_spp_generate_message(R_BLE_SPP_CMD_READ_BLOB, (uint8_t *)&handle, R_BLE_SPP_LEN_READ_BLOB);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_READ_BLOB + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/******************************************************************************************************************************
 * This function initiates the host to send the command that will request the server(slave) to write the value of an attribute
 * and acknowledge that this has been achieved in a Write Response
 *
 * @param	handle		Attribute Handle
 * @param	data		Payload
 * @param	length		data length
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 *******************************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_WriteCharacteristic(uint16_t handle, uint8_t *data, uint16_t length)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
//	uint8_t l_Data[length + R_BLE_SPP_HANDLE_LENGTH_SIZE];
    uint8_t l_Data[R_BLE_SPP_BUFF_SIZE];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if ((NULL == data) || (R_BLE_SPP_MAX_BUFFER_SIZE < length))
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	l_Data[0] = ENDIAN_LSB16(handle);
	l_Data[1] = ENDIAN_MSB16(handle);
	l_Data[2] = ENDIAN_LSB16(length);
	l_Data[3] = ENDIAN_MSB16(length);
	r_ble_spp_memcpy(&l_Data[4] , data ,length);
	r_ble_spp_generate_message(R_BLE_SPP_CMD_WRITE_CHARACTERISTIC, l_Data, (uint16_t)(length +R_BLE_SPP_HANDLE_LENGTH_SIZE));
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(length +R_BLE_SPP_HANDLE_LENGTH_SIZE + R_BLE_SPP_HEADER_TRAILER_SIZE));
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function initiates the host to send the  Write Command which is used to request the server to write the value
 * of an attribute, typically into a control-point attribute without response
 *
 * @param	handle		Attribute Handle
 * @param	data		Payload
 * @param	length		data length
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_WriteCharWithoutResponse(uint16_t handle, uint8_t *data, uint16_t length)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
//	uint8_t l_Data[length + R_BLE_SPP_HANDLE_LENGTH_SIZE];
    uint8_t l_Data[R_BLE_SPP_BUFF_SIZE];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if ((NULL == data) || (R_BLE_SPP_MAX_BUFFER_SIZE < length))
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

    l_Data[0] = ENDIAN_LSB16(handle);
	l_Data[1] = ENDIAN_MSB16(handle);
	l_Data[2] = ENDIAN_LSB16(length);
	l_Data[3] = ENDIAN_MSB16(length);
	r_ble_spp_memcpy(&l_Data[4] , data ,length);

	r_ble_spp_generate_message(R_BLE_SPP_CMD_WRITE_CHARACTERISTIC_NO_RESPONSE, l_Data, (uint16_t)(length + R_BLE_SPP_HANDLE_LENGTH_SIZE));
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)(length + R_BLE_SPP_HANDLE_LENGTH_SIZE) + R_BLE_SPP_HEADER_TRAILER_SIZE);
	if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function sets the pairing parameters required for the bonding and security features
 *
 * @param	p_pair_param	    contains io-capabilities,bonding modes,authentication methods
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 * @retval  R_BLE_APP_TRANSPORT_WRITE_ERROR Communication channel Write is Not successful
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetPairingParams(r_ble_spp_gap_pairing_param_t *p_pair_param)
{
    r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
    uint8_t data[R_BLE_SPP_LEN_SET_PAIRING_PARAMS];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == p_pair_param)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

    if (R_BLE_SPP_SEC_MODE_1_LEVEL_4 < p_pair_param->sec_level)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

    if (p_pair_param->sec_level == R_BLE_SPP_SEC_MODE_1_LEVEL_1)
    {
        //nothing
        data[0] = 0x00;//place holder
        data[1] = 0x00;//place holder
        data[3] = 0x00;//place holder
    }
    else
    {
        if ((((p_pair_param->iocap <= R_BLE_SPP_IO_CAP_KEYBOARD_DISPLAY))
           || (p_pair_param->iocap == R_BLE_SPP_IO_CAP_UNKNOWN ))
           && (p_pair_param->bonding <= R_BLE_SPP_BONDABLE_MODE))
        {
            if (p_pair_param->sec_level == R_BLE_SPP_SEC_MODE_1_LEVEL_2)
            {
                if ((p_pair_param->auth_methods <= R_BLE_SPP_SEC_LEVEL_2_METHOD_2))
                {
                    data[0]= p_pair_param->iocap;
                    data[1]= p_pair_param->bonding;
                    data[3]= p_pair_param->auth_methods;
                }
                else
                {
                    retVal = R_BLE_SPP_INVALID_PARAMETER;
                }
            }
            else
            {
                data[0]= p_pair_param->iocap;
                data[1]= p_pair_param->bonding;
                data[3]= 0x00;//Place holder
            }
        }
        else
        {
            retVal = R_BLE_SPP_INVALID_PARAMETER;
        }
    }
    data[2]= p_pair_param->sec_level;

    //Generate SPP command frame
    r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_PAIRING_PARAMS, data ,R_BLE_SPP_LEN_SET_PAIRING_PARAMS);
    retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_PAIRING_PARAMS + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
    {
        retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
    }
    return retVal;
}


/*********************************************************************************************************************
 * This function Enable/Disable address resolution and generation of a resolvable private address
 *
 *@param	enable to enable or disable RPA 1-enable 0- disable
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_EnableRpa(uint8_t enable)
{
	/* This function send advertising enable command */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (1 < enable)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_ENABLE_RPA, &enable ,R_BLE_SPP_LEN_SET_ENABLE_RPA);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_ENABLE_RPA + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function to configure address resolution and generation of a resolvable private address
 *
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_ConfigRpa(void)
{
	/* This function send advertising enable command */
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_ERROR;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }
    
    //Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_CONFIG_RPA, NULL ,R_BLE_SPP_LEN_SET_CONFIG_RPA);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_CONFIG_RPA + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function Enable/Disable address resolution and generation of a resolvable private address
 *
 *@param	oob_data_flag - to enable oob authentication. 1-enable 0- disable
 *@param	*oob_data		- oob data
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetRemOobData(uint8_t oob_data_flag, uint8_t *p_oob)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t data[R_BLE_SPP_LEN_SET_OOBDATA];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

    if (NULL == p_oob)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

    if (1 < oob_data_flag)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	data[0] = oob_data_flag;
	r_ble_spp_memcpy(&(data[1]), p_oob, R_BLE_SPP_LEN_SET_OOBDATA-1);

	//Generate SPP command frame
	r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_OOBDATA, data ,R_BLE_SPP_LEN_SET_OOBDATA);
	retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, R_BLE_SPP_LEN_SET_OOBDATA + R_BLE_SPP_HEADER_TRAILER_SIZE);
    if (R_BLE_SPP_SUCCESS != retVal)
	{
		retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function to set the attribute value
 *
 *@param	*p_value	consists of attribute handle, length of the data and data to be set in the attribute
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTS_SetAttr(r_ble_spp_gatt_hdl_value_pair_t *p_value)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
//	uint8_t l_Data[p_value->value.value_len + R_BLE_SPP_HANDLE_LENGTH_SIZE];
    uint8_t l_Data[R_BLE_SPP_BUFF_SIZE];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	if ((p_value->value.p_value != NULL) && (p_value->value.value_len <= R_BLE_SPP_MAX_BUFFER_SIZE))
	{
		l_Data[0] = ENDIAN_LSB16(p_value->attr_hdl);
		l_Data[1] = ENDIAN_MSB16(p_value->attr_hdl);
		l_Data[2] = ENDIAN_LSB16(p_value->value.value_len);
		l_Data[3] = ENDIAN_MSB16(p_value->value.value_len);

		r_ble_spp_memcpy(&(l_Data[4]) , p_value->value.p_value ,p_value->value.value_len);
		r_ble_spp_generate_message(R_BLE_SPP_CMD_SET_ATTR, l_Data , (uint16_t)(p_value->value.value_len +R_BLE_SPP_LEN_SET_ATTR));
		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, (uint32_t)p_value->value.value_len +R_BLE_SPP_LEN_SET_ATTR + R_BLE_SPP_HEADER_TRAILER_SIZE);
	    if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function to get the attribute value
 *
 *@param	*p_value	consists of attribute handle, length of the data to get from the attribute
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_OPEN  Communication Channel is not created
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTS_GetAttr(r_ble_spp_gatt_hdl_value_pair_t *p_value)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;
	uint8_t l_Data[4];

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        return R_BLE_SPP_DEVICE_NOT_OPEN;
    }

	if ((p_value->value.value_len != 0x0000) && (p_value->value.value_len <= R_BLE_SPP_MAX_BUFFER_SIZE))
	{
		l_Data[0] = ENDIAN_LSB16(p_value->attr_hdl);
		l_Data[1] = ENDIAN_MSB16(p_value->attr_hdl);
		l_Data[2] = ENDIAN_LSB16(p_value->value.value_len);
		l_Data[3] = ENDIAN_MSB16(p_value->value.value_len);

		r_ble_spp_generate_message(R_BLE_SPP_CMD_GET_ATTR, l_Data ,(uint16_t)(p_value->value.value_len +R_BLE_SPP_LEN_GET_ATTR));

		retVal = (r_ble_spp_cmd_rsp_t)wrap_sci_send(g_tx_data_buf, p_value->value.value_len + (uint32_t)(R_BLE_SPP_LEN_GET_ATTR + R_BLE_SPP_HEADER_TRAILER_SIZE));
	    if (R_BLE_SPP_SUCCESS != retVal)
		{
			retVal = R_BLE_APP_TRANSPORT_WRITE_ERROR;
		}
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}
	return retVal;
}

/*********************************************************************************************************************
 * This function closes the communication interface channel
 *
 *@param	p_api Contains open,read,write,close,mw_callback and context data to be used in wrapper
 *
 * @retval  R_BLE_SPP_SUCCESS		command frame is sent successfully over communication channel.
 * @retval  R_BLE_SPP_INVALID_PARAMETER  The parameters is invalid
 * @retval  R_BLE_SPP_DEVICE_NOT_CLOSED  Communication Channel is not closed
 **********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Close(r_ble_spp_cfg_t *p_api)
{
	r_ble_spp_cmd_rsp_t retVal = R_BLE_SPP_INVALID_PARAMETER;

    if (R_BLE_SPP_TRUE != g_r_ble_spp_open_complete)
    {
        retVal = R_BLE_SPP_DEVICE_NOT_OPEN ;
    }

    if (NULL == p_api)
    {
        return R_BLE_SPP_INVALID_PARAMETER;
    }

	if (R_BLE_SPP_SUCCESS != (r_ble_spp_cmd_rsp_t)wrap_sci_close())
	{
		retVal = R_BLE_SPP_DEVICE_NOT_CLOSED ;
	}
	else
	{
        retVal = R_BLE_SPP_SUCCESS ;
	    g_r_ble_spp_open_complete = R_BLE_SPP_FALSE;
	}
	return retVal;
}

/*********************************************************************************************************************
 * this function is used to pack the SPP Command ID and length of the Command
 * into  a tx data buffer
 *
 *@param   command          SPP command id
 *@param   *pay load         Pay load buffer
 *@param   payload_length   Length of the pay load
 * @retval R_BLE_SPP_SUCCESS        command message frame generated successfully.
 * @retval R_BLE_SPP_INVALID_PARAMETER The parameters is invalid
 **********************************************************************************************************************/
static r_ble_spp_cmd_rsp_t r_ble_spp_generate_message(uint16_t command, uint8_t *payload, uint16_t payload_length)
{
	/* This function is used for sending the COMMAND ID and LENGTH  to TX DATA Buffer */
	uint16_t checksum;
	r_ble_spp_cmd_rsp_t retVal;
	retVal = R_BLE_SPP_SUCCESS;

	g_tx_data_buf[0] = ENDIAN_LSB16(command);
	g_tx_data_buf[1] = ENDIAN_MSB16(command);
	g_tx_data_buf[2] = ENDIAN_LSB16(payload_length);
	g_tx_data_buf[3] = ENDIAN_MSB16(payload_length);

	if(((payload_length > 0) && (payload != NULL)) && (payload_length <= R_BLE_SPP_BUFF_SIZE))
	{
		r_ble_spp_memcpy(&g_tx_data_buf[4], payload, payload_length);
	}
	else
	{
		retVal = R_BLE_SPP_INVALID_PARAMETER;
	}

	checksum = r_ble_spp_fletcher16(g_tx_data_buf, payload_length + R_BLE_SPP_HEADER_SIZE);

	g_tx_data_buf[payload_length+4] = CHECKSUM_LSB(checksum);
	g_tx_data_buf[payload_length+5] = CHECKSUM_MSB(checksum);
	return retVal;
}

/*********************************************************************************************************************
 * this function is used to find the length of a string
 *
 * @param   *str    input string to calculate the length
 * @retval  length of the string
 **********************************************************************************************************************/
static uint8_t r_ble_spp_string_len(unsigned char* str)
{
	/* This function sends the Length of the DATA */
	uint8_t len = 0;
	if(*str)
	{
		while (str[len] != '\0')
		{
			len++;
		}
	}
	return len;
}

/*********************************************************************************************************************
 * user-defined memcpy function to copy the data from one buffer to another
 *
 * @param   *dst   destination buffer to the data
 * @param   *src    source buffer
 * @param   cnt     no of bytes to be copied from source to destination
 * @retval  NA
 **********************************************************************************************************************/
static void  r_ble_spp_memcpy(void* dst, const void* src, uint16_t cnt)
{
	/* This function copy the Data based on length from Source to Destination */
	char *dst_buf = (char *)dst;
	const char *src_buf =( const char*)src;
	if((dst_buf!= NULL) && (src_buf!= NULL))
	{
		while(cnt) //till cnt
		{
			//Copy byte by byte
			*(dst_buf++)= *(src_buf++);
			--cnt;
		}
	}
}

/*********************************************************************************************************************
 * checksum is calculated in this function.
 *
 * @param   pd           byte data to perform the checksum operation
 * @retval  checksum     calculated checksum is returned.
 **********************************************************************************************************************/
static uint16_t r_ble_spp_event_fletcher16(uint8_t pd)
{
	g_event_cksum1 = (uint16_t)((g_event_cksum1 + pd) % 255);
	g_event_cksum2 = (uint16_t)((g_event_cksum2 + g_event_cksum1) % 255);

	return (uint16_t)((g_event_cksum2 << 8) | g_event_cksum1);
}

/*********************************************************************************************************************
 * checksum is calculated in this function.
 *
 * @param   *pd          data buffer to perform the checksum operation
 * @param   count        size of the data buffer
 * @retval  checksum     calculated checksum is returned.
 **********************************************************************************************************************/
static uint16_t r_ble_spp_fletcher16(uint8_t *pd, int count)
{
	uint16_t sum1=0;
	uint16_t sum2=0;
	uint16_t index = 0;
	for (index = 0; index < count; ++index)
	{
		sum1 = (uint16_t)((sum1 + pd[index]) % 255);
		sum2 = (uint16_t)(sum2 + sum1) % 255;
	}
	return (uint16_t)(sum2 << 8) | sum1;
}

/*********************************************************************************************************************
 * this function is invoked for every time there is data/ISR call. It gets the event frame data byte by byte.
 * Once the complete frame is received it process the received data and fill the payload to send it to application
 *
 * @param   byte    byte data received from BLE module on uart rx buffer
 * @retval  NA
 **********************************************************************************************************************/
void r_ble_spp_rx_char_callback(uint8_t byte)
{
	static uint8_t rx_index = 0;
	uint16_t recvd_checksum = 0;
	uint8_t index_count=0;

//    wrap_sci_ctrl(SCI_CMD_RX_Q_FLUSH, NULL);

	/* Checking for leading FFs (this may happen in cases of 8258 powerup)
	There should be no messages coming in that are 0xFF bytes long,
	if this happens then we have received an errant 0xFF.*/
	if ((rx_index == 1) && (byte == 0xFF))
	{
		rx_index--;
	}
	/*store our new character to the buffer*/
	g_rx_data_buf[rx_index] = byte;

	/* Checking for end of message */
	/*If we have a length parameter already sent*/
	if ((g_rx_data_buf[0] == 0xFF))
	{
		if ((rx_index == 0) || (rx_index < g_rx_data_buf[1]+2))
		{
			g_event_cksum = r_ble_spp_event_fletcher16(byte);
		}
		if(rx_index > 1)
		{
			/*If our buffer has reached the last byte in this message*/
			if (rx_index > (g_rx_data_buf[1] + 2))
			{
				index_count = (uint8_t) (rx_index + 1);
				recvd_checksum = BYTES_TO_U16(g_rx_data_buf[index_count-1], g_rx_data_buf[index_count-2]);

				if (recvd_checksum != g_event_cksum)
				{
					/* handle error msg */
					g_p_payload.event_id = 0xFFFF;
				}
				else
				{
					/* Payload buffer is filled to be used by Application */
					g_p_payload.event_id = BYTES_TO_U16(g_rx_data_buf[3], g_rx_data_buf[2]);
					g_p_payload.out_len = g_rx_data_buf[1];

					r_ble_spp_memcpy(&g_payload_buf[0], g_rx_data_buf+4, index_count);

					g_p_payload.out_data = g_payload_buf;
				}

				g_r_ble_spp_cfg_internal.mw_callback(&g_p_payload);

				rx_index = 0;
				g_event_cksum = 0;
				g_event_cksum1 = 0;
				g_event_cksum2 = 0;
				g_p_payload.out_len = 0;
				g_p_payload.out_data = '\0';
				g_p_payload.event_id = 0;
				return;
			}
		}
	}
	else
	{
		/* update app with error event */
		return;
	}
	rx_index++;
}
