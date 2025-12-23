//
// CONNECTLINE 5.0 New Maps, cl_maps.h
//
// Copyright © 1996 by Mathias Mischler
// All Rights Reserved
//

#define MAPS_PREFSID MAKE_ID('M','A','P','S')
#define MAPS_PREFSSUB (TAGT_NOL|123)

struct maps_cmd
{
	char 	exec[ 256 ];
	long	minpri;
	UBYTE	user_cmd;
};

#define MAPSACCESS_SYSOP 0
#define MAPSACCESS_LOKAL 1
#define MAPSACCESS_ALL   2
#define MAPSACCESS_NONE  3
