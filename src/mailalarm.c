#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mailalarm.h"


/*
    This file is part of libquickmail.

    libquickmail is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libquickmail is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libquickmail.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "quickmail.h"
#include <stdio.h>
#include "log.h"

#define FROM        "ulf.haga@icloud.com"
#define TO          "ulf.haga@icloud.com"
//#define CC          "otheruser@domain.com"
//#define BCC         "otheruser@domain.com"
#define SMTPSERVER  "mailout.comhem.se"
#define SMTPPORT    25
#define SMTPUSER    NULL
#define SMTPPASS    NULL
#define MESSAGE_SIZE 128
#define SEND_MESSAGE_SIZE 2000

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


bool fileExists(const char *fname)
{
  FILE *file;
  if ((file = fopen(fname, "r")))
  {
    fclose(file);
    return true;
  }
  return false;
}

void
list_attachment_callback(quickmail mailobj, const char *filename,
                         quickmail_attachment_open_fn
                         email_info_attachment_open,
                         quickmail_attachment_read_fn
                         email_info_attachment_read,
                         quickmail_attachment_close_fn
                         email_info_attachment_close, void *callbackdata)
{
  printf("[%i]: %s\n", ++*(int *) callbackdata, filename);
}

int sendmail(char *email_message)
{
  int status = 0;
  char message[MESSAGE_SIZE + 1];
  logger(TRACE, "Sending email");
  if (email_message == NULL)
  {
    logger(WARN, "Email message is NULL. Not sending.");
    status = 1;
  } else
  {
    int quickmail_status = 0;
    snprintf(message, MESSAGE_SIZE, "libquickmail %s\n",
             quickmail_get_version());
    logger(INFO, message);

    quickmail_status = quickmail_initialize();
    if (quickmail_status == 0)
    {
      logger(DEBUG, "quickmail_create");
      quickmail mailobj = quickmail_create(FROM, "libquickmail test e-mail");
      if (mailobj == NULL)
      {
        logger(ERROR, "quickmail_create mailobj failed");
      }
      logger(DEBUG, "quickmail_create 0");
#ifdef TO
      quickmail_add_to(mailobj, TO);
#endif
#ifdef CC
      quickmail_add_cc(mailobj, CC);
#endif
#ifdef BCC
      quickmail_add_bcc(mailobj, BCC);
#endif
      logger(DEBUG, "quickmail_create 1");
      quickmail_add_header(mailobj, "Importance: Low");
      quickmail_add_header(mailobj, "X-Priority: 5");
      quickmail_add_header(mailobj, "X-MSMail-Priority: Low");
      quickmail_set_body(mailobj,
                         "This is a test e-mail.\nThis mail was sent using libquickmail.");
      logger(DEBUG, "quickmail_create 2");

      //quickmail_add_body_memory(mailobj, NULL, "This is a test e-mail.\nThis mail was sent using libquickmail.", 64, 0);

     
      char *mymess =
        "This is a <b>test</b> e-mail.<br/>\nThis html mail was sent using <u>libquickmail</u>, %s";
      int len = strlen(email_message) + strlen(mymess) - 2;

      char send_message[SEND_MESSAGE_SIZE];
      snprintf(send_message, SEND_MESSAGE_SIZE, mymess, email_message);

      snprintf(message, MESSAGE_SIZE, "len %i %i", len, strlen(send_message));
      logger(DEBUG,message);

      /*
         quickmail_add_body_memory(mailobj, "text/html",
         "This is a <b>test</b> e-mail.<br/>\nThis html mail was sent using <u>libquickmail</u>.",
         80, 0);
       */
      quickmail_add_body_memory(mailobj, "text/html", send_message,
                                strlen(send_message), 0);

      logger(DEBUG, "sendmail 1");

       /**/
//   quickmail_add_attachment_file(mailobj, "test_quickmail.c", NULL);
//   quickmail_add_attachment_file(mailobj, "test_quickmail.cbp", NULL);
/*
        if (fileExists("/var/www/html/pictur/a.jpg"))
      {
        quickmail_add_attachment_file(mailobj, "/var/www/html/pictur/a.jpg",
                                      NULL);
      }
*/
        logger(DEBUG, "sendmail 2");
      quickmail_add_attachment_memory(mailobj, "test.log", NULL, "Test\n123", 8,
                                      0);
      logger(DEBUG, "sendmail 3");
       /**/ const char *errmsg;
      //  quickmail_set_debug_log(mailobj, stderr);

      errmsg =
        quickmail_send(mailobj, SMTPSERVER, SMTPPORT, SMTPUSER, SMTPPASS);

      logger(DEBUG, "quickmail_send");
      if (errmsg != NULL)
      {
        snprintf(message, MESSAGE_SIZE, "Error sending e-mail: %s\n", errmsg);
        logger(ERROR, message);
      }
      quickmail_destroy(mailobj);
    } else
    {
      logger(ERROR, "Quickmail initialize failed");
    }
    quickmail_cleanup();
    logger(TRACE, "Sent email!");
  }
  return status;
}
