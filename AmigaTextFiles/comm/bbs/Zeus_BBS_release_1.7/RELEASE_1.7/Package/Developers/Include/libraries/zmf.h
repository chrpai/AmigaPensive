#ifndef LIBRARIES_ZMF_H
#define LIBRARIES_ZMF_H

//#ifndef PLMSG_H
#include <zmsg.h>
//#endif

#include <zconfig.h>

/**********************************************************************/
/*                              BAUDLIST                              */
/**********************************************************************/

#define BE_DOWNLOADS                     0x0001
#define BE_UPLOADS                       0x0002
#define BE_LOGONS                        0x0004

struct BaudEntry
{
		  ULONG                            bn_BPS;
		  ULONG                            bn_CPS;
		  ULONG                            bn_Flags;
} ;

struct BaudNode
{
		  struct Node                      bn_Node;
		  struct BaudEntry                 bn_Entry;
} ;


/**********************************************************************/
/*                             EDITORLIST                             */
/**********************************************************************/

#define REMOTEONLY              0
#define LOCALONLY               1
#define REMOTE&LOCAL            2

struct  Editor
{
		  char                     name [EDITORNAME_LENGTH];
		  int                      lo_access;
		  int                      high_access;
		  int                      type;
		  int                      seglength;
};

struct  EditorNode
{
		  struct  Node             en_Node;
		  struct  Editor          *editor;
		  UBYTE                   *seg;
		  struct  List             ScriptSegs; /* struct EdSegNode */
};

struct  EdSegData
{
		  char                     esd_Name[80];
		  UBYTE                   *esd_Pointer;
} ;

struct  EdSegNode
{
		  struct  Node             esn_Node;
		  struct  EdSegData        esn_Data;
} ;

/**********************************************************************/
/*                             EDITORLINE                             */
/*                      For Message Editor's Use                      */
/**********************************************************************/

#define MESSAGE_TEXT       1
#define FILE_DESCRIPTION   2

struct  EditorLine
{
		  struct  Line            *le;
		  ULONG                    Flags;
		  /* Message Editing Stuff */
		  char                     From[36];
		  char                     To[36];
		  char                     Subject[36];
		  BOOL                     Reply;
		  struct  FiveDFido        FromAddress;
		  struct  FiveDFido        DestAddress;
		  ULONG                    MessageArea;
		  ULONG                    MsgFlags;
		  struct  WholeMessage    *ReplyMsg;
		  /* File Description */
		  char                     FileName[20];
		  ULONG                    AreaNumber;                     
		  char                    *FileDescription;
} ;

/**********************************************************************/
/*                             ARCHIVERS                              */
/**********************************************************************/

#define ARC_SHOW_USER            0
#define ARC_NO_SHOW_USER         1

struct  Archiver
{
		  UBYTE                    Name[78];
		  UWORD                    Flags;         // This is not a bit flag.  It == ARC_SHOW.. or ..NO_SHOW..
		  UBYTE                    Compress[256];
		  UBYTE                    Extract[256];
		  UBYTE                    Recog[256];
		  UBYTE                    Test[256];
} ;

struct  ArchiverNode
{
		  struct  Node             an_Node;
		  struct  Archiver         an_Arc;
} ;

/**********************************************************************/
/*                              SCANINFO                              */
/*           for use with InitFileScanKey & InitMsgScanKey            */
/**********************************************************************/

#define  SA_NATURAL     1
#define  SA_ALPHA       2
#define  SA_CHRONO      3

#define  SA_FORWARDS    1
#define  SA_BACKWARDS   2

struct   ScanInfo
{
		  /* INITFILESCANKEY() data */
		  int     ScanFlag;       /* NATURAL/ALPHA/CHRONO */

		  /* INITMSGSCANKEY() data  */
		  ULONG   StartNumber;    /* Message Number to start at, or 0 to start */
										  /* at beginning of file.                     */

		  /* GLOBAL data            */
		  int     Direction;      /* FORWARDS/BACKWARDS */
		  int     Buffer;         /* 32 is a nice one... */
		  ULONG   Area;           /* File/Msg Area to Start At */
} ;

/**********************************************************************/
/*                             PROGRAMLOG                             */
/**********************************************************************/

#define PLFILE_LOG      1
#define PLMSG_LOG       2

