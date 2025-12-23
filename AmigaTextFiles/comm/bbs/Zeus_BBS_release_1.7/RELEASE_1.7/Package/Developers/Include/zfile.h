	
#ifndef ZFILE_H
#define ZFILE_H

#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/semaphores.h>

#include <dos/dos.h>
#include <dos/datetime.h>

#include <zconfig.h>
#include <libraries/zmf.h>
#include <libraries/filebase.h>

#include <zheaders.h>

/* LoadFile -> flags *****************************************************/

#define LF_DESCRIPTION     0x0001
#define LF_LOADDELETED     0x0002
#define LF_ISFILENAME      0x0004   // ULONG number argument is actually a pointer to a null terminated filename

/**********************************************************************/
/*                 FullFile Structure (as returned by                 */
/*              library functions ScanFiles and LoadFile              */
/**********************************************************************/

struct         fullFile
{
	struct      file   *ff_file;
	UBYTE           *ff_text;
	int         ff_result;
} ;


/**********************************************************************/
/*                           File Structure                           */
/**********************************************************************/

#ifdef OLDSTUFF
struct         oldfile
{
	ULONG       Number;
	UBYTE       FileName[20];
	ULONG       DescPosition;
	ULONG       DescLength;
	ULONG       FileSize;
	ULONG       Section;
	time_t         UploadDate;
	time_t         LastDownloaded;
	UWORD       NumberOfDownloads;
	ULONG       Flags;
	char        UploadedBy[40];
	LONG        AlphaPosNext; /* -1 for no next */
	LONG        AlphaPosPrev; /* -1 for no prev */
} ;
#endif

struct   file
{
	ULONG       Number;
	UBYTE       FileName[32];
	ULONG       DescPosition;
	ULONG       DescLength;
	ULONG       FileSize;
	ULONG       Section;
	time_t         UploadDate;
	time_t         LastDownloaded;
	UWORD       NumberOfDownloads;
	ULONG       Flags;
	char        UploadedBy[40];
	LONG        AlphaPosNext; /* -1 for no next */
	LONG        AlphaPosPrev; /* -1 for no prev */
} ;

/**********************************************************************/
/*                      Flags in File Structure                       */
/**********************************************************************/

#define     FI_UNVALIDATED       (ULONG) 0x0001
#define     FI_FREEFILE       (ULONG) 0x0002
#define     FI_DELETED        (ULONG) 0x0004
#define     FI_OFFLINE        (ULONG) 0x0008

/* main File Index *******************************************************/

struct         oldfileindex
{
	UBYTE       FileName[20];
	ULONG       FileNumber;
	ULONG       HdrPosition;
	UWORD       FileArea;
	UWORD       Deleted;
} ;

/**********************************************************************/
/*                             File Index                             */
/**********************************************************************/

struct         fileindex
{
	UBYTE       FileName[32];
	ULONG       FileNameCRC;
	ULONG       HdrPosition;
	UWORD       FileArea;
	BOOL        Deleted;
} ;

/**********************************************************************/
/*                        External File Index                         */
/**********************************************************************/

struct         extfileindex
{
	ULONG       FileNumber;
	ULONG       Position;
} ;


/**********************************************************************/
/*                          File Area Flags                           */
/**********************************************************************/

#define     FA_DOWNLOADS         (ULONG) 0x0001
#define     FA_UPLOADS        (ULONG) 0x0002
#define     FA_VIEWING        (ULONG) 0x0004
#define     FA_FREEFILES         (ULONG) 0x0008
#define     FA_JOINUSERS         (ULONG) 0x0010
#define     FA_DOSNAMES       (ULONG) 0x0020
#define     FA_VALIDATION        (ULONG) 0x0040
#define     FA_COPYTOTEMP        (ULONG) 0x0080
#define     FA_MALEONLY       (ULONG) 0x0100
#define     FA_FEMALEONLY        (ULONG) 0x0200

/**********************************************************************/
/*                        File Area Structure                         */
/**********************************************************************/

#define     PATH_RW           (UBYTE) 0
#define     PATH_R            (UBYTE) 1

struct         filearea
{
	ULONG    number;
	UBYTE    name[64];
	UBYTE    path[103];
	UBYTE    rw;
	ULONG    unique;
	unsigned short low_access;
	unsigned short  high_access;
	int      mindlbaud;
	int      minulbaud;
	unsigned short  minage;
	ULONG    flags;
	ULONG    mainfbID;
} ;

