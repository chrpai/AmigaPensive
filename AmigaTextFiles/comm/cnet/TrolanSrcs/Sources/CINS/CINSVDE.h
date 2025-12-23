/*
** CINS VDE Declarations
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:24:53 1999
*/

#ifndef _CINSVDE_H
#define _CINSVDE_H

#define VDE_SIZE(array)  (sizeof(array)/sizeof(struct VDEentry))

#ifdef CINSCONFIG_VDE_C

struct VDEentry CCfg[] = {
   {"Save changes         ",1,4,    /* 0 */
     0,0,0,
     -1,0,
     10,-1,
     0,0,0,1,0},

   {"Exit without saving  ",23,4,   /* 1 */
     0,0,0,
     -1,0,
     10,-2,
     1,1,0,1,0},

   {"<< Exit              ",1,4,    /* 2 */
     0,0,0,
     -1,0,
     0,-10,
     26,3,2,2,0},

   {"Access minimum      :",1,6,    /* 3 */
     0,31,4,
     4,offsetof(struct CINSCfg, Access),
     0,0,
     2,4,3,3,0},

   {"Add Users to DB?    :",1,7,  /* 4 */
     0,0,0,
     0,offsetof(struct CINSCfg, AddNewToDB),
     0,0,
     3,5,4,4,0},

   {"Max. FTPs at once   :",1,8,    /* 5 */
     0,999,4,
     4,offsetof(struct CINSCfg, FTPin),
     0,0,
     4,6,14,14,0},

   {"Expansion 1          ",1,9,    /* 6 */
     0,400,4,
     4,0,
     0,0,
     5,7,15,15,(1<<0)},

   {"BBSMenu start       :",1,10,    /* 7 */
     40,999,6,
     4,offsetof(struct CINSCfg, Menu),
     0,0,
     6,8,16,16,0},

   {"Expansion 2         :",1,11,    /* 8 */
     2100,9999,6,
     4,offsetof(struct CINSCfg, Line),
     0,0,
     7,9,17,17,(1<<0)},

   {"Log ID #            :",1,12,   /* 9 */
     600,700,6,
     4,offsetof(struct CINSCfg, Log),
     0,0,
     8,10,18,18,0},

   {"Def.User ANSIDir/Web:",1,13,   /* 10 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_DirANSI),
     0,0,
     9,11,19,19,0},

   {"Def.User Expert Mode:",1,14,   /* 11 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_Shell),
     0,0,
     10,12,20,20,0},

   {"Def.User Muffle All :",1,15,   /* 12 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_Muff),
     0,0,
     11,22,21,21,0},

   {" Default FTP Options ",40,6,      /* 13 */
     0,0,0,
     -1,0,
     0,0,
     -1,0,0,0,0},

   {"FTP Progress meter  :",40,8,    /* 14 */
     2250,4,18,
     9,offsetof(struct CINSCfg, def_ftp.Progress),
     0,0,
     21,15,5,5,0},

   {"ANSI Escapes enabled:",40,9,   /* 15 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_ftp.ANSIEsc),
     0,0,
     14,16,6,6,0},

   {"Auto-Binary Mode on :",40,10,   /* 16 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_ftp.AutoBin),
     0,0,
     15,17,7,7,0},

   {"Message Verbosity   :",40,11,   /* 17 */
     2255,3,18,
     9,offsetof(struct CINSCfg, def_ftp.Verbose),
     0,0,
     16,18,8,8,0},

   {"Auto-Anonymous login:",40,12,  /* 18 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_ftp.AutoAnon),
     0,0,
     17,19,9,9,0},

   {"Recent list         :",40,13,  /* 19 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_ftp.Recent),
     0,0,
     18,20,10,10,0},

   {"Show FTP Tips       :",40,14,  /* 20 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_ftp.Tips),
     0,0,
     19,21,11,11,0},

   {"Prompt on mget      :",40,15,  /* 21 */
     0,0,0,
     0,offsetof(struct CINSCfg, def_ftp.MGet),
     0,0,
     20,14,12,12,0},

   {"UserDir Path        :",1,17,      /* 22 */
     0,0,50,
     2,offsetof(struct CINSCfg, UserDir),
     0,0,
     12,23,22,22,0},

   {"Lynx default URL    :",1,18,      /* 23 */
     0,0,50,
     2,offsetof(struct CINSCfg, URL),
     0,0,
     22,24,23,23,0},

