/**
 *  \file mesh_dataflash.h
 */

#ifndef _MESH_DATAFLASH_
#define _MESH_DATAFLASH_

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#define DATAFLASH_EN    (0)

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
#if DATAFLASH_EN
extern uint8_t mesh_dataflash_open(void);
extern void    mesh_dataflash_close(void);
extern uint8_t mesh_dataflash_read(uint32_t addr, uint8_t *buff, uint16_t len);
extern uint8_t mesh_dataflash_write(uint32_t addr, uint8_t *buff, uint16_t len);
extern uint8_t mesh_dataflash_erase_all(void);
#endif /* DATAFLASH_EN */

#endif /* _MESH_DATAFLASH_ */

