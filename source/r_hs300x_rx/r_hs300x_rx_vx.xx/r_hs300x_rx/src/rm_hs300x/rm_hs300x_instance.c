/* ${REA_DISCLAIMER_PLACEHOLDER} */
/******************************************************************************************************************//**
 * @file          rm_hs3001_instance.c
 * Version      : 1.0
 * @brief         3.5a. Each file will maintain one of the file headers shown below.
 *                   i. If the file contains the main() function then it will have the expanded header shown in the
 *                      first example.
 *                  ii. If the file does not contain the main() function then it is allowed to have the smaller header
 *                      shown in the second example.
 *                   b. Doxygen format headers (@brief, ... . ) may be used for API and CFG files to enable simple
 *                      documentation of the API and configuration for customers. Refer to section 4.4 & 1.7.
 *                   c. Where files are not dependent upon any entry (such as a specific HW Platform), the entry
 *                   should be omitted, or specify the widest possible description of the entry.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.01.2018 1.00     First Release
 *         : 27.03.2019 6.00.00  CStd V6.0. Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
 #include "r_comms_i2c_if.h"
 #include "r_hs300x_if.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if (1 <= RM_HS300X_CFG_DEVICE_NUM_MAX)
void rm_hs300x_callback0(rm_comms_callback_args_t * p_args);
 #if (1 == RM_HS300X_CFG_DEVICE0_CALLBACK_ENABLE)
 void RM_HS300X_CFG_DEVICE0_CALLBACK(rm_hs300x_callback_args_t * p_args);
 #endif
#endif
#if (2 <= RM_HS300X_CFG_DEVICE_NUM_MAX)
void rm_hs300x_callback1(rm_comms_callback_args_t * p_args);
 #if (1 == RM_HS300X_CFG_DEVICE1_CALLBACK_ENABLE)
 void RM_HS300X_CFG_DEVICE1_CALLBACK(rm_hs300x_callback_args_t * p_args);
 #endif
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
extern void rm_hs300x_callback (rm_comms_callback_args_t * p_args);
#if (1 <= RM_HS300X_CFG_DEVICE_NUM_MAX)
rm_hs300x_instance_ctrl_t g_hs300x_sensor0_ctrl;
const rm_hs300x_cfg_t g_hs300x_sensor0_cfg =
{
    .p_instance = &(RM_HS300X_CFG_DEVICE0_COMMS_INSTANCE),
    .p_callback = (void (*)(rm_hs300x_callback_args_t *))RM_HS300X_CFG_DEVICE0_CALLBACK,
    .p_context  = NULL,
};
const rm_hs300x_instance_t g_hs300x_sensor0 =
{
    .p_ctrl = &g_hs300x_sensor0_ctrl,
    .p_cfg  = &g_hs300x_sensor0_cfg,
    .p_api  = &g_hs300x_on_hs300x,
};
void rm_hs300x_callback0(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_hs300x_sensor0_ctrl;
    rm_hs300x_callback(p_args);
}
#endif
#if (2 <= RM_HS300X_CFG_DEVICE_NUM_MAX)
const rm_hs300x_cfg_t g_hs300x_sensor1_cfg =
{
	.p_instance = &(RM_HS300X_CFG_DEVICE1_COMMS_INSTANCE),
    .p_callback = (void (*)(rm_hs300x_callback_args_t *))RM_HS300X_CFG_DEVICE1_CALLBACK,
    .p_context  = NULL,
};
rm_hs300x_instance_ctrl_t g_hs300x_sensor1_ctrl;
const rm_hs300x_instance_t g_hs300x_sensor1 =
{
    .p_ctrl = &g_hs300x_sensor1_ctrl,
    .p_cfg  = &g_hs300x_sensor1_cfg,
    .p_api  = &g_hs300x_on_hs300x,
};
void rm_hs300x_callback1(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_hs300x_sensor1_ctrl;
    rm_hs300x_callback(p_args);
}
#endif
/**********************************************************************************************************************
 * End of function rm_hs3001_callback1
 *********************************************************************************************************************/


