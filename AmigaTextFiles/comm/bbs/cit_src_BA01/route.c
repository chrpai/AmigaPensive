/*
*       Route.C
*
* Some of the C86Net routemail code.
*/
#include "ctdl.h"
/*
*       history
*
* 89Feb?? HAW  Created.
*/
/*
*                              contents
*
* netRouteMail()    Incoming route mail
* MakeRouted()    ST route mail => C86Net
*/
int    RouteSlot;
char   RouteMailForHere;
int    RouteToDirect = ERROR;
static int  RWorkBuf[7];
char RCount, SCount;
extern int       TransProtocol;
extern char      TrError;
extern PROTO_TABLE Table[];
extern CONFIG    cfg;
extern NetBuffer netBuf, netTemp;
extern NetTable  *netTab;
extern FILE      *upfd, *netMisc, *netLog;
extern int       thisNet;
extern MessageBuffer   msgBuf;
extern logBuffer logBuf;
extern long ByteCount, EncCount;
extern char logNetResults;
extern char netDebug;

#define BAD_ROUTE "%s did not recognize %s (%s) for routed mail (%s)."
#define NodeDisabled(x) (!(netTab[x].ntMemberNets & ALL_NETS))
static char ParseSTRoute(char *str, char **TargetSystem, char **Domain,
char **UserName);
static void prStStyle(int mode, char *Name, int  (*M)(int c), char *Domain);
extern int (*NetCharSource)(void);
/**** These functions handle incoming route mail. ****/
/*
* netRouteMail() should be called in the Net Receive routines, in the big
* case statement.
*/
int RMcount = 0;                /* Incoming RoutMail count for us       */
/*
* netRouteMail()
*
* The caller has asked us to route Mail somewhere.  We determine if we want
* to accept the mail or not, and if we do then we do, in fact, receive it.
*/
void netRouteMail(struct cmd_data *cmds)
  {
  DOMAIN_FILE fn;
  label       temp;
  int   val;
  char  MailForUs = FALSE, bad, dup;
  extern char *APPEND_ANY, PosId;
  label       Name, Id;
  /*
  * Do we do net routing?  At all?  From this node?  To the indicated
  * node?  Even if we don't do routing per se, we should accept routed
  * mail intended for this system, which is handled by RouteHere().
  * Also, we do not route to disabled nodes.
  */
  if (cfg.BoolFlags.debug)
    {
    short i;
    splitF(netLog, "netRouteMail(%08.8x)\n",cmds);
    splitF(netLog, " struct cmd ");
    splitF(netLog, "   {\n");
    splitF(netLog, "   command=%x\n",cmds->command);
    for(i=0;i<4;i++) splitF(netLog, "   fields[%d]:%s\n",i,cmds->fields[i]);
    splitF(netLog, "   }\n");
    splitF(netLog,"netBuf.nbflags.RouteFor(%x) RouteMail(%x)\n",
    (int)cfg.BoolFlags.RouteMail,(int)netBuf.nbflags.RouteFor);
    };
  bad = (!PosId ||
  (!(MailForUs = RouteHere(cmds->fields[0], cmds->fields[1],
  cmds->fields[2]))) &&
  (!cfg.BoolFlags.RouteMail || !netBuf.nbflags.RouteFor));
  if (!bad && !MailForUs)
    {
    if (!FindTheNode(cmds->fields[0], ""))
      {
      if (strLen(cmds->fields[2]) != 0)
        {
        bad = ((val=DomainMailFileName(fn, cmds->fields[2],
        cmds->fields[0], cmds->fields[1])) == REFUSE);

        }
      else val = (MailForUs) ? OURS : LOCALROUTE;
      if (!bad && val == LOCALROUTE)
        {
        bad = (!AcceptRoute(cmds->fields[0],"") ||
        NodeDisabled(RouteSlot));

        }
      /* trying to route mail without domain we don't know about? */
      if (bad && strLen(cmds->fields[2]) == 0)
        {
        if (SystemInSecondary(cmds->fields[1], cmds->fields[2], &dup))
        if (!dup)
          {
          bad = FALSE;
          val=DomainMailFileName(fn, cmds->fields[2],
          cmds->fields[0], cmds->fields[1]);

          }

        }

      }
    else val = LOCALROUTE;

    }
  else if (MailForUs) val = OURS;
  if (bad)
    {
    if( logNetResults )
    splitF(netLog, "Rejecting %s (%s)\n", cmds->fields[1], cmds->fields[0]);
    reply(BAD, "not routing to this node for you");
    return;

    }
  if (val == LOCALROUTE)
    {
    strCpy(Name, netTemp.netName);
    strCpy(Id, netTemp.netId);
    FindRouteSlot();

    }
  if (!MailForUs)
  MailForUs = val == OURS;
  if (MailForUs)
    {
    sPrintf(temp, "rmail.%d", RMcount++);
    makeSysName(fn, temp, &cfg.netArea);

    }
  else if (val == LOCALROUTE)
    {
    /* Ugly ugly kludge until we figure out how nbHiRouteInd is wrong */
    if (netTemp.nbHiRouteInd < 0) netTemp.nbHiRouteInd = 0;
    sPrintf(temp, "R%d.%d", RouteSlot, netTemp.nbHiRouteInd++);
    makeSysName(fn, temp, &cfg.netArea);

    }
  else
    {
    strCpy(Name, cmds->fields[1]);
    strCpy(Id, cmds->fields[0]);

    }
  if ((upfd = safeopen(fn, APPEND_ANY)) == NULL)
    {
    reply(BAD, "internal error");
    splitF(netLog, "internal error, couldn't open %s\n", fn);
    return;

    }
  if (!MailForUs)
    {
    strCpy(Name, UseNetAlias(Name, TRUE));
    fprintf(upfd, "%-20s", Id);
    putc(0, upfd);
    fprintf(upfd, "%-20s", Name);
    putc(0, upfd);

    }
  if( logNetResults )
    splitF(netLog, "Route mail for %s/%s/%s.\n", cmds->fields[1],Id,Name);
  if (!MailForUs)
  StartEncode(putFLChar);
  if (ITL_StartRecMsgs(fn, TRUE, FALSE, (!MailForUs) ? Encode : NULL)
  != ITL_SUCCESS)
    {
    switch (val)
      {
      case LOCALROUTE:
      netTemp.nbHiRouteInd--; break;
      case DOMAINFILE:
      DomainFileAddResult(cmds->fields[2], "", "", DOMAIN_FAILURE); break;

      }
    MailForUs = FALSE;
    splitF(netLog, "Problem receiving routemail\n");

    }
  else
    {
    /*
    * Now we note the fact that we have received routed mail.
    */
    switch (val)
      {
      case LOCALROUTE:
      netTemp.nbflags.HasRouted = TRUE;
      putNet(RouteSlot, &netTemp);
      break;
      case DOMAINFILE:
      DomainFileAddResult(cmds->fields[2], Name, cmds->fields[0],
      DOMAIN_SUCCESS); break;

      }

    }
  if (MailForUs)
  RouteMailForHere = TRUE;

  }
