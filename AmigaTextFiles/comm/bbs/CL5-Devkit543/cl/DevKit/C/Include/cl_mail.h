#ifndef _CL_CL_MAIL_H
#define _CL_CL_MAIL_H

/*
 *
 *  cl_mail_#?.library common definitions
 *
 */

typedef struct CLMail_SendInfo {
	  struct CLMail_SendInfo *next;
      struct Box *box;   /* Box */
      ULONG  flags;      /* noch NULL */
} CLMail_SendInfo;

typedef struct CLMail_BoxInfo {
	char   boxname[ 40 ];					/* Copy of boxname */
	struct Box *box;						/* Backlink for easy access */
    ULONG  lockers;							/* how many times box is locked */
	time_t data;							/* newest data */
	ULONG  out_mail,						/* currently outgoing mail */
           out_news,						/* currently outgoing news */
		   out_batched,						/* outgoing batched data */
           out_files;						/* outgoing other data (e.g. filereqs) */
	ULONG  out_accu;						/* outgoing data accumulated */
	ULONG  reserved[ 32 ];
} CLMail_BoxInfo;

/* Tags for send */

#define CLMAILTAG_BASE (TAG_USER)
#define CLMAILTAG_MEMORY (CLMAILTAG_BASE + 1)
#define CLMAILTAG_LENGTH (CLMAILTAG_BASE + 2)
#define CLMAILTAG_FILE	 (CLMAILTAG_BASE + 3)
#define CLMAILTAG_OFFSET (CLMAILTAG_BASE + 4)
#define CLMAILTAG_FILENAME (CLMAILTAG_BASE + 5)

#ifndef NOPRAG

#pragma libcall CLMailBase CLMAIL_SendA 1e A9803
#pragma tagcall CLMailBase CLMAIL_SendTags 1e A9803
#pragma libcall CLMailBase CLMAIL_AllocFile 24 0802
#pragma libcall CLMailBase CLMAIL_FreeFile 2a 801
#pragma libcall CLMailBase CLMAIL_FlushAll 30 0
#pragma libcall CLMailBase CLMAIL_Batch 36 0802
#pragma libcall CLMailBase CLMAIL_BoxInfo 3c 9802
#pragma libcall CLMailBase CLMAIL_HandleTrash 42 801
#pragma tagcall CLMailBase CLMAIL_HandleTrashTags 42 801
#pragma libcall CLMailBase CLMAIL_TestNetLogin 48 801
#pragma libcall CLMailBase CLMAIL_HandleNetcall 4e A9803
#pragma libcall CLMailBase CLMAIL_DoNetcall 54 B10A9806
#pragma libcall CLMailBase CLMAIL_QuerySpecial 5a 0

extern struct Library *CLMailBase;

int CLMAIL_SendA( struct CLMail_SendInfo *, HH, struct TagItem * );
int CLMAIL_SendTags( struct CLMail_SendInfo *, HH, ... );
void CLMAIL_AllocFile( struct Box *, int );
void CLMAIL_FreeFile( struct Box * );
void CLMAIL_FlushAll( void );
void CLMAIL_Batch( struct Box *box, int flags );
void CLMAIL_BoxInfo( struct Box *box, CLMail_BoxInfo *boxinfo );
int CLMAIL_HandleTrash( struct TagItems * );
int CLMAIL_HandleTrashTags( ... );
ULONG CLMAIL_TestNetLogin( STRPTR string );
ULONG CLMAIL_HandleNetcall( APTR serhandle, STRPTR portid, struct PortInfo *pi );
ULONG CLMAIL_DoNetcall( APTR serhandle, STRPTR portid, struct Box *box, ULONG retries, ULONG flags, struct PortInfo *pi );
STRPTR CLMAIL_QuerySpecial( void );

#endif

#endif
