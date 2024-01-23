/*------------------------------------------------------------------------*/
/* A Sample Code of User Provided OS Dependent Functions for FatFs        */
/*------------------------------------------------------------------------*/

/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 20.12.2019 4.00     First Release of Renesas.
*                                    Added support for FreeRTOS and 
*                                    Renesas uITRON (RI600V4).
*                                    Added support for GNUC and ICCRX.
*              : 31.08.2023 4.10     Updated FatFs ff15.
*******************************************************************************/

#include "ff.h"

/* Renesas modify */
#include "r_tfat_driver_rx_config.h"
#if FF_USE_LFN == 3
#include <stdlib.h>
#endif

#if BSP_CFG_RTOS_USED == 4 && BSP_CFG_RENESAS_RTOS_USED == 0    /* RI600V4 */
#if TFAT_DRIVE_ALLOC_NUM_0 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_0 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_1 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_1 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_2 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_2 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_3 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_3 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_4 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_4 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_5 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_5 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_6 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_6 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_7 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 || \
    RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_7 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_8 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8
#if RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 == RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 cannot be duplicated with Mutex IDs for other drives."
#endif    /* End of RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 == ... */
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_8 != TFAT_CTRL_NONE */

#if TFAT_DRIVE_ALLOC_NUM_9 != TFAT_CTRL_NONE
#if 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9 && 255 >= RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
#else
#error "RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9 is invalid."
#endif    /* End of 0 < RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9 <= 255 */
#endif    /* End of TFAT_DRIVE_ALLOC_NUM_9 != TFAT_CTRL_NONE */

static ID MUTEX_ID_TABLE[10] = {
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8, 
                                RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9
};
#endif    /* End of BSP_CFG_RTOS_USED == 4 && BSP_CFG_RENESAS_RTOS_USED == 0 */

#if BSP_CFG_RTOS_USED == 4 && BSP_CFG_RENESAS_RTOS_USED == 1    /* RI600PX */
#if 1 > RI600PX_LOCKED_MUTEX_TASK_PRIORITY || TMAX_TPRI < RI600PX_LOCKED_MUTEX_TASK_PRIORITY
#error "RI600PX_LOCKED_MUTEX_TASK_PRIORITY must be in the range 1 ~ TMAX_TPRI"
#endif
#endif    /* End of BSP_CFG_RTOS_USED == 4 && BSP_CFG_RENESAS_RTOS_USED == 1 */
/* End of Renesas modify */

#if FF_USE_LFN == 3	/* Use dynamic memory allocation */

/*------------------------------------------------------------------------*/
/* Allocate/Free a Memory Block                                           */
/*------------------------------------------------------------------------*/

void* ff_memalloc (	/* Returns pointer to the allocated memory block (null if not enough core) */
	UINT msize		/* Number of bytes to allocate */
)
{
	return malloc((size_t)msize);	/* Allocate a new memory block */
}


void ff_memfree (
	void* mblock	/* Pointer to the memory block to free (no effect if null) */
)
{
	free(mblock);	/* Free the memory block */
}

#endif




#if FF_FS_REENTRANT	/* Mutal exclusion */

/* Renesas modify */
#if BSP_CFG_RTOS_USED >= 1
/*------------------------------------------------------------------------*/
/* Definitions of Mutex                                                   */
/*------------------------------------------------------------------------*/

	/* Win32 */
//	#include <windows.h>
//	static HANDLE Mutex[FF_VOLUMES + 1];	/* Table of mutex handle */

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
#include "FreeRTOS.h"
#include "semphr.h"
static SemaphoreHandle_t Mutex[FF_VOLUMES + 1];	/* Table of mutex handle */

#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	#include "includes.h"
//	static OS_EVENT *Mutex[FF_VOLUMES + 1];	/* Table of mutex pinter */
#elif BSP_CFG_RTOS_USED == 4          /* Renesas ITRON OS (RI600PX) */
#if BSP_CFG_RENESAS_RTOS_USED == 1    /* RI600PX */
    ER_ID MUTEX_ID_TABLE[FF_VOLUMES + 1]
#endif
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	osMutexDef(cmsis_os_mutex);

//	Mutex[vol] = osMutexCreate(osMutex(cmsis_os_mutex));
//	return (int)(Mutex[vol] != NULL);
#endif

/*------------------------------------------------------------------------*/
/* Create a Mutex                                                         */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to create a new mutex
/  or semaphore for the volume. When a 0 is returned, the f_mount function
/  fails with FR_INT_ERR.
*/

