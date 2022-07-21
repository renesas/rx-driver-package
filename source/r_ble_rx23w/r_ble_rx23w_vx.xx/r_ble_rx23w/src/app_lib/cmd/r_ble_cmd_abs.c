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
* Copyright (C) 2019-2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "r_ble_rx23w_if.h"
#include "r_ble_rx23w_config.h"

#include "abs/r_ble_abs_api.h"
#include "cmd/r_ble_cmd.h"
#include "sec_data/r_ble_sec_data.h"
#include "r_ble_cmd_abs.h"

#if (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0)

#define BLE_LOG_TAG "abs_cmd"
#include "logger/r_ble_logger.h"

#define pf R_BLE_CLI_Printf

#define BLE_CMD_ADV_OWN_ADDR_TYPE_STR   (10)
#define BLE_CMD_ADV_TYPE_STR            (10)

static void exec_abs_adv(int argc, char *argv[]);
static void exec_abs_scan(int argc, char *argv[]);
static void exec_abs_set_priv(int argc, char *argv[]);
static void exec_abs_conn(int argc, char *argv[]);
static void abort_abs_conn(void);
static void exec_abs_auth(int argc, char *argv[]);
static void abort_abs_scan(void);
static void exec_abs_sync(int argc, char *argv[]);
static void abort_abs_sync(void);
static void exec_abs_disconn(int argc, char *argv[]);
static void exec_abs_device(int argc, char *argv[]);
static void exec_abs_white(int argc, char *argv[]);
static void exec_abs_conn_cfg(int argc, char *argv[]);
static void exec_abs_ver(int argc, char *argv[]);
#if (BLE_CFG_LIB_TYPE == 0) 
static void delete_sync_hdl(st_ble_evt_data_t * p_data);
#endif /* (BLE_CFG_LIB_TYPE == 0) */

static char gs_adv_own_addr_type[BLE_CMD_ADV_OWN_ADDR_TYPE_STR];
static char gs_adv_type[BLE_CMD_ADV_TYPE_STR];
static uint8_t gs_adv_wl;

static uint8_t gs_adv_data[] =
{
    0x02, 0x01,
    (BLE_GAP_AD_FLAGS_LE_GEN_DISC_MODE | BLE_GAP_AD_FLAGS_BR_EDR_NOT_SUPPORTED), 
    0x09, 0x09, 'R', 'B', 'L', 'E', '-', 'D', 'E', 'V', 
};

static uint8_t gs_sres_data[] =
{
    0x09, 0x09, 'R', 'B', 'L', 'E', '-', 'D', 'E', 'V', 
};

typedef struct
{
    uint16_t conn_hdl;
    st_ble_dev_addr_t addr;
} st_device_info_t;

static st_device_info_t connected_device_info[BLE_CFG_RF_CONN_MAX];

#if (BLE_CFG_ABS_API_EN != 0)
extern st_ble_dev_addr_t g_ble_priv_dummy_addr;
extern st_ble_gap_rslv_list_key_set_t g_ble_peer_dummy_irk;
#else /* (BLE_CFG_ABS_API_EN != 0) */
st_ble_dev_addr_t g_ble_priv_dummy_addr;
st_ble_gap_rslv_list_key_set_t g_ble_peer_dummy_irk;
#endif /* (BLE_CFG_ABS_API_EN != 0) */

static st_ble_abs_legacy_adv_param_t gs_legacy_adv_param =
{
    .p_addr                    = NULL,
    .p_adv_data                = gs_adv_data,
    .p_sres_data               = gs_sres_data,
    .fast_adv_intv             = 0x00000100,
    .slow_adv_intv             = 0x00000200,
    .fast_period               = 0x0100,
    .slow_period               = 0x0000,
    .adv_data_length           = sizeof(gs_adv_data),
    .sres_data_length          = sizeof(gs_sres_data),
    .adv_ch_map                = BLE_GAP_ADV_CH_ALL,
    .filter                    = BLE_ABS_ADV_ALLOW_CONN_ANY,
    .o_addr_type               = BLE_GAP_ADDR_RAND,
};

#if (BLE_CFG_LIB_TYPE == 0)
static st_ble_abs_ext_adv_param_t gs_ext_adv_param =
{
    .p_addr                    = NULL,
    .p_adv_data                = gs_adv_data,
    .fast_adv_intv             = 0x00000100,
    .slow_adv_intv             = 0x00000200,
    .fast_period               = 0x0300,
    .slow_period               = 0x0000,
    .adv_data_length           = sizeof(gs_adv_data),
    .adv_ch_map                = BLE_GAP_ADV_CH_ALL,
    .filter                    = BLE_ABS_ADV_ALLOW_CONN_ANY,
    .o_addr_type               = BLE_GAP_ADDR_RAND,
    .adv_phy                   = BLE_GAP_ADV_PHY_1M,
    .sec_adv_phy               = BLE_GAP_ADV_PHY_1M,
};
#endif /* (BLE_CFG_LIB_TYPE == 0) */

static st_ble_abs_non_conn_adv_param_t gs_non_conn_adv_param =
{
    .p_addr                    = NULL,
    .p_adv_data                = gs_adv_data,
    .adv_intv                  = 0x000000a0,
    .duration                  = 0x0000,
    .adv_data_length           = sizeof(gs_adv_data),
    .adv_ch_map                = BLE_GAP_ADV_CH_ALL,
    .o_addr_type               = BLE_GAP_ADDR_RAND,
    .adv_phy                   = BLE_GAP_ADV_PHY_1M,
    .sec_adv_phy               = BLE_GAP_ADV_PHY_1M,
};

#if (BLE_CFG_LIB_TYPE == 0)
static st_ble_abs_perd_adv_param_t gs_periodic_adv_param =
{
    .p_perd_adv_data           = gs_adv_data,
    .perd_intv                 = 0x0040,
    .perd_adv_data_length      = sizeof(gs_adv_data),
};
#endif /* (BLE_CFG_LIB_TYPE == 0) */

#if (BLE_CFG_LIB_TYPE != 2) 
static st_ble_abs_scan_phy_param_t gs_phy_param_1M =
{
    .fast_intv                 = 0x0200,
    .slow_intv                 = 0x0800,
    .fast_window               = 0x0100,
    .slow_window               = 0x0100,
    .scan_type                 = BLE_GAP_SCAN_PASSIVE,
};

#if (BLE_CFG_LIB_TYPE == 0) 
static st_ble_abs_scan_phy_param_t gs_phy_param_sync =
{
    .fast_intv                 = 0x0100,
    .slow_intv                 = 0x0100,
    .fast_window               = 0x0100,
    .slow_window               = 0x0100,
    .scan_type                 = BLE_GAP_SCAN_PASSIVE,
};
#endif /* (BLE_CFG_LIB_TYPE == 0) */

static uint8_t gs_filt_data[31];

static st_ble_abs_scan_param_t gs_scan_param =
{
    .p_phy_param_1M            = &gs_phy_param_1M,
    .p_phy_param_coded         = NULL,
    .p_filter_data             = gs_filt_data,
    .fast_period               = 0x0100,
    .slow_period               = 0x0000,
    .filter_data_length        = 0,
    .dev_filter                = BLE_ABS_SCAN_ALL_STATIC,
    .filter_dups               = BLE_GAP_SCAN_FILT_DUPLIC_DISABLE,
};

#if (BLE_CFG_LIB_TYPE == 0) 
static st_ble_abs_scan_param_t gs_sync_scan_param =
{
    .p_phy_param_1M            = &gs_phy_param_sync,
    .p_phy_param_coded         = NULL,
    .p_filter_data             = NULL,
    .fast_period               = 0x0000,
    .slow_period               = 0x0000,
    .filter_data_length        = 0,
    .dev_filter                = BLE_ABS_SCAN_WLST_STATIC,
    .filter_dups               = BLE_GAP_SCAN_FILT_DUPLIC_DISABLE,
};
#endif /* (BLE_CFG_LIB_TYPE == 0) */

static st_ble_abs_conn_phy_param_t gs_conn_phy_1m =
{
    .conn_intv                 = 0x00A0,
    .conn_latency              = 0x0000,
    .sup_to                    = 0x03E8,
};

static st_ble_dev_addr_t gs_conn_bd_addr;

static st_ble_abs_conn_param_t gs_conn_param =
{
    .filter                    = BLE_ABS_CONN_USE_ADDR_STATIC,
    .conn_to                   = 7,
    .p_conn_1M                 = &gs_conn_phy_1m,
    .p_conn_2M                 = NULL,
    .p_conn_coded              = NULL,
    .p_addr                    = &gs_conn_bd_addr,
};

#endif /* (BLE_CFG_LIB_TYPE != 2) */

