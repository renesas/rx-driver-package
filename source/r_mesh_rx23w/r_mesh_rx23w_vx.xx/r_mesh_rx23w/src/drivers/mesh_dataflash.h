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
* Copyright (C) 2019-2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

#ifndef _MESH_DATAFLASH_
#define _MESH_DATAFLASH_

/*******************************************************************************
* Compilation Switches
*******************************************************************************/
#define DATAFLASH_EN    (1)

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
#if DATAFLASH_EN
uint8_t mesh_dataflash_open(void);
void    mesh_dataflash_close(void);
uint8_t mesh_dataflash_read(uint32_t addr, uint8_t *buff, uint16_t len);
uint8_t mesh_dataflash_write(uint32_t addr, uint8_t *buff, uint16_t len);
uint8_t mesh_dataflash_erase_all(void);
#endif /* DATAFLASH_EN */

#endif /* _MESH_DATAFLASH_ */

