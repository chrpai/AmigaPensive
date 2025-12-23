#include <TechWWF.h>
#include <WWFData.h>
#include <ctype.h>

extern int CutName;

const char verstr[] = { "\0$VER: TechWWF " VERSION "  (" __COMMODORE_DATE__ ")" };

int nbrk(void) { return(0); }

struct MsgPort *TechConPort, *ReplyPort, *RxPort;

int NodeNum, CLocked;

int TotalAreas;

struct Library *RexxSysBase;

UBYTE *StrBuf;

struct MinList Areas, Files;

int NumAreas, NumMsgs;

UBYTE *nnode, *UserName;

struct CharSet *InCSet, *OutCSet;

BPTR DirLock;

int RecPos;

void AddFile(UBYTE *);
void WriteControl(void);
void PackArchive(void);
void UnArchive(void);
void ProcessReplies(void);

struct CharSet *LoadCSet(UBYTE *);
void ConvString(__A0 UBYTE *, __A1 UBYTE *, __A2 UBYTE *);

void CRtoLF(__A0 UBYTE *);
void LFtoCR(__A0 UBYTE *);

const char nothing[] = { "" };

void ProcessReplies(void)
{
    BPTR hdr, txt;
    struct WWFHdrRWF *rwf;
    char *hdrfile, *txtfile;
    struct MemMsg *mmsg;
    struct DateTime *dt;

    if(!(rwf = malloc(sizeof(struct WWFHdrRWF))) || !(mmsg = malloc(sizeof(struct MemMsg)))) return();
    if(!(dt = calloc(sizeof(struct DateTime) + 32, 1))) return();
    dt->dat_StrDate = (char *)&dt[1];
    dt->dat_StrTime = &dt->dat_StrDate[16];
    dt->dat_Format = FORMAT_DOS;
    strcpy(StrBuf, OutDir);
    AddPart(StrBuf, "Header.RWF", 256);
    if(hdr = Open(hdrfile = strdup(StrBuf), MODE_OLDFILE)) {
        strcpy(StrBuf, OutDir);
        AddPart(StrBuf, "Text.RWF", 256);
        if(txt = Open(txtfile = strdup(StrBuf), MODE_OLDFILE)) {
            while(FRead(hdr, rwf, sizeof(struct WWFHdrRWF), 1)) {
                bzero(mmsg, sizeof(struct MemMsg));
                mmsg->DestZone = rwf->DestZone;
                mmsg->DestNet = rwf->DestNet;
                mmsg->DestNode = rwf->DestNode;
                mmsg->DestPoint = rwf->DestPoint;
                if(rwf->OrigArea == rwf->Area) mmsg->Original = rwf->OrigNum;
                if(rwf->Flags & RWFHDR_PRIVATE) mmsg->Bits = MSG_PRIVATE;
                mmsg->FromUser = UserName;
                if(InCSet) {
                    mmsg->ToUser = malloc(strlen(rwf->ToUser) + 1);
                    ConvString(rwf->ToUser, mmsg->ToUser, InCSet->CnvFrom);
                    mmsg->Subject = malloc(strlen(rwf->Subject) + 1);
                    ConvString(rwf->Subject, mmsg->Subject, InCSet->CnvFrom);
                    {
                        char *tmp;

                        tmp = malloc(rwf->TextLen + 1);
                        tmp[rwf->TextLen] = 0;
                        Seek(txt, rwf->TextPos, OFFSET_BEGINNING);
                        FRead(txt, tmp, 1, rwf->TextLen);
                        mmsg->MsgText = malloc(rwf->TextLen + 1);
                        ConvString(tmp, mmsg->MsgText, InCSet->CnvFrom);
                        free(tmp);
                    }
                } else {
                    mmsg->ToUser = strdup(rwf->ToUser);
                    mmsg->Subject = strdup(rwf->Subject);
                    mmsg->MsgText = malloc(rwf->TextLen + 1);
                    mmsg->MsgText[rwf->TextLen] = 0;
                    Seek(txt, rwf->TextPos, OFFSET_BEGINNING);
                    FRead(txt, mmsg->MsgText, 1, rwf->TextLen);
                }
                LFtoCR(mmsg->MsgText);
                mmsg->AttFile = nothing;
                DateStamp(&dt->dat_Stamp);
                DateToStr(dt);
                sprintf(mmsg->DateTime, "%-9.9s  %-8.8s", dt->dat_StrDate, dt->dat_StrTime);
                sprintf(StrBuf, "PostMsgRaw %lx %lu", mmsg, rwf->Area);
                NumMsgs++;
                RexxCmd(StrBuf);
                free(mmsg->ToUser);
                free(mmsg->Subject);
                free(mmsg->MsgText);
            }
            Close(txt);
            remove(txtfile);
        } else {
            if(Quiet) RexxCmd("ShowMessage TechWWF Error: Couldn't find Text.RWF");
            else RexxCmd("SendModem \015\012TechWWF Error: Couldn't find Text.RWF");
        }
        Close(hdr);
        remove(hdrfile);
    } else {
        if(Quiet) RexxCmd("ShowMessage TechWWF Error: Couldn't find Header.RWF");
        else RexxCmd("SendModem \015\012TechWWF Error: Couldn't find Header.RWF");
    }
}

