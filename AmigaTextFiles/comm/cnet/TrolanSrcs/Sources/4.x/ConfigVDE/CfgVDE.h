/*
** VDE definitions for CNet/NewConfig1
** Copyright 1996-8
** by Kelly Cochran
** Last Updated: Wed Jan 28 21:54:32 1998
*/

#define VDE_SIZE(array)  (sizeof(array)/sizeof(struct VDEentry))

struct VDEentry CfgVDE[] = {

   /* Screen 10 - Exit routines */

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

   /* Screen 0 - Main menu */

   {"<< Exit           ",1,4,    /* 2 */
     0,0,0,
     -1,0,
     0,-10,
     12,3,2,2,0},

   {"System Name   :",1,6,        /* 3 */
     0,0,26,
     2,offsetof(struct NewConfig1, MySystemName),
     0,0,
     2,4,3,3,0},

   {"Operator Name :",1,7,        /* 4 */
     0,0,26,
     2,offsetof(struct NewConfig1, MySysopName),
     0,0,
     3,5,4,4,0},

   {"Location      :",1,8,        /* 5 */
     0,0,26,
     2,offsetof(struct NewConfig1, MyLocation),
     0,0,
     4,6,5,5,0},

   {"Phone Number  :",1,9,        /* 6 */
     0,0,20,
     2,offsetof(struct NewConfig1, MyPhoneNumber),
     0,0,
     5,7,6,6,0},

   {"System ID     :",1,10,       /* 7 */
     0,0,10,
     2,offsetof(struct NewConfig1, MyBBSID),
     0,0,
     6,8,7,7,0},

   {"INet Hostname :",1,11,       /* 8 */
     0,0,44,
     2,offsetof(struct NewConfig1, MyUUCPName),
     0,0,
     7,9,8,8,0},

   {"Display      >>",1,13,        /* 9 */
     0,0,0,
     -1,0,
     0,1,
     8,10,21,13,0},

   {"Limits       >>",1,14,        /* 10 */
     0,0,0,
     -1,0,
     0,2,
     9,11,22,14,0},

   {"Defaults     >>",1,15,       /* 11 */
     0,0,0,
     -1,0,
     0,3,
     10,12,23,15,0},

   {"Options      >>",1,16,       /* 12 */
     0,0,0,
     -1,0,
     0,4,
     11,2,24,16,0},

   {"Paths        >>",21,13,          /* 13 */
     0,0,0,
     -1,0,
     0,5,
     16,14,9,17,0},

   {"BBS Macros   >>",21,14,       /* 14 */
     0,0,0,
     -1,0,
     0,6,
     13,15,10,18,0},

   {"Term Macros  >>",21,15,       /* 15 */
     0,0,0,
     -1,0,
     0,7,
     14,16,11,19,0},

   {"Accounting   >>",21,16,    /* 16 */
     0,0,0,
     -1,0,
     0,0,
     15,13,12,20,(1>>0)},

   {"Archivers    >>",41,13,   /* 17 */
     0,0,0,
     -1,0,
     0,0,
     20,18,13,21,(1>>0)},

   {"Editors      >>",41,14,   /* 18 */
     0,0,0,
     -1,0,
     0,0,
     17,19,14,22,(1>>0)},

   {"Protocols    >>",41,15,      /* 19 */
     0,0,0,
     -1,0,
     0,0,
     18,20,15,23,(1>>0)},

   {"Expansion    >>",41,16,      /* 20 */
     0,0,0,
     -1,0,
     0,0,
     19,17,16,24,(1>>0)},

   {"Modems       >>",61,13,      /* 21 */
     0,0,0,
     -1,0,
     0,0,
     24,22,17,9,(1>>0)},

   {"Fido-Net     >>",61,14,      /* 22 */
     0,0,0,
     -1,0,
     0,0,
     21,23,18,10,(1>>0)},

   {"Logs         >>",61,15,      /* 23 */
     0,0,0,
     -1,0,
     0,0,
     22,24,19,11,(1>>0)},

   {"Tosser       >>",61,16,      /* 24 */
     0,0,0,
     -1,0,
     0,8,
     23,21,20,12,0},

   /* Screen 1 - Display */

   {"<< Previous screen   ",1,5,    /* 25 */
    0,0,0,
    -1,0,
    1,0,
    26,27,25,25,0},

