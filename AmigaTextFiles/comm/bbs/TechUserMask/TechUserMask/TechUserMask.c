/* TechUserMask v2.00! Written by Sami Klemola in the year of 1995. */

#include "exec/io.h"
#include "exec/memory.h"
#include "exec/libraries.h"
#include "dos/dos.h"
#include "proto/exec_protos.h"
#include "proto/dos_protos.h"
#include "techprg.h"

#define TRUE 1
#define FALSE 0

typedef unsigned char u_char;

extern LONG rawprint(u_char *),print(u_char *,...);

int _main(void);
ULONG GetUser(void),input(u_char *);
void ReadFile(u_char *),
    donewmask(void),gethotkey(void),error(u_char *),cleanup(void);

extern __far struct Library *DOSBase,*PrivateDosBase;

struct RDArgs *ra;
struct UserVal *user;

BPTR ifh,file;
ULONG len,cfglen,Mask,newmask,args[6];
u_char *userfile,buf[32],cmd[32],verstr[]="$VER: TechUserMask 2.00",
    template[]="User,MASK/K/N,CFG=CONFIG/K,INTER/S,FULL/S,CHECK/S",
    ERR_CFG[]="invalid configuration entry",
    ERR_MEM[]="unable to allocate memory",*username,*mem,*bits[32],ibuf[32];

int _main(void) {
ULONG i,b;
u_char *t;
    PrivateDosBase=DOSBase;
    GetProgramName(cmd,32);
    if(!(ra=ReadArgs(template,&args[0],NULL))) {
        PrintFault(i=IoErr(),cmd);
        exit(i);
    }
    ifh=Input();
    if(!(userfile=AllocVec(2048,MEMF_ANY))) error(ERR_MEM);
    username=(u_char *)args[0];
    if(args[1]) newmask=*((ULONG *)args[1]);
    ReadFile(args[2]?(u_char *)args[2]:(u_char *)"BBS:Cfg/TechMisc.Cfg");
    if(!(t=(u_char *)strstr(mem,"MASKBITS"))) error("configuration entry not found");
    for(i=0;i<32;i++) {
        while((*t!=10)&&*t) t++;
        if(!*t) error(ERR_CFG);
        *t++=0;
        while(*t==10) t++;
        if(!*t) error(ERR_CFG);
        if(*t!='-') bits[i]=t;
    }
    rawprint("TechUserMask v2.00!\n");
    if(args[5]) {
        rawprint("Checking bits...\n\n");
        for(i=0;i<32;i++) if(bits[i])
            print("Mask bit number %ld is used for access to %s\n",i,bits[i]);
            else if(args[4]) print("Mask bit number %ld is unused\n",i);
        rawprint("\nPress any key...");
        i=FGetC(ifh);
        rawprint("\n\n");
    }
    if(!username) rawprint("Viewing users' mask bits...\n\n");
        else print("Searching for %s...\n",username);
    if(!(GetUser())) error("user file not found");
    if(username) if(args[3]) {
        newmask=Mask;
        while(TRUE) {
            print("\fEditing access for %s. Current access:\n\n",buf);
            for(i=0;i<32;i++) if(bits[i])
                print("%ld\r\t\b\b\b\b%s..................................\r\t\t\t\t%s                              \n",i,bits[i],newmask&(1<<i)?"Yes":"No");
            if(!(input("\n\nEnter the number of the bit to be toggled: ")))
                break;
            b=atoi(ibuf);
            if(b>31) {
                rawprint("\n\nValue out of range.\n");
                gethotkey();
            } else {
                b=1<<b;
                if(newmask&b) newmask&=-1-b; else newmask|=b;
            }
        }
        if(newmask!=Mask) donewmask();
    } else {
        print("Mask bits for %s are 0x%lx.\n%s has access to:\n\n",
            buf,Mask,buf);
        if(!Mask) rawprint("...absolutely nowhere!\n");
            else for(i=0;i<32;i++) if((Mask&(1<<i))&&(bits[i]))
            print("........ %s\n",bits[i]);
        if(args[1]) if(newmask!=Mask) donewmask();
    }
    print("\nTechUserMask is copyright in 1995 by Sami Klemola.\n");
    cleanup();
    return(RETURN_OK);
};

