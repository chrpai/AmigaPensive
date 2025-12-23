/***************************************************************************/
/*						Traplist.library::Defined extra tag IDs						*/
/*-------------------------------------------------------------------------*/
/*						  © Copyright 1992-94 by Martin J. Laubach					*/
/*									All rights reserved										*/
/*																									*/
/***************************************************************************/
/*
 * $Header: Susan:nl/RCS/nltags.h,v 1.2 93/08/06 22:20:08 mjl Exp $
 */
/*-------------------------------------------------------------------------*/
#ifndef NL_NLTAGS_H
#define NL_NLTAGS_H 1

#ifndef MAKE_ID
#define	MAKE_ID(a,b,c,d)	\
	((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

/* Defined by TrapDoor Developement, 2:310/6 */
#define ID_PASS	MAKE_ID('P','A','S','S')	/* Password entry   */
#define ID_TCNF	MAKE_ID('U','C','N','F')	/* TD config entry  */
#define ID_ACCT	MAKE_ID('A','C','C','T')	/* TD accounting entry */

/* Defined by Martin Kuhne, 2:246/142.71, used in FFRS */
#define ID_FFRS   MAKE_ID('F','F','R','S')	/* Private binary data */
#define ID_GROUP  MAKE_ID('G','R','O','U')	/* Private binary data */

/* Defined by Mark Cassidy, 2:230/611, used in FServ */
#define ID_FSUR	MAKE_ID('F','S','U','R')

/* Defined by Fredrik Kjellberg, 2:203/602.416, used in TrapSched */
#define ID_DELI	MAKE_ID('D','E','L','I')
#define ID_POLL	MAKE_ID('P','O','L','L')

/* Defined by Brian Ipsen, 2:230/813.97, used in GizmoTick */
#define ID_GZTC	MAKE_ID('G','Z','T','C')

#endif

/*-------------------------------------------------------------------------*/
/* ID: 4.40@1538  Last Changed: 28 May 1994 15:40:30 by max */
