#ifndef NOVIA_PORTDATA_H
#define NOVIA_PORTDATA_H
#define NOVIA_PORTDATA_H_VERSION $VER: 0.18 (21.02.1999)
// V0.10 06.08.1998 (c) Copyright 1996-1998 by Thorsten Gehler
// V0.11 06.08.1998 (c) Copyright 1996-1998 by Thorsten Gehler
// V0.12 06.08.1998 (c) Copyright 1996-1998 by Thorsten Gehler
// V0.15 20.08.1998 (c) Copyright 1996-1998 by Thorsten Gehler		!!! new PortData Structure  !!!
// V0.16 22.08.1998 (c) Copyright 1996-1998 by Thorsten Gehler
// V0.17 17.10.1998 (c) Copyright 1996-1998 by Thorsten Gehler
// V0.18 21.02.1999 (c) Copyright 1996-1999 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.


#include <novia/novia_global.h>

#ifndef NOVIA_USERDATA_H
#include <novia/novia_userdata.h>
#endif

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

#ifndef EXEC_IO_H
#include <exec/io.h>
#endif

#ifndef DEVICES_SERIAL_H
#include <devices/serial.h>
#endif

#ifndef LIBRARIES_GADTOOLS_H
#include <libraries/gadtools.h>
#endif 

#define PORT_VALIDATED_TRUE 0xFD08C792

#ifndef DEVICES_TIMER_H
#include <devices/timer.h>
#endif

#ifndef NOVIA_DOS_H
#include <novia/novia_dos.h>
#endif

struct InitPortStructure
{
	void *exit_func;
};


struct NetworkLayer
{
	/* Low Level Functions */

	Hook	OpenNetworkSocket;			// 20  function to open serial.device or init socket() etc.
	Hook	CloseNetworkSocket;			// 40  close serial.device, Flush resources etc....
	Hook	WaitConnect;				// 60  Wait for next call
	Hook	ResetNetworkSocket;			// 80  Hang up after, clear buffer after LOSTCARRIER.
	Hook	CheckNetworkStatus;			// 100 Check Carrier.

	/* Communication Functions */

	Hook	Send;						// 120 function to send data to network layer
	Hook	Recv;						// 140 function to recieve data from network layer

	UBYTE	isopen;						// 141
	UBYTE	networktype;				// 142 type of network. Normaly is the same as ClientConig->Networktype. 
	UBYTE	online;						// 143 Network Transmission is running. FALSE == NOCARRIER
	UBYTE	can_read;					// 144 System can send data to NetworkLayer
	UBYTE	can_write;					// 145 System ingnore incommingdate... for User-Monitor etc...

	UBYTE	res[15];					// 160
	char	ReadBuf[32];				// 192 serial.device  readbuffer;
	ULONG	length;						// 196 actual length of data in buffer.
	APTR	networkdata;				// 200 pointer to private network data-structur..
};


struct SerialNetwork
{
	struct	MsgPort		*SerWritePort,			// 4 WriteioPort
						*SerReadPort,			// 8 
						*SerQueryPort;			// 12
	struct  IOExtSer	*SerRead,				// 16 SerRead-IORequest
						*SerWrite,				// 20 SerWrite-IORequest
						*SerQuery;				// 24 IOExtSer for check carrier, serialbuffer... etc.
	char				SerReadSend;			// 25
	char				res[3];
};


struct TCPPort {								// new V0.16
	ULONG	Netmask;				// 4
	ULONG	NetIP;					// 8
	ULONG	PortIP;					// 12
	UWORD	Port;					// 14
	char	flag_dynamic;			// 15
	char	flag_fromdb;			// 16
	char	flag_OSI4_Protocol;		// 17			0 = TCP, 1 = UDP
	char	flag_charset;			// 18			0 = linemode, 1 = charmode. (RFC 2066)
	char	flag_extascii;			// 19 										(RFC 698)
	char 	reserved[493];			// 512
};

struct ParPort {
	char	Device_Name[30];		// 30
	UWORD	Unit;					// 32
	BYTE	CableType;				// 33
	char	reserved[479];			// 512
};

struct SerPort {					//      |CC-Offset|
	char	Device_Name[30];		// 30		92
	UWORD	Unit;					// 32		122
	ULONG	IdleRate;				// 36		124			// not implanted yet !!!
	ULONG	OnlineBaud;				// 40		128			// used baudrate.
	ULONG	OfflineBaud;			// 44		132			// not implanted yet !!!

