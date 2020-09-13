/*------------------------------------------------------------------------*/
/* Sample Code of OS Dependent Functions for FatFs                        */
/* (C)ChaN, 2018                                                          */
/*------------------------------------------------------------------------*/

/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 20.12.2019 4.00     First Release of Renesas.
*                                    Added support for FreeRTOS and 
*                                    Renesas uITRON (RI600V4).
*                                    Added support for GNUC and ICCRX.
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

#if FF_USE_LFN == 3	/* Dynamic memory allocation */

/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/

void* ff_memalloc (	/* Returns pointer to the allocated memory block (null if not enough core) */
	UINT msize		/* Number of bytes to allocate */
)
{
	return malloc(msize);	/* Allocate a new memory block with POSIX API */
}


/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/

void ff_memfree (
	void* mblock	/* Pointer to the memory block to free (nothing to do if null) */
)
{
	free(mblock);	/* Free the memory block with POSIX API */
}

#endif

#if FF_FS_REENTRANT	/* Mutal exclusion */

/* Renesas modify */
#if BSP_CFG_RTOS_USED >= 1

/*------------------------------------------------------------------------*/
/* Create a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to create a new
/  synchronization object for the volume, such as semaphore and mutex.
/  When a 0 is returned, the f_mount() function fails with FR_INT_ERR.
*/

//const osMutexDef_t Mutex[FF_VOLUMES];	/* Table of CMSIS-RTOS mutex */

int ff_cre_syncobj (	/* 1:Function succeeded, 0:Could not create the sync object */
	BYTE vol,			/* Corresponding volume (logical drive number) */
	FF_SYNC_t* sobj		/* Pointer to return the created sync object */
)
{
	/* Win32 */
//	*sobj = CreateMutex(NULL, FALSE, NULL);
//	return (int)(*sobj != INVALID_HANDLE_VALUE);

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
    /* FreeRTOS */
    *sobj = xSemaphoreCreateMutex();
    return (int)(*sobj != NULL);
#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	OS_ERR err;
//	*sobj = OSMutexCreate(0, &err);
//	return (int)(err == OS_NO_ERR);
#elif BSP_CFG_RTOS_USED == 4          /* Renesas ITRON OS (RI600V4 or RI600PX) */
#if BSP_CFG_RENESAS_RTOS_USED == 0    /* RI600V4 */
/* When the RI600V4, the method of creating a mutex is limited to “static creation”.
   Create semphores by using static API “mutex[]” in the system configuration file. */
    *sobj = MUTEX_ID_TABLE[vol];
    return (int)(*sobj > 0);
#else                                 /* RI600PX */
    T_CMTX cmtx = {TA_CEILING, RI600PX_LOCKED_MUTEX_TASK_PRIORITY};
    *sobj = acre_mtx(&cmtx);
    return (int)(*sobj > 0);
#endif
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	*sobj = osMutexCreate(&Mutex[vol]);
//	return (int)(*sobj != NULL);
#endif
}

/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to delete a synchronization
/  object that created with ff_cre_syncobj() function. When a 0 is returned,
/  the f_mount() function fails with FR_INT_ERR.
*/

int ff_del_syncobj (	/* 1:Function succeeded, 0:Could not delete due to an error */
	FF_SYNC_t sobj		/* Sync object tied to the logical drive to be deleted */
)
{
	/* Win32 */
//	return (int)CloseHandle(sobj);

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
    vSemaphoreDelete(sobj);
    return 1;
#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	OS_ERR err;
//	OSMutexDel(sobj, OS_DEL_ALWAYS, &err);
//	return (int)(err == OS_NO_ERR);
#elif BSP_CFG_RTOS_USED == 4          /* Renesas ITRON OS (RI600V4 or RI600PX) */
#if BSP_CFG_RENESAS_RTOS_USED == 0    /* RI600V4 */
/* RI600V4 can not delete mutexes. Therfore, force unl_mtx(). */
    unl_mtx(sobj);
    return (int)(1);
#else                                 /* RI600PX */
    return (int)(del_mtx(sobj) == E_OK);
#endif
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	return (int)(osMutexDelete(sobj) == osOK);
#endif 
}

/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a 0 is returned, the file function fails with FR_TIMEOUT.
*/

int ff_req_grant (	/* 1:Got a grant to access the volume, 0:Could not get a grant */
	FF_SYNC_t sobj	/* Sync object to wait */
)
{
	/* Win32 */
//	return (int)(WaitForSingleObject(sobj, FF_FS_TIMEOUT) == WAIT_OBJECT_0);

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
    return (int)(xSemaphoreTake(sobj, pdMS_TO_TICKS(FF_FS_TIMEOUT)) == pdTRUE);
#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	OS_ERR err;
//	OSMutexPend(sobj, FF_FS_TIMEOUT, &err));
//	return (int)(err == OS_NO_ERR);
#elif BSP_CFG_RTOS_USED == 4             /* Renesas ITRON OS (RI600V4 or RI600PX) */
    return (int)(tloc_mtx(sobj, (TMO)FF_FS_TIMEOUT) == E_OK);
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	return (int)(osMutexWait(sobj, FF_FS_TIMEOUT) == osOK);
#endif
}

/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
	FF_SYNC_t sobj	/* Sync object to be signaled */
)
{
	/* Win32 */
//	ReleaseMutex(sobj);

#if BSP_CFG_RTOS_USED == 1               /* FreeRTOS */
    xSemaphoreGive(sobj);
#elif BSP_CFG_RTOS_USED == 2             /* embOS */
#error "Not Support: SEGGER embOS with TFAT"
#elif BSP_CFG_RTOS_USED == 3             /* MicroC_OS */
#error "Not Support: Micrium MicroC/OS with TFAT"
	/* uC/OS-II */
//	OSMutexPost(sobj);
#elif BSP_CFG_RTOS_USED == 4             /* Renesas ITRON OS (RI600V4 or RI600PX) */
    unl_mtx(sobj);
#else                                    /* Other RTOS */
#error "Not Support: Other RTOS with TFAT"
	/* CMSIS-RTOS */
//	osMutexRelease(sobj);
#endif
}

#endif    /* End of BSP_CFG_RTOS_USED >= 1 */
/* End of Renesas modify */

#endif

