#ifndef CNET_DCC_H
#define CNET_DCC_H

// irc-ii likes this size so we might as well use it
#define BIG_BUFFER_SIZE 1024

// information about DCC file in progress
struct DCCFile
	{
	char pathfile[FMSIZE];	// full path/filename of sent/received file
	BPTR fh;						// file handle of open file or 0 if none open
	ULONG txfer_bytes;		// bytes transferred so far
	ULONG start_time;			// local start time - reference for calculating CPS
	ULONG ETA;					// time to completion
	USHORT last_CPS;			// last calculated CPS
	};

#endif
