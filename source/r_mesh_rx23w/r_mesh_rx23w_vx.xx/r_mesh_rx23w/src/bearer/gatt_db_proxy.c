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
* Copyright (C) 2019-2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/**
 *  GATT DATABASE QUICK REFERENCE TABLE:
 *  Abbreviations used for Properties:
 *      RD  = Read
 *      WWR = Write Without Response
 *      WR  = Write
 *      SWR = Signed Write
 *      NTF = Notification
 *      IND = Indication
 *
 *    Handle  |  Type                                            |  Properties  |  Value                           
 *  ==========|==================================================|==============|==================================
 *    0x0001  |  0x2800 <<Primary Service>>                      |  RD          |  0x1800 <<Generic Access>>       
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x0002  |  0x2803 <<Characteristic>>                       |  RD          |  Properties,Value Handle,UUID    
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x0003  |  0x2A00 <<Device Name>>                          |  RD          |  "Renesas RX23W Bluetooth Mesh"  
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x0004  |  0x2803 <<Characteristic>>                       |  RD          |  Properties,Value Handle,UUID    
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x0005  |  0x2A01 <<Appearance>>                           |  RD          |  0 "Unknown"                     
 *  ==========|==================================================|==============|==================================
 *    0x0006  |  0x2800 <<Primary Service>>                      |  RD          |  0x1801 <<Generic Attribute>>    
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x0007  |  0x2803 <<Characteristic>>                       |  RD          |  Properties,Value Handle,UUID    
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x0008  |  0x2A05 <<Service Changed>>                      |  IND         |  N/A                             
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x0009  |  0x2902 <<Client Characteristic Configuration>>  |  RD, WR      |  0x0000                          
 *  ==========|==================================================|==============|==================================
 *    0x000A  |  0x2800 <<Primary Service>>                      |  RD          |  0x1828 <<Mesh Proxy>>           
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x000B  |  0x2803 <<Characteristic>>                       |  RD          |  Properties,Value Handle,UUID    
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x000C  |  0x2ADD <<Mesh Proxy Data In>>                   |  WWR         |  0                               
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x000D  |  0x2803 <<Characteristic>>                       |  RD          |  Properties,Value Handle,UUID    
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x000E  |  0x2ADE <<Mesh Proxy Data Out>>                  |  NTF         |  N/A                             
 *  ----------|--------------------------------------------------|--------------|----------------------------------
 *    0x000F  |  0x2902 <<Client Characteristic Configuration>>  |  RD, WR      |  0x0000                          
 *  ==========|==================================================|==============|==================================
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
#include "r_ble_rx23w_if.h"

#include "gatt_db_proxy.h"

/*****************************************************************************
* Global Variables definition
******************************************************************************/
static const uint8_t gs_gatt_const_uuid_arr[] =
{
    /* 0 - Primary Service Declaration */
    0x00, 0x28,

    /* 2 - Secondary Service Declaration */
    0x01, 0x28,

    /* 4 - Include Declaration */
    0x02, 0x28,

    /* 6 - Characteristic Declaration */
    0x03, 0x28,

    /* 8 - Generic Access */
    0x00, 0x18,

    /* 10 - Device Name */
    0x00, 0x2A,

    /* 12 - Appearance */
    0x01, 0x2A,

    /* 14 - Generic Attribute */
    0x01, 0x18,

    /* 16 - Service Changed */
    0x05, 0x2A,

    /* 18 - Client Characteristic Configuration */
    0x02, 0x29,

    /* 20 - Mesh Proxy */
    0x28, 0x18,

    /* 22 - Mesh Proxy Data In */
    0xDD, 0x2A,

    /* 24 - Mesh Proxy Data Out */
    0xDE, 0x2A,

};

static uint8_t gs_gatt_value_arr[] =
{
    /* 0 - Device Name */
    'R', 'e', 'n', 'e', 's', 'a', 's', ' ', 'R', 'X', '2', '3', 'W', ' ',
    'B', 'l', 'u', 'e', 't', 'o', 'o', 't', 'h', ' ', 'M', 'e', 's', 'h',
    0x00,

    /* 29 - Appearance */
    0x00, 0x00,

    /* 31 - Mesh Proxy Data In */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,

};