   {"<< Exit              ",1,4,    /* 26 */
    0,0,0,
    -1,0,
    1,-10,
    29,25,26,26,0},

   {"Font                :",1,7,     /* 27 */
    0,0,40,
    2,offsetof(struct NewConfig1, FontName),
    1,1,
    25,28,27,27,0},

   {"Screen dim pause    :",1,8,     /* 28 */
    0,600,4,
    6,offsetof(struct NewConfig1, BlankTicks),
    1,1,
    27,29,28,28,0},

   {"Screen dim intensity:",1,9,     /* 29 */
    0,600,4,
    6,offsetof(struct NewConfig1, BlankBright),
    1,1,
    28,30,29,29,0},

   {"Expansion 1          ",1,10,   /* 30 */
    0,0,0,
    0,0,
    1,1,
    29,31,30,30,(1>>0)},

   {"Expansion 2          ",1,11,   /* 31 */
    0,0,0,
    0,0,
    1,1,
    30,32,31,31,(1>>0)},

   {"Expansion 3          ",1,12,   /* 32 */
    0,0,0,
    0,0,
    1,1,
    31,26,32,32,(1>>0)},

   /* Screen 2 - Limits */

   {"<< Previous screen   ",1,5,    /* 33 */
    0,0,0,
    -1,0,
    2,0,
    34,35,33,33,0},

   {"<< Exit              ",1,4,    /* 34 */
    0,0,0,
    -1,0,
    2,-10,
    29,33,34,34,0},

   {"Highest BaseX Part. :",1,7,    /* 35 */
    0,32,3,
    6,offsetof(struct NewConfig1, nUDBASEx),
    2,2,
    33,36,44,44,0},

   {"Short Desc Lines    :",1,8,    /* 36 */
    2,16,3,
    6,offsetof(struct NewConfig1, MaxShortLines),
    2,2,
    35,37,45,45,0},

   {"Max. Doors Running  :",1,9,    /* 37 */
    0,100,4,
    6,offsetof(struct NewConfig1, MaxOpenPfiles),
    2,2,
    36,38,46,46,0},

   {"Max. User Accounts  :",1,10,   /* 38 */
    0,9999,5,
    6,offsetof(struct NewConfig1, maxUserAccounts),
    2,2,
    37,39,47,47,0},

   {"Max. Conference Room:",1,11,   /* 39 */
    0,99,3,
    6,offsetof(struct NewConfig1, NumRooms),
    2,2,
    38,40,48,48,0},

   {"High Network JL ID# :",1,12,   /* 40 */
    0,99,3,
    6,offsetof(struct NewConfig1, nLinkPorts),
    2,2,
    39,41,49,49,0},

   {"Max. UD/Base Subs   :",1,13,   /* 41 */
    0,99999,6,
    6,offsetof(struct NewConfig1, nsub),
    2,2,
    40,42,50,50,0},

   {"Max. Selected Files :",1,14,   /* 42 */
    0,999,4,
    6,offsetof(struct NewConfig1, nselect),
    2,2,
    41,43,51,51,0},

   {"Max. Uploads/batch  :",1,15,   /* 43 */
    0,999,4,
    6,offsetof(struct NewConfig1, nupload),
    2,2,
    42,34,52,52,0},

   {"Max. G/PFiles/list  :",40,7,   /* 44 */
    0,999,4,
    6,offsetof(struct NewConfig1, nlist),
    2,2,
    52,45,35,35,0},

   {"Max. Yank-tasks     :",40,8,   /* 45 */
    0,999,4,
    6,offsetof(struct NewConfig1, maxYankTasks),
    2,2,
    44,46,36,36,0},

   {"Max. Yank Size (K)  :",40,9,   /* 46 */
    0,99999,6,
    6,offsetof(struct NewConfig1, maxYankSize),
    2,2,
    45,47,37,37,0},

   {"Days to hold Yanks  :",40,10,  /* 47 */
    0,9999,5,
    6,offsetof(struct NewConfig1, maxYankDays),
    2,2,
    46,48,38,38,0},

   {"Max. logon attempts :",40,11,  /* 48 */
    0,999,4,
    6,offsetof(struct NewConfig1, maxLogonAttempts),
    2,2,
    47,49,39,39,0},

