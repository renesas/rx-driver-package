r_tfat_rx
=========


Overview
--------
TFAT is a FAT file system which just requres small memory.
TFAT is based on FatFs.


Features
--------
* TFAT is a FAT file system which just requres small memory.



File Structure
--------------
r_tfat_rx
|   readme.txt
|
+---doc
|   +---en
|       r20an0038ej0400-rx-tfat.pdf
|   +---ja
|       r20an0038jj0400-rx-tfat.pdf
|
+---src
|         diskio.h
|         ff.c
|         ff.h
|         ffconf.h
|         ffsystem.c
|         ffunicode.c
|
+---FITDemos
|   |
|   +---rx65n_1mb_rsk_sdcard_fat_sample
|   +---rx65n_1mb_rsk_usb_fat_sample
|   +---rx231_rsk_usb_fat_sample
|
+---ref
          r_tfat_rx_config_reference.h

r_config
    r_tfat_rx_config.h