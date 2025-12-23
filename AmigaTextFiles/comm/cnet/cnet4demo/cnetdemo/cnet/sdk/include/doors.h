
#ifndef CNET_DOORS
#define CNET_DOORS

#define CN_USERINPUT 0xff		// Message send to doors when user keyboard input
                         		// is waiting.
                         		// see SignalInputInit() in empty.c 

struct CPort {						// setup when a C pfile is ran
	struct	MsgPort  mport;	// 0
	short	align;
	struct	MainPort *myp;		// 36 pointer to MainPort
	struct	PortData *zp;		// 40 pointer to my PortData

	UBYTE	ack;						// 44 tell CNet we've arrived
	UBYTE	align2[3];
};

struct CMessage {						// passed between C pfile and C-Net
	struct	Message cn_Message;	// 0 communication stuff
	ULONG	arg1;							// 20
	ULONG	arg2;
	ULONG	arg3;
	ULONG	arg4;
	ULONG	result;
	UBYTE	command;						// 40 which command? see CNet Programmer's Reference Manual
	UBYTE	align[3];
};

#endif
