/* DoHighReadMagic.c */

#include "exec/io.h"
#include "exec/memory.h"
#include "exec/libraries.h"
#include "exec/execbase.h"
#include "dos/dos.h"
#include "sh/sub.h"
#include "techprg.h"
#include "proto/exec_protos.h"
#include "proto/dos_protos.h"

#define TRUE 1
#define FALSE 0

int _main(void);
void error(u_char *),cleanup(void);

struct UserVal *user;
struct RdArds *ra;

BPTR file,bkup;
ULONG args[5],len,*hr,num;

u_char template[]="PATH/A,BKUP,LOAD/S,SAVE/S,VIEW/S",bbuf[128],
    ver[]="$VER: DoHighReadMagic 1.00",cmd[32],buf[128],userfile[4096];

#define arg_path    (u_char *)args[0]
#define arg_bkup    (u_char *)args[1]
#define arg_load    args[2]
#define arg_save    args[3]
#define arg_view    args[4]

int _main(void) {
LONG i;
u_char *ch,*bufch;
    GetProgramName(cmd,32);
    if(!(ra=ReadArgs(template,args,NULL))) {
        PrintFault(i=IoErr(),cmd);
        exit(i);
    }
    strcpy(buf,arg_path);
    AddPart(buf,"User.dat",128);
    strcpy(bbuf,arg_path);
    AddPart(bbuf,arg_bkup?arg_bkup:(u_char *)"HighRead.backup",128);
    if(file=Open(buf,MODE_OLDFILE)) {
        Seek(file,0,OFFSET_END);
        len=Seek(file,0,OFFSET_BEGINNING);
        if(Read(file,userfile,len)==len) {
            ch=userfile;
            bufch=buf;
            while(*ch && *ch!=10 && *ch!=13) *bufch++=*ch++;
            *bufch=0;
            for(i=0;i<6;ch++,i++)
                while(*ch && *ch!=10 && *ch!=13) ch++;
            user=(struct UserVal *)ch;
            hr=(ULONG *)(user+1);
            num=*hr++;
            if(arg_view) {
                print("Viewing message pointers for %ld areas.\n",num);
                for(i=0;i<num;i++) print("Highread for area %ld: %ld\n",
                    i+1,hr[i]);
            } else if(arg_load) {
                if(bkup=Open(bbuf,MODE_OLDFILE)) {
                    Read(bkup,buf,8);
                    if(*((ULONG *)buf)==(ULONG)'HRBK') {
                        i=*((ULONG *)(buf+4));
                        if(i<num) rawprint("There are less message pointers stored in the backup than there\nare in the user file. The rest of the pointers will remain untouched.\n");
                            else if(i>num) rawprint("There are more message pointers stored in the backup than there\nare in the user file. The rest of the pointers will not be loaded.\n");
                        print("Loading message pointers for %ld areas.\n",num>i?i:num);
                        Read(bkup,hr,num*sizeof(ULONG));
                        Seek(file,0,OFFSET_BEGINNING);
                        Write(file,userfile,len);
                    } else print("%s: not a valid backup file\n",cmd);
                } else print("%s: unable to open backup file\n",cmd);
            } else if(arg_save) {
                if(bkup=Open(bbuf,MODE_NEWFILE)) {
                    Write(bkup,"HRBK",4);
                    Write(bkup,&num,4);
                    print("Saving message pointers for %ld areas.\n",num);
                    Write(bkup,hr,num*sizeof(ULONG));
                } else print("%s: unable to open backup file\n",cmd);
            } else print("No operation.\n");
            rawprint("©1995 by Sami Klemola.\n");
        } else print("%s: read error\n",cmd);
    } else print("%s: unable to open user file\n",cmd);
    cleanup();
    return(RETURN_OK);
};

void error(u_char *str) {
    print("%s: %s\n",cmd,str);
    cleanup();
    exit(RETURN_FAIL);
};

void cleanup(void) {
    if(bkup) Close(bkup);
    if(file) Close(file);
    if(ra) FreeArgs(ra);
};