static uint16_t gs_pairing_conn_hdl = BLE_GAP_INVALID_CONN_HDL;
static uint8_t gs_version_proc = 0x00;

#if (BLE_CFG_LIB_TYPE == 0) 
static uint8_t gs_sync_scan = 0;
static st_ble_dev_addr_t gs_sync_advr;
static uint8_t gs_adv_sid;
static uint32_t gs_sync_hdl = 0xFFFFFFFF;
static uint8_t gs_sync_term = 0;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

static const st_ble_cli_cmd_t abs_adv_cmd =
{
    .p_name = "adv",
    .exec   = exec_abs_adv,
    .p_help = "Usage: gap adv (legacy|ext|non-conn|periodic) (start|stop)\n"
                "Start or stop advertising.",
};

static const st_ble_cli_cmd_t abs_scan_cmd =
{
    .p_name = "scan",
    .exec   = exec_abs_scan,
    .abort  = abort_abs_scan,
    .p_help = "Usage: Start : gap scan start\n"
            "Stop : Ctrl-C.",
};

static const st_ble_cli_cmd_t abs_conn_cmd =
{
    .p_name = "conn",
    .exec   = exec_abs_conn,
    .abort  = abort_abs_conn,
    .p_help = "Usage: gap conn {addr}\n"
            "Create connection.",
};

static const st_ble_cli_cmd_t abs_priv_cmd =
{
    .p_name = "priv",
    .exec   = exec_abs_set_priv,
    .p_help = "Usage: gap priv {irk} {privacy_mode}\n"
            "Set local device privacy.",
};

static const st_ble_cli_cmd_t abs_auth_cmd =
{
    .p_name = "auth",
    .exec   = exec_abs_auth,
    .p_help = "Usage: gap auth (start|passkey|numcmp) {conn_hdl|passkey|yes}\n"
            "Start pairing or encryption. or Reply passkey entry or numeric comparison",
};

static const st_ble_cli_cmd_t gap_white_cmd =
{
    .p_name = "wl",
    .exec   = exec_abs_white,
    .p_help = "Usage: gap wl (reg|del|clear)\n"
              "Register or delete or clear white list.",
};

static const st_ble_cli_cmd_t gap_sync_cmd =
{
    .p_name = "sync",
    .exec   = exec_abs_sync,
    .abort  = abort_abs_sync,
    .p_help = "Usage: gap sync create | term  \n"
              "Start sync establishment | terminate sync. ",
};

static const st_ble_cli_cmd_t gap_conn_cfg_cmd =
{
    .p_name = "conn_cfg",
    .exec   = exec_abs_conn_cfg,
    .p_help = "Usage: gap conn_cfg (update|data_len|phy|def_phy) \n"
              "Update connection parameters | Set packet data length | Set PHY | Set Default PHY",
};

static const st_ble_cli_cmd_t gap_disconn_cmd =
{
    .p_name = "disconn",
    .exec   = exec_abs_disconn,
    .p_help = "Usage: gap disconn conn_hdl\n"
              "Disconnect a connection",
};

static const st_ble_cli_cmd_t gap_device_cmd =
{
    .p_name = "device",
    .exec   = exec_abs_device,
    .p_help = "Usage: gap device\n"
              "List connecting devices",
};

static const st_ble_cli_cmd_t gap_version_cmd =
{
    .p_name = "ver",
    .exec   = exec_abs_ver,
    .p_help = "Usage: gap ver\n"
              "Get LL/HCI, Host, lib, BLE FIT module version.",
};

static const st_ble_cli_cmd_t * const abs_sub_cmds[] =
{
    &abs_adv_cmd,
    &abs_scan_cmd,
    &abs_conn_cmd,
    &abs_priv_cmd,
    &abs_auth_cmd,
    &gap_device_cmd,
    &gap_disconn_cmd,
    &gap_sync_cmd,
    &gap_white_cmd,
    &gap_conn_cfg_cmd,
    &gap_version_cmd
};

const st_ble_cli_cmd_t g_abs_cmd =
{
    .p_name      = "gap",
    .p_cmds      = abs_sub_cmds,
    .num_of_cmds = ARRAY_SIZE(abs_sub_cmds),
    .p_help     = "Sub Command: adv, scan, conn, disconn, sync, wl, device, priv, auth, ver\n"
                  "Try 'gap subcmd help' for more information",
};

const st_ble_cli_cmd_t * const abs_cmd[] = {
    &g_abs_cmd,
};

#if (BLE_CFG_LIB_TYPE == 0)
static uint8_t gs_adv_cmd_status;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

/*----------------------------------------------------------------------------------------------------------------------
    adv command
----------------------------------------------------------------------------------------------------------------------*/
static ble_status_t set_adv_addr_type(st_ble_dev_addr_t ** pp_addr, st_ble_dev_addr_t * o_rnd_addr, uint8_t * o_addr_type, uint8_t * o_addr)
{
    ble_status_t ret;
#if (BLE_CFG_LIB_TYPE == 0)
    (void)&o_rnd_addr;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    ret = BLE_SUCCESS;

    if(strcmp(gs_adv_own_addr_type, "rnd") == 0)
    {
        * o_addr_type = BLE_GAP_ADDR_RAND;
        * pp_addr = NULL;
#if (BLE_CFG_LIB_TYPE == 0)
        memcpy(o_addr, o_rnd_addr->addr, BLE_BD_ADDR_LEN);
#endif /* (BLE_CFG_LIB_TYPE == 0) */
    }
    else if(strcmp(gs_adv_own_addr_type, "rpa_pub") == 0)
    {
        * o_addr_type = BLE_GAP_ADDR_RPA_ID_PUBLIC;
        * pp_addr = &g_ble_priv_dummy_addr;
        memset(o_addr, 0x00, BLE_BD_ADDR_LEN);
    }
    else if(strcmp(gs_adv_own_addr_type, "rpa_rnd") == 0)
    {
        * o_addr_type = BLE_GAP_ADDR_RPA_ID_RANDOM;
        * pp_addr = &g_ble_priv_dummy_addr;
#if (BLE_CFG_LIB_TYPE == 0)
        memcpy(o_addr, o_rnd_addr->addr, BLE_BD_ADDR_LEN);
#endif /* (BLE_CFG_LIB_TYPE == 0) */
    }
    else if(strcmp(gs_adv_own_addr_type, "pub") == 0)
    {
        * o_addr_type = BLE_GAP_ADDR_PUBLIC;
        * pp_addr = NULL;
        memset(o_addr, 0x00, BLE_BD_ADDR_LEN);
    }
    else
    {
        ret = BLE_ERR_INVALID_ARG;
    }

    return ret;
}

void start_adv_cmd(st_ble_dev_addr_t * o_rnd_addr)
{
    ble_status_t ret;

    ret = BLE_SUCCESS;

#if (BLE_CFG_LIB_TYPE == 0)
    if(0 == gs_adv_cmd_status)
    {
        return;
    }
#endif /* (BLE_CFG_LIB_TYPE == 0) */
   
    if(strcmp(gs_adv_type, "legacy") == 0)
    {
        gs_legacy_adv_param.filter = (0 == gs_adv_wl) ? BLE_ABS_ADV_ALLOW_CONN_ANY : BLE_ABS_ADV_ALLOW_CONN_WLST;
        ret = set_adv_addr_type(&gs_legacy_adv_param.p_addr, o_rnd_addr, &gs_legacy_adv_param.o_addr_type, gs_legacy_adv_param.o_addr);
        if(BLE_SUCCESS == ret)
        {
            ret = R_BLE_ABS_StartLegacyAdv(&gs_legacy_adv_param);
        }
    }
    else if(strcmp(gs_adv_type, "ext") == 0)
    {
#if (BLE_CFG_LIB_TYPE == 0)
        gs_ext_adv_param.filter = (0 == gs_adv_wl) ? BLE_ABS_ADV_ALLOW_CONN_ANY : BLE_ABS_ADV_ALLOW_CONN_WLST;
        ret = set_adv_addr_type(&gs_ext_adv_param.p_addr, o_rnd_addr, &gs_ext_adv_param.o_addr_type, gs_ext_adv_param.o_addr);
        if(BLE_SUCCESS == ret)
        {
            ret = R_BLE_ABS_StartExtAdv(&gs_ext_adv_param);
        }
#else /* (BLE_CFG_LIB_TYPE == 0) */
        ret = BLE_ERR_UNSUPPORTED;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
    }
    else if(strcmp(gs_adv_type, "non-conn") == 0)
    {
        gs_non_conn_adv_param.adv_intv = 0x00000180;
        ret = set_adv_addr_type(&gs_non_conn_adv_param.p_addr, o_rnd_addr, &gs_non_conn_adv_param.o_addr_type, gs_non_conn_adv_param.o_addr);
        if(BLE_SUCCESS == ret)
        {
            ret = R_BLE_ABS_StartNonConnAdv(&gs_non_conn_adv_param);
        }
    }
    else if(strcmp(gs_adv_type, "periodic") == 0)
    {
#if (BLE_CFG_LIB_TYPE == 0)
        gs_non_conn_adv_param.adv_intv = 0x00000100;
        gs_non_conn_adv_param.adv_data_length = 0;
        gs_periodic_adv_param.param = gs_non_conn_adv_param;
        ret = set_adv_addr_type(&gs_periodic_adv_param.param.p_addr, o_rnd_addr, &gs_periodic_adv_param.param.o_addr_type, gs_periodic_adv_param.param.o_addr);
        if(BLE_SUCCESS == ret)
        {
            ret = R_BLE_ABS_StartPerdAdv(&gs_periodic_adv_param);
        }
#else /* (BLE_CFG_LIB_TYPE == 0) */
        ret = BLE_ERR_UNSUPPORTED;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
    }
    else
    {
        R_BLE_CLI_PrintUnrecognized();
    }

    if(BLE_SUCCESS != ret)
    {
        R_BLE_CLI_PrintError(ret);
    }

    gs_adv_wl = 0;

#if (BLE_CFG_LIB_TYPE == 0)
    gs_adv_cmd_status = 0;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

}

