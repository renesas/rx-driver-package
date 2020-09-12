
/**
 *  \file blebrr.c
 *
 *  This File contains the BLE Bearer interface for the
 *  Mindtree Mesh stack.
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "blebrr.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#define BLEBRR_QUEUE_SIZE                   32
#define BLEBRR_ADV_TIMEOUT                  (EM_TIMEOUT_MILLISEC | 4)
#define BLEBRR_NCON_ADVTYPE_OFFSET          2
#define BLEBRR_ADVREPEAT_COUNT              5

/** Bearer state defines */
#define BLEBRR_STATE_IDLE                   0x00
#define BLEBRR_STATE_IN_SCAN_ENABLE         0x01
#define BLEBRR_STATE_IN_SCAN_DISABLE        0x02
#define BLEBRR_STATE_SCAN_ENABLED           0x04
#define BLEBRR_STATE_IN_ADV_ENABLE          0x10
#define BLEBRR_STATE_IN_ADV_DISABLE         0x20
#define BLEBRR_STATE_ADV_ENABLED            0x40

#define BLEBRR_MASK_STATE_ADV_ACTIVE \
                (BLEBRR_STATE_IN_ADV_ENABLE  | \
                 BLEBRR_STATE_IN_ADV_DISABLE | \
                 BLEBRR_STATE_ADV_ENABLED)

/** Beacon type defines */
#define BLEBRR_UPROV_ADV_BCON               0x00
#define BLEBRR_UPROV_ADV_URI                0x01
#define BLEBRR_UPROV_GATT_BCON              0x02
#define BLEBRR_UPROV_GATT_URI               0x03
#define BLEBRR_SECNET_BCON                  0x04
#define BLEBRR_NUM_BCONS                    0x05

/** GATT Mode GAP Connectable Advertising Service data offset */
#define BLEBRR_GATT_ADV_SERV_DATA_OFFSET    11
#define BLEBRR_GATT_ADV_SERV_DATALEN_OFFSET 7

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

#define BLEBRR_SET_STATE(x)                 blebrr_state = (x)
#define BLEBRR_GET_STATE()                  blebrr_state

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
static BRR_HANDLE blebrr_advhandle;

static UCHAR blebrr_bconidx;
static UCHAR blebrr_beacons;
static BLEBRR_Q_ELEMENT blebrr_bcon[BRR_BCON_COUNT];
static BLEBRR_Q blebrr_queue;
/* static UCHAR blebrr_adv_bcon_type; */

MS_DEFINE_MUTEX_TYPE(static, blebrr_mutex)
static EM_timer_handle blebrr_timer_handle;
static UCHAR blebrr_state;
static UCHAR blebrr_datacount;

/* static UCHAR blebrr_scan_type; */
static UCHAR blebrr_advrepeat_count;

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
             *      0x08: Simultaneous LE and BR/EDR to Same Device
             *            Capable (Controller)
             *      0x10: Simultaneous LE and BR/EDR to Same Device
             *            Capable (Host)
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
             */
             0x15, 0x16,
             0x27, 0x18,
             0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
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
             *      0x08: Simultaneous LE and BR/EDR to Same Device
             *            Capable (Controller)
             *      0x10: Simultaneous LE and BR/EDR to Same Device
             *            Capable (Host)
             */
            0x02, 0x01, 0x06,

            /**
             *  Service UUID List:
             *      Mesh Proxy Service (0x1828)
             */
            0x03, 0x03, 0x28, 0x18,

            /**
             *  Service Data List:
             *      Mesh Provisioning Service (0x1828)
             *      Type (1 Byte) "0x00 - Network ID; 0x01 - Node Identity"
             *      NetWork ID (8 Bytes)
             */
             0x0C, 0x16,
             0x28, 0x18,
             0x00,
             0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88
        },

        /** Advertising Data length */
        20
    }
};

static UCHAR blebrr_sleep;

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Enables Scan
*******************************************************************************/
void blebrr_scan_enable(void)
{
    BLEBRR_LOCK_VOID();

    if ((BLEBRR_STATE_IDLE == BLEBRR_GET_STATE()) &&
        (MS_TRUE != blebrr_sleep))
    {
        blebrr_scan_pl(MS_TRUE);

        /* Update state */
        BLEBRR_SET_STATE(BLEBRR_STATE_IN_SCAN_ENABLE);
    }

    BLEBRR_UNLOCK_VOID();
}

