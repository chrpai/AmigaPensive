/*
** Prototypes for CINS User Configuration Editor
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:32:14 1999
*/

#ifndef CINSEDIT_H
#define CINSEDIT_H

void edit_ncftp(void);
void command_loop(void);
void setup(void);
void commands(void);
void CINEdit_Main(void);
void read_netrc(void);
void write_netrc(void);
void read_ucfg(void);
void write_ucfg(void);
void do_irc_nick(void);
void new_user(void);
BOOL write_user_VDE(void);

#endif /* !CINSEDIT_H */
