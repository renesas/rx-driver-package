
/**
 *  \file gatt_db_proxy.c
 *  GATT Databse.
 */
/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/**
 *  GATT DATABASE QUICK REFERENCE TABLE:
 *  Abbreviations used for Permission:
 *      Rd  = Read
 *      WwR = Write Without Response
 *      Wr  = Write
 *      Swr  = Signed Write
 *      Ntf = Notification
 *      Ind = Indication
 *
 *  Handle  |      ATT_Type      |    Permission    |  ATT_VALUE
 *  ========+====================+==================+=====================
 *  0x0001  |       0x2800       | Rd               | 0x1800
 *  --------+--------------------+------------------+---------------------
 *  0x0002  |       0x2803       | Rd               | 0x02, 0x0003, 0x2A00
 *  --------+--------------------+------------------+---------------------
 *  0x0003  |       0x2A00       | Rd               | MT_Mesh_Proxy_Service
 *  --------+--------------------+------------------+---------------------
 *  0x0004  |       0x2803       | Rd               | 0x02, 0x0005, 0x2A01
 *  --------+--------------------+------------------+---------------------
 *  0x0005  |       0x2A01       | Rd               | 833.0
 *  ========+====================+==================+=====================
 *  0x0006  |       0x2800       | Rd               | 0x1801
 *  --------+--------------------+------------------+---------------------
 *  0x0007  |       0x2803       | Rd               | 0x20, 0x0008, 0x2A05
 *  --------+--------------------+------------------+---------------------
 *  0x0008  |       0x2A05       | Ind              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0009  |       0x2902       | Rd, Wr           | 0x0000
 *  ========+====================+==================+=====================
 *  0x000A  |       0x2800       | Rd               | 0x180F
 *  --------+--------------------+------------------+---------------------
 *  0x000B  |       0x2803       | Rd               | 0x12, 0x000C, 0x2A19
 *  --------+--------------------+------------------+---------------------
 *  0x000C  |       0x2A19       | Rd, Ntf          | 100.0
 *  --------+--------------------+------------------+---------------------
 *  0x000D  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x000E  |       0x2904       | Rd               | VALUE
 *  ========+====================+==================+=====================
 *  0x000F  |       0x2800       | Rd               | 0x180A
 *  --------+--------------------+------------------+---------------------
 *  0x0010  |       0x2803       | Rd               | 0x02, 0x0011, 0x2A29
 *  --------+--------------------+------------------+---------------------
 *  0x0011  |       0x2A29       | Rd               | Mindtree Limited.
 *  --------+--------------------+------------------+---------------------
 *  0x0012  |       0x2803       | Rd               | 0x02, 0x0013, 0x2A24
 *  --------+--------------------+------------------+---------------------
 *  0x0013  |       0x2A24       | Rd               | HRS-1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0014  |       0x2803       | Rd               | 0x02, 0x0015, 0x2A25
 *  --------+--------------------+------------------+---------------------
 *  0x0015  |       0x2A25       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0016  |       0x2803       | Rd               | 0x02, 0x0017, 0x2A26
 *  --------+--------------------+------------------+---------------------
 *  0x0017  |       0x2A26       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0018  |       0x2803       | Rd               | 0x02, 0x0019, 0x2A27
 *  --------+--------------------+------------------+---------------------
 *  0x0019  |       0x2A27       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x001A  |       0x2803       | Rd               | 0x02, 0x001B, 0x2A28
 *  --------+--------------------+------------------+---------------------
 *  0x001B  |       0x2A28       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x001C  |       0x2803       | Rd               | 0x02, 0x001D, 0x2A23
 *  --------+--------------------+------------------+---------------------
 *  0x001D  |       0x2A23       | Rd               | 11223344.0
 *  --------+--------------------+------------------+---------------------
 *  0x001E  |       0x2803       | Rd               | 0x02, 0x001F, 0x2A2A
 *  --------+--------------------+------------------+---------------------
 *  0x001F  |       0x2A2A       | Rd               | 0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0020  |       0x2803       | Rd               | 0x02, 0x0021, 0x2A50
 *  --------+--------------------+------------------+---------------------
 *  0x0021  |       0x2A50       | Rd               | 0
 *  ========+====================+==================+=====================
 *  0x0022  |       0x2800       | Rd               | 0x1828
 *  --------+--------------------+------------------+---------------------
 *  0x0023  |       0x2803       | Rd               | 0x04, 0x0024, 0x2ADD
 *  --------+--------------------+------------------+---------------------
 *  0x0024  |       0x2ADD       | Wwr              | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0025  |       0x2803       | Rd               | 0x10, 0x0026, 0x2ADE
 *  --------+--------------------+------------------+---------------------
 *  0x0026  |       0x2ADE       | Ntf              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0027  |       0x2902       | Rd, Wr           | 0x0000
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_ble_rx23w_if.h"
#include "gatt_db_proxy.h"

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
const uint8_t gatt_const_uuid_mesh_proxy[] =
{
    /* 0 - Primary Service UUID */
    0x00, 0x28,

    /* 2 - Secondary Service UUID */
    0x01, 0x28,

    /* 4 - Include UUID */
    0x02, 0x28,

    /* 6 - Characteristic UUID */
    0x03, 0x28,

    /* 8 - Characteristic Extended Properties UUID */
    0x00, 0x29,

    /* 10 - Characteristic User Description UUID */
    0x01, 0x29,

    /* 12 - Client Configuration UUID */
    0x02, 0x29,

    /* 14 - Server Configuration UUID */
    0x03, 0x29,

    /* 16 - Characteristic Format UUID */
    0x04, 0x29,

    /* 18 - Characteristic Aggregate Format UUID */
    0x05, 0x29,

    /* 20 - GAP Service UUID */
    0x00, 0x18,

    /* 22 - DeviceName Characteristic UUID */
    0x00, 0x2A,

    /* 24 - Appearance Characteristic UUID */
    0x01, 0x2A,

    /* 26 - GATT Service UUID */
    0x01, 0x18,

    /* 28 - Service Changed Characteristic UUID */
    0x05, 0x2A,

    /* 30 - Battery Service UUID */
    0x0F, 0x18,

    /* 32 - BatteryLevel Characteristic UUID */
    0x19, 0x2A,

    /* 34 - DeviceInformation Service UUID */
    0x0A, 0x18,

    /* 36 - ManufacturerName Characteristic UUID */
    0x29, 0x2A,

    /* 38 - ModelNumber Characteristic UUID */
    0x24, 0x2A,

    /* 40 - SerialNumber Characteristic UUID */
    0x25, 0x2A,

    /* 42 - FirmwareRevision Characteristic UUID */
    0x26, 0x2A,

    /* 44 - HardwareRevision Characteristic UUID */
    0x27, 0x2A,

    /* 46 - SoftwareRevision Characteristic UUID */
    0x28, 0x2A,

    /* 48 - SystemId Characteristic UUID */
    0x23, 0x2A,

    /* 50 - RegCertDataList Characteristic UUID */
    0x2A, 0x2A,

    /* 52 - PnPID Characteristic UUID */
    0x50, 0x2A,

    /* 54 - Mesh Proxy Service UUID */
    0x28, 0x18,

    /* 56 - Mesh Proxy Data IN Characteristic UUID */
    0xDD, 0x2A,

    /* 58 - Mesh Proxy Data OUT Characteristic UUID */
    0xDE, 0x2A,

};


