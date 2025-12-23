/*
*       MsgNFmt.c
*
* Prints a message in network format to blind target.
*/
/*
*       history
*
* 89Apr27 HAW Bad interaction of routemail and Other Recipients
* 89Feb05 HAW  Created.
*/
#include "ctdl.h"
/*
*       contents
*
* prNetStyle()    sends a msg in net format.
* NetCC()     write out Other Recipient fields
* NetOverride()   write out recipient override fields
*/
extern logBuffer logTmp;  /* Person buffer  */
extern int       callSlot;
char      netDebug = FALSE;
extern FILE     *netLog;
extern NetBuffer netBuf, netTemp;
extern SListBase MailForward;
extern CONFIG    cfg; /* Lots an lots of variables    */
extern MessageBuffer   msgBuf;
/* void NetOverride(); */
char   PrTransmit = TRUE; /* TRUE if writing to comm line */
/* FALSE if writing to disk */
int  (*NetCharSource)(void) = getNetChar;
static char Overrode;
#define WriteCC     WriteField
#define NetOverride   WriteField
void WriteField();
#define CC    0
#define OVERRIDE  1
#define NONE    2
char FieldType;
/*
* NetPrintTarget allows us to blindly choose what to do with
* the output.
*/
void (*NetPrintTarget)(char *format, ...);
static char *GoingFor;
/*
* prNetStyle()
*
* This sends a msg in NET format.  Target can be either disk or comm line.
*/
void prNetStyle(int NotMsgBase, int (*method)(int c), char GetMsg,
char *TargetSystem)
  {
  ForwardMail *address;
  long  val;
  int   c;
  int   (*SourceFn)(void);
  char  *rover;
  extern char inNet;
  GoingFor = TargetSystem;
  /*
  * Typically, we'll be getting a message to transmit from the
  * message base.  However, sometimes we don't.  In those very rare
  * cases we'll use the NetCharSource POINTER to find out where
  * to get the stuff.  This defaults to getNetChar, which is used
  * to read virtual room messages, but is also occasionally used to
  * read STadel routemail files to convert them to our route mail
  * files.
  */
  SourceFn = (NotMsgBase == 0) ? getMsgChar : NetCharSource;
  /* fill in local node in origin fields if local message: */
  if (!msgBuf.mborig[ 0])
  strCpy(msgBuf.mborig,  cfg.nodeId + cfg.codeBuf  );
  if (!msgBuf.mboname[0])
    {
    strCpy(msgBuf.mboname, cfg.nodeName + cfg.codeBuf);
    strCpy(msgBuf.mbdomain, cfg.nodeDomain + cfg.codeBuf);

    }
  /* Convert # to 8-bit Citadel style for compatibility   */
  if (!msgBuf.mbsrcId[0])
    {
    val = atol(msgBuf.mbId);
    sPrintf(msgBuf.mbsrcId, "%ld %ld",
    (val & 0xFFFF0000l) >> 16, val & 0xFFFFl);

    }
  if (netDebug)
    {
      MessageBuffer *msg = &msgBuf;
      splitF(netLog, "prNetStyle - message sent");
      splitF(netLog, "Message %6d Sector ID: %6d Author:%s\n",  msg->mbheadChar, msg->mbheadSector, msg->mbauth);
      splitF(netLog, "Date:%20s Time:%20s Local Id:%20s\n",  msg->mbdate,  msg-> mbtime,  msg-> mbId);
      splitF(netLog, "Human:%20s ID:%20s Room:%20s\n",  msg->mboname,  msg->mborig,  msg->mbroom);
      splitF(netLog, "Origin:%20s To:%20s\n",  msg->mbsrcId,  msg->mbto);
      splitF(netLog, "Route:%s\n", msg->mbaddr);
      splitF(netLog, "OtherNet:%s\n",msg->mbOther);
      splitF(netLog, "reply:%20s Domain:%20s\n",msg->mbreply, msg->mbdomain);
    };

  /* send header fields out: */
  if (msgBuf.mbauth[ 0])  (*NetPrintTarget)("A%s", msgBuf.mbauth );
  if (msgBuf.mbdate[ 0])  (*NetPrintTarget)("D%s", msgBuf.mbdate );
  if (msgBuf.mbtime[ 0])  (*NetPrintTarget)("C%s", msgBuf.mbtime );
  (*NetPrintTarget)("N%s", msgBuf.mboname);
  (*NetPrintTarget)("O%s", msgBuf.mborig );
  if (msgBuf.mbdomain[0])
  (*NetPrintTarget)("X%s", msgBuf.mbdomain);
  if (msgBuf.mbroom[ 0])  (*NetPrintTarget)("R%s", msgBuf.mbroom );
  (*NetPrintTarget)("S%s", msgBuf.mbsrcId);
  if (msgBuf.mbOther[0])  (*NetPrintTarget)("P%s", msgBuf.mbOther);
  FieldType = CC;
  RunList(&msgBuf.mbCC, WriteCC); /* write out Who Else fields */
  Overrode = FALSE;
  /*
  * this if handles a piece of user-generated route mail
  * in this situation there are no overrides and they must be
  * generated from the mbCC list.
  */
  if (!PrTransmit && inNet == NON_NET)
    {
    /*
    * If mbOverride has stuff, this is caused by forwarding set up
    * by one or more of the recipients of the mail.  This may
    * include mail initially locally forwarded ...
    */
    if (GetFirst(&msgBuf.mbInternal) == NULL)
    RunList(&msgBuf.mbCC, NetCC);
    else
    RunList(&msgBuf.mbInternal, NetCC);

    }
  /*
  * this else translates to route mail being written to disk caused by
  * mail forwarding during a network session -- that's the only case
  * that'll satisfy this if. In this case the only override generation can
  * come from the overrides already in place -- they are the only
  * recipients on this system, so only they can generate a mail forwarding
  * request.
  */
  else if (!PrTransmit && inNet != NON_NET)
    {
    RunList(&msgBuf.mbOverride, NetCC);
    RunList(&msgBuf.mbInternal, NetCC);

    }
  /*
  * this else translates to network transmission of "normal" mail --
  * that is, mail coming from the message base and therefore not
  * bearing overrides.  Any overrides must therefore be generated
  * from the Who Else list.
  */
  else if (!NotMsgBase && PrTransmit && inNet != NON_NET)
    {
    RunList(&msgBuf.mbCC, NetCC);

    }
  /*
  * this else covers route mail about to be transmitted on the net.
  * In this case the override fields already exist (from the originating
  * system) so we just write them out, and everything's cool.
  */
  else if (NotMsgBase && PrTransmit && inNet != NON_NET)
    {
    FieldType = OVERRIDE;
    RunList(&msgBuf.mbOverride, NetOverride);

    }
  if (msgBuf.mbaddr[ 0] && (!PrTransmit || Overrode))
    {
    (*NetPrintTarget)("Q%s", msgBuf.mbaddr );

    }
  /*
  * Basically, only do possible 'T' transformations if we're not outgoing
  * route mail and we're not writing route mail to disk generated by
  * incoming mail hitting a person with mail forwarding on.
  */
  if (!(NotMsgBase && PrTransmit && inNet != NON_NET))
    {
    /*
    * we use this control structure for backward compatability:
    * systems which do not recognize the 'w' field of net messages.
    */
    if (Overrode)
      {
      if (msgBuf.mbto[  0])
      (*NetPrintTarget)("T%s", msgBuf.mbto   );
      if (SearchList(&msgBuf.mbInternal, msgBuf.mbto) == NULL)
        {
        if (msgBuf.mbaddr[0])
        sPrintf(msgBuf.mbtext, "%s@%s", msgBuf.mbto, msgBuf.mbaddr);
        else
        strCpy(msgBuf.mbtext, msgBuf.mbto);
        NetCC(msgBuf.mbtext);

        }

      }
    else
      {
      if (msgBuf.mbto[   0])
        {
        address = SearchList(&MailForward, msgBuf.mbto);
        (*NetPrintTarget)("T%s",
        (address != NULL && strCmpU(address->System, GoingFor)
        == SAMESTRING) ? address->Alias : msgBuf.mbto);

        }
      else (*NetPrintTarget)("T%s", msgBuf.mbto);

      }

    }
  else if (msgBuf.mbto[0]) (*NetPrintTarget)("T%s", msgBuf.mbto);
  FieldType = NONE;
  RunList(&msgBuf.mbForeign, WriteField);
  /* send message text proper: */
  (*method)('M');
  if (GetMsg)
    {
    do
      {
      c = (*SourceFn)();
      if (c=='\n')  c='\r';
      if (!(*method)(c)) break;

      }
    while (c && c != -1);

    }
  else
    {
    for (rover = msgBuf.mbtext; *rover; rover++)
    (*method)(*rover);
    (*method)(0);

    }

  }
