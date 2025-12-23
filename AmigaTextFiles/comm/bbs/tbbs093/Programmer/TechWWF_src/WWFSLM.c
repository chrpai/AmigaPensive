#include <TechWWF.h>
#include <WWFData.h>

struct MsgReq *mr;

extern struct CharSet *InCSet, *OutCSet;

void ConvString(__A0 UBYTE *, __A1 UBYTE *, __A2 UBYTE *);

struct FMsg *LoadMsg(struct AreaCfg *ac, ULONG MsgNum)
{
    struct LoadedMsg *LM;

    if(!mr) mr = calloc(sizeof(struct MsgReq), 1);
    mr->Area = ac->AreaNr;
    mr->MsgNum = MsgNum;
    LM = SendBBSMsg(ID_LOADMSG, mr);
    if(LM) return(LM->msg);
    else return(NULL);
}

void SaveMsg(struct FMsg *bmsg, struct AreaCfg *ac, ULONG MsgLen, UBYTE *MText, UBYTE *ToUser)
{
    if(!mr) mr = calloc(sizeof(struct MsgReq), 1);
    mr->Area = ac->AreaNr;
    mr->bmsg = bmsg;
    mr->MsgLen = MsgLen;
    mr->MText = MText;
    mr->ToUser = ToUser;
    SendBBSMsg(ID_SAVEMSG, mr);
}

void UpdateMsg(struct FMsg *bmsg, struct AreaCfg *ac)
{
    if(!mr) mr = calloc(sizeof(struct MsgReq), 1);
    mr->bmsg = bmsg;
    mr->Area = ac->AreaNr;
    SendBBSMsg(ID_UPDATEMSG, mr);
}

struct MemMsg *F2MMsg(struct FMsg *fm)
{
    struct MemMsg *mmsg;
    UBYTE *scan;

    mmsg = calloc(sizeof(struct MemMsg), 1);
    memmove(mmsg, fm, sizeof(struct FMsg));
    mmsg->AttFile = scan = (UBYTE *)(&fm[1]);
    while(*scan) scan++;
    mmsg->FromUser = ++scan;
    while(*scan) scan++;
    mmsg->ToUser = ++scan;
    while(*scan) scan++;
    mmsg->Subject = ++scan;
    while(*scan) scan++;
    mmsg->MsgText = ++scan;
    if(OutCSet) {
        mmsg = realloc(mmsg, sizeof(struct MemMsg) + strlen(mmsg->FromUser) + strlen(mmsg->ToUser) + strlen(mmsg->Subject) + strlen(mmsg->MsgText) + 10);
        scan = (char *)&mmsg[1];
        ConvString(mmsg->FromUser, scan, OutCSet->CnvTo);
        mmsg->FromUser = scan;
        while(*scan) scan++;
        ConvString(mmsg->ToUser, ++scan, OutCSet->CnvTo);
        mmsg->ToUser = scan;
        while(*scan) scan++;
        ConvString(mmsg->Subject, ++scan, OutCSet->CnvTo);
        mmsg->Subject = scan;
        while(*scan) scan++;
        ConvString(mmsg->MsgText, ++scan, OutCSet->CnvTo);
        mmsg->MsgText = scan;
    }
    return(mmsg);
}
