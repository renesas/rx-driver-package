/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_graphics_ais.c
 *
 ****************************************************************************/
#include "cri_aero_config_ais.h"
#include "cri_aero_graphics_internal.h"
#include "r_sys_time_rx_if.h"
#include "r_glcdc_rx_if.h"				
#include "r_glcdc_rx_pinset.h"
#include "cri_aero_support_atom_internal.h"
#include "r_gpio_rx_if.h" 


static CriGraphics _gGraphics;

static bool _gWaitVsynch = CRI_FALSE;
static bool _gEnableFlip = CRI_FALSE;

static CriUint32 _gVNumSwitch = 0;
static CriUint32 _gVCount     = 0;

static glcdc_cfg_t         gs_glcdc_init_cfg;                 
static volatile bool       gs_first_interrupt_flag;           
static glcdc_runtime_cfg_t gs_glcdc_run_cfg;
static glcdc_clut_cfg_t    gs_glcdc_clut_cfg;
static glcdc_status_t      gs_glcdc_status;


#if !defined(QE_DISPLAY_CONFIGURATION) && (GLCDC_CFG_CONFIGURATION_MODE == 0)



static const gamma_correction_t gs_gamma_table_r =
{
    { IMGC_GAMMA_R_GAIN_00, IMGC_GAMMA_R_GAIN_01, IMGC_GAMMA_R_GAIN_02, IMGC_GAMMA_R_GAIN_03,
      IMGC_GAMMA_R_GAIN_04, IMGC_GAMMA_R_GAIN_05, IMGC_GAMMA_R_GAIN_06, IMGC_GAMMA_R_GAIN_07,
      IMGC_GAMMA_R_GAIN_08, IMGC_GAMMA_R_GAIN_09, IMGC_GAMMA_R_GAIN_10, IMGC_GAMMA_R_GAIN_11,
      IMGC_GAMMA_R_GAIN_12, IMGC_GAMMA_R_GAIN_13, IMGC_GAMMA_R_GAIN_14, IMGC_GAMMA_R_GAIN_15 },

    { IMGC_GAMMA_R_TH_01, IMGC_GAMMA_R_TH_02, IMGC_GAMMA_R_TH_03, IMGC_GAMMA_R_TH_04,
      IMGC_GAMMA_R_TH_05, IMGC_GAMMA_R_TH_06, IMGC_GAMMA_R_TH_07, IMGC_GAMMA_R_TH_08,
      IMGC_GAMMA_R_TH_09, IMGC_GAMMA_R_TH_10, IMGC_GAMMA_R_TH_11, IMGC_GAMMA_R_TH_12,
      IMGC_GAMMA_R_TH_13, IMGC_GAMMA_R_TH_14, IMGC_GAMMA_R_TH_15 }
};

static const gamma_correction_t gs_gamma_table_g =
{
    { IMGC_GAMMA_G_GAIN_00, IMGC_GAMMA_G_GAIN_01, IMGC_GAMMA_G_GAIN_02, IMGC_GAMMA_G_GAIN_03,
      IMGC_GAMMA_G_GAIN_04, IMGC_GAMMA_G_GAIN_05, IMGC_GAMMA_G_GAIN_06, IMGC_GAMMA_G_GAIN_07,
      IMGC_GAMMA_G_GAIN_08, IMGC_GAMMA_G_GAIN_09, IMGC_GAMMA_G_GAIN_10, IMGC_GAMMA_G_GAIN_11,
      IMGC_GAMMA_G_GAIN_12, IMGC_GAMMA_G_GAIN_13, IMGC_GAMMA_G_GAIN_14, IMGC_GAMMA_G_GAIN_15 },

    { IMGC_GAMMA_G_TH_01, IMGC_GAMMA_G_TH_02, IMGC_GAMMA_G_TH_03, IMGC_GAMMA_G_TH_04,
      IMGC_GAMMA_G_TH_05, IMGC_GAMMA_G_TH_06, IMGC_GAMMA_G_TH_07, IMGC_GAMMA_G_TH_08,
      IMGC_GAMMA_G_TH_09, IMGC_GAMMA_G_TH_10, IMGC_GAMMA_G_TH_11, IMGC_GAMMA_G_TH_12,
      IMGC_GAMMA_G_TH_13, IMGC_GAMMA_G_TH_14, IMGC_GAMMA_G_TH_15 }
};

