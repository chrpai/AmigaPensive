/* mas.c */

#include "exec/io.h"
#include "exec/libraries.h"
#include "dos/dos.h"
#include "dos/rdargs.h"
#include "rexx/rexxio.h"
#include "rexx/rxslib.h"
#include "sh/sub.h"
#include "techprg.h"

#include "proto/exec_protos.h"
#include "proto/dos_protos.h"
#include "proto/rexxsyslib_protos.h"

typedef unsigned short int uword;
typedef unsigned long int ulong;

_main(void);
ulong RexxCmd(ubyte *),GetSIG(void),doarea(struct AreaCfg *,ulong),
    input(u_char *);
long NumRexxFnc(u_char *),strval(u_char *);
u_char *RexxFnc(u_char *),*fprint(u_char *,UWORD);
void MakeTable(u_char **,u_char *,u_char *),GetAccess(void),
    dotext(struct SigDat *sigp),
    *SendBBSMsg(uword,void *),ScanAreas(struct SigDat *),gethotkey(void);

struct Library *RexxSysBase;
struct MinList *sigs;
struct SigDat *sig;
struct MsgPort *ReplyPort,*TCP;
struct BBSMsg bbsmsg;
struct RdArgs *ra;

ulong args[5],user,inter,totals[32],news[32],areanews,node,total,new,strv;
u_char cmd[32],APortName[32],Command[256],ibuf[32],buf[64],fprbuf[128],
    fncbuf[128],access[64],*FncArgs[16],FncSpace[128],
    template[]="SIG/N,ALL/S,NEW/S,FORCE/S,CONT/S,USER/K/N",
    v[]="$VER: Message Area Scanner 1.50";

#define ARG_ALL     args[1]
#define ARG_NEW     args[2]
#define ARG_FORCE   args[3]
#define ARG_CONT    args[4]

_main(void) {
struct SigDat *tmp;
struct Node *misc;
uword i,o,toscan,numsigs;
u_char *t;
    GetProgramName(cmd,32);
    if(!(ra=ReadArgs(template,args,NULL))) {
        PrintFault(i=IoErr(),cmd);
        return(i);
    } else {
        i=RETURN_FAIL;
        if(RexxSysBase=OpenLibrary("rexxsyslib.library",LIBRARY_MINIMUM)) {
            if(ReplyPort=CreateMsgPort()) {
                SendBBSMsg(ID_INCOPENCOUNT,NULL);
                if(TCP) {
                    sigs=SendBBSMsg(ID_GETSIGS,NULL);
                    if(args[5]) {
                        node=*(ulong *)args[5];
                        sprint(APortName,"Tech_%ld",node);
                        user=1;
                    }
                    GetAccess();
                    toscan=0;
                    if(!ARG_ALL) toscan=args[0]?*((ULONG *)args[0]):GetSIG();
                    while(TRUE) {
                        total=0;
                        new=0;
                        if(toscan!=-1) {
                            numsigs=0;
                            tmp=sigs->mlh_Head;
                            while(tmp->Node.mln_Succ) {
                                if(numsigs+1==toscan) sig=tmp;
                                tmp=tmp->Node.mln_Succ;
                                numsigs++;
                            }
                            if(toscan<=numsigs) {
                                i=RETURN_OK;
                                if(toscan) ScanAreas(sig);
                                    else if(ARG_CONT) {
                                    sig=sigs->mlh_Head;
                                    sig->Node.mln_Succ;
                                    dotext(sig);
                                    for(o=1;o<=numsigs;o++) {
                                        if(access[o]) {
                                            ScanAreas(sig);
                                            total+=*totals;
                                            totals[o-1]=*totals;
                                            if(user) {
                                                new+=areanews;
                                                news[o-1]=areanews;
                                            }
                                        }
                                        sig=sig->Node.mln_Succ;
                                    }
                                    if(user) print("\nTotal new messages: %ld\n",new);
                                        else rawprint("\n");
                                } else {
                                    sig=sigs->mlh_Head;
                                    sig->Node.mln_Succ;
                                    for(o=1;o<=numsigs;o++) {
                                        if(access[o]) {
                                            ScanAreas(sig);
                                            total+=*totals;
                                            totals[o-1]=*totals;
                                            if(user) {
                                                new+=areanews;
                                                news[o-1]=areanews;
                                                print("\nTotal for all areas so far: %ld messages, %ld new.\n",total,new);
                                            } else print("\nTotal for all SIGs so far: %ld messages.\n",total);
                                            gethotkey();
                                        }
                                        sig=sig->Node.mln_Succ;
                                    }
                                    RexxCmd("LogEntry Examining message area summary");
                                    rawprint("\fMessage Area Scanner by Sami Klemola.\n\n");
                                    fprint("Nr  SIG",36);
                                    print("Total%s",user?"      New\n\n":"\n\n");
                                    sig=sigs->mlh_Head;
                                    for(o=1;o<=numsigs;o++) {
                                        if(access[o]) {
                                            sprint(buf,"%ld",o);
                                            fprint(buf,4);
                                            fprint(sig->Name,28);
                                            print("%9ld",totals[o-1]);
                                            if(user) print("%9ld\n",news[o-1]);
                                                else rawprint("\n");
                                        }
                                        sig=sig->Node.mln_Succ;
                                    }
                                    fprint("\nTotal",33);
                                    print("%9ld",total);
                                    if(user) print("%9ld\n",new);
                                        else rawprint("\n");
                                }
                            } else print("%s: illegal SIG number\n",cmd);
                        } else {
                            i=RETURN_OK;
                            inter=FALSE;
                        }
                        if(!i&&inter) {
                            i=RETURN_FAIL;
                            gethotkey();
                            toscan=GetSIG();
                        } else break;
                    }
                    SendBBSMsg(ID_DECOPENCOUNT,NULL);
                } else print("%s: TechCon not running\n",cmd);
                DeleteMsgPort(ReplyPort);
            } else print("%s: unable to create a message port\n",cmd);
        } else print("%s: unable to open rexxsyslib.library\n",cmd);
    }
    FreeArgs(ra);
    return(i);
}

