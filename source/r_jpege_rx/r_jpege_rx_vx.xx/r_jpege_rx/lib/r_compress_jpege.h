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
* Copyright (C) 2013-2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name	   : r_compress_jpege.h
* Description  : Header file of JPEG File Compress Library.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
***********************************************************************************************************************/
#include "r_stdint.h"
#include "r_mw_version.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define COMPRESS_JPEGE_OK				0
#define COMPRESS_JPEGE_ERROR_ARG		-1
#define COMPRESS_JPEGE_ERROR_WRITE		-2

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
enum r_jpege_in_format_t
{
	JPEGE_IN_RGB565,
	JPEGE_IN_RGB888,
	JPEGE_IN_YCC422,
};

enum r_jpege_out_format_t
{
	JPEGE_OUT_YCC422,
	JPEGE_OUT_YCC420,
};

struct r_jpeg_encode_t
{
	/* JPEG setting */
	uint16_t width;						/* original image's width */
	uint16_t height;					/* original image's height */
	int16_t quality;					/* image quality when encoding an image (default: 64) */
	int16_t restart_interval;			/* RST marker value; when don't use RST marker, set 0 */

	/* input */
	const uint8_t *input_addr;
	uint16_t input_line_byte;
	enum r_jpege_in_format_t input_format;

	/* output */
	uint8_t *outbuff;
	int32_t outbuff_size;
	enum r_jpege_out_format_t output_format;
};

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
extern const mw_version_t R_compress_jpege_version;

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
/* API */
int32_t R_compress_jpeg(struct r_jpeg_encode_t *);

/* user defined function */
uint8_t * R_jpeg_write_out_buffer(int32_t *rest);

