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
|       r20an0038ej0411-rx-tfat.pdf
|   +---ja
|       r20an0038jj0411-rx-tfat.pdf
|
+---src
        diskio.h
        ff.c
        ff.h
        ffconf.h
        ffsystem.c
        ffunicode.c

r_config
    r_tfat_rx_config.h