void UnArchive(void)
{
    UBYTE *nm;

    nm = RexxFnc("GetUserPath", nnode, UserName, NULL);
    strcpy(StrBuf, nm);
    free(nm);
    {
        UBYTE Tmp[16];

        sprintf(Tmp, "%s.RRF", BBSID);
        AddPart(StrBuf, Tmp, 256);
    }
    nm = strdup(StrBuf);
    sprintf(StrBuf, "%s %s %s/", unpack, nm, OutDir);
    System(StrBuf, NULL);
    remove(nm);
    free(nm);
}

UBYTE *aname;

void PackArchive(void)
{
    UBYTE *nm;

    nm = RexxFnc("GetUserPath", nnode, UserName, NULL);
    strcpy(StrBuf, nm);
    free(nm);
    {
        UBYTE Tmp[16];

        sprintf(Tmp, "%s.WWF", BBSID);
        AddPart(StrBuf, Tmp, 256);
    }
    nm = strdup(StrBuf);
    remove(nm);
    UnLock(DirLock);
    DirLock = Lock(OutDir, ACCESS_READ);
    sprintf(StrBuf, "%s %s %s/#?", pack, nm, OutDir);
    System(StrBuf, NULL);
    {
        struct Node *nd;

        while(nd = GetHead(&Files)) {
            Remove(nd);
            remove(nd->ln_Name);
            free(nd->ln_Name);
            free(nd);
        }
    }
    aname = nm;
}

void AddFile(UBYTE *fname)
{
    struct Node *nd;

    nd = calloc(sizeof(struct Node), 1);
    nd->ln_Name = strdup(fname);
    nd->ln_Type = NT_USER;
    AddTail((struct List *)&Files, nd);
}