static const gamma_correction_t gs_gamma_table_b =
{
    { IMGC_GAMMA_B_GAIN_00, IMGC_GAMMA_B_GAIN_01, IMGC_GAMMA_B_GAIN_02, IMGC_GAMMA_B_GAIN_03,
      IMGC_GAMMA_B_GAIN_04, IMGC_GAMMA_B_GAIN_05, IMGC_GAMMA_B_GAIN_06, IMGC_GAMMA_B_GAIN_07,
      IMGC_GAMMA_B_GAIN_08, IMGC_GAMMA_B_GAIN_09, IMGC_GAMMA_B_GAIN_10, IMGC_GAMMA_B_GAIN_11,
      IMGC_GAMMA_B_GAIN_12, IMGC_GAMMA_B_GAIN_13, IMGC_GAMMA_B_GAIN_14, IMGC_GAMMA_B_GAIN_15 },

    { IMGC_GAMMA_B_TH_01, IMGC_GAMMA_B_TH_02, IMGC_GAMMA_B_TH_03, IMGC_GAMMA_B_TH_04,
      IMGC_GAMMA_B_TH_05, IMGC_GAMMA_B_TH_06, IMGC_GAMMA_B_TH_07, IMGC_GAMMA_B_TH_08,
      IMGC_GAMMA_B_TH_09, IMGC_GAMMA_B_TH_10, IMGC_GAMMA_B_TH_11, IMGC_GAMMA_B_TH_12,
      IMGC_GAMMA_B_TH_13, IMGC_GAMMA_B_TH_14, IMGC_GAMMA_B_TH_15 }
};
#endif


