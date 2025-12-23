/***********************************************************
 *
 * ConnectLine: Serverprefs und -daten
 *
 * Verwalter: Michael Balzer
 * 
 * $RCSfile: Server.h $
 * $Revision: 1.1 $
 * $Date: 1994/02/19 17:05:14 $
 *
 * $Author: balzer $
 * $Locker: balzer $
 * $State: Exp $
 *
 * $Log: Server.h $
 * Revision 1.1  1994/02/19  17:05:14  balzer
 * Initial revision
 *
 */

#ifndef _CL_Server_H
#define _CL_Server_H

#include <cl/types.h>
#include <time.h>
#include <cl/cl_prefs.h>


typedef struct CallLogNode CallLogNode;

struct CallLogNode
{
	char	name[40];		/* Name des Anrufers */
	ulong	portid;			/* an welchem Port */
	time_t	logouttime;		/* wann war der fertig */
};


typedef struct CLServer CLServer;

struct CLServer
{
	ulong	DayCntCalls;
	ulong	TotCntCalls;
	
	ulong	DayCntDownloads;
	ulong	TotCntDownloads;
	
	ulong	DayCntUploads;
	ulong	TotCntUploads;

	ulong	CntNews;
	ulong	CntMails;
	ulong	CntRegistrations;
	ulong	CntSysopMails;
};

// aus Performancegründen separate PrefsIDs
#define PREFSID_SERVER		MAKE_ID('M','P','S','M')
#define PREFSID_USERLOG		MAKE_ID('M','P','S','u')
#define PREFSID_SYSTEMLOG	MAKE_ID('M','P','S','s')
#define PREFSID_POINTLOG	MAKE_ID('M','P','S','p')

#define PTSRV_PRIMARY		(0 + TAGT_STANDARD)
#define PTSRV_CALLLOG		(0 + TAGT_NOL)


typedef struct PDServer PDServer;

struct PDServer
{
	CLServer	server, serverstamp;
	NOL			userlog, ulogstamp;
	NOL			systemlog, slogstamp;
	NOL			pointlog, plogstamp;
};


#endif