void PackMessages(void)
{
    BPTR datamsg, areahdr, phdr = NULL;
    struct Node *nd;
    struct WWFMsgHdr *hdr;
    struct WWFPersonal *wwfp;

    NewList((struct List *)&Files);
    strcpy(StrBuf, OutDir);
    AddPart(StrBuf, "Data.MSG", 256);
    AddFile(StrBuf);
    if(!(datamsg = Open(StrBuf, MODE_NEWFILE))) return();
    hdr = malloc(sizeof(struct WWFMsgHdr));
    wwfp = calloc(sizeof(struct WWFPersonal), 1);
    if(!Quiet) RexxCmd("SendModem \015\012\015\012Packing messages...");
    nd = GetHead(&Areas);
    while(nd) {
        struct AreaCfg *ac = (struct AreaCfg *)nd->ln_Name;
        ULONG HighRead;

        {
            UBYTE *rt;

            sprintf(StrBuf, "%lu", ac->AreaNr);
            rt = RexxFnc("GetHighRead", nnode, StrBuf, NULL);
            HighRead = strtol(rt, NULL, 0);
            free(rt);
        }
        if((HighRead < (ac->LowMsg - 1)) && (ac->LowMsg > 1)) HighRead = ac->LowMsg - 1;
        if((HighRead < ac->HiMsg) && (ac->HiMsg)) {
            ULONG NumMsg;

            areahdr = NULL;
            sprintf(StrBuf, "SendModem \015\012\015\012Area: %s  Messages: %-4lu", ac->Name, (ac->HiMsg - HighRead));
            if(!Quiet) RexxCmd(StrBuf);
            for(NumMsg = HighRead + 1; NumMsg <= ac->HiMsg; NumMsg++) {
                struct FMsg *fm;
                struct MemMsg *mmsg;

                if(fm = LoadMsg(ac, NumMsg)) {
                    mmsg = F2MMsg(fm);
                    if((mmsg->Bits & MSG_PRIVATE) && Stricmp(UserName, mmsg->ToUser) && Stricmp(UserName, mmsg->FromUser)) continue;
                    if(!areahdr) {
                        char *tmp;

                        sprintf(StrBuf, "%lu.HDR", ac->AreaNr);
                        tmp = strdup(StrBuf);
                        strcpy(StrBuf, OutDir);
                        AddPart(StrBuf, tmp, 256);
                        free(tmp);
                        AddFile(StrBuf);
                        areahdr = Open(StrBuf, MODE_NEWFILE);
                    }
                    if(areahdr) {
                        hdr->MsgNum = mmsg->MsgNum;
                        strncpy(hdr->Date, mmsg->DateTime, 20);
                        hdr->ReplyTo = mmsg->Original;
                        hdr->NextReply = mmsg->NextReply;
                        hdr->OrigZone = mmsg->OrigZone;
                        hdr->OrigNet = mmsg->OrigNet;
                        hdr->OrigNode = mmsg->OrigNode;
                        hdr->OrigPoint = mmsg->OrigPoint;
                        hdr->DestZone = mmsg->DestZone;
                        hdr->DestNet = mmsg->DestNet;
                        hdr->DestNode = mmsg->DestNode;
                        hdr->DestPoint = mmsg->DestPoint;
                        if(mmsg->Bits & MSG_PRIVATE) hdr->Flags = WWFHDR_PRIVATE;
                        else hdr->Flags = 0;
                        if(mmsg->Bits & MSG_RECEIVED) hdr->Flags |= WWFHDR_RECEIVED;
                        hdr->TextPos = Seek(datamsg, 0, OFFSET_CURRENT);
                        CRtoLF(mmsg->MsgText);
                        hdr->TextLen = FWrite(datamsg, mmsg->MsgText, 1, hdr->TextLen = strlen(mmsg->MsgText));
                        bzero(hdr->FromUser, 154);
                        strncpy(hdr->FromUser, mmsg->FromUser, 40);
                        strncpy(hdr->ToUser, mmsg->ToUser, 40);
                        strncpy(hdr->Subject, mmsg->Subject, 74);
                        FWrite(areahdr, hdr, sizeof(struct WWFMsgHdr), 1);
                        NumMsgs++;
                        if(!Stricmp(UserName, mmsg->ToUser)) {
                            if(!phdr) {
                                strcpy(StrBuf, OutDir);
                                AddPart(StrBuf, "Personal.HDR", 256);
                                AddFile(StrBuf);
                                phdr = Open(StrBuf, MODE_NEWFILE);
                            }
                            if(phdr && wwfp) {
                                wwfp->AreaNum = ac->AreaNr;
                                wwfp->MsgNum = hdr->MsgNum;
                                FWrite(phdr, wwfp, sizeof(struct WWFPersonal), 1);
                            }
                        }
                    }
                    free(mmsg);
                    FreeVec(fm);
                }
                if(!Quiet && !((ac->HiMsg - NumMsg) % 5)) {
                    sprintf(StrBuf, "SendModem \033[4D%-4lu", (ac->HiMsg - NumMsg));
                    RexxCmd(StrBuf);
                }
            }
            if(areahdr) Close(areahdr);
            if(!Quiet) RexxCmd("SendModem \033[4DOK...");
            sprintf(StrBuf, "SetHighRead %lu %lu", ac->AreaNr, ac->HiMsg);
            RexxCmd(StrBuf);
        }
        nd = GetSucc(nd);
    }
    if(datamsg) Close(datamsg);
    if(phdr) Close(phdr);
}