/**********************************************************************/
/*                       General Configuration                        */
/**********************************************************************/

struct   GeneralFileCfg
{
	ULONG    BufferSize;
	BOOL     FileNotes;
	UWORD    MaxFileMarks;
	UBYTE    TempPath[108];
	UBYTE    UploadPath[108];
	ULONG    MinFree;             // min free space on any files path.
} ;

/**********************************************************************/
/*                      Configuration Structure                       */
/**********************************************************************/

struct      Config
{
	struct      SignalSemaphore  cfg_Semaphore;
	struct      List     *cfg_AreaList;
	struct      XprStruct   *cfg_Xpr;
	struct      GeneralFileCfg  cfg_General;
} ;

struct XprStruct
{
	struct      SignalSemaphore  cfg_Semaphore;
	struct      List     *cfg_XprList;
} ;

#ifdef OLD_FILE_STUFF

/* struct Internal *******************************************************/

struct      Internal
{
	struct   IntFiles *int_files;
	struct  IntAreaFiles    *int_areafiles;
} ;

struct      IntFiles    /* Struct FileNode */
{
	struct   SignalSemaphore    if_FileSemaphore;
	struct   MinList     *if_Files;
} ;

struct      IntAreaFiles      /* Struct FileAreaNode */
{
	struct   SignalSemaphore    iaf_FileSemaphore;
	struct   MinList     *iaf_Files;
} ;


/* struct FileNode *******************************************************/

struct      FileNode
{
	struct   MinNode     fn_node;
	struct   fileindex      *fn_index;
} ;

/* struct FileAreaNode ***************************************************/

struct      FileAreaNode
{
	struct   MinNode     fan_node;
	ULONG       fan_Section;
	struct  MinList           fan_Alpha; /* struct IndexNode */
	struct   MinList     fan_Date;  /* struct IndexNode */
} ;

struct      IndexNode
{
	struct   MinNode     in_node;
	struct   extfileindex   in_ndx;
} ;

#endif

struct   NewMapEntry
{
	ULONG       nme_Area;
	time_t         nme_LastSave;
} ;

struct   Mark
{
	ULONG        mn_Number;
	ULONG        mn_Section;
	char         mn_FileName[32];
	char         mn_DosFileName[108];
	ULONG        mn_XferTime;
	BOOL         mn_DosFile;
	ULONG        mn_Size;
} ;

/* sk_Flag */

#define SEARCH_NONE     0
#define SEARCH_DATE     1
#define SEARCH_FILENAME    2
#define SEARCH_DESCRIPTION 3
#define SEARCH_TEXT     4

/* sk_SearchFlag */

#define SEARCH_SINCE    0
#define SEARCH_BEFORE      1

struct      SearchKey
{
	BOOL        sk_Active;
	ULONG       sk_Flag;
	ULONG       sk_SearchFlag;
	char        sk_Data[256];
	ULONG       sk_Length;
	time_t         sk_DateSearch;
} ;

/* struct FileStats ******************************************************/

struct      FileStats
{
	ULONG        NumOfFiles;
	ULONG        High;
} ;

struct   FileRatioData
{
	LONG     FilesDL;
	LONG     BytesDL;
	LONG     FilesUL;
	LONG     BytesUL;
	LONG     FileRatio;
	LONG     ByteRatio;
	LONG     FreeFiles;
	LONG     FreeBytes;
	LONG     MaxFiles;
	LONG     MaxBytes;
	LONG     BytesToday;
	LONG     FilesToday;
} ;

/* struct parsekey */

struct   parsekey
{
	long         firstnum;
	long         endno;
	int          n;
	char        *line;
	long         temp_firstnum;
	long         temp_endno;
} ;

struct   MarkControl
{
	int          MarkPos;
	UBYTE       *MarkArray[64];
	struct   parsekey  MarkKey;
	int          MarkResult;
	UBYTE       *PatternMatch;
	ULONG        MatchLen;
	BPTR         IdxSearch;
	int          MarkOffset;
	struct   Mark      ListMarked;
	ULONG        MarkNumber;
	BOOL         LoopNo;
} ;

struct   StoreData
{
	ULONG     descPos, dataPos;
	ULONG     RecordsLoaded;
	ULONG     RecordsReturned;
	char     *Records;
} ;

#define MODE_AREA   0
#define  MODE_SIG   1
#define MODE_ALL    2

