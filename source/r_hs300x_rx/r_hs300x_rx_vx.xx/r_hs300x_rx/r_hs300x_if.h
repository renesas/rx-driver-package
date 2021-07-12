/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef R_HS300X_IF_H_
#define R_HS300X_IF_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_hs300x.h"
#include "r_hs300x_rx_config.h"

#if (1 <= RM_HS300X_CFG_DEVICE_NUM_MAX)
extern const rm_hs300x_instance_t g_hs300x_sensor0;
#endif
#if (2 <= RM_HS300X_CFG_DEVICE_NUM_MAX)
extern const rm_hs300x_instance_t g_hs300x_sensor1;
#endif

#endif /* R_HS300X_IF_H_ */
