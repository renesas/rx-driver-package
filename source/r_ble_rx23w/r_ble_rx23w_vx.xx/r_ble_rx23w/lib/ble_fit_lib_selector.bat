
cd %~dp0

@set CONFIG_H=..\..\r_config\r_ble_rx23w_config.h
@REM set CONFIG_H=..\..\rble\include\r_ble_rx23w_config.h
@REM @set CONFIG_H=..\..\..\..\rble\include\r_ble_rx23w_config.h

@for /f "tokens=3 usebackq" %%i in (`findstr "BLE_CFG_LIB_TYPE" %CONFIG_H%`) do @SET RESULT=%%i
@set BLE_LIB_TYPE=%RESULT:~1,-1%

@IF %BLE_LIB_TYPE%==1 (
@   copy lib_ble_ps_ccrx_b.lib         lib_ble_ps_ccrx.lib
@   copy lib_ble_ps_hci_ccrx_b.lib     lib_ble_ps_hci_ccrx.lib
) ELSE IF %BLE_LIB_TYPE%==2 (
@   copy lib_ble_ps_ccrx_c.lib         lib_ble_ps_ccrx.lib
@   copy lib_ble_ps_hci_ccrx_c.lib     lib_ble_ps_hci_ccrx.lib
) ELSE (
@   copy lib_ble_ps_ccrx_a.lib         lib_ble_ps_ccrx.lib
@   copy lib_ble_ps_hci_ccrx_a.lib     lib_ble_ps_hci_ccrx.lib
)