   {"Login Time (1/10min):",40,12,  /* 49 */
    0,999,4,
    6,offsetof(struct NewConfig1, maxLogonTime),
    2,2,
    48,50,40,40,0},

   {"Max. Yank files/user:",40,13,  /* 50 */
    0,999,4,
    6,offsetof(struct NewConfig1, maxYanksPerUser),
    2,2,
    49,51,41,41,0},

   {"BBSList auto-purge  :",40,14,  /* 51 */
    0,9999,5,
    6,offsetof(struct NewConfig1, BListPurgeDays),
    2,2,
    50,52,42,42,0},

   {"Output Buffer(bytes):",40,15,  /* 52 */
    256,9999,5,
    4,offsetof(struct NewConfig1, ABufferSize),
    2,2,
    51,44,43,43,0},

   /* Screen 3 - Defaults */

   {"<< Previous screen   ",1,5,    /* 53 */
    0,0,0,
    -1,0,
    3,0,
    54,55,53,53,0},

   {"<< Exit              ",1,4,    /* 54 */
    0,0,0,
    -1,0,
    3,-10,
    146,53,54,54,0},

   {"Default Country     :",1,7,    /* 55 */
    0,0,4,
    2,offsetof(struct NewConfig1, MyCountry),
    3,3,
    53,56,62,62,0},

   {"Default Area Code   :",1,8,    /* 56 */
    0,0,11,
    2,offsetof(struct NewConfig1, MyAreaCode),
    3,3,
    55,57,63,63,0},

   {"Default Protocol    :",1,9,    /* 57 */
    0,0,2,
    2,offsetof(struct NewConfig1, DefDefProtocol),
    3,3,
    56,58,64,64,0},

   {"Yank Prefix         :",1,10,   /* 58 */
    0,0,3,
    2,offsetof(struct NewConfig1, Yprefix),
    3,3,
    57,59,65,65,0},

   {"Def. game points    :",1,11,   /* 59 */
    0,9999999,8,
    6,offsetof(struct NewConfig1, DefDoorPoints),
    3,3,
    58,60,66,66,0},

   {"Def. balance (cents):",1,12,   /* 60 */
    0,9999999,8,
    6,offsetof(struct NewConfig1, DefBalance),
    3,3,
    59,61,67,67,0},

   {"Def. time format    :",1,13,   /* 61 */
    1181,2,14,
    9,offsetof(struct NewConfig1, DefTimeForm),
    3,3,
    60,68,61,61,0},

   {"Def. Net credits    :",40,7,   /* 62 */
    0,9999999,8,
    6,offsetof(struct NewConfig1, DefNetCredits),
    3,3,
    67,63,55,55,0},

   {"Def. Byte credits   :",40,8,   /* 63 */
    0,9999999,8,
    6,offsetof(struct NewConfig1, DefByteCredits),
    3,3,
    62,64,56,56,0},

   {"Def. File credits   :",40,9,   /* 64 */
    0,9999999,8,
    6,offsetof(struct NewConfig1, DefFileCredits),
    3,3,
    63,65,57,57,0},

   {"Def. JoinLink ID#   :",40,10,  /* 65 */
    0,99,3,
    3,offsetof(struct NewConfig1, MyLinkID),
    3,3,
    64,66,58,58,0},

   {"JoinLink password   :",40,11,  /* 66 */
    0,0,10,
    2,offsetof(struct NewConfig1, MyLinkPass),
    3,3,
    65,67,59,59,0},

   {"Def. Mail Sorting   :",40,12,  /* 67 */
    2251,4,15,
    9,offsetof(struct NewConfig1, DefMailSort),
    3,3,
    66,62,60,60,0},

   {"NNTP Server         :",1,14,  /* 68 */
    0,0,40,
    2,offsetof(struct NewConfig1, NewsServer),
    3,3,
    61,146,68,68,0},

   /* Screen 4 - Options */

   {"<< Previous screen   ",1,5,    /* 69 */
    0,0,0,
    -1,0,
    4,0,
    70,71,69,69,0},

   {"<< Exit              ",1,4,    /* 70 */
    0,0,0,
    -1,0,
    4,-10,
    89,69,70,70,0},