/*
* AcceptRoute()
*
* Will we route to the given node?
*/
char AcceptRoute(char *id, char *name)
  {
  if (cfg.BoolFlags.debug)
    {
    splitF(netLog, "AcceptRoute(%s,%s)=%s\n",id,name
    , (( netTemp.nbflags.RouteTo) ? "TRUE ":"FALSE"));
    };
  if (!FindTheNode(id, name)) return FALSE;
  return (char)netTemp.nbflags.RouteTo;

  }
/*
* FindTheNode()
*
* This function will see if the node exists.  It will leave the index in
* RouteSlot and return TRUE if found, FALSE otherwise.  This does not try
* to utilize the secondary node lists.
*/
char FindTheNode(char *id, char *name)
  {
  if (cfg.BoolFlags.debug)
    {
    splitF(netLog, "FindTheNode(%s,%s)\n",id,name);
    };
  if ((RouteSlot = searchNet(id, &netTemp)) != ERROR) return TRUE;
  if (strLen(name) == 0) return FALSE;
  if ((RouteSlot = searchNameNet(name, &netTemp)) == ERROR &&
  (RouteSlot = searchNameNet(UseNetAlias(name,FALSE), &netTemp))==ERROR)
  return FALSE;
  return TRUE;

  }
/*
* FindRouteSlot()
*
* This function will find the slot of the routing node.  If found, netTemp
* will contain the new slot, as will RouteSlot, and RouteSlot will be
* returned.
*/
int FindRouteSlot()
  {
  /*
  * first check to see if we're the direct (final) link, either
  * expressly or because the route is outdated.
  */
  if (!DirectRoute(&netTemp))            /* expressly?   */
  getNet((RouteSlot = netTemp.nbRoute), &netTemp);
  return RouteSlot;

  }
