/*
** CINS Configuration declarations
** by Kelly Cochran
** Last updated: Mon Dec 29 01:07:26 1997
*/

#ifndef _CINS_CONFIG_H
#define _CINS_CONFIG_H

struct NcFTPCfg
{
   UBYTE Progress;         /*   0 */
   UBYTE ANSIEsc;          /*   1 */
   UBYTE AutoBin;          /*   2 */
   UBYTE Verbose;          /*   3 */
   UBYTE AutoAnon;         /*   4 */
   UBYTE Recent;           /*   5 */
   UBYTE Tips;             /*   6 */
   UBYTE MGet;             /*   7 */
};                         /*   8 */

struct CINSCfg
{
   char   UserDir [ 60 ];      /*   0 */
   char   URL     [ 60 ];      /*  60 */
   char   Gopher  [ 60 ];      /* 120 */
   char   IRC     [ 60 ];      /* 180 */
   char   Archie  [ 60 ];      /* 240 */
   struct NcFTPCfg def_ftp;    /* 300 */
   UBYTE  def_Shell;           /* 308 */
   UBYTE  def_Muff;            /* 309 */
   UBYTE  def_DirANSI;         /* 310 */
   UBYTE  Version;             /* 311 */
   long   Serial;              /* 312 */
   short  Line;                /* 316 */
   short  Log;                 /* 318 */
   short  Days;                /* 320 */
   short  Access;              /* 322 */
   short  FTPin;               /* 324 */
   short  Menu;                /* 326 */
   UBYTE  Expansion2[ 2 ];     /* 328 */
   UBYTE  StackType;           /* 330 */
   UBYTE  AddNewToDB;          /* 331 */
   long   obs_WebFree;         /* 332 */
   struct CINSAccEntry *AE;    /* 336 */   /* 32 in the array   */
   struct List *archie;        /* 340 */   /* Archie list       */
   struct List *ftp;           /* 344 */   /* Main ftp list     */
   struct List *irc;           /* 348 */   /* IRC list          */
   struct List *telnet;        /* 352 */   /* Main telnet list  */
};                             /* 356 */

struct CINSAccEntry
{
   ULONG WebFree;             /*   0 */
   ULONG FTPFree;             /*   4 */
   ULONG Rate;                /*   8 */
   ULONG Flags;               /*  12 */
   ULONG PurgeDays;           /*  16 */
   UBYTE Expansion [ 16 ];    /*  20 */
};                            /*  36 */ 

struct OldCINSAccEntry
{
   ULONG WebFree;             /*   0 */
   ULONG FTPFree;             /*   4 */
   ULONG Rate;                /*   8 */
   ULONG Flags;               /*  12 */
};                            /*  16 */ 

/* #defines for CINSAccEntry.Flags */

#define CINSAEF_UPLOADFTP   (1 << 0)      /* Enable Upload to UserDir?      */
#define CINSAEF_UPLOADWEB   (1 << 1)      /* Enable Upload to WebDir?       */
#define CINSAEF_CD          (1 << 2)      /* Enable CD command in DirMaint? */

/* End of CINSAccEntry.Flags #defines */

/* #defines for stack type */

#define ST_UNKNOWN 0
#define ST_AMITCP  1
#define ST_MIAMI   2

/* end of CINSCfg.StackType #defines */

struct CINSUCfg
{
   UBYTE IUC      [ 100 ];    /*   0 */
   char  HomeDir  [  64 ];    /* 100 */
   char  WebDir   [  64 ];    /* 164 */
   char  IRCNick  [  12 ];    /* 228 */
   struct NcFTPCfg ftp;       /* 240 */
   UBYTE Shell;               /* 248 */
   UBYTE Muff;                /* 249 */
   UBYTE DirANSI;             /* 250 */
   UBYTE Expansion1;          /* 251 */
   short TimeEnter;           /* 252 */
   UBYTE Expansion2 [ 2 ];    /* 254 */
   struct CINSAccEntry uae;   /* 256 */ /* Schedule for current user */
   struct List        *ftpnl; /* 292 */
   struct List        *tnnl;  /* 296 */
   struct List        *arcnl; /* 300 */
   struct List        *ircnl; /* 304 */
};                            /* 308 */

struct FTPSite
{
   char   Name  [ 80 ];      /*   0 */
   char   Host  [ 80 ],      /*  80 */
          Comm1 [ 80 ],      /* 160 */
          Comm2 [ 80 ];      /* 240 */
};

struct FTPEntry
{
   struct Node    ftp_Node;  /*   0 */
   ULONG  pad;               /*  14 */
   struct FTPSite ftp_Site;  /*  18 */
};                           /* 338 */

struct FTPMSite
{
   char   Menu [ 80 ];      /*  0 */
};                          /* 80 */

struct FTPMenu
{
   struct Node     ftpm_Node; /*   0 */
   ULONG  entries;            /*  14 */
   struct FTPMSite ftpm_Site; /*  18 */
   struct List    *ftpm_List; /*  98 */
};                            /* 102 */

struct TelnetSite
{
   char   Name  [ 80 ];     /*   0 */
   char   Host  [ 80 ],     /*  80 */
          Comm1 [ 80 ],     /* 160 */
          Comm2 [ 80 ];     /* 240 */
};

struct TelnetEntry
{
   struct Node       tn_Node; /*   0 */
   ULONG  pad;                /*  14 */
   struct TelnetSite tn_Site; /*  18 */
};                            /* 338 */

struct TelnetMSite
{
   char   Menu [ 80 ];      /*  0 */
};                          /* 80 */

struct TelnetMenu
{
   struct Node        tnm_Node;  /*   0 */
   ULONG  entries;               /*  14 */
   struct TelnetMSite tnm_Site;  /*  18 */
   struct List       *tnm_List;  /*  98 */
};                               /* 102 */

struct IRCSite
{
   char   Site [ 80 ];       /*   0 */
   char   Host [ 80 ];       /*  80 */
};                           /* 160 */

struct IRCEntry
{
   struct Node    irc_Node;  /*   0 */
   ULONG  pad;               /*  14 */
   struct IRCSite irc_Site;  /*  18 */
};                           /* 178 */

struct ArchieSite
{
   char   Site [ 64 ];      /*  0 */
};                          /* 80 */

struct ArchieEntry
{
   struct Node       arc_Node;  /*  0 */
   ULONG  pad;                  /* 14 */
   struct ArchieSite arc_Site;  /* 18 */
};                              /* 82 */

struct DirEntryNode
{
   char   FileName  [  32 ];    /*   0 */
   char   Date      [  32 ];    /*  32 */
   char   FNReal    [ 128 ];    /*  64 */
   long   Size;                 /* 192 */
};                              /* 196 */

struct DirEntryList
{
   struct Node         del_Node; /*   0 */
   struct DirEntryNode del_DEN;  /*  14 */
};                               /* 210 */

#endif /* _CINS_CONFIG_H */
