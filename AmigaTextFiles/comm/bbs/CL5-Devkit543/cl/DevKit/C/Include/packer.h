#ifndef _CL_PACKER_H
#define _CL_PACKER_H

//
//	cl/packer.h
//  -----------
//
//  definitions for netio packer preferences
//

#include <cl/cl_prefs.h>
#include <cl/cl_mui.h>

#define PACKER_PREFSID MAKE_ID('O','P','A','C')
#define PACKER_PREFSSUB (TAGT_NOL|765)

//
// Substitutors:
//
//  %a archive name
//  %A archive name without extension
//  %n name (or list of names) of files to add to or extract from the archive
//  %u unique filename (unpack only)
//  %d destination directory (unpack only, set to pr_CurrentDir)
//
//
//	ID-String works as follows: Entries are seperated by ","
//	? 		 matches any char
//  'a' 	 matches given character (or sequence of)
//  "a"		 dito, but case insensitve
//  $hh		 matches character (hex)
//  num		 matches character (dec)


typedef struct clpacker_prefs {
	char 	cmd_pack[ 256 ];
	char	cmd_unpack_all[ 256 ];
	char	def_extension[ 16 ];	// default extension of archive files
	UBYTE	flg_archive;			// archive files can contain more than one file
	UBYTE	flg_reserved[ 3 ];
	ULONG	stack;
	UWORD	errc_pack,				// Error return code level
			errc_unpack_all;
	char	id[ 256 ];				// ID-String to recognize packer
	char	cmd_test[ 256 ];		// Command to test archive
	char	testfailpattern[ 256 ];	// Output pattern to Match for failed archive
} clpacker_prefs;

#endif