   {"Allow feedback@logon:",1,7,    /* 71 */
    0,0,0,
    0,offsetof(struct NewConfig1, LogonFeedback),
    4,4,
    69,72,79,79,0},

   {"Allow Who/UL @logon :",1,8,    /* 72 */
    0,0,0,
    0,offsetof(struct NewConfig1, LogonSearch),
    4,4,
    71,73,80,80,0},

   {"Allow guest users   :",1,9,    /* 73 */
    0,0,0,
    0,offsetof(struct NewConfig1, GuestUsers),
    4,4,
    72,74,81,81,0},

   {"Status Win off@logon:",1,10,   /* 74 */
    0,0,0,
    0,offsetof(struct NewConfig1, HideStatus),
    4,4,
    73,75,82,82,0},

   {"Keep user conf.prefs:",1,11,   /* 75 */
    0,0,0,
    0,offsetof(struct NewConfig1, ConfProfile),
    4,4,
    74,76,83,83,0},

   {"Feedback to user #1 :",1,12,   /* 76 */
    0,0,0,
    0,offsetof(struct NewConfig1, ConfProfile),
    4,4,
    75,77,84,84,0},

   {"Indent mess. 1 space:",1,13,   /* 77 */
    0,0,0,
    0,offsetof(struct NewConfig1, IndentSpaces),
    4,4,
    76,78,85,85,0},

   {"Mail ID from handle?:",1,14,   /* 78 */
    0,0,0,
    0,offsetof(struct NewConfig1, MIDFromHandle),
    4,4,
    77,87,86,86,0},

   {"Validate with CID   :",40,7,   /* 79 */
    0,0,0,
    0,offsetof(struct NewConfig1, CIDvalidate),
    4,4,
    92,80,71,71,0},

   {"Skip handle with CID:",40,8,   /* 80 */
    0,0,0,
    0,offsetof(struct NewConfig1, CIDhandleskip),
    4,4,
    79,81,72,72,0},

   {"Don't answer w/o CID:",40,9,   /* 81 */
    0,0,0,
    0,offsetof(struct NewConfig1, CIDsilence),
    4,4,
    80,82,73,73,0},

   {"Skip idle on Who    :",40,10,  /* 82 */
    0,0,0,
    0,offsetof(struct NewConfig1, SkipIdlePorts),
    4,4,
    81,83,74,74,0},

   {"Del Usenet msg@purge:",40,11,  /* 83 */
    0,0,0,
    0,offsetof(struct NewConfig1, DeleteUUCP),
    4,4,
    82,84,75,75,0},

   {"File-task notify ok?:",40,12,   /* 84 */
    0,0,0,
    0,offsetof(struct NewConfig1, FileTaskNotify),
    4,4,
    83,85,76,76,0},

   {"Monitor UUMail?     :",40,13,   /* 85 */
    0,0,0,
    0,offsetof(struct NewConfig1, MonitorUUMail),
    4,4,
    84,86,77,77,0},

   {"Create Web Dir?     :",40,14,   /* 86 */
    0,0,0,
    0,offsetof(struct NewConfig1, CreateWebDir),
    4,4,
    85,90,78,78,0},

   {"News-Task Post?     :",1,15,   /* 87 */
    0,0,0,
    0,offsetof(struct NewConfig1, NewsTaskPost),
    4,4,
    78,88,90,90,0},

   {"Dynamic IP?         :",1,16,   /* 88 */
    0,0,0,
    0,offsetof(struct NewConfig1, dynamicIP),
    4,4,
    87,89,91,91,0},

   {"Expansion 3         :",1,17,   /* 89 */
    0,0,0,
    0,0,
    4,4,
    88,70,92,92,(1>>0)},

   {"Expansion 4         :",40,15,   /* 90 */
    0,0,0,
    0,0,
    4,4,
    86,91,87,87,(1>>0)},

   {"Expansion 5         :",40,16,   /* 91 */
    0,0,0,
    0,0,
    4,4,
    90,92,88,88,(1>>0)},

   {"Expansion 6         :",40,17,   /* 92 */
    0,0,0,
    0,0,
    4,4,
    91,79,89,89,(1>>0)},

   /* Screen 5 - Paths */

   {"<< Previous screen   ",1,5,    /* 93 */
    0,0,0,
    -1,0,
    5,0,
    94,95,93,93,0},

