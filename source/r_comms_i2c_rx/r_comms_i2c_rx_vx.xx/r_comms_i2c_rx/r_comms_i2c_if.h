/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef R_COMMS_I2C_IF_H_
#define R_COMMS_I2C_IF_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_comms_i2c.h"

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#if COMMS_I2C_CFG_DRIVER_I2C
#include "r_riic_rx_if.h"
#endif
#if COMMS_I2C_CFG_DRIVER_SCI_I2C
#include "r_sci_iic_rx_if.h"
#endif
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#include "inc/instances/rm_comms_i2c.h"
#include "r_cg_serial.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/* I2C device specific configuration */
typedef struct st_rm_comms_i2c_device_cfg
{
    uint32_t const slave_address;      ///< Slave address of I2C device
    uint32_t const slave_address_bits; ///< Indicates how slave fields should be interpreted
} rm_comms_i2c_device_cfg_t;

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
/* RX Driver Instance */
typedef struct st_i2c_driver_instance
{
    uint32_t const     driver_type;
    uint32_t const     driver_channel;
    void const       * p_info;
    void (* callback)(void);
} i2c_driver_instance_t;

#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
/* RL78 Driver Instance */
typedef struct st_i2c_driver_instance
{
    uint32_t   const    driver_type;
    uint32_t * const    p_slave_address;
    void       const *  p_api;
} i2c_driver_instance_t;
/* IICA APIs */
typedef struct st_iica_api
{
    void (* open)(void);                                                                     /**< Open */
    MD_STATUS (* read)(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait);  /**< Read */
    MD_STATUS (* write)(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait); /**< Write */
    void (* stopCondition)(void);                                                            /**< Stop Condition */
    void (* close)(void);                                                                    /**< Close */
} iica_api_t;

/* SAU IIC APIs */
typedef struct st_sau_api
{
    void (* open)(void);                                                  /**< Open */
    void (* read)(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num);  /**< Read */
    void (* write)(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num); /**< Write */
    void (* startCondition)(void);                                        /**< Start Condition */
    void (* stopCondition)(void);                                         /**< Stop Condition */
    void (* close)(void);                                                 /**< Close */
} sau_api_t;
#endif

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
#if (1 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device0;
#endif
#if (2 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device1;
#endif
#if (3 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device2;
#endif
#if (4 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device3;
#endif
#if (5 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device4;
#endif

#if defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#if (1 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern void rm_comms_i2c_bus0_callback(bool aborted);
#endif
#if (2 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern void rm_comms_i2c_bus1_callback(bool aborted);
#endif
#if (3 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern void rm_comms_i2c_bus2_callback(bool aborted);
#endif
#if (4 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern void rm_comms_i2c_bus3_callback(bool aborted);
#endif
#if (5 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern void rm_comms_i2c_bus4_callback(bool aborted);
#endif
#endif

#endif /* R_COMMS_I2C_IF_H_ */
