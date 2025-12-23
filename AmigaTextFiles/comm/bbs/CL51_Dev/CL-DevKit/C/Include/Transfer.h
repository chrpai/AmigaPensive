/***********************************************************
 *
 * ConnectLine: Datentransfer
 *
 * Verwalter: Oliver Wagner
 * 
 * $RCSfile: Transfer.h $
 * $Revision: 1.1 $
 * $Date: 1994/02/19 17:05:14 $
 *
 * $Author: balzer $
 * $Locker: balzer $
 * $State: Exp $
 *
 * $Log: Transfer.h $
 * Revision 1.1  1994/02/19  17:05:14  balzer
 * Initial revision
 *
 */

#ifndef _CL_Transfer_H
#define _CL_Transfer_H

#include <exec/types.h>
#include <cl/cl_prefs.h>


#define PREFSID_XPRINFO MAKE_ID('O','X','P','R')

struct CLXPRInfo
{
	char	ProtocolName[12];
	char	ProtocolComment[40];
	char	Library[32];
	char	Options[64];
	short	Efficiency;				/* in Prozent, für Zeitberechnung */
	ULONG	Flags;
};


#endif