	ULONG	IOBuffersize;			// 48		136
	UWORD	AnswerPause;			// 50		140
	UWORD	CarrierWait;			// 52		142
	BYTE	Parity;					// 53		144
	BYTE	Duplex;					// 54		145			// 0=RTS/CTS or 1=XON/XOFF
	BYTE	DataBits;				// 55		146
	BYTE	StopBits;				// 56		147

	BYTE	Handshake;				// 57		148
	BYTE	DTR_Hangup;				// 58		149
	BYTE	WaitDSR;				// 59		150
	BYTE	chrescape;				// 60		151
	BYTE	chrbreak;				// 61		152
	char	TermLinkPW[21];			// 82		153
	char	TermLinkECyrpt[21];		// 103		174
	char	ServerLinkPW[21];		// 124		195
	char	ServerLinkECyrpt[21];	// 145		216

	char	str_CONNECT[41];		// 186		237
	char	str_NOCARRIER[41];		// 227		278
	char	str_NODIALTONE[41];		// 268		319
	char	str_RING[41];			// 309		360
	char	str_BUSY[41];			// 350		401
	char	str_OK[41];				// 391		442
	char	str_TIMEOUT[41];		// 432		483
	char	str_ERROR[41];			// 473		524
	char	cmd_ANSWER[41];			// 514		565
	char	cmd_init[41];			// 555		606
	char	cmd_exit[41];			// 596		647
	char	cmd_hangup[41];			// 637		688
	char	dial_dialprefix[41];	// 678		739
	char	dial_dialsuffix[41];	// 719		770
	BYTE	dial_dialmode;			// 720		811
	char	reserved[100];			// 820		812
};

struct	ClientConfig
{
	ULONG						IDNumber;		// 4 Real PortID 
	ULONG						IPNumber;		// 8 Serial PortID
	ULONG						seek;			// 12 pos in sys.clientconfig
	char						datadir[80];	// 92 optional datadir

	struct	SerPort SerialData;					// 912
	struct	ParPort ParData;					// 1424
	struct	TCPPort TCPData;					// 1936
	ULONG	ScreenModeID;						// 1940
	ULONG	Colors;								// 1944

	BYTE	UseWorkbench;						// 1945 open the monitor-window on workbench
	BYTE	LoadPortatstart;					// 1946 Loading Client at booting
	
	BYTE	OpenScreenatstart;					// 1947
	BYTE	resbool;							// 1948

	char	ScreenFont[32];						// 1980
	UWORD	ScreenFontsize;						// 1982
	char	TermFont[32];						// 2014
	UWORD	TermFontsize;						// 2016
	BYTE	Networktype;						// 2017
	char	reserved[103];						// 2120
};

struct RexxProg
{
	struct RexxProg *ln_Succ;
	struct RexxPorg *ln_Pred;
	struct PortData *cport;
	struct MsgPort	*replyport;
	char ProgramName[256];	
	char tempfile[40];	
	char TaskName[32];
};

struct PortData
{
	struct 	ClientConfig	clientconfig;		// 2120
	struct	SystemDirs		*sysdirs;			// 2124		Only for Port-Init.

	struct	IOStdReq 	*ConRead;				// 2128		ConRead io-request
	struct	IOStdReq 	*ConWrite;				// 2132		ConWrite
	struct	timerequest *TimerReq;				// 2136
	struct	NetworkLayer network;				// 2336

	UBYTE		ConReadSend;						// 2337			console.device - CMD_READ send, waiting for data
	UBYTE		ChatBreak;							// 2338
	UBYTE		ich_glaube_komma_das_byte_benutz_isch_spaeter_einmal_komma_wenn_ich_lust_dazu_habe_punkt;
	UBYTE		SysopChat;				// 2340
	
	char 	argstring[10][80];					// 3140
	ULONG	argcounter;							// 3144

	struct	MsgPort		*ConPort,				// 3148
								*MainPort,				// 3152
								*LocalPort,				// 3156
								*rexxport,				// 3160		REXX-Command Port
								*TimePort,				// 3164		Msgport for timer.device
								*rtport,				// 3168		runtime port for rexx, dos programs.
								*nfs_port;

	struct	Screen		*scr;					// 3172
	struct	Window		*win;					// 3176

