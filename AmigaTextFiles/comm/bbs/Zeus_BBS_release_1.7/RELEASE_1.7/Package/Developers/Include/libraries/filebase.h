/**
*** Zeus File Base Structure/Flag Definitions
**/

#ifndef LIBRARIES_FILEBASE_H
#define LIBRARIES_FILEBASE_H

/* fbGetInfo() */

struct FileBaseInfo
{
	/* Don't shoot, fbi! */

	UBYTE    fbi_Name[32];
	UBYTE    fbi_Copyright[108];
	UBYTE    fbi_Author[108];
	UBYTE    fbi_Comment[108];
	ULONG    fbi_ID; /** Field referenced by struct filearea */
} ;

/* fbSaveFile() */

#define SF_NODUPECHECK     0x0001
#define SF_NOFILECHECK     0x0002
#define SF_NOFILENOTE      0x0004

/* in zfile.h

struct         fakefile
{
	time_t         UploadDate;
	time_t         LastDownloaded;
	UWORD       NumberOfDownloads;
} ;

struct         savefile
{
	ULONG       area;
	char        filename[32];
	UBYTE              *desc;     // MUST be Null Terminated
	ULONG       flags;      // See 'struct File' for info
	struct fakefile        *fakeinfo;   // Set to NULL if not required
   char        UserName[40];
} ;*/

/* fbDeleteCatFile() */

#define DF_PHYSICAL     0x0001

/* fbInitFileScanKey() */

#define  SA_NATURAL     1
#define  SA_ALPHA       2
#define  SA_CHRONO      3

#define  SA_FORWARDS    1
#define  SA_BACKWARDS   2

//struct    ScanInfo    // in zmf.h
//{
//   int   ScanFlag;      /* NATURAL/ALPHA/CHRONO */
//   ULONG StartNumber;   /* Message Number to start at, or 0 to start */
								/* at beginning of file.                     */
//   int   Direction;     /* FORWARDS/BACKWARDS */
//   int   Buffer;        /* 32 is a nice one... */
//   ULONG Area;          /* File/Msg Area to Start At */
//} ;

/* fbFindFile() */

struct   FindAreaNode
{
	struct   MinNode  node;
	ULONG             area;
} ;

/* LoadFileBaseList */

struct   FileBaseNode
{
	struct   Node            fbn_Node;
	struct   Library        *fbn_Library;
	struct   FileBaseInfo    fbn_Info;
} ;

struct   FBDiskConfig
{
	UBYTE       name[32];
} ;

#endif
