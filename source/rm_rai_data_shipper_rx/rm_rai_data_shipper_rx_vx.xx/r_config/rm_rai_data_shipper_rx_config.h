/* Generated configuration header file - do not edit */
/* generated configuration header file - do not edit */
#ifndef RM_RAI_DATA_SHIPPER_CFG_H_
#define RM_RAI_DATA_SHIPPER_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

/* Specify Used Device*/
#define RM_COMMS_UART                                                   (0)
#define RM_COMMS_USB_PCDC                                               (1)
#define RM_RAI_DATA_SHIPPER_CFG_DEVICE_USED                             RM_COMMS_UART

/* Common */
#define RM_RAI_DATA_SHIPPER_CFG_PARAM_CHECKING_ENABLE                   (BSP_CFG_PARAM_CHECKING_ENABLE)

/* Device */
#define RM_RAI_DATA_SHIPPER_CFG_FRAME_RATE_DIVIDER                      (0)
#define RM_RAI_DATA_SHIPPER_CFG_CALLBACK                                (rai_data_shipper0_callback)
#define RM_RAI_DATA_SHIPPER_CFG_DEVICE_COMMS_INSTANCE                   (g_comms_uart_device1)
#define RM_RAI_DATA_SHIPPER_CFG_CRC_ENABLE                              (1)
#define RM_RAI_DATA_SHIPPER_CFG_CRC_COMPONENT                           Config_CRC

#ifdef __cplusplus
}
#endif
#endif /* RM_RAI_DATA_SHIPPER_CFG_H_ */
