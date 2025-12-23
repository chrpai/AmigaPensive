//
// ConnectLine: Allgemeine Systemprefs und -daten
//
// $RCSfile: System.h $
// $Revision: 1.1 $
// $Date: 1994/02/19 17:05:14 $
//
// $Author: balzer $
// $Locker: balzer $
// $State: Exp $
//
// $Log: System.h $
// Revision 1.1  1994/02/19  17:05:14  balzer
// Initial revision
//
//

#ifndef _CL_System_H
#define _CL_System_H

#include <cl/types.h>
#include <cl/cl_prefs.h>
#include <cl/clutil.h>
#include <time.h>


//
// Kerndaten
//

typedef struct CLSystem CLSystem;

struct CLSystem
{
	char	SystemName[64];
	char	SystemPasswd[64];
	
	// Flags

	UBYTE	NoSysopChat;			// SysOp will nicht Chatten

	// Daten

	char	OfflineEditor[256];		// Texteditor für Offline-Logins

	char	NewRegistrations[256];	// EMP:

	char	ExportPathBin[256];
	char	ExportPathText[256];

	char	SystemLocation[32];
	char	SystemOwner[64];
	char	SystemSnailmail[128];
	char	SystemVoicephone[64];
	char	SystemOrganization[256];

	time_t	LoginBlockTime;			// Blockzeit nach falschem Passwd
};


//
// Zeitzonen
//

typedef struct TimeZone TimeZone;

struct TimeZone
{
	// Bezeichnung
	char	name[8];
	
	// Zeitverschiebung ggü. GMT in Minuten
	long	gmt_offset;
	
	// Beginn
	ulong	month;
	ulong	day;
	ulong	hour;
};

typedef struct CLTimeZones CLTimeZones;

struct CLTimeZones
{
	TimeZone	summer;
	TimeZone	winter;
};


//
// Tags
//

#define PREFSID_SYSTEM MAKE_ID('M','P','S','Y')

#define PTSYS_PRIMARY		(0 + TAGT_STANDARD)		// Kerndaten
#define PTSYS_DOMAINLIST	(1 + TAGT_NOL)			// Liste der Domains der Box
#define PTSYS_TIMEZONES		(2 + TAGT_STANDARD)		// Zeitzonen


//
// PrefsData-Struktur
//

typedef struct PDSystem PDSystem;

struct PDSystem
{
	CLSystem		da_primary, st_primary;
	NOL				da_domainlist, st_domainlist;
	CLTimeZones		da_timezones, st_timezones;
};


bool CLP_ReadSystemPrefs( PDSystem *prefs );
bool CLP_WriteSystemPrefs( PDSystem *prefs );
bool CLP_MergeSystemPrefs( PDSystem *prefs, ulong subtag );


#endif
