
/**
 *  \file blebrr.c
 *
 *  This File contains the BLE Bearer interface for the
 *  Mindtree Mesh stack.
 */

/*
 *  Copyright (C) 2016-2022. Mindtree Ltd.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "blebrr.h"
#include "timer/r_ble_timer.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#define BLEBRR_QUEUE_SIZE                   64
#define BLEBRR_ADV_TIMEOUT                  10
#define BLEBRR_ADV_ABORT_TIMEOUT            100
#define BLEBRR_NCON_ADVTYPE_OFFSET          2
#define BLEBRR_ADVREPEAT_RAND_DELAY         10

/** Bearer state defines */
#define BLEBRR_STATE_IDLE                   0x00
#define BLEBRR_STATE_IN_SCAN_ENABLE         0x01
#define BLEBRR_STATE_IN_SCAN_DISABLE        0x02
#define BLEBRR_STATE_SCAN_ENABLED           0x04
#define BLEBRR_STATE_IN_ADV_ENABLE          0x10
#define BLEBRR_STATE_IN_ADV_DISABLE         0x20
#define BLEBRR_STATE_ADV_ENABLED            0x40

#define BLEBRR_STATE_MASK_SCAN              0x0F
#define BLEBRR_STATE_MASK_ADV               0xF0

/** GATT Mode GAP Connectable Advertising Service data offset */
#define BLEBRR_GATT_ADV_SERV_DATALEN_OFFSET 7
#define BLEBRR_GATT_ADV_SERV_DATA_OFFSET    (BLEBRR_GATT_ADV_SERV_DATALEN_OFFSET + 4)

/** Advertising data maximum length */
#define BLEBRR_GAP_ADVDATA_LEN              31

/** Advertising data sets MAX */
#define BLEBRR_GAP_MAX_ADVDATA_SETS         2

#define BLEBRR_MUTEX_INIT()                 MS_MUTEX_INIT(blebrr_mutex, BRR);
#define BLEBRR_MUTEX_INIT_VOID()            MS_MUTEX_INIT_VOID(blebrr_mutex, BRR);
#define BLEBRR_LOCK()                       MS_MUTEX_LOCK(blebrr_mutex, BRR);
#define BLEBRR_LOCK_VOID()                  MS_MUTEX_LOCK_VOID(blebrr_mutex, BRR);
#define BLEBRR_UNLOCK()                     MS_MUTEX_UNLOCK(blebrr_mutex, BRR);
#define BLEBRR_UNLOCK_VOID()                MS_MUTEX_UNLOCK_VOID(blebrr_mutex, BRR);

#define BLEBRR_SET_STATE_SCAN(x)            blebrr_state = (x) | (blebrr_state & (~BLEBRR_STATE_MASK_SCAN))
#define BLEBRR_SET_STATE_ADV(x)             blebrr_state = (x) | (blebrr_state & (~BLEBRR_STATE_MASK_ADV))
#define BLEBRR_GET_STATE_SCAN()             (blebrr_state & BLEBRR_STATE_MASK_SCAN)
#define BLEBRR_GET_STATE_ADV()              (blebrr_state & BLEBRR_STATE_MASK_ADV)

/*******************************************************************************
* Type definitions
*******************************************************************************/
/** BLEBRR Data Queue Element */
typedef struct _BLEBRR_Q_ELEMENT
{
    /* "Allocated" Data Pointer */
    UCHAR *pdata;

    /*
     *  Data Length. If data length is zero, the element is considered
     *  invalid.
     */
    UINT16 pdatalen;

    /* Type of data element */
    UCHAR type;

} BLEBRR_Q_ELEMENT;

/** BLEBRR Data Queue */
typedef struct _BLEBRR_Q
{
    /* List of Bearer Queue elements */
    BLEBRR_Q_ELEMENT element[BLEBRR_QUEUE_SIZE];

    /* Queue start index */
    UINT16 start;

    /* Queue end index */
    UINT16 end;

} BLEBRR_Q;

/** Advertising Data type */
typedef struct _BLEBRR_GAP_ADV_DATA
{
    /** Data */
    UCHAR data[BLEBRR_GAP_ADVDATA_LEN];

    /** Data Length */
    UCHAR datalen;

} BLEBRR_GAP_ADV_DATA;

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
static BRR_BEARER_INFO blebrr_adv;
static BRR_HANDLE blebrr_advhandle = BRR_HANDLE_INVALID;