	struct	UserList	UL;						// 3192
	struct	UserData	LocalUser,				// 4344
						EditUser;				// 5496

	struct	Date		PortLoad;				// 5508 Port was loaded on... 
	struct	Date		PortInstalled;			// 5520 Port was installed on... 
	struct	Date		LastLogin;				// 5532 Last User has login on... 

	ULONG		RealID;							// 5536 Number of entry in sorted list
	BYTE		PortType;						// 5537 Port is local or externial 
	BYTE		PortStatus;						// 5538 0 - Port is closed
												/*		1 - Port is offline
														2 - online, waiting for call
														3 - online, carrier detected
														4 - online, user has login */

	BYTE	Supervisor,							// 5539 SV-Flag
			NewUser,							// 5540 NewUser Process 
			Dumped,								// 5541 auto-kickoff flag	
			HideAll,							// 5542 Hidden from ALL?	
			MuffAll,							// 5543  Muffling ALL?	
			MonitorAll;							// 5544 Monitoring ALL?
	ULONG	CurrentCPS;							// 5548 Current CPS Rate 
	Directory *cmddir;					// 5552 command directory
	ULONG	*numberofmci;						// 5556 Number of mci's
	struct	mcicode *mcicmd;					// 5560 MCI Table
	ULONG	PortValidated;						// 5564
	char	Where[40];							// 5604 WHo is't 					//new in V0.09

	struct	STR_Group *strgroup;				// 5608 							//new in V0.10
	UWORD 	strgroups;							// 5610
	UWORD	ressfsfdsfds;						// 5612
	char	*iobuffer;							// 5616 pointer to iobuffer;
	MinList	iobuffer_list;						// 5628									//new in V0.15
	MinList his_list;							// 5640 Input History List for Curser Up/Down
	MinList	select_list;						// 5652 Selected File/Item List
	ULONG	hiscounter;							// 5656 hisentrys;

	ULONG	RexxOpenCounter;					// 5660

	UBYTE	color;								// 5661 current Pen
	UBYTE	bcolor;								// 5662 current background color
	UBYTE	CurserX;							// 5663 curser column position
	UBYTE	CurserY;							// 5664 curser line position

	UBYTE	sssfsfsfssffre;						// 5665
	UBYTE	ProgramClose;						// 5666 Port Shut Down
	UBYTE	gfdgfdgfdhbhk;						// 5667
	UBYTE	connect_seq;						// 5668 connect sequence, waiting for next call.

	char 	ConBuf[32];							// 5700 console.device readbuffer;
	struct	Menu *winmenu;						// 5704
	APTR	vinfo;								// 5708
	struct 	NewMenu *mymenu;					// 5712
	char	*Titel;								// 5716 ScreenTitel
	char	actitel[80];						// 5796
	UBYTE	MonitorOpen;						// 5797 Monitor is open
	UBYTE	readnew;							// 5798 UDBase
	UBYTE	systemservice;						// 5799 User is a system administrator
	UBYTE	reserved;							// 5800
	char	whostring[32];						// 5832
	char	RexxPortName[32];					// 5864
	char	TaskName[32];						// 5896
	struct	Window *xprwin;						// 5900
	UBYTE	netecho;							// 5901 echo to network-device
	UBYTE	conecho;							// 5902
	UBYTE	res45604;							// 5903
	UBYTE	res45610;							// 5904
	struct 	Task *task;							// 5908
	MinList downloadlist;						// 5920 files to download
	MinList rexxlist;							// 5932
	struct  SelectItem *actual;					// 5936 actual file
	char	 *currentdirname;					// 5940 pointer of currentdir
	Directory *currentdir;						// 5944
	char	*homedir;							// 5948 pointer to users homedir
	char	*rootdir;							// 5952
	char	*inputbuffer;						// 5956 input buffer
	char	*argptr;							// 5960 pointer of first arg
	ItemEntryList itementrylist;				// 6000 list of ListEntryBlock's in currentdir
//	char	reservedBytes[20];					// 6000
};

struct PortData2 {					// This structure is only for ClientDatabase in Mainport
	Node		port_node;			// 14
	UWORD		fuck_commodore;		// 16
	PortData	*port_address;		// 20
};


struct XPRProtocol
{
	struct Node node;

	char name[41];
	char library[41];
	char options[41];
	LONG batch;
};


#define SIZEOF_CLIENTNAME 20

#endif	// NOVIA_PORTDATA_H