   {"<< Exit              ",1,4,    /* 94 */
    0,0,0,
    -1,0,
    5,-10,
    108,93,94,94,0},

   {"OLM Temp            :",1,7,    /* 95 */
    0,0,40,
    2,offsetof(struct NewConfig1, OLMpath),
    5,5,
    93,96,95,95,0},

   {"ARC examine dir     :",1,8,    /* 96 */
    0,0,40,
    2,offsetof(struct NewConfig1, EXTRACTpath),
    5,5,
    95,97,96,96,0},

   {"Yank/QWK Packing    :",1,9,    /* 97 */
    0,0,40,
    2,offsetof(struct NewConfig1, YANKwork),
    5,5,
    96,98,97,97,0},

   {"Editor temp files   :",1,10,   /* 98 */
    0,0,40,
    2,offsetof(struct NewConfig1, ZIPpath),
    5,5,
    97,99,98,98,0},

   {"Optional VDE        :",1,11,   /* 99 */
    0,0,40,
    2,offsetof(struct NewConfig1, RAMpath),
    5,5,
    98,100,99,99,0},

   {"Def. Term U/D       :",1,12,   /* 100 */
    0,0,40,
    2,offsetof(struct NewConfig1, TERMpath),
    5,5,
    99,101,100,100,0},

   {"CD-ROM U/D temp     :",1,13,   /* 101 */
    0,0,40,
    2,offsetof(struct NewConfig1, CDROMpath),
    5,5,
    100,102,101,101,0},

   {"Spelling Dictionary :",1,14,   /* 102 */
    0,0,40,
    2,offsetof(struct NewConfig1, DictPath),
    5,5,
    101,103,102,102,0},

   {"Fido Outbound       :",1,15,   /* 103 */
    0,0,40,
    2,offsetof(struct NewConfig1, OutboundPath),
    5,5,
    102,104,103,103,0},

   {"Fido Inbound        :",1,16,   /* 104 */
    0,0,40,
    2,offsetof(struct NewConfig1, InboundPath),
    5,5,
    103,105,104,104,0},

   {"Fido Nodelist       :",1,17,   /* 105 */
    0,0,40,
    2,offsetof(struct NewConfig1, Nodelist),
    5,5,
    104,106,105,105,0},

   {"Internet Mail Path  :",1,18,   /* 106 */
    0,0,40,
    2,offsetof(struct NewConfig1, UUMailPath),
    5,5,
    105,107,106,106,0},

   {"News Path           :",1,19,   /* 107 */
    0,0,40,
    2,offsetof(struct NewConfig1, NewsPath),
    5,5,
    106,108,107,107,0},

   {"Expansion 3         :",1,20,   /* 108 */
    0,0,40,
    2,0,
    5,5,
    107,94,108,108,(1<<0)},

   /* Screen 6 - BBS Macros */

   {"<< Previous screen   ",1,5,    /* 109 */
    0,0,0,
    -1,0,
    6,0,
    110,111,109,109,0},

   {"<< Exit              ",1,4,    /* 110 */
    0,0,0,
    -1,0,
    6,-10,
    120,109,110,110,0},

   {"Function key 1 :",1,7,    /* 111 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[0]),
    6,6,
    109,112,111,111,0},

   {"Function key 2 :",1,8,    /* 112 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[1]),
    6,6,
    111,113,112,112,0},

   {"Function key 3 :",1,9,   /* 113 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[2]),
    6,6,
    112,114,113,113,0},

   {"Function key 4 :",1,10,   /* 114 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[3]),
    6,6,
    113,115,114,114,0},

   {"Function key 5 :",1,11,   /* 115 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[4]),
    6,6,
    114,116,115,115,0},

   {"Function key 6 :",1,12,   /* 116 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[5]),
    6,6,
    115,117,116,116,0},

   {"Function key 7 :",1,13,   /* 117 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[6]),
    6,6,
    116,118,117,117,0},

   {"Function key 8 :",1,14,   /* 118 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[7]),
    6,6,
    117,119,118,118,0},

   {"Function key 9 :",1,15,   /* 119 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[8]),
    6,6,
    118,120,119,119,0},

   {"Function key 10:",1,16,  /* 120 */
    0,0,60,
    2,offsetof(struct NewConfig1, FKey[9]),
    6,6,
    119,110,120,120,0},
    
