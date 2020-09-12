PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_ble_rx23w
=======================

Document Number
---------------
R01AN4860EJ
R01AN4860JJ

Version
-------
v1.10

Overview
--------
The r_ble_rx23w module provides Bluetooth Low Energy(BLE) functions 
which are compliant with Bluetooth version 5.0.

Features
--------
* Bluetooth ver 5.0 Features
  - LE 2M PHY
  - LE Coded PHY
  - LE Advertising Extensions
  - LE Channel Selection Algorithm #2
  - High Duty Cycle Non-Connectable Advertising

* Bluetooth ver 4.2 Features
  - LE Secure Connections
  - Link Layer privacy
  - Link Layer Extended Scan Filter policies
  - LE Data Packet Length Extension

* Bluetooth ver 4.1 Features
  - LE L2CAP Connection Oriented Channel Support
  - Low Duty Cycle Directed Advertising
  - 32-bit UUID Support in LE
  - LE Link Layer Topology
  - LE Ping

Supported MCUs
--------------
* RX23W

Boards Tested On
----------------
* Target Board for RX23W
* RSSKRX23W

Limitations
-----------
* Support only little endian.
* Required ICLK and PCLKB more than 8MHz.
* R_BLE API must be executed in supervisor mode.

Peripherals Used Directly
-------------------------
* BLE (Bluetooth Low Energy)
* BLE_IRQ
* CMT(CMT2, CMT3)


Required Packages
-----------------
* r_bsp
* r_cmt_rx
* r_lpc_rx
* r_flash_rx (option)
* r_gpio_rx (option)
* r_irq_rx (option)
* r_sci_rx / r_byteq (option)

How to add to your project
--------------------------
This module must be added to each project in which it is used.
Renesas recommends using "Smart Configurator" described in (1) or (3).
However, "Smart Configurator" only supports some RX devices.
Please use the methods of (2) or (4) for unsupported RX devices.

(1)    Adding the FIT module to your project using "Smart Configurator" in e2 studio
By using the "Smart Configurator" in e2 studio, 
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(2)    Adding the FIT module to your project using "FIT Configurator" in e2 studio
By using the "FIT Configurator" in e2 studio,
the FIT module is automatically added to your project.
Refer to "Adding Firmware Integration Technology Modules to Projects (R01AN1723)" for details.

(3)    Adding the FIT module to your project using "Smart Configurator" on CS+
By using the "Smart Configurator Standalone version" in CS+,
the FIT module is automatically added to your project.
Refer to "Renesas e2 studio Smart Configurator User Guide (R20AN0451)" for details.

(4)    Adding the FIT module to your project in CS+
In CS+, please manually add the FIT module to your project.
Refer to "Adding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)" for details.


Toolchain(s) Used
-----------------
* Renesas RX v2.08
* IAR C/C++ Compiler for Renesas RX version 4.12.1

File Structure
--------------

-+--r_ble_rx23w
 |  |  readme.txt
 |  |  r_ble_rx23w_if.h
 |  |
 |  +---doc
 |  |    |  r_ble_api_spec.chm
 |  |    |
 |  |    +---en
 |  |    |      r01an4860ej0110-rx23w-ble.pdf
 |  |    +---ja
 |  |           r01an4860jj0110-rx23w-ble.pdf
 |  |
 |  +---lib
 |  |      ble_fit_lib_selector.bat
 |  |      lib_ble_ps_ccrx.lib
 |  |      lib_ble_ps_ccrx_a.lib
 |  |      lib_ble_ps_ccrx_b.lib
 |  |      lib_ble_ps_ccrx_c.lib
 |  |      lib_ble_ps_hci_ccrx.lib
 |  |      lib_ble_ps_hci_ccrx_a.lib
 |  |      lib_ble_ps_hci_ccrx_b.lib
 |  |      lib_ble_ps_hci_ccrx_c.lib
 |  |
 |  +---ref
 |  |      r_ble_rx23w_config_reference.h
 |  |
 |  +---src
 |       +---app_lib
 |       |    +---abs
 |       |    |      r_ble_abs_api.c
 |       |    |      r_ble_abs_api.h
 |       |    |
 |       |    +---board
 |       |    |      r_ble_board.c
 |       |    |      r_ble_board.h
 |       |    |
 |       |    +---cli
 |       |    |  |  r_ble_cli.c
 |       |    |  |  r_ble_cli.h
 |       |    |  |
 |       |    |  +---console
 |       |    |      |  r_ble_console.h
 |       |    |      |
 |       |    |      +---rx_family
 |       |    |              r_ble_console.c
 |       |    |
 |       |    +---cmd
 |       |    |      r_ble_cmd.c
 |       |    |      r_ble_cmd.h
 |       |    |      r_ble_cmd_abs.c
 |       |    |      r_ble_cmd_abs.h
 |       |    |      r_ble_cmd_sys.c
 |       |    |      r_ble_cmd_sys.h
 |       |    |      r_ble_cmd_vs.c
 |       |    |      r_ble_cmd_vs.h
 |       |    |
 |       |    +---discovery
 |       |    |      r_ble_disc.c
 |       |    |      r_ble_disc.h
 |       |    |
 |       |    +---logger
 |       |    |      r_ble_logger.c
 |       |    |      r_ble_logger.h
 |       |    |
 |       |    +---profile_cmn
 |       |    |      r_ble_profile_cmn.h
 |       |    |      r_ble_servc_if.c
 |       |    |      r_ble_servc_if.h
 |       |    |      r_ble_servs_if.c
 |       |    |      r_ble_servs_if.h
 |       |    |      r_ble_serv_common.c
 |       |    |      r_ble_serv_common.h
 |       |    |
 |       |    +---sec_data
 |       |    |      r_ble_sec_data.c
 |       |    |      r_ble_sec_data.h
 |       |    |
 |       |    +---timer
 |       |          r_ble_timer.c
 |       |          r_ble_timer.h
 |       |          r_ble_timer_rx23w.c
 |       |
 |       +---platform
 |               |  r_ble_pf_configs.c
 |               |  r_ble_pf_config_private.h
 |               |  r_ble_pf_functions.c
 |               |  r_ble_pf_lowpower.c
 |               |
 |               +---driver
 |                     +---dataflash
 |                           r_ble_dataflash.c
 |   
-+--r_config
        r_ble_rx23w_config.h
