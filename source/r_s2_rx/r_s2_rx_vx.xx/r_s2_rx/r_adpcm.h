/*******************************************************************************
* File Name     : r_adpcm.h
* Version       : 3.04
* Device(s)     : -
* Tool-Chain    : -
* H/W Platform  : -
* Description   : ADPCM Encoder/Decoder header file
******************************************************************************/
/******************************************************************************
* History  : DD.MM.YYYY Version Description
*          : 11.08.2011 3.00    First Release
*          : 20.09.2012 3.01    Added R_s2_version
*          : 18.11.2015 3.04    Deleted NEAR FAR macro
******************************************************************************/
#ifndef __R_ADPCM_H__
#define __R_ADPCM_H__

#include "r_stdint.h"
#include "r_mw_version.h"

/******************************/
/*typedef for ADPCM  */
/******************************/
#define ADPCM_WORKSIZE_IN_UINT32 (5)
typedef struct
{
    uint32_t work[ADPCM_WORKSIZE_IN_UINT32]; /* working environment for ADPCM codec */
} adpcm_env;

typedef struct
{
    uint8_t cb[3];   /* cord buffer */
    uint8_t far *cp; /* pointer to ADPCM cord data */
    int16_t *ip;     /* pointer to input data */
    int16_t vp_e;    /* predicted value(Encode use) */
    int16_t id_e;    /* index(Encode use) */
    int16_t st_e;    /* Step size(Encode use) */
} adpcm_enc;

typedef struct
{
    uint8_t *input;  /* top address of ADPCM (input) */
    int16_t *output; /* top address of PCM (output) */
    uint16_t size;   /* number to decode */
    int8_t id;       /* unused */
    int16_t vp;      /* unused */
} adpcm_dec;


/*--------------------------------------------------------------------------*/
/* function prototype                                                       */
/*--------------------------------------------------------------------------*/
void R_adpcm_initEnc(adpcm_env *);
void R_adpcm_refreshEnc(int16_t *, uint8_t *, adpcm_env *);
int16_t R_adpcm_encode(int16_t, adpcm_env *);

void R_adpcm_initDec(adpcm_env *);
void R_adpcm_refreshDec(uint8_t *, int16_t *, adpcm_env *);
int16_t R_adpcm_decode(int16_t, adpcm_env *);

extern const mw_version_t R_s2_version;

#endif /* _R_ADPCM_H_ */