static void exec_abs_adv(int argc, char *argv[])
{
    ble_status_t ret;
    
    if ((3 > argc) || (5 < argc))
    {
        R_BLE_CLI_PrintUnrecognized();
        return;
    }

    ret = BLE_SUCCESS;

    if (strcmp(argv[2], "start") == 0)
    {
        strncpy(gs_adv_type, (const char*)argv[1], sizeof(gs_adv_type));

#if (BLE_CFG_LIB_TYPE == 0)
        gs_adv_cmd_status = 1;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

        if (strcmp(argv[argc-1], "-wl") == 0)
        {
            if((strcmp(argv[1], "non-conn") == 0) || (strcmp(argv[1], "periodic") == 0))
            {
                R_BLE_CLI_PrintUnrecognized();
                return;
            }
            else
            {
                gs_adv_wl = 1;
            }
        }

        if((3 == argc) || ((4 == argc) && (1 == gs_adv_wl)))
        {
            strncpy(gs_adv_own_addr_type, "rnd", sizeof(gs_adv_own_addr_type));
        }
        else
        {
            strncpy(gs_adv_own_addr_type, (const char*)argv[3], sizeof(gs_adv_own_addr_type));
        }
#if (BLE_CFG_LIB_TYPE == 0)
        if((strcmp(gs_adv_own_addr_type, "rnd") == 0) || (strcmp(gs_adv_own_addr_type, "rpa_rnd") == 0))
        {
            ret = R_BLE_VS_GetBdAddr(BLE_VS_ADDR_AREA_REG, BLE_GAP_ADDR_RAND);
            if(BLE_SUCCESS != ret)
            {
                gs_adv_cmd_status = 0;
                gs_adv_wl = 0;
                R_BLE_CLI_PrintError(ret);
            }

            return;
        }
        else
        {
            if((strcmp(gs_adv_own_addr_type, "pub") != 0) && (strcmp(gs_adv_own_addr_type, "rpa_pub") != 0))
            {
                gs_adv_cmd_status = 0;
                gs_adv_wl = 0;
                R_BLE_CLI_PrintUnrecognized();
                return;
            }
        }
#else /* (BLE_CFG_LIB_TYPE == 0) */
        if((strcmp(gs_adv_own_addr_type, "pub") != 0) && 
            (strcmp(gs_adv_own_addr_type, "rnd") != 0) &&
            (strcmp(gs_adv_own_addr_type, "rpa_pub") != 0) &&
            (strcmp(gs_adv_own_addr_type, "rpa_rnd") != 0))
        {
            R_BLE_CLI_PrintUnrecognized();
            gs_adv_wl = 0;
            return;
        }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

        start_adv_cmd(NULL);
    }
    else
    {
        uint8_t adv_hdl = 0xFF;
        if(strcmp(argv[1], "legacy") == 0)
        {
            adv_hdl = 0x00;
        }
        else if(strcmp(argv[1], "ext") == 0)
        {
            adv_hdl = 0x01;
        }
        else if(strcmp(argv[1], "non-conn") == 0)
        {
            adv_hdl = 0x02;
        }
        else if(strcmp(argv[1], "periodic") == 0)
        {
            adv_hdl = 0x03;
        }
        else 
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        if (strcmp(argv[2], "stop") == 0)
        {
            ret = R_BLE_GAP_StopAdv(adv_hdl);
        }
        else if (strcmp(argv[2], "remove") == 0)
        {
            ret = R_BLE_GAP_RemoveAdvSet(BLE_GAP_RMV_ADV_SET_REM_OP, adv_hdl);
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
        }
    }

    if(BLE_SUCCESS != ret)
    {
        R_BLE_CLI_PrintError(ret);
    }

}

/*----------------------------------------------------------------------------------------------------------------------
    scan command
----------------------------------------------------------------------------------------------------------------------*/
#if (BLE_CFG_LIB_TYPE != 2) 
static uint8_t check_scan_addr_type(char* argv)
{
    uint8_t scan_addr = 0xFF;
    if (strcmp(argv, "pub") == 0)
    {
        scan_addr = BLE_GAP_ADDR_PUBLIC;
    }
    else if (strcmp(argv, "rnd") == 0)
    {
        scan_addr = BLE_GAP_ADDR_RAND;
    }
    else if (strcmp(argv, "rpa_pub") == 0)
    {
        scan_addr = BLE_GAP_ADDR_RPA_ID_PUBLIC;
    }
    else if (strcmp(argv, "rpa_rnd") == 0)
    {
        scan_addr = BLE_GAP_ADDR_RPA_ID_RANDOM;
    }
    return scan_addr;
}
#endif /* (BLE_CFG_LIB_TYPE != 2) */

static void exec_abs_scan(int argc, char *argv[])
{
#if (BLE_CFG_LIB_TYPE == 2) 
    R_BLE_CLI_PrintError(BLE_ERR_UNSUPPORTED);
#else /* (BLE_CFG_LIB_TYPE == 2) */
    ble_status_t ret;
    uint8_t scan_wl;
    uint8_t scan_addr;

    if((argc == 2) && (strcmp(argv[1], "stop") == 0))
    {
        R_BLE_CLI_SetCmdComp();
        ret = R_BLE_GAP_StopScan();
    }
    else
    {
        if (5 < argc)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        scan_wl = 0;
        if (strcmp(argv[argc-1], "-wl") == 0)
        {
            scan_wl = 1;
        }

        scan_addr = 0xFF;

        gs_scan_param.filter_data_length = 0;
        if (1 < argc)
        {
            scan_addr = check_scan_addr_type(argv[1]);
            if(scan_addr == 0xFF)
            {
                if( (scan_wl == 0) && (argc == 2) )
                {
                    R_BLE_CLI_PrintUnrecognized();
                    return;
                }
                else if( argc >= 3 )
                {
                    uint16_t len;
                    gs_scan_param.filter_ad_type = (uint8_t)strtol(argv[1], NULL, 0);
                    R_BLE_CMD_ParseValues(argv[2], gs_filt_data, &len);
                    if(len == 0)
                    {
                        R_BLE_CLI_PrintUnrecognized();
                        return;
                    }
                    else
                    {
                        gs_scan_param.filter_data_length = len;
                    }
                }
            }
            else
            {
                if (3 < argc)
                {
                    R_BLE_CLI_PrintUnrecognized();
                    return;
                }
                else if( (argc == 3) && (scan_wl != 1) )
                {
                    R_BLE_CLI_PrintUnrecognized();
                    return;
                }
            }
        }
        if (3 < argc)
        {
            scan_addr = check_scan_addr_type(argv[3]);
            if(scan_addr == 0xFF)
            {
                if( (scan_wl == 1) && (argc != 4) )
                {
                    R_BLE_CLI_PrintUnrecognized();
                    return;
                }
            }
            else
            {
                if( (scan_wl == 1) && (argc != 5) )
                {
                    R_BLE_CLI_PrintUnrecognized();
                    return;
                }
            }
        }

        if(0xFF == scan_addr)
        {
            gs_phy_param_1M.scan_type = BLE_GAP_SCAN_PASSIVE;
            scan_addr = BLE_GAP_ADDR_RAND;
        }
        else
        {
            gs_phy_param_1M.scan_type = BLE_GAP_SCAN_ACTIVE;
        }

        gs_scan_param.dev_filter = (0 == scan_wl) ? 
                    (BLE_GAP_SCAN_ALLOW_ADV_ALL  | (scan_addr << 4)) : 
                    (BLE_GAP_SCAN_ALLOW_ADV_WLST | (scan_addr << 4));

#if (BLE_CFG_LIB_TYPE == 0) 
        gs_sync_scan = 0;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
        ret = R_BLE_ABS_StartScan(&gs_scan_param);
    }

    if(BLE_SUCCESS != ret)
    {
        R_BLE_CLI_PrintError(ret);
    }
#endif /* (BLE_CFG_LIB_TYPE == 2) */
}