uint8_t gatt_value_mesh_proxy[] =
{
    /* 0 - BatteryLevel(3); Default: 100.0 */
    0x64,

    /* 1 - Mesh Proxy Data IN(13); Default: 0 */
    0x42,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 68 - Mesh Proxy Data OUT(14); Default: N/A */
    0x42,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


const uint8_t gatt_const_value_mesh_proxy[] =
{

    /* 0 - DeviceName(0); Property: 0x02, Value Handle: 0x0003, UUID: 0x2A00 */
    0x02, 0x03, 0x00, 0x00, 0x2A,

    /* 5 - DeviceName(0); Default: MT_Mesh_Proxy_Service */
    0x4D, 0x54, 0x5F, 0x4D, 0x65, 0x73, 0x68, 0x5F, 0x50, 0x72, 0x6F, 0x78,
    0x79, 0x5F, 0x53, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65,

    /* 26 - Appearance(1); Property: 0x02, Value Handle: 0x0005, UUID: 0x2A01 */
    0x02, 0x05, 0x00, 0x01, 0x2A,

    /* 31 - Appearance(1); Default: 833.0 */
    0x03, 0x41,

    /* 33 - Service Changed(2); Property: 0x20, Value Handle: 0x0008, UUID: 0x2A05 */
    0x20, 0x08, 0x00, 0x05, 0x2A,

    /* 38 - BatteryLevel(3); Property: 0x12, Value Handle: 0x000C, UUID: 0x2A19 */
    0x12, 0x0C, 0x00, 0x19, 0x2A,

    /* 43 - Presentation Fromat for BatteryLevel */
    /**
     * Characteristic Presentation Format for BatteryLevel
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x04, 0x00, 0xAD, 0x27, 0x01, 0x00, 0x00,

    /* 50 - ManufacturerName(4); Property: 0x02, Value Handle: 0x0011, UUID: 0x2A29 */
    0x02, 0x11, 0x00, 0x29, 0x2A,

    /* 55 - ManufacturerName(4); Default: Mindtree Limited. */
    0x4D, 0x69, 0x6E, 0x64, 0x74, 0x72, 0x65, 0x65, 0x20, 0x4C, 0x69, 0x6D,
    0x69, 0x74, 0x65, 0x64, 0x2E,

    /* 72 - ModelNumber(5); Property: 0x02, Value Handle: 0x0013, UUID: 0x2A24 */
    0x02, 0x13, 0x00, 0x24, 0x2A,

    /* 77 - ModelNumber(5); Default: HRS-1.0.0 */
    0x48, 0x52, 0x53, 0x2D, 0x31, 0x2E, 0x30, 0x2E, 0x30,

    /* 86 - SerialNumber(6); Property: 0x02, Value Handle: 0x0015, UUID: 0x2A25 */
    0x02, 0x15, 0x00, 0x25, 0x2A,

    /* 91 - SerialNumber(6); Default: 1.0.0 */
    0x31, 0x2E, 0x30, 0x2E, 0x30,

    /* 96 - FirmwareRevision(7); Property: 0x02, Value Handle: 0x0017, UUID: 0x2A26 */
    0x02, 0x17, 0x00, 0x26, 0x2A,

    /* 101 - FirmwareRevision(7); Default: 1.0.0 */
    0x31, 0x2E, 0x30, 0x2E, 0x30,

    /* 106 - HardwareRevision(8); Property: 0x02, Value Handle: 0x0019, UUID: 0x2A27 */
    0x02, 0x19, 0x00, 0x27, 0x2A,

    /* 111 - HardwareRevision(8); Default: 1.0.0 */
    0x31, 0x2E, 0x30, 0x2E, 0x30,

    /* 116 - SoftwareRevision(9); Property: 0x02, Value Handle: 0x001B, UUID: 0x2A28 */
    0x02, 0x1B, 0x00, 0x28, 0x2A,

    /* 121 - SoftwareRevision(9); Default: 1.0.0 */
    0x31, 0x2E, 0x30, 0x2E, 0x30,

    /* 126 - SystemId(10); Property: 0x02, Value Handle: 0x001D, UUID: 0x2A23 */
    0x02, 0x1D, 0x00, 0x23, 0x2A,

    /* 131 - SystemId(10); Default: 11223344.0 */
    0x31, 0x31, 0x32, 0x32, 0x33, 0x33, 0x34, 0x34,

    /* 139 - RegCertDataList(11); Property: 0x02, Value Handle: 0x001F, UUID: 0x2A2A */
    0x02, 0x1F, 0x00, 0x2A, 0x2A,

    /* 144 - RegCertDataList(11); Default: 0.0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 176 - PnPID(12); Property: 0x02, Value Handle: 0x0021, UUID: 0x2A50 */
    0x02, 0x21, 0x00, 0x50, 0x2A,

    /* 181 - PnPID(12); Default: 0 */
    0x01, 0x6A, 0x00, 0x4D, 0x01, 0x00, 0x01,

    /* 188 - Mesh Proxy Data IN(13); Property: 0x04, Value Handle: 0x0024, UUID: 0x2ADD */
    0x04, 0x24, 0x00, 0xDD, 0x2A,

    /* 193 - Mesh Proxy Data OUT(14); Property: 0x10, Value Handle: 0x0026, UUID: 0x2ADE */
    0x10, 0x26, 0x00, 0xDE, 0x2A,
};

uint8_t gatt_db_peer_specific_val_mesh_proxy[BLE_GATT_DB_MAX_PEER_CONFIGURATION];

const uint8_t gatt_db_const_peer_specific_val_mesh_proxy[BLE_GATT_DB_PEER_VALUE_ARRAY_SIZE] =
{
    /* 0 - Client Configuration for Service Changed */
    /* Client Instance 0 */
    0x00, 0x00,

    /* 2 - Client Configuration for BatteryLevel */
    /* Client Instance 0 */
    0x00, 0x00,

    /* 4 - Client Configuration for Mesh Proxy Data OUT */
    /* Client Instance 0 */
    0x00, 0x00,
};

const st_ble_gatts_db_uuid_cfg_t gatt_type_mesh_proxy[] =
{
    {
        /* UUID Offset */
        0,

        /* First Occurence for Type */
        1,

        /* Last Occurence for Type */
        34,

    },
    {
        /* UUID Offset */
        6,

        /* First Occurence for Type */
        2,

        /* Last Occurence for Type */
        37,

    },
    {
        /* UUID Offset */
        12,

        /* First Occurence for Type */
        9,

        /* Last Occurence for Type */
        39,

    },
    {
        /* UUID Offset */
        16,

        /* First Occurence for Type */
        14,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        20,

        /* First Occurence for Type */
        1,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        22,

        /* First Occurence for Type */
        3,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        24,

        /* First Occurence for Type */
        5,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        26,

        /* First Occurence for Type */
        6,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        28,

        /* First Occurence for Type */
        8,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        30,

        /* First Occurence for Type */
        10,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        32,

        /* First Occurence for Type */
        12,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        34,

        /* First Occurence for Type */
        15,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        36,

        /* First Occurence for Type */
        17,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        38,

        /* First Occurence for Type */
        19,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        40,

        /* First Occurence for Type */
        21,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        42,

        /* First Occurence for Type */
        23,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        44,

        /* First Occurence for Type */
        25,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        46,

        /* First Occurence for Type */
        27,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        48,

        /* First Occurence for Type */
        29,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        50,

        /* First Occurence for Type */
        31,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        52,

        /* First Occurence for Type */
        33,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        54,

        /* First Occurence for Type */
        34,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        56,

        /* First Occurence for Type */
        36,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
    {
        /* UUID Offset */
        58,

        /* First Occurence for Type */
        38,

        /* Last Occurence for Type */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

    },
};


const st_ble_gatts_db_attr_cfg_t gatt_db_attr_mesh_proxy[] =
{
    /* Dummy */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* Value */
        NULL,
    },

    /* Handle - 0x0001 */
    /* 0 - GAP - Service Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        6,

        /* UUID Offset */
        0,

        /* Value */
        (uint8_t *)(gatt_const_uuid_mesh_proxy + 20),
    },

    /* Handle - 0x0002 */
    /* 0 - DeviceName - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        4,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 0),
    },

    /* Handle - 0x0003 */
    /* 0 - DeviceName - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        21,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        22,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 5),
    },

    /* Handle - 0x0004 */
    /* 1 - Appearance - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        7,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 26),
    },

    /* Handle - 0x0005 */
    /* 1 - Appearance - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        24,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 31),
    },

    /* Handle - 0x0006 */
    /* 1 - GATT - Service Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        10,

        /* UUID Offset */
        0,

        /* Value */
        (uint8_t *)(gatt_const_uuid_mesh_proxy + 26),
    },

    /* Handle - 0x0007 */
    /* 2 - Service Changed - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        11,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 33),
    },

    /* Handle - 0x0008 */
    /* 2 - Service Changed - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        0,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        28,

        /* Value */
        NULL,
    },

    /* Handle - 0x0009 */
    /* 2 - Service Changed - CCD */
    {
        /* Property */
        BLE_GATT_DB_READ |
        BLE_GATT_DB_WRITE,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY |
        BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        13,

        /* UUID Offset */
        12,

        /* Value */
        gatt_db_peer_specific_val_mesh_proxy + 0,
    },

    /* Handle - 0x000A */
    /* 2 - Battery - Service Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        15,

        /* UUID Offset */
        0,

        /* Value */
        (uint8_t *)(gatt_const_uuid_mesh_proxy + 30),
    },

    /* Handle - 0x000B */
    /* 3 - BatteryLevel - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        16,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 38),
    },

    /* Handle - 0x000C */
    /* 3 - BatteryLevel - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        32,

        /* Value */
        gatt_value_mesh_proxy + 0,
    },

    /* Handle - 0x000D */
    /* 3 - BatteryLevel - CCD */
    {
        /* Property */
        BLE_GATT_DB_READ |
        BLE_GATT_DB_WRITE,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY |
        BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        39,

        /* UUID Offset */
        12,

        /* Value */
        gatt_db_peer_specific_val_mesh_proxy + 2,
    },

    /* Handle - 0x000E */
    /* Characteristic 3 - BatteryLevel - Presentation Format */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        16,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 43),
    },

    /* Handle - 0x000F */
    /* 3 - DeviceInformation - Service Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        34,

        /* UUID Offset */
        0,

        /* Value */
        (uint8_t *)(gatt_const_uuid_mesh_proxy + 34),
    },

    /* Handle - 0x0010 */
    /* 4 - ManufacturerName - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        18,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 50),
    },

    /* Handle - 0x0011 */
    /* 4 - ManufacturerName - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        17,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        36,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 55),
    },

    /* Handle - 0x0012 */
    /* 5 - ModelNumber - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        20,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 72),
    },

    /* Handle - 0x0013 */
    /* 5 - ModelNumber - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        9,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        38,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 77),
    },

    /* Handle - 0x0014 */
    /* 6 - SerialNumber - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        22,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 86),
    },

    /* Handle - 0x0015 */
    /* 6 - SerialNumber - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        40,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 91),
    },

    /* Handle - 0x0016 */
    /* 7 - FirmwareRevision - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        24,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 96),
    },

    /* Handle - 0x0017 */
    /* 7 - FirmwareRevision - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        42,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 101),
    },

    /* Handle - 0x0018 */
    /* 8 - HardwareRevision - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        26,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 106),
    },

    /* Handle - 0x0019 */
    /* 8 - HardwareRevision - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        44,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 111),
    },

    /* Handle - 0x001A */
    /* 9 - SoftwareRevision - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        28,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 116),
    },

    /* Handle - 0x001B */
    /* 9 - SoftwareRevision - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        46,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 121),
    },

    /* Handle - 0x001C */
    /* 10 - SystemId - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        30,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 126),
    },

    /* Handle - 0x001D */
    /* 10 - SystemId - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        8,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        48,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 131),
    },

    /* Handle - 0x001E */
    /* 11 - RegCertDataList - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        32,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 139),
    },

    /* Handle - 0x001F */
    /* 11 - RegCertDataList - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        32,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        50,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 144),
    },

    /* Handle - 0x0020 */
    /* 12 - PnPID - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        35,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 176),
    },

    /* Handle - 0x0021 */
    /* 12 - PnPID - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        52,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 181),
    },

    /* Handle - 0x0022 */
    /* 4 - Mesh Proxy - Service Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        0,

        /* Value */
        (uint8_t *)(gatt_const_uuid_mesh_proxy + 54),
    },

    /* Handle - 0x0023 */
    /* 13 - Mesh Proxy Data IN - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        37,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 188),
    },

    /* Handle - 0x0024 */
    /* 13 - Mesh Proxy Data IN - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_WRITE_WITHOUT_RSP,

        /* Auxillary Property */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (66 + 1),

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        56,

        /* Value */
        gatt_value_mesh_proxy + 1,
    },

    /* Handle - 0x0025 */
    /* 14 - Mesh Proxy Data OUT - Characteristic Declaration */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        6,

        /* Value */
        (uint8_t *)(gatt_const_value_mesh_proxy + 193),
    },

    /* Handle - 0x0026 */
    /* 14 - Mesh Proxy Data OUT - Characteristic Value */
    {
        /* Property */
        BLE_GATT_DB_READ,

        /* Auxillary Property */
        BLE_GATT_DB_NO_AUXILIARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (66 + 1),

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        58,

        /* Value */
        gatt_value_mesh_proxy + 68,
    },

    /* Handle - 0x0027 */
    /* 14 - Mesh Proxy Data OUT - CCD */
    {
        /* Property */
        BLE_GATT_DB_READ |
        BLE_GATT_DB_WRITE,

        /* Auxillary Property */
        BLE_GATT_DB_FIXED_LENGTH_PROPERTY |
        BLE_GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROXY,

        /* UUID Offset */
        12,

        /* Value */
        gatt_db_peer_specific_val_mesh_proxy + 4,
    },

};


