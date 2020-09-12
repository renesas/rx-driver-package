/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011-2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_ftp_server.c
* Version      : --.--
* Device(s)    :
* Tool-Chain   :
* OS           : none
* H/W Platform :
* Description  : FTP server code.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.03.2011 1.00    First Release
*         : 16.04.2013 1.01    Changed sys_time to none static variable.
*         : 09.05.2014 1.02    Corresponded to FIT Modules
*         : 01.07.2014 1.03    Clean up source code
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "r_t4_itcpip.h"
#include "r_t4_file_driver_rx_if.h"
#include "r_t4_ftp_server_rx_config.h"
#include "r_t4_ftp_server_rx_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define CONST const

/* status */
#define FTP_CLOSED         0
#define FTP_CLOSING        1
#define FTP_ACCEPT         2
#define FTP_CONNECTION     3
#define FTP_PASS_WORD      4
#define FTP_LOGIN          5

/* exec_command */
#define FTP_DATA_NONE     0
#define FTP_DATA_LIST     1
#define FTP_DATA_STOR     2
#define FTP_DATA_RETR     3
#define FTP_DATA_ABOR     4
#define FTP_DATA_PORT     6

/* exec_command_subseq */
#define FTP_DATA_LIST_SUBSEQ04   4
#define FTP_DATA_LIST_SUBSEQ08   8
#define FTP_DATA_LIST_SUBSEQ12   12
#define FTP_DATA_LIST_SUBSEQ16   16

#define FTP_DATA_STOR_SUBSEQ04   4

#define FTP_DATA_RETR_SUBSEQ04   4
#define FTP_DATA_RETR_SUBSEQ08   8
#define FTP_DATA_RETR_SUBSEQ12   12
#define FTP_DATA_RETR_SUBSEQ16   16

#define FTP_DATA_ABOR_SUBSEQ04   4
#define FTP_DATA_ABOR_SUBSEQ08   8


#define FTP_DATA_SUBSEQ_ERR  (-1)


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct cep_
{
    uint8_t  status;        /* Connection state of the endpoint         */
    uint8_t  *buff_ptr;     /* Top address of the transmission and reception buffer    */
    int32_t  remain_data;   /* remainder transmission and reception number of bytes    */
    int32_t  now_data;      /* The number of bytes finished with transmission and reception  */
    T_IPV4EP dstaddr;       /* Client IP address            */
    T_IPV4EP myaddr;        /* Server IP address            */
    uint8_t  api_cancel;    /* API cancel request flag            */
} FTP_CEP;


typedef struct
{
    uint8_t   cmd_buff[CMD_BUF_SIZE];         /* transmission and reception buffer of the command endpoint    */
    uint8_t   data_buff[DATA_BUF_SIZE];       /* transmission and reception buffer of the data endpoint      */
    int8_t    current_path[PATH_NAME_SIZE];   /* Directory management              */
    int16_t   valid_dstaddr;                  /* PORT command receptionist            */
    uint8_t   trans_mode;                     /* ASCII or BINARY               */
    uint8_t   rnfr;                           /* RNFR command receptionist            */
    int8_t    fname[PATH_NAME_SIZE];          /* File name                */
    int32_t   file_index;                     /* A file maintenance structure body index number       */
    int16_t   exec_command;                   /* The data transmission classification that executing      */
    int16_t   exec_command_subseq;            /* A subsequence of data transmission          */
    int16_t   user_id;                        /* The number of the USER ID management table        */
    uint8_t   read_crlf_check;                /* character on the way at the time of the file reading with ASCII mode  */
    uint8_t   cep_reset_req;                  /* ftp_cep reset flag               */
    int32_t   dir_read_index;                 /* The reading position of the directory          */
    FILE_LIST file_list[FTP_MAX_FILE_LIST];   /* A list of files to generate in NLST,LIST command       */
} _FTP_STAT;

/******************************************************************************
Private global variables and functions
******************************************************************************/
static const uint8_t user_data[MAX_USER][16] = {USER_LIST};
static const uint8_t pass_data[MAX_USER][16] = {PASS_LIST};
static const uint8_t root_dir[MAX_USER][16]  = {ROOT_DIR_LIST};

static ER ftp_rcv_request(ID cepid, uint8_t *buf, int16_t buf_len, ER rcv_dat_size);
static ER strcat_with_check_len(uint8_t *src, uint8_t *concat, int32_t size);

static int16_t cmdline(int8_t *buf, int16_t *argc, int8_t **argv);

