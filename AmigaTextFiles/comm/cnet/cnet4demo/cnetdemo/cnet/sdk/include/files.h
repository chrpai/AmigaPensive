
#ifndef CNET_FILES
#define CNET_FILES

struct FileListType {
	ULONG	key, date;

	char	subject[80], path[80];

	ULONG	area, uploader;
	ULONG	text, length;			// short description
	ULONG	size, downloads, bestcps;
	ULONG	used, DLableFrom, DLableTo, PurgeOn;

	long	ByteRewards;			// # of bytes awarded so far
	long	BytePayBack;
	long	ByteCharges;
	long	ByteDownload;			// Number of byte cred to download

	long	FileRewards;			// # of files awarded so far
	short	FilePayBack;
	short	FileCharges;
	short	FileDownload;			// Number of file cred to download

	UBYTE	DLnotifyULer;
	UBYTE	VirusChecked;

	UBYTE	override;
	UBYTE	Free;
	UBYTE	Favorite;
	UBYTE	Validated;

	UBYTE	Finished;
	UBYTE	Transformed;
	UBYTE	PurgeKill;
	UBYTE	MissingFile;

	BYTE	Integrity;			//	0 == not tested
									//	1 == passed
					       		//	-1 == failed
									//	2 == not testable

	UBYTE	PurgeStatus;		//	0 == Auto
									//	1 == @DL
									//	2 == Query@DL
									//	3 == @Amaint
									//	4 == Protected
	UBYTE	deleted;
};

struct FileKey {
	ULONG	index;
	ULONG	key;
	ULONG	area;					//	for access during searchs!
};

struct FileKeyNode {
	struct	FileKey fk[512];
};

struct FileListBase {
	struct	SignalSemaphore sem;
	char	portName[24];
	char	path[80];

	struct	FileKeyNode *fkn[256];	//	enough for 128K files */
	ULONG	nf;
	USHORT	numlocks;
	UBYTE	savekeys;
	UBYTE	exclusive;
};

struct FileListHandle {
	struct	FileListType file;

	struct	FileListBase *flb;
	struct	UserKey *uk;
	struct	FileKey *fk;

	BPTR	fh, sh;
	ULONG	pf, cf;

	char	path0[80];
};

#endif
