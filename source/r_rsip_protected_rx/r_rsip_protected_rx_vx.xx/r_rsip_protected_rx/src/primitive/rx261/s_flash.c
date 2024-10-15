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
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : s_flash.c
* Description  : Key information file.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release.
***********************************************************************************************************************/
 /**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "./r_rsip_primitive.h"
/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
RSIP_SEC_C_SECURE_BOOT
uint32_t const S_FLASH[] =
{
#if defined __LIT
	    0x0F528B59, 0x8AE8C44C, 0x63FBF156, 0x6F146062,
	    0xBDC29F82, 0x222AE4EA, 0x8770B75A, 0xA5D430D6,
	    0x7B38C7CE, 0x29F2A318, 0x93BA92AD, 0x1501F677,
	    0x9846839E, 0xFCB4A405, 0x009D2215, 0x2B94AF29,
	    0x4A10EEF4, 0xEBBAD02E, 0x24935214, 0x013A3C96,
	    0x0465C1AF, 0x28E393B2, 0x2298E128, 0xF255B1BE,
	    0x3C8E91C1, 0x8226BFEA, 0xD49EE419, 0xCAFB428E,
	    0x6E610814, 0x0A4DEFEB, 0x70B05955, 0x0CE21EF3,
	    0x96C1708A, 0x72A77539, 0x9B83F50C, 0x838FD765,
	    0x74261180, 0x6AE980CC, 0xC0A0C5B5, 0xE6DA5FB0,
	    0xEE001537, 0xC4B8DD40, 0x775010E5, 0xDA4EFDB7,
	    0xCFB4167B, 0x30A14915, 0x659C0518, 0xB020F787,
	    0x6201D8F0, 0x567F969F, 0xFDE28162, 0x1B5D7F3D,
	    0x5862F149, 0x2ECE9514, 0x4A034DDE, 0xD3AABD2F,
	    0xAD9631A1, 0x86152739, 0xD5F3D672, 0x707A53C5,
	    0x6FEF1FCB, 0x50F269A1, 0x45AFFD49, 0xF492D948,
	    0x4B3F2DCF, 0x8480FB4A, 0x4450A525, 0xF5F0F5E7,
	    0x25123ACB, 0xDB07B9C3, 0xD375506E, 0x6FFD059B,
	    0x95B846B4, 0x25B1DA22, 0x4DEC5340, 0xF2EC47E8,
	    0xF2136A22, 0x5951D0CE, 0xC23BE361, 0x6D322D90,
	    0xC9FA978C, 0x874AC23D, 0xCE77B128, 0xBB0D337F,
	    0x7792BD48, 0xA50EA23C, 0x2838494F, 0xDDB9B053,
	    0xA73E8791, 0x04A7309C, 0x53E01105, 0xA967905F,
	    0x8DCC6A74, 0xA6C1457F, 0xABF4C020, 0xC74C3460,
	    0x61E2D2F5, 0x7C5BD4EC, 0xE668B629, 0x7D687706,
	    0x0AF47A48, 0xA70FE7B7, 0xFED92F06, 0xBBF2EF4A,
	    0x2A9E7381, 0x8836ABEE, 0x5CCBCA6E, 0x2D39948F,
	    0x3579DEDD, 0x38DD468C, 0x31DBEFDD, 0xB78AC398,
	    0xD3F830E8, 0xCEB204D8, 0xE2995682, 0x5A975822,
	    0xE6B4DF47, 0x2C5895D7, 0xC0EDB4F1, 0xAF1B5505,
	    0x002B5B00, 0x111BE0C0, 0x2E1BCFBD, 0xA40D1D50,
	    0x007E4E09, 0x0B71EC2F, 0xBDCD929A, 0xBFBEBB35,
#else
		0x598b520f, 0x4cc4e88a, 0x56f1fb63, 0x6260146f,
		0x829fc2bd, 0xeae42a22, 0x5ab77087, 0xd630d4a5,
		0xcec7387b, 0x18a3f229, 0xad92ba93, 0x77f60115,
		0x9e834698, 0x05a4b4fc, 0x15229d00, 0x29af942b,
		0xf4ee104a, 0x2ed0baeb, 0x14529324, 0x963c3a01,
		0xafc16504, 0xb293e328, 0x28e19822, 0xbeb155f2,
		0xc1918e3c, 0xeabf2682, 0x19e49ed4, 0x8e42fbca,
		0x1408616e, 0xebef4d0a, 0x5559b070, 0xf31ee20c,
		0x8a70c196, 0x3975a772, 0x0cf5839b, 0x65d78f83,
		0x80112674, 0xcc80e96a, 0xb5c5a0c0, 0xb05fdae6,
		0x371500ee, 0x40ddb8c4, 0xe5105077, 0xb7fd4eda,
		0x7b16b4cf, 0x1549a130, 0x18059c65, 0x87f720b0,
		0xf0d80162, 0x9f967f56, 0x6281e2fd, 0x3d7f5d1b,
		0x49f16258, 0x1495ce2e, 0xde4d034a, 0x2fbdaad3,
		0xa13196ad, 0x39271586, 0x72d6f3d5, 0xc5537a70,
		0xcb1fef6f, 0xa169f250, 0x49fdaf45, 0x48d992f4,
		0xcf2d3f4b, 0x4afb8084, 0x25a55044, 0xe7f5f0f5,
		0xcb3a1225, 0xc3b907db, 0x6e5075d3, 0x9b05fd6f,
		0xb446b895, 0x22dab125, 0x4053ec4d, 0xe847ecf2,
		0x226a13f2, 0xced05159, 0x61e33bc2, 0x902d326d,
		0x8c97fac9, 0x3dc24a87, 0x28b177ce, 0x7f330dbb,
		0x48bd9277, 0x3ca20ea5, 0x4f493828, 0x53b0b9dd,
		0x91873ea7, 0x9c30a704, 0x0511e053, 0x5f9067a9,
		0x746acc8d, 0x7f45c1a6, 0x20c0f4ab, 0x60344cc7,
		0xf5d2e261, 0xecd45b7c, 0x29b668e6, 0x0677687d,
		0x487af40a, 0xb7e70fa7, 0x062fd9fe, 0x4aeff2bb,
		0x81739e2a, 0xeeab3688, 0x6ecacb5c, 0x8f94392d,
		0xddde7935, 0x8c46dd38, 0xddefdb31, 0x98c38ab7,
		0xe830f8d3, 0xd804b2ce, 0x825699e2, 0x2258975a,
		0x47dfb4e6, 0xd795582c, 0xf1b4edc0, 0x05551baf,
		0x005b2b00, 0xc0e01b11, 0xbdcf1b2e, 0x501d0da4,
		0x094e7e00, 0x2fec710b, 0x9a92cdbd, 0x35bbbebf,
#endif
};
RSIP_SEC_DEFAULT