const st_ble_gatts_db_char_cfg_t gatt_char_mesh_proxy[] =
{

    /* 0 - DeviceName */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0002,

        /* Characteristic Service Index */
        0,
    },

    /* 1 - Appearance */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0004,

        /* Characteristic Service Index */
        0,
    },

    /* 2 - Service Changed */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0007,

        /* Characteristic Service Index */
        1,
    },

    /* 3 - BatteryLevel */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4,
        },

        /* Characteristic Start Handle */
        0x000B,

        /* Characteristic Service Index */
        2,
    },

    /* 4 - ManufacturerName */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0010,

        /* Characteristic Service Index */
        3,
    },

    /* 5 - ModelNumber */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0012,

        /* Characteristic Service Index */
        3,
    },

    /* 6 - SerialNumber */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0014,

        /* Characteristic Service Index */
        3,
    },

    /* 7 - FirmwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0016,

        /* Characteristic Service Index */
        3,
    },

    /* 8 - HardwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0018,

        /* Characteristic Service Index */
        3,
    },

    /* 9 - SoftwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x001A,

        /* Characteristic Service Index */
        3,
    },

    /* 10 - SystemId */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x001C,

        /* Characteristic Service Index */
        3,
    },

    /* 11 - RegCertDataList */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x001E,

        /* Characteristic Service Index */
        3,
    },

    /* 12 - PnPID */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0020,

        /* Characteristic Service Index */
        3,
    },

    /* 13 - Mesh Proxy Data IN */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0023,

        /* Characteristic Service Index */
        4,
    },

    /* 14 - Mesh Proxy Data OUT */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0025,

        /* Characteristic Service Index */
        4,
    },
};


