
struct portdef {
	char 	txt[ 80 ];		/* Current State, Text form */
	char	lastuser[ 40 ];	/* Letzer User */
	time_t	lastuser_when;
	char	lastsys[ 40 ];		// Bei IP-Ports Remote-IP
	time_t	lastsys_when;
	char	lastpoint[ 40 ];	// Bei IP-Ports Remote-Host
	time_t	lastpoint_when;

	ULONG	state;			/* Current state */
	ULONG	online;			/* Flag: True if port is supposed to be online */
	ULONG	visible;		/* Flag: Port is visible */
	ULONG	init;			/* Active Init */

	ULONG	displayed;		/* Data has been displayed */
};

#define PORTCMD_INIT	5000
#define PORTCMD_REREAD  5001
#define PORTCMD_ATA     5002
#define PORTCMD_BUSY    5003
#define PORTCMD_NETCALL 5004
#define PORTCMD_KILL	5005
#define PORTCMD_TERM	5006

#define PORTCMD_MESSAGE		4
#define PORTCMD_CANCEL 6

#ifndef _NO_PORTINFO
struct PortInfo{
	char PortDeviceName[41];
	char Comment[41];          			/*Sysopinfo*/
	char PortSysname[11]; 				/* Leer=Default-Systemname s.u. */
	char PortModemInit[64];
	char PortModemInit2[64];
	char PortModemInit3[64];
	char PortModemInit4[64];
	char PortModemWaehlen[41];
	char PortModemAuflegen[41]; 		/* Auflegebefehl ohne +++ ! */
	char PortModemStatus[41];			// Statusabfrage nach Hangup
	char PortModemBusy[41];				// FREE!
	char PortFaxConnect[41];
	char PortFaxCmd[82];
	char port_slipmsg[41];				// FREE!
	char PortModemTurnOnline[41]; 		/*=Besetzt schalten */   
	char OnlineUser[41];				// FREE
	long PortBaud;
	long PortBuffer;
	long NumberOfCalls;
	short PortEscChar;
	short PortModemDelay;
	short PortUnitNumber;
	short PortSerialFlags;
	short NumberOfRings;
	short InternalSerflags;
	int WaitForConnect;
	int RedialDelay;
	long WaitQuiet; 					/* siehe flags */
	long supported_protos; 				/* Dummy */
	ULONG fax_baud;
	char port_slipdev[ 30 ];
	ULONG port_slipunit;
	char port_pppdev[ 30 ];
	ULONG port_pppunit;
	UBYTE port_slipallowed, port_pppallowed;
	char reserved[2];
	UBYTE fax;			/* wenn 42, dann Fax vorhanden */
	UBYTE active_init;	/* Aktiver Modeminit */
	UBYTE fax_close,
		  fax_adapt;
};

// InternalSerFlags

#define SER_DISABLE_CARRIER 1
#define SER_LOCKBAUD 2
#define SER_TERMMODE 8

#endif
