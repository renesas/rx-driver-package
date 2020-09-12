PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_mesh_rx23w
=======================

Document Number
---------------
R01AN4930EJ0101
R01AN4930JJ0101

Version
-------
v1.01

Overview
--------
The r_mesh_rx23w module provides the features to perform many-to-many wireless communication
in a mesh network which is compliant with Bluetooth Mesh Networking Specifications.

Features
--------
* Bluetooth Mesh Profile layers
  - Access layer
  - Upper Transport layer
  - Lower Transport layer
  - Newtwork layer
  - Bearer layer
  - Provisioning layer

* Bluetooth Mesh Profile optional features
  - Relay feature
  - Proxy feature
  - Friend feature
  - Low Power feature

* Bluetooth Mesh Models
  - Configuration model
  - Health model
  - Generic models
  - Light models
  - Scheduler model
  - Sensor model
  - Scene model
  - Time models

Supported MCUs
--------------
* RX23W

Limitations
-----------
* Support only little endian.

Peripherals Used Directly
-------------------------
* BLE (Bluetooth Low Energy)
* E2 Data Flash memory

Required Packages
-----------------
* r_bsp
* r_ble_rx23w
* r_flash_rx

Toolchain(s) Used
-----------------
* Renesas CC-RX v2.08

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (2).

(1)    Adding the FIT module to your project using "Smart Configurator" in e2 studio
    By using the "Smart Configurator" in e2 studio, the FIT module is automatically added to your project.
    Refer to "RX Smart Configurator User Guide: e2 studio (R20AN0451)" for details.

(2)    Adding the FIT module to your project using "Smart Configurator" on CS+
    By using the "Smart Configurator Standalone version" in CS+, the FIT module is automatically added to your project.
    Refer to "RX Smart Configurator User Guide: e2 studio (R20AN0451)" for details.

(3)    Adding the FIT module to your project using "FIT Configurator" in e2 studio
    By using the "FIT Configurator" in e2 studio, the FIT module is automatically added to your project.
    Refer to "Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(4)    Adding the FIT module to your project in CS+
    In CS+, please manually add the FIT module to your project.
    Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.

File Structure
--------------
+---r_mesh_rx23w
|   |   readme.txt
|   |   r_mesh_rx23w_if.h
|   |   
|   +---doc
|   |   |   blemesh_api.chm
|   |   |   
|   |   +---en
|   |   |       r01an4930ej0101-rx23w-blemesh.pdf
|   |   +---ja
|   |           r01an4930jj0101-rx23w-blemesh.pdf
|   |           
|   +---lib
|   |       lib_ble_ms_ccrx.lib
|   |       
|   +---ref
|   |       r_mesh_rx23w_config_reference.h
|   |       
|   +---src
|       |   
|       +---bearer
|       |       blebrr.c
|       |       blebrr.h
|       |       blebrr_gatt.c
|       |       blebrr_pl.c
|       |       gatt_clients.c
|       |       gatt_clients.h
|       |       gatt_db_prov.c
|       |       gatt_db_prov.h
|       |       gatt_db_proxy.c
|       |       gatt_db_proxy.h
|       |       gatt_services.c
|       |       gatt_services.h
|       |       
|       +---drivers
|       |       mesh_dataflash.c
|       |       mesh_dataflash.h
|       |       mesh_resources.c
|       |       mesh_resources.h
|       |       
|       +---include
|           |   MS_access_api.h
|           |   MS_assigned_numbers.h
|           |   MS_brr_api.h
|           |   MS_common.h
|           |   MS_config_api.h
|           |   MS_error.h
|           |   MS_generic_battery_api.h
|           |   MS_generic_default_transition_time_api.h
|           |   MS_generic_level_api.h
|           |   MS_generic_location_api.h
|           |   MS_generic_onoff_api.h
|           |   MS_generic_power_level_api.h
|           |   MS_generic_power_onoff_api.h
|           |   MS_generic_property_api.h
|           |   MS_health_client_api.h
|           |   MS_health_server_api.h
|           |   MS_light_ctl_api.h
|           |   MS_light_hsl_api.h
|           |   MS_light_lc_api.h
|           |   MS_light_lightness_api.h
|           |   MS_light_xyl_api.h
|           |   MS_ltrn_api.h
|           |   MS_model_states.h
|           |   MS_net_api.h
|           |   MS_prov_api.h
|           |   MS_scene_api.h
|           |   MS_scheduler_api.h
|           |   MS_sensor_api.h
|           |   MS_time_api.h
|           |   MS_trn_api.h
|           |   MS_version.h
|           |   
|           +---platform
|                   EM_os.h
|                   EM_platform.h
|                   EM_timer.h
|                   mempool_pl.h
|                   MS_features.h
|                   nvs.h
|                   nvsto.h
|                   prov_pl.h
|                   storage_pl.h
|                   
+---r_config
        r_mesh_rx23w_config.h