void ScanAreas(struct SigDat *toscan) {
struct Node *areanode;
struct AreaCfg *area;
ulong areatotal,areanew;
u_char *t;
    areanode=&toscan->Areas;
    areanode=areanode->ln_Succ;
    area=(struct AreaCfg *)areanode->ln_Name;
    sprint(buf,"HasMsgAcc %ld %ld",node,area->AreaNr);
    if(user) if(!(NumRexxFnc(buf))) {
        if(!ARG_CONT) rawprint("\nYou do not have access to this SIG.\n");
        return();
    }
    if(!ARG_CONT) dotext(toscan);
    *totals=0;
    areanews=0;
    while(areanode->ln_Succ) {
        areatotal=area->HiMsg?area->HiMsg-area->LowMsg+1:0;
        if(user) {
            sprint(buf,"GetHighRead %ld %ld",node,area->AreaNr);
            areanew=areatotal?area->HiMsg-NumRexxFnc(buf):0;
        }
        if(area->Type&1) if(doarea(area,areanew)) {
            *totals+=areatotal;
            areanews+=areanew;
            sprint(buf,"%ld",area->AreaNr);
            fprint(buf,4);
            fprint(area->Name,28);
            print("%9ld%9ld%9ld",area->LowMsg,area->HiMsg,areatotal);
            if(user) print("%9ld\n",areanew);
                else rawprint("\n");
        }
        areanode=areanode->ln_Succ;
        area=(struct AreaCfg *)areanode->ln_Name;
    }
    if(!ARG_CONT) if(user)
        print("\nTotal: %ld messages, %ld new.\n",*totals,areanews);
        else print("\nTotal: %ld messages.\n",*totals);
};

ulong doarea(struct AreaCfg *area,ulong areanew) {
    if(!user) return(TRUE);
    if(ARG_NEW&&!areanew) return(FALSE);
    sprint(buf,"HasMsgAcc %ld %ld",node,area->AreaNr);
    if(NumRexxFnc(buf)) {
        if(ARG_FORCE) return(TRUE);
        sprint(buf,"GetAreaMode %ld %ld",node,area->AreaNr);
        return(NumRexxFnc(buf));
    }
    return(FALSE);
};