/**********************************************************************/
/*                            SIG RELATED                             */
/**********************************************************************/

#define DEFAULT_ADD     0x0001

struct  ConfigPickNode
{
		  struct  Node    apn_Node;
		  UBYTE           apn_Num[9];
		  ULONG           apn_Number;
		  UBYTE           apn_Name[64];
		  BYTE            apn_On;
} ;

struct  SigNode
{
		  struct  Node     sn_Node;
		  UBYTE            sn_Tag[9];
		  UBYTE            sn_Desc[40];
		  UBYTE            sn_Low;
		  UBYTE            sn_High;
		  UWORD            sn_Flags;
		  UBYTE            sn_DirTag[9];
		  struct  List     sn_Directories;
		  ULONG           *sn_MsgAreas;
		  ULONG           *sn_FileAreas;
		  UWORD            sn_NumMsgAreas;
		  UWORD            sn_NumFileAreas;
} ;

struct  SigSavedData
{
		  UBYTE            ssd_Tag[9];
		  UBYTE            ssd_Desc[40];
		  UBYTE            ssd_Low;
		  UBYTE            ssd_High;
		  UWORD            ssd_Flags;
		  UBYTE            ssd_DirTag[9];
		  LONG             ssd_MsgAreas;
		  LONG             ssd_FileAreas;
} ;

/**********************************************************************/
/*                      Address Parsing Routines                      */
/**********************************************************************/

struct  ReadAdrNode
{
		  struct  Node             ran_Node;
		  struct  FidoAddress     *ran_Address;
} ;
		  
#define FIDO_STANDARD_N         1
#define FIDO_STANDARD_R         2
#define FIDO_STANDARD_RN        3

/**********************************************************************/
/*                  Find(File|Message)FromDate Flags                  */
/**********************************************************************/

#define FMFD_EQUAL     0x0001
#define FMFD_LESS_THAN 0x0002
#define FMFD_MORE_THAN 0x0004
#define FMFD_MONTH     0x0008
#define FMFD_HOUR      0x0010
#define FMFD_MIN       0x0020
#define FMFD_SECONDS   0x0040
#define FMFD_DAY       0x0080

/**********************************************************************/
/*                         LoadMessage Flags                          */
/**********************************************************************/

#define LM_NOFIDOINFO  0x0001
#define LM_NEXTMSG     0x0002
#define LM_NOTEXT      0x0004

/**********************************************************************/
/*                        Template Definitions                        */
/**********************************************************************/

struct  TemplateData
{
		  LONG    CallsDay;
		  LONG    MinsCall;
		  LONG    MinsDay;
		  LONG    FileRatio;
		  LONG    ByteRatio;
		  LONG    MaxBytes;
		  LONG    MaxFiles;
		  LONG    FreeFiles;
		  LONG    FreeBytes;
		  LONG    MaxMsgs;
} ;

struct  Template
{
		  LONG                    te_AccessLvl;
		  UBYTE                   te_Description[40];
		  BOOL                    te_AutoAdd;
		  ULONG                   te_MessageClone;
		  ULONG                   te_FileClone;
		  struct  List            te_MessageAreas;
		  struct  List            te_FileAreas;
		  struct  TemplateData    te_Data;
} ;

struct  TemplateNode
{
		  struct  Node            tn_Node;
		  struct  Template        tn_Template;
} ;

struct  TemplateAreaNode
{
		  struct  Node            tan_Node;
		  ULONG                   tan_Low;     // lowest area
		  ULONG                   tan_High;    // highest area
		  ULONG                   tan_Flags;
		  BOOL                    tan_FileOrMsg; /* 0 = File, 1 = Msg */
} ;

struct  SaveTemplate
{
		  LONG                    st_AccessLvl;
		  UBYTE                   st_Description[40];
		  BOOL                    st_AutoAdd;
		  ULONG                   st_MessageClone;
		  ULONG                   st_FileClone;
		  struct  TemplateData    st_Data;
		  ULONG                   st_FileAreas; /* Files saved First */
		  ULONG                   st_MsgAreas;  /* and then Messages */
} ;

struct  SaveAreaTemplate
{
		  ULONG                   sat_Low;
		  ULONG                   sat_High;
		  ULONG                   sat_Flags;
} ;

