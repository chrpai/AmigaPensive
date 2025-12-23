#ifndef NOVIA_DATE_H
#define NOVIA_DATE_H
#define NOVIA_DATE_H_VERSION "$VER: 0.01 (08.05.1998)"
// (c) Copyright 1996-1998 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define DATEFLAG_USEDATE	1
#define DATEFLAG_USETIME	2

struct Date {
	ULONG	datecode;	// 4
	WORD	year;		// 6
	BYTE	month;		// 7
	BYTE	day;		// 8
	BYTE	hours;		// 9
	BYTE	mins;		// 10
	BYTE	secs;		// 11
	BYTE	weekday;	// 12
};

#endif