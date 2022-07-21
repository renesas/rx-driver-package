@rem ;;==================================================================
@rem ;; Rename bluetooth protocol stack library 
@rem ;;==================================================================

cd %~dp0

@rem ;; Set configuration option header
@set CONFIG_H=..\..\r_config\r_ble_rx23w_config.h
@REM set CONFIG_H=..\..\rble\include\r_ble_rx23w_config.h
@REM @set CONFIG_H=..\..\..\..\rble\include\r_ble_rx23w_config.h

@rem ;; Set library type environment variable
@for /f "tokens=3 usebackq" %%i in (`findstr  /B /C:"#define BLE_CFG_LIB_TYPE" %CONFIG_H%`) do @SET RESULT=%%i
@set BLE_LIB_TYPE=%RESULT:~1,-1%

@rem ;; Set HCI support environment variable
@for /f "tokens=3 usebackq" %%i in (`findstr /B /C:"#define BLE_CFG_HCI_MODE_EN" %CONFIG_H%`) do @SET HCI_RESULT=%%i
@set HCI_SUPPORT=%HCI_RESULT:~1,-1%

@echo off

@rem ;; Set source library prefix
IF %HCI_SUPPORT%==1 (
    @set PS_S_LIB_PREFIX=lib_ble_ps_hci_
) ELSE (
    @set PS_S_LIB_PREFIX=lib_ble_ps_
)

@rem ;; Set source library type
IF %BLE_LIB_TYPE%==1 (
    @set PS_S_LIB_TYPE=b
) ELSE IF %BLE_LIB_TYPE%==2 (
    @set PS_S_LIB_TYPE=c
) ELSE (
    @set PS_S_LIB_TYPE=a
)

@rem ;; Rename library
copy %PS_S_LIB_PREFIX%ccrx_%PS_S_LIB_TYPE%.lib     lib_ble_ps_ccrx.lib

@echo on
