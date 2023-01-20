#!/bin/bash
# ==================================================================
#  Rename bluetooth protocol stack library 
# ==================================================================

cd `dirname $0`

# Set configuration option header
CONFIG_H=../../r_config/r_ble_rx23w_config.h
# CONFIG_H=../../rble/include/r_ble_rx23w_config.h
# CONFIG_H=../../../../rble/include/r_ble_rx23w_config.h

# Set library type environment variable
#@for /f "tokens=3 usebackq" %%i in (`findstr  /B /C:"#define BLE_CFG_LIB_TYPE" %CONFIG_H%`) do @SET RESULT=%%i
#@set BLE_LIB_TYPE=%RESULT:~1,-1%
lib_type_grep=($(grep "^#define BLE_CFG_LIB_TYPE" $CONFIG_H))
BLE_LIB_TYPE=${lib_type_grep[2]:1:1}

# Set HCI support environment variable
#@for /f "tokens=3 usebackq" %%i in (`findstr /B /C:"#define BLE_CFG_HCI_MODE_EN" %CONFIG_H%`) do @SET HCI_RESULT=%%i
#@set HCI_SUPPORT=%HCI_RESULT:~1,-1%
hci_mode_grep=($(grep "^#define BLE_CFG_HCI_MODE_EN" $CONFIG_H))
HCI_SUPPORT=${hci_mode_grep[2]:1:1}


# Set source library prefix
if [ $HCI_SUPPORT -eq 1 ]; then
    PS_S_LIB_PREFIX=lib_ble_ps_hci_
else
    PS_S_LIB_PREFIX=lib_ble_ps_
fi

# Set source library type
if [ $BLE_LIB_TYPE -eq 1 ]; then
    PS_S_LIB_TYPE=b
elif [ $BLE_LIB_TYPE -eq 2 ]; then
    PS_S_LIB_TYPE=c
else
    PS_S_LIB_TYPE=a
fi

# Rename library
cp -f ${PS_S_LIB_PREFIX}ccrx_${PS_S_LIB_TYPE}.lib     lib_ble_ps_ccrx.lib

