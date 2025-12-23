/************************************************************************/
/*                              SysDep.H                                */
/*      #include file for all Citadel C files; contains system          */
/*      dependent code.                                                 */
/************************************************************************/
/*
* This is here because Borland International requires all licensees to place
* copyrights on any distributable s/w.  If you are porting C-86 to another
* machine, you should certainly replace the value here with something else.
* A "" is probably acceptable.
*/
#define COPYRIGHT " Custom Services 1996"
#include "fctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "exec/types.h"
#include "exec/exec.h"
#include "devices/console.h"
#include "devices/keymap.h"
#include "devices/serial.h"
#include "devices/timer.h"
#include "libraries/dos.h"
#include "libraries/dosextens.h"
#include "intuition/intuition.h"
#include "proto/all.h"
#define ANSI_PROTOTYPING
#define IS_MOTOROLA
#define OUTSIDE_EDITOR_SUPPORT
#define SYSTEM_CLOCK
/**
  Special definitions used in message strings
**/
#define VARIANT_NAME    "Citadel-68k"
#define VERSION_NAME  "3.42"
#ifdef _M68020
#define SYSDEP_NAME   "Q16/030"
#else
#define SYSDEP_NAME   "Q16"
#endif
/**
  This structure is used by the BadPeople.sys file processing
  to collect the list of suppressed user/room/system names

  A * in the field indicates that the comparision for that
  field is skipped and is a match.

  An entry of "* * *" would suppress all messages in all
  rooms for all systems.  This is illegal and will be rejected
  as an error.
**/
struct User_Id
  {
  char *name;   /* suppressed user  ( * == all users ) */
  char *room;   /* suppressed room  ( * == all rooms ) */
  char *system; /* suppressed system( * is all systems ) */
  };

#define HORRID_AMIGA_LATTICE_BUG

/* these are system debug functions.  */
/* note: some are scattered through out the code */
/*#define NEED_MSG_PEEKING        /* sysop menu items */
/*#define NEED_MSG_LIST           /* sysop menu items */
/*#define NEEDED                    /* sysop menu items */
/*#define EVENT_DEBUG               /* examine events */
/*#define NEED_NET_DEBUG          /* network debug messages */
/*#define NEED_NET_DEBUG_ERRORS   /* network debug messages */
/*#define NEED_TRANS_NET_DEBUG    /* network debug messages */
/*#define TEST_SYS                /* enable debug output */
/*#define NET_DEBUG               /* network debug output */

#define sPrintf  sprintf

void *special_GetDynamic(unsigned size, char *file, int);
#define GetDynamic(size) special_GetDynamic(size,__FILE__,__LINE__)


/************************************************************************/
/* These constants, etc. are only for use by SYSDEP.C and other source  */
/* files that contain code that depends on the computer system in use.  */
/* Access is via #define in the appropriate source files.               */
/************************************************************************/
/*
* Required! The following MUST be defined:
*   MSG_SECT_SIZE -- size, in bytes, of a (psuedo) "sector" on disk
*                    for the message file
*   SIZE_SYS_FILE -- longest length, in bytes, of a "system" (i.e., *.SYS,
*                    *.BLB, *.MNU, *.HLP, and net files) file plus 1 (for
*                    the NULL byte.  Variables using this constant are used for
*                    defining a name of a file and then opening it.
*   MSG_NUMBER  -- just what is* a message number
*   SECTOR_ID   -- just what to use to identify a sector
*   AN_UNSIGNED -- an unsigned quantity, very preferably 8 bits
*   ROOM_MSG    -- this needs* to be 16 bits, for a kludge in listRooms()
*   DATA_BLOCK[MSG_SECT_SIZE] -- a block of msg data from disk
*   CRC_TYPE    -- sufficient to hold a XMODEM CRC value
*   SYS_FILE    -- a variable used in makeSysName() that holds the "name" of
*                  a system file (see SIZE_SYS_FILE).  This is passed to
*                  safeopen().  Normally, this would be just a string using
*                  the SIZE_SYS_FILE constant, but I can see situations in
*                  which a struct would be more appropriate.  If you have*
*                  to go with a struct, I suggest making safeopen() a macro
*                  that takes the address of the name (see description of
*                  safeopen for its function).  I.e...
*  #define safeopen(x, y)    ourOpen(&x, y)
*   ROOM_AREA   -- definition of an area attached to a room
*   NET_AREA    -- definition of an area accessible to net functions
*   SYS_AREA    -- definition of an area that may be occupied by a Citadel
*                  system file.
*   MODEM_DATA  -- all necessary modem data for this implementation
*
*/
#define MSG_SECT_SIZE   128
#define SIZE_SYS_FILE   128             /* Should be enough             */
#define MAX_FILENAME   24
#define LOCKFILE        "ctdllock.sys"

