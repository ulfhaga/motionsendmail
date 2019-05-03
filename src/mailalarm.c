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

#define FROM        "ulf.haga@gmail.com"
#define TO          "ulf.haga@icloud.com"
//#define CC          "otheruser@domain.com"
//#define BCC         "otheruser@domain.com"
//#define SMTPSERVER  "mailout.comhem.se"
#define SMTPSERVER  "smtp.gmail.com"
//#define SMTPPORT    25
#define SMTPPORT    587

//#define SMTPUSER    NULL
//#define SMTPPASS    NULL

#define SMTPUSER  "ulf.haga@gmail.com"
#define SMTPPASS  "ulferik2"

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
  logger(TRACE, "Start sending email");
  quickmail_initialize();
  quickmail mailobj = quickmail_create(FROM, "libquickmail test e-mail");
  quickmail_add_to(mailobj, TO);
  quickmail_add_header(mailobj, "Importance: Low");
  quickmail_add_header(mailobj, "X-Priority: 5");
  quickmail_add_header(mailobj, "X-MSMail-Priority: Low");
  quickmail_set_body(mailobj, "Alarm");
  // quickmail_add_body_memory(mailobj, "text/html", "This is a <b>test</b> e-mail.<br/>\nThis mail was sent using <u>libquickmail</u>.", 80, 0);
  quickmail_add_body_memory(mailobj, "text/html", email_message,
                            strlen(email_message), 0);
  quickmail_add_attachment_memory(mailobj, "test.log", NULL, "Test\n123", 8, 0);
  const char *errmsg;
  if ((errmsg =
       quickmail_send(mailobj, SMTPSERVER, SMTPPORT, SMTPUSER,
                      SMTPPASS)) != NULL)
  {
    fprintf(stderr, "Error sending e-mail: %s\n", errmsg);
  }
  quickmail_destroy(mailobj);
  quickmail_cleanup();
  logger(TRACE, "Sending email finish");
  return 0;
}