static int16_t ftps_retr(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_port(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_pasv(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_user(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_pass(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_type(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_quit(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_noop(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_stor(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_list_nlst(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_rnto(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_rnfr(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_dele(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_cwd(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_pwd(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_mkd(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_rmd(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_cdup(int16_t argc, int8_t **argv, ID cepid);
static int16_t ftps_abor(int16_t argc, int8_t **argv, ID cepid);
static int16_t non_support_cmd(ID cepid, int8_t *argv);
static int16_t ftps_list_nlst_dataport(ID cepid);
static int16_t ftps_list_nlst_finish(ID cepid);
static int16_t ftps_stor_dataport(ID cepid, int32_t rcv_len);
static int16_t ftps_stor_finish(ID cepid);
static int16_t ftps_retr_dataport(ID cepid);
static int16_t ftps_retr_finish(ID cepid);
static void ftps_abor_finish(ID cepid);
static int32_t stricmp_rx(const uint8_t *s1, const uint8_t *s2, int32_t count);
static int32_t path_concatenate(ID cepid , uint8_t *buf, uint32_t buf_len, uint8_t *path);

ER ftp_callback(ID cepid, FN fncd , VP p_parblk);
ER ftp_data_callback(ID cepid, FN fncd , VP p_parblk);

//SYS_TIME sys_time;

/*------------ GLOBAL -----------*/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
FTP_CEP  ftp_cep[FTP_TCP_CEP_NUM];
_FTP_STAT ftp_stat[FTP_TCP_CEP_NUM/2];

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern char _T4_Version[];
//extern const H __tcpcepn;
//extern T_TCP_CCEP tcp_ccep[];
extern T_TCP_CREP tcp_crep[];
//extern const H __tcprepn;
extern TCPUDP_ENV tcpudp_env;

uint8_t far *ftps_cmd_tbl[]
= { "STOR", "RETR", "PASV", "ABOR",
    "USER", "PASS", "TYPE", "PWD", "XPWD", "CWD", "CDUP", "QUIT", "DELE",
    "PORT", "LIST", "NLST", "MKD", "XMKD",
    "RMD", "XRMD",  "RNFR", "RNTO", "NOOP",
    NULL
  };
int16_t (*ftps_cmd_exe[])(int16_t, int8_t **, ID cepid)
= { ftps_stor, ftps_retr, ftps_pasv, ftps_abor,
    ftps_user, ftps_pass, ftps_type, ftps_pwd, ftps_pwd, ftps_cwd, ftps_cdup, ftps_quit, ftps_dele,
    ftps_port, ftps_list_nlst, ftps_list_nlst, ftps_mkd, ftps_mkd,
    ftps_rmd, ftps_rmd, ftps_rnfr, ftps_rnto, ftps_noop,
  };

/*------------ PROCESS -----------*/

/******************************************************************************
* Function Name : R_ftp_srv_open
* Description   : Start the FTP server.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_ftp_srv_open(void)
{
    int16_t loop;
    int32_t ip_work;

    memset(ftp_cep, 0, sizeof(ftp_cep));
    memset(ftp_stat, 0, sizeof(ftp_stat));

    ip_work = ((uint32_t)tcpudp_env.ipaddr[0] << 24) + ((uint32_t)tcpudp_env.ipaddr[1] << 16) + ((uint32_t)tcpudp_env.ipaddr[2] << 8) + (uint32_t)tcpudp_env.ipaddr[3];

    for (loop = 0; loop < FTP_TCP_CEP_NUM; loop++)
    {
        ftp_cep[loop].myaddr.ipaddr = ip_work;
        if ((loop % 2) == 1)
        {
            ftp_cep[loop].buff_ptr = ftp_stat[loop/2].data_buff;
            ftp_cep[loop].myaddr.portno = 20;  /* Port number */
        }
        else
        {
            ftp_cep[loop].buff_ptr = ftp_stat[loop/2].cmd_buff;
            ftp_cep[loop].myaddr.portno = 21;  /* Port number */
        }
    }
    return;
}


/******************************************************************************
* Function Name : R_ftpd
* Description   : Check the FTP server status.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_ftpd(void)
{
    ER   ercd;
    int32_t i;
    int32_t ip_work;

    if (0 != (FTP_START_TCP_CEP % 2))
    {
        while (1); /* System Error!! */
    }

    /* checks the state of all communication endpoints */
    for (i = 0; i < FTP_TCP_CEP_NUM; i += 2)
    {
        switch (ftp_cep[i].status)
        {
            case FTP_CLOSED:
                /* accept */
                ercd = tcp_acp_cep(i + FTP_START_TCP_CEP + 1, i + FTP_START_TCP_CEP + 1, &(ftp_cep[i].dstaddr), TMO_NBLK);
                if (ercd != E_WBLK)
                {
                    while (1);
                }
                ftp_cep[i].status = FTP_ACCEPT;
                break;
            default:
                break;
        }
    }

    for (i = 0;i < FTP_TCP_CEP_NUM / 2;i++)
    {
        if (ftp_stat[i].cep_reset_req > 0)
        {
            ftp_cep[i*2].api_cancel = 0;
            ftp_cep[i*2+1].api_cancel = 0;
            switch (ftp_stat[i].cep_reset_req & 0xf0)
            {
                case 0x10:
                    ftp_stat[i].cep_reset_req = 0x20;
                    ercd = tcp_can_cep(i * 2 + FTP_START_TCP_CEP + 2, TFN_TCP_ALL);
                    if (ercd == E_OK)
                    {
                    }
                    else
                    {
                        tcp_cls_cep(i*2 + FTP_START_TCP_CEP + 2, 1);
                        ftp_stat[i].cep_reset_req |= 0x03;
                    }
                    ercd = tcp_can_cep(i * 2 + FTP_START_TCP_CEP + 1, TFN_TCP_ALL);
                    if (ercd == E_OK)
                    {
                    }
                    else
                    {
                        tcp_cls_cep(i*2 + FTP_START_TCP_CEP + 1, 1);
                        ftp_stat[i].cep_reset_req |= 0x0c;
                    }
                    /* not break */
                case 0x20:
                    if ((ftp_stat[i].cep_reset_req & 0x01) == 0x01)
                    {
                        if ((ftp_stat[i].cep_reset_req & 0x02) != 0x02)
                        {
                            tcp_cls_cep(i*2 + FTP_START_TCP_CEP + 2, 1);
                            ftp_stat[i].cep_reset_req |= 0x02;
                        }
                    }
                    if ((ftp_stat[i].cep_reset_req & 0x04) == 0x04)
                    {
                        if ((ftp_stat[i].cep_reset_req & 0x08) != 0x08)
                        {
                            tcp_cls_cep(i*2 + FTP_START_TCP_CEP + 1, 1);
                            ftp_stat[i].cep_reset_req |= 0x08;
                        }
                    }
                    if ((ftp_stat[i].cep_reset_req & 0x0f) == 0x0f)
                    {
                        file_close(ftp_stat[i].file_index);
                        memset(&ftp_cep[i*2] , 0 , sizeof(FTP_CEP));
                        memset(&ftp_cep[i*2+1] , 0 , sizeof(FTP_CEP));
                        memset(&ftp_stat[i], 0 , sizeof(_FTP_STAT));
                        ip_work = ((uint32_t)tcpudp_env.ipaddr[0] << 24) + ((uint32_t)tcpudp_env.ipaddr[1] << 16) + ((uint32_t)tcpudp_env.ipaddr[2] << 8) + (uint32_t)tcpudp_env.ipaddr[3];
                        ftp_cep[i*2].myaddr.ipaddr = ip_work;
                        ftp_cep[i*2].buff_ptr = ftp_stat[i].cmd_buff;
                        ftp_cep[i*2].myaddr.portno = 21;  /* Port number */
                        ftp_cep[i*2+1].myaddr.ipaddr = ip_work;
                        ftp_cep[i*2+1].buff_ptr = ftp_stat[i].data_buff;
                        ftp_cep[i*2+1].myaddr.portno = 20;  /* Port number */
                        ftp_stat[i].cep_reset_req = 0;
                    }
                    break;
            }

        }
    }
    for (i = 0; i < FTP_TCP_CEP_NUM; i++)
    {
        if (ftp_cep[i].api_cancel == 1)
        {
            ercd = tcp_can_cep(i + FTP_START_TCP_CEP + 1, TFN_TCP_ALL);
            ftp_cep[i].api_cancel = 2;
        }
    }
}

/******************************************************************************
* Function Name : R_ftp_srv_close
* Description   : Close the FTP server.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_ftp_srv_close(void)
{
    return;
}



static ER ftp_rcv_request(ID cepid, uint8_t *buf, int16_t buf_len, ER rcv_dat_size)
{

    uint8_t *ptr;

    if (rcv_dat_size <= 0)
    {
        ftp_cep[cepid-1].remain_data = 0;
        return -1;
    }

    ftp_cep[cepid-1].remain_data += rcv_dat_size;

    if (ftp_cep[cepid-1].remain_data > (buf_len - 1))
    {
        /* full of buffer */
        ftp_cep[cepid-1].remain_data = 0;
        return -1;
    }
    buf[ftp_cep[cepid-1].remain_data] = '\0';

    ptr = (uint8_t *)strstr((char*)buf, "\r\n");
    if (ptr != 0)
    {
        *(ptr + 2) = '\0';
        ftp_cep[cepid-1].remain_data = 0;
        return 0;
    }

    /* continue receiving */
    return 1;
}



static ER strcat_with_check_len(uint8_t *src, uint8_t *concat, int32_t size)
{
    if (strlen((const char *)src) + strlen((const char *)concat) >= size)
    {
        return -1;
    }
    strcat((char *)src, (const char *)concat);
    return 0;
}



static int16_t cmdline(int8_t *buf, int16_t *argc, int8_t **argv)
{
    uint8_t *p;
    int16_t  cnt = 0;

    p = (uint8_t *)buf;

    while (1)
    {
        if (cnt == 0)
        {
            /*  find command head : skip space(0x20) and skip excluding ascii code (ex. abort sequence) until finding command head */
            while (*p >= 0x80 || (isspace(*p) != 0))
            {
                p++;
            }
        }
        else
        {
            /* find parameter head : skip space(0x20) */
            while (isspace(*p) != 0)
            {
                p++;
            }
        }

        if (*p == '\0')
        {
            /* command fixes (ex : [command],[sp],[null]) */
            break;
        }
        argv[cnt++] = (int8_t *)p;

        while ((isspace(*p) == 0) && (*p != '\0'))
        {
            /* find command or parameter end */
            p++;
        }

        if (*p == '\0')
        {
            /* command fixes (ex : [command],[sp],[parameter],[null]) */
            break;
        }
        else
        {
            /* contine searching */
            *p++ = '\0';
        }

        if (cnt >= MAX_ARGC)
        {
            /* check parameter number limitation */
            break;
        }
    }

    *argc = cnt;
    return (cnt);
}


static int16_t ftps_user(int16_t argc, int8_t **argv, ID cepid)
{
    uint8_t username[USER_NAME_SIZE+1];
    int8_t loop;

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (argc != 2)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }

    strcpy((char *)username, (const char *)argv[1]);

    for (loop = 0; loop < MAX_USER; loop++)
    {
        if (strcmp((char*)user_data[loop], (char*)(argv[1])) == 0)
        {
            ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PASSWD_REQ, (char*)(username));
            ftp_stat[(cepid-1)/2].user_id = loop;
            ftp_cep[cepid-1].status = FTP_PASS_WORD;
            break;
        }
    }
    if (loop >= MAX_USER)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }
    return (0);
}



static int16_t ftps_pass(int16_t argc, int8_t **argv, ID cepid)
{
    int16_t ret = -1;

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status != FTP_PASS_WORD)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_CMD_BAD_SEQ) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_CMD_BAD_SEQ);
        ftp_cep[cepid-1].status = FTP_CONNECTION;
    }
    else if (argc != 2)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
    }
    else
    {
        if (strcmp((char*)pass_data[ftp_stat[(cepid-1)/2].user_id], (char*)(argv[1])) == 0)
        {
            ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_AUTHENTICATION_OK, ((char*)user_data[ftp_stat[(cepid-1)/2].user_id]));
            strcpy((char *)(ftp_stat[(cepid-1)/2].current_path), (const char *)root_dir[ftp_stat[(cepid-1)/2].user_id]);
            ftp_cep[cepid-1].status = FTP_LOGIN;
            ftp_stat[(cepid-1)/2].trans_mode = 'A';
            ret = 0;
        }
        else
        {
            ftp_cep[cepid-1].remain_data = sizeof(MSG_AUTHENTICATION_NG) - 1;
            strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_AUTHENTICATION_NG);
            ftp_cep[cepid-1].status = FTP_CONNECTION;
        }
    }
    return (ret);
}


