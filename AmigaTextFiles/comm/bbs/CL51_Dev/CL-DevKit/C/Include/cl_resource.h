/*
**
**	Global CL Server Resource (private)
**
*/

struct clresource {
	struct Library lib;
	time_t boottime;

	ULONG telnetallowed;
};

#define CLRESNAME "Connectline-RESIDENT" 