static UCHAR blebrr_get_beacon_type (UCHAR type, UCHAR bcon)
{
    return (BRR_BCON_PASSIVE == type)?
           ((BRR_BCON_TYPE_UNPROV_DEVICE == bcon)? BLEBRR_UPROV_ADV_BCON: BLEBRR_SECNET_BCON):
           ((BRR_BCON_TYPE_UNPROV_DEVICE == bcon)? BLEBRR_UPROV_GATT_BCON: BLEBRR_NUM_BCONS);
}

static BLEBRR_Q_ELEMENT * blebrr_enqueue_alloc (void)
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

static BLEBRR_Q_ELEMENT * blebrr_dequeue (void)
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

        /* Is Adv data type in element? */
        if (BRR_BCON_COUNT == elt->type)
        {
            /* Update the data availability */
            blebrr_datacount--;
        }

        /* Update queue start */
        si++;
        si &= (BLEBRR_QUEUE_SIZE - 1);
        blebrr_queue.start = si;
    }

    return elt;
}

static void blebrr_clear_bcon (UCHAR bcon)
{
    BLEBRR_Q_ELEMENT * elt;

    /* Get reference to the beacon queue element */
    elt = &blebrr_bcon[bcon];

    /* Clear the element and the next one for the given type of beacon */
    if (NULL != elt->pdata)
    {
        EM_free_mem (elt->pdata);
        elt->pdata = NULL;
        elt->pdatalen = 0;

        elt->type = BRR_BCON_COUNT;

        if ((BRR_BCON_TYPE_UNPROV_DEVICE == bcon) &&
            (NULL != (elt + 1)->pdata) &&
            (0 != (elt + 1)->pdatalen))
        {
            EM_free_mem((elt + 1)->pdata);
            (elt + 1)->pdata = NULL;
            (elt + 1)->pdatalen = 0;

            (elt + 1)->type = BRR_BCON_COUNT;
        }

        blebrr_datacount--;
    }
}

static BLEBRR_Q_ELEMENT * blebrr_get_next_beacon(void)
{
    BLEBRR_Q_ELEMENT * elt;
    UCHAR index;

    elt = NULL;

    if (0 == blebrr_beacons)
    {
        return elt;
    }

    index = blebrr_bconidx;

    do
    {
        if (blebrr_beacons & (UCHAR)(1 << index))
        {
            elt = &blebrr_bcon[index];
        }

        index++;
        if (BRR_BCON_COUNT == index)
        {
            index = 0;
        }
    } while (NULL == elt);

    blebrr_bconidx = index;

    return elt;
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

    /* No Data, Check if any beacon available */
    if (NULL == elt)
    {
        elt = blebrr_get_next_beacon();
    }

    /* Any beacon or data to be transmitted? */
    if (NULL == elt)
    {
        /* No, return */
        return API_FAILURE;
    }

    /* Set the type */
    type = ((BRR_BCON_COUNT == elt->type) ? BRR_BCON_PASSIVE : elt->type);

    /* Update global adv bcon type */
    /* blebrr_adv_bcon_type = type; */

    /* Set the advertising data */
    blebrr_advrepeat_count = 1;
    blebrr_advertise_data_pl(type, elt->pdata, elt->pdatalen);
    /* BLEBRR_LOG("\n ### [ADV-Tx]: blebrr adv send of type 0x%02X:\n", type);
    appl_dump_bytes(elt->pdata, elt->pdatalen); */

    /* Is Adv data type in element? */
    if (BRR_BCON_COUNT == elt->type)
    {
        /* Yes, Free the element */
        EM_free_mem(elt->pdata);
        elt->pdatalen = 0;
    }

    return API_SUCCESS;
}

static void blebrr_send
                 (
                     UCHAR type,
                     void * pdata,
                     UINT16 datalen,
                     BLEBRR_Q_ELEMENT * elt
                 )
{
    API_RESULT retval;
    UCHAR * data;
    UINT16 packet_len;
    UCHAR offset;

    data = (UCHAR *)pdata;

    /* Get the offset based on the type */
    offset = (0 != type)? BLEBRR_NCON_ADVTYPE_OFFSET: 0;

    /* Calculate the total length, including Adv Data Type headers */
    packet_len = datalen + offset;

    /* Allocate and save the data */
    elt->pdata = EM_alloc_mem(packet_len);
    if (NULL == elt->pdata)
    {
        BLEBRR_LOG("Failed to allocate memory!\n");
        return;
    }

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
    EM_mem_copy((elt->pdata + offset), data, datalen);
    elt->pdatalen = packet_len;

    /* Is the Adv/Scan timer running? */
    if (EM_TIMER_HANDLE_INIT_VAL != blebrr_timer_handle)
    {
        /* Yes. Do nothing */
    }
    else
    {
        #if 1
        if ((BLEBRR_MASK_STATE_ADV_ACTIVE & BLEBRR_GET_STATE()) == 0)
        #else
        if ((BLEBRR_STATE_IDLE == BLEBRR_GET_STATE()) ||
            (BLEBRR_STATE_SCAN_ENABLED == BLEBRR_GET_STATE()))
        #endif
        {
            /* No, Enable Advertising with Data */
            retval = blebrr_update_advdata();

            if (API_SUCCESS == retval)
            {
                /* Update state */
                BLEBRR_SET_STATE(BLEBRR_STATE_IN_ADV_ENABLE);
            }
        }
    }

    return;
}