static UCHAR blebrr_sleep;
static UCHAR blebrr_adv_disabled;

static BLEBRR_Q blebrr_queue;

MS_DEFINE_MUTEX_TYPE(static, blebrr_mutex)
static UINT32 blebrr_timer_handle = BLE_TIMER_INVALID_HDL;
static UCHAR blebrr_state;
static UCHAR blebrr_datacount = 0;
static UCHAR blebrr_memcount = 0;

BLEBRR_GAP_ADV_DATA blebrr_gap_adv_data[BLEBRR_GAP_MAX_ADVDATA_SETS] =
{
    /* Index 0x00: Mesh Provisioning Service ADV Data */
    {
        {
            /**
             *  Flags:
             *      0x01: LE Limited Discoverable Mode
             *      0x02: LE General Discoverable Mode
             *      0x04: BR/EDR Not Supported
             *      0x08: Simultaneous LE and BR/EDR to Same Device Capable (Controller)
             *      0x10: Simultaneous LE and BR/EDR to Same Device Capable (Host)
             */
            0x02, 0x01, 0x06,

            /**
             *  Service UUID List:
             *      Mesh Provisioning Service (0x1827)
             */
            0x03, 0x03, 0x27, 0x18,

            /**
             *  Service Data List:
             *      Mesh Provisioning Service (0x1827)
             *      Mesh UUID (16 Bytes)
             *      Mesh OOB Info (2 Bytes)
             *  NOTE: UUID and OOB Info are set by the blebrr_bcon_send().
             */
             0x15, 0x16,
             0x27, 0x18,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x00
        },

        /** Advertising Data length */
        29
    },
    /* Index 0x01: Mesh Proxy Service ADV Data */
    {
        {
            /**
             *  Flags:
             *      0x01: LE Limited Discoverable Mode
             *      0x02: LE General Discoverable Mode
             *      0x04: BR/EDR Not Supported
             *      0x08: Simultaneous LE and BR/EDR to Same Device Capable (Controller)
             *      0x10: Simultaneous LE and BR/EDR to Same Device Capable (Host)
             */
            0x02, 0x01, 0x06,

            /**
             *  Service UUID List:
             *      Mesh Proxy Service (0x1828)
             */
            0x03, 0x03, 0x28, 0x18,

            /**
             *  Service Data List:
             *      Mesh Proxy Service (0x1828)
             *      Type (1 Byte) 
             *          0x00: Network ID
             *          0x01: Node Identity
             *      Network ID (8 Bytes), if Type is 0x00
             *      Hash (8 Bytes) and Random (8 Bytes), if Type is 0x01
             *  NOTE: AD Length is calculated and set by the blebrr_bcon_send().
             *  NOTE: Type, Network ID, Hash and Random are set by the blebrr_bcon_send().
             *  NOTE: The BLEBRR_GATT_ADV_SERV_DATALEN_OFFSET macro specifies offset to this Service Data List AD Structure.
             *  NOTE: The BLEBRR_GATT_ADV_SERV_DATA_OFFSET macro specifies offset to the Service Data.
             */
             0x00, 0x16,
             0x28, 0x18,
             0x00,
             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        },

        /** Advertising Data length */
        20
    }
};

static void blebrr_timer_start(UINT32 timeout);

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Enable ADV Bearer
*******************************************************************************/
void blebrr_adv_enable(void)
{
    BLEBRR_LOCK_VOID();

    if ((BLEBRR_STATE_IDLE == BLEBRR_GET_STATE_SCAN()) &&
        (MS_TRUE != blebrr_sleep))
    {
        blebrr_scan_pl(MS_TRUE);

        /* Update state */
        BLEBRR_SET_STATE_SCAN(BLEBRR_STATE_IN_SCAN_ENABLE);
    }

    blebrr_adv_disabled = MS_FALSE;

    BLEBRR_UNLOCK_VOID();
}

