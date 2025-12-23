/*
** Definitions and prototypes for CINS Web Directory Maintenance Module
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:32:37 1999
*/

#ifndef CINSWEB_H
#define CINSWEB_H

#define PAT_DEL   1
#define PAT_DIR   2
#define PAT_DL    3
#define PAT_LIST  4
#define PAT_TAG   5

#define CINSWEB_MENU      MENUNUM_NEWMSGFILE
#define CINSWEB_QUIT      11
#define CINSWEB_HELP      18
#define CINSWEB_LIST       8
#define CINSWEB_DELETE     6
#define CINSWEB_READ      40
#define CINSWEB_TAG       17
#define CINSWEB_DL         3
#define CINSWEB_UL        12
#define CINSWEB_DIR       10
#define CINSWEB_CD        27
#define CINSWEB_EDIT      28

void  CINSWeb_Main(void);
void  DirRead(short type);
void  ParseTrash(char *arg);
char *MakeFileDate(struct DateStamp *date);
void  setup(void);
void  command_loop(void);
void  ANSI_loop(void);
int   ReadDirEntries(LONG *size, struct List *list);
void  do_cd(char *path);
long  check_dir_size(void);
void  check_user_prefs(void);
void  check_web_dir(void);
void  pat_match(char *file,int action);
int   QueryDelete(char *file);

#endif /* !CINSWEB_H */
