
struct JobType {
	ULONG	date,
         valid,
         repeat;

	ULONG	memo,
         length;

	USHORT event,
          type;

	char	args[80];
	char	ports[40];

	UBYTE	exclude_days,
         status,
         deleted;
};


// updated for v4.12d
struct JobType4 {
	char		Name[40];				// Event identifier/Name - for user recognition - MUST be unique to other event names !
	char		args[128];				// command to execute
	char		ports[40];				// CNet port range string (ie. 1-3,5,7)

	USHORT	invoke;					// Immediate, etc,.. see INVOKETYPE_* below
	USHORT	type;						// Event Type - see EVENT_* below

	UBYTE		status;					// ready, disabled, etc.  see EVENTSTATUS_* below

	ULONG		StartTime;				// Date to start event - includes start time & date (since midnight, Jan-01-1970)
	ULONG		DateExecuted;			// time event last executed (packed LONG value in seconds since Jan-01-1970)
	ULONG		valid;					// how long (in seconds) is event valid after ExeTime
	LONG		Days;						// What days to perform events - Bits 0-6 = Sun-Sat

	UBYTE		deleted;					// TRUE if event deleted/obsolete - only deleted if the event is an "execute once/delete" event

	struct	JobType4	*NextJob;	// next job/event in list or NULL if none

	ULONG		repeat;					// when to repeat event - days, hours & minutes expressed in seconds
	UBYTE		runport;					// 0=do not run port, 1=run port if not loaded and leave running, 2=run port and then close after door execution
};


// event types
#define EVENT_RUNCNETC	0
#define EVENT_RUNAREXX	1
#define EVENT_RUNDOS		2
#define EVENT_READFILE	3
#define EVENT_DOSCMD		4
#define EVENT_CLOSEPORT	5
#define EVENT_CHARGES	6
#define EVENT_LOGONBPS	7
#define EVENT_DLOADBPS	8
#define EVENT_ULOADBPS	9
#define EVENT_LOGONACC	10
#define EVENT_XFERSACC	11
#define EVENT_PFILEACC	12
#define EVENT_MODEMNUM	13
#define EVENT_CALLBACK	14
#define EVENT_AVALIDNUM	15
#define EVENT_SYSOPIN	16
#define EVENT_UDBASE		17
#define EVENT_BASE		18
#define EVENT_NEWUSERS	19
#define EVENT_PFILES		20
#define EVENT_JOINLINK	21
#define EVENT_ONLINE		22

// event status
#define EVENTSTATUS_READY     0x0000
#define EVENTSTATUS_ACTIVE    0x0001
#define EVENTSTATUS_HOLD      0x0002
#define EVENTSTATUS_CANCELLED 0x0003

// when/how to invoke event
#define INVOKETYPE_IMMEDIATE	0x0001
#define INVOKETYPE_FORCEIDLE	0x0002
#define INVOKETYPE_OFFLINE		0x0004
#define INVOKETYPE_ONLINE		0x0008