/*
* DirectRoute()
*
* This function will discover if we directly or indirectly talk to this node.
*/
char DirectRoute(NetBuffer *n)
  {
  if (cfg.BoolFlags.debug)
    {
    splitF(netLog, "DirectRoute:nbRoute %d ", n->nbRoute );
    };
  return (char) (n->nbRoute == -1 ||             /* expressly?   */
  n->nbRoute >= cfg.netSize ||  /* outdated? */
  !netTab[n->nbRoute].ntflags.in_use ||    /* outdated?    */
  netTab[n->nbRoute].ntGen != n->nbRouteGen);/* outdated?*/

  }
/**** These functions handle outgoing route mail. ****/
/*
* RouteOut()
*
* This is the manager of sending outgoing route mail to the current node.  It
* must handle error conditions.
*/
void RouteOut()
  {
  char            abort = FALSE, failed = FALSE;
  int             rover, result;
  label           temp, Tid, Tname, ThisId;
  SYS_FILE        fn;
  extern char     *READ_ANY, OverRides;
  extern AN_UNSIGNED RecBuf[];
  if (netBuf.nbflags.Stadel)
    {
    RouteToDirect = 0;
    return;

    }
  normId(netBuf.netId, ThisId);
  for (rover = 0; rover <= netBuf.nbHiRouteInd && !abort; rover++)
    {
    sPrintf(temp, "R%d.%d", thisNet, rover);
    makeSysName(fn, temp, &cfg.netArea);
    if ((result = SendRouteMail(fn, "", Tid, Tname, FALSE)) == GOOD_SEND)
    unlink(fn);
    else if (result == REFUSED_ROUTE)
      {
      /*
      * Bad return.  Could be
      *   a) system is not compatible at this level, or
      *   b) system doesn't know about target
      */
      if (strCmpU(ThisId, Tid) == SAMESTRING)
        {
        if( netDebug && logNetResults )
          splitF(netLog, "Rerouting to use normal mail.\n");
        if (RouteToDirect == -1)
          {
          RouteToDirect = rover;

          }

        }
      else
        {
        sPrintf(msgBuf.mbtext, BAD_ROUTE, netBuf.netName,
        Tname, Tid, RecBuf + 1);
        netResult(msgBuf.mbtext);
        failed = TRUE;

        }

      }
    else failed = TRUE; /* something failed, dunno what */

    }
  if (!abort && !failed)
    {
    netBuf.nbflags.HasRouted = FALSE;
    netBuf.nbHiRouteInd      = 0;

    }

  }