/***************************************************************************//**
* @brief Disable ADV Bearer
*******************************************************************************/
void blebrr_adv_disable(void)
{
    BLEBRR_LOCK_VOID();

    if (BLEBRR_STATE_SCAN_ENABLED == BLEBRR_GET_STATE_SCAN())
    {
        blebrr_scan_pl(MS_FALSE);

        /* Update state */
        BLEBRR_SET_STATE_SCAN(BLEBRR_STATE_IN_SCAN_DISABLE);
    }

    blebrr_adv_disabled = MS_TRUE;

    BLEBRR_UNLOCK_VOID();
}

static BLEBRR_Q_ELEMENT * blebrr_enqueue_alloc(void)
{
    BLEBRR_Q_ELEMENT * elt;
    UINT16 ei;

    /* Get reference to the requested Queue block members */
    elt = blebrr_queue.element;
    ei = blebrr_queue.end;

    /* Check if queue end element is free */
    if (0 != (elt + ei)->pdatalen)
    {
        /* Not free */
        elt = NULL;
    }
    else
    {
        /* Set the element to be returned */
        elt = (elt + ei);

        /* Update the data availability */
        blebrr_datacount++;

        /* Update queue end */
        ei++;
        ei &= (BLEBRR_QUEUE_SIZE - 1);
        blebrr_queue.end = ei;
    }

    return elt;
}

static BLEBRR_Q_ELEMENT * blebrr_dequeue(void)
{
    BLEBRR_Q_ELEMENT * elt;
    UINT16 si;

    /* Get reference to the requested Queue block members */
    elt = blebrr_queue.element;
    si = blebrr_queue.start;

    /* Check if queue start element is valid */
    if (0 == (elt + si)->pdatalen)
    {
        /* Not valid */
        elt = NULL;
    }
    else
    {
        /* Set the element to be returned */
        elt = (elt + si);

        /* Update the data availability */
        blebrr_datacount--;

        /* Update queue start */
        si++;
        si &= (BLEBRR_QUEUE_SIZE - 1);
        blebrr_queue.start = si;
    }

    return elt;
}

static UCHAR blebrr_is_queue_left(void)
{
    BLEBRR_Q_ELEMENT * elt;
    UINT16 si;

    /* Get reference to the requested Queue block members */
    elt = blebrr_queue.element;
    si = blebrr_queue.start;

    /* Check if queue start element is valid */
    return (0 != (elt + si)->pdatalen) ? MS_TRUE : MS_FALSE;
}

static API_RESULT blebrr_update_advdata(void)
{
    BLEBRR_Q_ELEMENT * elt;
    UCHAR type;

    elt = NULL;

    /**
     * Prioritize transmission of data over beacons.
     */
    elt = blebrr_dequeue();

    /* Any beacon or data to be transmitted? */
    if (NULL == elt)
    {
        /* Update state */
        BLEBRR_SET_STATE_ADV(BLEBRR_STATE_IDLE);

        return API_FAILURE;
    }

    /* Set the type */
    type = elt->type;

    /* Set the advertising data */
    blebrr_advertise_data_pl(type, elt->pdata, elt->pdatalen);

    /* Yes, Free the element */
    EM_free_mem(elt->pdata);
    blebrr_memcount--;
    elt->pdata = NULL;
    elt->pdatalen = 0;

    #if (0 != BLEBRR_ADV_ABORT_TIMEOUT)
    /* Start Advertising Abort Timer, fail-safe to recover from unexpected R_BLE API error */
    blebrr_timer_start(BLEBRR_ADV_ABORT_TIMEOUT);
    #endif /* (0 != BLEBRR_ADV_ABORT_TIMEOUT) */

    /* Update state */
    BLEBRR_SET_STATE_ADV(BLEBRR_STATE_IN_ADV_ENABLE);

    return API_SUCCESS;
}

static blebrr_adv_setup(void)
{
    UINT32 delay_ms = 0;

    if (MS_TRUE == blebrr_is_queue_left())
    {
        #if (0 != BLEBRR_ADVREPEAT_RAND_DELAY)
        /* Add Random Transmission Delay */
        delay_ms = MS_rand_u32_pl(BLEBRR_ADVREPEAT_RAND_DELAY);
        #endif /* (0 != BLEBRR_ADVREPEAT_RAND_DELAY) */
    }

    if (0 != delay_ms)
    {
        blebrr_timer_start(delay_ms);
    }
    else
    {
        blebrr_update_advdata();
    }
}

