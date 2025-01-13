/************************************************************************************
 *
 *  Copyright (C) 2009-2012 Broadcom Corporation
 *
 *  This program is the proprietary software of Broadcom Corporation and/or its
 *  licensors, and may only be used, duplicated, modified or distributed
 *  pursuant to the terms and conditions of a separate, written license
 *  agreement executed between you and Broadcom (an "Authorized License").
 *  Except as set forth in an Authorized License, Broadcom grants no license
 *  (express or implied), right to use, or waiver of any kind with respect to
 *  the Software, and Broadcom expressly reserves all rights in and to the
 *  Software and all intellectual property rights therein.
 *  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS
 *  SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 *  ALL USE OF THE SOFTWARE.
 *
 *  Except as expressly set forth in the Authorized License,
 *
 *  1.     This program, including its structure, sequence and organization,
 *         constitutes the valuable trade secrets of Broadcom, and you shall
 *         use all reasonable efforts to protect the confidentiality thereof,
 *         and to use this information only in connection with your use of
 *         Broadcom integrated circuit products.
 *
 *  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 *         "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 *         REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 *         OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 *         DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 *         NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 *         ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 *         CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT
 *         OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 *  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
 *         ITS LICENSORS BE LIABLE FOR
 *         (i)   CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
 *               DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *               YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 *               HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
 *         (ii)  ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
 *               SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 *               LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *               ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *
 ************************************************************************************/

/************************************************************************************
 *
 *  Filename:      bluedroidtest.c
 *
 *  Description:   Bluedroid Test application
 *
 ***********************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <linux/capability.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>



/************************************************************************************
**  Constants & Macros
************************************************************************************/

#define PID_FILE "/data/.bdt_pid"

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#define CASE_RETURN_STR(const) case const: return #const;

/************************************************************************************
**  Local type definitions
************************************************************************************/

/************************************************************************************
**  Static variables
************************************************************************************/

static unsigned char main_done = 0;
/************************************************************************************
**  Static functions
************************************************************************************/

static void process_cmd(char *p, unsigned char is_job);

/************************************************************************************
**  Externs
************************************************************************************/
extern void proc_rx_test(unsigned char channel);
extern void proc_tx_test(unsigned char channel, unsigned char data_len, unsigned char pattern);
extern void proc_test_end(void);
extern void proc_dut(void);
extern void proc_reset(void);
extern void hci_send_cmd(unsigned char *buffer , int len);
extern void read_event_ex(unsigned char *buffer);
/************************************************************************************
**  Functions
************************************************************************************/


/************************************************************************************
**  Shutdown helper functions
************************************************************************************/

static void bdt_shutdown(void)
{
    fprintf(stderr, "shutdown bdroid test app\n");
    main_done = 1;
}


/*******************************************************************************
 ** Console helper functions
 *******************************************************************************/

void skip_blanks(char **p)
{
  while (**p == ' ')
    (*p)++;
}

uint32_t get_int(char **p, int DefaultValue)
{
  uint32_t Value = 0;
  unsigned char   UseDefault;

  UseDefault = 1;
  skip_blanks(p);

  while ( ((**p)<= '9' && (**p)>= '0') )
    {
      Value = Value * 10 + (**p) - '0';
      UseDefault = 0;
      (*p)++;
    }

  if (UseDefault)
    return DefaultValue;
  else
    return Value;
}

int get_signed_int(char **p, int DefaultValue)
{
  int    Value = 0;
  unsigned char   UseDefault;
  unsigned char  NegativeNum = 0;

  UseDefault = 1;
  skip_blanks(p);

  if ( (**p) == '-')
    {
      NegativeNum = 1;
      (*p)++;
    }
  while ( ((**p)<= '9' && (**p)>= '0') )
    {
      Value = Value * 10 + (**p) - '0';
      UseDefault = 0;
      (*p)++;
    }

  if (UseDefault)
    return DefaultValue;
  else
    return ((NegativeNum == 0)? Value : -Value);
}

void get_str(char **p, char *Buffer)
{
  skip_blanks(p);

  while (**p != 0 && **p != ' ')
    {
      *Buffer = **p;
      (*p)++;
      Buffer++;
    }

  *Buffer = 0;
}

uint32_t get_hex(char **p, int DefaultValue)
{
  uint32_t Value = 0;
  unsigned char   UseDefault;

  UseDefault = 1;
  skip_blanks(p);

  while ( ((**p)<= '9' && (**p)>= '0') ||
          ((**p)<= 'f' && (**p)>= 'a') ||
          ((**p)<= 'F' && (**p)>= 'A') )
    {
      if (**p >= 'a')
        Value = Value * 16 + (**p) - 'a' + 10;
      else if (**p >= 'A')
        Value = Value * 16 + (**p) - 'A' + 10;
      else
        Value = Value * 16 + (**p) - '0';
      UseDefault = 0;
      (*p)++;
    }

  if (UseDefault)
    return DefaultValue;
  else
    return Value;
}


