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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_dns_client.c
* Version      : --.--
* Device(s)    : any
* Tool-Chain   : any
* OS     : any
* H/W Platform : any
* Description  : This is the dns client code.
* Operation    :
* Limitations  :
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 04.04.2011 1.00    First Release
*         : 09.05.2014 1.01    Corresponded to FIT Modules
*         : 01.07.2014 1.02    Clean up source code
*         : 01.04.2016 1.03    XML file update
*         : 01.03.2017 1.04    Change API function name
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>
#include <stdint.h>
#include "r_t4_itcpip.h"
#include "r_t4_dns_client_rx_config.h"
#include "r_t4_dns_client_rx_if.h"
#include "r_dns_client.h"
#include "r_sys_time_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
#ifdef _UNIT_TEST_DNS
extern uint8_t flag_sntp_process;
extern uint32_t dns_test_timeout_set;
extern uint8_t flag_proc_C3;
extern uint8_t flag_proc_D4;
extern uint8_t flag_proc_I3;
#endif

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
extern void timer_interrupt(void* pdata);

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
DNS_CLIENT_STATIC uint8_t dns_buffer[512];
DNS_CLIENT_STATIC dns_packet_t dns_packet;
DNS_CLIENT_STATIC uint16_t TRANSACTION_ID = 0;
DNS_CLIENT_STATIC name_table_t name_table;
DNS_CLIENT_STATIC dns_mng_t  dns_mng;
DNS_CLIENT_STATIC dns_address_t dns_ipaddr;
DNS_CLIENT_STATIC dns_address_t dns_ipaddr_sub;