CriBool CRIAPI criAeroGraphics_Initialize_AIS( CriGraphics *pGraphics )
{
	if( pGraphics == NULL ){
		return CRI_FALSE;
	}

	_gGraphics = *pGraphics;

#if !defined(QE_DISPLAY_CONFIGURATION) && (GLCDC_CFG_CONFIGURATION_MODE == 0)


	gs_glcdc_init_cfg.output.htiming.front_porch = 39u;
	gs_glcdc_init_cfg.output.htiming.back_porch = 18u;
	gs_glcdc_init_cfg.output.htiming.display_cyc = CRI_AERO_CONFIG_AIS_LCD_WIDTH;
	gs_glcdc_init_cfg.output.htiming.sync_width = 41u;

	gs_glcdc_init_cfg.output.vtiming.front_porch = 2u;
	gs_glcdc_init_cfg.output.vtiming.back_porch = 2u;
	gs_glcdc_init_cfg.output.vtiming.display_cyc = CRI_AERO_CONFIG_AIS_LCD_HEIGHT;
	gs_glcdc_init_cfg.output.vtiming.sync_width = 10u;

	gs_glcdc_init_cfg.output.format = GLCDC_OUT_FORMAT_16BITS_RGB565;

	gs_glcdc_init_cfg.output.data_enable_polarity = GLCDC_SIGNAL_POLARITY_HIACTIVE;
	gs_glcdc_init_cfg.output.hsync_polarity = GLCDC_SIGNAL_POLARITY_LOACTIVE;
	gs_glcdc_init_cfg.output.vsync_polarity = GLCDC_SIGNAL_POLARITY_LOACTIVE;

	gs_glcdc_init_cfg.output.sync_edge = GLCDC_SIGNAL_SYNC_EDGE_RISING;

	gs_glcdc_init_cfg.output.tcon_hsync = GLCDC_TCON_PIN_2;
	gs_glcdc_init_cfg.output.tcon_vsync = GLCDC_TCON_PIN_0;
	gs_glcdc_init_cfg.output.tcon_de = GLCDC_TCON_PIN_3;

	gs_glcdc_init_cfg.output.correction_proc_order = GLCDC_BRIGHTNESS_CONTRAST_TO_GAMMA;

	gs_glcdc_init_cfg.output.brightness.enable = true;
	gs_glcdc_init_cfg.output.brightness.r = 512u;
	gs_glcdc_init_cfg.output.brightness.g = 512u;
	gs_glcdc_init_cfg.output.brightness.b = 512u;

	gs_glcdc_init_cfg.output.contrast.enable = true;
	gs_glcdc_init_cfg.output.contrast.r = 128u;
	gs_glcdc_init_cfg.output.contrast.g = 128u;
	gs_glcdc_init_cfg.output.contrast.b = 128u;

	gs_glcdc_init_cfg.output.gamma.enable = true;
	gs_glcdc_init_cfg.output.gamma.p_r = (gamma_correction_t*) &gs_gamma_table_r;
	gs_glcdc_init_cfg.output.gamma.p_g = (gamma_correction_t*) &gs_gamma_table_g;
	gs_glcdc_init_cfg.output.gamma.p_b = (gamma_correction_t*) &gs_gamma_table_b;

	gs_glcdc_init_cfg.output.dithering.dithering_on = true;
	gs_glcdc_init_cfg.output.dithering.dithering_mode = GLCDC_DITHERING_MODE_2X2PATTERN;
	gs_glcdc_init_cfg.output.dithering.dithering_pattern_a = GLCDC_DITHERING_PATTERN_11;
	gs_glcdc_init_cfg.output.dithering.dithering_pattern_b = GLCDC_DITHERING_PATTERN_00;
	gs_glcdc_init_cfg.output.dithering.dithering_pattern_c = GLCDC_DITHERING_PATTERN_10;
	gs_glcdc_init_cfg.output.dithering.dithering_pattern_d = GLCDC_DITHERING_PATTERN_01;


	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].p_base        = (uint32_t *)_gGraphics.pForegroundFrameBuffer;
	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].hsize         = _gGraphics.PanelWidth;
	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].vsize         = _gGraphics.PanelHeight;
	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].offset        = _gGraphics.PanelPitch;
	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].format        = GLCDC_IN_FORMAT_CLUT8;
	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].frame_edge    = false;
	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.x  = 0;	
	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.y  = 0;	
	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].bg_color.argb = 0x00CCCCCC;

	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].visible            = true;
	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].blend_control      = GLCDC_BLEND_CONTROL_NONE;

	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].frame_edge         = false;
	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].fixed_blend_value  = 0x00;
	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].fade_speed         = 0x00;
	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.x = 0;
	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.y = 0;
	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.x   = _gGraphics.PanelWidth;
	gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.y   = _gGraphics.PanelHeight;

	gs_glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].enable      = false;

	gs_glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].before.argb = 0x00FFFFFF;
	gs_glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].after.argb  = 0xFFFFFFFF;

	gs_glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].enable = true;

	gs_glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].p_base = _gGraphics.pPalette;
	gs_glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].start  = 0;
	gs_glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].size   = _gGraphics.PaletteNum;

	gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_1].p_base = (uint32_t *) NULL; 


    gs_glcdc_init_cfg.output.bg_color.argb = 0x00CCCCCC;

    gs_glcdc_init_cfg.output.endian = GLCDC_ENDIAN_LITTLE;

    gs_glcdc_init_cfg.output.color_order = GLCDC_COLOR_ORDER_RGB;  

    gs_glcdc_init_cfg.output.clksrc          = GLCDC_CLK_SRC_INTERNAL;
    gs_glcdc_init_cfg.output.clock_div_ratio = GLCDC_PANEL_CLK_DIVISOR_24;

    gs_glcdc_init_cfg.detection.vpos_detect  = true;
    gs_glcdc_init_cfg.detection.gr1uf_detect = false;
    gs_glcdc_init_cfg.detection.gr2uf_detect = false;

    gs_glcdc_init_cfg.interrupt.vpos_enable  = true;
    gs_glcdc_init_cfg.interrupt.gr1uf_enable = false;
    gs_glcdc_init_cfg.interrupt.gr2uf_enable = false;

	gs_glcdc_init_cfg.p_callback = (void (*) (void *))criAeroGraphics_VSyncCallback_AIS;

#endif

	gs_first_interrupt_flag = false;


	if( R_GLCDC_Open( &gs_glcdc_init_cfg) != GLCDC_SUCCESS ){
		return CRI_FALSE;
	}

	R_GLCDC_PinSet();


#if (CRI_AERO_PIN_USE_LCD_DISPLAY == 1)
	R_GPIO_PinDirectionSet(CRI_AERO_PIN_CONFIG_LCD_DISPLAY, GPIO_DIRECTION_OUTPUT);
	R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_LCD_DISPLAY, GPIO_LEVEL_HIGH);
#endif