static API_RESULT blebrr_send
                       (
                           UCHAR type,
                           void * pdata,
                           UINT16 datalen,
                           BLEBRR_Q_ELEMENT ** pelt
                       )
{
    UCHAR * data, * ptr;
    UINT16 packet_len;
    UCHAR offset;
    BLEBRR_Q_ELEMENT * elt;

    data = (UCHAR *)pdata;
    elt = *pelt;

    /* Get the offset based on the type */
    offset = (0 != type)? BLEBRR_NCON_ADVTYPE_OFFSET: 0;

    /* Calculate the total length, including Adv Data Type headers */
    packet_len = (UINT16)(datalen + offset);

    /* Allocate and save the data */
    ptr = EM_alloc_mem(packet_len);
    if (NULL == ptr)
    {
        BLEBRR_LOG("Failed to allocate memory!\n");
        return API_FAILURE;
    }
    blebrr_memcount++;

    if (NULL == elt)
    {
        /* Allocate the next free element in the data queue */
        elt = blebrr_enqueue_alloc();

        /* Is any element free? */
        if (NULL == elt)
        {
            /* Unlock */
            BLEBRR_UNLOCK();

            /* Free the memory allocated */
            EM_free_mem (ptr);
            blebrr_memcount--;

            BLEBRR_LOG("Queue Full!\n");
            return API_FAILURE;
        }

        /* Update element type */
        elt->type = (0 != type)? BRR_BCON_PASSIVE: BRR_BCON_ACTIVE;
    }

    /* Assign the allocated memory */
    elt->pdata = ptr;

    if (offset >= 1)
    {
        /* Add the Length and Adv type headers */
        elt->pdata[0] = (UCHAR)(datalen + (offset - 1));

        if (offset - 1)
        {
            elt->pdata[1] = type;
        }
    }

    /* Update the data and datalen */
    memcpy((elt->pdata + offset), data, datalen);
    elt->pdatalen = packet_len;

    if (BLEBRR_STATE_IDLE == BLEBRR_GET_STATE_ADV())
    {
        /* Enable Advertising with Data */
        blebrr_adv_setup();
    }

    return API_SUCCESS;
}

