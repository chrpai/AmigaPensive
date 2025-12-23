/***********************************************************
 *
 * ConnectLine: Cronprefs und -daten
 *
 * Verwalter: Michael Balzer
 * 
 * $RCSfile: Cron.h $
 * $Revision: 1.1 $
 * $Date: 1994/02/19 14:29:09 $
 *
 * $Author: balzer $
 * $Locker: balzer $
 * $State: Exp $
 *
 * $Log: Cron.h $
 * Revision 1.1  1994/02/19  14:29:09  balzer
 * Initial revision
 *
 */

#ifndef _CL_Cron_H
#define _CL_Cron_H

#include <time.h>
#include <CL/Types.h>
#include <CL/cl_prefs.h>


/* Options are saved in subid 0 */

typedef struct CronOptions CronOptions;

struct CronOptions
{
	bool biggui;
};


typedef struct CronTimeDesc CronTimeDesc;

struct CronTimeDesc
{
	char min[128];		/* "*" ODER Liste von Zahlen (00..59)
						   bspw. "00,15,30,45" */
	char std[64];		/* ebenso, nur 00..23 */
	char wtag[16];		/* ebenso, nur 0..6 wobei 0=Sonntag */
	char mtag[64];		/* 01..31 */
	char monat[32];		/* 01..12 */
};


typedef struct CronEvent CronEvent;

struct CronEvent
{
	ulong id;
	
	CronTimeDesc time;

	char type;			/* s.u. */

	union
	{
		struct
		{
			char boxname[128];
			char portname[32];
			ulong tries;
		} netcall;
		
		struct
		{
			ulong type;
			char mask[240];
			ulong days;
			ulong kb;
			ulong backup;
		} crunch;
		
		struct
		{
			char command[256];
		} execute;
		
		struct
		{
			char command[256];
		} arexx;
		
		struct
		{
			char portname[32];
		} offline;
		
		struct
		{
			char portname[32];
		} online;
		
		struct
		{
			char portname[32];
			char text[224];
		} message;
		
	} args;

	long execcnt;
};


/* CronEvent.type */
#define TYPE_NETCALL	0
#define TYPE_CRUNCH		1
#define TYPE_EXECUTE	2
#define TYPE_AREXX		3
#define TYPE_OFFLINE	4
#define TYPE_ONLINE		5
#define TYPE_MESSAGE	6

/* args.crunch.type */
#define CRUNCH_BOARDS	0
#define CRUNCH_PMS		1
#define CRUNCH_MSGIDS	2


/*
 * PrefsData-Struktur
 */

typedef struct PDCronEvent CronEventPrefsData;
typedef struct PDCronEvent PDCronEvent;

struct PDCronEvent
{
	CronEventPrefsData *next;
	CronEvent event, stamp;
};


#define PREFSID_CRON MAKE_ID('M','P','C','R')


#endif
