/**
 *  \file gatt_db_mesh_prov.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_PROV_
#define _H_GATT_DB_PROV_
/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */
#define BLE_GATT_DB_INVALID_ATTR_HANDLE_VAL_MESH_PROV     0x0000

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
extern st_ble_gatts_db_cfg_t g_gatt_db_table_mesh_prov;

/* --------------------------------------------- Structures/Data Types */
enum
{
    MESH_PROV_DECL_HDL          = 0x0022,
    MESH_PROV_DATA_IN_DECL_HDL  = 0x0023,
    MESH_PROV_DATA_IN_VAL_HDL   = 0x0024,
    MESH_PROV_DATA_OUT_DECL_HDL = 0x0025,
    MESH_PROV_DATA_OUT_VAL_HDL  = 0x0026,
    MESH_PROV_DATA_OUT_CCD_HDL  = 0x0027,
};

/* --------------------------------------------- Macros */
#define BLE_GATT_DB_PEER_VALUE_ARRAY_SIZE         6

/**
 * TODO: Check if this is dependent on Max Remote Supported
 */
#define BLE_GATT_DB_MAX_PEER_CONFIGURATION        \
        (BLE_GATT_DB_PEER_VALUE_ARRAY_SIZE * (1))

#endif /* _H_GATT_DB_PROV_ */