#define FLAG_UNVAL     (UWORD) 0x01
#define FLAG_TEXT   (UWORD) 0x02
#define FLAG_DESC   (UWORD) 0x04
#define FLAG_FILENAME     (UWORD) 0x08
#define FLAG_NEW    (UWORD) 0x10
#define FLAG_OLD    (UWORD) 0x20

struct   SimpleList
{
	struct   ScanInfo     si;
	APTR         key;
	UWORD        mode;
	ULONG        area;
	UBYTE        txt[128];
	time_t          date;
	UWORD        flag;
	BOOL         first;
	UBYTE       *descpos;
	ULONG        maxlines;
	ULONG        writeflags;
	struct   AreaNode    *node;
	int          stdindent;
	int          length;
	int          plus;   
	UBYTE       *fileformat;
	UBYTE       *scanareas;
	UBYTE       *dateformat;
	UBYTE       *firstfile;
	struct   fullFile  ff;
	UBYTE       *outputbuf;
	ULONG        buflen;

} ;

/* struct UserFileData ***************************************************/

struct      UserFileData
{
	ULONG        currentarea;
	ULONG        xprproto;
	struct   fullFile *CurrListFile;
	ULONG        DescLength;
	char        *FileHdr;
	struct   ScanningAreas  *Scan;      /* non-Zero if ScanAreas is in use */
	char         Buffer1 [256]; /* These buffers are not to */
	char         Buffer2 [80];  /* be used in ExpandCC!     */
	BPTR         UploadLock;
	struct   EditorLine  *EdLine;
	char        *FileDesc;     /* NextFile */
	struct   XferInfo *TransferInfo;
	ULONG        CurrentSig;
	struct   XferInfoNode   *XferNode, *SaveFileNode;
	struct   SearchKey    SKey;
	struct   FileRatioData   Ratios;
	struct   MarkControl  MarkCtrl;
	struct   StoreData    StData;
	struct   SimpleList   QuickList;
	ULONG        LastSessionScan;
	ULONG        LastScanDate;
	struct   List      AccessChanges;
} ;

/* struct PF_Globals *****************************************************/

struct      PF_Globals
{
	char         version [6];  /* Format : x.xx */
	struct   FileStats    stats;
	struct  Config    *config;
	struct  PFUser    *user;
	ULONG        NumOfUsers;
	BOOL         RebuilderActive;
	APTR         NewMap;
	ULONG        NewMapEntries;
	BOOL         LogOpen, WinLogOpen;
	BPTR         LogHandle, WinLogHandle;
	struct   Paths    *PipePaths;
	struct   SignalSemaphore  IndexSems[27];
	struct   SignalSemaphore    NumberedIndex;
	struct   SignalSemaphore    HiLowIndex;
	struct   List      MultiPaths;
	struct   ListIndex   *ListIdx;                /* File Area List Indexing */
	ULONG        IndexEntries;
	struct   List      FileBaseLibraries;      /* 1.2 */
} ;

/* struct PFUser *******************************************************/

struct      PFUser
{
	BOOL         un_OnLine;
	ULONG        un_LineNumber;
	char         un_UserName[40];
	struct  UserFileData  un_Ufd;
} ;


/* struct Changes ********************************************************/

struct      Changes
{
	BOOL         AreaList;
} ;

struct      ScanningAreas
{
	struct      ScanInfo  si;
	APTR            ScanKey;
	long            Wrap;
	long            Indent;
	BOOL            IndentFirstLine;
	long            MaxLines;        /* if 0 then display */
							  /* ALL the lines of  */
							  /* the Text!           */
	long            Number;
	long            filesize;
	char           *cp;
	BOOL            FileAlreadyActive;
} ;

/* long __saveds AddToList ( struct Line *le, int Flags ) */

#define  MARKLIST    0x0001
#define DOWNLOADLIST    0x0002

/* struct PrevFileSession ************************************************/

#define  PREVFILESESSIONFILENAME "FileSession.Dat"

struct      PrevFileSession
{
	ULONG       currentarea;
	ULONG       xprproto;
} ;

/* struct AreaNode *******************************************************/

struct      AreaNode
{
	struct   Node      nn_Node;
	struct   filearea    *area;
	struct   SignalSemaphore  sem;     /* Obtain this Semaphore if
															accessing the file areas
															data files */
	LONG         alpha_top; /* File Position of first
							 file alphabetically */
	LONG         alpha_bottom; /* File Position of last
						  file alphabetically */
	UWORD        num_files;
	struct   Library  *FileBase;
} ;

