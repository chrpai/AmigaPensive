/*
** CINS Shared Library Definitions
** Copyright 1997, TGGH, Inc.
** by Kelly Cochran
** Last Updated: Wed Dec 03 00:33:17 1997
*/

#ifndef PROTO_CINS_PROTOS_H
#define PROTO_CINS_PROTOS_H

#define CINSLIBNAME     "CINS:Libs/cins.library"
#define CINSLIBVERSION  4L
#define CINSLIBREVISION 16L

struct List *CINS_InitList(void);

void CINS_FreeList(struct List **list);

void CINS_AddFTPName(struct List *list,
                struct FTPMSite *ftpms);
void CINS_AddFTPDataName(struct List *list,
                    struct FTPSite *ftps);
void CINS_AddTelnetName(struct List *list,
                   struct TelnetMSite *tnms);
void CINS_AddTelnetDataName(struct List *list,
                       struct TelnetSite *tns);
void CINS_AddIRCName(struct List *list,
                struct IRCSite *ircs);
void CINS_AddArchieName(struct List *list,
                   struct ArchieSite *arcs);
void CINS_AddDirEntryName(struct List *list,
                     struct DirEntryNode *den);
BOOL CINS_RemoveNode(struct List *list,
                int node);
struct Node *CINS_FindOrdinal(struct List *list,
                         int node);
void CINS_InsertArchieName(struct List *list,
            struct ArchieSite *arcs,
            int pos);
void CINS_InsertFTPName(struct List *list,
            struct FTPMSite *ftpms,
            int pos); 
void CINS_InsertFTPDataName(struct List *list,
            struct FTPSite *ftps,
            int pos); 
void CINS_InsertIRCName(struct List *list,
            struct IRCSite *ircs,
            int pos); 
void CINS_InsertTelnetName(struct List *list,
            struct TelnetMSite *tnms,
            int pos); 
void CINS_InsertTelnetDataName(struct List *list,
            struct TelnetSite *tns,
            int pos); 
#endif
