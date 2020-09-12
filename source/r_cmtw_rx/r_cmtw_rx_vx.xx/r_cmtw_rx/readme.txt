PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

r_cmtw_rx
=========

Overview
--------
This module creates a timer tick using a CMTW channel based on an input by the user.  
The module also supports output compare and input capture features to generate 
waveforms and detect external events.  

Features
--------
* Create periodic or one-shot timer
* Create periodic or one-shot waveforms
* Detect external events and capture time 
* User can be alerted through a callback function


File Structure
--------------
r_cmtw_rx
|   readme.txt
|   r_cmtw_rx_if.h
|
+---doc
|   |
|   +---en
|   |   r01an2199ej{VERSION_NUMBER}-rx-cmtw.pdf
|   |
|   |
|   +---ja
|       r01an2199jj{VERSION_NUMBER}-rx-cmtw.pdf
|
+---ref
|       r_cmtw_rx_config_reference.h
|
\---src
        r_cmtw_rx.c

r_config
    r_cmtw_rx_config.h
    
r_cmtw_rx.ftl