void WriteControl(void)
{
    {
    struct InfoBBS *InfoBBS;
    BPTR ctl;

    if(InfoBBS = calloc(sizeof(struct InfoBBS), 1)) {
        strncpy(InfoBBS->BBSName, BBSName, 80);
        strncpy(InfoBBS->SysOpName, BBSSysOp, 40);
        if(ForceISO) InfoBBS->Flags |= WWFINFO_ONLYISO;
        InfoBBS->Flags |= 2;
        strncpy(InfoBBS->UserName, UserName, 40);
        {
            char *tmp;

            if(UAlias) {
                if(tmp = RexxFnc("GetUserMisc", nnode, UAlias, NULL)) {
                    strncpy(InfoBBS->UserAlias, tmp, 40);
                    free(tmp);
                }
            }
            if(UAddress) {
                if(tmp = RexxFnc("GetUserMisc", nnode, UAddress, NULL)) {
                    strncpy(InfoBBS->UserAddress, tmp, 30);
                    free(tmp);
                }
            }
            if(UPostCode) {
                if(tmp = RexxFnc("GetUserMisc", nnode, UPostCode, NULL)) {
                    strncpy(InfoBBS->UserPostalCode, tmp, 10);
                    free(tmp);
                }
            }
            if(UCity) {
                if(tmp = RexxFnc("GetUserMisc", nnode, UCity, NULL)) {
                    strncpy(InfoBBS->UserCity, tmp, 30);
                    free(tmp);
                }
            }
            if(UCountry) {
                if(tmp = RexxFnc("GetUserMisc", nnode, UCountry, NULL)) {
                    strncpy(InfoBBS->UserCountry, tmp, 30);
                    free(tmp);
                }
            }
            if(UPhone) {
                if(tmp = RexxFnc("GetUserMisc", nnode, UPhone, NULL)) {
                    strncpy(InfoBBS->UserPhone, tmp, 20);
                    free(tmp);
                }
            }
        }
        strncpy(InfoBBS->BBSID, BBSID, 80);
        strcpy(StrBuf, OutDir);
        AddPart(StrBuf, "Info.BBS", 256);
        AddFile(StrBuf);
        if(ctl = Open(StrBuf, MODE_NEWFILE)) {
            FWrite(ctl, InfoBBS, sizeof(struct InfoBBS), 1);
            Close(ctl);
        }
        free(InfoBBS);
    }
    }
    {
        struct AreaIWF *IWF;
        BPTR iwf;
        long Acc = 0, Msk = 0;

        {
            char *tmp;

            if(tmp = RexxFnc("GetUserAccess", nnode, NULL)) {
                Acc = strtol(tmp, NULL, 0);
                free(tmp);
            }
            if(tmp = RexxFnc("GetMask", nnode, NULL)) {
                Msk = strtol(tmp, NULL, 0);
                free(tmp);
            }
        }
        if(IWF = malloc(sizeof(struct AreaIWF))) {
            strcpy(StrBuf, OutDir);
            AddPart(StrBuf, "Area.IWF", 256);
            AddFile(StrBuf);
            if(iwf = Open(StrBuf, MODE_NEWFILE)) {
                struct Node *an;
                struct AreaCfg *ac;

                an = GetHead(&Areas);
                while(an) {
                    ac = (struct AreaCfg *)an->ln_Name;
                    IWF->AreaNum = ac->AreaNr;
                    bzero(IWF->AreaName, 34);
                    strncpy(IWF->AreaName, ac->Name, 34);
                    if(!ac->Type) IWF->Flags = WWFAREA_NETMAIL;
                    else if(ac->Type == 1) IWF->Flags = WWFAREA_ECHO;
                    else if(ac->Type == 2) IWF->Flags = WWFAREA_FEEDB;
                    else IWF->Flags = 0;
                    if((Acc >= ac->WriteAcc) && ((Msk & ac->WriteMask) == ac->WriteMask)) IWF->Flags |= WWFAREA_WRITE | WWFAREA_REPLY;
                    FWrite(iwf, IWF, sizeof(struct AreaIWF), 1);
                    an = GetSucc(an);
                }
                Close(iwf);
            }
            free(IWF);
        }
    }
}