static int16_t ftps_pwd(int16_t argc, int8_t **argv, ID cepid)
{
    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }
    ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PWD_RESPONSE, ftp_stat[(cepid-1)/2].current_path);
    return (0);
}



static int16_t ftps_type(int16_t argc, int8_t **argv, ID cepid)
{

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }

    if (argc == 2)
    {
        if (strcmp((char *)argv[1], "A") == 0 || strcmp((char *)argv[1], "I") == 0)
        {
            ftp_stat[(cepid-1)/2].trans_mode = argv[1][0];
            ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_TYPE_SET, ftp_stat[(cepid-1)/2].trans_mode);
            return (0);
        }
    }

    ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
    strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
    return (-1);

}


static int16_t ftps_list_nlst(int16_t argc, int8_t **argv, ID cepid)
{
    int16_t ret;
    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    pftpstat->rnfr = 0;
    if (pcep->status < FTP_LOGIN)
    {
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_NOT_AVAILABLE);
        pcep->status = FTP_CLOSING;
        return (-1);
    }

    pftpstat->exec_command = FTP_DATA_LIST;
    pftpstat->exec_command_subseq = FTP_DATA_LIST_SUBSEQ04;

    if (pftpstat->trans_mode == 'A')
    {
        pftpstat->dir_read_index = 0;
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_OPEN_DATA_CON, "ASCII");
        ret = 0;
    }
    else
    {
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_INVALID_PATH);
        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
        ret = -1;
    }
    pcep->remain_data = strlen((const char *)pcep->buff_ptr);

    return (ret);
}


static int16_t ftps_list_nlst_dataport(ID cepid)
{
    uint8_t *p;
    int32_t ret;
    int32_t count;
    int32_t total_count;
    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;
    uint8_t tmp_buf[512];
    uint8_t tmp_buf2[12];
    ER ercd = 0;

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    tmp_buf[0] = 0;

    pftpstat->exec_command_subseq = FTP_DATA_LIST_SUBSEQ08;

    p = pcep->buff_ptr;

    pcep->buff_ptr[0] = 0;

    total_count = 0;
    while (ercd == 0)
    {
        /* It retrieves file information for the number of FTP_MAX_FILE_LIST */
        ret = get_file_list_info((uint8_t *)pftpstat->current_path, pftpstat->file_list, FTP_MAX_FILE_LIST, pftpstat->dir_read_index);

        if (ret < 0)
        {
            pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
            return -1;
        }

        for (count = 0; count < ret; count++, total_count++)
        {
            if ((pftpstat->file_list[count].file_attr & FILE_ATTR_DIR) == FILE_ATTR_DIR)
            {
                strcpy((char *)tmp_buf, "drwxrwxrwx 1 root default          0 ");
            }
            else
            {
                strcpy((char *)tmp_buf, "-rwxrwxrwx 1 root default ");
                sprintf((char *)tmp_buf2, "%10ld ", pftpstat->file_list[count].file_size);
                strcat((char *)tmp_buf, (const char *)tmp_buf2);
            }
            ercd += strcat_with_check_len(pcep->buff_ptr, tmp_buf, DATA_BUF_SIZE);

            /* Last modified */
            sprintf((char *)tmp_buf, "%s %02d %02d:%02d: ",
                    pftpstat->file_list[count].date_info.month,
                    pftpstat->file_list[count].date_info.day,
                    pftpstat->file_list[count].date_info.hour,
                    pftpstat->file_list[count].date_info.min
                   );
            ercd += strcat_with_check_len(pcep->buff_ptr, tmp_buf, DATA_BUF_SIZE);

            strcpy((char *)tmp_buf, (char *)pftpstat->file_list[count].file_name);
            strcat((char *)tmp_buf, "\r\n");

            ercd += strcat_with_check_len(pcep->buff_ptr, tmp_buf, DATA_BUF_SIZE);
            if (ercd == 0)
            {
                /* There is a space in the transmission of a message buffer. NULL insertion point update */
                p = &pcep->buff_ptr[strlen((char*)pcep->buff_ptr)];
            }
            else
            {
                /* There is no a space in the transmission of a message buffer. NULL insert */
                *p = '\0';
                break;
            }
        }
        pftpstat->dir_read_index += count;
        if (ret < FTP_MAX_FILE_LIST)
        {
            /* Finish reading file information */
            break;
        }
    }
    if (count >= ret)
    {
        if (ret < FTP_MAX_FILE_LIST)
        {
            /* Finish reading file information */
            pftpstat->exec_command_subseq = FTP_DATA_LIST_SUBSEQ12;
        }
        else
        {
            /* continue reading */
            *p = '\0';
        }
    }
    else
    {
        /* continue reading */
        *p = '\0';
    }
    pcep->remain_data = strlen((const char *)pcep->buff_ptr);

    return total_count;
}


static int16_t ftps_list_nlst_finish(ID cepid)
{
    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    if (pftpstat->exec_command_subseq != FTP_DATA_LIST_SUBSEQ16)
    {
        pcep->remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_EXEC_ERROR);
    }
    else
    {
        pcep->remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_TRNS_CMPLT);
    }

    pftpstat->exec_command = FTP_DATA_NONE;
    pftpstat->exec_command_subseq = 0;

    pcep->remain_data = strlen((const char *)pcep->buff_ptr);

    return (0);
}


static int16_t ftps_port(int16_t argc, int8_t **argv, ID cepid)
{
    int16_t   ip0, ip1, ip2, ip3, port0, port1;
    FTP_CEP   *cmd_pcep;
    FTP_CEP   *data_pcep;
    _FTP_STAT *pftpstat;

    cmd_pcep = (&ftp_cep[cepid-1]);
    data_pcep = (&ftp_cep[cepid]);
    pftpstat = (&ftp_stat[(cepid-1)/2]);

    pftpstat->rnfr = 0;
    if (cmd_pcep->status < FTP_LOGIN)
    {
        cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_NOT_AVAILABLE);
        cmd_pcep->status = FTP_CLOSING;
        return (-1);
    }

    if (argc != 2)
    {
        cmd_pcep->remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(cmd_pcep->buff_ptr), MSG_PARAM_ERROR);
        return (-1);
    }

    sscanf((char*)argv[1], "%hd,%hd,%hd,%hd,%hd,%hd", &ip0, &ip1, &ip2, &ip3, &port0, &port1);
    data_pcep->dstaddr.ipaddr = ((uint32_t)ip0 << 24) + ((uint32_t)ip1 << 16)
                                + ((uint32_t)ip2 << 8) + (uint32_t)ip3;
    data_pcep->dstaddr.portno = port0 * 256 + port1;  /* Port number */

    data_pcep->myaddr.portno = 20;  /* Port number */

    pftpstat->valid_dstaddr = 1;

    /* The making of the command normal end message */
    cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_CMD_ACCEPTED, "PORT");
    pftpstat->exec_command = FTP_DATA_PORT;
    return (0);

}


