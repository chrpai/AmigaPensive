#ifndef _CL_CL_MSG_H
#define _CL_CL_MSG_H

#define CLMFT_TAGBASE (TAG_USER+0x8775)

/*
**  Tags for CLMSG_GetIndex()
*/

#define CLMFT_WRITE	(CLMFT_TAGBASE+1)		/* LONGBOOL: Get Index for Write */
#define CLMFT_ATTEMPTONLY (CLMFT_TAGBASE+2)	/* LONGBOOL: Attempt to alloc index, fail if in use */
#define CLMFT_USER (CLMFT_TAGBASE+26)		/* LONGBOOL: Obj really is Pointer to User */

/*
**  Tags for CLMSG_Crunch()
*/

#define CLMFT_GAUGE		(CLMFT_TAGBASE+27)		/* APTR: MUI gauge object to update */
#define CLMFT_APP		(CLMFT_TAGBASE+30)		/* APTR: MUI app object to do Input_Buffered on */
#define CLMFT_HOLD_DAYS (CLMFT_TAGBASE+28)		/* ULONG: Hold days */
#define CLMFT_HOLD_KB	(CLMFT_TAGBASE+29)		/* ULONG: Hold KB */

/*
**  Attributes for CLMSG_Set() / CLMSG_Get()
*/

#define CLMSGA_NUMBER 		(CLMFT_TAGBASE+3)	/* [..G] ULONG Current msg number */
#define CLMSGA_NUMBEROFMSGS 	(CLMFT_TAGBASE+30)	/* [..G] ULONG number of messages */
#define CLMSGA_SUBJECT 		(CLMFT_TAGBASE+4)	/* [.SG] STRPTR Subject of msg */
#define CLMSGA_FILENAME 	(CLMFT_TAGBASE+5)	/* [.SG] STRPTR Filename of msg */
#define CLMSGA_SHORTCOMMENT (CLMFT_TAGBASE+6)	/* [.SG] STRPTR Comment */
#define CLMSGA_FROM			(CLMFT_TAGBASE+7)	/* [.SG] STRPTR From */
#define CLMSGA_MSGID		(CLMFT_TAGBASE+8)	/* [.SG] STRPTR MessageID */
#define CLMSGA_INCOMDATE 	(CLMFT_TAGBASE+9)	/* [.SG] time_t Income date */
#define CLMSGA_SENDDATE 	(CLMFT_TAGBASE+10)	/* [.SG] time_t Send date */
#define CLMSGA_READCOUNT 	(CLMFT_TAGBASE+11)	/* [.SG] ULONG read count */
#define CLMSGA_FLG_BIN		(CLMFT_TAGBASE+12)	/* [.SG] LONGBOOL msg is binary */
#define CLMSGA_FLG_NET		(CLMFT_TAGBASE+13)	/* [.SG] LONGBOOL msg arrived via net */
#define CLMSGA_FLG_HOLD		(CLMFT_TAGBASE+14)	/* [.SG] LONGBOOL do not delete msg */
#define CLMSGA_FLG_DEL		(CLMFT_TAGBASE+15)	/* [.SG] LONGBOOL msg was deleted */
#define CLMSGA_FLG_ERROR	(CLMFT_TAGBASE+16)	/* [ISG] LONGBOOL error report */
#define CLMSGA_FLG_CENSORED	(CLMFT_TAGBASE+17)	/* [ISG] LONGBOOL msg has been censored */
#define CLMSGA_FLG_NO_RATIO	(CLMFT_TAGBASE+18)	/* [.SG] LONGBOOL don't account for U/L ratio */

#define CLMSGA_BODY_LEN		(CLMFT_TAGBASE+19)	/* [ISG] ULONG length of msg body */
#define CLMSGA_BODY_FILE	(CLMFT_TAGBASE+20)	/* [ISG] STRPTR body from/to file	*/
#define CLMSGA_BODY_MEM		(CLMFT_TAGBASE+21)	/* [ISG] STRPTR body from/to memory. MUST SPECIFY CLMSGA_BODY_LEN! */

