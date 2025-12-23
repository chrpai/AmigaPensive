/* Space Message File */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "dos.h"
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include "space.h"
#include "extra.h"
#include "io.h"
void send_messages ( void );
void Get_Msgs ( int to );
int getplayer ( int yourteam );
void optlog ( char *, char *);
extern long DOSBase;
#define MAXANSI 7
extern char *ansii[MAXANSI];
extern char spcpath[80];
extern char rnews[80];
extern struct Player *gpn;  /* global pointer to current player */
extern struct Team *team;
extern struct Player *player;
extern int pn;
extern struct Player *pz, *px;
void send_messages()
  {
  int z;
  if((boolean("\nSend any messages?",2) ==  1))
    {
    more:
    if((z =  getplayer(0))!=  -1)
      {
      Get_Msgs(z);
      }
    if((boolean("\nSend any more?",2) ==  1)) goto more;

    }

  }
void Get_Msgs(to)
int to;
  {
  int mark,i;
  char ostring[820],zstring[80];
  struct Player *pf;
  pf = &player[pn];
  sprintf(zstring,"%sMessages%d.TXT",spcpath,to);
  for(i=0; i < 820; i++)ostring[i] = '\0';
  strcat(ostring,pf->pname);
  mark = strlen(pf->pname);
  ostring[mark++] = ':';
  ostring[mark++] = '\n';
  printf("Enter Message, \".\" on a blank line to end. 800 Character Max\n");
  while( !(ostring[mark-1] == '.' && ostring[mark-2] == '\n') && mark < 820)
    {
    ostring[mark++] = getchar();
    }
  printf("\nSaving Message..................\n");
  ostring[mark-1] = ' ';
  optlog(ostring,zstring);
  while( getchar() != '\n');  /* flush the newline */
  }