#if (CRI_AERO_PIN_USE_LCD_BACK_LIGHT == 1)
	R_GPIO_PinDirectionSet(CRI_AERO_PIN_CONFIG_LCD_BACK_LIGHT, GPIO_DIRECTION_OUTPUT);
	R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_LCD_BACK_LIGHT, GPIO_LEVEL_HIGH);
#endif


	if( R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, NULL) != GLCDC_SUCCESS ){
		return CRI_FALSE;
	}


#if defined(QE_DISPLAY_CONFIGURATION)

	gs_glcdc_clut_cfg.enable = true;
	gs_glcdc_clut_cfg.p_base = _gGraphics.pPalette;
	gs_glcdc_clut_cfg.start  = 0;
	gs_glcdc_clut_cfg.size   = _gGraphics.PaletteNum;
	R_GLCDC_ClutUpdate( GLCDC_FRAME_LAYER_2, &gs_glcdc_clut_cfg );

#endif

	gs_glcdc_run_cfg.input	   = gs_glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2];
	gs_glcdc_run_cfg.blend	   = gs_glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2];
	gs_glcdc_run_cfg.chromakey = gs_glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2];

	return CRI_TRUE;
}

void CRIAPI criAeroGraphics_VSyncCallback_AIS( void *pData )
{
	CriAeroSupportAtom_EnableInterrupt_AIS();

	if (false == gs_first_interrupt_flag)
	{
		gs_first_interrupt_flag = true;

	}
	else
	{
		glcdc_callback_args_t * p_decode;
		p_decode = (glcdc_callback_args_t *) pData;

		if (GLCDC_EVENT_GR1_UNDERFLOW == p_decode->event)
		{
			nop(); 
		}
		else if (GLCDC_EVENT_GR2_UNDERFLOW == p_decode->event)
		{
			nop(); 
		}
		else 
		{
			_gVCount++;

			if( _gWaitVsynch == true ){
				if( _gVCount < _gVNumSwitch ){
					return;
				}
				_gVCount = 0;

				if( _gEnableFlip == true ){
					{
						gs_glcdc_clut_cfg.enable = true;
						gs_glcdc_clut_cfg.p_base = _gGraphics.pPalette;
						gs_glcdc_clut_cfg.start  = 0;
						gs_glcdc_clut_cfg.size   = _gGraphics.PaletteNum;
						R_GLCDC_ClutUpdate_NoReflect( GLCDC_FRAME_LAYER_2, &gs_glcdc_clut_cfg );


					}

					{
						if( gs_glcdc_run_cfg.input.p_base == (uint32_t *) _gGraphics.pForegroundFrameBuffer ){
							gs_glcdc_run_cfg.input.p_base = (uint32_t *) _gGraphics.pBackgroundFrameBuffer;
							CriAeroGraphics_SetForegroundFrameBuffer((void *)_gGraphics.pBackgroundFrameBuffer);
							CriAeroGraphics_SetBackgroundFrameBuffer((void *)_gGraphics.pForegroundFrameBuffer);
						} else {
							gs_glcdc_run_cfg.input.p_base = (uint32_t *) _gGraphics.pForegroundFrameBuffer;
							CriAeroGraphics_SetForegroundFrameBuffer((void *)_gGraphics.pForegroundFrameBuffer);
							CriAeroGraphics_SetBackgroundFrameBuffer((void *)_gGraphics.pBackgroundFrameBuffer);
						}

						R_GLCDC_LayerChange( GLCDC_FRAME_LAYER_2, &gs_glcdc_run_cfg );
					}

					_gEnableFlip = false;
				}

				do{
					R_GLCDC_GetStatus( &gs_glcdc_status );
				}while( gs_glcdc_status.fade_status[GLCDC_FRAME_LAYER_2] != GLCDC_FADE_STATUS_NOT_UNDERWAY );

				_gWaitVsynch = false;
			}
		}
	}

	return;
}

void CRIAPI criAeroGraphics_WaitVSync_AIS( CriBool Flip )
{
	_gWaitVsynch = CRI_TRUE;
	_gEnableFlip = Flip;
	while( _gWaitVsynch == CRI_TRUE ){
		CriAeroSupportAtom_Nop_AIS();
	}

	return;
}

void CRIAPI criAeroGraphics_SetNumVSynckSwitch_AIS( CriUint32 NumSwitch )
{
	_gVNumSwitch = NumSwitch;
	_gVCount     = 0;

	return;
}

