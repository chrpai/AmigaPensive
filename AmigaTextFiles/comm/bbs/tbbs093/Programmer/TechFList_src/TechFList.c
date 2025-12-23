#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <dos/dos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lists.h>

#include <clib/alib_protos.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>

#include "techprg.h"

const char verstr[] = { "\0$VER: TechFList 0.02  (" __COMMODORE_DATE__ ")" };

void main(int, char **);
void *SendBBSMsg(UWORD, void *);
void WriteList(struct DirNode *, BPTR);

struct MsgPort *ReplyPort, *TechConPort;

char *StrBuf, *AreaName;

struct DateTime *dt;

void main(int ac, char **av)
{
    if(!(StrBuf = malloc(1024 + sizeof(struct DateTime) + 16))) exit(103);
    dt = StrBuf + 1024;
    bzero(dt, sizeof(struct DateTime));
    dt->dat_Format = FORMAT_DOS;
    dt->dat_StrDate = (STRPTR)(dt + 1);
    if(ac == 5) {
        {
            BPTR bptr;

            if(bptr = Lock(av[2], ACCESS_READ)) UnLock(bptr);
            else if(bptr = Open(av[2], MODE_NEWFILE)) Close(bptr);
            SetComment(av[2], av[4]);
        }
        if(ReplyPort = CreateMsgPort()) {
            Forbid();
            if(TechConPort = FindPort("TechCon_Private")) SendBBSMsg(ID_INCOPENCOUNT, NULL);
            Permit();
            if(TechConPort) {
                struct DirNode *FileRoot;

                if(FileRoot = SendBBSMsg(ID_LOCKFILEROOT, NULL)) {
                    BPTR list, header;

                    if(list = Open(av[2], MODE_NEWFILE)) {
                        if(header = Open(av[3], MODE_OLDFILE)) {
                            while(FGets(header, StrBuf, 511)) FPuts(list, StrBuf);
                            Close(header);
                        }
                        AreaName = StrBuf;
                        *AreaName = 0;
                        WriteList(FileRoot, list);
                        Close(list);
                    }
                }
                SendBBSMsg(ID_FREEFILEROOT, NULL);
                SendBBSMsg(ID_DECOPENCOUNT, NULL);
            } else PutStr("Couldn't find TechCon\n");
            DeleteMsgPort(ReplyPort);
        }
    } else Printf("Usage: %s <ignored> <filelist> <header> <comment>\n", av[0]);
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
    PutMsg(TechConPort, (struct Message *)sndmsg);
    Wait(1 << ReplyPort->mp_SigBit);
    while(GetMsg(ReplyPort)) ;
    return(sndmsg->result);
}

const char filefmt[] = { "%-16s %8lu %s %s\n" };

void WriteList(struct DirNode *dnode, BPTR fh)
{
    if(dnode->fc->Misc) {
        struct Node *misc = GetHead(dnode->fc->Misc);

        while(misc) {
            if(!Strnicmp(misc->ln_Name, "*NOLIST", 7)) return();
            misc = GetSucc(misc);
        }
    }
    strcpy(AreaName, dnode->fc->Name);
    if(dnode->flist) {
        struct FileNode *fn;

        FPrintf(fh, "\nArea: %s\n\n", StrBuf);
        fn = GetHead(dnode->flist);
        while(fn) {
            dt->dat_Stamp.ds_Days = fn->Days;
            DateToStr(dt);
            if(fn->LongDesc && fn->DescLines) {
                long cnt;

                FPrintf(fh, filefmt, fn->Name, fn->Size, dt->dat_StrDate, fn->DescLines[0]);
                for(cnt = 1; cnt < fn->LongDesc; cnt++) FPrintf(fh, "%36s%s\n", "", fn->DescLines[cnt]);
            } else FPrintf(fh, filefmt, fn->Name, fn->Size, dt->dat_StrDate, fn->Comment);
            fn = GetSucc(fn);
        }
    }
    if(dnode->dlist) {
        struct DirNode *dn;
        char *OldName = AreaName;

        strcat(AreaName, " -> ");
        AreaName += strlen(AreaName);
        dn = GetHead(dnode->dlist);
        while(dn) {
            WriteList(dn, fh);
            dn = GetSucc(dn);
        }
        AreaName = OldName;
    }
}