static void abort_abs_scan(void)
{
    R_BLE_GAP_StopScan();
}

/*----------------------------------------------------------------------------------------------------------------------
    conn command
----------------------------------------------------------------------------------------------------------------------*/
#if (BLE_CFG_LIB_TYPE != 2) 
static void abs_conn_set_event(void)
{
    ble_status_t ret;
    ret = R_BLE_ABS_CreateConn(&gs_conn_param);
    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
    }
}
#endif /* (BLE_CFG_LIB_TYPE != 2) */

static void exec_abs_conn(int argc, char *argv[])
{
#if (BLE_CFG_LIB_TYPE == 2) 
    R_BLE_CLI_PrintError(BLE_ERR_UNSUPPORTED);
#else /* (BLE_CFG_LIB_TYPE == 2) */
    ble_status_t ret;
    uint8_t set_event_conn;
    uint8_t conn_wl;

    if ((3 > argc) || (6 < argc))
    {
        R_BLE_CLI_PrintUnrecognized();
        return;
    }

    conn_wl = 0;
    if (strcmp(argv[argc-1], "-wl") == 0)
    {
        conn_wl = 1;
    }

    R_BLE_CMD_ParseAddr(argv[1], gs_conn_bd_addr.addr);

    if ((strcmp(argv[2], "pub") == 0) || (strcmp(argv[2], "rpa_pub") == 0))
    {
        gs_conn_bd_addr.type = BLE_GAP_ADDR_PUBLIC;
    }
    else if ((strcmp(argv[2], "rnd") == 0) || (strcmp(argv[2], "rpa_rnd") == 0))
    {
        gs_conn_bd_addr.type = BLE_GAP_ADDR_RAND;
    }
    else
    {
        R_BLE_CLI_PrintUnrecognized();
        return;
    }

    gs_conn_param.filter = BLE_ABS_CONN_USE_ADDR_STATIC;
    set_event_conn = 0;

    if (3 < argc)
    {
        if (strcmp(argv[3], "rpa_pub") == 0)
        {
            gs_conn_param.filter = BLE_ABS_CONN_USE_ADDR_RPA_PUBLIC;
        }
        else if (strcmp(argv[3], "rpa_rnd") == 0)
        {
            gs_conn_param.filter = BLE_ABS_CONN_USE_ADDR_RPA_STATIC;
        }
        else if (strcmp(argv[3], "rnd") == 0)
        {
            gs_conn_param.filter = BLE_ABS_CONN_USE_ADDR_STATIC;
        }
        else if (strcmp(argv[3], "pub") == 0)
        {
            gs_conn_param.filter = BLE_ABS_CONN_USE_ADDR_PUBLIC;
        }
        else if (strcmp(argv[3], "-wl") != 0)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }
    }

    if(0 != conn_wl)
    {
        gs_conn_param.filter = gs_conn_param.filter + BLE_GAP_INIT_FILT_USE_WLST;
    }

    if(((5 == argc) && (0 == conn_wl)) ||
       ((6 == argc) && (0 != conn_wl)))
    {
        if(BLE_ABS_CONN_USE_ADDR_RPA_PUBLIC > gs_conn_param.filter)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        if (strcmp(argv[4], "dummy_irk") == 0)
        {
            set_event_conn = 1;
            ret = R_BLE_GAP_ConfRslvList(BLE_GAP_LIST_ADD_DEV, &gs_conn_bd_addr, &g_ble_peer_dummy_irk, 1);
            R_BLE_SetEvent(abs_conn_set_event);
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }
    }

    if(0 == set_event_conn)
    {
        ret = R_BLE_ABS_CreateConn(&gs_conn_param);
    }

    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
    }
#endif /* (BLE_CFG_LIB_TYPE == 2) */
}

static void abort_abs_conn(void)
{
    R_BLE_GAP_CancelCreateConn();
}

/*----------------------------------------------------------------------------------------------------------------------
    conn_cfg command
----------------------------------------------------------------------------------------------------------------------*/

static void exec_abs_conn_cfg(int argc, char *argv[])
{
    uint16_t conn_hdl;
    ble_status_t ret;

    ret = BLE_SUCCESS;

    if (strcmp(argv[1], "update") == 0)
    {
        st_ble_gap_conn_param_t updt_param;

        if (argc == 6)
        {
            conn_hdl                 = (uint16_t)strtol(argv[2], NULL, 0);
            updt_param.conn_intv_min = (uint16_t)strtol(argv[3], NULL, 0);
            updt_param.conn_intv_max = updt_param.conn_intv_min;
            updt_param.conn_latency  = (uint16_t)strtol(argv[4], NULL, 0);
            updt_param.sup_to        = (uint16_t)strtol(argv[5], NULL, 0);
            updt_param.min_ce_length = 0xFFFF;
            updt_param.max_ce_length = 0xFFFF;

            ret = R_BLE_GAP_UpdConn(conn_hdl, BLE_GAP_CONN_UPD_MODE_REQ, 0x0000, &updt_param);
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
        }
    }
    else if (strcmp(argv[1], "phy") == 0)
    {
#if (BLE_CFG_LIB_TYPE == 2) 
        ret = BLE_ERR_UNSUPPORTED;
#else /* (BLE_CFG_LIB_TYPE == 2) */
        st_ble_gap_set_phy_param_t phy;
        if (argc == 6)
        {
            conn_hdl               = (uint16_t)strtol(argv[2], NULL, 0);
            phy.tx_phys            = (uint8_t)strtol(argv[3], NULL, 0);
            phy.rx_phys            = (uint8_t)strtol(argv[4], NULL, 0);
            phy.phy_options        = (uint8_t)strtol(argv[5], NULL, 0);

            ret = R_BLE_GAP_SetPhy(conn_hdl, &phy);
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
        }
#endif /* (BLE_CFG_LIB_TYPE == 2) */
    }
    else if (strcmp(argv[1], "def_phy") == 0)
    {
#if (BLE_CFG_LIB_TYPE == 2) 
        ret = BLE_ERR_UNSUPPORTED;
#else /* (BLE_CFG_LIB_TYPE == 2) */
        st_ble_gap_set_def_phy_param_t phy;
        if (argc == 4)
        {
            phy.tx_phys            = (uint8_t)strtol(argv[2], NULL, 0);
            phy.rx_phys            = (uint8_t)strtol(argv[3], NULL, 0);

            ret = R_BLE_GAP_SetDefPhy(&phy);
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
        }
#endif /* (BLE_CFG_LIB_TYPE == 2) */
    }
    else if (strcmp(argv[1], "data_len") == 0)
    {
        uint16_t tx_len;
        uint16_t tx_time;
        if (argc == 5)
        {
            conn_hdl           = (uint16_t)strtol(argv[2], NULL, 0);
            tx_len             = (uint16_t)strtol(argv[3], NULL, 0);
            tx_time            = (uint16_t)strtol(argv[4], NULL, 0);

            ret = R_BLE_GAP_SetDataLen(conn_hdl, tx_len, tx_time);
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
        }
    }
    else
    {
        R_BLE_CLI_PrintUnrecognized();
    }

    if(BLE_SUCCESS != ret)
    {
        R_BLE_CLI_PrintError(ret);
    }

}