static const uint8_t gs_gatt_const_value_arr[] =
{
    /* 0 - Device Name */
    0x0A,       // Properties
    0x03, 0x00, // Attr Handle
    0x00, 0x2A, // UUID

    /* 5 - Appearance */
    0x02,       // Properties
    0x05, 0x00, // Attr Handle
    0x01, 0x2A, // UUID

    /* 10 - Service Changed */
    0x20,       // Properties
    0x08, 0x00, // Attr Handle
    0x05, 0x2A, // UUID

    /* 15 - Mesh Proxy Data In */
    0x04,       // Properties
    0x0C, 0x00, // Attr Handle
    0xDD, 0x2A, // UUID

    /* 20 - Mesh Proxy Data Out */
    0x10,       // Properties
    0x0E, 0x00, // Attr Handle
    0xDE, 0x2A, // UUID

};

static const uint8_t gs_gatt_db_const_peer_specific_val_arr[] =
{
    /* 0 - Service Changed : Client Characteristic Configuration */
    0x00, 0x00,

    /* 2 - Mesh Proxy Data Out : Client Characteristic Configuration */
    0x00, 0x00,

};

static uint8_t gs_gatt_db_peer_specific_val_arr[sizeof(gs_gatt_db_const_peer_specific_val_arr)*BLE_CFG_RF_CONN_MAX];

static const st_ble_gatts_db_uuid_cfg_t gs_gatt_type_table[] =
{
    /* 0 - Primary Service Declaration */
    {
        /* UUID Offset */
        0,
        /* First Occurrence for Type */
        0x0001,
        /* Last Occurrence for Type */
        0x000A,
    },

    /* 1 - Generic Access */
    {
        /* UUID Offset */
        8,
        /* First Occurrence for Type */
        0x0001,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 2 - Characteristic Declaration */
    {
        /* UUID Offset */
        6,
        /* First Occurrence for Type */
        0x0002,
        /* Last Occurrence for Type */
        0x000D,
    },

    /* 3 - Device Name */
    {
        /* UUID Offset */
        10,
        /* First Occurrence for Type */
        0x0003,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 4 - Appearance */
    {
        /* UUID Offset */
        12,
        /* First Occurrence for Type */
        0x0005,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 5 - Generic Attribute */
    {
        /* UUID Offset */
        14,
        /* First Occurrence for Type */
        0x0006,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 6 - Service Changed */
    {
        /* UUID Offset */
        16,
        /* First Occurrence for Type */
        0x0008,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 7 - Client Characteristic Configuration */
    {
        /* UUID Offset */
        18,
        /* First Occurrence for Type */
        0x0009,
        /* Last Occurrence for Type */
        0x000F,
    },

    /* 8 - Mesh Proxy */
    {
        /* UUID Offset */
        20,
        /* First Occurrence for Type */
        0x000A,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 9 - Mesh Proxy Data In */
    {
        /* UUID Offset */
        22,
        /* First Occurrence for Type */
        0x000C,
        /* Last Occurrence for Type */
        0x0000,
    },

    /* 10 - Mesh Proxy Data Out */
    {
        /* UUID Offset */
        24,
        /* First Occurrence for Type */
        0x000E,
        /* Last Occurrence for Type */
        0x0000,
    },

};

static const st_ble_gatts_db_attr_cfg_t gs_gatt_db_attr_table[] =
{
    /* Handle : 0x0000 */
    /* Blank */
    {
        /* Properties */
        0,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        1,
        /* Next Attribute Type Index */
        0x0001,
        /* UUID Offset */
        0,
        /* Value */
        NULL,
    },

    /* Handle : 0x0001 */
    /* Generic Access : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0006,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 8),
    },

    /* Handle : 0x0002 */
    /* Device Name : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0004,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 0),
    },

    /* Handle : 0x0003 */
    /* Device Name */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        29,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        10,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 0),
    },

    /* Handle : 0x0004 */
    /* Appearance : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0007,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 5),
    },

    /* Handle : 0x0005 */
    /* Appearance */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        12,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 29),
    },

    /* Handle : 0x0006 */
    /* Generic Attribute : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x000A,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 14),
    },

    /* Handle : 0x0007 */
    /* Service Changed : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x000B,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 10),
    },

    /* Handle : 0x0008 */
    /* Service Changed */
    {
        /* Properties */
        0,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        4,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        16,
        /* Value */
        (NULL),
    },

    /* Handle : 0x0009 */
    /* Service Changed : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x000F,
        /* UUID Offset */
        18,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 0),
    },

    /* Handle : 0x000A */
    /* Mesh Proxy : Primary Service Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        0,
        /* Value */
        (uint8_t *)(gs_gatt_const_uuid_arr + 20),
    },

    /* Handle : 0x000B */
    /* Mesh Proxy Data In : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x000D,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 15),
    },

    /* Handle : 0x000C */
    /* Mesh Proxy Data In */
    {
        /* Properties */
        BLE_GATT_DB_WRITE_WITHOUT_RSP,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        66,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        22,
        /* Value */
        (uint8_t *)(gs_gatt_value_arr + 31),
    },

    /* Handle : 0x000D */
    /* Mesh Proxy Data Out : Characteristic Declaration */
    {
        /* Properties */
        BLE_GATT_DB_READ,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        5,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        6,
        /* Value */
        (uint8_t *)(gs_gatt_const_value_arr + 20),
    },

    /* Handle : 0x000E */
    /* Mesh Proxy Data Out */
    {
        /* Properties */
        0,
        /* Auxiliary Properties */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,
        /* Value Size */
        66,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        24,
        /* Value */
        (NULL),
    },

    /* Handle : 0x000F */
    /* Mesh Proxy Data Out : Client Characteristic Configuration */
    {
        /* Properties */
        BLE_GATT_DB_READ | BLE_GATT_DB_WRITE,
        /* Auxiliary Properties */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,
        /* Value Size */
        2,
        /* Next Attribute Type Index */
        0x0000,
        /* UUID Offset */
        18,
        /* Value */
        (uint8_t *)(gs_gatt_db_peer_specific_val_arr + 2),
    },

};