/**********************************************************************/
/*                           Config Update                            */
/**********************************************************************/

struct  ConfigUpdate
{
		  /* Message Areas */
		  void                    (*FreeMsgAreaNodes)( struct List * );
		  void                    (*CloseLibraries)( void );
		  int                     (*MsgAreasLoad)( struct List * );
		  int                     (*OpenLibraries)( void );

		  /* File Areas */
		  void                    (*ScanFileAreas)( void );
		  void                    (*FreeAreas)( void );

		  /* General Configuration */
		  int                     (*LoadGeneralCfg)( struct GeneralCfg * );

		  /* Editors */
		  void                    (*FreeEditorNodes)( struct List * );

		  /* Baud Rates */
		  void                    (*FreeBaudList)( void );

		  /* Xpr Protocols */
		  void                    (*FreeXprNodes)( struct List * );

		  /* Archivers */
		  int                     (*ArchiverLoad)( void );
		  void                    (*FreeArchivers)( void );

		  /* Sigs */
		  void                    (*FreeSigs)( struct List * );

		  /* Templates */
		  BOOL                    (*LoadTemplates)( struct List * );
		  void                    (*FreeTemplates)( struct List * );

		  /* General Config */
		  BOOL                    (*LoadGeneralFileCfg)( struct GeneralFileCfg * );
} ;

struct  AreaAccess
{
		  UWORD                   aa_Num;
		  UWORD                   aa_Flags;
		  UWORD                   aa_TemplateFlag;
} ;

struct  ExtAreaAccess
{
		  UWORD                   aa_Num;
		  UWORD                   aa_Flags;
		  UWORD                   aa_TemplateFlag;
} ;

struct  AccessChange
{
		  struct  MinNode          node;
		  struct  ExtAreaAccess    acc;
} ;

/*
** Multiple Path Support
*/

// NEW
struct FilePath
{
	UBYTE              name[109];
	UBYTE              type;
	ULONG              fbID;
} ;

struct   PathEntryNode
{
	struct   Node     pen_Node;
	ULONG       pen_fbID;
} ;

// OLD
struct  MultiPathNode
{
		  struct  Node            mpn_Node;
		  UWORD                   mpn_Area;
		  struct  List            mpn_Paths; /* struct Node */
} ;

struct  MultiPath
{
		  UWORD                   mp_Area;
		  UWORD                   mp_Paths;

		  /*
		  UBYTE                   mp_Path[64];
										  :
										  .
		  */
} ;

struct  ListIndex
{
		  ULONG                   node;
		  ULONG                   number;
} ;

#define MARK_DOS                0
#define MARK_BBS                1

#define MARK_OK                  1

#define MARK_EXISTS             -1
#define NO_USER                 -2
#define NO_FILE                 -3
#define NO_ADD                  -4

/**
*** OutputHdrCodes ()
**/

#define FORWARD_HDR                     1
#define CROSSREPLY_HDR          2
#define REPLY_HDR                       3

/*
** EventNode for DataManager
*/

#define EV_UP_TEMPLATES  0x0001
#define EV_UP_SIGS       0x0002
#define EV_UP_DELETE     0x0004
#define EV_UP_RENUMBER   0x0008

struct  SavedEvent
{
		  ULONG            area;
		  ULONG            flags;
		  ULONG            from, to;
		  BOOL             file;
} ;

struct  EventNode
{
		  struct  Node     node;
		  ULONG            area;
		  UBYTE            name[80];
		  ULONG            flags;
		  ULONG            from, to;
		  BOOL             file;
} ;

/*
** For use with SetMsgAccess, SetFileAccess
*/

#define FLAG_EQUAL      0
#define FLAG_ADD        1
#define FLAG_REMOVE     2

#define SCAN_MESSAGE_AREAS      0
#define SCAN_FILE_AREAS         1

#define SCANBACK_MESSAGE_AREAS  2
#define SCANBACK_FILE_AREAS     3

/** DataManager structures **/

struct   SnapshotHdr
{
	ULONG        Areas;
	BOOL         File;
} ;

struct   Snapshot
{
	ULONG Area;
	ULONG ID;
} ;

#define ADD_EVENT 0
#define DELETE_EVENT 1
#define RENUMBER_EVENT  2

#endif /* LIBRARIES_ZMF_H */