void donewmask() {
ULONG i;
u_char *m;
    print("\nAccording to your new specification %s would have access to:\n\n",buf);
    if(!newmask) rawprint("...absolutely nowhere!\n");
        else for(i=0;i<32;i++) if((newmask&(1<<i))&&(bits[i]))
    print("........ %s\n",bits[i]);
    print("\nAre you sure you want to change this user's mask to 0x%lx? ",newmask);
    i=FGetC(ifh);
    if((i==(ULONG)'y')||(i==(ULONG)'Y')) {
        m=((u_char *)&user->Mask)+3;
        for(i=0;i<4;i++) {
            *m--=(u_char)newmask;
            newmask=newmask>>8;
        }
        Seek(file,0,OFFSET_BEGINNING);
        Write(file,userfile,len);
        rawprint("Done");
    } else rawprint("Operation cancelled");
};

ULONG GetUser(void) {
struct AnchorPath *anch;
u_char *ch,*bufch,*m;
ULONG err,i;
    if(!(anch=AllocVec(sizeof(struct AnchorPath)+256,MEMF_CLEAR|MEMF_PUBLIC))) error(ERR_MEM);
    anch->ap_Strlen=256;
    err=MatchFirst("BBS:User/#?/User.dat",anch);
    while(!err) {
        if(anch->ap_Info.fib_DirEntryType<0) {
            if(file) Close(file);
            file=Open(anch->ap_Buf,MODE_OLDFILE);
            if(file) {
                Seek(file,0,OFFSET_END);
                len=Seek(file,0,OFFSET_BEGINNING);
                if(Read(file,userfile,len)==len) {
                    ch=userfile;
                    bufch=buf;
                    while(*ch && *ch!=10 && *ch!=13) *bufch++=*ch++;
                    *bufch=0;
                    for(err=0;err<6;ch++,err++)
                        while(*ch && *ch!=10 && *ch!=13) ch++;
                    user=(struct UserVal *)ch;
                    Mask=0;
                    m=((u_char *)&user->Mask);
                    for(i=0;i<4;i++) {
                        Mask=Mask<<8;
                        Mask+=(ULONG)m[i];
                    }
                    if(!username)
                        print("%s\r\t\t\t\t0x%lx\n",buf,Mask);
                    else if(!(strcmp(buf,username))) {
                        MatchEnd(anch);
                        FreeVec(anch);
                        return(TRUE);
                    }
                }
            }
        }
        err=MatchNext(anch);
    }
    MatchEnd(anch);
    FreeVec(anch);
    return(username?FALSE:TRUE);
};

void ReadFile(unsigned char *fn) {
BPTR cfgfile;
    if(!(cfgfile=Open(fn,MODE_OLDFILE)))
        error("can not read configuration file in");
    Seek(cfgfile,0,OFFSET_END);
    if(!(cfglen=Seek(cfgfile,0,OFFSET_BEGINNING))) {
        Close(cfgfile);
        error("configuration file is empty");
    }
    if(!(mem=(u_char *)AllocVec(cfglen,MEMF_ANY))) error(ERR_MEM);
    Read(cfgfile,mem,cfglen);
    Close(cfgfile);
};

void gethotkey(void) {
    rawprint("\nPress enter... ");
    while(FGetC(ifh)!=10);
    rawprint("\n");
};

ULONG input(u_char *query) {
ULONG i,c;
u_char *t;
    rawprint(query);
    t=ibuf;
    i=0;
    while(i<30) if((c=FGetC(ifh))==10) break; else {
        *t++=c;
        i++;
    }
    *t=0;
    return(i);
};

void error(unsigned char *errstr) {
    cleanup();
    print("%s: %s\n",cmd,errstr);
    exit(RETURN_FAIL);
};

void cleanup(void) {
    if(file) Close(file);
    if(userfile) FreeVec(userfile);
    if(mem) FreeVec(mem);
    FreeArgs(ra);
};