void dotext(struct SigDat *sigp) {
    sprint(buf,"LogEntry Scanning%s%s areas of %s...",
        ARG_NEW?" for new messages in ":" ",ARG_FORCE|!user?"all":"joined",
        ARG_CONT?(u_char *)".":(u_char *)sigp->Name);
    if(ARG_CONT) strcpy(&buf[strlen(buf)-8],"...");
    print("\fMessage Area Scanner by Sami Klemola.\n\n%s\n\n",&buf[9]);
    buf[strlen(buf)-3]=0;
    if(user) RexxCmd(buf);
    fprint("Nr  Name",38);
    print("Low     High    Total%s",user?"      New\n\n":"\n\n");
};

void GetAccess(void) {
struct Node *areanode;
struct AreaCfg *area;
uword i;
    sig=sigs->mlh_Head;
    i=1;
    while(sig->Node.mln_Succ) {
        areanode=&sig->Areas;
        areanode=areanode->ln_Succ;
        area=(struct AreaCfg *)areanode->ln_Name;
        sprint(buf,"HasMsgAcc %ld %ld",node,area->AreaNr);
        access[i]=user?(NumRexxFnc(buf)?TRUE:FALSE):TRUE;
        sig=sig->Node.mln_Succ;
        i++;
    }
};

ulong GetSIG(void) {
uword i,o;
    rawprint("\fSIGs available:\n\n");
    sig=sigs->mlh_Head;
    i=1;
    while(sig->Node.mln_Succ) {
        if(access[i]) {
            sprint(buf,"%ld",i);
            fprint(buf,4);
            print("%s\n",sig->Name);
        }
        sig=sig->Node.mln_Succ;
        i++;
    }
    while(TRUE) {
        if(!(input("\nSelect one to scan (enter to exit, 0 for all): ")))
            return(-1);
        if((o=strval(ibuf))<i) if(!o||access[o]) break;
            else rawprint("\nYou do not have access to this SIG.\n");
    }
    inter=TRUE;
    return(o);
}

u_char *fprint(u_char *str,UWORD len) {
UWORD i;
    strcpy(fprbuf,str);
    for(i=strlen(fprbuf);i<len;i++) fprbuf[i]=32;
    fprbuf[i]=0;
    rawprint(fprbuf);
};

long strval(u_char *str) {
    if(StrToLong(str,&strv)<0) return(-1);
    return(strv);
};

void gethotkey(void) {
    rawprint("\nPress enter... ");
    while(FGetC(Input())!=10);
    rawprint("\n");
};

ulong input(u_char *query) {
ulong i,c;
u_char *t;
    rawprint(query);
    t=ibuf;
    i=0;
    while(i<30) if((c=FGetC(Input()))==10) break; else {
        *t++=c;
        i++;
    }
    *t=0;
    return(i);
};

/* This function creates the argument array used by RexxFnc(). You must
   provide the data areas for it. See the code for RexxFnc() for more. */

void MakeTable(u_char **table,u_char *tabledata,u_char *string) {
u_char *t,*s;
uword i;
    clear(table,128);
    clear(t=tabledata,128);
    s=string;
    for(i=0;i<16;i++) {
        table[i]=t;
        while(*s&&(*s!=32)) *t++=*s++;
        if(!*s) break;
        while(*s==32) s++;
        t++;
    }
}

/*
 * RexxCmd - Send a command to an ARexx port
 *
 * You should have rexxsyslib.library open and a global variable called
 * APortName containing the name of the ARexx port the command should be
 * sent to.  You also need a message port called ReplyPort.
 *
 * Usage: RexxCmd("<Command>");
 *
 * Return: 0 - success, 1 - no port, 2 - error
 *
 */

