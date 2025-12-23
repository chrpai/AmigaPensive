#ifndef _CL_MAILMASTER_H
#define _CL_MAILMASTER_H

#ifndef _CL_MAIL_H
#include <cl/cl_mail.h>
#endif

/*
**   cl_mailmaster.h 
**   ---------------
**
**   Spezifikationen zur Benutzung der
**   "cl_mailmaster.library"
**
*/

//
// AmigaOS library specifications.
// Do not use yourself!
//

#define CLMAILMASTERNAME "cl_mailmaster.library"
#define CLMAILMASTERVERSION 6

extern struct Library *CLMailMasterBase;

//
// Function prototypes
//

#ifndef _NOPROTOS

#pragma libcall CLMailMasterBase CLMM_SendFile 1e A9803
#pragma tagcall CLMailMasterBase CLMM_SendFileTags 1e A9803
#pragma libcall CLMailMasterBase CLMM_MailSysList 24 0
#pragma libcall CLMailMasterBase CLMM_GetMailLib 2a 801
#pragma libcall CLMailMasterBase CLMM_AllocFiles 30 0802
#pragma libcall CLMailMasterBase CLMM_FreeFiles 36 801
#pragma libcall CLMailMasterBase CLMM_FlushAll 3c 0
#pragma libcall CLMailMasterBase CLMM_Batch 42 0802
#pragma libcall CLMailMasterBase CLMM_BoxInfo 48 9802
#pragma libcall CLMailMasterBase CLMM_TestNetLogin 4e 9802
#pragma libcall CLMailMasterBase CLMM_HandleNetcall 54 BA9804
#pragma libcall CLMailMasterBase CLMM_DoNetcall 5a B10A9806
#pragma libcall CLMailMasterBase CLMM_QuerySpecial 60 9802
#pragma libcall CLMailMasterBase CLMM_GetMailSysList 66 0
#pragma libcall CLMailMasterBase CLMM_GetMailSysIndex 6c 801

ULONG CLMM_SendFile( STRPTR from, STRPTR *destinations, struct TagItem *tags );
ULONG CLMM_SendFileTags( STRPTR from, STRPTR *destinations, ... );

APTR CLMM_MailSysList( void );
struct Library * CLMM_GetMailLib( STRPTR netname );
int CLMM_AllocFiles( struct Box *box, int flags );
int CLMM_FreeFiles( struct Box *box );
void CLMM_FlushAll( void );
void CLMM_Batch( struct Box *box, int flags );
void CLMM_BoxInfo( struct Box *box, CLMail_BoxInfo *boxinfo );

ULONG CLMM_TestNetLogin( STRPTR loginstring, STRPTR mailsysbuffer );
ULONG CLMM_DoNetcall( APTR serhandle, STRPTR portid, struct Box *box, ULONG retries, ULONG flags, struct PortInfo *pi );
ULONG CLMM_HandleNetcall( APTR serhandle, STRPTR portid, STRPTR mailsys, struct PortInfo *pi );

STRPTR CLMM_QuerySpecial( struct Box *box, STRPTR netname );

STRPTR * CLMM_GetMailSysList( void );
LONG CLMM_GetMailSysIndex( STRPTR netname );

#endif

//
// Tag Values
//

#define CLMM_TAGBASE (TAG_USER+0x2532)

#define CLMMT_SourceFile 	(CLMM_TAGBASE+1)		// STRPTR, filename
#define CLMMT_SourceBuffer	(CLMM_TAGBASE+2)		// APTR, memory buffer
#define CLMMT_SourceLength  (CLMM_TAGBASE+3)		// ULONG, length
#define CLMMT_SourceCharset (CLMM_TAGBASE+10)		// ULONG, see CLMM_CS_#? below

#define CLMMT_CommentFile 	(CLMM_TAGBASE+4)		// STRPTR, filename
#define CLMMT_CommentBuffer	(CLMM_TAGBASE+5)		// APTR, memory buffer
#define CLMMT_CommentLength (CLMM_TAGBASE+6)		// ULONG, length
#define CLMMT_CommentCharset (CLMM_TAGBASE+11)		// ULONG, see CLMM_CS_#? below

#define CLMMT_ShortComment	(CLMM_TAGBASE+7)		// STRPTR, short file comment
#define CLMMT_Subject		(CLMM_TAGBASE+8)		// STRPTR, subject line
#define CLMMT_ContentsType	(CLMM_TAGBASE+9)		// ULONG, see CLMM_CTF_#? below

#define CLMMT_Priority		(CLMM_TAGBASE+12)		// ULONG, mail priority (0/10)
#define CLMMT_ExportName 	(CLMM_TAGBASE+14)		// STRPTR, informational file name

#define CLMMT_External		(CLMM_TAGBASE+13)		// BOOL, msg contents are stored externally
#define CLMMT_LocalOnly		(CLMM_TAGBASE+15)		// BOOL, store msg locally only

#define CLMMT_HeaderList    (CLMM_TAGBASE+16)		// HH, additional header lines

#define CLMMT_SortinPri		(CLMM_TAGBASE+17)		// ULONG, sortin priority (Ascii-Char only); Defaults to 'A' (highest priority)

//
// Flags
//

#define CLMM_CTF_TEXT 	0		// Msgtype is text
#define CLMM_CTF_BIN	1		// Msgtype is binary

#define CLMM_CS_ISO		0 		// Charset is ISO, internally converted to IBM850
#define CLMM_CS_IBM		1		// Charset is IBM, not converted

#define CLMM_MP_STD		0		// Mail priority: standard routing
#define CLMM_MP_FAST	10		// Mail priority: fast routing if possible

//
// Errors
//

#define CLMME_OK		0
#define CLMME_NOSOURCE	1			// no source specification
#define CLMME_CANTOPENOUTFILE 2		// can't open outfile file
#define CLMME_ARGS		3			// Argumente im Arsch
#define CLMME_CANTOPENSOURCEFILE 4	// can't open input file
#define CLMME_CANTOPENCOMMENTFILE 5 // can't open comment file
#define CLMME_WRITEERROR 6			// Filesystem reported some write error...
#define CLMME_NODEST 7				// no destinations?!?
#define CLMME_READERROR_COMMENT 8	// DOS Error during read of comment
#define CLMME_RENAMEERR 9			// DOS Error while renaming from PRESPOOL to SPOOL
#define CLMME_READERROR_BODY 10		// DOS Error during read of body

#endif
