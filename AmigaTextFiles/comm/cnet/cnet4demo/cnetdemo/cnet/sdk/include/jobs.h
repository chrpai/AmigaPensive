
#include "EVENTDEFS.H"

struct JobType {
	ULONG	date, valid, repeat;
	ULONG	memo, length;

	USHORT	event, type;
	char	args[80];
	char	ports[40];

	UBYTE	exclude_days, status, deleted;
};


/* NEW for v4.12 - May-16-96 */
struct NewJobType {
	char		Name[40];		// Event identifier/Name - for user recognition - MUST be unique to other event names !
	char		args[128];		// command to execute
	char		ports[40];		// packed CNet port range string (ie. 1-3,5,7)

	USHORT	invoke;			// Immediate, etc,.. see EVENTDEFS.H for explanation
	USHORT	type;				// Event Type - CNetC, ARexx, LogonBPS, etc,. see EVENTDEFS.H

	UBYTE		status;			// ready, disabled..

	// see include:time.h for time_t and tm attributes
	// all time_t values are converted to tm structures before comparison with critical times

	ULONG		ExeTime;						// time at which to execute event
	ULONG		DateExecuted;				// time event last executed (packed time_t value) - cnet only regards the "tm_day" of this value
	ULONG		valid;						// how long (in seconds) is event valid after ExeTime
	LONG		Days;							// What days TO PERFORM events - Bits 1-7 = Sun-Sat

	UBYTE		deleted;						// TRUE if event deleted/obsolete - only deleted if the event is an "execute once/delete" event or if there is no repeat time

	struct	NewJobType	*NextJob;	// next job/event in list or NULL if none
};


// updated for v4.12d
struct JobType4 {
	char		Name[40];				// Event identifier/Name - for user recognition - MUST be unique to other event names !
	char		args[128];				// command to execute
	char		ports[40];				// CNet port range string (ie. 1-3,5,7)

	USHORT	invoke;					// Immediate, etc,.. see EVENTDEFS.H for explanation
	USHORT	type;						// Event Type - CNetC, ARexx, LogonBPS, etc,. see EVENTDEFS.H

	UBYTE		status;					// ready, disabled..

	ULONG		StartTime;				// Date to start event - includes start time & date (since midnight, Jan-01-1970)
	ULONG		DateExecuted;			// time event last executed (packed LONG value in seconds since Jan-01-1970)
	ULONG		valid;					// how long (in seconds) is event valid after ExeTime
	LONG		Days;						// What days to perform events - Bits 0-6 = Sun-Sat

	UBYTE		deleted;					// TRUE if event deleted/obsolete - only deleted if the event is an "execute once/delete" event

	struct	JobType4	*NextJob;	// next job/event in list or NULL if none

	ULONG		repeat;					// when to repeat event - days, hours & minutes expressed in seconds
	UBYTE		runport;					// 0=do not run port, 1=run port if not loaded and leave running, 2=run port and then close after door execution
};