void MakeAreaList(void)
{
    int cc;

    NewList((struct List *)&Areas);
    TotalAreas = (int)SendBBSMsg(ID_GETNUMAREAS, NULL);
    NumAreas = 0;
    for(cc = 1; cc <= TotalAreas; cc++) {
        UBYTE *rt;

        sprintf(StrBuf, "%lu", cc);
        rt = RexxFnc("GetAreaMode", nnode, StrBuf, NULL);
        if(strtol(rt, NULL, 0)) {
            struct Node *nd;

            nd = calloc(sizeof(struct Node), 1);
            nd->ln_Name = (UBYTE *)SendBBSMsg(ID_GETMSGAREA, (APTR)cc);
            nd->ln_Type = NT_USER;
            AddTail((struct List *)&Areas, nd);
            NumAreas++;
        }
        free(rt);
    }
}

struct Process *MyProc;

void main(int ac, char **av)
{
    onbreak(nbrk);
    MyProc = (struct Process *)FindTask(NULL);
    if(!(StrBuf = malloc(256))) Close_All();
    if(!(ReplyPort = CreateMsgPort())) Close_All();
    if(!(RexxSysBase = OpenLibrary("rexxsyslib.library", 0))) Close_All();
    Forbid();
    TechConPort = FindPort("TechCon_Private");
    RxPort = FindPort("TechCon");
    Permit();
    if(!TechConPort) Close_All();
    SendBBSMsg(ID_INCOPENCOUNT, NULL);
    SendBBSMsg(ID_BEGINMSGH, NULL);
    if(!RxPort) Close_All();
    if(ac != 4) {
        fprintf(stderr, "Usage: %s <node number> <I/O> <config>\n", av[0]);
        Close_All();
    }
    NodeNum = strtol(av[1], NULL, 0);
    {
        UBYTE *PName;

        sprintf(StrBuf, "%lu", NodeNum);
        nnode = strdup(StrBuf);
        PName = RexxFnc("Con_LineActive", nnode, NULL);
        if(!strlen(PName)) {
            fprintf(stderr, "Node %lu not active\n", NodeNum);
            Close_All();
        }
        Forbid();
        if(RxPort = FindPort(PName)) {
            RexxCmd("LockCarrier");
            CLocked = 1;
        }
        Permit();
        free(PName);
        if(!RxPort) Close_All();
        UserName = RexxFnc("GetUserName", nnode, NULL);
    }
    {
        BPTR fh;

        if(fh = Open(av[3], MODE_OLDFILE)) {
            parsebbscfg(fh);
            Close(fh);
        } else {
            fprintf(stderr, "Couldn't open %s\n", av[3]);
            Close_All();
        }
        if(!BBSName || !BBSSysOp || !BBSID || !OutDir) {
            fprintf(stderr, "Invalid configuration\n");
            if(!Quiet) RexxCmd("SendModem \015\012\015\012Invalid configuration\015\012\015\012");
            Close_All();
        }
        {
            UBYTE *str;

            if(str = RexxFnc("GetUserMisc", nnode, "WWFTASKPRI", NULL)) {
                if(strlen(str)) TaskPri = strtol(str, NULL, 0);
                free(str);
            }
            if(str = RexxFnc("GetUserMisc", nnode, "WWFARC", NULL)) {
                if(strlen(str)) pack = str;
                else free(str);
            }
            if(str = RexxFnc("GetUserMisc", nnode, "WWFUNARC", NULL)) {
                if(strlen(str)) unpack = str;
                else free(str);
            }
            if(str = RexxFnc("GetUserMisc", nnode, "WWFASYNC", NULL)) {
                if(strlen(str)) Quiet = strtol(str, NULL, 0);
                free(str);
            }
            if(!ForceISO) {
                if(str = RexxFnc("GetUserMisc", nnode, "WWFINCSET", NULL)) {
                    if(strlen(str)) InCSet = LoadCSet(str);
                    free(str);
                }
                if(str = RexxFnc("GetUserMisc", nnode, "WWFOUTCSET", NULL)) {
                    if(strlen(str)) OutCSet = LoadCSet(str);
                    free(str);
                }
            }
        }
        SetTaskPri((struct Task *)MyProc, TaskPri);
        {
            UBYTE Tmp[30];

            sprintf(Tmp, "tWWF_%lx", MyProc);
            strcpy(StrBuf, OutDir);
            AddPart(StrBuf, Tmp, 256);
            free(OutDir);
            OutDir = strdup(StrBuf);
            DirLock = CreateDir(OutDir);
        }
    }
    if(av[2][0] == 'I') {
        if(!Quiet) RexxCmd("SendModem \015\012\015\012Unarchiving replies...");
        UnArchive();
        if(!Quiet) RexxCmd("SendModem \015\012\015\012Processing...");
        ProcessReplies();
        if(NumMsgs) {
            if(!Quiet) sprintf(StrBuf, "SendModem \015\012\015\012Imported %lu messages\015\012\015\012", NumMsgs);
            else sprintf(StrBuf, "ShowMessage TechWWF: Imported %lu messages", NumMsgs);
            RexxCmd(StrBuf);
        } else {
            if(!Quiet) RexxCmd("SendModem \015\012\015\012No messages imported\015\012\015\012");
            else RexxCmd("ShowMessage TechWWF: No messages imported");
        }
    } else if(av[2][0] == 'O') {
        MakeAreaList();
        if(!NumAreas) {
            if(!Quiet) RexxCmd("SendModem \015\012\015\012No areas selected\015\012\015\012");
            Close_All();
        }
        PackMessages();
        WriteControl();
        if(!Quiet) RexxCmd("SendModem \015\012\015\012Archiving messages...");
        PackArchive();
        sprintf(StrBuf, "SendModem \015\012\015\012Packed %lu messages\015\012\015\012", NumMsgs);
        if(!Quiet) RexxCmd(StrBuf);
        else {
            sprintf(StrBuf, "ShowMessage TechWWF: Packed %lu messages\015\012\015\012%s", NumMsgs, DoneString);
            RexxCmd(StrBuf);
            sprintf(StrBuf, "MarkAnyFile %s", aname);
            RexxCmd(StrBuf);
        }
    } else fprintf(stderr, "Unknown command '%s'\n", av[2][0]);
    Close_All();
}