#define ALL_FILES       "*"
#define CACHED_FILES    "*.msg"

typedef unsigned long   MSG_NUMBER;       /* Msg number for PClone 32 bits*/
typedef unsigned short  SECTOR_ID;        /* Sector ID -- 16 bits         */
typedef unsigned char   AN_UNSIGNED;
typedef unsigned char   DATA_BLOCK[MSG_SECT_SIZE];
typedef unsigned short  CRC_TYPE;
typedef char            SYS_FILE[SIZE_SYS_FILE];
typedef char            DOMAIN_FILE[40];
typedef unsigned long   MULTI_NET_DATA;
typedef unsigned long   UNS_32;
typedef unsigned short  UNS_16;
struct MenuData
  {
  struct Window *MenuWindow;
  char *MenuCharList;
  struct MsgPort *MenuIOPort;
  struct IOStdReq *MenuIOMsg;
  int DeviceOpen;

  };
typedef struct MenuData *MenuId;
#define S_MSG_MASK      0x7fffffffl     /* hi bit, right?       */
#define NO_MENU   NULL
typedef short ROOM_AREA;          /* This might carry the # of the room   */
typedef struct
  {
  char        naDirname[100]; /* Directory anywhere on system         */

  }
NET_AREA;
typedef struct
  {
  char  saDirname[64];

  }
SYS_AREA;
#define NO_CLOCK        0
#define BUSY_CLOCK      1
#define ALWAYS_CLOCK    2
#define SER_7WIRE       4
struct timePacket
  {
  unsigned long clock[2];

  };
typedef struct
  {
  char Editor[15],              /* Outside editor name          */
  EditArea[40],                 /* Where should we do our work? */
  ModemSetup[60],           /* First init of modem.             */
  HiSpeedInit[30];        /* High speed init            */
  short  Clock;
  char LockPort;
  unsigned short ScreenWidth,ScreenHeight,ScreenDepth,Color0,Color1;
  char StartUpEcho,DirectToChip,UnitNumber,DevName[32];

  }
DependentData;

#define BOOLEAN_FLAG(x)         unsigned x : 1
#define mvToHomeDisk(x)
#define makeVASysName(x, y)     sPrintf(x, "virtual/%s", y)
#define CreateVAName(fn, slot, dir, num) sPrintf(fn, "virtual/%d/%s/%ld", slot, dir, num)
#define ChatEat(c)      c == 0
#define ChatSend(c)     c == 0
#define makeSysName(x, y, z)    sPrintf(x, "%s%s", (z)->saDirname, y)
#define RoomSys(x)      roomBuf.rbArea = x
#define zero_struct(x)     setmem(&x, ( (long)sizeof( x ) & 0x0000ffffL), 0)
#define zero_array(x)      setmem( x, ( (long)sizeof( x ) & 0x0000ffffL), 0)
#define copy_struct(src, dest)  memcpy(&dest, &src, ( (long)sizeof( src )) )
#define copy_array(src, dest)   memcpy(dest,   src, ( (long)sizeof( src )) )
#define copy_ptr(src, dest, s)  memcpy(dest,   src, ( (long)(sizeof( src[0] )) * (long)s) )
#undef  memmove
#define memmove(dest, src, len)  movmem(src, dest, ( (long)len & 0x0000FFFFL) )
#define fastMod outMod          /* temporary */
#define ToTempArea()    MakeTempDir()
extern char TDirBuffer[];
#define KillTempArea()  homeSpace(), rmdir(TDirBuffer)
#define ScreenUser() ScrNewUser()
#define setNetCallBaud(x,y) CitadelBaudRate(minimum(x, cfg.sysBaud),y);
/* sPrintf(temp, "banners\\%s.%d", str, GetSecond()); */
#define makeBanner(x, y, z)     sPrintf(x, "banners/%s.%d", y, z)
#define RedirectName(b, d, f)  sPrintf(b, "%s/%s", d, f);
#define MakeDomainFileName(buffer, Dir, filename) \
sPrintf(buffer, "%s%d/%s", cfg.domainArea.saDirname, Dir, filename)
#define MakeDomainDirectory(x)  DoDomainDirectory(x, FALSE)
#define KillDomainDirectory(x)  DoDomainDirectory(x, TRUE)
#define MakeBioName(buffer, name) \
sPrintf(buffer, "%s%s", cfg.bioArea.saDirname, name)
#define MoveToBioDirectory()    chdir(cfg.bioArea.saDirname);


