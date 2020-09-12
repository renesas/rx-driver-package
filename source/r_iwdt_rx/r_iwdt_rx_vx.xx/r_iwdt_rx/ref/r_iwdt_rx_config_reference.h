/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_iwdt_rx_config.h
* Description   : Configures the IWDT drivers
************************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*           07.06.2013 1.00    First Release     
*           06.09.2013 1.10    Move internal macro checking to private header file
*           19.03.2014 1.20    Removed equates relative to OFS0 settings.
*           21.07.2017 1.80    Update the 'Copyright' year  
***********************************************************************************************************************/
#ifndef IWDT_RX_CONFIG_H
#define IWDT_RX_CONFIG_H

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING 
   Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting (default)
   Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define IWDT_CFG_PARAM_CHECKING_ENABLE  (BSP_CFG_PARAM_CHECKING_ENABLE)


/* NOTE:
 * To have the IWDT start automatically at powerup instead of using an Open() call,
 * change the BSP_CFG_OFS0_REG_VALUE mask to the desired settings in r_bsp_config.h.
 */


#endif /* IWDT_RX_CONFIG_H */