static int16_t ftps_pasv(int16_t argc, int8_t **argv, ID cepid)
{
    uint16_t   ip0, ip1, ip2, ip3, port0, port1;
    uint32_t  ip_work, port_work;
    ER      ercd;
    FTP_CEP   *cmd_pcep;
    FTP_CEP   *data_pcep;
    _FTP_STAT *pftpstat;

    if (0 != (FTP_START_TCP_CEP % 2))
    {
        while (1); /* System Error!! */
    }

    cmd_pcep = (&ftp_cep[cepid-1]);
    data_pcep = (&ftp_cep[cepid]);
    pftpstat = (&ftp_stat[(cepid-1)/2]);

    pftpstat->rnfr = 0;
    if (cmd_pcep->status < FTP_LOGIN)
    {
        cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_NOT_AVAILABLE);
        cmd_pcep->status = FTP_CLOSING;
        return (-1);
    }

    ip_work = cmd_pcep->myaddr.ipaddr;
    data_pcep->myaddr.portno = (1024 + cepid + FTP_START_TCP_CEP);
    tcp_crep[cepid + FTP_START_TCP_CEP].myaddr.portno = (1024 + cepid + FTP_START_TCP_CEP);
    port_work = data_pcep->myaddr.portno;

    ip0 = (uint16_t)((ip_work >> 24) % 256);
    ip1 = (uint16_t)((ip_work >> 16) % 256);
    ip2 = (uint16_t)((ip_work >> 8) % 256);
    ip3 = (uint16_t)(ip_work % 256);
    port0 = (uint16_t)(port_work / 256);
    port1 = (uint16_t)(port_work % 256);

    /* generate normal end message */
    cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_PASV_RES, ip0, ip1, ip2, ip3, port0, port1);
    ercd = tcp_acp_cep(cepid + FTP_START_TCP_CEP + 1, cepid + FTP_START_TCP_CEP + 1, &(data_pcep->dstaddr), TMO_NBLK);
    if (ercd != E_WBLK)
    {
        return (-1);
    }

    return (0);

}



static int16_t ftps_quit(int16_t argc, int8_t **argv, ID cepid)
{

    ftp_stat[(cepid-1)/2].rnfr = 0;
    ftp_cep[cepid-1].remain_data = sizeof(MSG_GOODBY) - 1;
    strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_GOODBY);

    return (0);
}


static int16_t ftps_noop(int16_t argc, int8_t **argv, ID cepid)
{
    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }

    /* generate normal end message */
    ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_CMD_SUCCESS, "NOOP");

    return (0);
}


static int16_t ftps_retr(int16_t argc, int8_t **argv, ID cepid)
{

    int32_t file_index;
    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;
    uint8_t  tmp_buf[512];

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    pftpstat->rnfr = 0;
    if (pcep->status < FTP_LOGIN)
    {
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_NOT_AVAILABLE);
        pcep->status = FTP_CLOSING;
        return (-1);
    }

    pftpstat->exec_command = FTP_DATA_RETR;
    pftpstat->exec_command_subseq = FTP_DATA_RETR_SUBSEQ04;
    pftpstat->read_crlf_check = 0;

    if (argc != 2)
    {
        pcep->remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(pcep->buff_ptr), MSG_PARAM_ERROR);
        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
        return -1;
    }

    if (0 != path_concatenate(cepid , tmp_buf, sizeof(tmp_buf), (uint8_t *)argv[1]))
    {
        pcep->remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(pcep->buff_ptr), MSG_EXEC_ERROR);
        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
        return -1;
    }

    file_index = file_open((uint8_t *)tmp_buf, FILE_READ);
    if (file_index < 0)
    {
        pcep->remain_data = sizeof(MSG_NO_SUCH_FILE) - 1;
        strcpy((char *)(pcep->buff_ptr), MSG_NO_SUCH_FILE);
        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
        return -1;
    }
    else
    {
        pftpstat->file_index = file_index;
        if (pftpstat->trans_mode == 'A')
        {
            pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_OPEN_DATA_CON, "ASCII");
        }
        else
        {
            pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_OPEN_DATA_CON, "IMAGE");
        }
    }

    return (0);
}


static int16_t ftps_retr_dataport(ID cepid)
{
    int32_t i, sizeret;
    int32_t flag;
    int32_t readsize;

    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    pftpstat->exec_command_subseq = FTP_DATA_RETR_SUBSEQ08;

    if (pftpstat->trans_mode == 'A')
    {
        /* ASCII mode */
        readsize = file_read(pftpstat->file_index, pcep->buff_ptr + DATA_BUF_SIZE / 2, DATA_BUF_SIZE / 2);
        if (readsize < 0)
        {
            file_close(pftpstat->file_index);
            pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
            return -1;
        }

        if (readsize == 0)
        {
            flag = pftpstat->read_crlf_check;
            if (flag == 1)
            {
                /* flag == ON : last one character of data read ahead == 0x0d */
                sizeret = 1;
                pcep->buff_ptr[0] = 0x0a;
                flag = 0;
            }
            else
            {
                /* flag == OFF : last one character of data read ahead != 0x0d */
                file_close(pftpstat->file_index);
                pftpstat->exec_command_subseq = FTP_DATA_RETR_SUBSEQ12;
                sizeret = 0;
            }
        }
        else
        {

            flag =  pftpstat->read_crlf_check;
            for (i = 0, sizeret = 0; i < readsize; i++)
            {
                switch (pcep->buff_ptr[DATA_BUF_SIZE/2 + i])
                {
                    case 0x0d:
                        if (flag == 1)
                        {
                            /* ... 0x0d, 0x0d */
                            pcep->buff_ptr[sizeret++] = 0x0a;
                        }
                        flag = 1;
                        break;
                    case 0x0a:
                        if (flag != 1)
                        {
                            /* ...  not 0x0d, 0x0a */
                            pcep->buff_ptr[sizeret++] = 0x0d;
                        }
                        flag = 0;
                        break;
                    default:
                        if (flag == 1)
                        {
                            /* ... 0x0d, 0xXX */
                            pcep->buff_ptr[sizeret++] = 0x0a;
                        }
                        flag = 0;
                        break;
                }
                pcep->buff_ptr[sizeret++] = pcep->buff_ptr[DATA_BUF_SIZE/2 + i];
            }
        }
        pftpstat->read_crlf_check = flag;
        pcep->remain_data = sizeret;

    }
    else
    {
        /* BINARY mode */
        readsize = file_read(pftpstat->file_index, pcep->buff_ptr, DATA_BUF_SIZE);
        if (readsize < 0)
        {
            file_close(pftpstat->file_index);
            pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
            return -1;
        }

        if (readsize == 0)
        {
            file_close(pftpstat->file_index);
            pftpstat->exec_command_subseq = FTP_DATA_RETR_SUBSEQ12;
        }

        pcep->remain_data = readsize;

    }


    return pcep->remain_data;
}


static int16_t ftps_retr_finish(ID cepid)
{
    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    if (pftpstat->exec_command_subseq != FTP_DATA_RETR_SUBSEQ16)
    {
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_EXEC_ERROR);
    }
    else
    {
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_TRNS_CMPLT);
    }

    pftpstat->exec_command = FTP_DATA_NONE;
    pftpstat->exec_command_subseq = 0;

    pcep->remain_data = strlen((const char *)pcep->buff_ptr);

    return (0);
}


static int16_t ftps_stor(int16_t argc, int8_t **argv, ID cepid)
{
    int32_t file_index;
    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;
    uint8_t  tmp_buf[512];

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    pftpstat->rnfr = 0;
    if (pcep->status < FTP_LOGIN)
    {
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_NOT_AVAILABLE);
        pcep->status = FTP_CLOSING;
        return (-1);
    }

    pftpstat->exec_command = FTP_DATA_STOR;
    pftpstat->exec_command_subseq = FTP_DATA_STOR_SUBSEQ04;

    if (argc != 2)
    {
        pcep->remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(pcep->buff_ptr), MSG_PARAM_ERROR);
        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
        return -1;
    }

    strcpy((char *)(pftpstat->fname), (const char *)argv[1]);

    if (0 != path_concatenate(cepid , tmp_buf, sizeof(tmp_buf), (uint8_t *)argv[1]))
    {
        pcep->remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(pcep->buff_ptr), MSG_EXEC_ERROR);
        return -1;
    }

    file_index = file_open((uint8_t *)tmp_buf, FILE_WRITE);
    if (file_index < 0)
    {
        pcep->remain_data = sizeof(MSG_NO_SUCH_FILE) - 1;
        strcpy((char *)(pcep->buff_ptr), MSG_NO_SUCH_FILE);
        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
        return -1;
    }
    else
    {
        pftpstat->file_index = file_index;
        if (pftpstat->trans_mode == 'A')
        {
            pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_OPEN_DATA_CON, "ASCII");
        }
        else
        {
            pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_OPEN_DATA_CON, "IMAGE");
        }
    }

    return (0);
}