   /* Screen 7 - Term Macros */

   {"<< Previous screen   ",1,5,    /* 121 */
    0,0,0,
    -1,0,
    7,0,
    122,133,121,121,0},

   {"<< Exit              ",1,4,    /* 122 */
    0,0,0,
    -1,0,
    7,-10,
    132,121,122,122,0},

   {"Function key 1 :",1,7,    /* 123 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[0]),
    7,7,
    122,124,123,123,0},

   {"Function key 2 :",1,8,    /* 124 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[1]),
    7,7,
    123,125,124,124,0},

   {"Function key 3 :",1,9,   /* 125 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[2]),
    7,7,
    124,126,125,125,0},

   {"Function key 4 :",1,10,   /* 126 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[3]),
    7,7,
    125,127,126,126,0},

   {"Function key 5 :",1,11,   /* 127 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[4]),
    7,7,
    126,128,127,127,0},

   {"Function key 6 :",1,12,   /* 128 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[5]),
    7,7,
    127,129,128,128,0},

   {"Function key 7 :",1,13,   /* 129 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[6]),
    7,7,
    128,130,129,129,0},

   {"Function key 8 :",1,14,   /* 130 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[7]),
    7,7,
    129,131,130,130,0},

   {"Function key 9 :",1,15,   /* 131 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[8]),
    7,7,
    130,132,131,131,0},

   {"Function key 10:",1,16,  /* 132 */
    0,0,60,
    2,offsetof(struct NewConfig1, TFKey[9]),
    7,7,
    131,122,132,132,0},
    
   /* Screen 8 - Tosser */

   {"<< Previous screen   ",1,5,    /* 133 */
    0,0,0,
    -1,0,
    8,0,
    134,135,133,133,0},

   {"<< Exit              ",1,4,    /* 134 */
    0,0,0,
    -1,0,
    8,-10,
    145,133,134,134,0},

   {"Input buffer size   :",1,7,    /* 135 */
    0,9999999,8,
    6,offsetof(struct NewConfig1, TossInputSize),
    8,8,
    134,136,135,135,0},

   {"Output buffer size  :",1,8,    /* 136 */
    0,9999999,8,
    6,offsetof(struct NewConfig1, TossOutputSize),
    8,8,
    135,137,136,136,0},

   {"Def. Dupe table size:",1,9,    /* 137 */
    0,9999,5,
    5,offsetof(struct NewConfig1, DupeTableSize),
    8,8,
    136,138,137,137,0},

   {"Unknown echo to bad :",1,10,   /* 138 */
    0,0,0,
    0,offsetof(struct NewConfig1, BadUnknown),
    8,8,
    137,139,138,138,0},

   {"TD 1.74+ Front-end  :",1,11,   /* 139 */
    0,0,0,
    0,offsetof(struct NewConfig1, Trapdoor174),
    8,8,
    138,140,139,139,0},

   {"Dupe echomail to bad:",1,12,   /* 140 */
    0,0,0,
    0,offsetof(struct NewConfig1, BadDupes),
    8,8,
    139,141,140,140,0},

   {"Show kludges        :",1,13,   /* 141 */
    0,0,0,
    0,offsetof(struct NewConfig1, ShowKludges),
    8,8,
    140,142,141,141,0},

   {"Use 3D w/o point-net:",1,14,   /* 142 */
    0,0,0,
    0,offsetof(struct NewConfig1, ThreeD),
    8,8,
    141,144,142,142,0},

   {"X/IFido start 2.msg :",1,15,   /* 143 */
    0,0,0,
    0,offsetof(struct NewConfig1, TwoDotMsg),
    8,8,
    142,144,143,143,0},

   {"Expansion 1         :",1,16,   /* 144 */
    0,0,0,
    0,0,
    8,8,
    143,145,144,144,(1>>0)},

   {"Expansion 2         :",1,17,   /* 145 */
    0,0,0,
    0,0x00,
    8,8,
    144,134,145,145,(1>>0)},

   /* Addendum area */

   {"Def. Root Identd    :",1,15,    /* 146 */
    0,0,8,
    2,offsetof(struct NewConfig1, RootName),
    3,3,
    68,54,146,146,0},
};
