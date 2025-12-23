#include <WWFDefs.h>

/* <areanum>.hdr */

struct WWFMsgHdr {
    ULONG   MsgNum;
    UBYTE   Date[20];
    ULONG   ReplyTo;
    ULONG   NextReply;
    UWORD   OrigZone;
    UWORD   OrigNet;
    UWORD   OrigNode;
    UWORD   OrigPoint;
    UWORD   DestZone;
    UWORD   DestNet;
    UWORD   DestNode;
    UWORD   DestPoint;
    UWORD   Flags;
    ULONG   TextPos;    /* In Data.MSG */
    ULONG   TextLen;
    UBYTE   FromUser[40];
    UBYTE   ToUser[40];
    UBYTE   Subject[74];
};

#define WWFHDR_PRIVATE  1
#define WWFHDR_RECEIVED 2

/* Header.RWF */

struct WWFHdrRWF {
    ULONG   TextPos;    /* In Text.RWF */
    ULONG   TextLen;
    ULONG   OrigArea;
    ULONG   OrigNum;
    ULONG   Area;
    ULONG   Flags;
    UWORD   DestZone;
    UWORD   DestNet;
    UWORD   DestNode;
    UWORD   DestPoint;
    UBYTE   ToUser[40];
    UBYTE   Subject[74];
    UBYTE   AreaName[16];
    ULONG   Fill;
};

#define RWFHDR_PRIVATE  1

/* Info.BBS */

struct InfoBBS {
    ULONG   Version;    /* 0 */
    UBYTE   BBSName[80];
    UBYTE   SysOpName[40];
    ULONG   Flags;
    UBYTE   UserName[40];
    UBYTE   UserAlias[40];
    UBYTE   UserAddress[30];
    UBYTE   UserPostalCode[10];     /* Originally "PortalCode", probably a mistake */
    UBYTE   UserCity[30];
    UBYTE   UserCountry[30];
    UBYTE   UserPhone[20];
    UBYTE   BBSID[80];
};

#define WWFINFO_ONLYISO 1

/* Personal.HDR */

struct WWFPersonal {
    ULONG   AreaNum;
    ULONG   MsgNum;
    UBYTE   Fill[12];
};

/* Area.IWF */

struct AreaIWF {
    ULONG   AreaNum;
    UWORD   Flags;
    UBYTE   AreaName[34];
};

#define WWFAREA_ECHO    1
#define WWFAREA_NETMAIL 2
#define WWFAREA_WRITE   4
#define WWFAREA_FEEDB   8
#define WWFAREA_REPLY   16