/*----------------------------------------------------------------------------------------------------------------------
    priv command
----------------------------------------------------------------------------------------------------------------------*/
static void exec_abs_set_priv(int argc, char *argv[])
{
    uint8_t irk[16];
    uint8_t priv;
    char tmp[2];
    ble_status_t ret;
    int32_t i;
    char * p_mode;
    uint8_t * p_irk;
    st_ble_dev_addr_t rem_addr;

    ret = BLE_SUCCESS;

    if (strcmp(argv[1], "set") == 0)
    {
        if (argc == 4)
        {
            if(32 != strlen(argv[2]))
            {
                pf("invalid irk size\n");
                R_BLE_CLI_SetCmdComp();
                return;
            }

            for(i=0; i<BLE_GAP_IRK_SIZE; i++)
            {
                memcpy(tmp, argv[2] + i * 2, 2);
                irk[i] = (uint8_t)strtol(tmp, NULL, 16);
            }

            p_mode = argv[3];
            p_irk = irk;
        }
        else if (argc == 3)
        {
            p_mode = argv[2];
            p_irk = NULL;
        }
        else
        {
            R_BLE_CLI_PrintError(BLE_ERR_INVALID_ARG);
            return;
        }

        if (strcmp(p_mode, "net") == 0)
        {
            priv = BLE_ABS_PRIV_NET_STATIC_IDADDR;
        }
        else if (strcmp(p_mode, "dev") == 0)
        {
            priv = BLE_ABS_PRIV_DEV_STATIC_IDADDR;
        }
        else if(strcmp(p_mode, "net_pub") == 0)
        {
            priv = BLE_ABS_PRIV_NET_PUBLIC_IDADDR;
        }
        else if (strcmp(p_mode, "dev_pub") == 0)
        {
            priv = BLE_ABS_PRIV_DEV_PUBLIC_IDADDR;
        }
        else if(strcmp(p_mode, "net_rnd") == 0)
        {
            priv = BLE_ABS_PRIV_NET_STATIC_IDADDR;
        }
        else if (strcmp(p_mode, "dev_rnd") == 0)
        {
            priv = BLE_ABS_PRIV_DEV_STATIC_IDADDR;
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        ret = R_BLE_ABS_SetLocPrivacy(p_irk, priv);
    }
    else if (strcmp(argv[1], "remove") == 0)
    {

        if (argc != 4)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        R_BLE_CMD_ParseAddr(argv[2], rem_addr.addr);

        if (strcmp(argv[3], "pub") == 0)
        {
            rem_addr.type = BLE_GAP_ADDR_PUBLIC;
        }
        else if (strcmp(argv[3], "rnd") == 0)
        {
            rem_addr.type = BLE_GAP_ADDR_RAND;
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        ret = R_BLE_GAP_ConfRslvList(BLE_GAP_LIST_REM_DEV, &rem_addr, NULL, 1);
    }
    else if (strcmp(argv[1], "get") == 0)
    {
        /* set dummy address */
        memset(&rem_addr, 0x00, sizeof(st_ble_dev_addr_t));

        if ((argc != 3) && (argc != 5))
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        if (5 == argc)
        {
            R_BLE_CMD_ParseAddr(argv[3], rem_addr.addr);

            if (strcmp(argv[4], "pub") == 0)
            {
                rem_addr.type = BLE_GAP_ADDR_PUBLIC;
            }
            else if (strcmp(argv[4], "rnd") == 0)
            {
                rem_addr.type = BLE_GAP_ADDR_RAND;
            }
            else
            {
                R_BLE_CLI_PrintUnrecognized();
                return;
            }
        }

        if (strcmp(argv[2], "lrpa") == 0)
        {
            ret = R_BLE_GAP_ReadRpa(&rem_addr);
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }
    }
    else if (strcmp(argv[1], "off") == 0)
    {
        if (argc != 2)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        ret = R_BLE_GAP_EnableRpa(0);
    }
    else
    {
        R_BLE_CLI_PrintUnrecognized();
        return;
    }

    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
    }

}

/*----------------------------------------------------------------------------------------------------------------------
    white list command
----------------------------------------------------------------------------------------------------------------------*/
static void exec_abs_white(int argc, char *argv[])
{
    ble_status_t ret;

    ret = BLE_SUCCESS;

    if (strcmp(argv[1], "reg") == 0)
    {
        if (argc != 4)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }
        st_ble_dev_addr_t dev;

        R_BLE_CMD_ParseAddr(argv[2], dev.addr);

        if (strcmp(argv[3], "rnd") == 0)
        {
            dev.type = BLE_GAP_ADDR_RAND;
        }
        else if (strcmp(argv[3], "pub") == 0)
        {
            dev.type = BLE_GAP_ADDR_PUBLIC;
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }
        
        ret = R_BLE_GAP_ConfWhiteList(BLE_GAP_LIST_ADD_DEV, &dev, 1);
    }
    else if (strcmp(argv[1], "del") == 0)
    {
        st_ble_dev_addr_t dev;

        if (argc != 4)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }
        R_BLE_CMD_ParseAddr(argv[2], dev.addr);

        if (strcmp(argv[3], "rnd") == 0)
        {
            dev.type = BLE_GAP_ADDR_RAND;
        }
        else if (strcmp(argv[3], "pub") == 0)
        {
            dev.type = BLE_GAP_ADDR_PUBLIC;
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        ret = R_BLE_GAP_ConfWhiteList(BLE_GAP_LIST_REM_DEV, &dev, 1);
    }
    else if (strcmp(argv[1], "clear") == 0)
    {
        ret = R_BLE_GAP_ConfWhiteList(BLE_GAP_LIST_CLR, NULL, 1);
    }
    else
    {
        R_BLE_CLI_PrintUnrecognized();
    }

    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
    }
}


/*----------------------------------------------------------------------------------------------------------------------
    periodic sync command
----------------------------------------------------------------------------------------------------------------------*/
#if (BLE_CFG_LIB_TYPE == 0) 
static void abs_sync_set_event(void)
{
    ble_status_t ret;

    gs_sync_scan = 1;
    
    ret = R_BLE_ABS_StartScan(&gs_sync_scan_param);

    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
    }
}
#endif /* (BLE_CFG_LIB_TYPE == 0)  */

static void exec_abs_sync(int argc, char *argv[])
{
#if (BLE_CFG_LIB_TYPE == 0) 
    st_ble_dev_addr_t advr;
    ble_status_t ret;

    ret = BLE_SUCCESS;

    if (strcmp(argv[1], "create") == 0)
    {
        if (argc != 4)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }
        R_BLE_CMD_ParseAddr(argv[2], advr.addr);
        if (strcmp(argv[3], "rnd") == 0)
        {
            advr.type = BLE_GAP_ADDR_RAND;
        }
        else if (strcmp(argv[3], "pub") == 0)
        {
            advr.type = BLE_GAP_ADDR_PUBLIC;
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        /* white list */
        ret = R_BLE_GAP_ConfWhiteList(BLE_GAP_LIST_ADD_DEV, &advr, 1);

        if(BLE_SUCCESS == ret)
        {
            /* scan */
            R_BLE_SetEvent(abs_sync_set_event);
        }
    }
    else if (strcmp(argv[1], "term") == 0)
    {
        if ((argc != 2) && (argc != 3))
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        if(argc == 2)
        {
            if(0xFFFF != (uint16_t)gs_sync_hdl)
            {
                ret = R_BLE_GAP_TerminateSync((uint16_t)gs_sync_hdl);
                gs_sync_term = 1;
            }
            else
            {
                if(0xFFFF != (uint16_t)(gs_sync_hdl >> 16))
                {
                    ret = R_BLE_GAP_TerminateSync((uint16_t)(gs_sync_hdl >> 16));
                }
                else
                {
                    ret = BLE_ERR_INVALID_HDL;
                }
            }
        }
        else
        {
            uint16_t sync_hdl;
            sync_hdl = (uint16_t)strtol(argv[2], NULL, 0);
            ret = R_BLE_GAP_TerminateSync(sync_hdl);
        }
    }
    else
    {
        R_BLE_CLI_PrintUnrecognized();
    }

    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
    }
#else  /* (BLE_CFG_LIB_TYPE == 0)  */
    R_BLE_CLI_PrintError(BLE_ERR_UNSUPPORTED);
#endif /* (BLE_CFG_LIB_TYPE == 0)  */
}

static void abort_abs_sync(void)
{
    R_BLE_GAP_CancelCreateSync();
}

#if (BLE_CFG_LIB_TYPE == 0) 
static void delete_sync_hdl(st_ble_evt_data_t * p_data)
{
    st_ble_gap_sync_hdl_evt_t * p_sync_evt;
    uint16_t sync_hdl_1;

    gs_sync_scan = 0;

    p_sync_evt = (st_ble_gap_sync_hdl_evt_t *)p_data->p_param;
    pf("sync_hdl : 0x%04x \n", p_sync_evt->sync_hdl);
    sync_hdl_1 = (uint16_t)(gs_sync_hdl);
    if(sync_hdl_1 == p_sync_evt->sync_hdl)
    {
        gs_sync_hdl |= 0x0000FFFF;
    }
    else
    {
        gs_sync_hdl |= 0xFFFF0000;
    }

}
#endif /* (BLE_CFG_LIB_TYPE == 0) */