/* struct MarkNode *******************************************************/
/* Obsolete

struct      MarkNode
{
	struct   MinNode      mn_Node;
	struct   MarkedFile  *mn_File;
} ;

struct      MarkedFile
{
	char         mn_FileName [32];
	ULONG        mn_Number;
	ULONG        mn_Area;
	ULONG        mn_MarkNum;
	BOOL         mn_DosFile;
	char        *mn_DosFileName;
} ;

struct   DownLoadNode
{
	struct   Node     dln_Node;   
	char        dln_FileName [256];
	ULONG       dln_FileSize;
	ULONG       dln_XferTime;
	ULONG       dln_FileNumber;
} ;

*/

struct   FileScanKey
{
	/* PRIVATE DATA */
	BPTR        descfH, datafH;
	int         RecordsLoaded;
	int         RecordsReturned;
	int         Buffering;
	char        *Records;
	int         ScanFlag, Direction;
	/* 095 */
	short       noBackScan;
	/* 0991 for Alpha Sorting */
	ULONG        Offset;
	ULONG        AreaNumber;
	ULONG        DescSize;
	UBYTE       *Desc;
	ULONG        DescPosition;
	ULONG        MemSize;
};

/* Xpr Stuff **/

struct   XprProtoNode
{
	struct   Node     xpn_node;
	struct  XprProto       *xpn_xprproto;
} ;

#define XP_BIDIRECTIONAL 0x0001
#define XP_AUTOUPLOAD    0x0002
#define XP_AUTODOWNLOAD  0x0004
#define XP_BATCH         0x0008
#define XP_EXECUTABLE    0x0010

struct   XprProto
{
	char  xp_name     [40];
	char  xp_trxpath  [256];
	char  xp_rxpath   [256];
	char  xp_libname  [50];
	int   xp_loaccess;
	int   xp_hiaccess;
	ULONG   xp_flags;
} ;

/* OLD Xpr Info Structures **************************/

struct   XPRLine
{
	struct   Line     *le;
	char        *LibName;
	char        *TxString;
	char        *RxString;
	char        *SendFiles;
	char        *ReceiveFiles;
	UWORD        XferMode;
	UWORD        ProtocolSpecs;
} ;

#define  XM_RX    0x0001
#define  XM_TX    0x0002
#define  XM_BI    0x0004   

#define  XM_AUTOUL   0x0001
#define  XM_AUTODL   0x0002
#define  XM_BATCH 0x0004

/**********************************************************************/
/*                         File Area Accesses                         */
/**********************************************************************/

#define FA_ACCESS 0x0001
#define FA_VIEW      0x0002
#define FA_DOWNLOAD  0x0004
#define FA_UPLOAD 0x0008
#define FA_EDITOWN   0x0010
#define FA_EDITALL   0x0020
#define FA_UNVALIDATED  0x0040
#define FA_VALIDATE  0x0080
#define FA_DELETE 0x0100
#define FA_SYSOP  0x0200
#define FA_MASKEDOUT    0x0400
#define FA_PACK      0x0800

struct   UserFileAccess
{
	ULONG FileRatio;
	ULONG ByteRatio;
	ULONG MaxBytesPerDay;
	ULONG MaxFilesPerDay;
	ULONG FreeBytes;
	ULONG FreeDownloads;
} ;

struct   FileAreaIndex
{
	ULONG Section;
	ULONG AlphaTop;
	ULONG AlphaBottom;
	ULONG NumberOfFiles;
} ;

struct  numidx
{
	LONG     HdrOffset;
	LONG     IdxOffset;
	char     FileName[32];
} ;

struct   FileSavedData
{
	ULONG    Protocol;
	ULONG    FileArea;
	ULONG    Sig;
	ULONG    LastScanDate;
} ;

/* EOF ******************************************************************/


/* What *is* this fake file thing???? */

/* struct fakefile *******************************************************/

struct         fakefile
{
	time_t         UploadDate;
	time_t         LastDownloaded;
	UWORD       NumberOfDownloads;
} ;

/* struct savefile *******************************************************/

struct         savefile
{
	ULONG       area;
	char        filename [32];
	UBYTE              *desc;     // MUST be Null Terminated
	ULONG       flags;      // See 'struct File' for info
	BOOL        yesnoFakeInfo;  // Set to TRUE and allocate struct fakefile if required
	struct fakefile        *fakeinfo;   // Set to NULL if not required
   char        UserName[40];
} ;



#endif /* ZFILE_H */
