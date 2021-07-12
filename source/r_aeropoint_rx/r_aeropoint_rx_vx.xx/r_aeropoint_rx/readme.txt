PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_aeropoint_rx
==========

Document Number
---------------
R01AN5793EJ0100 (English version)
R01AN5793JJ0100 (Japanese version)

Version
-------
v1.00

Overview
--------
Aeropoint GUI is designed to provide the one-stop solution to plan, design, embed and validate in a GUI development for embedded system with MCU. Aeropoint GUI realizes the division of labor between designing GUI and developing MCU system. Engineers and designers can design and implement GUI with modifying PowerPoint(r) file. It makes easy to make variations such as multiple variations and inexpensive edition.

Aeropoint FIT module provides above features as library files to be used in RX MCU. This module can be implemented with
using Smart Configurator and QE for Display. Then the dependent modules can also be implemented simultaneously. In 
order to implement this module manually, the configuration options need to be modified which is found in 
"r_config\cri_aero_config_ais.h". An original copy of the configuration file is stored in 
"r_aeropoint_rx\ref\cri_aero_config_ais_reference.h".

Features
--------
* Output GUI to the LCD panel.

Supported MCUs
--------------
* RX651 Group
* RX65N Group(ROM 1.5-2.0MB Models)
* RX72M Group
* RX72N Group
* RX66N Group

Boards Tested On
----------------
* Renesas Envision Kit RPBRX65N
* Renesas Envision Kit RPBRX72N
* RSKRX72N

Required Packages
-----------------
AeropointGUI Engine
* r_bsp
* r_glcdc_rx
* r_cmt_rx

SD
* r_sdhi_rx
* r_sdc_sdmem_rx
* r_longq
* r_dmaca_rx

D-Amp
* r_cmt_rx

Flash
* r_flash_spi
* r_qspi_smstr_rx
* r_memdrv_rx
* r_longq
* r_dmaca_rx
* r_sys_time_rx

LAN
* r_ether_rx
* r_socket_rx
* r_t4_diriver_rx
* r_t4_driver

UART
* r_sci_rx
* r_dmaca_rx
* r_byteq

Touch
* r_sci_iic_rx

Toolchain(s) Used
-----------------
* Renesas RX v3.02.00
* Renesas RX v2.08.01

File Structure
--------------
r_aeropoint_rx
|      readme.txt
|
+---doc
|   +---en
|   |       AeropointGUI_UsersManual_xxxxxxxx_EN.pdf
|   |       r01an5793ej0100-rx-aeropoint.pdf
|   |
|   +---ja
|           AeropointGUI_UsersManual_xxxxxxxx_JP.pdf
|           r01an5793jj0100-rx-aeropoint.pdf
|
+---include
|      cri_aero.h
|      cri_aero_color_def.h
|      cri_aero_command_def.h
|      cri_aero_command_internal.h
|      cri_aero_command_queue.h
|      cri_aero_command_queue_internal.h
|      cri_aero_config_internal.h
|      cri_aero_dataflash.h
|      cri_aero_debugtext.h
|      cri_aero_draw.h
|      cri_aero_error.h
|      cri_aero_flash_internal.h
|      cri_aero_flash_map_internal.h
|      cri_aero_font.h
|      cri_aero_graphics.h
|      cri_aero_graphics_internal.h
|      cri_aero_kitting_def.h
|      cri_aero_logprint.h
|      cri_aero_main.h
|      cri_aero_matrix.h
|      cri_aero_mnumber.h
|      cri_aero_movie.h
|      cri_aero_player.h
|      cri_aero_primitive_draw.h
|      cri_aero_report_internal.h
|      cri_aero_sd_internal.h
|      cri_aero_sd_map_internal.h
|      cri_aero_serial.h
|      cri_aero_serial_internal.h
|      cri_aero_shape.h
|      cri_aero_slide.h
|      cri_aero_sound.h
|      cri_aero_speech_recognition.h
|      cri_aero_storage_format_internal.h
|      cri_aero_stream_internal.h
|      cri_aero_support_atom_internal.h
|      cri_aero_testio.h
|      cri_aero_timer_internal.h
|      cri_aero_touch.h
|      cri_aero_touch_internal.h
|      cri_atom.h
|      cri_atom_device.h
|      cri_atom_rxv2.h
|      cri_atom_rxv3.h
|      cri_atom_sg.h
|      cri_sfr_config_rxv2.h
|      cri_sfr_config_rxv3.h
|      cri_xpt.h
|      cri_xpts_aerolite.h
|      cri_xpt_post.h
|      r_flash_spi_lib.h
|      r_sdc_sdmem_rx_lib.h
|      r_sdc_sdmem_rx_pin.h
|      r_sdc_sdmem_rx_pin_config.h
|
+---lib
|      libcri_AeropointLite_RXv2_CCRX.lib
|      libcri_AeropointLite_RXv3_CCRX.lib
|      libcri_Atom_Multiplayer_RXv2_CCRX.lib
|      libcri_Atom_Multiplayer_RXv3_CCRX.lib
|
+---ref
|      cri_aero_config_ais_reference.h
|
+---src
|  |  cri_aero_config_ais.c
|  |  cri_aero_flash_ais.c
|  |  cri_aero_flash_map_ais.c
|  |  cri_aero_graphics_ais.c
|  |  cri_aero_sd_ais.c
|  |  cri_aero_sd_map_ais.c
|  |  cri_aero_serial_ais.c
|  |  cri_aero_socket_ais.c
|  |  cri_aero_support_atom_ais.c
|  |  cri_aero_timer_ais.c
|  |  cri_aero_touch_ais.c
|  |  r_flash_spi_lib.c
|  |  r_sdc_sdmem_rx_lib.c
|  |  r_sdc_sdmem_rx_pin.c
|  |
|  +---targets
|      +---rx65n
|      |      cri_aero_support_atom_rx65n_ais.c
|      |      
|      +---rx66n
|      |      cri_aero_support_atom_rx66n_ais.c
|      |      
|      +---rx72m
|      |      cri_aero_support_atom_rx72m_ais.c
|      |      
|      +---rx72n
|             cri_aero_support_atom_rx72n_ais.c
|
+---tool
        AeropointLiteToolKit_xxxxxxxxRx_CRI_64bit.msi

r_config
       cri_aero_config_ais.h