/*
* NetCC()
*
* This function is used to write out Other Recipient fields and sometimes
* to write override fields (only when system origin is this system, not
* another, however).
*/
void NetCC(char *d)
  {
  ForwardMail *address;
  label person;
  char  system[(2 * NAMESIZE) + 2];
  switch (SepNameSystem(d, person, system, &netTemp))
    {
    case IS_SYSTEM:
    if (strCmpU(system, GoingFor) == SAMESTRING ||
    strCmpU(LocalName(system), GoingFor) == SAMESTRING)
      {
      Overrode = TRUE;
      (*NetPrintTarget)("w%s", person);

      }
    break;
    case NOT_SYSTEM:  /* forward to this system??? */
    if ((address = SearchList(&MailForward, person)) != NULL &&
    strCmpU(address->System, GoingFor) == SAMESTRING)
      {
      Overrode = TRUE;
      (*NetPrintTarget)("w%s", address->Alias);

      }
    break;

    }

  }
#ifdef OLD_STYLE
/*
* WriteCC()
*
* This function is used to write out a 'W' field.
*/
static void WriteCC(char *d)
  {
  (*NetPrintTarget)("W%s", d);

  }
/*
* NetOverride()
*
* This function is used to write out recipient override fields ('w').
*/
static void NetOverride(char *d)
  {
  (*NetPrintTarget)("w%s", d);

  }
#else
#endif
void WriteField(char *d)
  {
  (*NetPrintTarget)((FieldType == OVERRIDE) ? "w%s" :
  (FieldType == CC) ? "W%s" : "%s", d);

  }