static int16_t ftps_stor_dataport(ID cepid, int32_t rcv_len)
{
    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    if (file_write(pftpstat->file_index, pcep->buff_ptr, rcv_len) < 0)
    {
        file_close(pftpstat->file_index);
        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
        return -1;
    }
    return 0;
}


static int16_t ftps_stor_finish(ID cepid)
{
    _FTP_STAT *pftpstat;
    FTP_CEP   *pcep;

    pftpstat =  &ftp_stat[(cepid-1)/2];
    pcep  =  &ftp_cep[cepid-1];

    if (pftpstat->exec_command_subseq != FTP_DATA_STOR_SUBSEQ04)
    {
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_EXEC_ERROR);
    }
    else
    {
        pcep->remain_data = sprintf((char *)(pcep->buff_ptr), MSG_TRNS_CMPLT);
    }

    pftpstat->exec_command = FTP_DATA_NONE;
    pftpstat->exec_command_subseq = 0;

    pcep->remain_data = strlen((const char *)pcep->buff_ptr);

    return (0);
}


static int16_t ftps_mkd(int16_t argc, int8_t **argv, ID cepid)
{
    int32_t ret;
    uint8_t tmp_path[256];

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }
    if (argc != 2)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
        return -1;
    }

    if (0 != path_concatenate(cepid , tmp_path, sizeof(tmp_path), (uint8_t *)argv[1]))
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_EXEC_ERROR);
        return -1;
    }

    ret = make_dir((uint8_t*)tmp_path);
    switch (ret)
    {
        case -1:
            ftp_cep[cepid-1].remain_data = sizeof(MSG_INVALID_PATH) - 1;
            strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_INVALID_PATH);
            break;
        default:
            ftp_cep[cepid-1].remain_data = sizeof(MSG_MKD_RESPONSE) - 1;
            strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_MKD_RESPONSE);
            break;
    }
    return (0);
}


static int16_t ftps_cwd(int16_t argc, int8_t **argv, ID cepid)
{
    int32_t ret;
    uint8_t tmp_path[256];
    uint8_t cmdname[5];
    int32_t len;

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }
    if (argc != 2)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
        return -1;
    }

    strcpy((char *)cmdname, (const char *)argv[0]);

    if (0 != path_concatenate(cepid , tmp_path, sizeof(tmp_path), (uint8_t *)argv[1]))
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_EXEC_ERROR);
        return -1;
    }

    len = strlen((char *)tmp_path);
    if (len > 1 && tmp_path[len-1] == '/')
    {
        tmp_path[len-1] = '\0';
    }

    ret = change_dir(tmp_path);
    if (ret == 0)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_CMD_SUCCESS, cmdname);
        strcpy((char *)(ftp_stat[(cepid-1)/2].current_path), (const char *)tmp_path);

    }
    else
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_INVALID_PATH) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_INVALID_PATH);
    }


    return ret;
}


static int16_t ftps_cdup(int16_t argc, int8_t **argv, ID cepid)
{
    uint8_t tmp_path[256];
    uint8_t cmdname[5];
    uint16_t len, loop;

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }

    strcpy((char *)cmdname, (const char *)argv[0]);

    strcpy((char *)tmp_path, (char *)(ftp_stat[(cepid-1)/2].current_path));
    if (strlen((char *)(ftp_stat[(cepid-1)/2].current_path)) == 1)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_INVALID_PATH) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_INVALID_PATH);
    }
    else
    {
        len = strlen((char*)tmp_path);
        for (loop = len;loop > 0; loop--)
        {
            if (tmp_path[loop] == '/')
            {
                tmp_path[loop] = 0;
                break;
            }
            else
            {
                tmp_path[loop] = 0;
            }
        }
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_CMD_SUCCESS, cmdname);
        strcpy((char *)(ftp_stat[(cepid-1)/2].current_path), (const char *)tmp_path);
    }

    return (0);
}


static int16_t ftps_dele(int16_t argc, int8_t **argv, ID cepid)
{
    uint8_t tmp_path[256];
    uint8_t cmdname[5];

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }
    if (argc != 2)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
        return -1;
    }

    strcpy((char *)cmdname, (const char *)argv[0]);

    if (0 != path_concatenate(cepid , tmp_path, sizeof(tmp_path), (uint8_t *)argv[1]))
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_EXEC_ERROR);
        return -1;
    }


    if (file_delete((uint8_t *)tmp_path) == 0)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_CMD_SUCCESS, cmdname);
        return (0);
    }
    else
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_INVALID_PATH) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_INVALID_PATH);
        return (-1);
    }
}


static int16_t ftps_rmd(int16_t argc, int8_t **argv, ID cepid)
{
    int32_t ret;

    uint8_t tmp_path[256];

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }
    if (argc != 2)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
        return -1;
    }

    if (0 != path_concatenate(cepid , tmp_path, sizeof(tmp_path), (uint8_t *)argv[1]))
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_EXEC_ERROR);
        return -1;
    }

    ret = remove_dir((uint8_t *)tmp_path);
    switch (ret)
    {
        case -1:
            ftp_cep[cepid-1].remain_data = sizeof(MSG_INVALID_PATH) - 1;
            strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_INVALID_PATH);
            break;
        default:
            ftp_cep[cepid-1].remain_data = sizeof(MSG_MKD_RESPONSE) - 1;
            strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_MKD_RESPONSE);
            break;
    }
    return (0);
}


static int16_t ftps_rnfr(int16_t argc, int8_t **argv, ID cepid)
{
    uint8_t tmp_path[256];

    ftp_stat[(cepid-1)/2].rnfr = 0;
    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        return (-1);
    }
    if (argc != 2)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
        return -1;
    }


    if (0 != path_concatenate(cepid , tmp_path, sizeof(tmp_path), (uint8_t *)argv[1]))
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_EXEC_ERROR);
        return -1;
    }

    if (file_exist((uint8_t *)tmp_path) == 0)
    {
        strcpy((char *)(ftp_stat[(cepid-1)/2].fname), (const char *)tmp_path);
        ftp_cep[cepid-1].remain_data = sizeof(MSG_RNFR_RES) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_RNFR_RES);
        ftp_stat[(cepid-1)/2].rnfr = 1;
    }
    else
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_INVALID_PATH) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_INVALID_PATH);
    }
    return (0);
}


static int16_t ftps_rnto(int16_t argc, int8_t **argv, ID cepid)
{
    uint8_t old_path[256];
    uint8_t new_path[256];
    int8_t cmdname[5];

    if (ftp_cep[cepid-1].status < FTP_LOGIN)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_NOT_AVAILABLE);
        ftp_cep[cepid-1].status = FTP_CLOSING;
        ftp_stat[(cepid-1)/2].rnfr = 0;
        return (-1);
    }
    if (ftp_stat[(cepid-1)/2].rnfr != 1)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_CMD_BAD_SEQ);
        return (-1);
    }
    if (argc != 2)
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_PARAM_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_PARAM_ERROR);
        return -1;
    }

    strcpy((char *)cmdname, (const char *)argv[0]);

    ftp_stat[(cepid-1)/2].rnfr = 0;

    if (0 != path_concatenate(cepid , new_path, sizeof(new_path), (uint8_t *)argv[1]))
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_EXEC_ERROR);
        return -1;
    }

    if (0 != path_concatenate(cepid , old_path, sizeof(old_path), (uint8_t *)ftp_stat[(cepid-1)/2].fname))
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_EXEC_ERROR) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_EXEC_ERROR);
        return -1;
    }

    if (file_rename((uint8_t *)old_path, (uint8_t *)new_path) == 0)
    {
        ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_CMD_SUCCESS, cmdname);
    }
    else
    {
        ftp_cep[cepid-1].remain_data = sizeof(MSG_INVALID_PATH) - 1;
        strcpy((char *)(ftp_cep[cepid-1].buff_ptr), MSG_INVALID_PATH);
    }
    return 0;
}


