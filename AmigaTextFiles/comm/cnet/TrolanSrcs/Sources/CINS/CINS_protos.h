/*
** Prototypes for functions in CINS main executable
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:34:18 1999
*/

#ifndef CINS_PROTOS_H
#define CINS_PROTOS_H

void check_uucp(void);
void check_user_prefs(int ent);
long check_dir_size(void);
void check_keyfile(void);

void GetOut(void);

void DirRead(char *dirname);
void CINS_Main(void);
void setup(void);
void new_user(void);
void commands(void);
void command_loop(void);
void do_shell(void);

void do_netstat(void);

void do_ping(void);
void do_ping_exe(char *site);

void do_finger(void);
void do_finger_exe(char *user);

void do_lynx(char *url);

void do_traceroute(void);
void do_traceroute_exe(char *site);

void do_gopher(void);
void do_gopher_exe(char *site);

void do_irc(void);
void do_irc_nick(char *nick);
BOOL do_irc_notebook(char *server);
void save_irc_notebook(void);
void do_irc_exe(char *nick, char *server);
void do_irc_manual(char *server);

void do_telnet_menu(void);
void do_telnet_submenu(int menu);
void do_telnet_manual(void);
void do_telnet_notebook(void);
void save_telnet_notebook(void);
void do_telnet_exe(char *site);

void do_ftp_menu(void);
void do_ftp_submenu(int menu);
void do_ftp_manual(void);
void do_ftp_notebook(void);
void save_ftp_notebook(void);
void do_ftp_exe(char *site);
int check_ftp_open(void);
void check_ftp_close(void);

int do_menu(void);
void view_menu(char *menu);

int do_notebook_menu(void);

void do_talk(void);
void do_talk_exe(char *user);

void view_balance(void);

void view_help(void);
void view_help_file(char *file);

void archie(void);
void archie_pref(char *srch);
void archie_results(char *nice);
void archie_form(char *one);
void archie_server(char *server);
void archie_search(char *search);
BOOL do_archie_notebook(char *server);
void save_archie_notebook(void);
void do_archie_manual(char *server);
int archie_matches(void);

#endif /* !CINS_PROTOS_H */