const st_ble_gatts_db_serv_cfg_t gatt_serv_mesh_proxy[] =
{

    /* 0 - GAP */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        BLE_GATT_DB_SER_NO_SECURITY_PROPERTY,

        /* Service Start Handle */
        0x0001,

        /* Service End Handle */
        0x0005,

        /* Characteristic Start Index */
        0,

        /* Characteristic End Index */
        1
    },

    /* 1 - GATT */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        BLE_GATT_DB_SER_NO_SECURITY_PROPERTY,

        /* Service Start Handle */
        0x0006,

        /* Service End Handle */
        0x0009,

        /* Characteristic Start Index */
        2,

        /* Characteristic End Index */
        2
    },

    /* 2 - Battery */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        BLE_GATT_DB_SER_NO_SECURITY_PROPERTY,

        /* Service Start Handle */
        0x000A,

        /* Service End Handle */
        0x000E,

        /* Characteristic Start Index */
        3,

        /* Characteristic End Index */
        3
    },

    /* 3 - DeviceInformation */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        BLE_GATT_DB_SER_NO_SECURITY_PROPERTY,

        /* Service Start Handle */
        0x000F,

        /* Service End Handle */
        0x0021,

        /* Characteristic Start Index */
        4,

        /* Characteristic End Index */
        12
    },

    /* 4 - Mesh Proxy */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        BLE_GATT_DB_SER_NO_SECURITY_PROPERTY,

        /* Service Start Handle */
        0x0022,

        /* Service End Handle */
        0x0027,

        /* Characteristic Start Index */
        13,

        /* Characteristic End Index */
        14
    },
};

st_ble_gatts_db_cfg_t g_gatt_db_table_mesh_proxy =
{
    gatt_const_uuid_mesh_proxy,
    gatt_value_mesh_proxy,
    gatt_const_value_mesh_proxy,
    gatt_db_peer_specific_val_mesh_proxy,
    gatt_db_const_peer_specific_val_mesh_proxy,
    gatt_type_mesh_proxy,
    gatt_db_attr_mesh_proxy,
    gatt_char_mesh_proxy,
    gatt_serv_mesh_proxy,
    ARRAY_SIZE(gatt_serv_mesh_proxy),
    ARRAY_SIZE(gatt_char_mesh_proxy),
    ARRAY_SIZE(gatt_type_mesh_proxy),
    ARRAY_SIZE(gatt_db_peer_specific_val_mesh_proxy),
};