/*----------------------------------------------------------------------------------------------------------------------
    gap disconn command
----------------------------------------------------------------------------------------------------------------------*/
static void exec_abs_disconn(int argc, char *argv[])
{
    if (argc != 2)
    {
        R_BLE_CLI_PrintUnrecognized();
        return;
    }

    uint16_t conn_hdl;
    conn_hdl = (uint16_t)strtol(argv[1], NULL, 0);

    ble_status_t ret;
    /* 0x13: Remote User Terminated Connection. */
    ret = R_BLE_GAP_Disconnect(conn_hdl, 0x13);
    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
    }
}

/*----------------------------------------------------------------------------------------------------------------------
    gap device command
----------------------------------------------------------------------------------------------------------------------*/
static void exec_abs_device(int argc, char *argv[])
{
    for (int i = 0; i < BLE_CFG_RF_CONN_MAX; i++)
    {
        if (connected_device_info[i].conn_hdl != BLE_GAP_INVALID_CONN_HDL)
        {
            pf("conn_hdl=0x%04X, bdaddr=%s\n",
                             connected_device_info[i].conn_hdl,
                             BLE_BD_ADDR_STR(connected_device_info[i].addr.addr,
                                             connected_device_info[i].addr.type));
        }
    }
}

/*----------------------------------------------------------------------------------------------------------------------
    gap ver command
----------------------------------------------------------------------------------------------------------------------*/
static void exec_abs_ver(int argc, char *argv[])
{
    ble_status_t ret;
    ret = R_BLE_GAP_GetVerInfo();
    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
        gs_version_proc = 0x00;
    }
    else
    {
        gs_version_proc = 0x01;
    }
}



/*----------------------------------------------------------------------------------------------------------------------
    auth command
----------------------------------------------------------------------------------------------------------------------*/
static void exec_abs_auth(int argc, char *argv[])
{
    uint16_t conn_hdl;
    uint32_t passkey;
    ble_status_t ret = BLE_SUCCESS;
    uint8_t resp;

    if (argc > 6)
    {
        R_BLE_CLI_PrintUnrecognized();
        return;
    }

    if (strcmp(argv[1], "start") == 0)
    {
        conn_hdl = (uint16_t)strtol(argv[2], NULL, 0);
        ret = R_BLE_ABS_StartAuth(conn_hdl);
    }
    else if(strcmp(argv[1], "passkey") == 0)
    {
        if(3 != argc)
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }
        passkey = (uint32_t)strtol(argv[2], NULL, 10);
        ret = R_BLE_GAP_ReplyPasskeyEntry(gs_pairing_conn_hdl, passkey, BLE_GAP_PAIRING_ACCEPT);
    }
    else if(strcmp(argv[1], "numcmp") == 0)
    {
        resp = BLE_GAP_PAIRING_REJECT;
        if(strcmp(argv[2], "yes") == 0)
        {
            resp = BLE_GAP_PAIRING_ACCEPT;
        }

        ret = R_BLE_GAP_ReplyNumComp(gs_pairing_conn_hdl, resp);
    }
    else if(strcmp(argv[1], "del") == 0)
    {
        int32_t local = BLE_GAP_SEC_DEL_LOC_NONE;
        int32_t remote = BLE_GAP_SEC_DEL_REM_NONE;
        ble_gap_del_bond_cb_t cb = NULL;
        st_ble_dev_addr_t rem_dev;

        if(strcmp(argv[2], "local") == 0)
        {
            local = BLE_GAP_SEC_DEL_LOC_ALL;
        }
        else if(strcmp(argv[2], "remote") == 0)
        {
            if(argc > 3)
            {
                if(strcmp(argv[3], "addr") == 0 && (6 == argc))
                {
                    remote = BLE_GAP_SEC_DEL_REM_SA;
                    R_BLE_CMD_ParseAddr(argv[4], rem_dev.addr);

                    if (strcmp(argv[5], "rnd") == 0)
                    {
                        rem_dev.type = BLE_GAP_ADDR_RAND;
                    }
                    else if (strcmp(argv[5], "pub") == 0)
                    {
                        rem_dev.type = BLE_GAP_ADDR_PUBLIC;
                    }
                    else
                    {
                        R_BLE_CLI_PrintUnrecognized();
                        return;
                    }
                }
                else if(strcmp(argv[3], "all") == 0)
                {
                    remote = BLE_GAP_SEC_DEL_REM_ALL;
                }
                else if(strcmp(argv[3], "not_conn") == 0)
                {
                    remote = BLE_GAP_SEC_DEL_REM_NOT_CONN;
                }
                else
                {
                    R_BLE_CLI_PrintUnrecognized();
                    return;
                }
#if (BLE_CFG_EN_SEC_DATA == 1)
                cb = R_BLE_SECD_DelRemKeys;
#endif /* (BLE_CFG_EN_SEC_DATA == 1) */
            }
            else
            {
                R_BLE_CLI_PrintUnrecognized();
                return;
            }
        }
        else if(strcmp(argv[2], "all") == 0)
        {
            local = BLE_GAP_SEC_DEL_LOC_ALL;
            remote = BLE_GAP_SEC_DEL_REM_ALL;
#if (BLE_CFG_EN_SEC_DATA == 1)
            cb = R_BLE_SECD_DelRemKeys;
#endif /* (BLE_CFG_EN_SEC_DATA == 1) */
        }
        else
        {
            R_BLE_CLI_PrintUnrecognized();
            return;
        }

        R_BLE_GAP_DeleteBondInfo(local, remote, &rem_dev, cb);
#if (BLE_CFG_EN_SEC_DATA == 1)
        if(BLE_GAP_SEC_DEL_LOC_ALL== local)
        {
            R_BLE_SECD_DelLocInfo();
        }
#endif /* (BLE_CFG_EN_SEC_DATA == 1) */

    }
    else
    {
        R_BLE_CLI_PrintUnrecognized();
    }

    if (ret != BLE_SUCCESS)
    {
        R_BLE_CLI_PrintError(ret);
    }
}

