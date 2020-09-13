r_tfat_driver_rx
=========


Overview
--------
This memory driver interface connects TFAT and Device Drivers(USB etc).
TFAT is a FAT file system which just requres small memory.
TFAT is based on FatFs.
<br/>
Please refer to the
 https://www.renesas.com/mw/tfat


Features
--------
* This memory driver interface connects TFAT and Device Drivers(USB etc).



File Structure
--------------
r_config
    r_tfat_driver_rx_config.h

r_tfat_driver_rx
|   readme.txt
|   r_tfat_driver_rx_if.h
|
+---doc
|   |
|   +---en
|   |   r20an0335ej0200-rx-tfat.pdf
|   +---ja
|       r20an0335jj0200-rx-tfat.pdf
+---src
|   |   r_tfat_drv_if.c
|   |   r_tfat_drv_if_dev.h
|   |
|   +---device
|       |
|       +---sdmem
|       |      r_tfat_drv_if_sdmem.c
|       +---usb
|       |      r_tfat_drv_if_usb.c
|       +---usb_mini
|              r_tfat_drv_if_usb_mini.c
|
+---ref
          r_tfat_driver_rx_config_reference.h