static API_RESULT blebrr_bcon_send(BRR_HANDLE * handle, void * pdata, UINT16 datalen)
{
    BRR_BEACON_INFO * info;
    BLEBRR_Q_ELEMENT * elt;
    UCHAR op, action, type, bcon, bcontype;

    /* Get the beacon information */
    info = (BRR_BEACON_INFO *)pdata;

    /* Get the Operation and Action */
    op = (info->action & 0x0F);
    action = ((info->action & 0xF0) >> 4);

    /* Get the Broadcast/Observe type and Beacon type */
    type = (info->type & 0x0F);
    bcon = ((info->type & 0xF0) >> 4);

    /* Lock */
    BLEBRR_LOCK();

    /* Check the operations */
    switch (op)
    {
        case BRR_OBSERVE:
            /* blebrr_scan_type = type; */
            break;

        case BRR_BROADCAST:
            /* Get the Beacon mapping at the BLEBRR */
            bcontype = blebrr_get_beacon_type (type, bcon);

            if (BRR_ENABLE == action)
            {
                /* Update the connectable beacon packet */
                if ((BRR_BCON_ACTIVE == type) && ((NULL != info->bcon_data)))
                {
                    /* Active Beacon (advdata) Source Index */
                    UCHAR abs_index;

                    abs_index = blebrr_gatt_mode_get();

                    if (BLEBRR_GATT_PROV_MODE == abs_index)
                    {
                        /* Copy the incoming UUID and OOB info to global connectable ADV data for PB GATT */
                        /* TODO have a state to decide about provisioned and unprovisioned state */

                        EM_mem_copy
                        (
                            blebrr_gap_adv_data[abs_index].data + BLEBRR_GATT_ADV_SERV_DATA_OFFSET,
                            info->bcon_data + 1,
                            16 + 2
                        );

                        /**
                         * NOTE: It is not need to calculate assign the Service Data Length as
                         *       Service Data length is Fixed for Connectable Provisioning ADV.
                         * This data length is : 1 + 2 + 16 + 2 = 0x15 Bytes, already updated
                         * in the global data strucutre blebrr_gap_adv_data[0].
                         */

                        /* Update the beacon type */
                        bcon = BRR_BCON_TYPE_GATT_UNPROV_DEVICE;
                    }
                    /* Assuming that this Active Beacon is for GATT Proxy*/
                    else
                    {
                        /* Copy the incoming UUID and OOB info to global connectable ADV data for PB GATT */
                        /* TODO have a state to decide about provisioned and unprovisioned state */

                        abs_index = BLEBRR_GATT_PROXY_MODE;

                        /* Copy the incoming Proxy ADV data */
                        EM_mem_copy
                        (
                            blebrr_gap_adv_data[abs_index].data + BLEBRR_GATT_ADV_SERV_DATA_OFFSET,
                            info->bcon_data,
                            info->bcon_datalen
                        );

                        /* Copy the incoming Proxy ADV datalen + the BLEBRR_GATT_ADV_SERV_DATA_OFFSET */
                        blebrr_gap_adv_data[abs_index].datalen = BLEBRR_GATT_ADV_SERV_DATA_OFFSET + info->bcon_datalen;

                        /**
                         * Assign the service data length correctly for Proxy ADVs
                         * Total incoming data + 1 Byte of AD Flags + 2 Bytes of Service UUID
                         */
                        blebrr_gap_adv_data[abs_index].data[BLEBRR_GATT_ADV_SERV_DATALEN_OFFSET] =
                            info->bcon_datalen + 1 + 2;
                    }

                    /* Re-assign updated ADV data to Info Structure */
                    info->bcon_data    = blebrr_gap_adv_data[abs_index].data;
                    info->bcon_datalen = blebrr_gap_adv_data[abs_index].datalen;
                }

                /* Check if beacon element is free */
                if (0 != blebrr_bcon[bcon].pdatalen)
                {
                    /* Unlock */
                    BLEBRR_UNLOCK();

                    BLEBRR_LOG("Beacon Not Free!\n");
                    return API_FAILURE;
                }

                /* Update the active beacon mask */
                blebrr_beacons |= (UCHAR)(1 << bcon);

                elt = &blebrr_bcon[bcon];

                blebrr_datacount++;

                /* Update element type */
                elt->type = type;

                /* Schedule to send */
                blebrr_send
                (
                    ((BRR_BCON_TYPE_UNPROV_DEVICE == bcon) &&
                     (BRR_BCON_ACTIVE != type))? MESH_AD_TYPE_BCON : 0,
                    info->bcon_data,
                    info->bcon_datalen,
                    elt
                );

                /* Check if URI data is present for Unprovisioned device */
                if ((BRR_BCON_TYPE_UNPROV_DEVICE == bcon) &&
                    (NULL != info->uri) &&
                    (NULL != info->uri->payload) &&
                    (0 != info->uri->length))
                {
                    elt = &blebrr_bcon[bcon + 1];

                    /* Update element type */
                    elt->type = bcontype + 1;

                    /* Schedule to send */
                    blebrr_send
                    (
                        0,
                        info->uri->payload,
                        info->uri->length,
                        elt
                    );
                }
            }
            else
            {
                /* Update beacon type for Unprovisioned GATT */
                if ((BRR_BCON_TYPE_UNPROV_DEVICE == bcon) &&
                    (BRR_BCON_ACTIVE == type))
                {
                    bcon = BRR_BCON_TYPE_GATT_UNPROV_DEVICE;
                }

                /* Remove the beacon with type from the queue */
                blebrr_clear_bcon (bcon);

                /* Update the active beacon mask */
                blebrr_beacons &= (UCHAR)(~(1 << bcon));
            }
            break;

        default:
            break;
    }

    /* Unlock */
    BLEBRR_UNLOCK();

    return API_SUCCESS;
}


