/*
** CINS Link Library Definitions
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:34:32 1999
*/

#ifndef CINS_LIB_H
#define CINS_LIB_H

#include "CINS.h"

extern struct MainPort *myp;
extern struct Library  *CNetBase;
extern struct Library  *CNetCBase;
extern struct CINSCfg   Config;
extern struct CINSUCfg  UCfg;

#ifndef CINS_MAINT_H
extern struct PortData *z;
extern char **bm;
extern void err(char *error);
#endif

extern void GetOut(void);

void read_config(void);

#endif /* !CINS_LIB_H */
