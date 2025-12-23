/*
 *	File:					EasyRexx.h
 *	Description:	Headerfile for the easyrexx.library
 *
 *	(C) 1994,1995, Ketil Hunn
 *
 *	Set Tab=2 for best readability
 *
 */

#ifndef LIBRARIES_EASYREXX_H
#define LIBRARIES_EASYREXX_H

#ifndef	EXEC_PORTS_H
#include <exec/ports.h>
#endif

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

#define	EASYREXXNAME		"easyrexx.library"
#define	EASYREXXVERSION	1L

#define	ER_SIGNAL(c)		(1L<<c->port->mp_SigBit)
#define ARG(c,i)				(c->argv[i])
#define	ARGNUMBER(c,i)	(*((LONG *)c->argv[i]))
#define	ARGSTRING(c,i)	((UBYTE *)c->argv[i])
#define	ARGBOOL(c,i)		(c->argv[i]==NULL ? FALSE:TRUE)
#define	TABLE_END				NULL,NULL,NULL,NULL

/***** STRUCTS ***********************************************************************/

struct ARexxCommandTable
{
	LONG	id;
	UBYTE *command,
				*template;
	APTR userdata;
};

struct ARexxContext
{
	/* PRIVATE */
	struct MsgPort						*port;
	struct ARexxCommandTable	*table;
	UBYTE											*argcopy,
														*portname,
														maxargs;
	struct RDArgs							*rdargs;
	struct RexxMsg						*msg;
	ULONG											flags;

	/* PUBLIC */
	LONG											id,
														argv[0];
};


#ifndef CLIB_EASYREXX_PROTOS_H
#include <clib/easyrexx_protos.h>
#endif

#ifndef REXX_ERRORS_H
#include <rexx/errors.h>
#endif

/***** TAGS **************************************************************************/

#define	ER_TagBase				(TAG_USER)
#define	ER_Portname				(ER_TagBase+1)	/* Name of AREXX port											*/
#define	ER_CommandTable		(ER_TagBase+2)	/* Table of supported AREXX commands			*/
#define	ER_ReturnCode			(ER_TagBase+3)	/* Primary result (return code)						*/
#define	ER_Result					(ER_ReturnCode)	/* Alias for ER_ReturnCode								*/
#define	ER_Result1				(ER_ReturnCode)	/* Alias for ER_ReturnCode								*/
#define	ER_Result2				(ER_TagBase+4)	/* Secondary result (string)							*/
#define	ER_Port						(ER_TagBase+5)	/* Use already created port								*/
#define	ER_ResultString		(ER_TagBase+6)	/* Secondary result (string)							*/
#define	ER_ResultLong			(ER_TagBase+7)	/* Secondary result (long)								*/

#endif
