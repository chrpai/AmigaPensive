/* Notes:
In CTDL.C instead of have the doHelp() call tutorial() it should now
call printHelp(filename).
The code isn't commented, but you should be able to see what's happening,
it is very straigtforward. If you can't decipher something drop me a
note.
In the HeLP files, you insert lines containing % sign followed by the
topic (read: filename) of the entries you want to have displayed in
the menu. Add a space and then enter text to describe it.
For example, an exceprt from a help file:
%FILES This menu item will display FILES.HLP
%DOHELP This entry will re-show the main help file
%FOO Help for idiots... :-)
etc...
The file nsame will be padded out to 8 characters and a letter inside
square brackets will be added. The above will format into:
[a] FILES     This menu item will display FILES.HLP
[b] DOHELP    This entry will reshow the main help file
[c] FOO       Help for idiots... :-)
And then the prompt asking for a choice will appear. Every help file
can contain these entries, and there is no limit to the depth that
this routine can display menus.  If there are no % signs in the help
file then no prompt for a choice is printed (cause no choices were
displayed, right?).  I believe these are all the changes I have made,
I put these routines in MISC.C...
Paul Gauthier
*/
#ifndef NORMAL_HELP
#include "ctdl.h"
extern CONFIG    cfg;
extern logBuffer logBuf;
extern char      *NoFileStr;
extern char      outFlag;
extern char      haveCarrier;
extern char      onConsole;
extern char      whichIO;
extern MessageBuffer     msgBuf;
/************************************************************************/
/*    printHelp() does a tree structured help tutorial                  */
/************************************************************************/
int printHelp(char *filename)
  {
  FILE *file;
  char nextfile[NAMESIZE + 10];
  char next= TRUE;
  char list[26][NAMESIZE + 10];
  char plist[2];
  char nlist= 0;
  int toReturn, errcount;
  SYS_FILE fn;
  extern char *READ_TEXT;
  char hit;
  toReturn= TRUE;
  strCpy(nextfile, filename);
  while (next && toReturn != ERROR)
    {
    nlist= 0;
    makeSysName(fn, nextfile, &cfg.homeArea);
    if ((file = safeopen(fn, READ_TEXT)) == NULL)
      {
      mPrintf(NoFileStr, nextfile);
      toReturn= ERROR;
      sPrintf(msgBuf.mbtext, "Missing Help file: %s.",fn);
      aideMessage(NULL,FALSE);

      }
    else
      {
      getFileChar(file, 1);
      if (outFlag != IMPERVIOUS) outFlag = OUTOK;
      Output_Citadel_Message("HOTHLP",NULL,NULL,NULL);
      while (getHelpNames(file, list[nlist], nlist) && nlist < 25)
      nlist++;
      nlist--;
      if (nlist == -1)
        {
        next= 0;

        }
      else
        {
        char ccx;
        outFlag = OUTOK;

        Output_Citadel_Message("ENTRTN",NULL, NULL, NULL);
        if (nlist == 0)
          {
          Output_Citadel_Message("ENTLOC",NULL, NULL, NULL);
          }
        else
          {
          plist[0] = 'a' + nlist;
          plist[1] = '\0';
          Output_Citadel_Message("ENTLET", (long)&plist[0],NULL, NULL);
          };
        next = errcount = 0;
        while (onLine() && !next && (hit= toUpper( (ccx=modIn()))) != '\r' && hit != '\n')
          {
          if (hit - 'A' < 0 || hit - 'A' > nlist)
            {
            /*    mPrintf("\b \b");     */
            /*    oChar('\b');
            oChar(' ');
            oChar('\b'); */
            if (++errcount > 5 && whichIO == MODEM)
            HangUp(FALSE);

            }
          else
            {
            mPrintf("%c\n \n", hit);
            strCat(list[hit - 'A'], ".HLP");
            strCpy(nextfile, list[hit- 'A']);
            next= TRUE;
            errcount = 0;

            }

          }

        }
      fclose(file);

      }

    }
  return toReturn;

  }
static char PB = 0;
char getFileChar(FILE *file, int init)
  {
  static int aloop= -1;
  static char line[MAXWORD];
  char ret;
  if (init)
    {
    aloop= -1;
    return (char) aloop;

    }
  if (PB != 0)
    {
    ret = PB;
    PB = 0;
    return ret;

    }
  if (aloop == -1)
    {
    fgets(line, MAXWORD, file);
    aloop= 0;

    }
  input:
  if ((ret= line[aloop++]) == 0)
    {
    if(!fgets(line, MAXWORD, file))
    return 0;
    aloop= 0;
    goto input;

    }
  if (outFlag == OUTSKIP) ret= 0;
  return ret;

  }
#define INT_PTR         0
#define CHAR_PTR        1
#define FUNC_PTR        2
static struct
  {
  char *ourname;
  char type;
  union
    {
    UNS_16 *where;
    char   *addr;
    void   (*funcptr)(char *);

    }
  goo;

  }