/*
* SendRouteMail()
*
* This function will send route mail.
*/
char SendRouteMail(char *filename, char *domainname, char *Tid, char *Tname,
char LocalCheck)
  {
  struct cmd_data cmds;
  char work[(2 * NAMESIZE) + 10];
  label ThisId;
  normId(netBuf.netId, ThisId);
  if ((netMisc = safeopen(filename, READ_ANY)) != NULL)
    {
    getMsgStr(getNetChar, cmds.fields[0], NAMESIZE);
    getMsgStr(getNetChar, cmds.fields[1], NAMESIZE);
    if (!normId(cmds.fields[0], Tid) || strLen(Tid) == 0)
    strCpy(cmds.fields[0], "  ");
    else
    strCpy(cmds.fields[0], Tid);
    if (LocalCheck)
    if (!netBuf.nbflags.Stadel && strCmpU(Tid,ThisId) != SAMESTRING)
      {
      fclose(netMisc);
      return REFUSED_ROUTE; /* actually, just a lie */

      }
    NormStr(cmds.fields[1]);
    strCpy(Tname, cmds.fields[1]);    /* reporting purposes */
    strCpy(cmds.fields[2], domainname); /* just for luck        */
    cmds.command = ROUTE_MAIL;
    if( logNetResults )
      {
      if (strLen(domainname) != 0 )
        splitF(netLog, "Routing mail to %s _ %s/%s/%s\n", cmds.fields[1],
        domainname,
        (Tid != NULL) ? Tid : "NULL", (Tname != NULL) ? Tname : "NULL" );
      else
        splitF(netLog, "Routing mail to %s/%s/%s\n", cmds.fields[1],
        (Tid != NULL) ? Tid : "NULL", (Tname != NULL) ? Tname : "NULL" );
      };
    if (LocalCheck || sendNetCommand(&cmds, "Route Mail"))
      {
      if (LocalCheck || ITL_SendMessages())
        {
        StartDecode(ReadRoutedDest);
        RCount = SCount = 0;
        NetCharSource = ReadRouted;
        sPrintf(work,
        (strLen(domainname) != 0) ? "%s _ %s" : "%s%s",
        cmds.fields[1], domainname);
        while (getMessage(ReadRouted, TRUE, FALSE, TRUE))
        if (netBuf.nbflags.Stadel)
        prStStyle(1, Tname, sendITLchar, domainname);
        else
        prNetStyle(1, sendITLchar, TRUE, work);
        NetCharSource = getNetChar;
        fclose(netMisc);
        if (!LocalCheck) ITL_StopSendMessages();
        if (cfg.BoolFlags.debug)
        splitF(netLog, "Encoded %ld bytes, sent %ld bytes.\n", EncCount, ByteCount);
        if (TrError == TRAN_SUCCESS)
        return GOOD_SEND;
        else
        return UNKNOWN_ERROR;

        }

      }
    else
      {
      if( logNetResults && netDebug )splitF(netLog, "RouteMail rejection!\n");
      fclose(netMisc);
      return REFUSED_ROUTE;

      }

    }
  return NO_SUCH_FILE;

  }
/*
* AdjustRoute()
*
* This function adjusts the routed files names.  Basically, it finds "holes"
* in the sequence of numerical filenames and adjusts names to fill those
* holes.
*/
void AdjustRoute()
  {
  int      rover, next;
  label    temp;
  SYS_FILE fn, fn2;
  rover = 0;
  while (rover < netBuf.nbHiRouteInd)
    {
    sPrintf(temp, "R%d.%d", thisNet, rover);
    makeSysName(fn, temp, &cfg.netArea);
    if (access(fn, 0) != 0)
      {
      next = rover + 1;
      do
        {
        sPrintf(temp, "R%d.%d", thisNet, next++);
        makeSysName(fn2, temp, &cfg.netArea);

        }
      while (access(fn2, 0) != 0 && next <= netBuf.nbHiRouteInd);
      if (access(fn2, 0) != 0)
        {
        if (rover == 0)
          {
          netBuf.nbflags.HasRouted = FALSE;
          netBuf.nbHiRouteInd = 0;

          }
        else
        netBuf.nbHiRouteInd = rover - 1;
        break;

        }
      else
        {
        rename(fn2, fn);
        rover++;

        }

      }
    rover++;

    }
  /* I think* this is right. */
  if (rover == 0)
    {
    netBuf.nbflags.HasRouted = FALSE;
    netBuf.nbHiRouteInd = 0;

    }

  }
