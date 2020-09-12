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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_dns_client.h
* Version      : --.--
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 04.04.2011 1.00    First Release
*         : 09.05.2014 1.01    Corresponded to FIT Modules
*         : 01.07.2014 1.02    Clean up source code
*         : 01.04.2016 1.03    XML file update
*         : 01.03.2017 1.04    Change API function name
***********************************************************************************************************************/
#ifndef R_DNS_CLIENT_H
#define R_DNS_CLIENT_H

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define DNS_QUERY_TYPE_A       (0x01)
#define DNS_QUERY_TYPE_CNAME   (0x05)
#define DNS_QUERY_TYPE_UNKNOWN (0xff)

#define DNS_ERROR_COUNT  (3)
#define DNS_BUFFER_COUNT (1)
#define DNS_CANCEL_TIMEOUT_MS (100)
#ifdef _UNIT_TEST_DNS
#define DNS_CLIENT_STATIC
#else
#define DNS_CLIENT_STATIC static
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct DNS_HEADER_STRUCTURE
{

    uint16_t ID;
    uint16_t Flag;
    uint16_t QDCOUNT;
    uint16_t ANCOUNT;
    uint16_t NSCOUNT;
    uint16_t ARCOUNT;

}dns_header_t;

typedef struct DNS_QUESTION_STRUCTURE
{

    char     *QNAME;
    uint16_t QTYPE;
    uint16_t QCLASS;

}dns_question_t;

typedef struct DNS_RESOUCE_RECORD_STRUCTURE
{

    char     *NAME;
    uint16_t TYPE;
    uint16_t CLASS;
    uint16_t TTL;
    uint16_t RDLENGTH;
    char     *RDATA;

}dns_resouce_record_t;

typedef struct DNS_REQUEST_PACKET_STRUCTURE
{

    dns_header_t         header;
    dns_question_t       question;
    dns_resouce_record_t resouce_record;

}dns_packet_t;

typedef struct _dns_address_table
{
    uint32_t dnsaddr1;         /* DNS server IP address     */
    uint32_t dnsaddr2;         /* DNS server IP address sub */
}dns_address_t;

typedef enum
{
    DNS_PROC_REST = 0,                  /* A.resting DNS client                  */
    DNS_PROC_UNRECEIVED_REQUEST,        /* B.unreceived request                  */
    DNS_PROC_START_SENDING,             /* C.start sending                       */
    DNS_PROC_SENDING_REQUEST_WAIT,      /* D.wait to complete sending request    */
    DNS_PROC_RECEIVING_REQUEST_WAIT,    /* E.wait to complete receiving request  */
    DNS_PROC_FINISH,                    /* F.finish                              */
    DNS_PROC_CANCELLING,                /* G.cancelling                          */
    DNS_PROC_CANCELLING_WAIT,           /* H.wait to complete cancelling         */
    DNS_PROC_CANCELLED,                 /* I.cancelled                           */
    DNS_PROC_NUM                        /* the number of process                 */ 
} dns_proc_t;

typedef enum
{
    DNS_EVENT_CALL_OPEN_FUNCTION,         /* 1.called open function         */
    DNS_EVENT_RECEIVED_REQUEST,           /* 2.received request             */
    DNS_EVENT_PERIODIC,                   /* 3.periodic                     */
    DNS_EVENT_CALLBACK_SEND,              /* 4.complete sending callback    */
    DNS_EVENT_CALLBACK_RECEIVE,           /* 5.complete receiving callback  */
    DNS_EVENT_CALLBACK_CANCEL,            /* 6.complete canceling callback  */
    DNS_EVENT_CALLBACK_SEND_ARP_TIMEOUT,  /* 7.send ARP timeout callback    */
    DNS_EVENT_CALL_CLOSE_FUNCTION,        /* 8.called close function        */
    DNS_EVENT_NUM                         /* the number of event            */
} dns_event_t;

typedef enum
{
    DNS_SELECT_PRIMARY,        /* select primary DNS server          */
    DNS_SELECT_SECONDARY,      /* select secondary DNS server        */
    DNS_SELECT_NUM             /* the number of selecting DNS server */
} dns_select_t;


typedef struct
{
    dns_proc_t     proc;                      /* state of DNS process           */
    dns_select_t   dns_select;                /* selected DNS server            */
    dns_select_t   dns_select_sub;            /* selected DNS server            */
    DNS_CB_FUNC    cb_func;                   /* callback function              */
    char           name[DNS_CFG_NAME_SIZE];   /* domain name                    */
    uint32_t       retry_cnt;                 /* retry counter                  */
    ID             cepid;                     /* the communication endpoint ID  */
    uint32_t       dns_timer;                 /* timer for detect timeout       */
    uint32_t       dns_error_type;            /* error type                     */
} dns_mng_t;

typedef dns_err_t (*DNS_PROC_FUNC)(void);

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
DNS_CLIENT_STATIC int32_t send_dns_request(char *mame, uint32_t dstaddr);
DNS_CLIENT_STATIC int32_t receive_dns_response(name_table_t *table);
DNS_CLIENT_STATIC int32_t convert_name_to_dns_query(char *query, char *name);
DNS_CLIENT_STATIC int32_t check_query(char *query);
DNS_CLIENT_STATIC int32_t check_answer(char *answer, char *dns_response, name_table_t *table, uint8_t *query_type);
DNS_CLIENT_STATIC int32_t get_name_till_stringend(const char *string, const char *dns_response, name_table_t *table);
DNS_CLIENT_STATIC void clear_table(name_table_t *table);
DNS_CLIENT_STATIC uint16_t htons(uint16_t data);
DNS_CLIENT_STATIC uint32_t htonl(uint32_t data);
DNS_CLIENT_STATIC void dns_reset_timer(void);
DNS_CLIENT_STATIC void  dns_timeout_timer(void);
static uint32_t convert_array_to_long(uint8_t *ipaddr);

#endif /* R_DNS_CLIENT_H */
