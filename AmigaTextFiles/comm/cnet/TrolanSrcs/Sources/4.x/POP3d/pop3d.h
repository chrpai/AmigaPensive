/*
** CNet/4 POP3 Daemon Header Definitions
** by Kelly Cochran
** Last Change: Mon Jan 11 19:08:31 1999
*/

#ifndef _POP3D_H
#define _POP3D_H

#include <bsdsocket.h>
#include <inetd.h>
#include <sys/syslog.h>
#include <amitcp/socketbasetags.h>

#define MAX_OPTIONS 8

#define HEADER_BUFFER      80
#define POP3_BUFFER_SIZE  512
#define POP3_TIMEOUT      300 /* Seconds of idle allowed */

#define MTT_PATTERN CNETOUTBOX"/%08X.mtt"
#define MTL_PATTERN CNETOUTBOX"/%08X.mtl"
#define CMH_PATTERN CNETOUTBOX"/%08X.cmh"

#define VER         "1.21"

enum
{
    STATE_IDLE = 1,
    STATE_AUTH,
    STATE_USER,
    STATE_QUIT,
};

#define POP_FAILURE 0
#define POP_SUCCESS 1

/* Command interpreter routines */

int              cmd_parse(char *buffer, char **args);
void             cmd_lower(char *buff);
struct POP_Cmd  *command_parser(char *cmd, int *state, int args);
struct XTND_Cmd *sub_command_parser(int argc, char **argv);

/* Command handlers */

int cmd_top  (int argc, char **argv);
int cmd_list (int argc, char **argv);
int cmd_quit (int argc, char **argv);
int cmd_pass (int argc, char **argv);
int cmd_user (int argc, char **argv);
int cmd_retr (int argc, char **argv);
int cmd_dele (int argc, char **argv);
int cmd_stat (int argc, char **argv);
int cmd_uidl (int argc, char **argv);
int cmd_euidl(int argc, char **argv);
int cmd_rset (int argc, char **argv);
int cmd_auth (int argc, char **argv);
int cmd_capa (int argc, char **argv);
int cmd_apop (int argc, char **argv);
int cmd_rpop (int argc, char **argv);
int cmd_xtnd (int argc, char **argv);
int cmd_xlst (int argc, char **argv);
int cmd_xmit (int argc, char **argv);

/* Basic Utilities */

void                arpa_date(ULONG date, char *buff);
struct List        *init_list(void);
void                AddMHToList(struct List *list, struct MailHeader4 *mh4);
int                 LoadMHList(struct UserData *udata);
void                free_list(struct List **list);
struct MailNode    *FindMNode(struct List *list, int node);
int                 write_mh4(struct List *list, struct UserData *udata, int quit);
void                file_retrcpt(struct MailNode *mn, struct UserData *udata);
int                 print_headers(struct MailNode *mn, BOOL msg);
struct timerequest *init_timer(void);
void                delete_timer(struct timerequest *tr);
long                parse_mail_output(BPTR fp, long mail_size);
long                parse_header_output(BPTR fp, long mail_size);
void                pop_log(int pri, char *fmt, ...);
ULONG               PrintHeaderToSock(char *file, ULONG file_size);
char               *find_header(struct MailNode *mn, char *header, BOOL preserve);
int                 strincmp(char *s1, char *s2, int n);

/* Socket handlers */

BOOL                init_inetd_server(int *SOCK);
int                 pop_get_msg(int sock, char *buffer);
int                 pop_msg(int status, char *fmt, ...);

enum
{
   POP_NULL = 0,
   POP_OK,
   POP_ERR,
};

enum
{
   HDR_FROM = 0,
   HDR_DATE,
   HDR_SUBJECT,
   HDR_TO,
   HDR_UIDL,
   HDR_CONTENT_LENGTH,
   HDR_RECEIVED,
};

struct HeaderDef
{
   char *Name;
   int   value;
};

/* Structure declarations */

extern struct MainPort *myp;

struct POP_Cmd
{
   int   req_state;                            /* state required to  use this command */
   char *command;                              /* Command name     */
   int   (*function)(int argc, char **argv);   /* function to call */
   int   min_ops;                              /* minimum options  */
   int   max_ops;                              /* maximum options  */
   int   result [ 2 ];                         /* new state (0:Fail 1:Succeed) */
};

struct XTND_Cmd
{
   char *command;                              /* Command name     */
   int   (*function)(int argc, char **argv);   /* function to call */
   int   min_ops;                              /* minimum options  */
   int   max_ops;                              /* maximum options  */
};

struct MailList
{
   struct List *mh4_List;     /* Exec List of MailHeader4 structs    */
   long         num_Items;    /* Number of mail items loaded         */
   long         Deleted;      /* Number of mail items marked Deleted */
   long         Bytes;        /* Number of bytes in undeleted mails  */
};

struct MailNode
{
   struct Node        mn_Node;   /* Exec Node header          */
   struct MailHeader4 mn_MH4;    /* MailHeader4               */
   BOOL   Deleted;               /* This message Deleted?     */
};

#endif /* !_POP3D_H */