void Close_All(void)
{
    if(DirLock) UnLock(DirLock);
    if(OutDir) {
        sprintf(StrBuf, "delete %s all quiet", OutDir);
        System(StrBuf, NULL);
    }
    if(CLocked) RexxCmd("UnLockCarrier");
    if(TechConPort) {
        SendBBSMsg(ID_ENDMSGH, NULL);
        SendBBSMsg(ID_DECOPENCOUNT, NULL);
    }
    if(RexxSysBase) CloseLibrary(RexxSysBase);
    if(ReplyPort) DeleteMsgPort(ReplyPort);
    exit(0);
}

void *SendBBSMsg(UWORD ID, void *data)
{
    static struct BBSMsg *sndmsg = NULL;

    if(!sndmsg) sndmsg = calloc(sizeof(struct BBSMsg), 1);
    else bzero(sndmsg, sizeof(struct BBSMsg));
    sndmsg->SendingNode = -1;
    sndmsg->ID = ID;
    sndmsg->data = data;
    sndmsg->Msg.mn_ReplyPort = ReplyPort;
    Forbid();
    PutMsg(TechConPort, (struct Message *)sndmsg);
    Permit();
    Wait(1 << ReplyPort->mp_SigBit);
    while(GetMsg(ReplyPort)) ;
    return(sndmsg->result);
}

void RexxCmd(UBYTE *arg)
{
    struct RexxMsg *rxm;

    if(rxm = CreateRexxMsg(ReplyPort, NULL, NULL)) {
        rxm->rm_Node.mn_Node.ln_Name = "REXX";
        rxm->rm_Action = RXCOMM;
        ARG0(rxm) = CreateArgstring(arg, strlen(arg));
        Forbid();
        PutMsg(RxPort, (struct Message *)rxm);
        Permit();
        Wait(1 << ReplyPort->mp_SigBit);
        while(GetMsg(ReplyPort)) ;
        ClearRexxMsg(rxm, 1);
        DeleteRexxMsg(rxm);
    }
}

STRPTR theArgs[16];