ulong RexxCmd(ubyte *arg) {
struct RexxMsg *rxm;
struct MsgPort *RxPort;
ulong res;
    res=0;
    if(rxm = CreateRexxMsg(ReplyPort, NULL, NULL)) {
        rxm->rm_Node.mn_Node.ln_Name = "REXX";
        rxm->rm_Action = RXCOMM;
        ARG0(rxm) = CreateArgstring(arg, strlen(arg));
        Forbid();
        if(RxPort = FindPort(APortName)) PutMsg(RxPort, (struct Message *)rxm);
        Permit();
        if(!RxPort) {
            res=1;
        } else {
            WaitPort(ReplyPort);
            while(GetMsg(ReplyPort)) ;
            if(rxm->rm_Result1 > 4) {
                res=2;
            }
        }
        ClearRexxMsg(rxm, 1);
        DeleteRexxMsg(rxm);
    }
    return(res);
}

/* NumRexxFnc - Call a numeric ARexx function (Written by S.Klemola)

/* Calls an ARexx function that returns a numeric value. Returns FALSE
   by default. You get this if an error occurs. Usage like RexxFnc(). */

long NumRexxFnc(u_char *Fnc) {
long v;
u_char *t;
    v=0;
    if(t=RexxFnc(Fnc)) v=strval(t);
    return(v);
};

/*
 * RexxFnc - Call an ARexx function (Revised by S.Klemola)
 *
 * Requires the same stuff as RexxCmd
 *
 * Note! Instead of duplicating the string returned by the function, it
 * is copied to a global buffer, fncbuf. You must provide this. Do *NOT*
 * call free() on the result! Note also the new usage. Returns NULL if an
 * error occurred.
 *
 * Usage: result = RexxFnc("<Function> [<Arg1>] [<Arg2] [...]",);
 *
 */

u_char *RexxFnc(u_char *Fnc) {
struct RexxMsg *rxm;
struct MsgPort *RxPort;
int cc;
ubyte *res;
    MakeTable(FncArgs,FncSpace,Fnc);
    res = fncbuf;
    if(rxm = CreateRexxMsg(ReplyPort, NULL, NULL)) {
        rxm->rm_Node.mn_Node.ln_Name = "REXX";
        rxm->rm_Action = RXFUNC | RXFF_RESULT;
        for(cc = 0; FncArgs[cc] && cc < 16; cc++) rxm->rm_Args[cc] =
            CreateArgstring(FncArgs[cc], strlen(FncArgs[cc]));
        rxm->rm_Action |= (cc - 1) & 0xf;
        Forbid();
        if(RxPort = FindPort(APortName)) PutMsg(RxPort, (struct Message *)rxm);
        Permit();
        if(!RxPort) {
            res = 0;
        } else {
            WaitPort(ReplyPort);
            while(GetMsg(ReplyPort)) ;
            if(!rxm->rm_Result1) {
                if(rxm->rm_Result2) {
                    strcpy(res,(u_char *)rxm->rm_Result2);
                    DeleteArgstring((ubyte *)rxm->rm_Result2);
                }
            } else if(rxm->rm_Result1 > 5) res = 0;
        }
        ClearRexxMsg(rxm, 1);
        DeleteRexxMsg(rxm);
    }
    return(res);
}

/* Enhanced by S. Klemola */

/*
 * Note-a-mundos:
 *
 * - Provide a global uninitialized BBSMsg structure
 * - Provide a global variable called TCP (it can be used to check after
 *   a call to this function if TechCon is running)
 * - clear() is an assembly function that performs the same as bzero()
 * - Permit() has only significance when the port wasn't found. Otherwise
 *   the Wait() call will already have braken Forbid()
 *
 */

void *SendBBSMsg(uword ID,void *data) {
struct BBSMsg *sndmsg;
    sndmsg=&bbsmsg;
    clear(sndmsg,sizeof(struct BBSMsg));
    sndmsg->SendingNode = -1;
    sndmsg->ID = ID;
    sndmsg->data = data;
    sndmsg->Msg.mn_ReplyPort = ReplyPort;
    Forbid();
    if(TCP=FindPort("TechCon_Private")) {
        PutMsg(TCP,(struct Message *)sndmsg);
        Wait(1 << ReplyPort->mp_SigBit);
        while(GetMsg(ReplyPort));
    }
    Permit();
    return(TCP?sndmsg->result:NULL);
}