#define is_cmd(str) ((strlen(str) == strlen(cmd)) && strncmp((const char *)&cmd, str, strlen(str)) == 0)
#define if_cmd(str)  if (is_cmd(str))

typedef void (t_console_cmd_handler) (char *p);

typedef struct {
    const char *name;
    t_console_cmd_handler *handler;
    const char *help;
    unsigned char is_job;
} t_cmd;


const t_cmd console_cmd_list[];
static int console_cmd_maxlen = 0;

static void cmdjob_handler(void *param)
{
    char *job_cmd = (char*)param;

    fprintf(stderr, "cmdjob starting (%s)", job_cmd);

    process_cmd(job_cmd, 1);

    fprintf(stderr, "cmdjob terminating");

    free(job_cmd);
}

static int create_cmdjob(char *cmd)
{
    pthread_t thread_id;
    char *job_cmd;

    job_cmd = malloc(strlen(cmd)+1); /* freed in job handler */
    strcpy(job_cmd, cmd);

    if (pthread_create(&thread_id, NULL,
                       (void*)cmdjob_handler, (void*)job_cmd)!=0)
      perror("pthread_create");

    return 0;
}


/*******************************************************************************
 ** HAL test functions & callbacks
 *******************************************************************************/

void setup_test_env(void)
{
    int i = 0;

    while (console_cmd_list[i].name != NULL)
    {
        console_cmd_maxlen = MAX(console_cmd_maxlen, (int)strlen(console_cmd_list[i].name));
        i++;
    }
}