__stkargs void *RexxFnc(UBYTE *arg, ...)
{
    va_list va;
    UBYTE *res;
    int cc;

    va_start(va, arg);
    theArgs[0] = arg;
    for(cc = 1; (cc < 16) && (((ULONG *)va)[cc - 1]); cc++) theArgs[cc] = (APTR)((ULONG *)va)[cc - 1];
    if(cc < 16) theArgs[cc] = NULL;
    res = DoRxFnc(theArgs);
    va_end(va);
    return(res);
}

void *DoRxFnc(UBYTE **arg)
{
    struct RexxMsg *rxm;
    UBYTE *res;

    res = strdup("");
    if(rxm = CreateRexxMsg(ReplyPort, NULL, NULL)) {
        int cc;

        rxm->rm_Node.mn_Node.ln_Name = "REXX";
        rxm->rm_Action = RXFUNC | RXFF_RESULT;
        for(cc = 0; (arg[cc]) && (cc < 16); cc++) rxm->rm_Args[cc] = CreateArgstring(arg[cc], strlen(arg[cc]));
        rxm->rm_Action |= (cc - 1) & 0xf;
        Forbid();
        PutMsg(RxPort, (struct Message *)rxm);
        Permit();
        Wait(1 << ReplyPort->mp_SigBit);
        while(GetMsg(ReplyPort)) ;
        if(!rxm->rm_Result1) {
            if(rxm->rm_Result2) {
                free(res);
                res = strdup((char *)rxm->rm_Result2);
                DeleteArgstring((UBYTE *)rxm->rm_Result2);
            }
        }
        ClearRexxMsg(rxm, 1);
        DeleteRexxMsg(rxm);
    }
    return(res);
}

void CRLFtoEOS(char *);

void CRLFtoEOS(char *s)
{
    while(*s && (*s != 10) && (*s != 13)) s++;
    *s = 0;
}

void parsebbscfg(BPTR cfh)
{
    UBYTE *buf, *bpos;

    buf = calloc(1, 256);
    while(bpos = FGets(cfh, buf, 256)) {
        CRLFtoEOS(bpos);
        while(*bpos == ' ' || *bpos == 9) bpos++;
        if(*bpos == '>') System(++bpos, NULL);
        else if(*bpos != ';' && *bpos != '#' && *bpos) {
            int cc = 0;
            UBYTE *scan;

            scan = bpos;
            while(*scan && *scan != ' ' && *scan != 9) scan++;
            if(*scan) *scan++ = 0;
            while(CfgList[cc].id) {
                if(!(Stricmp(bpos, CfgList[cc].id))) {
                    if(CfgList[cc].val >= 0) *CfgList[cc].var = CfgList[cc].val;
                    else {
                        bpos = scan;
                        while(*bpos == ' ' || *bpos == 9) bpos++;
                        if(*bpos) switch(CfgList[cc].val) {
                            case -1:
                                *CfgList[cc].var = strtol(bpos, NULL, 0);
                                break;
                            case -2:
                                if(*bpos != '"') *CfgList[cc].var = strdup(bpos);
                                else {
                                    UBYTE *bscan;

                                    bscan = ++bpos;
                                    while(*bscan != '"' && *bscan) bscan++;
                                    *bscan = 0;
                                    *CfgList[cc].var = strdup(bpos);
                                }
                        }
                    }
                    break;
                }
                cc++;
            }
        }
    }
    free(buf);
}

struct CharSet *LoadCSet(UBYTE *Name)
{
    struct CharSet *ncset;
    BPTR csfh;

    ncset = NULL;
    sprintf(StrBuf, "%s.DAT", Name);
    if(csfh = Open(StrBuf, MODE_OLDFILE)) {
        ncset = malloc(sizeof(struct CharSet));
        ncset->CnvFrom = malloc(512);
        ncset->CnvTo = &(ncset->CnvFrom[256]);
        FRead(csfh, ncset->CnvFrom, 256, 2);
        ncset->Name = strdup(Name);
        CRLFtoEOS(FGets(csfh, StrBuf, 256));
        ncset->Extension = strdup(StrBuf);
        Close(csfh);
    }
    return(ncset);
}
