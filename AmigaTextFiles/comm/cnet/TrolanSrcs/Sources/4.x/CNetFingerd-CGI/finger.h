/*
** CNet/4 Fingerd-CGI Header
** by Kelly Cochran
** Last updated: Sun Nov 30 19:49:01 1997
*/

#define VERS       "4.5"

struct MainPort *myp       = NULL;
struct Library  *CNetBase  = NULL;
struct Library  *CNet4Base = NULL;

void PrintFile(char *file);
void printMCI(char *text);
void do_finger(char *uids);
void __stdargs _CXOVF(void) { __exit(20) ; } /* Prevents requestors */

const char *version = "\0$VER: CNet/4_Fingerd_CGI "VERS" "__AMIGADATE__;