static API_RESULT blebrr_bcon_send(BRR_HANDLE * handle, void * pdata, UINT16 datalen)
{
    BRR_BEACON_INFO * info;
    BLEBRR_Q_ELEMENT * elt;
    UCHAR op, action, type, bcon;
    API_RESULT retval;

    MS_IGNORE_UNUSED_PARAM(handle);
    MS_IGNORE_UNUSED_PARAM(datalen);

    /* Get the beacon information */
    info = (BRR_BEACON_INFO *)pdata;

    /* Get the Operation and Action */
    op = (info->action & 0x0F);
    action = ((info->action & 0xF0) >> 4);

    /* Get the Broadcast/Observe type and Beacon type */
    type = (info->type & 0x0F);
    bcon = ((info->type & 0xF0) >> 4);

    retval = API_SUCCESS;

    /* Lock */
    BLEBRR_LOCK();

    /* Check the operations */
    switch (op)
    {
        case BRR_OBSERVE:
            break;

        case BRR_BROADCAST:
            if (BRR_ENABLE == action)
            {
                /* Update the connectable beacon packet */
                if ((BRR_BCON_ACTIVE == type) && ((NULL != info->bcon_data)))
                {
                    /* Active Beacon (advdata) Source Index */
                    UCHAR abs_index;

                    abs_index = R_MS_BRR_Get_GattMode();

                    if (BLEBRR_GATT_PROV_MODE == abs_index)
                    {
                        /* Copy the incoming UUID and OOB info to global connectable ADV data for PB GATT */
                        /* TODO have a state to decide about provisioned and unprovisioned state */

                        memcpy
                        (
                            blebrr_gap_adv_data[abs_index].data + BLEBRR_GATT_ADV_SERV_DATA_OFFSET,
                            info->bcon_data + 1,
                            16 + 2
                        );

                        /**
                         * NOTE: It is not need to calculate assign the Service Data Length as
                         * Service Data length is Fixed for Connectable Provisioning ADV.
                         * This data length is : 1 + 2 + 16 + 2 = 0x15 Bytes, already updated
                         * in the global data structure blebrr_gap_adv_data[0].
                         */

                        /* Update the beacon type */
                        bcon = BRR_BCON_TYPE_GATT_UNPROV_DEVICE;
                    }
                    /* Assuming that this Active Beacon is for GATT Proxy*/
                    else
                    {
                        /* Copy the incoming Type and either Network ID or Hash and Random to global connectable ADV data for PB GATT */
                        /* TODO have a state to decide about provisioned and unprovisioned state */

                        abs_index = BLEBRR_GATT_PROXY_MODE;

                        /* Copy the incoming Proxy ADV data */
                        memcpy
                        (
                            blebrr_gap_adv_data[abs_index].data + BLEBRR_GATT_ADV_SERV_DATA_OFFSET,
                            info->bcon_data,
                            info->bcon_datalen
                        );

                        /* Copy the incoming Proxy ADV datalen + the BLEBRR_GATT_ADV_SERV_DATA_OFFSET */
                        blebrr_gap_adv_data[abs_index].datalen = (UCHAR)(BLEBRR_GATT_ADV_SERV_DATA_OFFSET + info->bcon_datalen);

                        /**
                         * Assign the service data length correctly for Proxy ADVs
                         * Total incoming data + 1 Byte of AD Flags + 2 Bytes of Service UUID
                         */
                        blebrr_gap_adv_data[abs_index].data[BLEBRR_GATT_ADV_SERV_DATALEN_OFFSET] =
                            (UCHAR)(info->bcon_datalen + 1 + 2);
                    }

                    /* Re-assign updated ADV data to Info Structure */
                    info->bcon_data    = blebrr_gap_adv_data[abs_index].data;
                    info->bcon_datalen = blebrr_gap_adv_data[abs_index].datalen;
                }

                elt = NULL;

                /* Schedule to send */
                retval = blebrr_send
                         (
                             ((BRR_BCON_TYPE_UNPROV_DEVICE == bcon) &&
                              (BRR_BCON_ACTIVE != type))? MESH_AD_TYPE_BCON : 0,
                             info->bcon_data,
                             info->bcon_datalen,
                             &elt
                         );

                if (API_SUCCESS == retval)
                {
                    /* Check if URI data is present for Unprovisioned device */
                    if ((BRR_BCON_TYPE_UNPROV_DEVICE == bcon) &&
                        (NULL != info->uri) &&
                        (NULL != info->uri->payload) &&
                        (0 != info->uri->length) && ((BLEBRR_GAP_ADVDATA_LEN - BLEBRR_NCON_ADVTYPE_OFFSET) >= info->uri->length))
                    {
                        elt = NULL;

                        /* Schedule to send */
                        retval = blebrr_send
                                 (
                                     0x24, /* AD Type: <<URI>> */
                                     info->uri->payload,
                                     info->uri->length,
                                     &elt
                                 );
                    }
                }
            }
            break;

        default:
            break;
    }

    /* Unlock */
    BLEBRR_UNLOCK();

    return retval;
}


/**
*  \brief
*
*  \par Description
*
*
*  \param handle
*  \param pdata
*  \param datalen
*
*  \return void
*/
static API_RESULT blebrr_adv_send(BRR_HANDLE * handle, UCHAR type, void * pdata, UINT16 datalen)
{
    BLEBRR_Q_ELEMENT * elt;
	API_RESULT       retval;

    /* Validate handle */
    if (*handle != blebrr_advhandle)
    {
        return API_FAILURE;
    }

    if ((NULL == pdata) || (0 == datalen))
    {
        return API_FAILURE;
    }

    if (MS_TRUE == blebrr_adv_disabled)
    {
        return API_FAILURE;
    }

    /* If beacon type, pass to the handler */
    if (MESH_AD_TYPE_BCON == type)
    {
        BRR_BEACON_INFO * info;

        /* Get reference to the beacon info */
        info = (BRR_BEACON_INFO *)pdata;

        if (BRR_BCON_TYPE_SECURE_NET != (info->type >> 4))
        {
            return blebrr_bcon_send(handle, pdata, datalen);
        }
        else
        {
            /* Update the data and length reference */
            pdata = info->bcon_data;
            datalen = info->bcon_datalen;
        }
    }

    /* Lock */
    BLEBRR_LOCK();


    elt = NULL;

    /* Schedule to send */
    retval = blebrr_send
             (
                 type,
                 pdata,
                 datalen,
                 &elt
             );

    /* Unlock */
    BLEBRR_UNLOCK();

    return retval;
}