static const st_ble_gatts_db_char_cfg_t gs_gatt_characteristic[] =
{
    /* 0 - Device Name */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0002,
        /* Service Index */
        0,
    },

    /* 1 - Appearance */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x0004,
        /* Service Index */
        0,
    },

    /* 2 - Service Changed */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x0007,
        /* Service Index */
        1,
    },

    /* 3 - Mesh Proxy Data In */
    {
        /* Number of Attributes */
        {
            2,
        },
        /* Start Handle */
        0x000B,
        /* Service Index */
        2,
    },

    /* 4 - Mesh Proxy Data Out */
    {
        /* Number of Attributes */
        {
            3,
        },
        /* Start Handle */
        0x000D,
        /* Service Index */
        2,
    },

};

static const st_ble_gatts_db_serv_cfg_t gs_gatt_service[] =
{
    /* Generic Access */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x0001,
        /* Service End Handle */
        0x0005,
        /* Characteristic Start Index */
        0,
        /* Characteristic End Index */
        1,
    },

    /* Generic Attribute */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x0006,
        /* Service End Handle */
        0x0009,
        /* Characteristic Start Index */
        2,
        /* Characteristic End Index */
        2,
    },

    /* Mesh Proxy */
    {
        /* Num of Services */
        {
            1,
        },
        /* Description */
        0,
        /* Service Start Handle */
        0x000A,
        /* Service End Handle */
        0x000F,
        /* Characteristic Start Index */
        3,
        /* Characteristic End Index */
        4,
    },

};

st_ble_gatts_db_cfg_t g_gatt_db_table_mesh_proxy =
{
    gs_gatt_const_uuid_arr,
    gs_gatt_value_arr,
    gs_gatt_const_value_arr,
    gs_gatt_db_peer_specific_val_arr,
    gs_gatt_db_const_peer_specific_val_arr,
    gs_gatt_type_table,
    gs_gatt_db_attr_table,
    gs_gatt_characteristic,
    gs_gatt_service,
    ARRAY_SIZE(gs_gatt_service),
    ARRAY_SIZE(gs_gatt_characteristic),
    ARRAY_SIZE(gs_gatt_type_table),
    ARRAY_SIZE(gs_gatt_db_const_peer_specific_val_arr),
};