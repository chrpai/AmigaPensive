/************************************************************************/
/*        sysedit.c             */
/*                  */
/*  Message Editor facilities.          */
/************************************************************************/
/************************************************************************/
/*        history         */
/*                  */
/* 89Nov12 AP Created.              */
/************************************************************************/
#define SYSTEM_DEPENDENT
#include "ctdl.h"
/************************************************************************/
/*        Contents        */
/*                  */
/*  OutsideEditor()   use an outside editor for sysop.  */
/************************************************************************/
typedef struct
  {
  char *Name;
  char *CmdLine;

  }
Editor;

int UnQueueSerRead(void);
int QueueSerRead(int);
int Jsystem(char *);


static void *EatEditorLine(char *line);
void *FindEditorSelector();
static SListBase ExtEditors =
  {
  NULL, FindEditorSelector, NULL, NULL,
  EatEditorLine

  };
static char **TheOpts;
extern CONFIG  cfg;            /* Lots an lots of variables    */
extern MessageBuffer msgBuf;         /* Message buffer               */
void OutsideEditorWork(char *EditLine);
/************************************************************************/
/*  OutsideEditor() use an outside editor for sysop.    */
/************************************************************************/
void OutsideEditor()
  {
  OutsideEditorWork(cfg.DepData.Editor);

  }
/************************************************************************/
/*  OutsideEditorWork() execute an outside editor.      */
/************************************************************************/
void ReActivate_Window(void);
void OutsideEditorWork(char *EditLine)
  {
  char cmdline[120];
  extern FILE *upfd;
  extern int outPut;
  doCR(); /* this gets crtColumn back to zero */
  sPrintf(cmdline, "%stempmsg.sys", cfg.DepData.EditArea);
  if (!redirect(cmdline)) return;
  mFormat(msgBuf.mbtext);
  undirect();
  MakeCmdLine(cmdline, EditLine, "", sizeof cmdline - 1);
  sPrintf(lbyte(cmdline), " %stempmsg.sys", cfg.DepData.EditArea);
  UnQueueSerRead();
  Jsystem(cmdline);
  ReActivate_Window();   /* AmigaDos: Make console active and front */
  QueueSerRead(8);
  homeSpace();
  msgBuf.mbtext[0] = 0;
  sPrintf(cmdline, "%stempmsg.sys", cfg.DepData.EditArea);
  ingestFile(cmdline, &msgBuf);
  unlink(cmdline);

  }
/************************************************************************/
/*  InitExternEditors() initialization function.      */
/************************************************************************/
void InitExternEditors()
  {
  SYS_FILE fn;
  makeSysName(fn, "editors.sys", &cfg.roomArea);
  MakeList(&ExtEditors, fn, NULL);

  }
/************************************************************************/
/*  EatEditorLine() eat a line from editors.sys.      */
/*  format is "<selector letter> <s.name> <command line>"   */
/************************************************************************/
static void *EatEditorLine(char *line)
  {
  Editor *work;
  char   *c;
  if ((c = strchr(line, ' ')) == NULL) return NULL;
  if (line[0] == '"')
    {
    line++;
    if ((c = strchr(line, '"')) == NULL) return NULL;
    *c = 0;
    c++;

    }
  *c = 0;
  work = GetDynamic(sizeof *work);
  work->Name = strdup(line);
  work->CmdLine = strdup(c + 1);
  return work;

  }
/************************************************************************/
/*  OtherEditOptions() add options to the editor menu.    */
/************************************************************************/
void OtherEditOptions(char **Options)
  {
  void AddOurEditOpts();
  TheOpts = Options;
  RunList(&ExtEditors, AddOurEditOpts);

  }
/************************************************************************/
/*  ShowOutsideEditors() show available external editors    */
/************************************************************************/
void ShowOutsideEditors()
  {
  void DisplayEditOpts();
  RunList(&ExtEditors, DisplayEditOpts);

  }
/************************************************************************/
/*  DisplayEditOpts() display editing options     */
/************************************************************************/
void DisplayEditOpts(Editor *d)
  {
  mPrintf("<%c>%s\n ", d->Name[0], d->Name + 1);

  }
/************************************************************************/
/*  AddOurEditOpts() add an external editor option.     */
/************************************************************************/
void AddOurEditOpts(Editor *d)
  {
  ExtraOption(TheOpts, d->Name);

  }
/************************************************************************/
/*  FindEditorSelector() find which editor has been requested.  */
/************************************************************************/
void *FindEditorSelector(Editor *d, int *s)
  {
  if (d->Name[0] == *s)
  return d;
  return NULL;

  }
/************************************************************************/
/*  RunRemoteEditor() run any of the extended editors.    */
/************************************************************************/
void RunRemoteEditor(int s)
  {
  Editor *TheEditor;
  if ((TheEditor = SearchList(&ExtEditors, &s)) != NULL)
  OutsideEditorWork(TheEditor->CmdLine);

  }