#define CLMSGA_COMMENT_LEN	(CLMFT_TAGBASE+22)	/* [ISG] ULONG length of msg comment */
#define CLMSGA_COMMENT_FILE	(CLMFT_TAGBASE+23)	/* [ISG] STRPTR comment from/to file	*/
#define CLMSGA_COMMENT_MEM	(CLMFT_TAGBASE+24)	/* [ISG] STRPTR comment from/to memory. MUST SPECIFY CLMSGA_COMMENT_LEN! */

#define CLMSGA_HEADER		(CLMFT_TAGBASE+25)	/* [ISG] HH Header to/from headerlist */

#define CLMSGA_STORAGE_FILENAME (CLMFT_TAGBASE+31)	/* [..G] External Filename. NULL if not available */

/*
**  Positions for CLMSG_Move()
*/

#define CLMSG_MOVE_NEXT 1		/* Move to next mail */
#define CLMSG_MOVE_PREV 2		/* Move to previous mail */
#define CLMSG_MOVE_GOTO 3		/* Move to specified index number */
#define CLMSG_MOVE_MSGID 4		/* Move to specific msgid */
#define CLMSG_MOVE_NEWER 5		/* Move to newer mail */

/*
**	Flags for CLMSG_Store
*/

#define CLMSG_STORE_FILE 1		/* source is file instead of buffer */
#define CLMSG_STORE_BIN 2		/* Store as binary file */
#define CLMSG_STORE_ISISO 4		/* Msg contents are ISO-8859-1 */

/*
**	Error codes
*/

#define CLMSG_ERR_NOMSG 1					/* Get(): Index not set to a valid msg */
#define CLMSG_ERR_IX_ACCESS 2				/* error occured during (valid) index access */
#define CLMSG_ERR_MESSAGE_ACCESS 3			/* error occured during (valid) MESSAGE access */
#define CLMSG_ERR_EXTERNAL_FILE_ACCESS	4 	/* Error while accessing external file */
#define CLMSG_ERR_OUTBOUND_ACCESS	5 		/* Error while accessing outbound file */
#define CLMSG_ERR_BAD_ARGS 			6 		/* bad args submitted to some function */
#define CLMSG_ERR_PATH_ACCESS 		7 		/* Couldn't access board or user path */
#define CLMSG_ERR_EXTERNAL			8 		/* Requested operation not possible on external Index */
#define CLMSG_ERR_TEMP_ACCESS		9 		/* Error regarding handling temporary files (Crunch) */
#define CLMSG_ERR_EXTERNAL_ACCESS	10 		/* Error accessing external file */

#ifndef _NO_PRAG
extern struct Library *CLMBase;
#pragma libcall CLMBase CLMSG_GetIndex 1e 9802
#pragma tagcall CLMBase CLMSG_GetIndexTags 1e 9802
#pragma libcall CLMBase CLMSG_FreeIndex 24 801
#pragma libcall CLMBase CLMSG_GetA 2a 9802
#pragma tagcall CLMBase CLMSG_Get 2a 9802
#pragma libcall CLMBase CLMSG_SetA 30 9802
#pragma tagcall CLMBase CLMSG_Set 30 9802
#pragma libcall CLMBase CLMSG_Move 36 10803
#pragma libcall CLMBase CLMSG_Store 3c 21A90806
#pragma libcall CLMBase CLMSG_Crunch 42 9802
#pragma tagcall CLMBase CLMSG_CrunchTags 42 9802
#endif

typedef APTR CLMSGIX;

#ifndef _NO_PROTO
CLMSGIX CLMSG_GetIndex( APTR userboard, struct TagItem *tags );
CLMSGIX CLMSG_GetIndexTags( APTR userboard, ... );
void CLMSG_FreeIndex( CLMSGIX index );
LONG CLMSG_GetA( CLMSGIX index, struct TagItem *values );
LONG CLMSG_Get( CLMSGIX index, ... );
LONG CLMSG_SetA( CLMSGIX index, struct TagItem *values );
LONG CLMSG_Set( CLMSGIX index, ... );
LONG CLMSG_Move( CLMSGIX index, LONG where, LONG position );
LONG CLMSG_Store( CLMSGIX index, ULONG flags, HH headerlist, APTR source, ULONG sourcelen, ULONG commentlen );
LONG CLMSG_Crunch( CLMSGIX index, struct TagItem *tags );
LONG CLMSG_CrunchTags( CLMSGIX index, ... );
#endif

#endif
