#include <WWFProto.h>

#define VERSION "0.90"

struct CharSet {
    UBYTE *CnvFrom;
    UBYTE *CnvTo;
    UBYTE *Name;
    UBYTE *Extension;
};

struct CfgItem {
    UBYTE *id;
    ULONG *var;
    LONG  val;
};

struct BBSMsg {
    struct Message Msg;
    int SendingNode;
    UWORD ID;
    void *result;
    int Ret;
    void *data;
};

#define ID_BEGINMSGH    11
#define ID_ENDMSGH      12
#define ID_LOADMSG      13
#define ID_SAVEMSG      14
#define ID_UPDATEMSG    15
#define ID_GETMSGAREA   16
#define ID_GETFILEROOT  17
#define ID_UPDATEFILES  18
#define ID_LOADUSER     19
#define ID_SAVEUSER     20
#define ID_GETUSERPATH  21
#define ID_KILLUSER     22
#define ID_GETINDEX     23
#define ID_GETUSERNUM   26
#define ID_FREEDVEC     27
#define ID_INCOPENCOUNT 28
#define ID_DECOPENCOUNT 29
#define ID_GETNUMAREAS  30
#define ID_GETNUMNODES  31
#define ID_GETUSERNAME  37

struct LoadedMsg {
    struct FMsg *msg;
    ULONG Length;
};

struct FAreaCfg {
    UBYTE *Name;
    int MinAcc;
    int UpAcc;
    int DownAcc;
    int Mask;
    int UpMask;
    int DownMask;
    struct MinList *FreeDL;
    struct MinList *NoUpload;
    UBYTE *Redirect;
};

struct DirNode {
    struct Node Node;
    UBYTE *DirName;
    struct MinList *flist;
    struct MinList *dlist;
    struct DirNode *Parent;
    struct FAreaCfg *fc;
};

struct AreaCfg {
    int AreaNr;
    UBYTE *Name;
    int Type;
    ULONG LowMsg;
    ULONG HiMsg;
    int MsgFile;
    int MinAcc;
    int ReadAcc;
    int WriteAcc;
    int RPAcc;
    int Mask;
    int ReadMask;
    int WriteMask;
    int RPMask;
    UBYTE *EchoTag;
    UBYTE *Origin;
    ULONG HiWater;
    int NoImport;
    struct MinList *ExportTo;
    struct MinList *Misc;
    struct FidoNode *AKA;
};

struct MsgId {
    UWORD Zone;
    UWORD Net;
    UWORD Node;
    UWORD Point;
    ULONG ID;
};

struct IndexEntry {
    ULONG MsgNum;
    UWORD FileNum;
    UWORD OffSet;
    ULONG MsgLen;
    struct MsgId MSGID;
    struct MsgId REPLY;
};

#define MSG_PRIVATE     0x0001
#define MSG_LOCAL       0x0002
#define MSG_RECEIVED    0x0004
#define MSG_SENT        0x0008
#define MSG_FILEATT     0x0010
#define MSG_KILLSENT    0x0020
#define MSG_DEAD        0x0040

struct MemMsg {
    ULONG MsgNum;
    ULONG Original;
    ULONG FirstReply;
    ULONG PrevReply;
    ULONG NextReply;
    UWORD Bits;
    UWORD OrigZone;
    UWORD OrigNet;
    UWORD OrigNode;
    UWORD OrigPoint;
    UWORD DestZone;
    UWORD DestNet;
    UWORD DestNode;
    UWORD DestPoint;
    UBYTE DateTime[20];
    UBYTE *AttFile;
    UBYTE *FromUser;
    UBYTE *ToUser;
    UBYTE *Subject;
    UBYTE *MsgText;
};

struct FMsg {
    ULONG MsgNum;
    ULONG Original;
    ULONG FirstReply;
    ULONG PrevReply;
    ULONG NextReply;
    UWORD Bits;
    UWORD OrigZone;
    UWORD OrigNet;
    UWORD OrigNode;
    UWORD OrigPoint;
    UWORD DestZone;
    UWORD DestNet;
    UWORD DestNode;
    UWORD DestPoint;
    UBYTE DateTime[20];
};

struct FileNode {
    struct Node Node;
    UBYTE *Name;
    UBYTE *Comment;
    ULONG Size;
    ULONG Prot;
    ULONG Days;
    ULONG Mins;
    ULONG Ticks;
    unsigned Free : 1;
};

struct MsgReq {
    int Area;
    ULONG MsgNum;
    struct FMsg *bmsg;
    ULONG MsgLen;
    UBYTE *MText;
    UBYTE *ToUser;
};

struct UserVal {
    UWORD Lines;
    WORD Access;
    LONG BytesLeft;
    LONG FilesLeft;
    ULONG BytesDown;
    ULONG FilesDown;
    ULONG BytesUp;
    ULONG FilesUp;
    UWORD ByteRatio;
    UWORD FileRatio;
    ULONG Messages;
    WORD TimeDay;
    WORD TimeCall;
    WORD TimeLeft;
    WORD CallsDay;
    WORD CallsLeft;
    WORD TimeBank;
    WORD MaxTimeBank;
    ULONG AvgCps;
    ULONG AvgCpsMul;
    ULONG Mask;
    ULONG TotalCalls;
    ULONG LastCall;
    unsigned StripCols : 1;
    unsigned SlowTerm : 1;
    unsigned SimpleTerm : 1;
    unsigned StripAll : 1;
};

struct BBSUser {
    UBYTE *Name;
    UBYTE *Password;
    UBYTE *CharSet;
    UBYTE *Extension;
    UBYTE *Locale;
    UBYTE *Protocol;
    ULONG *HighRead;
    UBYTE *AMode;
    struct MinList Misc;
    int Number;
    struct UserVal val;
};

struct UserReq {
    struct BBSUser *user;
    UBYTE *name;
};