/*
* SendRoutedAsLocal()
*
* This function will reroute route mail to use local mail.
*/
int SendRoutedAsLocal()
  {
  label ThisId, Tid, Name, temp;
  SYS_FILE fn;
  char finished = FALSE;
  int  toReturn = 0, result;
  extern char OverRides;
  if (RouteToDirect == -1 && !netBuf.nbflags.Stadel) return 0;
  normId(netBuf.netId, ThisId);
  do
    {
    sPrintf(temp, "R%d.%d", thisNet, RouteToDirect++);
    makeSysName(fn, temp, &cfg.netArea);
    if ((result = SendRouteMail(fn, "", Tid, Name, TRUE)) == GOOD_SEND)
      {
      unlink(fn);
      toReturn++;

      }
    else if (result == NO_SUCH_FILE)
    finished = TRUE;

    }
  while (!finished);
  return toReturn;

  }
/*
* ReadRoutedDest()
*
* This work function will help read encrypted (style 1) data.
*/
int ReadRoutedDest(int c)
  {
  RWorkBuf[RCount++] = c;
  return TRUE;

  }
/*
* ReadRouted()
*
* This function will read a routed char for getMessage().
*/
int ReadRouted()
  {
  int c;
  if (RCount != SCount)
  return RWorkBuf[SCount++];
  RCount = SCount = 0;
  while (SCount == RCount && (c = fgetc(netMisc)) != EOF)
  Decode(c);
  if (RCount != SCount)
  return RWorkBuf[SCount++];
  if (c == EOF) StopDecode();
  if (RCount != SCount)
  return RWorkBuf[SCount++];
  return -1;

  }
/*
* prStStyle()
*
* This is used to handle problems inherent in STadel mail routing.
*/
static void prStStyle(int mode, char *Name, int (*M)(int c), char *Domain)
  {
  char work[4 * NAMESIZE];
  extern SListBase MailForward;
  ForwardMail *address;
  if (strCmpU(netBuf.netName, Name) != SAMESTRING)
    {
    if (!msgBuf.mbaddr[0])
      {
      if ((address = SearchList(&MailForward, msgBuf.mbto)) != NULL)
      strCpy(msgBuf.mbto, address->Alias);

      }
    if (strLen(Domain) != 0)
      {
      sPrintf(work, "%s.%s!%s", Name, Domain, msgBuf.mbto);
      strCpy(msgBuf.mbto, work);

      }
    else
      {
      sPrintf(work, "%s!%s", netBuf.netName, Name);
      NormStr(work);  /* shave off trailing blanks... */
      sPrintf(lbyte(work), "!%s", msgBuf.mbto);
      strCpy(msgBuf.mbto, work);

      }

    }
  if (strLen(msgBuf.mbdomain) != 0 && strLen(msgBuf.mboname) != 0)
    {
    sPrintf(work, "%s.%s!%s", msgBuf.mboname, msgBuf.mbdomain,
    msgBuf.mbauth);
    strCpy(msgBuf.mbauth, work);

    }
  prNetStyle(mode, M, TRUE, Name);

  }
