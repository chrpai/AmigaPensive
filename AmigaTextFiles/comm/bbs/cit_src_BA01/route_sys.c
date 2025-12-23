#include "ctdl.h"
#include "2ndfmt.h"
/*
 * Routing records - constructed from ROUTING.SYS.
 */
/**
   External Data
**/

extern MessageBuffer   msgBuf;
extern NetTable       *netTab;
extern NetBuffer       netTemp;
extern FILE           *netLog;
extern NetBuffer       netBuf;
extern CONFIG          cfg;
extern AN_UNSIGNED     RecBuf[SECTSIZE + 5];
extern int             thisNet;
extern int             TrError;


/**
  Type definition created and maintained here
**/
typedef struct
  {
  int  Target, Via;   /* system slots */
  char checked;       /* avoid loops */
  } Routing;

extern FILE             *netLog;
extern char              netDebug;


static int send_direct_mail(int which, char *name);


/**
     route.sys handling functions

  These functions will create a list from the file route.sys.
  The file format is:

  target : route

  target is a destination for a mail message.  We may have created
  it here on this system or it might have arrived from another system.

  route is a node on our nodelist.

  OnTarget    -
  EatRoute    -
  RoutingNode - Find the routine node for a given system
*/
static void *OnTarget( Routing *element, int *i );
static void *EatRoute( char *line );

SListBase Routes = { NULL, OnTarget, NULL, NULL, EatRoute };
/**
  RoutingNode

  This function finds the routing node for the given system slot, if any.
**/
int RoutingNode(int slot )
  {
  Routing *record;

  if ((record = SearchList(&Routes, &slot)) != NULL)
    {
    if( netDebug )
      {
      splitF(netLog, "RoutingNode(%d): Target Slot: %d  via: %d\n",
      slot, record->Target, record->Via);

      };
    return record->Via;

    }
    return slot;
}

/**
  OnTarget()

  This function helps find a routing record based on the Target field.
**/
static void *OnTarget(Routing *element, int *i)
{
    if (element->Target == *i)
      {
      if( netDebug )
        {
        splitF(netLog, "OnTarget(%08.8lx, %d): Target Slot: %d  via: %d\n",
        element, *i, element->Target, element->Via);
        };

      return element;
      };
    return NULL;
}

/**
  EatRoute()

  This function eats a line from ROUTING.SYS.
**/
static void *EatRoute(char *line)
{
    Routing *record;
    char *target, *via;
    int  targind, viaind;
    if( netDebug )
      {
      splitF(netLog, "EatRoute(%s)\n",line);
      };

    if ((target = strchr(line, '#')) != NULL) *target = 0;
    if ((target = strtok(line, ":")) == NULL) return NULL;
    if ((via = strtok(NULL, ":")) == NULL)    return NULL;
    NormStr(target);
    NormStr(via);
    if ((targind = searchNameNet(target, &netTemp)) == ERROR) return NULL;
    if ((viaind = searchNameNet(via, &netTemp)) == ERROR) return NULL;
    record = GetDynamic(sizeof *record);
    record->Target = targind;
    record->Via = viaind;
    return record;              /* checked is automatically initialized */
}

/*
 * SendOtherRoutedMail()
 *
 * This function sends mail for other systems via a routing node.
 */
void SendOtherRoutedMail(Routing *element, int *net)
  {
  struct cmd_data cmds;
  if( netDebug )
    {
    splitF(netLog, "SendOtherRouteMail(%08.8lx,%d): Target: %d  via: %d\n",
      *net, element->Target, element->Via);
    };

  if (element->Via != *net) return;
  if (!netTab[element->Target].ntflags.normal_mail &&
      !netTab[element->Target].ntflags.HasRouted)  return;

  if (netTab[element->Target].ntflags.Stadel)  return;

  getNet(element->Target, &netTemp);

  if (gotCarrier() && netTab[element->Target].ntflags.normal_mail)
    {
    zero_struct(cmds);
    normId(netTemp.netId, cmds.fields[0]);
    strcpy(cmds.fields[1], netTemp.netName);
    cmds.command = ROUTE_MAIL;
    if (sendNetCommand(&cmds, "Route Mail"))
      {
      splitF(netLog, "Routing mail to %s\n", netTemp.netName);
      if (ITL_SendMessages())
        {
        send_direct_mail(element->Target, netTemp.netName);
        if (gotCarrier())
          {
          ITL_StopSendMessages();
          netTemp.nbflags.normal_mail = FALSE;
          }
        }
      else
        {
        splitF(netLog, "ITL_SM failed\n");
        killConnection();
        }
      }
    else
      {
      sprintf(msgBuf.mbtext,"Mail Routing: %s did not recognize %s (%s) for routed mail."
      , netBuf.netName, netTemp.netName, netTemp.netId);
        netResult(msgBuf.mbtext);
      };
    };
  if (gotCarrier() && netTab[element->Target].ntflags.HasRouted)
    {
    RouteOut(&netTemp, element->Target, FALSE);
    }

  /* All done - so save what we did */
  putNet(element->Target, &netTemp);
}


/*
 * send_direct_mail()
 *
 * This sends mail normal (non-route mail).
 */
static int send_direct_mail(int which, char *name)
{
    FILE     *ptrs;
    label    fntemp;
    SYS_FILE fn;
    int      messCount = 0;
    struct   netMLstruct buf;
    extern char *READ_ANY;

    sprintf(fntemp, "%d.ml", which);
    makeSysName(fn, fntemp, &cfg.netArea);
    if ((ptrs = fopen(fn, READ_ANY)) == NULL) {
    if (netTab[which].ntflags.normal_mail) {
        sprintf(msgBuf.mbtext, "Send Mail: Mail file for %s missing.",
                            netBuf.netName);
        netResult(msgBuf.mbtext);
    }
    return 0;
    }

    while (getMLNet(ptrs, buf) && TrError == TRAN_SUCCESS) {
    if (findMessage(buf.ML_loc, buf.ML_id, TRUE)) {
        if (thisNet != which && netTab[thisNet].ntflags.Stadel) {
        prStStyle(FALSE, getMsgChar, name, sendITLchar, "");
        }
        else
         {
         prNetStyle(FALSE, getMsgChar,  sendITLchar, TRUE, name);
         }
       messCount++;
      }
    }

    fclose(ptrs);
    if (TrError == TRAN_SUCCESS) {
    unlink(fn);
    return messCount;
    }

    splitF(netLog, "\nFailed transferring mail!\n");
    killConnection();
    return 0;
}