static int16_t ftps_abor(int16_t argc, int8_t **argv, ID cepid)
{

    _FTP_STAT *pftpstat;
    FTP_CEP   *cmd_pcep;
    FTP_CEP   *data_pcep;

    pftpstat  =  &ftp_stat[(cepid-1)/2];
    cmd_pcep  =  &ftp_cep[cepid-1];
    data_pcep  =  &ftp_cep[cepid];

    pftpstat->rnfr = 0;
    if (cmd_pcep->status < FTP_LOGIN)
    {
        cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_NOT_AVAILABLE);
        cmd_pcep->status = FTP_CLOSING;
        return (-1);
    }

    /* analyze FTP server status to judge which API(T4) should be canceled */
    if (pftpstat->exec_command != FTP_DATA_NONE)
    {
        cmd_pcep->api_cancel = 0;
        cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_ABORT);
        switch (pftpstat->exec_command)
        {
            case FTP_DATA_LIST:
                break;
            case FTP_DATA_RETR:
            case FTP_DATA_STOR:
                file_close(pftpstat->file_index);
                break;
        }
        pftpstat->exec_command = FTP_DATA_ABOR;
        pftpstat->exec_command_subseq = FTP_DATA_ABOR_SUBSEQ04;

        if (data_pcep->status != FTP_CLOSED)
        {
            /* ftpstat == (command processing && establised */
            data_pcep->api_cancel = 1;
            return 3;
        }
        else
        {
            /* ftpstat == (command processing && closed) */
            pftpstat->exec_command_subseq = FTP_DATA_ABOR_SUBSEQ08;
            cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_ABORT);
            return 2;
        }
    }
    else if (data_pcep->status > FTP_CLOSING)
    {
        /* ftpstat == (not in command processing && (established) */
        cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_TRNS_CMPLT);
        return 1;
    }
    else
    {
        /* ftpstat == (not in command processing && (closed || closing)) */
        cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_TRNS_CMPLT);
        return 0;
    }
}


static void ftps_abor_finish(ID cepid)
{
    _FTP_STAT *pftpstat;
    FTP_CEP   *cmd_pcep;

    pftpstat  =  &ftp_stat[(cepid-1)/2];
    cmd_pcep  =  &ftp_cep[cepid-1];

    switch (pftpstat->exec_command_subseq)
    {
        case FTP_DATA_ABOR_SUBSEQ04:
            cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_ABORT);
            pftpstat->exec_command_subseq = FTP_DATA_ABOR_SUBSEQ08;
            break;
        case FTP_DATA_ABOR_SUBSEQ08:
            cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_TRNS_CMPLT);
            pftpstat->exec_command = FTP_DATA_NONE;
            pftpstat->exec_command_subseq = 0;
            break;
    }
}



static int16_t non_support_cmd(ID cepid, int8_t *argv)
{
    uint8_t cmdname[256];

    ftp_stat[(cepid-1)/2].rnfr = 0;

    strncpy((char *)cmdname, (const char *)argv, 256);
    cmdname[sizeof(cmdname)-1] = '\0';
    ftp_cep[cepid-1].remain_data = sprintf((char *)(ftp_cep[cepid-1].buff_ptr), MSG_CMD_FAILED, cmdname);
    return(-1);
}


static int32_t stricmp_rx(const uint8_t *s1, const uint8_t *s2, int32_t count)
{
    int32_t ret_i;
    int8_t c1, c2;
    int32_t loop;

    ret_i = 0;
    for (loop = 0; loop < count; loop++)
    {
        c1 = (int8_t)tolower(*(s1 + loop));
        c2 = (int8_t)tolower(*(s2 + loop));
        ret_i = c1 - c2;
        if (ret_i != 0)
        {
            break;
        }
        if (c1 == '\0')
        {
            break;
        }
    }
    return ret_i;

}

static int32_t path_concatenate(ID cepid , uint8_t *buf, uint32_t buf_len, uint8_t *path)
{
    int32_t  ret;
    _FTP_STAT *pftpstat;

    pftpstat = (&ftp_stat[(cepid-1)/2]);

    ret = 0;
    buf[0] = '\0';
    if (path[0] != '/')
    {
        /* current path + file/folder name */
        ret |= strcat_with_check_len(buf, (uint8_t *)pftpstat->current_path, buf_len);
        if (buf[strlen((char *)buf)-1] != '/')
        {
            ret |= strcat_with_check_len(buf, "/", buf_len);
        }
    }
    ret |= strcat_with_check_len(buf, path, buf_len);
    return ret;
}

