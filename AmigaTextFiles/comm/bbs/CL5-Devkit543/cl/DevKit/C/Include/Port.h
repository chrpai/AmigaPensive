/*
 * ConnectLine: Port-Daten
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

#ifndef _CL_Port_H
#define _CL_Port_H

#include <cl/types.h>
#include <cl/clutil.h>
#include <cl/cl_prefs.h>


//
// Port-Daten
//


typedef struct CLPort CLPort;

struct CLPort
{
	// name: NOL-Key
	char		type[32];			// Name des Getties ("Serial", "IP", ...)
	bool		enabled;			// für zeitl. begr. Ausschalten
	ulong		attributes[8];		// Flags (V42, ISDN, ...)
};


//
// Flag-Verwaltung
//

typedef struct CLPortFlags CLPortFlags;

struct CLPortFlags
{
	short		flagmax;
	char		flagname[256][16];
};


//
// PrefsData-Struktur
//


typedef struct PDPorts PDPorts;

struct PDPorts
{
	CLPortFlags	da_portflags, st_portflags;
	NOL			da_portlist, st_portlist;
};


#define PREFSID_PORTS MAKE_ID('M','P','P','L')

#define PTPRT_PORTFLAGS		(0 + TAGT_STANDARD)
#define PTPRT_PORTLIST		(1 + TAGT_NOL)


bool CLP_ReadPortPrefs( PDPorts *prefs );
bool CLP_WritePortPrefs( PDPorts *prefs );
bool CLP_MergePortPrefs( PDPorts *prefs, ULONG which );


#endif