int ff_mutex_create (	/* Returns 1:Function succeeded or 0:Could not create the mutex */
	int vol				/* Mutex ID: Volume mutex (0 to FF_VOLUMES - 1) or system mutex (FF_VOLUMES) */
)
{
	/* Win32 */
	//Mutex[vol] = CreateMutex(NULL, FALSE, NULL);
	//return (int)(Mutex[vol] != INVALID_HANDLE_VALUE);

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
    /* FreeRTOS */
    Mutex[vol] = xSemaphoreCreateMutex();
    return (int)(Mutex[vol] != NULL);
#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	OS_ERR err;

//	Mutex[vol] = OSMutexCreate(0, &err);
//	return (int)(err == OS_NO_ERR);
#elif BSP_CFG_RTOS_USED == 4          /* Renesas ITRON OS (RI600V4 or RI600PX) */
#if BSP_CFG_RENESAS_RTOS_USED == 0    /* RI600V4 */
/* When the RI600V4, the method of creating a mutex is limited to “static creation”.
   Create semphores by using static API “mutex[]” in the system configuration file. */
    return (int)(MUTEX_ID_TABLE[vol] > 0);
#else                                 /* RI600PX */
    T_CMTX cmtx = {TA_CEILING, RI600PX_LOCKED_MUTEX_TASK_PRIORITY};
    MUTEX_ID_TABLE[vol] = acre_mtx(&cmtx);
    return (int)(MUTEX_ID_TABLE[vol] > 0);
#endif
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	osMutexDef(cmsis_os_mutex);

//	Mutex[vol] = osMutexCreate(osMutex(cmsis_os_mutex));
//	return (int)(Mutex[vol] != NULL);

#endif
}

/*------------------------------------------------------------------------*/
/* Delete a Mutex                                                         */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to delete a mutex or
/  semaphore of the volume created with ff_mutex_create function.
*/

void ff_mutex_delete (	/* Returns 1:Function succeeded or 0:Could not delete due to an error */
	int vol				/* Mutex ID: Volume mutex (0 to FF_VOLUMES - 1) or system mutex (FF_VOLUMES) */
)
{
	/* Win32 */
//	CloseHandle(Mutex[vol]);

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
    vSemaphoreDelete(Mutex[vol]);
#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	OS_ERR err;

//	OSMutexDel(Mutex[vol], OS_DEL_ALWAYS, &err);

#elif BSP_CFG_RTOS_USED == 4          /* Renesas ITRON OS (RI600V4 or RI600PX) */
#if BSP_CFG_RENESAS_RTOS_USED == 0    /* RI600V4 */
/* RI600V4 can not delete mutexes. Therfore, force unl_mtx(). */
    unl_mtx(MUTEX_ID_TABLE[vol]);
#else                                 /* RI600PX */
    del_mtx(MUTEX_ID_TABLE[vol]);
#endif
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	osMutexDelete(Mutex[vol]);
#endif 
}

/*------------------------------------------------------------------------*/
/* Request a Grant to Access the Volume                                   */
/*------------------------------------------------------------------------*/
/* This function is called on enter file functions to lock the volume.
/  When a 0 is returned, the file function fails with FR_TIMEOUT.
*/

int ff_mutex_take (	/* Returns 1:Succeeded or 0:Timeout */
	int vol			/* Mutex ID: Volume mutex (0 to FF_VOLUMES - 1) or system mutex (FF_VOLUMES) */
)
{
	/* Win32 */
//	return (int)(WaitForSingleObject(Mutex[vol], FF_FS_TIMEOUT) == WAIT_OBJECT_0);

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
    return (int)(xSemaphoreTake(Mutex[vol], pdMS_TO_TICKS(FF_FS_TIMEOUT)) == pdTRUE);
#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	OS_ERR err;

//	OSMutexPend(Mutex[vol], FF_FS_TIMEOUT, &err));
//	return (int)(err == OS_NO_ERR);
#elif BSP_CFG_RTOS_USED == 4             /* Renesas ITRON OS (RI600V4 or RI600PX) */
    return (int)(tloc_mtx(MUTEX_ID_TABLE[vol], (TMO)FF_FS_TIMEOUT) == E_OK);
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	return (int)(osMutexWait(Mutex[vol], FF_FS_TIMEOUT) == osOK);
#endif
}


/*------------------------------------------------------------------------*/
/* Release a Grant to Access the Volume                                   */
/*------------------------------------------------------------------------*/
/* This function is called on leave file functions to unlock the volume.
*/

void ff_mutex_give (
	int vol			/* Mutex ID: Volume mutex (0 to FF_VOLUMES - 1) or system mutex (FF_VOLUMES) */
)
{
	/* Win32 */
//	ReleaseMutex(Mutex[vol]);

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
    xSemaphoreGive(Mutex[vol]);
#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	OSMutexPost(Mutex[vol]);
#elif BSP_CFG_RTOS_USED == 4             /* Renesas ITRON OS (RI600V4 or RI600PX) */
    unl_mtx(MUTEX_ID_TABLE[vol]);
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	osMutexRelease(Mutex[vol]);
#endif
}

#endif    /* End of BSP_CFG_RTOS_USED >= 1 */
/* End of Renesas modify */

#endif	/* FF_FS_REENTRANT */

