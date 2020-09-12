PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION.

r_longq
=======

Overview
--------------------------------------------------------------------------------
The r_longq module is a collection of circular buffer routines for 32-bit data.
The application passes a buffer to be used as a circular buffer to the Open() 
function which assigns a queue control block to it to handle indexing. The 
Open() function returns a handle which is then used as a queue/buffer id for all 
other API functions. These functions include routines for adding and removing 
data from a queue, inspecting the amount of data in a queue, and the ability to 
flush a queue.

The queue control blocks can be allocated at compile time or dynamically at run
time. A configuration option for this exists in "r_config\r_longq_config.h".
An original copy of the configuration file is stored in "r_longq\ref\
r_longq_config_reference.h".


Features
--------
* Statically or dynamically allocated queue control blocks.
* Number of queues limited only by the amount of RAM available on the mcu.

File Structure
--------------
r_longq
|   readme.txt
|   r_longq_if.h
|
+---doc
|    +---en
|    |      r01an1889ej{VERSION_NUMBER}-rx.pdf
|    |
|    +---ja
|           r01an1889jj{VERSION_NUMBER}-rx.pdf
|
+---ref
|       r_longq_config_reference.h
|
+---src
        r_longq.c
        r_longq_private.h
   
r_config
    r_longq_config.h