DNS_CLIENT_STATIC dns_err_t dns_dummy_proc(void);
DNS_CLIENT_STATIC dns_err_t dns_error_proc(void);
DNS_CLIENT_STATIC dns_err_t dns_already_proc(void);
DNS_CLIENT_STATIC dns_err_t dns_close_proc(void);
DNS_CLIENT_STATIC dns_err_t dns_no_proc(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_A1(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_B2(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_C3(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_D3(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_D4(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_D7(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_E3(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_E5(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_F3(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_G3(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_H3(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_H6(void);
DNS_CLIENT_STATIC dns_err_t dns_proc_I3(void);

static const DNS_PROC_FUNC dns_proc_table[DNS_PROC_NUM][DNS_EVENT_NUM] =
{
    /* A.resting DNS client */
    {
        dns_proc_A1,       /* 1.called open function         */
        dns_error_proc,    /* 2.received request             */
        dns_error_proc,    /* 3.periodic                     */
        dns_error_proc,    /* 4.complete sending callback    */
        dns_error_proc,    /* 5.complete receiving callback  */
        dns_error_proc,    /* 6.complete canceling callback  */
        dns_error_proc,    /* 7.send ARP timeout callback    */
        dns_error_proc     /* 8.called close function        */
    },

    /* B.unreceived request    */
    {
        dns_already_proc,  /* 1.called open function         */
        dns_proc_B2,       /* 2.received request             */
        dns_no_proc,       /* 3.periodic                     */
        dns_dummy_proc,    /* 4.complete sending callback    */
        dns_no_proc,       /* 5.complete receiving callback  */
        dns_dummy_proc,    /* 6.complete canceling callback  */
        dns_dummy_proc,    /* 7.send ARP timeout callback    */
        dns_close_proc     /* 8.called close function        */
    },

    /* C.start sending    */
    {
        dns_already_proc,  /* 1.called open function         */
        dns_dummy_proc,    /* 2.received request             */
        dns_proc_C3,       /* 3.periodic                     */
        dns_dummy_proc,    /* 4.complete sending callback    */
        dns_no_proc,       /* 5.complete receiving callback  */
        dns_dummy_proc,    /* 6.complete canceling callback  */
        dns_dummy_proc,    /* 7.send ARP timeout callback    */
        dns_close_proc     /* 8.called close function        */
    },
    /* D.wait to complete sending request */
    {
        dns_already_proc,  /* 1.called open function         */
        dns_dummy_proc,    /* 2.received request             */
        dns_proc_D3,       /* 3.periodic                     */
        dns_proc_D4,       /* 4.complete sending callback    */
        dns_no_proc,       /* 5.complete receiving callback  */
        dns_dummy_proc,    /* 6.complete canceling callback  */
        dns_proc_D7,       /* 7.send ARP timeout callback    */
        dns_close_proc     /* 8.called close function        */
    },
    /* E.wait to complete receiveing request*/
    {
        dns_already_proc,  /* 1.called open function         */
        dns_dummy_proc,    /* 2.received request             */
        dns_proc_E3,       /* 3.periodic                     */
        dns_dummy_proc,    /* 4.complete sending callback    */
        dns_proc_E5,       /* 5.complete receiving callback  */
        dns_dummy_proc,    /* 6.complete canceling callback  */
        dns_dummy_proc,    /* 7.send ARP timeout callback    */
        dns_close_proc     /* 8.called close function        */
    },
    /* F.finish       */
    {
        dns_already_proc,  /* 1.called open function         */
        dns_dummy_proc,    /* 2.received request             */
        dns_proc_F3,       /* 3.periodic                     */
        dns_dummy_proc,    /* 4.complete sending callback    */
        dns_no_proc,       /* 5.complete receiving callback  */
        dns_dummy_proc,    /* 6.complete canceling callback  */
        dns_dummy_proc,    /* 7.send ARP timeout callback    */
        dns_close_proc     /* 8.called close function        */
    },
    /* G.cancelling      */
    {
        dns_already_proc,  /* 1.called open function         */
        dns_dummy_proc,    /* 2.received request             */
        dns_proc_G3,       /* 3.periodic                     */
        dns_dummy_proc,    /* 4.complete sending callback    */
        dns_no_proc,       /* 5.complete receiving callback  */
        dns_dummy_proc,    /* 6.complete canceling callback  */
        dns_dummy_proc,    /* 7.send ARP timeout callback    */
        dns_close_proc     /* 8.called close function        */
    },
    /* H.wait to complete cancelling */
    {
        dns_already_proc,  /* 1.called open function         */
        dns_dummy_proc,    /* 2.received request             */
        dns_proc_H3,       /* 3.periodic                     */
        dns_dummy_proc,    /* 4.complete sending callback    */
        dns_no_proc,       /* 5.complete receiving callback  */
        dns_proc_H6,       /* 6.complete canceling callback  */
        dns_dummy_proc,    /* 7.send ARP timeout callback    */
        dns_close_proc     /* 8.called close function        */
    },
    /* I.cancelled      */
    {
        dns_already_proc,  /* 1.called open function         */
        dns_dummy_proc,    /* 2.received request             */
        dns_proc_I3,       /* 3.periodic                     */
        dns_dummy_proc,    /* 4.complete sending callback    */
        dns_no_proc,       /* 5.complete receiving callback  */
        dns_dummy_proc,    /* 6.complete canceling callback  */
        dns_dummy_proc,    /* 7.send ARP timeout callback    */
        dns_close_proc     /* 8.called close function        */
    }
};

DNS_CLIENT_STATIC dns_err_t dns_dummy_proc(void)
{
    /* None */

    return DNS_SUCCESS;
}

DNS_CLIENT_STATIC dns_err_t dns_no_proc(void)
{
    /* None */

    return DNS_SUCCESS;
}

DNS_CLIENT_STATIC dns_err_t dns_error_proc(void)
{
    /* None */
    return DNS_ERR_NOT_OPENED;
}

DNS_CLIENT_STATIC dns_err_t dns_already_proc(void)
{
    /*None*/
    return DNS_ERR_ALREADY_OPENED;
}

DNS_CLIENT_STATIC dns_err_t dns_close_proc(void)
{

    dns_mng.proc = DNS_PROC_REST;   /* proc <- A.rested */

    if (R_SYS_TIME_UnregisterPeriodicCallback((callback_from_sys_time_t)&dns_client_process) == SYS_TIME_BAD_FUNCTION_POINTER)
    {
        return DNS_ERR_NOT_OPENED;
    }

    memset(&dns_mng.name[0], 0, sizeof(dns_mng.name)); /* domain name <- all '\0'        */
    clear_table(&name_table);

    return DNS_SUCCESS;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_A1(void)
{
    dns_mng.proc = DNS_PROC_UNRECEIVED_REQUEST;

    return DNS_SUCCESS;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_B2(void)
{
    dns_mng.proc = DNS_PROC_START_SENDING; /* proc <- C.start sending */

    return DNS_SUCCESS;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_C3(void)
{
    dns_err_t  ercd;
    uint32_t dstaddr;

    ercd = DNS_SUCCESS;

    if (DNS_SELECT_PRIMARY == dns_mng.dns_select)
    {
        dstaddr = dns_ipaddr.dnsaddr1;
    }
    else if (DNS_SELECT_SECONDARY == dns_mng.dns_select)
    {
        dstaddr = dns_ipaddr.dnsaddr2;
    }
    else
    {
        /* None */
    }

    if (E_WBLK == send_dns_request(dns_mng.name, dstaddr))
    {
        dns_reset_timer();
        dns_mng.proc = DNS_PROC_SENDING_REQUEST_WAIT; /* proc <- D.wait to complete sending request */
#ifdef _UNIT_TEST_DNS
        flag_proc_C3 = 1;
#endif
    }
    else
    {
        ercd = DNS_ERR_BAD_UDP_SETTINGS;
        dns_mng.proc = DNS_PROC_CANCELLED;  /* proc <- I.cancelled */
        dns_mng.retry_cnt = 0;     /* no retry */
#ifdef _UNIT_TEST_DNS
        flag_proc_C3 = 1;
#endif
    }

    return ercd;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_D3(void)
{
    dns_err_t  ercd;

    ercd = DNS_SUCCESS;

    dns_timeout_timer();

    if (0 == dns_mng.dns_timer)
    {
        dns_mng.proc = DNS_PROC_CANCELLING;  /* proc <- G.cancelling */
        ercd = DNS_ERR_TIMEOUT;
    }
    return ercd;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_D4(void)
{
    dns_reset_timer();
    dns_mng.proc = DNS_PROC_RECEIVING_REQUEST_WAIT; /* proc <- E.wait to complete receiveing request */
#ifdef _UNIT_TEST_DNS
    flag_proc_D4 = 1;
#endif

    return DNS_SUCCESS;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_D7(void)
{
    dns_mng.proc = DNS_PROC_CANCELLED;  /* proc <- I.cancelled */
    dns_mng.retry_cnt = 0;     /* no retry */

    return DNS_ERR_TIMEOUT;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_E3(void)
{
    dns_err_t  ercd;

    ercd = DNS_SUCCESS;
    dns_timeout_timer();

    if (0 == dns_mng.dns_timer)
    {
        dns_mng.proc = DNS_PROC_CANCELLED;  /* proc <- I.cancelled */
        ercd = DNS_ERR_TIMEOUT;
    }
    return ercd;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_E5(void)
{
    dns_err_t ercd;

    clear_table(&name_table);

    if (0 <= receive_dns_response(&name_table))
    {
        dns_mng.proc = DNS_PROC_FINISH;   /* proc <- F.finish */
        ercd = DNS_SUCCESS;
    }
    else
    {
        dns_mng.proc = DNS_PROC_CANCELLED;  /* proc <- I.cancelled */
        dns_mng.retry_cnt = 0;     /* no retry */
        dns_mng.dns_error_type = DNS_ERR_INVALID_PACKET_RECEIVED;
        ercd = DNS_ERR_INVALID_PACKET_RECEIVED;
    }

    return ercd;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_F3(void)
{
    dns_err_t ercd;

    ercd = DNS_SUCCESS;

    if (NULL != dns_mng.cb_func)
    {
        dns_mng.cb_func(ercd, &name_table);
    }

    dns_mng.proc = DNS_PROC_UNRECEIVED_REQUEST;  /* proc <- A.unreceived request */

    return ercd;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_G3(void)
{
    dns_err_t ercd;

    ercd = DNS_SUCCESS;

    if (E_OK == udp_can_cep(dns_mng.cepid, TFN_UDP_ALL))
    {
        dns_mng.dns_timer = DNS_CANCEL_TIMEOUT_MS;
        dns_mng.proc = DNS_PROC_CANCELLING_WAIT; /* proc <- H.wait to complete cancelling */
    }
    else
    {
        dns_mng.proc = DNS_PROC_CANCELLED;  /* proc <- A.unreceived request */
        ercd = DNS_ERR_BAD_UDP_SETTINGS;
    }
    return ercd;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_H3(void)
{

    dns_timeout_timer();

    if (0 == dns_mng.dns_timer)
    {
        dns_mng.proc = DNS_PROC_CANCELLED;  /* proc <- I.cancelled */
        dns_mng.retry_cnt = 0;     /* no retry */
    }

    return DNS_ERR_TIMEOUT;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_H6(void)
{
    dns_mng.proc = DNS_PROC_CANCELLED;  /* proc <- I.cancelled */

    return DNS_ERR_TIMEOUT;
}

DNS_CLIENT_STATIC dns_err_t dns_proc_I3(void)
{
    dns_err_t ercd;

    ercd = DNS_SUCCESS;
#ifdef _UNIT_TEST_DNS
    flag_proc_I3 = 1;
#endif

    if (dns_mng.retry_cnt > 0)
    {
        dns_mng.retry_cnt--;
        dns_mng.proc = DNS_PROC_START_SENDING; /* proc <- C.start sending */
    }
    else
    {
        if(DNS_ERR_BAD_UDP_SETTINGS == dns_mng.dns_error_type)
        {
            if (NULL != dns_mng.cb_func)
            {
                dns_mng.cb_func(dns_mng.dns_error_type, NULL);
                dns_mng.proc = DNS_PROC_UNRECEIVED_REQUEST;
            }
        }
        else if(DNS_ERR_TIMEOUT == dns_mng.dns_error_type)  /*if(dns_mng.dns_error_type == DNS_ERR_BAD_UDP_SETTINGS)*/
        {
            if (DNS_SELECT_PRIMARY == dns_mng.dns_select)
            {
                dns_mng.dns_select = DNS_SELECT_SECONDARY;
                dns_mng.retry_cnt = DNS_ERROR_COUNT;
                dns_mng.proc = DNS_PROC_START_SENDING; /* proc <- C.start sending */
            }
            else
            {
                dns_mng.proc = DNS_PROC_UNRECEIVED_REQUEST;  /* proc <- A.unreceived request */

                if (NULL != dns_mng.cb_func)
                {
                    dns_mng.cb_func(dns_mng.dns_error_type, NULL);
                }
            }
        }
        else if(DNS_ERR_INVALID_PACKET_RECEIVED == dns_mng.dns_error_type)    /*if(dns_mng.dns_error_type == DNS_ERR_BAD_UDP_SETTINGS)*/
        {
            if (DNS_SELECT_PRIMARY == dns_mng.dns_select)
            {
                dns_mng.dns_select = DNS_SELECT_SECONDARY;
                dns_mng.retry_cnt = DNS_ERROR_COUNT;
                dns_mng.proc = DNS_PROC_START_SENDING; /* proc <- C.start sending */
            }
            else
            {
                dns_mng.proc = DNS_PROC_UNRECEIVED_REQUEST;  /* proc <- A.unreceived request */

                if (NULL != dns_mng.cb_func)
                {
                    dns_mng.cb_func(dns_mng.dns_error_type, NULL);
                }
            }
        }
        else
        {
          /* None */
        }
    }
    return ercd;
}

/***********************************************************************************************************************
* Function Name: send_dns_request
* Description  : sending DNS packet
* Arguments    : *name -
*                    pointer to name table
*                *dstaddr -
*                    pointer to address of destination
* Return Value : count
***********************************************************************************************************************/
DNS_CLIENT_STATIC int32_t send_dns_request(char *name, uint32_t dstaddr)
{
    static T_IPV4EP dns;
    int32_t ercd;

    dns.ipaddr = dstaddr;
    dns.portno = 53;

    dns_packet.header.ID       = htons(TRANSACTION_ID);
    dns_packet.header.Flag     = htons(0x0100);
    dns_packet.header.QDCOUNT  = htons(1);
    dns_packet.header.ANCOUNT  = htons(0);
    dns_packet.header.NSCOUNT  = htons(0);
    dns_packet.header.ARCOUNT  = htons(0);

    memcpy(dns_buffer, &dns_packet.header, sizeof(dns_packet.header));

    dns_packet.question.QNAME   = name;
    dns_packet.question.QTYPE   = htons(0x0001);
    dns_packet.question.QCLASS  = htons(0x0001);

    convert_name_to_dns_query((char*)&dns_buffer[sizeof(dns_packet.header)], name);

    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+1]  = 0;

#if defined(__BIG)
    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+2]  = ((dns_packet.question.QTYPE & 0xff00) >> 8);
    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+3]  = (dns_packet.question.QTYPE & 0x00ff);
    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+4]  = ((dns_packet.question.QCLASS & 0xff00) >> 8);
    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+5]  = (dns_packet.question.QCLASS & 0x00ff);
#else
    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+3]  = ((dns_packet.question.QTYPE & 0xff00) >> 8);
    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+2]  = (dns_packet.question.QTYPE & 0x00ff);
    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+5]  = ((dns_packet.question.QCLASS & 0xff00) >> 8);
    dns_buffer[((sizeof(dns_packet.header))+strlen((char*)name))+4]  = (dns_packet.question.QCLASS & 0x00ff);
#endif

    ercd = udp_snd_dat(dns_mng.cepid, &dns, dns_buffer, ((sizeof(dns_packet.header)) + strlen((char*)name)) + 6, TMO_NBLK);

    return ercd;

}

/***********************************************************************************************************************
* Function Name: receive_dns_response
* Description  : receiving DNS response
* Arguments    : *table -
*                    pointer to name table
* Return Value : count
***********************************************************************************************************************/
DNS_CLIENT_STATIC int32_t receive_dns_response(name_table_t *table)
{

    T_IPV4EP dstaddr;
    int32_t receive_data_len = 0;
    uint16_t answer_offset = 0;
    int32_t query_size = 0;
    uint8_t answer_count = 0;
    uint8_t answer_check_count = 0;
    uint16_t answer_size = 0;
    uint8_t query_type = 0;

    receive_data_len = udp_rcv_dat(1, &dstaddr, dns_buffer, 512 , TMO_POL);

    if (receive_data_len < 0)
    {
        return -1;
    }

    memcpy(&dns_packet.header, dns_buffer, sizeof(dns_packet.header));

    answer_count = answer_check_count = htons(dns_packet.header.ANCOUNT);
    if (answer_count > DNS_BUFFER_COUNT)
    {
        answer_count = answer_check_count = DNS_BUFFER_COUNT;
    }
    else if (0 == answer_count)
    {
        return -1;
    }
    else
    {

    }

    if (htons(TRANSACTION_ID++) != dns_packet.header.ID )
    {
        return -1;
    }

    if (!(dns_packet.header.Flag & htons(0x8000)))  // Response Bit(1) : Message is a response
    {
        return -1;
    }

    if (dns_packet.header.Flag & htons(0x7800))   // Op Code(0000) : Standard query
    {
        return -1;
    }

    if (dns_packet.header.Flag & htons(0x000f))   // Reply Code(0000) : No error
    {
        return -1;
    }

    query_size = check_query((char*) & dns_buffer[sizeof(dns_packet.header)]);
    if (query_size < 0)
    {
        return -1;
    }
    answer_offset = query_size + (sizeof(dns_packet.header));

    while (answer_check_count)
    {
        answer_size = check_answer((char*) & dns_buffer[answer_offset], (char*)dns_buffer, table, &query_type);
        answer_offset += answer_size;
        if (DNS_QUERY_TYPE_A == query_type)
        {
            answer_check_count--;
        }
    }

    return 0;
}

/***********************************************************************************************************************
* Function Name: convert_name_to_dns_query
* Description  : converting domain name to DNS query
* Arguments    : *query -
*                    pointer to the query
*                 *name -
*                    pointer to name table
* Return Value : count
***********************************************************************************************************************/
DNS_CLIENT_STATIC int32_t convert_name_to_dns_query(char *query, char *name)
{
    uint8_t count = 0;
    uint8_t loop_counter = 0;
    uint8_t label_cnt = 0;
    uint8_t name_len = strlen((char*)name);
    uint8_t charactor;

    memcpy(query + 1, name, strlen((char*)name));

    while (loop_counter < name_len)
    {
        charactor = *name;
        name++;
        if (charactor == '.')
        {
            *query = label_cnt;
            query += (label_cnt + 1);
            label_cnt = 0;
        }
        else
        {
            label_cnt++;
        }
        loop_counter++;
    }

    *query = label_cnt;

    return count;
}

/***********************************************************************************************************************
* Function Name: check_query
* Description  : Checking the query
* Arguments    : *query -
*                    pointer to the query
* Return Value : ((query + 5) - check)
***********************************************************************************************************************/
DNS_CLIENT_STATIC int32_t check_query(char *query)
{
    char (*check) = query;

    while (0x00 != (*query))
    {
        query += ((*query) + 1);
        if ((query - check) > 0xff)
        {
            return -1;
        }
    }
    return ((query + 5) - check);

}

/***********************************************************************************************************************
* Function Name: check_answer
* Description  : Checking the received answer from DNS server
* Arguments    : *answer -
*                    pointer to the received answer from DNS server
*                *dns_response -
*                    pointer to the received response from DNS server
*                *table -
*                    Name table
*                *query_type -
*                    type of query
* Return Value : answer_length
***********************************************************************************************************************/
DNS_CLIENT_STATIC int32_t check_answer(char *answer, char *dns_response, name_table_t *table, uint8_t *query_type)
{

    uint16_t src_offset = 0;
    uint16_t answer_length = 0;
    uint16_t query_length = 0;

    if (((*answer) & 0xc0) == 0xc0)
    {
        switch (answer[3])
        {

            case DNS_QUERY_TYPE_A:
                src_offset = ((*answer) & (~0xc0)) << 8;
                src_offset |= (*(answer + 1));

                query_length = get_name_till_stringend(&dns_response[src_offset], dns_response, table);

                memcpy(table->ipaddr, &answer[12], 4);
                answer_length = 16;
                *query_type = DNS_QUERY_TYPE_A;
                break;

            case DNS_QUERY_TYPE_CNAME:
                table->name[0] = 0;
                answer_length = answer[11] + 12;
                *query_type = DNS_QUERY_TYPE_CNAME;
                break;

            default:
                *query_type = DNS_QUERY_TYPE_UNKNOWN;
                break;
        }
    }
    else
    {
        query_length = get_name_till_stringend(answer, dns_response, table);

        switch (answer[query_length + 2])
        {
            case DNS_QUERY_TYPE_A:
                memcpy(table->ipaddr, &answer[query_length + 11], 4);
                answer_length = 15 + query_length;
                *query_type = DNS_QUERY_TYPE_A;
                break;

            case DNS_QUERY_TYPE_CNAME:
                table->name[0] = 0;
                answer_length = (answer[query_length + 10] + query_length) + 11;
                *query_type = DNS_QUERY_TYPE_CNAME;
                break;

            default:
                *query_type = DNS_QUERY_TYPE_UNKNOWN;
                break;
        }
    }
    return answer_length;
}

/***********************************************************************************************************************
* Function Name: get_name_till_stringend
* Description  :
* Arguments    :
* Return Value : src_offset
***********************************************************************************************************************/
DNS_CLIENT_STATIC int32_t get_name_till_stringend(const char *string, const char *dns_response, name_table_t *table)
{
    uint16_t dst_offset = 0;
    uint16_t src_offset = 0;
    uint16_t next_offset = 0;

    while (0x00 != string[src_offset])
    {
        if (strlen(table->name) > DNS_CFG_NAME_SIZE)
        {
            return -1;
        }
        if ((string[src_offset] & 0xc0) == 0xc0)
        {
            next_offset = (string[src_offset] & (~0xc0)) << 8;
            next_offset |= (string[src_offset + 1]);
            dst_offset += get_name_till_stringend(&dns_response[next_offset], dns_response, table);

            while ((string[src_offset] & 0xc0) == 0xc0)
            {
                src_offset += 2;
                if (src_offset > DNS_CFG_NAME_SIZE)
                {
                    return -1;
                }
            }
        }

        else
        {
            strncat((char*)table->name, (char*)&string[src_offset+1], string[src_offset]);
            dst_offset += (string[src_offset] + 1);
            src_offset += (string[src_offset] + 1);
            if (0x00 != string[src_offset])
            {
                strcat((char*)table->name, ".");
            }
        }
    }
    return src_offset;
}

/***********************************************************************************************************************
* Function Name: clear_table
* Description  : clear to 0 on name table
* Arguments    :
* Return Value : void
***********************************************************************************************************************/
DNS_CLIENT_STATIC void clear_table(name_table_t *table)
{
    memset(table, 0, sizeof(name_table_t));
}

/***********************************************************************************************************************
* Function Name: htons
* Description  : convert from little endian to big endian
* Arguments    :
* Return Value : data
***********************************************************************************************************************/
DNS_CLIENT_STATIC uint16_t htons(uint16_t data)
{

    uint16_t tmp = 0;
#ifndef __BIG
    tmp  = (data & 0x00ff) << 8;
    tmp  |= ((data & 0xff00) >> 8);
    data = tmp;
#endif
    return data;
}

/***********************************************************************************************************************
* Function Name: dns_timeout_timer
* Description  : counting timeout
* Arguments    :
* Return Value : void
***********************************************************************************************************************/
DNS_CLIENT_STATIC void  dns_timeout_timer(void)
{
    if(dns_mng.dns_timer > 0)
    {
        dns_mng.dns_timer--;
    }
}

/***********************************************************************************************************************
* Function Name: dns_reset_timer
* Description  : reset timeout timer
* Arguments    :
* Return Value : void
***********************************************************************************************************************/
DNS_CLIENT_STATIC void dns_reset_timer(void)
{
#ifndef _UNIT_TEST_DNS
    dns_mng.dns_timer = DNS_CFG_TIMEOUT * 100;
#else
    dns_mng.dns_timer = dns_test_timeout_set * 100;
#endif
}

/***********************************************************************************************************************
* Function Name: dns_client_process
* Description  : DNS client main process.
*                This function must be called periodic.
* Arguments    : none
* Return Value : error code
***********************************************************************************************************************/
dns_err_t dns_client_process(void)
{
    dns_err_t ercd;

    ercd = DNS_SUCCESS;
#ifdef _UNIT_TEST_DNS
    flag_sntp_process = 1;
#endif

    if (dns_mng.proc < DNS_PROC_NUM)
    {
        ercd = dns_proc_table[dns_mng.proc][DNS_EVENT_PERIODIC]();
    }
    else
    {
        /* System Error */
    }

    if (DNS_ERR_TIMEOUT == ercd)
    {
        dns_mng.dns_error_type = DNS_ERR_TIMEOUT;
    }
    else if (DNS_ERR_BAD_UDP_SETTINGS == ercd)
    {
        dns_mng.dns_error_type = DNS_ERR_BAD_UDP_SETTINGS;
    }
    else if (DNS_ERR_INVALID_PACKET_RECEIVED == ercd)
    {
        dns_mng.dns_error_type = DNS_ERR_INVALID_PACKET_RECEIVED;
    }
    else
    {
        /* None */
    }
    return DNS_SUCCESS;
}

/***********************************************************************************************************************
* Function Name: R_TCPIP_DnsClientOpen
* Description  : Initialize DNS management variables
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
dns_err_t R_TCPIP_DnsClientOpen(void)
{
    dns_err_t ercd;

    ercd = DNS_SUCCESS;

    if (false == R_SYS_TIME_IsPeriodicCallbackRegistered((callback_from_sys_time_t) &timer_interrupt))
    {
        return DNS_ERR_MODULE_NOT_REGISTERED;
    }
    dns_mng.dns_select = DNS_SELECT_PRIMARY;           /* selected dns server <- primary */
    dns_mng.cb_func = NULL;                            /* callback function <- NULL      */
    memset(&dns_mng.name[0], 0, sizeof(dns_mng.name)); /* domain name <- all '\0'        */
    dns_mng.cepid = 0;                                 /* invalid ID                     */
    dns_ipaddr.dnsaddr1 = 0;
    dns_ipaddr.dnsaddr2 = 0;
    dns_mng.cepid = DNS_CFG_UDP_CEP_ID;
    if (dns_mng.proc < DNS_PROC_NUM)
    {
        ercd = dns_proc_table[dns_mng.proc][DNS_EVENT_CALL_OPEN_FUNCTION]();
    }
    else
    {
        /* System Error!! */
    }
    R_SYS_TIME_RegisterPeriodicCallback((callback_from_sys_time_t)&dns_client_process, 1);

    return ercd;
}

/***********************************************************************************************************************
* Function Name: R_TCPIP_DnsClientRegisterServerAddress
* Description  : Setting the DNS server IP address
* Arguments    : dns_ip1 -
*                       DNS server IP address 1
*                dns_ip2 -
*                       DNS server IP address 2
* Return Value : error code
***********************************************************************************************************************/

dns_err_t R_TCPIP_DnsClientRegisterServerAddress(uint32_t dns_ipaddr1, uint32_t dns_ipaddr2)
{
    if(DNS_PROC_REST == dns_mng.proc)
    {
        return DNS_ERR_NOT_OPENED;
    }

    if ((0 != dns_ipaddr1) && (0 != dns_ipaddr2))
    {
        dns_mng.dns_select_sub = DNS_SELECT_PRIMARY;           /* selected dns server <- primary */
        dns_ipaddr_sub.dnsaddr1 = dns_ipaddr1;
        dns_ipaddr_sub.dnsaddr2 = dns_ipaddr2;
    }
    else if ((0 != dns_ipaddr1) && (0 == dns_ipaddr2))
    {
        dns_mng.dns_select_sub = DNS_SELECT_SECONDARY;           /* selected dns server <- secondary */
        dns_ipaddr_sub.dnsaddr2 = dns_ipaddr1;
    }
    else if (0 == dns_ipaddr1)
    {
#ifdef _UNIT_TEST_DNS
        dns_ipaddr_sub.dnsaddr1 = dns_ipaddr1;
        dns_ipaddr_sub.dnsaddr2 = dns_ipaddr2;
#endif
        return DNS_ERR_SERVER_NOT_REGISTERED;
    }
    else
    {
        /* None */
    }

    return DNS_SUCCESS;
}

/***********************************************************************************************************************
* Function Name: R_TCPIP_DnsClientResolveName
* Description  : Setting the domain name you want to resolve
* Arguments    : name -
*                    address of domain name
*                func -
*                    callback function
* Return Value : error code
***********************************************************************************************************************/
dns_err_t R_TCPIP_DnsClientResolveName(char *name, DNS_CB_FUNC func)
{
    dns_err_t ercd;

    ercd = DNS_SUCCESS;

    if ((0 == dns_ipaddr_sub.dnsaddr1) && (0 == dns_ipaddr_sub.dnsaddr2))
    {
        return DNS_ERR_SERVER_NOT_REGISTERED;
    }

    if (DNS_PROC_UNRECEIVED_REQUEST == dns_mng.proc)
    {
#ifndef _UNIT_TEST_DNS
        dns_mng.cepid = DNS_CFG_UDP_CEP_ID;
#endif
        dns_mng.dns_select = dns_mng.dns_select_sub;
        memcpy(&dns_ipaddr, &dns_ipaddr_sub, sizeof(dns_ipaddr_sub));

        strncpy(&dns_mng.name[0], name, sizeof(dns_mng.name));
        dns_mng.cb_func = func;
        dns_mng.retry_cnt = DNS_ERROR_COUNT;                          /* Initialize retry counter       */

        if (dns_mng.proc < DNS_PROC_NUM)
        {
            ercd = dns_proc_table[dns_mng.proc][DNS_EVENT_RECEIVED_REQUEST]();
        }
        else
        {
            /* System Error!! */
        }
    }
    else if (DNS_PROC_REST == dns_mng.proc)
    {
        ercd = DNS_ERR_NOT_OPENED;
    }
    else
    {
        ercd = DNS_ERR_PROCESSING;
    }
    return ercd;
}


/***********************************************************************************************************************
* Function Name: R_TCPIP_DnsClientCallback
* Description  : DNS callback function
* Arguments    : cepid -
*                    the communication endpoint ID
*                fncd -
*                    function code
*                p_parblk -
*                    error code pointer
* Return Value : error code
***********************************************************************************************************************/
ER R_TCPIP_DnsClientCallback(ID cepid, FN fncd , VP p_parblk)
{
    ER parblk;
    ER ercd;

    parblk = *(ER *)p_parblk;
    ercd   = DNS_SUCCESS;

    /* cepid is 1 fixed */
    if (dns_mng.cepid != cepid)
    {
        /* System Error!! */
    }

    if (DNS_PROC_NUM < dns_mng.proc)
    {
        /* System Error!! */
    }

    switch (fncd)
    {
        case TFN_UDP_SND_DAT:
            if (0 < parblk)    /* completed sending */
            {
                ercd = dns_proc_table[dns_mng.proc][DNS_EVENT_CALLBACK_SEND]();
            }
            else if (E_CLS == parblk) /* send ARP timeout */
            {
                ercd = dns_proc_table[dns_mng.proc][DNS_EVENT_CALLBACK_SEND_ARP_TIMEOUT]();
            }
            else if (E_RLWAI == parblk)
            {
                ercd = dns_proc_table[dns_mng.proc][DNS_EVENT_CALLBACK_CANCEL]();
            }
            else
            {
                /* None */
            }
            break;

        case TEV_UDP_RCV_DAT:
            if (0 < parblk)    /* completed receiving */
            {
                ercd = dns_proc_table[dns_mng.proc][DNS_EVENT_CALLBACK_RECEIVE]();
            }
            break;

        default:
            /* None */
            break;
    }
    return ercd;
}

/***********************************************************************************************************************
* Function Name: R_TCPIP_DnsClientClose
* Description  : DNS close function
* Arguments    : None
* Return Value : error code
***********************************************************************************************************************/

dns_err_t  R_TCPIP_DnsClientClose(void)
{
    dns_err_t ercd;

    ercd = dns_proc_table[dns_mng.proc][DNS_EVENT_CALL_CLOSE_FUNCTION]();

    return ercd;
}

/*****************************************************************************
* Function Name: R_TCPIP_DnsClientGetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#pragma inline(R_TCPIP_DnsClientGetVersion)
uint32_t  R_TCPIP_DnsClientGetVersion(void)
{
    uint32_t const version = (T4_DNS_CLIENT_VERSION_MAJOR << 16) | T4_DNS_CLIENT_VERSION_MINOR;

    return version;
}
