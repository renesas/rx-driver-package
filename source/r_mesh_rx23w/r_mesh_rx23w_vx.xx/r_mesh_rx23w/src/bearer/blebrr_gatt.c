
/**
 *  \file blebrr_gatt.c
 *
 *  This File contains the BLE Bearer interface for the GATT bearer over
 *  Mindtree Mesh stack.
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "blebrr.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/* GATT Proxy Segmentation and Reassembly related states */
#define BLEBRR_GATT_SAR_COMPLETE_PKT        0x00
#define BLEBRR_GATT_SAR_START_PKT           0x01
#define BLEBRR_GATT_SAR_CONTINUE_PKT        0x02
#define BLEBRR_GATT_SAR_END_PKT             0x03

/* GATT Proxy Segmentation and Reassembly related state values */
#define BLEBRR_GATT_SAR_INIT_STATE          0x00
#define BLEBRR_GATT_SAR_TX_PROGRESS         0x01

/* GATT PDU Size in octets */
#define BLEBRR_GATT_PDU_SIZE                75

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
/** GATT bearer Information */
static BRR_BEARER_INFO blebrr_gatt;

/* TODO: Check if the MTU be global or part of specific context */
/** GATT bearer specific identifiers */
static BRR_BEARER_CH_INFO blebrr_gatt_ch_info;

/** GATT Current mode identifier - PROV or PROXY */
static UCHAR blebrr_gatt_mode = 0xFF;

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Sets new GATT bearer mode
*******************************************************************************/
void blebrr_gatt_mode_set(UCHAR flag)
{
    /**
     * The valid values for 'flag' are:
     *  0x00: BLEBRR_GATT_PROV_MODE  GATT Provisioning Mode
     *  0x01: BLEBRR_GATT_PROXY_MODE GATT Proxy Mode
     *  All other values are RFU.
     */
     blebrr_gatt_mode = ((flag == BLEBRR_GATT_PROV_MODE) || (flag == BLEBRR_GATT_PROXY_MODE))?
                        flag: 0xFF;

     /* Notify GATT mode setting to blebrr pl */
     blebrr_set_gattmode_pl (blebrr_gatt_mode);
}

/***************************************************************************//**
* @brief Gets current GATT bearer mode
*******************************************************************************/
UCHAR blebrr_gatt_mode_get(void)
{
    /**
     * The valid return values are:
     *  0x00: BLEBRR_GATT_PROV_MODE  GATT Provisioning Mode
     *  0x01: BLEBRR_GATT_PROXY_MODE GATT Proxy Mode
     *  All other values are RFU.
     */
   return blebrr_gatt_mode;
}

static API_RESULT blebrr_gatt_send
                       (
                           BRR_HANDLE * handle,
                           UCHAR type,
                           void * pdata,
                           UINT16 datalen
                       )
{
    API_RESULT retval;

    MS_IGNORE_UNUSED_PARAM(type);

    retval = blebrr_gatt_send_pl
             (
                 handle,
                 pdata,
                 datalen
             );

    if (API_SUCCESS != retval)
    {
        BLEBRR_LOG("Error - 0x%04X\n", retval);
    }

    return retval;
}

/***************************************************************************//**
* @brief Handles packet received over GATT Bearer
*******************************************************************************/
API_RESULT blebrr_pl_recv_gattpacket
           (
               BRR_HANDLE * handle,
               UCHAR * pdata,
               UINT16 pdatalen
           )
{
    if (NULL != blebrr_gatt.bearer_recv)
    {
        blebrr_gatt.bearer_recv
        (
            handle,
            pdata,
            pdatalen,
            NULL
        );
    }

    return API_SUCCESS;
}

/***************************************************************************//**
* @brief Registers GATT Bearer with Mesh Stack
*******************************************************************************/
API_RESULT blebrr_pl_gatt_connection
           (
               BRR_HANDLE * handle,
               UCHAR        role,
               UCHAR        mode,
               UINT16       mtu
           )
{
    MS_BUFFER buffer;
    API_RESULT retval;

    /* Initialize */
    retval = API_FAILURE;

    /* Populate the Bearer GATT Channel info */
    blebrr_gatt_ch_info.role = role;
    blebrr_gatt_ch_info.mtu  = mtu;

    /* Add bearer to the mesh network */
    blebrr_gatt.bearer_send = blebrr_gatt_send;
    blebrr_gatt.binfo = &buffer;
    buffer.payload = (UCHAR*)&blebrr_gatt_ch_info;
    buffer.length = sizeof(blebrr_gatt_ch_info);
    retval = MS_brr_add_bearer(BRR_TYPE_GATT, &blebrr_gatt, handle);

    /* Check the PDU type received and Add bearer to Mesh stack */
    if (BLEBRR_GATT_PROXY_MODE == mode)
    {
        if (BLEBRR_SERVER_ROLE == role)
        {
            /* Start observing */
            blebrr_scan_enable();
        }
        else if (BLEBRR_CLIENT_ROLE == role)
        {
            /* Do Nothing */
            /**
             * Currently, not enabling scan for Proxy Client.
             * Typically, Proxy Client supports only GATT Bearer.
             * Hence, not initiating 'SCAN' on Bearer UP event.
             */
        }
    }

    return retval;
}

/***************************************************************************//**
* @brief Unregisters GATT Bearer from Mesh Stack
*******************************************************************************/
API_RESULT blebrr_pl_gatt_disconnection
           (
               BRR_HANDLE * handle
           )
{
    API_RESULT retval;

    retval = MS_brr_remove_bearer(BRR_TYPE_GATT, handle);

    return retval;
}

