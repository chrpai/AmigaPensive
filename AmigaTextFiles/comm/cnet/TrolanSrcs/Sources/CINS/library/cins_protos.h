/*
** CINS Shared Library Definitions
** Copyright 1997, TGGH, Inc.
** by Kelly Cochran
** Last Updated: Wed Dec 03 00:31:45 1997
*/

#ifndef CINSLIBRARY_PROTOS_H
#define CINSLIBRARY_PROTOS_H

#include <exec/types.h>
#include <exec/memory.h>

#include <proto/exec.h>
#include <proto/intuition.h>

extern ULONG __saveds __asm CINSReserved1(void);

extern struct List * __saveds __asm CINS_InitList(void);

extern void __saveds __asm CINS_FreeList(register __a1 struct List **list);

extern void __saveds __asm CINS_AddFTPName(register __a1 struct List *list,
                               register __a2 struct FTPMSite *ftpms);
extern void __saveds __asm CINS_AddFTPDataName(register __a1 struct List *list,
                                   register __a2 struct FTPSite *ftps);
extern void __saveds __asm CINS_AddTelnetName(register __a1 struct List *list,
                                  register __a2 struct TelnetMSite *tnms);
extern void __saveds __asm CINS_AddTelnetDataName(register __a1 struct List *list,
                                      register __a2 struct TelnetSite *tns);
extern void __saveds __asm CINS_AddIRCName(register __a1 struct List *list,
                               register __a2 struct IRCSite *ircs);
extern void __saveds __asm CINS_AddArchieName(register __a1 struct List *list,
                                  register __a2 struct ArchieSite *arcs);
extern void __saveds __asm CINS_AddDirEntryName(register __a1 struct List *list,
                                    register __a2 struct DirEntryNode *den);
extern BOOL __saveds __asm CINS_RemoveNode(register __a1 struct List *list,
                               register __d1 int node);
extern struct Node * __saveds __asm CINS_FindOrdinal(register __a1 struct List *list,
                               register __d1 int node);
extern void __saveds __asm CINS_InsertArchieName(register __a1 struct List *list,
                                  register __a2 struct ArchieSite *arcs,
                                  register __d1 int pos); 
extern void __saveds __asm CINS_InsertFTPName(register __a1 struct List *list,
                                  register __a2 struct FTPMSite *ftpms,
                                  register __d1 int pos); 
extern void __saveds __asm CINS_InsertFTPDataName(register __a1 struct List *list,
                                  register __a2 struct FTPSite *ftps,
                                  register __d1 int pos); 
extern void __saveds __asm CINS_InsertIRCName(register __a1 struct List *list,
                                  register __a2 struct IRCSite *ircs,
                                  register __d1 int pos); 
extern void __saveds __asm CINS_InsertTelnetName(register __a1 struct List *list,
                                  register __a2 struct TelnetMSite *tnms,
                                  register __d1 int pos); 
extern void __saveds __asm CINS_InsertTelnetDataName(register __a1 struct List *list,
                                  register __a2 struct TelnetSite *tns,
                                  register __d1 int pos); 
#endif