static void blebrr_adv_sleep(BRR_HANDLE * handle)
{
    BLEBRR_LOCK_VOID();

    /* Set bearer sleep state */
    blebrr_sleep = MS_TRUE;

    if ((BLEBRR_STATE_SCAN_ENABLED == BLEBRR_GET_STATE_SCAN()) ||
        (BLEBRR_STATE_IN_SCAN_ENABLE == BLEBRR_GET_STATE_SCAN()))
    {
        /* Disable Scan */
        blebrr_scan_pl(MS_FALSE);

        /* Update state */
        BLEBRR_SET_STATE_SCAN(BLEBRR_STATE_IN_SCAN_DISABLE);
    }

    /* Enter platform sleep */
    EM_enter_sleep_pl();

    BLEBRR_UNLOCK_VOID();
}

static void blebrr_adv_wakeup(BRR_HANDLE * handle, UINT8 mode)
{
    BLEBRR_LOCK_VOID();

    /* Exit platform sleep */
    EM_exit_sleep_pl();

    if (BRR_RX & mode)
    {
        /* Reset bearer sleep state */
        blebrr_sleep = MS_FALSE;

        if ((BLEBRR_STATE_IDLE == BLEBRR_GET_STATE_SCAN()) ||
            (BLEBRR_STATE_IN_SCAN_DISABLE == BLEBRR_GET_STATE_SCAN()))
        {
            /* Enable Scan */
            blebrr_scan_pl(MS_TRUE);

            /* Update state */
            BLEBRR_SET_STATE_SCAN(BLEBRR_STATE_IN_SCAN_ENABLE);
        }
    }

    BLEBRR_UNLOCK_VOID();
}

/**
 *  \brief
 *
 *  \par Description
 *
 *
 *  \param args
 *  \param size
 *
 *  \return void
 */
static void blebrr_advscan_timeout_handler(UINT32 timer_hdl)
{
    MS_IGNORE_UNUSED_PARAM(timer_hdl);

    BLEBRR_LOCK_VOID();

    /* Check the state of AdvScan procedure */
    switch (BLEBRR_GET_STATE_ADV())
    {
        case BLEBRR_STATE_IDLE:
        case BLEBRR_STATE_IN_ADV_DISABLE:
            /* Enable Advertising with Next Data */
            blebrr_update_advdata();
            break;

        case BLEBRR_STATE_ADV_ENABLED:
            /* Disable Adv */
            blebrr_advertise_pl (MS_FALSE);

            /* Update state */
            BLEBRR_SET_STATE_ADV(BLEBRR_STATE_IN_ADV_DISABLE);
            break;

        case BLEBRR_STATE_IN_ADV_ENABLE:
            #if (0 != BLEBRR_ADV_ABORT_TIMEOUT)
            /* Advertising Abort Timer expired, Enable Advertising with Next Data */
            blebrr_update_advdata();
            #endif /* (0 != BLEBRR_ADV_ABORT_TIMEOUT) */
            break;

        default:
            /* Should not reach here */
            break;
    }

    BLEBRR_UNLOCK_VOID();
}


/**
 *  \brief
 *
 *  \par Description
 *
 *
 *  \param timeout
 *
 *  \return void
 */
static void blebrr_timer_start(UINT32 timeout)
{
    R_BLE_TIMER_UpdateTimeout(blebrr_timer_handle, timeout);
}

/***************************************************************************//**
* @brief Handles Scan operation
*******************************************************************************/
void blebrr_pl_scan_setup(UCHAR enable)
{
    BLEBRR_LOCK_VOID();

    /* Is scan enabled? */
    if (MS_TRUE == enable)
    {
        /* Yes, Update state */
        BLEBRR_SET_STATE_SCAN(BLEBRR_STATE_SCAN_ENABLED);
    }
    else
    {
        /* Update state */
        BLEBRR_SET_STATE_SCAN(BLEBRR_STATE_IDLE);
    }

    BLEBRR_UNLOCK_VOID();
}