ER ftp_callback(ID cepid, FN fncd , VP p_parblk)
{

    ER   parblk = *(ER *)p_parblk;
    ER   ercd;
    int16_t   func_ret;
    int16_t   argc;
    int8_t   *argv[MAX_ARGC];
    int32_t  ret;
    int16_t  i;
    int8_t   **p_cmd;

    ID   cmd_cepid;
    ID   data_cepid;
    FTP_CEP   *cmd_pcep;
    FTP_CEP   *data_pcep;
    _FTP_STAT *pftpstat;
    ID   cmd_cepid_oft;
    ID   data_cepid_oft;

    if (0 != (FTP_START_TCP_CEP % 2))
    {
        while (1); /* System Error !! */
    }

    if (cepid > FTP_START_TCP_CEP)
    {
        cmd_cepid_oft = cepid - FTP_START_TCP_CEP;
        data_cepid_oft = cmd_cepid_oft + 1;
    }
    else /* if (cepid > FTP_START_TCP_CEP) */
    {
        while (1);  /* System error !! */
    }

    cmd_cepid  = (cepid);
    data_cepid = (cepid + 1);
    cmd_pcep   = (&ftp_cep[cmd_cepid_oft-1]);
    data_pcep  = (&ftp_cep[data_cepid_oft-1]);
    pftpstat   = (&ftp_stat[(cmd_cepid_oft-1)/2]);

    if (parblk == E_RLWAI)
    {
        if (pftpstat->cep_reset_req == 0)
        {
            cmd_pcep->api_cancel = 0;
            switch (pftpstat->exec_command)
            {
                case FTP_DATA_LIST:
                    ftps_list_nlst_finish(cmd_cepid_oft);
                    tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                    break;
                case FTP_DATA_STOR:
                    ftps_stor_finish(cmd_cepid_oft);
                    tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                    break;
                case FTP_DATA_RETR:
                    ftps_retr_finish(cmd_cepid_oft);
                    tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                    break;
            }
        }
        else
        {
            pftpstat->cep_reset_req |= 0x04;
        }
        return 0;
    }
    if (parblk < 0)
    {
        cmd_pcep->now_data = 0;
        cmd_pcep->remain_data = 0;
        pftpstat->cep_reset_req = 0x10;
        return 0;
    }
    if (pftpstat->cep_reset_req > 0)
    {
        return 0;
    }
    switch (fncd)
    {
        case TFN_TCP_ACP_CEP:
            cmd_pcep->status = FTP_CONNECTION;
            cmd_pcep->remain_data = sprintf((char *)(cmd_pcep->buff_ptr), MSG_SERVER_READY);
            cmd_pcep->now_data = 0;
            tcp_snd_dat(cmd_cepid, cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
            break;
        case TFN_TCP_SND_DAT:
            cmd_pcep->now_data += parblk;
            cmd_pcep->remain_data -= parblk;
            if (cmd_pcep->remain_data > 0)
            {
                tcp_snd_dat(cmd_cepid, (VP)(cmd_pcep->buff_ptr + cmd_pcep->now_data), data_pcep->remain_data, TMO_NBLK);
                break;
            }

            cmd_pcep->now_data = 0;

            if (cmd_pcep->status == FTP_CLOSING)
            {
                tcp_cls_cep(cmd_cepid, TMO_NBLK);
                break;
            }
            if (pftpstat->exec_command != FTP_DATA_NONE)
            {
                switch (pftpstat->exec_command)
                {
                    case FTP_DATA_LIST:
                        ret = ftps_list_nlst_dataport(data_cepid_oft);
                        if (ret < 0)
                        {
                            /* error */
                            tcp_cls_cep(data_cepid, TMO_NBLK);
                            data_pcep->status = FTP_CLOSING;
                        }
                        else if (ret > 0)
                        {
                            tcp_snd_dat(data_cepid, (VP)(data_pcep->buff_ptr), data_pcep->remain_data, TMO_NBLK);
                        }
                        else
                        {
                            /* There is no more send data. */
                            tcp_cls_cep(data_cepid, TMO_NBLK);
                            data_pcep->status = FTP_CLOSING;
                        }
                        tcp_rcv_dat(cmd_cepid, cmd_pcep->buff_ptr, CMD_BUF_SIZE, TMO_NBLK);
                        break;
                    case FTP_DATA_STOR:
                        tcp_rcv_dat(cmd_cepid, cmd_pcep->buff_ptr, CMD_BUF_SIZE, TMO_NBLK);
                        break;
                    case FTP_DATA_RETR:
                        ret = ftps_retr_dataport(data_cepid_oft);
                        if (ret < 0)
                        {
                            /* error */
                            tcp_cls_cep(data_cepid, TMO_NBLK);
                            data_pcep->status = FTP_CLOSING;
                        }
                        else if (ret > 0)
                        {
                            tcp_snd_dat(data_cepid, (VP)(data_pcep->buff_ptr), data_pcep->remain_data, TMO_NBLK);
                        }
                        else
                        {
                            /* There is no more send data. */
                            tcp_cls_cep(data_cepid, TMO_NBLK);
                            data_pcep->status = FTP_CLOSING;
                        }
                        tcp_rcv_dat(cmd_cepid, cmd_pcep->buff_ptr, CMD_BUF_SIZE, TMO_NBLK);
                        break;
                    case FTP_DATA_ABOR:
                        ftps_abor_finish(cmd_cepid_oft);
                        tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                        break;
                    default:
                        break;
                }
                break;
            }
            tcp_rcv_dat(cmd_cepid, cmd_pcep->buff_ptr, CMD_BUF_SIZE, TMO_NBLK);
            break;
        case TFN_TCP_RCV_DAT:
            if (parblk == 0)
            {
                cmd_pcep->now_data = 0;
                tcp_cls_cep(cmd_cepid, TMO_NBLK);
                cmd_pcep->status = FTP_CLOSING;
                break;
            }
            /* The reception of the command: Return value -1: Error 0: Completion 1: There are continuation data  */
            ercd = ftp_rcv_request(cmd_cepid_oft, cmd_pcep->buff_ptr, CMD_BUF_SIZE, *(ER *)p_parblk);

            if (ercd == 0)
            {
                cmd_pcep->now_data = 0;

                /* Analyze a command and an argument */
                cmdline((int8_t *)(cmd_pcep->buff_ptr), &argc, argv);

                if (argc == 0)
                {
                    /* no command */
                    cmd_pcep->buff_ptr[0] = '\0';
                    non_support_cmd(cmd_cepid_oft, (int8_t *)cmd_pcep->buff_ptr);
                    tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                    break;
                }

                /* Analysis and the execute of the input command */
                p_cmd = (int8_t far **)ftps_cmd_tbl;
                for (i = 0; *p_cmd != NULL; i++, p_cmd++)
                {
                    /* search command */
                    if (stricmp_rx((uint8_t *)*p_cmd, (uint8_t *)argv[0], 5) == 0)
                    {
                        func_ret = (*ftps_cmd_exe[i])(argc, argv, cmd_cepid_oft);
                        break;
                    }
                }
                if (*p_cmd == NULL)
                {
                    /* No support command */
                    non_support_cmd(cmd_cepid_oft, argv[0]);
                    tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                    break;
                }
                if (pftpstat->exec_command != FTP_DATA_NONE && func_ret < 0)
                {
                    /* There is no send data. */
                    pftpstat->exec_command = FTP_DATA_NONE;
                    pftpstat->exec_command_subseq = 0;
                    tcp_cls_cep(data_cepid, TMO_NBLK);
                    data_pcep->status = FTP_CLOSING;
                    tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                    break;
                }
                switch (pftpstat->exec_command)
                {
                    case FTP_DATA_ABOR:
                        if (func_ret == 3)
                        {
                            /* command processing and the data port is established */
                        }
                        else if (func_ret == 2)
                        {
                            /*  command processing and the data port is closeed */
                            tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                        }
                        else if (func_ret == 1)
                        {
                            /* not in command processing and the data port is established */
                            pftpstat->exec_command = FTP_DATA_NONE;
                            pftpstat->exec_command_subseq = 0;
                            tcp_cls_cep(data_cepid, TMO_NBLK);
                            data_pcep->status = FTP_CLOSING;
                            tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                        }
                        else
                        {
                            /* not in command processing and the data port is closeed, or closing */
                            pftpstat->exec_command = FTP_DATA_NONE;
                            pftpstat->exec_command_subseq = 0;
                            tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                        }
                        break;
                    case FTP_DATA_PORT:
                        tcp_con_cep((data_cepid), &(data_pcep->myaddr), &(data_pcep->dstaddr), TMO_NBLK);
                        break;
                    case FTP_DATA_LIST:
                        tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                        break;
                    case FTP_DATA_STOR:
                        /* The reception in the data port */
                        data_pcep->remain_data = DATA_BUF_SIZE;
                        data_pcep->now_data = 0;
                        tcp_rcv_dat(data_cepid, data_pcep->buff_ptr, CMD_BUF_SIZE, TMO_NBLK);
                        tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                        break;
                    case FTP_DATA_RETR:
                        tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                        break;
                    default:
                        tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                        break;
                }
            }
            /* continue receiving */
            else if (ercd == 1)
            {
                tcp_rcv_dat(cmd_cepid, cmd_pcep->buff_ptr + cmd_pcep->remain_data, CMD_BUF_SIZE - cmd_pcep->remain_data, TMO_NBLK);
            }
            /* Error */
            else
            {
                tcp_cls_cep(cmd_cepid, TMO_NBLK);
                cmd_pcep->status = FTP_CLOSING;
                break;

            }
            break;
        case TFN_TCP_SHT_CEP:
            tcp_cls_cep(cmd_cepid, TMO_NBLK);
            cmd_pcep->status = FTP_CLOSING;
            break;
        case TFN_TCP_CLS_CEP:
            cmd_pcep->status = FTP_CLOSED;
            break;
        default:
            break;
    }
    return(0);
}


ER ftp_data_callback(ID cepid, FN fncd , VP p_parblk)
{

    ER   parblk = *(ER *)p_parblk;
    ER   ret;

    ID   cmd_cepid;
    ID   data_cepid;
    FTP_CEP   *cmd_pcep;
    FTP_CEP   *data_pcep;
    _FTP_STAT *pftpstat;
    ID   cmd_cepid_oft;
    ID   data_cepid_oft;

    if (0 != (FTP_START_TCP_CEP % 2))
    {
        while (1); /* System Error !! */
    }

    if (cepid > FTP_START_TCP_CEP)
    {
        cmd_cepid_oft = cepid - FTP_START_TCP_CEP - 1;
        data_cepid_oft = cmd_cepid_oft + 1;
    }
    else /* if (cepid > FTP_START_TCP_CEP) */
    {
        while (1);  /* System error !! */
    }

    cmd_cepid  = (cepid - 1);
    data_cepid = (cepid);
    cmd_pcep   = (&ftp_cep[cmd_cepid_oft-1]);
    data_pcep  = (&ftp_cep[data_cepid_oft-1]);
    pftpstat   = (&ftp_stat[(cmd_cepid_oft-1)/2]);


    if (parblk == E_RLWAI)
    {
        if (pftpstat->cep_reset_req == 0)
        {
            data_pcep->api_cancel = 0;
            switch (pftpstat->exec_command)
            {
                case FTP_DATA_ABOR:
                    ftps_abor_finish(cmd_cepid_oft);
                    tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                    tcp_cls_cep(data_cepid, TMO_NBLK);
                    data_pcep->status = FTP_CLOSING;
                    break;
            }
        }
        else
        {
            pftpstat->cep_reset_req |= 0x01;
        }
        return 0;
    }
    if (parblk < 0)
    {
        pftpstat->cep_reset_req = 0x10;
        return 0;
    }
    if (pftpstat->cep_reset_req > 0)
    {
        return 0;
    }
    switch (fncd)
    {
        case TFN_TCP_ACP_CEP:
            data_pcep->now_data = 0;
            data_pcep->remain_data = 0;
            data_pcep->status = FTP_CONNECTION;
        default:
            break;
        case TFN_TCP_CON_CEP:
            if (pftpstat->exec_command == FTP_DATA_PORT)
            {
                tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                pftpstat->exec_command = FTP_DATA_NONE;
            }
            data_pcep->now_data = 0;
            data_pcep->remain_data = 0;
            data_pcep->status = FTP_CONNECTION;
            break;
        case TFN_TCP_RCV_DAT:
            if (parblk == 0)
            {
                /* FIN */
                tcp_cls_cep(data_cepid, TMO_NBLK);
                data_pcep->status = FTP_CLOSING;
                break;
            }
            switch (pftpstat->exec_command)
            {
                case FTP_DATA_STOR:
                    if (pftpstat->exec_command_subseq == FTP_DATA_STOR_SUBSEQ04)
                    {
                        /* uploading */
                        data_pcep->now_data += parblk;
                        data_pcep->remain_data -= parblk;
                        if (data_pcep->remain_data > 0)
                        {
                            /* There are continuation data */
                            tcp_rcv_dat(data_cepid, data_pcep->buff_ptr + data_pcep->now_data, data_pcep->remain_data, TMO_NBLK);
                        }
                        else
                        {
                            if (ftps_stor_dataport(data_cepid_oft, data_pcep->now_data) < 0)
                            {
                                /* when FTP server failed to write file, FTP server ignores the reception data */
                                data_pcep->now_data = 0;
                                tcp_rcv_dat(data_cepid, data_pcep->buff_ptr, DATA_BUF_SIZE, TMO_NBLK);
                            }
                            else
                            {
                                /* File write success. continue receiving */
                                data_pcep->remain_data = DATA_BUF_SIZE;
                                data_pcep->now_data = 0;
                                tcp_rcv_dat(data_cepid, data_pcep->buff_ptr, DATA_BUF_SIZE, TMO_NBLK);
                            }
                        }
                    }
                    else
                    {
                        /* error occured in uploading */
                        data_pcep->now_data = 0;
                        tcp_rcv_dat(data_cepid, data_pcep->buff_ptr, DATA_BUF_SIZE, TMO_NBLK);
                    }
                    break;
            }
            break;
        case TFN_TCP_SND_DAT:
            switch (pftpstat->exec_command)
            {
                case FTP_DATA_LIST:
                    data_pcep->now_data += parblk;
                    data_pcep->remain_data -= parblk;
                    if (data_pcep->remain_data > 0)
                    {
                        tcp_snd_dat(data_cepid, (VP)(data_pcep->buff_ptr + data_pcep->now_data), data_pcep->remain_data, TMO_NBLK);
                    }
                    else
                    {
                        data_pcep->now_data = 0;
                        if (pftpstat->exec_command_subseq == FTP_DATA_LIST_SUBSEQ08)
                        {
                            /* transmit a list */
                            ret = ftps_list_nlst_dataport(data_cepid_oft);
                            if (ret > 0)
                            {
                                tcp_snd_dat(data_cepid, (VP)data_pcep->buff_ptr, data_pcep->remain_data, TMO_NBLK);
                            }
                            else
                            {
                                /* transmit lists. */
                                tcp_cls_cep(data_cepid, TMO_NBLK);
                                data_pcep->status = FTP_CLOSING;
                            }
                        }
                        else
                        {
                            /* transmit lists. */
                            tcp_cls_cep(data_cepid, TMO_NBLK);
                            data_pcep->status = FTP_CLOSING;
                        }
                    }

                    break;
                case FTP_DATA_RETR:
                    data_pcep->now_data += parblk;
                    data_pcep->remain_data -= parblk;
                    if (data_pcep->remain_data > 0)
                    {
                        tcp_snd_dat(data_cepid, (VP)(data_pcep->buff_ptr + data_pcep->now_data), data_pcep->remain_data, TMO_NBLK);
                    }
                    else
                    {
                        data_pcep->now_data = 0;
                        if (pftpstat->exec_command_subseq == FTP_DATA_RETR_SUBSEQ08)
                        {
                            ret = ftps_retr_dataport(data_cepid_oft);
                            if (ret > 0)
                            {
                                tcp_snd_dat(data_cepid, (VP)data_pcep->buff_ptr, data_pcep->remain_data, TMO_NBLK);
                            }
                            else
                            {
                                /* finish reading */
                                tcp_cls_cep(data_cepid, TMO_NBLK);
                                data_pcep->status = FTP_CLOSING;
                            }
                        }
                        else
                        {
                            /* error */
                            tcp_cls_cep(data_cepid, TMO_NBLK);
                            data_pcep->status = FTP_CLOSING;
                        }
                    }

                    break;
                default:
                    data_pcep->now_data = 0;
                    tcp_cls_cep(data_cepid, TMO_NBLK);
                    data_pcep->status = FTP_CLOSING;
                    break;
            }
            break;
        case TFN_TCP_SHT_CEP:
            tcp_cls_cep(data_cepid, TMO_NBLK);
            data_pcep->status = FTP_CLOSING;
            break;
        case TFN_TCP_CLS_CEP:
            data_pcep->status = FTP_CLOSED;
            switch (pftpstat->exec_command)
            {
                case FTP_DATA_LIST:
                    if (pftpstat->exec_command_subseq >= FTP_DATA_LIST_SUBSEQ12)
                    {
                        /* success */
                        pftpstat->exec_command_subseq = FTP_DATA_LIST_SUBSEQ16;
                    }
                    else
                    {
                        /* fail */
                        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
                    }
                    cmd_pcep->api_cancel = 1;

                    break;
                case FTP_DATA_STOR:
                    if (data_pcep->now_data > 0)
                    {
                        /* write last data block in tail of file */
                        ftps_stor_dataport(data_cepid_oft, data_pcep->now_data);
                    }
                    file_close(pftpstat->file_index);
                    cmd_pcep->api_cancel = 1;
                    break;
                case FTP_DATA_RETR:
                    if (pftpstat->exec_command_subseq == FTP_DATA_RETR_SUBSEQ12)
                    {
                        /* success */
                        pftpstat->exec_command_subseq = FTP_DATA_RETR_SUBSEQ16;
                    }
                    else
                    {
                        /* fail */
                        file_close(pftpstat->file_index);
                        pftpstat->exec_command_subseq = FTP_DATA_SUBSEQ_ERR;
                    }
                    cmd_pcep->api_cancel = 1;
                    break;
                case FTP_DATA_ABOR:
                    data_pcep->api_cancel = 0;
                    if (pftpstat->exec_command_subseq == FTP_DATA_ABOR_SUBSEQ04)
                    {
                        ftps_abor_finish(cmd_cepid_oft);
                        tcp_snd_dat(cmd_cepid, (VP)cmd_pcep->buff_ptr, cmd_pcep->remain_data, TMO_NBLK);
                    }
                    break;
            }
            break;
    }
    return 0;
}

/*****************************************************************************
* Function Name: R_T4_FTP_SERVER_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#pragma inline(R_T4_FTP_SERVER_GetVersion)
uint32_t  R_T4_FTP_SERVER_GetVersion(void)
{
    uint32_t const version = (T4_FTP_SERVER_VERSION_MAJOR << 16) | T4_FTP_SERVER_VERSION_MINOR;

    return version;
}

/* Copyright 2005,2011 RENESAS ELECTRONICS CORPORATION */







