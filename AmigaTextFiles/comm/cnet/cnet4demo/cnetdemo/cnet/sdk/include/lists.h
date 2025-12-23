
struct ListItem {
	short	Version;					// should be 225
	char	Path       [54];		//   2
	char	Description[64];		//  56
	char	Arguments  [64];		// 120

	long	Access;					// 184
	long	UnionFlags;				// 188

	struct	IsDate Date;		// 192

	short	SCharge;					// 198 accounting use-of-item charge

	UBYTE	Subdirectory_NOT;		// 200
	UBYTE	FifoFlags;				// 201 1==RAW startup
	UBYTE	ReadFlags;				// internal only
	UBYTE	Environ;

	UBYTE	DailyPfileTime;		// 204 subtract from daily pfile time?
	UBYTE	Disabled;				// 205
	UBYTE	Type;						// 206 not really used yet
	UBYTE	PurgeKill;				// 207 Delete item when purged?

	UBYTE	Expansion[14];			// 208

	struct	IsDate PurgeDate;	// 222
};

/* Length== 176 */

struct OldListItem {
	char	Path       [51];	// 0
	char	Description[61];	// 51
	char	Arguments  [20];

	struct	IsDate Date;	// 132

	long	Access;				// 138 un-al

	UBYTE	Subdirectory;		// 142
	UBYTE	DaysToKeep;
	UBYTE	ReadFlags;			// internal only
	UBYTE	Environ;

	short	SCharge;				// 146 override a subdir schedule

	long	UnionFlags;

	UBYTE	DailyPfileTime;	// subtract from daily pfile time?

	UBYTE	Expansion[23];		// 148
};

// LONG aligned.  Length==48
