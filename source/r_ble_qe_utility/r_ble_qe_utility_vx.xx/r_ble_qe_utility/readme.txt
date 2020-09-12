PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_ble_qe_utility
=======================

Document Number
---------------
R01AN4907EJ
R01AN4907JJ

Version
-------
v1.00

Overview
--------
BLE QE Utility module provides you with the feature of generating programs from QE for BLE.

Features
--------
* Supported service
    Alert Notification Service                  Automation IO Service
    Battery Service                             Blood Pressure Service
    Body Composition Service                    Bond Management Service
    Continuous Glucose Monitoring Service       Current Time Service
    Cycling Power Service                       Cycling Speed and Cadence Service
    Device Information Service                  Environmental Sensing Service
    Fitness Machine Service                     Generic Access Profile Service
    Generic Attribute Service                   Glucose Service
    Health Thermometer Service                  Heart Rate Service
    Human Interface Device Service              Immediate Alert Service
    Insulin Delivery Service                    Link Loss Service
    Location and Navigation Service             Next DST Change Service
    Object Transfer Service                     Phone Alert Status Service
    Pulse Oximeter Service                      Reconnection Configuration Service
    Reference Time Update Service               Running Speed and Cadence Service
    Scan Parameters Service                     Tx Power Service
    User Data Service                           Weight Scale Service

Supported MCUs
--------------
* RX23W

Boards Tested On
----------------
* Target Board for RX23W

Limitations
-----------
* Support only little endian

Peripherals Used Directly
-------------------------
* None

Required Packages
-----------------
* r_ble_rx23w
* r_bsp

How to add to your project
--------------------------
This module must be added to each project in which it is used.
 Renesas recommends using ÅgSmart ConfiguratorÅh described in (1) or (3).

(1) Adding the FIT module to your project using ÅgSmart ConfiguratorÅh in e2 studio. By using the ÅgSmart ConfiguratorÅh in e2 studio, the FIT module is automatically added to your project. Refer to ÅgRenesas e2 studio Smart Configurator User Guide (R20AN0451)Åh for details.

(2) Adding the FIT module to your project using ÅgFIT ConfiguratorÅh in e2 studio. By using the ÅgFIT ConfiguratorÅh in e2 studio, the FIT module is automatically added to your project. Refer to ÅgAdding Firmware Integration Technology Modules to Projects (R01AN1723)Åh for details.

(3) Adding the FIT module to your project using ÅgSmart ConfiguratorÅh on CS+ By using the ÅgSmart Configurator Standalone versionÅh in CS+, the FIT module is automatically added to your project. Refer to ÅgRenesas e2 studio Smart Configurator User Guide (R20AN0451)Åh for details.

(4) Adding the FIT module to your project in CS+ In CS+, please manually add the FIT module to your project. Refer to ÅgAdding Firmware Integration Technology Modules to CS+ Projects (R01AN1826)Åh for details.


Toolchain(s) Used
-----------------
* Renesas RX v2.08


File Structure
--------------

r_ble_qe_utility
| readme.txt
|
+---doc
|   |
|   +---en
|   |       r01an4907ej0100-ble-qe-util.pdf
|   +---ja
|           r01an4907jj0100-ble-qe-util.pdf
+---utilities
        qe-rble-cli.zip
        qe-sig-adopted-json.zip



