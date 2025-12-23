/*
 * ConnectLine: Serial-Getty-Daten
 *
 * Verwalter: Michael Balzer
 * 
 * $RCSfile$
 * $Revision$
 * $Date$
 *
 * $Author$
 * $Locker$
 * $State$
 *
 * $Log$
 */

#ifndef _CL_SerialGetty_H
#define _CL_SerialGetty_H

#include <cl/types.h>
#include <cl/clutil.h>
#include <cl/cl_prefs.h>


#define PREFSID_SERIALGETTY MAKE_ID('M','P','G','S')


#define PTSERGET_PORTLIST		(0 + TAGT_NOL)

struct PDSerialGetty
{
	char	devicename[32];
	ulong	unit;

	ulong	serflags;			// Shared, 7Wire, ...
	ulong	baudrate;
	ulong	buffersize;
	
	char	cmd_init[256];
	char	cmd_dial[32];
	char	cmd_hangup[32];
	char	cmd_escape;
	
	char	txt_connect[32];
	char	txt_busy[32];
	char	txt_voice[32];
	char	txt_ok[32];
	char	txt_ring[32];
	char	txt_nocarrier[32];
};


#endif
