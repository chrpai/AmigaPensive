
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
