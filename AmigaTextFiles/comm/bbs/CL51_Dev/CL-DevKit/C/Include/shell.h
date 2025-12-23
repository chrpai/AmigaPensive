#ifndef _CL_SHELL_H
#define _CL_SHELL_H

//
//	cl/shell.h
//  -----------
//
//  definitions for shell command preferences
//

#include <cl/cl_prefs.h>
#include <cl/cl_mui.h>

#define SHELL_PREFSID MAKE_ID('S','H','C','M')
#define SHELL_PREFSSUB (TAGT_NOL|765)

typedef struct clshell_cmd {
	char	exec[ 128 ];		// Execute or Alias
	UBYTE	type;
	UBYTE	minpri;				// Minimum priority
	UBYTE	flg_log;			// Log access
	UBYTE	log_pri;
	UBYTE	flg_ser;
	UBYTE   reserved[ 3 ];
	char	helpfilenode[ 80 ]; // Format: file/node, path: CONNECTLINE:Online/<Language>/Help/file...
} clshell_cmd;

#define CLSC_DOS 0
#define CLSC_ALIAS 1

#endif
