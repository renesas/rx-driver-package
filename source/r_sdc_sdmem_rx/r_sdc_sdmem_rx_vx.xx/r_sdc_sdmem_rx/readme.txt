r_sdc_sdmem_rx (SD Mode SD Memory Card Driver)
=======================


Overview
--------
This driver is a block type device driver to control the SD mode SD memory card using SD Host Interface (SDHI).


Features
--------
* SD Specifications Part 1 Physical Layer Simplified Specifications version 6.00 (SD memory control commands are supported)
* Block type device driver which takes 1 block to be 512 bytes
* SD mode (4 bits) is supported.
* Default speed mode are supported.
* Standard capacity SD memory cards (SDSC) and high-capacity SD memory cards (SDHC, SDXC) are supported.
* Only user areas are supported.
* Only CD pin detection is supported.



File Structure
--------------
r_sdc_sdmem_rx
|   r_sdc_sd_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |   r01an4233ej0300-rx-sdmemory.pdf
|   |
|   +---ja
|       r01an4233jj0300-rx-sdmemory.pdf
|
+---ref
|       r_sdc_sd_rx_config_reference.h
|
+---src
    |   r_sdc_sd_cd.c
    |   r_sdc_sd_cmd.c
    |   r_sdc_sd_config.c
    |   r_sdc_sd_initialize.c
    |   r_sdc_sd_int.c
    |   r_sdc_sd_open.c
    |   r_sdc_sd_rx_private.h
    |   r_sdc_sd_trans.c
    |   r_sdc_sd_util.c
    |   r_sdc_sd_wrapper.c
    |
    +---mem
            r_sdc_sdmem_initialize.c
            r_sdc_sdmem_param.c
            r_sdc_sdmem_read.c
            r_sdc_sdmem_util.c
            r_sdc_sdmem_wrapper.c
            r_sdc_sdmem_write.c

r_config
    r_sdc_sd_rx_config.h