void
proc_dut()
{
    unsigned char hci_set_event_filter[] =
	    { 0x01, 0x05, 0x0C, 0x03, 0x02, 0x00, 0x02};

    unsigned char hci_write_current_iac_lap[] =
	    { 0x01, 0x3a, 0x0c, 0x04, 0x01, 0x33, 0x8b, 0x9e};

    unsigned char hci_write_inquriy_scan_activity[] =
	    { 0x01, 0x1e, 0x0c, 0x04, 0x00, 0x08, 0x12, 0x00};

    unsigned char hci_write_scan_enable[] =
	    { 0x01, 0x1A, 0x0C, 0x01, 0x03};

    unsigned char hci_set_event_mask[] =
	    { 0x01, 0x01, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    unsigned char hci_enter_dut[] =
	    { 0x01, 0x03, 0x18, 0x00};

	hci_send_cmd(hci_set_event_filter,
		sizeof(hci_set_event_filter));

    usleep(100000);


	hci_send_cmd(hci_write_current_iac_lap,
		sizeof(hci_write_current_iac_lap));

    usleep(100000);

	hci_send_cmd(hci_write_inquriy_scan_activity,
		sizeof(hci_write_inquriy_scan_activity));

    usleep(100000);

	hci_send_cmd(hci_set_event_mask,
		sizeof(hci_set_event_mask));

    usleep(100000);

	hci_send_cmd(hci_write_scan_enable,
		sizeof(hci_write_scan_enable));

    usleep(100000);

	hci_send_cmd(hci_enter_dut,
		sizeof(hci_enter_dut));

    usleep(100000);
}


void proc_rx_test(unsigned char channel)
{
    unsigned char buf[] ={0x01, 0x1d, 0x20, 0x01, 0x00};

    buf[4] = channel;
	hci_send_cmd(buf, sizeof(buf));

}

void proc_tx_test(unsigned char channel, unsigned char data_len, unsigned char pattern)
{
    unsigned char buf[] ={0x01, 0x1e, 0x20, 0x03, 0x00, 0x00, 0x00};

    buf[4] = channel;
    buf[5] = data_len;
    buf[6] = pattern;
	hci_send_cmd(buf, sizeof(buf));
}


void proc_test_end()
{
    unsigned char buf[] ={0x01, 0x1f, 0x20, 0x00};

	hci_send_cmd(buf, sizeof(buf));
}


void bdt_dut_mode_configure(char *p)
{
    int32_t mode = -1;

    mode = get_signed_int(&p, mode);
    if ((mode != 0) && (mode != 1)) {
        fprintf(stderr, "Please specify mode: 1 to enter, 0 to exit\n");
        return;
    }

    if(mode == 1) {
        proc_dut();
    } else {
        /*lgh send cmd*/
        proc_reset();
    }
}

void bdt_le_test_mode(char *p)
{
    int cmd;
    unsigned char buf[3];
    int arg1, arg2, arg3;

    fprintf(stderr, "BT LE TEST MODE\n");

    memset(buf, 0, sizeof(buf));
    cmd = get_int(&p, 0);
    switch (cmd)
    {
        case 0x1: /* RX TEST */
           arg1 = get_int(&p, -1);
           if (arg1 < 0)
           {
                fprintf(stderr, "%s Invalid arguments\n", __FUNCTION__);
           } else {
               buf[0] = arg1;
               proc_rx_test(buf[0]);
           }
           break;
        case 0x2: /* TX TEST */
            arg1 = get_int(&p, -1);
            arg2 = get_int(&p, -1);
            arg3 = get_int(&p, -1);
            if ((arg1 < 0) || (arg2 < 0) || (arg3 < 0)) {
                fprintf(stderr, "%s Invalid arguments\n", __FUNCTION__);
            } else {
                buf[0] = arg1;
                buf[1] = arg2;
                buf[2] = arg3;
                proc_tx_test(arg1, arg2, arg3);
            }
           break;
        case 0x3: /* END TEST */
            proc_test_end();
           break;
        default:
            fprintf(stderr, "Unsupported command\n");
            return;
            break;
    }

    return;
}


void bdt_hcicmd(char *p)
{
    unsigned char buf[256];
    int i=0, temp;

    fprintf(stderr, "send arbitrary HCI cmd\n");

    memset(buf, 0, sizeof(buf));

    buf[0] = 0x01;
    i=1;
    do{
        temp = get_hex(&p, -1);
        if(temp <0) {
            break;
        }
        buf[i] = temp;
        i++;
    } while(i <256);

    if(i<3) {
        fprintf(stderr, "invalid HCI cmd\n");
    }

    hci_send_cmd(buf, i);

    return;
}

void bdt_cleanup(void)
{
    fprintf(stderr, "CLEANUP\n");
}

/*******************************************************************************
 ** Console commands
 *******************************************************************************/

void do_help(char *p)
{
    int i = 0;
    char line[128];
    int pos = 0;

    while (console_cmd_list[i].name != NULL)
    {
        pos = sprintf(line, "%s", (char*)console_cmd_list[i].name);
        fprintf(stderr, "%s %s\n", (char*)line, (char*)console_cmd_list[i].help);
        i++;
    }
}

void do_quit(char *p)
{
    bdt_shutdown();
}

/*******************************************************************
 *
 *  BT TEST  CONSOLE COMMANDS
 *
 *  Parses argument lists and passes to API test function
 *
*/


void do_dut_mode_configure(char *p)
{
    bdt_dut_mode_configure(p);
}

void do_le_test_mode(char *p)
{
    bdt_le_test_mode(p);
}

void do_cleanup(char *p)
{
    bdt_cleanup();
}

void do_hcicmd(char *p)
{
    bdt_hcicmd(p);
}


/*******************************************************************
 *
 *  CONSOLE COMMAND TABLE
 *
*/

const t_cmd console_cmd_list[] =
{
    /*
     * INTERNAL
     */

    { "help", do_help, "lists all available console commands", 0 },
    { "quit", do_quit, "", 0},

    /*
     * API CONSOLE COMMANDS
     */

     /* Init and Cleanup shall be called automatically */
//    { "enable", do_enable, ":: enables bluetooth", 0 },
//    { "disable", do_disable, ":: disables bluetooth", 0 },
    { "dut_mode_configure", do_dut_mode_configure, ":: DUT mode - 1 to enter,0 to exit", 0 },
    { "le_test_mode", do_le_test_mode, ":: LE Test Mode - RxTest - 1 <rx_freq>, \n\t \
                      TxTest - 2 <tx_freq> <test_data_len> <payload_pattern>, \n\t \
                      End Test - 3 <no_args>", 0 },
    { "hcicmd", do_hcicmd, "send arbitrary hci cmd, e.g.\n\t \
                      hcicmd 03 0c 00", 0 },
    /* add here */

    /* last entry */
    {NULL, NULL, "", 0},
};

/*
 * Main console command handler
*/

static void process_cmd(char *p, unsigned char is_job)
{
    char cmd[64];
    int i = 0;
    char *p_saved = p;

    get_str(&p, cmd);

    /* table commands */
    while (console_cmd_list[i].name != NULL)
    {
        if (is_cmd(console_cmd_list[i].name))
        {
            if (!is_job && console_cmd_list[i].is_job)
                create_cmdjob(p_saved);
            else
            {
                console_cmd_list[i].handler(p);
            }
            return;
        }
        i++;
    }
    fprintf(stderr, "%s : unknown command\n", p_saved);
    do_help(NULL);
}

extern int patchram_download (int argc, char **argv);

void readevent_handler(void *p)
{
    unsigned char buffer[256];
    while(1) {
//        fprintf(stderr, "Read thread is started\n");
        sleep(1);
        read_event_ex(buffer);
    }
}
#include <stdio.h>
#include <time.h>

int main (int argc, char * argv[])
{
    pthread_t thread_id;
    int usb_fd = -1;
    struct timespec current_time, prev_time = {0, 0};

	patchram_download(argc, argv);

    do_help(NULL);
    pthread_create(&thread_id, NULL, (void*)readevent_handler, NULL);

    while(!main_done)
    {
        char line[128];

        /* command prompt */
        printf( ">" );
        fflush(stdout);

        fgets (line, 128, stdin);

        if (line[0]!= '\0')
        {
            /* remove linefeed */
            line[strlen(line)-1] = 0;

            process_cmd(line, 0);
            memset(line, '\0', 128);
        }
    }

    fprintf(stderr, ":: Bluedroid test app terminating\n");

    return 0;
}