static API_RESULT blebrr_adv_send(BRR_HANDLE * handle, UCHAR type, void * pdata, UINT16 datalen)
{
    BLEBRR_Q_ELEMENT * elt;

    /* Validate handle */
    if (*handle != blebrr_advhandle)
    {
        return API_FAILURE;
    }

    if ((NULL == pdata) ||
        (0 == datalen))
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

    /* Allocate the next free element in the data queue */
    elt = blebrr_enqueue_alloc();

    /* Is any element free? */
    if (NULL == elt)
    {
        /* Unlock */
        BLEBRR_UNLOCK();

        BLEBRR_LOG("Queue Full!\n");
        return API_FAILURE;
    }

    /* Update element type */
    elt->type = BRR_BCON_COUNT;

    /* Schedule to send */
    blebrr_send
    (
        type,
        pdata,
        datalen,
        elt
    );

    /* Unlock */
    BLEBRR_UNLOCK();

    return API_SUCCESS;
}

static void blebrr_adv_sleep(BRR_HANDLE * handle)
{
    BLEBRR_LOCK_VOID();

    /* Set bearer sleep state */
    blebrr_sleep = MS_TRUE;

    if (BLEBRR_STATE_SCAN_ENABLED == BLEBRR_GET_STATE())
    {
        /* Disable Scan */
        blebrr_scan_pl(MS_FALSE);

        /* Update state */
        BLEBRR_SET_STATE(BLEBRR_STATE_IN_SCAN_DISABLE);
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

        if (BLEBRR_STATE_IDLE == BLEBRR_GET_STATE())
        {
            /* Enable Scan */
            blebrr_scan_pl(MS_TRUE);

            /* Update state */
            BLEBRR_SET_STATE(BLEBRR_STATE_IN_SCAN_ENABLE);
        }
    }

    BLEBRR_UNLOCK_VOID();
}

static void blebrr_advscan_timeout_handler (void * args, UINT16 size)
{
    MS_IGNORE_UNUSED_PARAM(args);
    MS_IGNORE_UNUSED_PARAM(size);

    BLEBRR_LOCK_VOID();

    /* Reset Timer Handler */
    blebrr_timer_handle = EM_TIMER_HANDLE_INIT_VAL;

    /* Check the state of AdvScan procedure */
    switch (BLEBRR_GET_STATE())
    {
        case BLEBRR_STATE_ADV_ENABLED:
            /* Disable Adv */
            blebrr_advertise_pl (MS_FALSE);

            /* Update state */
            BLEBRR_SET_STATE(BLEBRR_STATE_IN_ADV_DISABLE);
            break;

        default:
            /* Should not reach here */
            break;
    }

    BLEBRR_UNLOCK_VOID();
}


