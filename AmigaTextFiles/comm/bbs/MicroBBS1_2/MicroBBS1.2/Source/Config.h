// Config.h

#ifndef MBBS_CONFIG_H
#define MBBS_CONFIG_H

#include    <dos/dos.h>
#include    <exec/types.h>
#include    <exec/memory.h>
#include    <exec/lists.h>
#include    <graphics/text.h>
#include    <intuition/intuition.h>

#include    <clib/exec_protos.h>

#include    "Misc.h"

#define     CONFIGNODE_ID   100
#define     CONFIGSUB_ID    101

enum long
{
    DEVICE,
    UNIT,
    BAUD,
    NODE,
    LOCAL,
    CONNECT,
    CONFIG,
    EXCLUSIVE,
    RD_Size
};

struct  ConfigNode
{
    struct Node cn_Node;
    char        cn_ID[ STRING_LENGTH ];
    char        cn_String[ STRING_LENGTH ];
    long        cn_Value;
};

void CheckArgs();

BOOL ReadConfig( char *, struct List**);
void ReadConfigSub( char *, char *, struct List*);

void AnalyseConfig();

void WriteConfig( char *, struct List *);

char *FindAtom( char *, char *, char);
char *FindAtomEnd( char *, char *);
char *FindNextChar( char *, char *);
char *FindNextSpace( char *, char *);

struct Node *FindNameNoCase( struct List *, char *);

struct ConfigNode *AddNode( char *, char *, long, struct List *);
struct List *AddHeadNode( char *, struct List *);
void ClearList( struct List *);
void PutList( struct List *, BPTR, long);

#endif