#define VirtualCopyFileToFile(fn, vfn)  CitSystem(FALSE, "copy %s %s > nul", fn, vfn)

/*
 * These are for handling the net caching stuff.
 */
#define ChangeToCacheDir(x)     chdir(x)
#define NetCacheName(buf, slot, name)       sPrintf(buf, "%snetcache/%d/%s", cfg.netArea.saDirname, slot, name)
#define MakeNetCacheName(buf, slot)       sPrintf(buf, "%snetcache/%d",    cfg.netArea.saDirname, slot)
#define MakeNetCache(buf)                       sPrintf(buf, "%snetcache",       cfg.netArea.saDirname)
#define MakeDeCompressedFilename(fn, Fn)  sPrintf(fn, "%s%s", cfg.netArea.saDirname, Fn)
typedef struct {
        char *FileName;
        char *MenuEntry;
        char *DeWork;
        char *IntWork;
        char *CompWork;
} DeCompElement;

extern DeCompElement DeComp[];

extern DeCompElement DeComp[];
#define GetCompEnglish(CompType)   DeComp[(CompType) - 1].MenuEntry
#define DeCompAvailable(CompType) (DeComp[(CompType) - 1].DeWork != NULL)
#define AnyCompression() (DeComp[LHA_COMP-1].CompWork != NULL || DeComp[ARC_COMP-1].CompWork != NULL || DeComp[ZOO_COMP-1].CompWork != NULL || DeComp[ZIP_COMP-1].CompWork != NULL)

#define netSetNewArea(fd) realSetSpace((fd)->naDirname)

#ifndef CONFIGURE
/* we use our own printf in sysdep3.c */
#undef printf
int printf(const char *format, ...);
#endif
/************************************************************************/
/* These constants, etc. are only for use by SYSDEP.C and other source  */
/* files that contain code that depends on the computer system in use.  */
/* Access is via #define in the appropriate source files.               */
/************************************************************************/
#ifdef CONFIGURE
/**
  parameters for checks in confg
**/
#define HELP            0
#define LOG             1
#define ROOM            2
#define MSG             3
#define MSG2            4
#define NET_STUFF       5
#define CALL            6
#define HOLD            7
#define FLOORA          8
#define DOMAIN_STUFF    9
#define TEMP_AREA      10   /* temp files area                    */
#define QWK2           11   /* BBS Packet name defined            */
#define QWK3           12   /* Files Area Defined                 */
#define QWK4           13   /* Maximum number of rooms at once    */
#define QWK5           14   /* Maximum number of messages at once */
#define BIO_AREA       15   /* biography area defined             */
#define QWK1           16   /* location defined                   */
#define MAX_NEEDED     17   /* total number of parameters checked */
/**
  End of CONFG check parameters
**/
#define I_HANGUP        0
#define I_INIT          1
#define I_CARRDET       2
#define I_SET300        3
#define I_SET1200       4
#define I_SET2400       5
#define I_SETHI         6
#define I_CHECKB        7
#define I_ENABLE        8
#define I_DISABLE       9
#define I_SET4800       10
#define I_SET9600       11
#define I_SET14400      12
#define I_SET19200      13
/* Hmmmmmm.  This stuff shouldn't be here, actually. */
#define R_SH_MARK       "&&"    /* Rather than external char    */
#define NON_LOC_NET     "%%"
#endif
/* Or this.  Fix this someday */
#ifdef V_ADMIN
#define R_SH_MARK       "&&"    /* Rather than external char    */
#define NON_LOC_NET     "%%"
#define LOC_NET         "++"
#endif
#define strCmp          strcmp
#define strLen          strlen
#define toUpper         toupper
#define toLower         tolower
#define strCpy          strcpy
#define strCat          strcat
#define isAlpha         isalpha
#define strnCmp         strncmp
#define isSpace         isspace
#define SEEK_CUR                1
#define NO_STAT
/*#define CTDL_BACKGROUND*/
void Intel16ToMotorola(UNS_16 *);
void Intel32ToMotorola(UNS_32 *);
/*
** Caller Count routines
*/
void Update_Caller_Count(void);
long Get_Call_Count(void);
char *Display_Privledges(void);