/***************************************************************************//**
* @brief Handles Advertisement operation
*******************************************************************************/
void blebrr_pl_advertise_setup(UCHAR enable)
{
    BLEBRR_LOCK_VOID();

    /* Is advertise enabled? */
    if (MS_TRUE == enable)
    {
        /* Yes, Update state */
        BLEBRR_SET_STATE_ADV(BLEBRR_STATE_ADV_ENABLED);

        /* Start bearer timer for Adv Timeout */
        blebrr_timer_start (BLEBRR_ADV_TIMEOUT);
    }
    else
    {
        /* No, Enable Advertising with Next Data */
        blebrr_adv_setup();
    }

    BLEBRR_UNLOCK_VOID();
}

/***************************************************************************//**
* @brief Handles Advertising Reports
*******************************************************************************/
void blebrr_pl_recv_advpacket(UCHAR type, UCHAR * pdata, UINT16 pdatalen, UCHAR rssi)
{
    MS_BUFFER info;

    /* Handle only if Non-Connectable (Passive) Advertising */
    if (BRR_BCON_PASSIVE != type)
    {
        return;
    }

    /* Pack the RSSI as metadata */
    info.payload = &rssi;
    info.length = sizeof(UCHAR);

    /* Deliver the packet to the bearer */
    if (NULL != blebrr_adv.bearer_recv)
    {
        blebrr_adv.bearer_recv(&blebrr_advhandle, pdata, pdatalen, &info);
    }
    else
    {
        BLEBRR_LOG("BEARER RECV Callback Currently NULL !!\n");
    }
}

/***************************************************************************//**
* @brief Registers ADV Bearer with Mesh Stack and Start Scan
*******************************************************************************/
void R_MS_BRR_Setup(void)
{
    /* Initialize locals */
    BLEBRR_MUTEX_INIT_VOID();
    BLEBRR_SET_STATE_SCAN(BLEBRR_STATE_IDLE);
    BLEBRR_SET_STATE_ADV(BLEBRR_STATE_IDLE);
    blebrr_queue.start = 0;
    blebrr_queue.end = 0;

    /* Initialize Timer */
    R_BLE_TIMER_Create(&blebrr_timer_handle, BLEBRR_ADV_TIMEOUT, BLE_TIMER_ONE_SHOT, blebrr_advscan_timeout_handler);

    /* Reset the bearer sleep */
    blebrr_sleep = MS_FALSE;
    blebrr_adv_disabled = MS_FALSE;

    /* Add the ADV Bearer */
    blebrr_adv.bearer_send = blebrr_adv_send;
    blebrr_adv.bearer_sleep = blebrr_adv_sleep;
    blebrr_adv.bearer_wakeup = blebrr_adv_wakeup;
    MS_brr_add_bearer(BRR_TYPE_ADV, &blebrr_adv, &blebrr_advhandle);

    /* Start Observing */
    BLEBRR_LOG ("Start Observing...\n");
    blebrr_scan_pl (MS_TRUE);

    /* Update state */
    BLEBRR_SET_STATE_SCAN(BLEBRR_STATE_IN_SCAN_ENABLE);
}

/***************************************************************************//**
* @brief Unregisters ADV Bearer and free the allocated resources
*******************************************************************************/
void R_MS_BRR_Close(void)
{
    BLEBRR_Q_ELEMENT * elt;

    /* Remove the Adv Bearer */
    MS_brr_remove_bearer(BRR_TYPE_ADV, &blebrr_advhandle);
    blebrr_advhandle = BRR_HANDLE_INVALID;

    /* Finalize Timer */
    R_BLE_TIMER_Delete(&blebrr_timer_handle);

    /* Free the TX Queues */
    for (UINT16 idx = 0; idx < BLEBRR_QUEUE_SIZE; idx++)
    {
        elt = blebrr_dequeue();
        if (NULL == elt)
        {
            break;
        }
        EM_free_mem(elt->pdata);
        elt->pdatalen = 0;
        blebrr_memcount--;
    }
}
