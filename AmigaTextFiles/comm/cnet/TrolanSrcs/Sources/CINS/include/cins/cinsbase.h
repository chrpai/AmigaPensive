/*
** CINS Shared Library structure definition
** Last updated: Tue Sep 30 23:52:05 1997
*/

#ifndef CINS_CINSBASE_H
#define CINS_CINSBASE_H

#include <exec/libraries.h>

struct CINSBase
{
 struct Library         cins_LibNode;
 APTR                   cins_SegList;
 struct ExecBase       *cins_SysBase;
 struct IntuitionBase  *cins_IntuitionBase;
 struct GfxBase        *cins_GfxBase;
};

#endif
