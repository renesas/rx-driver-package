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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/*******************************************************************************
* File Name    : r_mesh_rx23w_if.h
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version  Description
*           30.09.2019 1.00     First Release.
*           29.11.2019 1.01     Add random-delay to advertising transmission.
*******************************************************************************/

#ifndef R_MESH_RX23W_IF_H
#define R_MESH_RX23W_IF_H

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Mesh Stack headers */
#include "MS_access_api.h"
#include "MS_assigned_numbers.h"
#include "MS_brr_api.h"
#include "MS_common.h"
#include "MS_config_api.h"
#include "MS_error.h"
#include "MS_generic_battery_api.h"
#include "MS_generic_default_transition_time_api.h"
#include "MS_generic_level_api.h"
#include "MS_generic_location_api.h"
#include "MS_generic_onoff_api.h"
#include "MS_generic_power_level_api.h"
#include "MS_generic_power_onoff_api.h"
#include "MS_generic_property_api.h"
#include "MS_health_client_api.h"
#include "MS_health_server_api.h"
#include "MS_light_ctl_api.h"
#include "MS_light_hsl_api.h"
#include "MS_light_lc_api.h"
#include "MS_light_lightness_api.h"
#include "MS_light_xyl_api.h"
#include "MS_ltrn_api.h"
#include "MS_model_states.h"
#include "MS_net_api.h"
#include "MS_prov_api.h"
#include "MS_scene_api.h"
#include "MS_scheduler_api.h"
#include "MS_sensor_api.h"
#include "MS_time_api.h"
#include "MS_trn_api.h"
#include "MS_version.h"
#include "EM_os.h"
#include "EM_platform.h"
#include "EM_timer.h"
#include "mempool_pl.h"
#include "MS_features.h"
#include "nvs.h"
#include "nvsto.h"
#include "storage_pl.h"
#include "prov_pl.h"

/* Mesh Stack configurations */
#include "r_mesh_rx23w_config.h"

#endif /* R_MESH_RX23W_IF_H */