static void blebrr_timer_start (UINT32 timeout)
{
    EM_RESULT retval;

    retval = EM_start_timer
             (
                 &blebrr_timer_handle,
                 timeout,
                 blebrr_advscan_timeout_handler,
                 NULL,
                 0
             );

    if (EM_SUCCESS != retval)
    {
        /* TODO: Log */
    }
}

/***************************************************************************//**
* @brief Handles Scan operation
*******************************************************************************/
void blebrr_pl_scan_setup (UCHAR enable)
{
    BLEBRR_LOCK_VOID();

    /* Is scan enabled? */
    if (MS_TRUE == enable)
    {
        /* Yes, Update state */
        BLEBRR_SET_STATE(BLEBRR_STATE_SCAN_ENABLED);
    }
    else
    {
        /* Update state */
        BLEBRR_SET_STATE(BLEBRR_STATE_IDLE);
    }

    BLEBRR_UNLOCK_VOID();
}

/***************************************************************************//**
* @brief Handles Advertisement operation
*******************************************************************************/
void blebrr_pl_advertise_setup (UCHAR enable)
{
    API_RESULT retval;
    UINT32 delay_ms = 0;

    BLEBRR_LOCK_VOID();

    /* Is advertise enabled? */
    if (MS_TRUE == enable)
    {
        /* Yes, Update state */
        BLEBRR_SET_STATE(BLEBRR_STATE_ADV_ENABLED);

        /* Add Random Transmision Delay: 0 to 7 ms */
        cry_rand_generate((UCHAR*)&delay_ms, sizeof(UINT32));
        delay_ms &= 0x7;

        /* Start bearer timer for Adv Timeout */
        blebrr_timer_start (BLEBRR_ADV_TIMEOUT + delay_ms);
    }
    else
    {
        if (BLEBRR_ADVREPEAT_COUNT > blebrr_advrepeat_count)
        {
            blebrr_advrepeat_count++;
            blebrr_advertise_pl(MS_TRUE);

            /* Update state */
            BLEBRR_SET_STATE(BLEBRR_STATE_IN_ADV_ENABLE);
        }
        else
        {
            retval = blebrr_update_advdata();

            if (API_SUCCESS == retval)
            {
                /* Update state */
                BLEBRR_SET_STATE(BLEBRR_STATE_IN_ADV_ENABLE);
            }
            else
            {
                /* Update state */
                BLEBRR_SET_STATE(((MS_TRUE == blebrr_sleep)?BLEBRR_STATE_IDLE: BLEBRR_STATE_SCAN_ENABLED));
            }
        }
    }

    BLEBRR_UNLOCK_VOID();
}

/***************************************************************************//**
* @brief Advertisement End function
*******************************************************************************/
void blebrr_pl_advertise_end (void)
{
    BLEBRR_LOCK_VOID();

    BLEBRR_SET_STATE(BLEBRR_STATE_IDLE);
    /* blebrr_adv_bcon_type = 0; */

    BLEBRR_UNLOCK_VOID();
}

/***************************************************************************//**
* @brief Handles Advertising Reports
*******************************************************************************/
void blebrr_pl_recv_advpacket (UCHAR type, UCHAR * pdata, UINT16 pdatalen, UCHAR rssi)
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
* @brief Registers ADV Bearer with Mesh Stack
*******************************************************************************/
void blebrr_register(void)
{
    /* Initialize locals */
    BLEBRR_MUTEX_INIT_VOID();

    /* Initialize Timer Handler */
    blebrr_timer_handle = EM_TIMER_HANDLE_INIT_VAL;

    /* Reset the bearer sleep */
    blebrr_sleep = MS_FALSE;

    /* Add the Adv Bearer */
    blebrr_adv.bearer_send = blebrr_adv_send;
    blebrr_adv.bearer_sleep = blebrr_adv_sleep;
    blebrr_adv.bearer_wakeup = blebrr_adv_wakeup;
    MS_brr_add_bearer(BRR_TYPE_ADV, &blebrr_adv, &blebrr_advhandle);

    /* Start Observing */
    BLEBRR_LOG ("Start Observing...\n");
    blebrr_scan_pl (MS_TRUE);

    /* Update state */
    BLEBRR_SET_STATE(BLEBRR_STATE_IN_SCAN_ENABLE);
}

