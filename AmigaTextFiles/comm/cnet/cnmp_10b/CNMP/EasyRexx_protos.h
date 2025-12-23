/*
 *	File:					EasyRexx_protos.h
 *	Description:	
 *
 *	(C) 1994,1995, Ketil Hunn
 *
 */

#ifndef CLIB_EASYREXX_PROTOS_H
#define	CLIB_EASYREXX_PROTOS_H

#ifndef  CLIB_EXEC_PROTOS_H
#include <clib/exec_protos.h>
#endif

#ifndef ER_LIB
#ifndef PRAGMAS_EASYREXX_PRAGMAS_H
#include <pragmas/EasyRexx_pragmas.h>
#endif
#endif

#ifndef ER_LIB
#ifndef EASYREXX_H
#include <libraries/EasyRexx.h>
#endif
#endif

/*** PROTOTYPES **********************************************************************/
__asm __saveds struct ARexxContext *AllocARexxContextA(register __a0 struct TagItem	*taglist);
__asm __saveds void FreeARexxContext(register __a0 struct ARexxContext *context);
__asm __saveds BYTE GetARexxMsg(register __a0 struct ARexxContext *context);
__asm __saveds LONG SendArexxCommandA(register __a1 UBYTE						*command,
																			register __a0 struct TagItem	*taglist);
__asm __saveds void ReplyARexxMsgA(	register __a1 struct ARexxContext *context,
																		register __a0 struct TagItem			*taglist);


/*** PROTOTYPES FOR TAGCALLS *********************************************************/
struct ARexxContext *AllocARexxContext(Tag tag1, ...);
LONG SendARexxCommand(UBYTE *command, Tag tag1, ...);
void ReplyARexxMsg(struct ARexxContext *context, Tag tag1, ...);


/*** PROTOTYPES FOR LINKLIBRARY ******************************************************/
LONG HandleARexxFuncs(struct ARexxContext *context);
																				
#endif