/*
* MakeRouted()
*
* This function transforms an ST route mail message into C86Net.
*
* Format: <system>![<system>!...]<username> ... I hope!
*
* There should also be allowances for using domain addresses.
*/
void MakeRouted()
  {
  char  dup, *UserName, *TargetSystem, *Domain, work[(2 * NAMESIZE) + 6];
  label TheDomain, System;
  int   Slot, val = ERROR;
  /*
  * We parse the author field (which is the route back to the
  * author, form [<system>!...]<home system>!<username>.  TargetSystem
  * should have the name of the home system, UserName is obvious.  If
  * TargetSystem contains a '.', then the address is domain-defined.
  */
  printf("MakeRouted\n");
  if (ParseSTRoute(msgBuf.mbauth, &TargetSystem, &Domain, &UserName))
    {
    strCpy(msgBuf.mboname, TargetSystem);
    msgBuf.mborig[0] = 0;
    /*
    * Is this domain mail?  If so, just set the domain stuff up
    * correctly and don't worry whether we know this guy personally.
    */
    if (Domain != NULL)
      {
      strCpy(msgBuf.mbdomain, Domain);
      strCpy(msgBuf.mboname, TargetSystem);

      }
    else
      {
      /*
      * So now we find out if we know the name.  If we do, netTemp will
      * contain the appropriate net data.
      */
      if (IdStName(msgBuf.mboname) != ERROR)
      strCpy(msgBuf.mborig, netTemp.netId);
      strCpy(msgBuf.mbauth, UserName);

      }

    }
  /*
  * Now we parse the To field, which is potentially in the same format
  * as the Author field, above.  TargetSystem is the final target,
  * User name is the recipient.  If, for some reason, no target system
  * is specified, we assume we are the target.
  */
  if (!ParseSTRoute(msgBuf.mbto, &TargetSystem, &Domain, &UserName))
    {
    TargetSystem = cfg.nodeName + cfg.codeBuf;
    UserName = msgBuf.mbto;

    }
  if( logNetResults )
  if (Domain == NULL)
  splitF(netLog, "Routing mail to %s.\n", TargetSystem);
  else
  splitF(netLog, "Routing mail to %s _ %s.\n", TargetSystem, Domain);
  /* If we are the target, just deliver the damn message and get out. */
  if (RouteHere("", TargetSystem, Domain))
    {
    strCpy(msgBuf.mbto, UserName);
    putMessage(&logBuf);
    return;

    }
  /*
  * OK, so the Mail is not for our system.  Find out whom and put in
  * routing.
  */
  /*
  * If we can't figure out the target system, dump it and die.
  */
  if ((Slot = IdStName(TargetSystem)) == ERROR)
    {
    /* search secondary list now */
    if (Domain != NULL && strLen(Domain) != 0)
    sPrintf(work, "%s _ %s", TargetSystem, Domain);
    else
    strCpy(work, TargetSystem);
    if (!SystemInSecondary(work, TheDomain, &dup) || dup)
      {
      strCpy(System, TargetSystem);
      strCpy(msgBuf.mbto, UserName);
      if( logNetResults && netDebug)splitF(netLog, "Could not identify target '%s'.\n", TargetSystem);
      netMailOut(TRUE, System, "unknown", FALSE, 0);
      return ;

      }
    Domain = TheDomain;
    TargetSystem = work;

    }
  else TargetSystem = netTemp.netName;
  /* We know where, now to put it all together. */
  strCpy(msgBuf.mbto, UserName);
  if (!cfg.BoolFlags.RouteMail ||
  (val == LOCALROUTE && !netBuf.nbflags.RouteFor))
    {
    sPrintf(msgBuf.mbtext, "System %s tried to illegally route via you.\n",
    netBuf.netName);
    netResult(msgBuf.mbtext);
    return ;

    }
  #ifdef WANTED
  if (val == LOCALROUTE && !netTemp.nbflags.RouteTo)
    {
    sPrintf(msgBuf.mbtext, "System %s tried to route to %s.\n",
    netBuf.netName, netTemp.netName);
    netResult(msgBuf.mbtext);
    return ;

    }
  #endif
  if (Domain != NULL)
  netMailOut((Domain != NULL), TargetSystem, Domain, FALSE, Slot);
  if (val == LOCALROUTE && thisNet == Slot)
    {
    /* kludge fix if system is routing back to itself */
    netBuf.nbflags.HasRouted = netTemp.nbflags.HasRouted;
    netBuf.nbHiRouteInd++;

    }

  }
/*
* IdStName()
*
* This function will find the STadel route name in our lists. It leaves
* netTemp with the net element which matched, and returns the
* slot number.  It does limited translations for underscores.
*/
int IdStName(char *name)
  {
  int Slot;
  char *Alias;
  int StSearch(char *name);
  while ((Alias = strchr(name, ' ')) != NULL)
  *Alias = '_';
  if ((Slot = StSearch(name)) != ERROR) return Slot;
  if ((Alias = strchr(name, '_')) != NULL)
    {
    while ((Alias = strchr(name, '_')) != NULL)
    *Alias = ' ';
    Slot = StSearch(name);

    }
  return Slot;

  }