VarNames[] =
  {
    {    "nodetitle",  INT_PTR, &cfg.nodeTitle     },
    {    "nodename",   INT_PTR, &cfg.nodeName     },
    {    "nodedomain", INT_PTR, &cfg.nodeDomain     },
    {    "nodeid",     INT_PTR, &cfg.nodeId     },
    {    "baseroom",   INT_PTR, &cfg.bRoom     },
    {    "mainfloor",  INT_PTR, &cfg.MainFloor     },
    {    "variantname",CHAR_PTR,(unsigned short *)VARIANT_NAME     },
    {    "sysopname",  CHAR_PTR,(unsigned short *)cfg.SysopName     },
    {    "ulprotocols",FUNC_PTR,(unsigned short *)UpProtsEnglish     },
    {    "dlprotocols",FUNC_PTR,(unsigned short *)DownProtsEnglish     },
    {    "doorlist",   FUNC_PTR,(unsigned short *)DoorHelpListing     },

  };
getHelpNames(FILE *file, char *name, char count)
  {
  char work[20], c, *t;
  char buf[MAXWORD + 1];
  int loop= 0, i;
  extern MessageBuffer msgBuf;
  SYS_FILE fn;
  round:
  while((buf[loop]= getFileChar(file, 0)) != '%' && buf[loop] && loop < MAXWORD-1)
    {
    if (buf[loop] == '^')
      {
      if (loop > MAXWORD - 50)
        {
        buf[loop] = 0;
        loop= 0;
        mPrintf("%s", buf);

        }
      i = 0;
      while(isalpha(work[i] = getFileChar(file, 0)) &&
      i < (sizeof work) - 1)
      i++;
      c = work[i];
      work[i] = 0;
      /* printf("\nSearching on '%s'.\n", work); */
      for (i = 0; i < NumElems(VarNames); i++)
      if (strCmpU(VarNames[i].ourname, work) == SAMESTRING)
      break;
      if (i < NumElems(VarNames))
        {
        switch (VarNames[i].type)
          {
          case INT_PTR:
          t = cfg.codeBuf + *VarNames[i].goo.where;
          break;
          case CHAR_PTR:
          t = VarNames[i].goo.addr;
          break;
          case FUNC_PTR:
          t = msgBuf.mbtext;
          (*VarNames[i].goo.funcptr)(msgBuf.mbtext);
          if (strLen(t) + loop > sizeof buf - 1)
            {
            buf[loop] = 0;
            mPrintf("%s", buf);
            loop = 0;
            if (strLen(msgBuf.mbtext) > sizeof buf)
              {
              mPrintf("%s", msgBuf.mbtext);
              msgBuf.mbtext[0] = 0;

              }

            }
          break;

          }
        buf[loop] = 0;
        strcat(buf, t);
        loop = strLen(buf);

        }
      else
        {
        for (loop++, i = 0; work[i] && loop < MAXWORD - 1;
        loop++, i++)
        buf[loop] = work[i];

        }
      buf[loop++] = c;

      }
    /*
    * prevent words from being cut in half -- occasionally screws up
    * formatting if we don't do this in long help files.
    */
    else if (buf[loop] == ' ' && loop > MAXWORD - 12)
      {
      break;

      }
    else
    loop++;

    }
  if (!buf[loop])
    {
    mPrintf("%s", buf);
    return FALSE;

    }
  /*     if(loop == MAXWORD-1) */
  if (buf[loop] != '%')
    {
    buf[loop+1]= 0;
    loop= 0;
    mPrintf("%s", buf);
    goto round;

    }
  buf[loop]= 0;
  outFlag = OUTOK;
  mPrintf("%s", buf);
  loop= 0;
  while((name[loop]= getFileChar(file, 0)) != ' ' && name[loop] && loop <= 7)
  loop++;
  name[loop]= 0;
  sPrintf(work, "%s.hlp", name);
  makeSysName(fn, work, &cfg.homeArea);
  if (access(fn, 0) != 0)
    {
    /*
    * don't show the file name or its comment -- so the unwary won't
    * stumble into nothingness.
    */
    while ((c = getFileChar(file, 0)) != 0 && c != '\n')
    ;
    loop= 0;
    buf[0]= 0;
    /*
    * This bizarre little if with a pushback is the result of noticing
    * when help files are missing, the help file trying to find them
    * will pile up the leading spaces which characterize most help files
    * in the " %MYHELP" stuff.  This makes the listing of help files to
    * jump to "lumpy."  The if right here takes care of that by peeking
    * ahead and killing a leading space if it's there.  This is obviously
    * not a perfect solution, but hopefully the imperfections will rarely
    * show up.
    */
    if ((c = getFileChar(file, 0)) != ' ')
    PB = c;
    goto round; /* eeeeeeeyyyyyyyuuuuuuuuuucccccccckkkk! */

    }
  mPrintf("[%c] %-8s  ", 'a' + count, name);
  return TRUE;

  }
/**************************** EOS (end of source) ****************************/
#else
/************************************************************************/
/*    printHelp() does a tree structured help tutorial                  */
/************************************************************************/
int printHelp(char *filename)
  {
  tutorial(filename, 1);

  }
#endif
