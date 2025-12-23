/* Structures and command IDs collected from TechPrg.guide */

/* 
 * Some typedefs so that we can avoid having to use the exec/types.h
 * typenames
 */

typedef unsigned long ulong;
typedef unsigned short uword;
typedef unsigned char ubyte;

typedef void *obj;      /* This is more useful in some other programs */

struct BBSMsg {
    struct Message Msg; /* An exec message */
    int SendingNode;    /* The number of the node sending the message or
                           -1 if sent by a utility */
    uword ID;           /* The ID number of the command this message is
                           supposed to cause TechCon to perform */
    obj result;         /* The result field pointing to whatever result
                           data is returned by the call (if any) */
    int Ret;            /* Used by some calls for all kinds of stuff
                           (used to be a result field) */
    obj data;           /* Additional data for the command, whenever the
                           command requires any */
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
#define ID_GETSIGS      43
#define ID_SETLOWMSG    44
#define ID_DELMSGFILE   45
#define ID_LOCKFILEROOT 46 /* TechCon 0.91+ */
#define ID_FREEFILEROOT 47 /* TechCon 0.91+ */
#define ID_MOVEFILE     48 /* TechCon 0.92+ */

struct SigDat {         /* SIG configuration structure */
    struct MinNode Node;
    int SigNum;
    char *Name;
    int MinAcc;
    int Mask;
    struct MinList Areas; /* List of nodes with ln_Name fields pointing to
                             a struct AreaCfg */
};

struct FAreaCfg {       /* File area configuration structure */
    ubyte *Name;
    int MinAcc;
    int UpAcc;
    int DownAcc;
    int Mask;
    int UpMask;
    int DownMask;
    struct MinList *FreeDL;     /* List of nodes with ln_Name fields
                                   pointing to a parsed pattern */
    struct MinList *NoUpload;   /* List of nodes with ln_Name fields
                                   pointing to a parsed pattern */
    ubyte *Redirect;
    struct MinList *Misc;       /* TechCon 0.91+ */
};

struct FidoNode {       /* FidoNet address structure */
    struct Node nd;
    int Zone;
    int Net;
    int Node;
    int Point;
};

struct AreaCfg {        /* Message area configuration */
    int AreaNr;
    ubyte *Name;
    int Type;
    ulong LowMsg;
    ulong HiMsg;
    int MsgFile;
    int MinAcc;
    int ReadAcc;
    int WriteAcc;
    int RPAcc;
    int Mask;
    int ReadMask;
    int WriteMask;
    int RPMask;
    ubyte *EchoTag;
    ubyte *Origin;
    ulong HiWater;
    int NoImport;
    struct MinList *ExportTo;   /* List of struct FidoNodes */
    struct MinList *Misc;       /* List of nodes w/ ln_Name pointing to a
                                   string */
    struct FidoNode *AKA;
    int nmdt;   /* PRIVATE - don't touch */
    long mdt;   /* PRIVATE - don't touch */
    long idx;   /* PRIVATE - don't touch */
    int mod;   /* Set this if you change anything during a message handling
                  session */
};

struct BBSMsg {         /* See info on TechCon private port for comments in
                           fields */
    struct Message Msg;
    int SendingNode;
    uword ID;
    void *result;
    int Ret;
    void *data;
    int Node;
};

struct LoadedMsg {      /* This is what LoadMsg() returns (for some reason) */
    struct FMsg *msg;
    ulong Length;
};

struct MsgReq {         /* Used for lots of stuff */
    int Area;
    ulong MsgNum;
    struct FMsg *bmsg;
    ulong MsgLen;
    ubyte *MText;
    ubyte *ToUser;
};

struct FileMoveDat {
    struct DirNode *sdir, *ddir;
    struct FileNode *fncopy;
};

struct MsgId {          /* A binary parsed version of a MSGID: or REPLY:
                           field */
    uword Zone;
    uword Net;
    uword Node;
    uword Point;
    ulong ID;
};

struct IndexEntry {     /* Message index */
    ulong MsgNum;
    uword FileNum;
    uword OffSet;
    ulong MsgLen;
    struct MsgId MSGID;
    struct MsgId REPLY;
};

#define MSG_PRIVATE     0x0001
#define MSG_LOCAL       0x0002 /* Stupid, I don't remember what this is for
                                  (ignore it) */
#define MSG_RECEIVED    0x0004
#define MSG_SENT        0x0008 /* Has been sent (FidoNet) */
#define MSG_FILEATT     0x0010 /* Has a file attached */
#define MSG_KILLSENT    0x0020 /* On local, kill on receive (not useful) */
#define MSG_DEAD        0x0040 /* A dead message - Loading will fail */

struct MemMsg {         /* Recommended format for message storage in memory */
    ulong MsgNum;
    ulong Original;
    ulong FirstReply;
    ulong PrevReply;
    ulong NextReply;
    uword Bits;
    uword OrigZone;
    uword OrigNet;
    uword OrigNode;
    uword OrigPoint;
    uword DestZone;
    uword DestNet;
    uword DestNode;
    uword DestPoint;
    ubyte DateTime[20];
    ubyte *AttFile;
    ubyte *FromUser;
    ubyte *ToUser;
    ubyte *Subject;
    ubyte *MsgText;
};

struct FMsg {           /* Message format as found in .MDT files */
    ulong MsgNum;
    ulong Original;
    ulong FirstReply;
    ulong PrevReply;
    ulong NextReply;
    uword Bits;
    uword OrigZone;
    uword OrigNet;
    uword OrigNode;
    uword OrigPoint;
    uword DestZone;
    uword DestNet;
    uword DestNode;
    uword DestPoint;
    ubyte DateTime[20];
        /* Null terminated attached file name (full, w/path) */
        /* Null terminated from */
        /* Null terminated to */
        /* Null terminated subject */
        /* Null terminated message text */
};

struct FileNode {       /* File structure */
    struct Node Node;
    ubyte *Name;
    ubyte *Comment;
    ulong Size;
    ulong Prot;
    ulong Days;
    ulong Mins;
    ulong Ticks;
    ubyte *Uploader;
    uword LongDesc;
    ubyte **DescLines;
    uword Number;
    unsigned Free : 1;
};

struct DirNode {        /* Directory structure */
    struct Node Node;
    ubyte *DirName;
    struct MinList *flist; /* List of struct FileNodes */
    struct MinList *dlist; /* List of struct DirNodes */
    struct DirNode *Parent; /* This node can be found in Parent->dlist */
    struct FAreaCfg *fc;
    unsigned HasExt : 1;
};

struct UserVal {        /* This is actually in the file like this */
    uword Lines;
    short Access;
    long BytesLeft;
    long FilesLeft;
    ulong BytesDown;
    ulong FilesDown;
    ulong BytesUp;
    ulong FilesUp;
    uword ByteRatio;
    uword FileRatio;
    ulong Messages;
    short TimeDay;
    short TimeCall;
    short TimeLeft;
    short CallsDay;
    short CallsLeft;
    short TimeBank;
    short MaxTimeBank;
    ulong AvgCps;
    ulong AvgCpsMul;
    ulong Mask;
    ulong TotalCalls;
    ulong LastCall;
    unsigned StripCols : 1;
    unsigned SlowTerm : 1;
    unsigned SimpleTerm : 1;
    unsigned StripAll : 1;
};

struct BBSUser {        /* User structure */
    ubyte *Name;
    ubyte *Password;
    ubyte *CharSet;
    ubyte *Extension;
    ubyte *Locale;      /* Used to be a locale, now a string file
                           (determines language, anyhow) */
    ubyte *Protocol;
    ulong *HighRead;    /* Array of highread pointers */
    ubyte *AMode;       /* Array of 'S' or 'N' */
    struct MinList Misc; /* A list of nodes with ln_Name ptr to string
                            containing misc definition for user */
    int Number;
    struct UserVal val;
};
