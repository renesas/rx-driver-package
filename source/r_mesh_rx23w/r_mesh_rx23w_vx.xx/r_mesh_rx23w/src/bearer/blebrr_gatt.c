
/**
 *  \file blebrr_gatt.c
 *
 *  This File contains the BLE Bearer interface for the GATT bearer over
 *  Mindtree Mesh stack.
 */

/*
 *  Copyright (C) 2016-2021. Mindtree Ltd.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "blebrr.h"

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
/** GATT bearer Information */
static BRR_BEARER_INFO blebrr_gatt;

/* TODO: Check if the MTU be global or part of specific context */
/** GATT bearer specific identifiers */
static BRR_BEARER_CH_INFO blebrr_gatt_ch_info;

/** GATT Current mode identifier - PROV or PROXY */
static UCHAR blebrr_gatt_mode = BLEBRR_GATT_UNINIT_MODE;

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Sets new GATT bearer mode
*******************************************************************************/
void R_MS_BRR_Set_GattMode(UCHAR mode)
{
    /**
     * The valid values for 'mode' are:
     *  0x00: BLEBRR_GATT_PROV_MODE  GATT Provisioning Mode
     *  0x01: BLEBRR_GATT_PROXY_MODE GATT Proxy Mode
     *  All other values are RFU.
     */
    if ((BLEBRR_GATT_PROV_MODE == mode) || (BLEBRR_GATT_PROXY_MODE == mode))
    {
        if (blebrr_gatt_mode != mode)
        {
            blebrr_gatt_mode = mode;

            /* Notify GATT mode setting to blebrr pl */
            blebrr_set_gattmode_pl(blebrr_gatt_mode);
        }
    }
}

/***************************************************************************//**
* @brief Gets current GATT bearer mode
*******************************************************************************/
UCHAR R_MS_BRR_Get_GattMode(void)
{
    /**
     * The valid return values are:
     *  0x00: BLEBRR_GATT_PROV_MODE  GATT Provisioning Mode
     *  0x01: BLEBRR_GATT_PROXY_MODE GATT Proxy Mode
     *  All other values are RFU.
     */
   return blebrr_gatt_mode;
}

static API_RESULT blebrr_gatt_send(BRR_HANDLE * handle, UCHAR  type, void * pdata, UINT16 datalen)
{
    API_RESULT retval;

    MS_IGNORE_UNUSED_PARAM(type);

    retval = blebrr_gatt_send_pl(handle, pdata, datalen);

    return retval;
}

/***************************************************************************//**
* @brief Handles packet received over GATT Bearer
*******************************************************************************/
API_RESULT blebrr_pl_recv_gattpacket(BRR_HANDLE * handle, UCHAR * pdata, UINT16 pdatalen)
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
API_RESULT blebrr_pl_gatt_connection(BRR_HANDLE * handle, UCHAR role, UINT16 mtu, UCHAR mode)
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

    if (BLEBRR_GATT_PROXY_MODE == mode)
    {
        if (BLEBRR_CLIENT_ROLE == role)
        {
            /**
             * Disable ADV Bearer for Proxy Client
             * Typically, Proxy Client supports only GATT Bearer.
             */
            blebrr_adv_disable();
        }
    }

    return retval;
}

/***************************************************************************//**
* @brief Unregisters GATT Bearer from Mesh Stack
*******************************************************************************/
API_RESULT blebrr_pl_gatt_disconnection(BRR_HANDLE * handle, UCHAR role, UCHAR mode)
{
    API_RESULT retval;

    retval = MS_brr_remove_bearer(BRR_TYPE_GATT, handle);

    if (BLEBRR_GATT_PROXY_MODE == mode)
    {
        if (BLEBRR_CLIENT_ROLE == role)
        {
            blebrr_adv_enable();
        }
    }

    return retval;
}
