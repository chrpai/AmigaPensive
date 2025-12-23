/*
** Definitions and prototypes for CINSConfig
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:27:40 1999
*/

#ifndef _CINSCONFIG_H
#define _CINSCONFIG_H

#include <libraries/bgui.h>
#include <libraries/bgui_macros.h>
#include <proto/bgui.h>

enum {
   CINS_ACCESS_SLIDER = 1,
#define CINS_ACCESS_SLIDER_MIN    0
#define CINS_ACCESS_SLIDER_MAX   31
   CINS_FILEDIR_STRING,
   CINS_URL_STRING,
   CINS_IRC_STRING,
   CINS_ARCHIE_STRING,
   CINS_GOPHER_STRING,
   CINS_LOG_INTEGER,
   CINS_QUIT_BUTTON,
   CINS_SAVE_BUTTON,
   CINS_FTP_INTEGER,
   CINS_MENU_INTEGER,
   CINS_SAVEQUIT_BUTTON,
   CINS_EDITARCHIE_BUTTON,
   CINS_EDITIRC_BUTTON,
   CINS_EDITTELNET_BUTTON,
   CINS_EDITFTP_BUTTON,
   CINS_USERDB_CHECK,
   CINS_ACCOUNTING_BUTTON,
   CINS_ACCESS_INTEGER,
   CINS_DEFAULTUSER_BUTTON,

   CINS_ARC_LISTVIEW,
   CINS_ARC_SERVER_STRING,
   CINS_ARC_ADD_BUTTON,
   CINS_ARC_INSERT_BUTTON,
   CINS_ARC_DELETE_BUTTON,
   CINS_ARC_QUIT_BUTTON,

   CINS_IRC_LISTVIEW,
   CINS_IRC_SERVER_STRING,
   CINS_IRC_HOST_STRING,
   CINS_IRC_ADD_BUTTON,
   CINS_IRC_INSERT_BUTTON,
   CINS_IRC_DELETE_BUTTON,
   CINS_IRC_QUIT_BUTTON,

   CINS_ACC_AG_LISTVIEW,
   CINS_ACC_FTPFREE_INTEGER,
   CINS_ACC_WEBFREE_INTEGER,
   CINS_ACC_RATE_INTEGER,
   CINS_ACC_PURGEDAYS_INTEGER,
   CINS_ACC_QUIT_BUTTON,
   CINS_ACC_UPLOADFTP_CHECK,
   CINS_ACC_UPLOADWEB_CHECK,
   CINS_ACC_CD_CHECK,
   CINS_ACC_GADGET_COUNT,

   CINS_FTP_LIST_LISTVIEW,
   CINS_FTP_ADDLIST_BUTTON,
   CINS_FTP_DELLIST_BUTTON,
   CINS_FTP_INSLIST_BUTTON,
   CINS_FTP_LIST_STRING,
   CINS_FTP_SERVER_LISTVIEW,
   CINS_FTP_ADDSERVER_BUTTON,
   CINS_FTP_DELSERVER_BUTTON,
   CINS_FTP_INSSERVER_BUTTON,
   CINS_FTP_NAME_STRING,
   CINS_FTP_HOST_STRING,
   CINS_FTP_COMM1_STRING,
   CINS_FTP_COMM2_STRING,
   CINS_FTP_QUIT_BUTTON,

   CINS_TELNET_LIST_LISTVIEW,
   CINS_TELNET_ADDLIST_BUTTON,
   CINS_TELNET_DELLIST_BUTTON,
   CINS_TELNET_INSLIST_BUTTON,
   CINS_TELNET_LIST_STRING,
   CINS_TELNET_SERVER_LISTVIEW,
   CINS_TELNET_ADDSERVER_BUTTON,
   CINS_TELNET_DELSERVER_BUTTON,
   CINS_TELNET_INSSERVER_BUTTON,
   CINS_TELNET_NAME_STRING,
   CINS_TELNET_HOST_STRING,
   CINS_TELNET_COMM1_STRING,
   CINS_TELNET_COMM2_STRING,
   CINS_TELNET_QUIT_BUTTON,

   CINS_DEFUSER_QUIT_BUTTON,
   CINS_DEFUSER_ANSIDIR_CHECK,
   CINS_DEFUSER_SHELL_CHECK,
   CINS_DEFUSER_MUFFLE_CHECK,
   CINS_DEFUSER_FTPPROGRESS_CYCLE,
   CINS_DEFUSER_FTPANSI_CHECK,
   CINS_DEFUSER_FTPAUTOBIN_CHECK,
   CINS_DEFUSER_FTPVERBOSE_CYCLE,
   CINS_DEFUSER_FTPAUTOANON_CHECK,
   CINS_DEFUSER_FTPRECENT_CHECK,
   CINS_DEFUSER_FTPTIPS_CHECK,
   CINS_DEFUSER_FTPMGET_CHECK,

   CINS_ABOUT,

   CINS_GADGET_COUNT
};

extern struct CINSCfg        Config;

/* Internal Messaging port */
extern struct MsgPort *CINSCfgMsgPort;

/* Gadget stuff */
extern Object *CINSGads[CINS_GADGET_COUNT];

/* CNet structs */
extern struct MainPort      *myp;    /* Pointer to CNet port--ALL info!      */
extern struct PortData      *z;
extern struct Library       *CNetBase;
extern struct Library       *BGUIBase;
extern struct Library       *CINSBase;
extern struct Library       *CNetCBase;   /* only set if run from CNet */
extern struct CNetCContext  *context;

void GUI_main(void);
void VDE_main(void);
BOOL make_config_VDE(void);
void GUI_pre_save(struct Gadget **CINSGads);
LONG check_keyfile(void);

/* Config handlers */
BOOL load_config(void);
void save_config(void);
int  load_archie_config(struct List **list);
void save_archie_config(struct List *list);
int  load_ftp_config(struct List **list);
int  load_ftp_data_config(struct List **list, int i);
void save_ftp_config(struct List *list);
void save_ftp_data_config(struct List *list, int i);
int  load_irc_config(struct List **list);
void save_irc_config(struct List *list);
int  load_telnet_config(struct List **list);
int  load_telnet_data_config(struct List **list, int i);
void save_telnet_config(struct List *list);
void save_telnet_data_config(struct List *list, int i);

void free_data_list(struct List **list);

/* GUI Window Handlers */
struct Window *OpenCINSMainWindow(void);
struct Window *OpenCINSAccWindow(void);
struct Window *OpenCINSArchieWindow(void);
struct Window *OpenCINSDefaultUserWindow(void);
struct Window *OpenCINSFTPWindow(void);
struct Window *OpenCINSIRCWindow(void);
struct Window *OpenCINSTelnetWindow(void);

ULONG MakeRequest(struct Window *win, char *title, char *gads, char *string, ...);

/* GUI Event Handlers */
struct Window *HandleAccWindow(struct Window *win);
struct Window *HandleArchieWindow(struct Window *win);
struct Window *HandleDefaultUserWindow(struct Window *win);
struct Window *HandleFTPWindow(struct Window *win);
struct Window *HandleIRCWindow(struct Window *win);
struct Window *HandleTelnetWindow(struct Window *win);

/* CNet Direct functions */
void GetOut( void );

/* CNetC handlers */
void do_config(void);
long check_keyfile(void);

#endif /* _CINSCONFIG_H */