   {"Def. Gopher server  :",1,19,      /* 24 */
     0,0,50,
     2,offsetof(struct CINSCfg, Gopher),
     0,0,
     23,25,24,24,0},

   {"Def. IRC Server     :",1,20,      /* 25 */
     0,0,50,
     2,offsetof(struct CINSCfg, IRC),
     0,0,
     24,26,25,25,0},

   {"Def. Archie server  :",1,21,      /* 26 */
     0,0,50,
     2,offsetof(struct CINSCfg, Archie),
     0,0,
     25,2,26,26,0},

   {"    CINS v"VERS"       ",1,1,      /* 27 */
     0,0,0,
     -1,0,
     0,0,
     -1,0,0,0,0},

   {"          License #: ",1,2,    /* 28 */
     0,0,12,
     6,offsetof(struct CINSCfg, Serial),
     0,0,
     -1,0,0,0,0},
};

#endif /* CINSCONFIG_C */

#ifdef CINSEDIT_C

struct VDEentry UserConfig[] = {
   {"Save changes         ",1,4,/* 0 */
     0,0,0,
     -1,0,
     10,-1,
     0,0,0,1,0},

   {"Exit without saving  ",23,4,/* 1 */
     0,0,0,
     -1,0,
     10,-2,
     1,1,0,1,0},

   {"<< Exit              ",1,4,/* 2 */
     0,0,0,
     -1,0,
     0,-10,
     16,3,2,2,0},

   {"Muffle other users  :",1,6,  /* 3  */
     0,0,0,
     0,offsetof(struct CINSUCfg, Muff),
     0,0,
     2,4,6,6,0},

   {"Expert mode enabled :",1,7,  /* 4  */
     0,0,0,
     0,offsetof(struct CINSUCfg, Shell),
     0,0,
     3,5,7,7,0},

   {"ANSI Dir/WebMaint   :",1,8,  /* 5  */
     0,0,0,
     0,offsetof(struct CINSUCfg, DirANSI),
     0,0,
     4,9,8,8,0},

   {"IRC Nickname        :",40,6,  /* 6  */
     0,0,12,
     2,offsetof(struct CINSUCfg, IRCNick),
     0,0,
     8,7,3,3,0},

   {"Expansion 2         :",40,7,  /* 7  */
     0,0,0,
     -1,-1,
     0,0,
     6,8,4,4,(1<<1)},         /* z->uz[1] == TRUE? */

   {"Expansion 3         :",40,8,  /* 8  */
     0,0,0,
     -1,-1,
     0,0,
     7,6,5,5,(1<<1)},

   {"FTP progress meter  :",1,10,  /* 9  */
     2250,4,20,
     9,offsetof(struct CINSUCfg, ftp.Progress),
     0,0,
     5,10,9,9,0},

   {"ANSI escapes        :",1,11,  /* 10  */
     0,0,0,
     0,offsetof(struct CINSUCfg, ftp.ANSIEsc),
     0,0,
     9,11,10,10,0},

   {"Auto-binary mode on :",1,12,  /* 11  */
     0,0,0,
     0,offsetof(struct CINSUCfg, ftp.AutoBin),
     0,0,
     10,12,11,11,0},

   {"Message verbosity   :",1,13,  /* 12  */
     2255,3,20,
     9,offsetof(struct CINSUCfg, ftp.Verbose),
     0,0,
     11,13,12,12,0},

   {"Auto-anonymous login:",1,14,  /* 13  */
     0,0,0,
     0,offsetof(struct CINSUCfg, ftp.AutoAnon),
     0,0,
     12,14,13,13,0},

   {"Recent list         :",1,15,  /* 14  */
     0,0,0,
     0,offsetof(struct CINSUCfg, ftp.Recent),
     0,0,
     13,15,14,14,0},

   {"Show FTP tips       :",1,16,  /* 15  */
     0,0,0,
     0,offsetof(struct CINSUCfg, ftp.Tips),
     0,0,
     14,16,15,15,0},

   {"Prompt on mget      :",1,17,  /* 16   */
     0,0,0,
     0,offsetof(struct CINSUCfg, ftp.MGet),
     0,0,
     15,2,16,16,0},

   {" CINS v"VERS"     "__DATE__"",1,1,      /* 17 */
     0,0,0,
     -1,0,
     0,0,
     0,0,0,0,0},

};

#endif /* CINSEDIT_C */

#endif /* _CINSVDE_H */
