r_sdc_spi_rx (SPI Mode SD Memory Card Driver)
=======================


Overview
--------
This driver is a block type device driver to control the SD mode SD memory card using SPI Interface.


Features
--------
* SD Specifications Part 1 Physical Layer Simplified Specifications version 6.00 (SD memory control commands are supported)
* Block type device driver which takes 1 block to be 512 bytes
* SPI mode (1 bits) is supported.
* Default speed mode are supported.
* Standard capacity SD memory cards (SDSC) and high-capacity SD memory cards (SDHC, SDXC) are supported.
* Only SD Card operating voltage 3.3V supported.
* Only user areas are supported.
* Only CD pin detection is supported.



File Structure
--------------
r_sdc_spi_rx
|   r_sdc_spi_rx_if.h
|   readme.txt
|
+---doc
|   |
|   +---en
|   |   r01an6908ej0100-rx-sdmemory-spi.pdf
|   |
|   +---ja
|       r01an6908jj0100-rx-sdmemory-spi.pdf
|
+---src
        r_sdc_spi_close.c
        r_sdc_spi_get_info.c
        r_sdc_spi_get_status.c
        r_sdc_spi_getcarddetection.c
        r_sdc_spi_initialize.c
        r_sdc_spi_log.c
        r_sdc_spi_open.c
        r_sdc_spi_private.h
        r_sdc_spi_read.c
        r_sdc_spi_rx.c
        r_sdc_spi_sub.c
        r_sdc_spi_write.c

r_config
    r_sdc_spi_rx_config.h