void R_BLE_CMD_AbsGapCb(uint16_t type, ble_status_t result, st_ble_evt_data_t * p_data)
{
    (void)result;

    switch (type)
    {
        case BLE_GAP_EVENT_STACK_ON:
        {
            memset(connected_device_info,0xFF,sizeof(connected_device_info));
#if (BLE_CFG_LIB_TYPE == 0)
            gs_adv_cmd_status = 0;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
        } break;

        case BLE_GAP_EVENT_LOC_VER_INFO:
        {
            if(0x01 == gs_version_proc)
            {
                st_ble_gap_loc_dev_info_evt_t * p_ver_evt_param =
                    (st_ble_gap_loc_dev_info_evt_t *)p_data->p_param;
                pf("Link Layer / HCI Version\n");
                pf("HCI version           : 0x%02x\n", p_ver_evt_param->l_bt_info.hci_ver);
                pf("HCI revision          : 0x%04x\n", p_ver_evt_param->l_bt_info.hci_rev);
                pf("Link Layer version    : 0x%02x\n", p_ver_evt_param->l_bt_info.lmp_ver);
                pf("Link Layer subversion : 0x%04x\n", p_ver_evt_param->l_bt_info.lmp_sub_ver);
                pf("Manufacturer ID       : 0x%04x\n", p_ver_evt_param->l_bt_info.mnf_name);
                pf("Host stack Version\n");
                pf("major version         : 0x%02x\n", p_ver_evt_param->l_ver_num.major);
                pf("minor version         : 0x%02x\n", p_ver_evt_param->l_ver_num.minor);
                pf("subminor version      : 0x%02x\n", p_ver_evt_param->l_ver_num.subminor);
                pf("BLE FIT module Version\n");
                pf("major | minor version : 0x%08x\n", R_BLE_GetVersion());
                pf("lib type              : 0x%08x\n", R_BLE_GetLibType());
                gs_version_proc = 0x00;
                R_BLE_CLI_SetCmdComp();
            }
        } break;

        case BLE_GAP_EVENT_ADV_REPT_IND:
        {
            st_ble_gap_adv_rept_evt_t * p_adv_rept_evt_param =
                (st_ble_gap_adv_rept_evt_t *)p_data->p_param;

            switch (p_adv_rept_evt_param->adv_rpt_type)
            {
                case 0x00:
                {
                    st_ble_gap_adv_rept_t * p_adv_rept_param =
                        (st_ble_gap_adv_rept_t *)p_adv_rept_evt_param->param.p_adv_rpt;
                        pf("%s\n", BLE_BD_ADDR_STR(p_adv_rept_param->p_addr, p_adv_rept_param->addr_type));
                } break;

#if (BLE_CFG_LIB_TYPE == 0) 
                case 0x01:
                {
                    st_ble_gap_ext_adv_rept_t * p_ext_adv_rept_param =
                        (st_ble_gap_ext_adv_rept_t *)p_adv_rept_evt_param->param.p_ext_adv_rpt;
                    pf("%s %02x %04x\n", BLE_BD_ADDR_STR(p_ext_adv_rept_param->p_addr, p_ext_adv_rept_param->addr_type), p_ext_adv_rept_param->adv_sid, p_ext_adv_rept_param->perd_adv_intv);

                    if((1 == gs_sync_scan) && (0xFF != p_ext_adv_rept_param->adv_sid) && (0x0000 != p_ext_adv_rept_param->perd_adv_intv))
                    {
                        gs_sync_scan = 2;

                        /* found */
                        memcpy(gs_sync_advr.addr, p_ext_adv_rept_param->p_addr, BLE_BD_ADDR_LEN);
                        gs_sync_advr.type = p_ext_adv_rept_param->addr_type;
                        gs_adv_sid = p_ext_adv_rept_param->adv_sid;
                        R_BLE_GAP_ConfPerdAdvList(BLE_GAP_LIST_ADD_DEV, &gs_sync_advr, &gs_adv_sid, 1);
                    }

                } break;

                case 0x02:
                {
                    st_ble_gap_perd_adv_rept_t * p_perd_adv_rept_param =
                        (st_ble_gap_perd_adv_rept_t *) p_adv_rept_evt_param->param.p_per_adv_rpt;
                    pf("sync_hdl : 0x%04x \n", p_perd_adv_rept_param->sync_hdl);
                } break;
#endif /* (BLE_CFG_LIB_TYPE == 0)  */

                default:
                    break;
            }
        } break;

        case BLE_GAP_EVENT_CONN_IND:
        {
            pf("receive BLE_GAP_EVENT_CONN_IND result : 0x%04x\n", result);
            if (result == BLE_SUCCESS)
            {
                st_ble_gap_conn_evt_t *conn_evt_param = 
                    (st_ble_gap_conn_evt_t *)p_data->p_param;


                int32_t i;

                for (i = 0; i < BLE_CFG_RF_CONN_MAX; i++)
                {
                    if (connected_device_info[i].conn_hdl == BLE_GAP_INVALID_CONN_HDL)
                    {
                        break;
                    }
                }

                if(BLE_CFG_RF_CONN_MAX == i)
                {
                    R_BLE_CLI_SetCmdComp();
                    break;
                }

                connected_device_info[i].conn_hdl = conn_evt_param->conn_hdl;
                connected_device_info[i].addr.type = conn_evt_param->remote_addr_type;
                memcpy(connected_device_info[i].addr.addr,
                        conn_evt_param->remote_addr,
                        BLE_BD_ADDR_LEN);

                pf("gap: connected conn_hdl:0x%04X, addr:%s\n",
                   connected_device_info[i].conn_hdl,
                   BLE_BD_ADDR_STR(connected_device_info[i].addr.addr,
                                   connected_device_info[i].addr.type));
            }

            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_CREATE_CONN_COMP:
        {
            if(BLE_SUCCESS != result)
            {
                pf("receive BLE_GAP_EVENT_CREATE_CONN_COMP result : 0x%04x\n", result);
                R_BLE_CLI_SetCmdComp();
            }
        } break;

        case BLE_GAP_EVENT_DISCONN_IND:
        {
            st_ble_gap_disconn_evt_t *disc_evt_param =
                (st_ble_gap_disconn_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_DISCONN_IND result : 0x%04x\n", result);

            for (int32_t i = 0; i < BLE_CFG_RF_CONN_MAX; i++)
            {
                if (connected_device_info[i].conn_hdl == disc_evt_param->conn_hdl)
                {
                    pf("gap: disconnected conn_hdl:0x%04X, addr:%s, reason:0x%02x\n",
                       connected_device_info[i].conn_hdl,
                       BLE_BD_ADDR_STR(connected_device_info[i].addr.addr,
                                       connected_device_info[i].addr.type),
                       disc_evt_param->reason);

                    connected_device_info[i].conn_hdl = BLE_GAP_INVALID_CONN_HDL;
                    break;
                }
            }
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_SET_DATA_LEN_COMP:
        {
            st_ble_gap_conn_hdl_evt_t * p_data_len = (st_ble_gap_conn_hdl_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_SET_DATA_LEN_COMP result : 0x%04x, conn_hdl : 0x%04x\n", result, p_data_len->conn_hdl);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_DATA_LEN_CHG:
        {
            st_ble_gap_data_len_chg_evt_t * p_data_len = (st_ble_gap_data_len_chg_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_DATA_LEN_CHG result : 0x%04x, conn_hdl : 0x%04x \n", result, p_data_len->conn_hdl);
            pf("tx_octets : 0x%04x\n", p_data_len->tx_octets);
            pf("tx_time   : 0x%04x\n", p_data_len->tx_time);
            pf("rx_octets : 0x%04x\n", p_data_len->rx_octets);
            pf("rx_time   : 0x%04x\n", p_data_len->rx_time);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_PHY_SET_COMP :
        {
            st_ble_gap_conn_hdl_evt_t * p_phy_comop = (st_ble_gap_conn_hdl_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_PHY_SET_COMP result : 0x%04x, conn_hdl : 0x%04x\n", result, p_phy_comop->conn_hdl);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_PHY_UPD:
        {
            st_ble_gap_phy_upd_evt_t * p_phy_up = (st_ble_gap_phy_upd_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_PHY_UPD result : 0x%04x, conn_hdl : 0x%04x\n", result, p_phy_up->conn_hdl);
            pf("tx_phy : 0x%04x\n", p_phy_up->tx_phy);
            pf("rx_phy : 0x%04x\n", p_phy_up->rx_phy);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_CONN_PARAM_UPD_COMP:
        {
            st_ble_gap_conn_upd_evt_t * p_conn_up = (st_ble_gap_conn_upd_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_CONN_PARAM_UPD_COMP result : 0x%04x, conn_hdl : 0x%04x\n", result, p_conn_up->conn_hdl);
            pf("conn_intv    : 0x%04x\n", p_conn_up->conn_intv);
            pf("conn_latency : 0x%04x\n", p_conn_up->conn_latency);
            pf("sup_to       : 0x%04x\n", p_conn_up->sup_to);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_ADV_ON:
        {
            st_ble_gap_adv_set_evt_t * p_adv_on = (st_ble_gap_adv_set_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_ADV_ON result : 0x%04x, adv_hdl : 0x%04x\n", result, p_adv_on->adv_hdl);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_ADV_OFF:
        {
            st_ble_gap_adv_off_evt_t * p_adv_off = (st_ble_gap_adv_off_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_ADV_OFF result : 0x%04x, adv_hdl : 0x%04x\n", result, p_adv_off->adv_hdl);
            R_BLE_CLI_SetCmdComp();
        } break;


        case BLE_GAP_EVENT_PASSKEY_ENTRY_REQ:
        {
            pf("receive BLE_GAP_EVENT_PASSKEY_ENTRY_REQ\n");
            st_ble_gap_conn_hdl_evt_t * p_passkey_evt_param =
                (st_ble_gap_conn_hdl_evt_t *)p_data->p_param;

            gs_pairing_conn_hdl = p_passkey_evt_param->conn_hdl;
        } break;

        case BLE_GAP_EVENT_PASSKEY_DISPLAY_REQ:
        {
            pf("receive BLE_GAP_EVENT_PASSKEY_DISPLAY_REQ\n");
            st_ble_gap_passkey_display_evt_t * p_passkey_disp_evt_param =
                (st_ble_gap_passkey_display_evt_t *)p_data->p_param;
            pf("conn_hdl : 0%04x, passkey : %06d\n", 
                p_passkey_disp_evt_param->conn_hdl, p_passkey_disp_evt_param->passkey);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_NUM_COMP_REQ:
        {
            pf("receive BLE_GAP_EVENT_NUM_COMP_REQ\n");
            st_ble_gap_num_comp_evt_t * p_numcmp_evt_param =
                (st_ble_gap_num_comp_evt_t *)p_data->p_param;

            gs_pairing_conn_hdl = p_numcmp_evt_param->conn_hdl;
            pf("numeric : %06d\n", p_numcmp_evt_param->numeric);
        } break;

        case BLE_GAP_EVENT_PEER_KEY_INFO:
        {
            pf("receive BLE_GAP_EVENT_PEER_KEY_INFO\n");
            st_ble_gap_peer_key_info_evt_t * p_peer_key =
                (st_ble_gap_peer_key_info_evt_t *)p_data->p_param;

            pf("LTK : %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x \n", 
                    p_peer_key->key_ex_param.p_keys_info->enc_info[0],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[1],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[2],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[3],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[4],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[5],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[6],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[7],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[8],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[9],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[10],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[11],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[12],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[13],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[14],
                    p_peer_key->key_ex_param.p_keys_info->enc_info[15]);

            if(0 != (p_peer_key->key_ex_param.keys & BLE_GAP_KEY_DIST_IDKEY))
            {
                pf("IRK : %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x \n", 
                    p_peer_key->key_ex_param.p_keys_info->id_info[0],
                    p_peer_key->key_ex_param.p_keys_info->id_info[1],
                    p_peer_key->key_ex_param.p_keys_info->id_info[2],
                    p_peer_key->key_ex_param.p_keys_info->id_info[3],
                    p_peer_key->key_ex_param.p_keys_info->id_info[4],
                    p_peer_key->key_ex_param.p_keys_info->id_info[5],
                    p_peer_key->key_ex_param.p_keys_info->id_info[6],
                    p_peer_key->key_ex_param.p_keys_info->id_info[7],
                    p_peer_key->key_ex_param.p_keys_info->id_info[8],
                    p_peer_key->key_ex_param.p_keys_info->id_info[9],
                    p_peer_key->key_ex_param.p_keys_info->id_info[10],
                    p_peer_key->key_ex_param.p_keys_info->id_info[11],
                    p_peer_key->key_ex_param.p_keys_info->id_info[12],
                    p_peer_key->key_ex_param.p_keys_info->id_info[13],
                    p_peer_key->key_ex_param.p_keys_info->id_info[14],
                    p_peer_key->key_ex_param.p_keys_info->id_info[15]);
                
                st_ble_dev_addr_t remote;
                memcpy(remote.addr, &p_peer_key->key_ex_param.p_keys_info->id_addr_info[1], BLE_BD_ADDR_LEN);
                remote.type = p_peer_key->key_ex_param.p_keys_info->id_addr_info[0];
                pf("remote id addr : %s \n", BLE_BD_ADDR_STR(remote.addr, remote.type));
            }

            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_PAIRING_COMP:
        {
            pf("receive BLE_GAP_EVENT_PAIRING_COMP result : 0x%04x\n", result);
            st_ble_gap_pairing_info_evt_t * p_param;
            p_param = (st_ble_gap_pairing_info_evt_t *)p_data->p_param;
            pf("sec : 0x%02x, mode : 0x%02x,  bond : 0x%02x, key_size : 0x%02x\n", 
                p_param->auth_info.security, p_param->auth_info.pair_mode, 
                p_param->auth_info.bonding, p_param->auth_info.ekey_size);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_ENC_CHG:
        {
            pf("receive BLE_GAP_EVENT_ENC_CHG result : 0x%04x\n", result);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_SCAN_OFF:
        {
            pf("receive BLE_GAP_EVENT_SCAN_OFF result : 0x%04x\n", result);
#if (BLE_CFG_LIB_TYPE == 0) 
            if(3 == gs_sync_scan)
            {
                R_BLE_GAP_ConfWhiteList(BLE_GAP_LIST_REM_DEV, &gs_sync_advr, 1);
                R_BLE_GAP_ConfPerdAdvList(BLE_GAP_LIST_REM_DEV, &gs_sync_advr, &gs_adv_sid, 1);
            }
#endif /* (BLE_CFG_LIB_TYPE == 0)  */
        } break;

        case BLE_GAP_EVENT_WHITE_LIST_CONF_COMP:
        {
            pf("receive BLE_GAP_EVENT_WHITE_LIST_CONF_COMP result : 0x%04x\n", result);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_RSLV_LIST_CONF_COMP :
        {
            pf("receive BLE_GAP_EVENT_RSLV_LIST_CONF_COMP result : 0x%04x\n", result);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_PRIV_MODE_SET_COMP :
        {
            pf("receive BLE_GAP_EVENT_PRIV_MODE_SET_COMP result : 0x%04x\n", result);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_RPA_EN_COMP :
        {
            pf("receive BLE_GAP_EVENT_RPA_EN_COMP result : 0x%04x\n", result);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_RD_RPA_COMP :
        {
            st_ble_gap_rd_rpa_evt_t * p_param;
            p_param = (st_ble_gap_rd_rpa_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_RD_RPA_COMP result : 0x%04x\n", result);
            if(BLE_SUCCESS == result)
            {
                pf(" local rpa:%s \n", BLE_BD_ADDR_STR(p_param->addr.addr, p_param->addr.type));
                R_BLE_CLI_SetCmdComp();
            }
        } break;

#if (BLE_CFG_LIB_TYPE == 0) 
        case BLE_GAP_EVENT_PERD_ADV_ON:
        {
            pf("receive BLE_GAP_EVENT_PERD_ADV_ON result : 0x%04x\n", result);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_PERD_ADV_OFF:
        {
            pf("receive BLE_GAP_EVENT_PERD_ADV_OFF result : 0x%04x\n", result);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_CREATE_SYNC_COMP:
        {
            if(BLE_SUCCESS == result)
            {
                pf("sync req has been sent.\n");
                R_BLE_CLI_SetCmdComp();
            }
            else
            {
                pf("sync req has not been sent.\n");
                R_BLE_CLI_SetCmdComp();
            }
        } break;

        case BLE_GAP_EVENT_PERD_LIST_CONF_COMP:
        {
            pf("receive BLE_GAP_EVENT_PERD_LIST_CONF_COMP result : 0x%04x\n", result);
            R_BLE_CLI_SetCmdComp();
            if(2 == gs_sync_scan)
            {
                R_BLE_GAP_CreateSync(NULL, 0, 0x0000, 0x4000);
            }
        } break;

        case BLE_GAP_EVENT_SYNC_EST:
        {
            gs_sync_scan = 3;
            if(BLE_SUCCESS == result)
            {
                pf("sync established\n");

                uint16_t sync_hdl_1;
                uint16_t sync_hdl_2;
                st_ble_gap_sync_est_evt_t * p_sync_evt;
                p_sync_evt = (st_ble_gap_sync_est_evt_t *)p_data->p_param;
                pf("sync_hdl : 0x%04x \n", p_sync_evt->sync_hdl);
                sync_hdl_1 = (uint16_t)(gs_sync_hdl);
                sync_hdl_2 = (uint16_t)(gs_sync_hdl >> 16);
                if((0xFFFFFFFF == gs_sync_hdl) ||
                    ((0xFFFF == sync_hdl_1) && (p_sync_evt->sync_hdl != sync_hdl_2)))
                {
                    gs_sync_hdl &= 0xFFFF0000;
                    gs_sync_hdl |= (uint16_t)p_sync_evt->sync_hdl;
                }
                else
                {
                    if((0xFFFF == sync_hdl_2) && (p_sync_evt->sync_hdl != sync_hdl_1))
                    {
                        gs_sync_hdl &= 0x0000FFFF;
                        gs_sync_hdl |= (uint32_t)(p_sync_evt->sync_hdl << 16);
                    }
                }
            }
            else
            {
                pf("sync not established\n");
                gs_sync_scan = 0;
            }
            R_BLE_CLI_SetCmdComp();

        } break;

        case BLE_GAP_EVENT_SYNC_TERM:
        {
            pf("receive BLE_GAP_EVENT_SYNC_TERM result : 0x%04x \n", result);
            if(BLE_SUCCESS == result)
            {
                pf("sync terminated\n");
                delete_sync_hdl(p_data);
                if(1 == gs_sync_term)
                {
                    if(0xFFFF != (uint16_t)(gs_sync_hdl >> 16))
                    {
                        R_BLE_GAP_TerminateSync((uint16_t)(gs_sync_hdl >> 16));
                    }
                }
            }
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_SYNC_LOST:
        {
            pf("sync lost\n");
            delete_sync_hdl(p_data);
            R_BLE_CLI_SetCmdComp();
        } break;

        case BLE_GAP_EVENT_ADV_SET_REMOVE_COMP:
        {
            st_ble_gap_rem_adv_set_evt_t * p_adv_set = (st_ble_gap_rem_adv_set_evt_t *)p_data->p_param;
            pf("receive BLE_GAP_EVENT_ADV_SET_REMOVE_COMP result : 0x%04x, adv_hdl : 0x%04x\n", result, p_adv_set->adv_hdl);
            R_BLE_CLI_SetCmdComp();
        } break;
#endif /* (BLE_CFG_LIB_TYPE == 0)  */

        default:
            break;
    }
}

#else /* (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */

void R_BLE_CMD_AbsGapCb(uint16_t type, ble_status_t result, st_ble_evt_data_t * p_data)
{
    (void)type;
    (void)result;
    (void)&p_data;
    return;
}

const st_ble_cli_cmd_t g_abs_cmd;

#endif /* (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */
