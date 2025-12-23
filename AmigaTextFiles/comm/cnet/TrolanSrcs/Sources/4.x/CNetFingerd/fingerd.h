/*
** CNet/4 Fingerd Header
** by Kelly Cochran
** Last updated: Mon Mar 22 13:27:49 1999
*/

#include <sys/syslog.h>
#include <bsdsocket.h>
#include <clib/netlib_protos.h>

#define FINGER_DIR "SysText:fingerd"
#define VERS       "3.9"

struct MainPort *myp;
struct Library  *CNetBase  = NULL;
struct Library  *CNet4Base = NULL;

void main(void);
void FRemoveMCI(char *in);
void PrintFile(char *file);
void FDisplay(long uid, char *name);
void do_who(void);
void do_finger(char *uids);

const char *version = "\0$VER: CNet/4_Fingerd "VERS" "__AMIGADATE__;
