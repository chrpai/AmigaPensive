/*
** Zeus Asynchronos Packet Protocol (ZAPP)
**
** ZT_* types define the protocols
** ZS_* types define packets used during the ZT_* protocol
*/

#ifndef	ZAPP_H
#define	ZAPP_H

#ifndef	EXEC_TYPES_H
#include	<exec/types.h>
#endif

#ifndef	EXEC_PORTS_H
#include	<exec/ports.h>
#endif

#ifndef	EXEC_IO_H
#include	<exec/io.h>
#endif

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d)	((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#define	ZAPP_CHAR		(0xBF)
#define	ZAPP_ESC1		(0xA1)
#define	ZAPP_ESC2		(0xA2)
#define	ZAPP_ESC3		(0xA3)

struct ZappAddr
{
	UBYTE						 Zone;
	UBYTE						 Region;
	UBYTE						 Site;
	UBYTE						 Line;
};

#define	ZA_WILD			 (0xFF)

struct IOZappReq
{
	struct	IOStdReq		 zr_IO;
	struct	ZappAddr		 zr_LAddr;		/* (R)    Local address */
	struct	ZappAddr		 zr_RAddr;		/* (R/WO) Remote address */
	APTR						 zr_Result;		/* (R/W)  Result data */
	ULONG						 zr_ResLen;		/* (R/W)  Result length */
	ULONG						 zr_Type;		/* (R/WO) ZT_ Protocol define */
	UWORD						 zr_ID;			/* (R)    Unique ID for this session - !!PRIVATE!! */
	UWORD						 zr_RID;			/* (R)    Remote ID for this session - !!PRIVATE!! */
	UWORD						 zr_TimeOut;	/* (W)    Timeout for request */
	UWORD						 zr_Time;		/* (R)    Time taken to complete request */
	UBYTE						 zr_SubType;	/* (R/W)  ZS_ Packet Subtypes */
};

/*
** Zapp Link Statistics
*/

struct	ZappStat
{
	ULONG								 baudrate;

	ULONG								 rx_pkts;		/* Number of received packets */
	ULONG								 tx_pkts;		/* Number of transmitted packets */
	ULONG								 rx_bytes;		/* Amount of received bytes */
	ULONG								 tx_bytes;		/* Amound of transmitted bytes */

	ULONG								 err_crc;		/* CRC Errors */
	ULONG								 err_sync;		/* Sync Errors */
	ULONG								 err_orun;		/* Buffer Overruns */
	ULONG								 err_unpack;	/* Unpacking Errors */
	
	LONG								 cmp_avg;		/* Average compression percent */ 
};

/*
** ZAPP Error Returns
*/

#define	ZE_NONE				(0)
#define	ZE_UNKNOWN_TYPE	(1)
#define	ZE_UNKNOWN_ID		(2)
#define	ZE_NO_HOST			(3)
#define	ZE_REFUSED			(4)
#define	ZE_NO_CARRIER		(5)
#define	ZE_TIME_OUT			(6)
#define	ZE_BAD_LINE			(7)
#define	ZE_NO_MEMORY		(8)
#define	ZE_NOT_ZAPP_MODE	(9)
#define	ZE_NO_DEV_OPEN		(10)
#define	ZE_CLOSING_DOWN	(11)
#define	ZE_CLOSED			(13)
#define	ZE_MAX_CLIENTS		(15)
#define	ZE_OLD_VERSION		(16)
#define	ZE_NO_ROUTE			(17)
#define	ZE_UNKNOWN_INFO	(18)
#define	ZE_UNKNOWN_ADDR	(19)
#define	ZE_BAD_ADDR			(20)
#define	ZE_NO_SCREEN		(21)

/*
** ZAPP Commands
*/

#define	ZCMD_IMMEDIATE		(CMD_NONSTD+1)
#define	ZCMD_REGISTER		(CMD_NONSTD+3)
#define	ZCMD_INFO_REQ		(CMD_NONSTD+4)
#define	ZCMD_INFO_REP		(CMD_NONSTD+5)
#define	ZCMD_RESOLVE		(CMD_NONSTD+6)
#define	ZCMD_HOSTADDR		(CMD_NONSTD+7)
#define	ZCMD_GETSCREEN		(CMD_NONSTD+8)
#define	ZCMD_FREESCREEN	(CMD_NONSTD+9)
#define	ZCMD_SETSTATUS		(CMD_NONSTD+10)
#define	ZCMD_CLOSE			(CMD_NONSTD+11)
#define	ZCMD_DEVSTATUS		(CMD_NONSTD+12)

/*
** Zapp Protocol Types
*/

#define	ZT_LOGON			MAKE_ID( 'L','G','O','N' )
#define	ZT_FILEXFER		MAKE_ID( 'X','F','E','R' )
#define	ZT_SIMPLEMSG	MAKE_ID( 'S','M','S','G' )
#define	ZT_INFO			MAKE_ID( 'I','N','F','O' )
#define	ZT_DEVINFO		MAKE_ID( 'D','I','N','F' )

#define	ZS_ANY			(0x00)
#define	ZS_END			(0xFF)

/*
** IO Control Commands (Zeus Only!)
*/

#define	IO_QUIT					(0)
#define	IO_OPEN_SERIAL			(1)
#define	IO_CLOSE_SERIAL		(2)
#define	IO_SER_LISTEN			(3)
#define	IO_SER_UNLISTEN		(4)
#define	IO_ZAPP_ON				(5)
#define	IO_ZAPP_OFF				(6)

struct	Client
{
	struct	Node			 Node;
	UWORD						 ID;
	struct	ZappAddr		 Dest;
	struct	Task			*Task;
	ULONG						 Secs;
	ULONG						 Micros;
	UBYTE						 Status[200];
};

/*
** zapp.lib prototypes
*/

BYTE zapp_Resolve( struct IOZappReq *io, UBYTE *data );
BYTE zapp_HostAddr( struct IOZappReq *io );
BYTE zapp_Write( struct IOZappReq *io, APTR data, LONG length );
BYTE zapp_Read( struct IOZappReq *io, APTR data, LONG length );
BYTE zapp_Register( struct IOZappReq *io );
BYTE zapp_Close( struct IOZappReq *io );
BYTE zapp_SetStatus( struct IOZappReq *io, APTR data );
BYTE zapp_GetScreen( struct IOZappReq *io, struct Screen **scr );
BYTE zapp_FreeScreen( struct IOZappReq *io, struct Screen **scr );

#endif	/* #ifndef ZAPP_H */