/*
* StSearch()
*
* This function will search the ctdlnet for the given node by name.  If the
* initial search fails, then the alias list is searched for an alias and
* another attempt is made using the result.
*/
int StSearch(char *name)
  {
  int Slot;
  if ((Slot = searchNameNet(name, &netTemp)) != ERROR)
  return Slot;
  if ((Slot = searchNameNet(UseNetAlias(name, FALSE), &netTemp)) != ERROR)
  return Slot;
  return ERROR;

  }
static label SearchResult;
static char  *SearchTarget, GetAlias;
/*
* UseNetAlias()
*
* This will find a usenet alias or the converse from ALIASES.SYS.
*/
char *UseNetAlias(char *Name, char FindAlias)
  {
  void *EatTrans();
  SListBase Dummy =
    {
    NULL, NULL, NULL, NULL, EatTrans

    };
  SYS_FILE fn;
  char *c, *WorkName;
  WorkName = strdup(Name);  /* use a work buffer */
  SearchResult[0] = 0;
  SearchTarget = WorkName;
  if (!FindAlias) while ((c = strchr(WorkName, ' ')) != NULL) *c = '_';
  makeSysName(fn, "aliases.sys", &cfg.roomArea);
  GetAlias = FindAlias;
  MakeList(&Dummy, fn, NULL); /* CHEAT!  WHEEEEEE! */
  free(WorkName);
  if (strLen(SearchResult) == 0) return Name;
  if (FindAlias) while ((c = strchr(SearchResult, '_')) != NULL) *c = ' ';
  return SearchResult;

  }
/*
* EatTrans()
*
* This digests a line of input from the ALIASES.SYS file, and, depending
* on the value of the GetAlias variable, checks the variable SearchTarget
* against either the alias or the name found on the input line.  If a
* match is found the other field value is then copied into SearchResult,
* a global variable.  Since this is only called from MakeList(), and we're
* not really making a list, this always returns NULL.  We use MakeList()
* just as a cheat.
*/
void *EatTrans(char *line)
  {
  char *c;
  NormStr(line);
  if ((c = strchr(line, ' ')) != NULL)
    {
    *c = 0;
    if (GetAlias)
      {
      /* check second field */
      if (strCmpU(c + 1, SearchTarget) == SAMESTRING)
        {
        strCpy(SearchResult, line);

        }

      }
    else
      {
      /* check first field */
      if (strCmpU(line, SearchTarget) == SAMESTRING)
        {
        strCpy(SearchResult, c + 1);

        }

      }

    }
  return NULL;

  }
/*
* ParseSTRoute()
*
* This parses a ST route path for recipient and target.  It can handle
* domain specifications, too.
*
* RETURNS:
*
*  o FALSE if this does not appear to have any STadel routing information
*    information in the field.
*
*  o TRUE if there did seem to be information and it was parsed out.
*
* str - the field containing the unparsed information.  This will be
* changed by this function.
*
* TargetSystem - will contain the target system name after parsing.
*
* Domain - will contain the domain name after parsing.
*
* UserName - will contain the user's name after parsing.
*/
static char ParseSTRoute(char *str, char **TargetSystem, char **Domain,
char **UserName)
  {
  if ((*UserName = strrchr(str, '!')) == NULL)
    {
    return FALSE;

    }
  **UserName = 0; /* now recipient's name does not interfere */
  (*UserName)++;    /* with system target */
  if ((*TargetSystem = strrchr(str, '!')) == NULL)
  *TargetSystem = str;
  else
  (*TargetSystem)++;    /* gets us past our search target */
  if ((*Domain = strrchr(str, '.')) != NULL)
    {
    **Domain = 0;
    (*Domain)++;    /* with system target */

    }
  return TRUE;

  }
