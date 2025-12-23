/*
** Definitions and prototypes for CINS Directory Maintenance Module
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:31:43 1999
*/

#ifndef CINSDIR_H
#define CINSDIR_H

#define PAT_DEL   1
#define PAT_DIR   2
#define PAT_DL    3
#define PAT_LIST  4
#define PAT_TAG   5

#define CINSDIR_MENU      MENUNUM_NEWMSGFILE
#define CINSDIR_QUIT      11
#define CINSDIR_HELP      18
#define CINSDIR_LIST       8
#define CINSDIR_DELETE     6
#define CINSDIR_READ      40
#define CINSDIR_TAG       17
#define CINSDIR_DL         3
#define CINSDIR_UL        12
#define CINSDIR_DIR       10
#define CINSDIR_CD        27

void  CINSDir_Main(void);
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
void  pat_match(char *file,int action);
int   QueryDelete(char *file);

#endif /* !CINSDIR_H */
