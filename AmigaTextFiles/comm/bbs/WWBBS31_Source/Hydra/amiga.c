/*
**	Amiga support module for HYDRA protocol sample implementation.
**
**	Written by	Olaf Barthel
**			Brabeckstrasse 35
**			D-30559 Hannover
**
**			eMail: olsen@sourcery.han.de
**
**	Freely distributable.
*/

	/* System includes. */

#include <utility/date.h>

#include <devices/serial.h>
#include <devices/timer.h>

#include <hardware/cia.h>

#include <dos/dosextens.h>
#include <dos/filehandler.h>
#include <dos/dosasl.h>

#include <exec/memory.h>

	/* Correct a nasty bug in the prototypes. */

#define CheckIO foo21234

#include <clib/utility_protos.h>
#include <clib/timer_protos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/macros.h>

	/* Get the CheckIO prototype right. */

#undef CheckIO

struct IORequest *CheckIO(struct IORequest *);

#include "hydracom.h"

	/* Serial buffer size. */

#define BUFFER_SIZE 8192

	/* A handy macro. */

#define ClrSignal(s)	SetSignal(0,s)

	/* Signal masks. */

#define SIG_SERREAD	(1UL << ReadPort -> mp_SigBit)
#define SIG_SERWRITE	(1UL << WritePort -> mp_SigBit)
#define SIG_TIMER	(1UL << TimePort -> mp_SigBit)
#define SIG_WINDOW	(1UL << LocalWindow -> UserPort -> mp_SigBit)

	/* A serial buffer structure. */

struct SerialBuffer
{
	struct IOExtSer	*SerialRequest;
	UBYTE		*SerialBuffer,
			*SerialIndex,
			*SerialTop;
	LONG		 SerialSize,
			 SerialFilled;
	BOOL		 IsClone,
			 IsBusy;
};

	/* Library bases. */

struct Library
			*UtilityBase,
			*TimerBase;

	/* Timer data. */

struct MsgPort		*TimePort;
struct timerequest	*TimeRequest;

	/* Serial data. */

struct MsgPort		*ReadPort,
			*WritePort;

struct SerialBuffer	*ThisBuffer,
			*NextBuffer,
			*ReadBuffer;

	/* DOS Data. */

struct AnchorPath	*Anchor;
BOOL			 AnchorUsed = FALSE;

	/* CloneSerialBuffer():
	 *
	 *	Clone a SerialBuffer structure.
	 */

STATIC struct SerialBuffer * __regargs
CloneSerialBuffer(struct SerialBuffer *Source,struct MsgPort *MsgPort)
{
	struct SerialBuffer *Buffer;

	if(Buffer = (struct SerialBuffer *)AllocVec(sizeof(struct SerialBuffer) + Source -> SerialSize,MEMF_ANY | MEMF_PUBLIC))
	{
		Buffer -> SerialBuffer	= Buffer -> SerialIndex = (UBYTE *)(Buffer + 1);
		Buffer -> SerialFilled	= 0;
		Buffer -> SerialTop	= Buffer -> SerialBuffer + Source -> SerialSize;
		Buffer -> SerialSize	= Source -> SerialSize;
		Buffer -> IsClone	= TRUE;
		Buffer -> IsBusy	= FALSE;

		if(Buffer -> SerialRequest = (struct IOExtSer *)AllocVec(sizeof(struct IOExtSer),MEMF_ANY | MEMF_PUBLIC))
		{
			CopyMem(Source -> SerialRequest,Buffer -> SerialRequest,sizeof(struct IOExtSer));

			Buffer -> SerialRequest -> IOSer . io_Message . mn_ReplyPort = MsgPort;

			return(Buffer);
		}
		else
			cprint("Could not create serial request\n");

		FreeVec(Buffer);
	}
	else
		cprint("Could not create serial buffer\n");

	return(NULL);
}

	/* DeleteSerialBuffer():
	 *
	 *	Delete a SerialBuffer structure.
	 */

STATIC VOID __regargs
DeleteSerialBuffer(struct SerialBuffer *Buffer)
{
	if(Buffer)
	{
		if(Buffer -> IsBusy)
		{
			if(!CheckIO(Buffer -> SerialRequest))
				AbortIO(Buffer -> SerialRequest);

			WaitIO(Buffer -> SerialRequest);
		}
/* Clear */
		Buffer -> SerialRequest -> IOSer . io_Command = CMD_CLEAR;
		DoIO(Buffer -> SerialRequest);

		if(Buffer -> IsClone)
			FreeVec(Buffer -> SerialRequest);
		else
		{
			CloseDevice(Buffer -> SerialRequest);

			DeleteIORequest(Buffer -> SerialRequest);
		}

		FreeVec(Buffer);
	}
}

	/* CreateSerialBuffer():
	 *
	 *	Create a serial buffer structure.
	 */

STATIC struct SerialBuffer * __regargs
CreateSerialBuffer(STRPTR Device,LONG Unit,LONG Size,struct MsgPort *MsgPort)
{
	struct SerialBuffer *Buffer;

	if(Buffer = (struct SerialBuffer *)AllocVec(sizeof(struct SerialBuffer) + Size,MEMF_ANY | MEMF_PUBLIC))
	{
		Buffer -> SerialBuffer	= Buffer -> SerialIndex = (UBYTE *)(Buffer + 1);
		Buffer -> SerialFilled	= 0;
		Buffer -> SerialTop	= Buffer -> SerialBuffer + Size;
		Buffer -> SerialSize	= Size;
		Buffer -> IsClone	= FALSE;
		Buffer -> IsBusy	= FALSE;

		if(Buffer -> SerialRequest = (struct IOExtSer *)CreateIORequest(MsgPort,sizeof(struct IOExtSer)))
		{
			Buffer -> SerialRequest -> io_SerFlags |= SERF_SHARED;
			if(!OpenDevice(Device,Unit,Buffer -> SerialRequest,NULL))
				return(Buffer);
			else
			{
				cprint("Could not open \"%s\", unit %d\n",Device,Unit);

				DeleteIORequest(Buffer -> SerialRequest);
			}
		}
		else
			cprint("Could not create serial request\n");

		FreeVec(Buffer);
	}
	else
		cprint("Could not create serial buffer\n");

	return(NULL);
}

	/* OpenAll():
	 *
	 *	Allocate all the resources required.
	 */

STATIC BOOL
OpenAll(STRPTR Device,LONG Unit)
{
	if(!(UtilityBase = OpenLibrary("utility.library",37)))
	{
		cprint("Could not open utility.library v37\n");

		return(FALSE);
	}

	if(!(TimePort = CreateMsgPort()))
	{
		cprint("Could not create timer port\n");

		return(FALSE);
	}

	if(!(TimeRequest = (struct timerequest *)CreateIORequest(TimePort,sizeof(struct timerequest))))
	{
		cprint("Could not create timer request\n");

		return(FALSE);
	}

	if(OpenDevice(TIMERNAME,UNIT_VBLANK,TimeRequest,NULL))
	{
		cprint("Could not open timer\n");

		return(FALSE);
	}

	TimerBase = (struct Library *)TimeRequest -> tr_node . io_Device;

	if(!(Anchor = (struct AnchorPath *)AllocVec(sizeof(struct AnchorPath) + 512,MEMF_ANY | MEMF_CLEAR)))
	{
		cprint("Could not allocate pattern matching buffe\n");

		return(FALSE);
	}

	Anchor -> ap_Strlen = 512;

	if(!(ReadPort = CreateMsgPort()))
	{
		cprint("Could not create serial read port\n");

		return(FALSE);
	}

	if(!(WritePort = CreateMsgPort()))
	{
		cprint("Could not create serial write port\n");

		return(FALSE);
	}

	if(!(ReadBuffer = CreateSerialBuffer(Device,Unit,BUFFER_SIZE,ReadPort)))
		return(FALSE);

	if(!(ThisBuffer = CloneSerialBuffer(ReadBuffer,WritePort)))
		return(FALSE);

	if(!(NextBuffer = CloneSerialBuffer(ReadBuffer,WritePort)))
		return(FALSE);

	return(TRUE);
}

	/* CloseAll():
	 *
	 *	Close all the resources.
	 */

STATIC VOID
CloseAll(VOID)
{
	if(AnchorUsed)
		MatchEnd(Anchor);

	if(Anchor)
		FreeVec(Anchor);

	DeleteSerialBuffer(NextBuffer);
	DeleteSerialBuffer(ThisBuffer);
	DeleteSerialBuffer(ReadBuffer);

	if(WritePort)
		DeleteMsgPort(WritePort);

	if(ReadPort)
		DeleteMsgPort(ReadPort);

	if(TimeRequest)
	{
		if(TimeRequest -> tr_node . io_Device)
			CloseDevice(TimeRequest);

		DeleteIORequest(TimeRequest);
	}

	if(TimePort)
		DeleteMsgPort(TimePort);

	if(UtilityBase)
		CloseLibrary(UtilityBase);

}

	/* dtr_out(byte flag):
	 *
	 *	If flag == 0 -> drop DTR signal, else set it.
	 */

VOID
dtr_out(byte flag)
{
	if(!flag)
	{
		if(ThisBuffer -> IsBusy)
		{
			WaitIO(ThisBuffer -> SerialRequest);
	
			ThisBuffer -> IsBusy		= FALSE;
			ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;
		}
	
		if(ReadBuffer -> IsBusy)
		{
			if(!CheckIO(ReadBuffer -> SerialRequest))
				AbortIO(ReadBuffer -> SerialRequest);

			WaitIO(ReadBuffer -> SerialRequest);
	
			ReadBuffer -> IsBusy		= FALSE;
			ReadBuffer -> SerialFilled	= ReadBuffer -> SerialRequest -> IOSer . io_Actual;
			ReadBuffer -> SerialIndex	= ReadBuffer -> SerialBuffer;
	
			ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_QUERY;
	
			DoIO(ReadBuffer -> SerialRequest);
	
			if(ReadBuffer -> SerialRequest -> IOSer . io_Actual)
			{
				LONG Size = ReadBuffer -> SerialSize - ReadBuffer -> SerialFilled;
	
				if(Size > 0)
				{
					if(Size > ReadBuffer -> SerialRequest -> IOSer . io_Actual)
						Size = ReadBuffer -> SerialRequest -> IOSer . io_Actual;
	
					ReadBuffer -> SerialRequest -> IOSer . io_Command	= CMD_READ;
					ReadBuffer -> SerialRequest -> IOSer . io_Data		= ReadBuffer -> SerialBuffer + ReadBuffer -> SerialFilled;
					ReadBuffer -> SerialRequest -> IOSer . io_Length	= Size;
	
					DoIO(ReadBuffer -> SerialRequest);
	
					ReadBuffer -> SerialFilled += ReadBuffer -> SerialRequest -> IOSer . io_Actual;
				}
			}
		}

		CloseDevice(ReadBuffer -> SerialRequest);

		TimeRequest -> tr_node . io_Command	= TR_ADDREQUEST;
		TimeRequest -> tr_time . tv_secs	= 1;
		TimeRequest -> tr_time . tv_micro	= 0;

		DoIO(TimeRequest);

		if(OpenDevice(device,port,ReadBuffer -> SerialRequest,NULL))
		{
			CloseAll();

			exit(10);
		}
		else
		{
			ReadBuffer -> SerialRequest -> io_Baud		= ThisBuffer -> SerialRequest -> io_Baud;
			ReadBuffer -> SerialRequest -> io_ReadLen	= ThisBuffer -> SerialRequest -> io_ReadLen;
			ReadBuffer -> SerialRequest -> io_WriteLen	= ThisBuffer -> SerialRequest -> io_WriteLen;
			ReadBuffer -> SerialRequest -> io_SerFlags	= ThisBuffer -> SerialRequest -> io_SerFlags;

			ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_SETPARAMS;

			DoIO(ReadBuffer -> SerialRequest);

			CopyMem(ReadBuffer -> SerialRequest,ThisBuffer -> SerialRequest,sizeof(struct IOExtSer));

			ThisBuffer -> SerialRequest -> IOSer . io_Message . mn_ReplyPort = WritePort;

			CopyMem(ReadBuffer -> SerialRequest,NextBuffer -> SerialRequest,sizeof(struct IOExtSer));

			NextBuffer -> SerialRequest -> IOSer . io_Message . mn_ReplyPort = WritePort;
		}
	}
}

	/* com_flow(byte flags):
	 *
	 *	The bit mask `flags' determines the style(s) of
	 *	handshaking:
	 *
	 *	if (flags & 9) -> enable xON/xOFF software handshaking,
	 *	if (flags & 2) -> enable RTS/CTS hardware handshaking
	 */

VOID
com_flow(byte flags)
{
	if(ThisBuffer -> IsBusy)
	{
		WaitIO(ThisBuffer -> SerialRequest);

		ThisBuffer -> IsBusy		= FALSE;
		ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;
	}

	if(ReadBuffer -> IsBusy)
	{
		if(!CheckIO(ReadBuffer -> SerialRequest))
			AbortIO(ReadBuffer -> SerialRequest);

		WaitIO(ReadBuffer -> SerialRequest);

		ReadBuffer -> IsBusy		= FALSE;
		ReadBuffer -> SerialFilled	= ReadBuffer -> SerialRequest -> IOSer . io_Actual;
		ReadBuffer -> SerialIndex	= ReadBuffer -> SerialBuffer;

		ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_QUERY;

		DoIO(ReadBuffer -> SerialRequest);

		if(ReadBuffer -> SerialRequest -> IOSer . io_Actual)
		{
			LONG Size = ReadBuffer -> SerialSize - ReadBuffer -> SerialFilled;

			if(Size > 0)
			{
				if(Size > ReadBuffer -> SerialRequest -> IOSer . io_Actual)
					Size = ReadBuffer -> SerialRequest -> IOSer . io_Actual;

				ReadBuffer -> SerialRequest -> IOSer . io_Command	= CMD_READ;
				ReadBuffer -> SerialRequest -> IOSer . io_Data		= ReadBuffer -> SerialBuffer + ReadBuffer -> SerialFilled;
				ReadBuffer -> SerialRequest -> IOSer . io_Length	= Size;

				DoIO(ReadBuffer -> SerialRequest);

				ReadBuffer -> SerialFilled += ReadBuffer -> SerialRequest -> IOSer . io_Actual;
			}
		}
	}

	ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_SETPARAMS;

	DoIO(ReadBuffer -> SerialRequest);

	ThisBuffer -> SerialRequest -> io_SerFlags = ReadBuffer -> SerialRequest -> io_SerFlags;
	NextBuffer -> SerialRequest -> io_SerFlags = ReadBuffer -> SerialRequest -> io_SerFlags;
}

	/* com_setspeed(word speed):
	 *
	 *	Set the transfer speed (in bits/second).
	 */

VOID
com_setspeed(word speed)
{
	if(ThisBuffer -> IsBusy)
	{
		WaitIO(ThisBuffer -> SerialRequest);

		ThisBuffer -> IsBusy		= FALSE;
		ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;
	}

	if(ReadBuffer -> IsBusy)
	{
		if(!CheckIO(ReadBuffer -> SerialRequest))
			AbortIO(ReadBuffer -> SerialRequest);

		WaitIO(ReadBuffer -> SerialRequest);

		ReadBuffer -> IsBusy		= FALSE;
		ReadBuffer -> SerialFilled	= ReadBuffer -> SerialRequest -> IOSer . io_Actual;
		ReadBuffer -> SerialIndex	= ReadBuffer -> SerialBuffer;

		ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_QUERY;

		DoIO(ReadBuffer -> SerialRequest);

		if(ReadBuffer -> SerialRequest -> IOSer . io_Actual)
		{
			LONG Size = ReadBuffer -> SerialSize - ReadBuffer -> SerialFilled;

			if(Size > 0)
			{
				if(Size > ReadBuffer -> SerialRequest -> IOSer . io_Actual)
					Size = ReadBuffer -> SerialRequest -> IOSer . io_Actual;

				ReadBuffer -> SerialRequest -> IOSer . io_Command	= CMD_READ;
				ReadBuffer -> SerialRequest -> IOSer . io_Data		= ReadBuffer -> SerialBuffer + ReadBuffer -> SerialFilled;
				ReadBuffer -> SerialRequest -> IOSer . io_Length	= Size;

				DoIO(ReadBuffer -> SerialRequest);

				ReadBuffer -> SerialFilled += ReadBuffer -> SerialRequest -> IOSer . io_Actual;
			}
		}
	}

	ReadBuffer -> SerialRequest -> io_Baud = speed;

	ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_SETPARAMS;

	DoIO(ReadBuffer -> SerialRequest);

	ThisBuffer -> SerialRequest -> io_Baud		= ReadBuffer -> SerialRequest -> io_Baud;
	ThisBuffer -> SerialRequest -> io_ReadLen	= ReadBuffer -> SerialRequest -> io_ReadLen;
	ThisBuffer -> SerialRequest -> io_WriteLen	= ReadBuffer -> SerialRequest -> io_WriteLen;
	ThisBuffer -> SerialRequest -> io_SerFlags	= ReadBuffer -> SerialRequest -> io_SerFlags;

	NextBuffer -> SerialRequest -> io_Baud		= ReadBuffer -> SerialRequest -> io_Baud;
	NextBuffer -> SerialRequest -> io_ReadLen	= ReadBuffer -> SerialRequest -> io_ReadLen;
	NextBuffer -> SerialRequest -> io_WriteLen	= ReadBuffer -> SerialRequest -> io_WriteLen;
	NextBuffer -> SerialRequest -> io_SerFlags	= ReadBuffer -> SerialRequest -> io_SerFlags;
}

	/* com_putblock(byte *s,word len):
	 *
	 *	Send a data block asynchronously.
	 */

VOID
com_putblock(byte *s,word len)
{
	struct SerialBuffer *Swap = ThisBuffer;

	if(ThisBuffer -> IsBusy)
		WaitIO(ThisBuffer -> SerialRequest);
	else
	{
		if(ThisBuffer -> SerialIndex > ThisBuffer -> SerialBuffer)
		{
			ThisBuffer -> SerialRequest -> IOSer . io_Command	= CMD_WRITE;
			ThisBuffer -> SerialRequest -> IOSer . io_Data		= ThisBuffer -> SerialBuffer;
			ThisBuffer -> SerialRequest -> IOSer . io_Length	= ThisBuffer -> SerialIndex - ThisBuffer -> SerialBuffer;

			DoIO(ThisBuffer -> SerialRequest);
		}
	}

	ThisBuffer -> IsBusy		= FALSE;
	ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;

	ThisBuffer = NextBuffer;
	NextBuffer = Swap;

	if(ThisBuffer -> SerialIndex > ThisBuffer -> SerialBuffer)
	{
		ThisBuffer -> SerialRequest -> IOSer . io_Command	= CMD_WRITE;
		ThisBuffer -> SerialRequest -> IOSer . io_Data		= ThisBuffer -> SerialBuffer;
		ThisBuffer -> SerialRequest -> IOSer . io_Length	= ThisBuffer -> SerialIndex - ThisBuffer -> SerialBuffer;

		DoIO(ThisBuffer -> SerialRequest);
	}

	ThisBuffer -> IsBusy		= FALSE;
	ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;

	while(len > 2 * ThisBuffer -> SerialSize)
	{
		ThisBuffer -> SerialRequest -> IOSer . io_Command	= CMD_WRITE;
		ThisBuffer -> SerialRequest -> IOSer . io_Data		= s;
		ThisBuffer -> SerialRequest -> IOSer . io_Length	= ThisBuffer -> SerialSize;

		s	+= ThisBuffer -> SerialSize;
		len	-= ThisBuffer -> SerialSize;

		DoIO(ThisBuffer -> SerialRequest);
	}

	CopyMem(s,ThisBuffer -> SerialBuffer,MIN(len,ThisBuffer -> SerialSize));

	ThisBuffer -> IsBusy					= TRUE;
	ThisBuffer -> SerialIndex				= ThisBuffer -> SerialBuffer + MIN(len,ThisBuffer -> SerialSize);
	ThisBuffer -> SerialRequest -> IOSer . io_Command	= CMD_WRITE;
	ThisBuffer -> SerialRequest -> IOSer . io_Data		= ThisBuffer -> SerialBuffer;
	ThisBuffer -> SerialRequest -> IOSer . io_Length	= MIN(len,ThisBuffer -> SerialSize);

	len	-= ThisBuffer -> SerialRequest -> IOSer . io_Length;
	s	+= ThisBuffer -> SerialRequest -> IOSer . io_Length;

	ClrSignal(SIG_SERWRITE);
	SendIO(ThisBuffer -> SerialRequest);

	if(len > 0)
	{
		CopyMem(s,NextBuffer -> SerialBuffer,len);

		NextBuffer -> SerialIndex = NextBuffer -> SerialBuffer + len;
	}
}

	/* breakfunc():
	 *
	 *	Cleanup routine for SAS/C.
	 */

static int
breakfunc(void)
{
	CloseAll();

	return(1);
}

	/* sys_init(VOID):
	 *
	 *	Initialize this driver implementation.
	 */

VOID
sys_init(VOID)
{
	if(!OpenAll(device,port))
	{
		CloseAll();

		endprog(2);
	}

	onbreak(breakfunc);
}

	/* sys_reset(VOID):
	 *
	 *	Perform cleanup for this driver implementation.
	 */

VOID
sys_reset(VOID)
{
	CloseAll();
}

	/* sys_idle(VOID):
	 *
	 *	This routine gets called when the system is idle.
	 *	That's a nice one. We are supposed to call the
	 *	system scheduler, etc.
	 */

VOID
sys_idle(VOID)
{
	ULONG Signals;

	if(ReadBuffer -> SerialFilled <= 0 && !ReadBuffer -> IsBusy)
	{
		ReadBuffer -> IsBusy					= TRUE;
		ReadBuffer -> SerialIndex				= ReadBuffer -> SerialBuffer;
		ReadBuffer -> SerialRequest -> IOSer . io_Command	= CMD_READ;
		ReadBuffer -> SerialRequest -> IOSer . io_Data		= ReadBuffer -> SerialBuffer;
		ReadBuffer -> SerialRequest -> IOSer . io_Length	= 1;

		ClrSignal(SIG_SERREAD);
		SendIO(ReadBuffer -> SerialRequest);
	}

	TimeRequest -> tr_node . io_Command	= TR_ADDREQUEST;
	TimeRequest -> tr_time . tv_secs	= 1;
	TimeRequest -> tr_time . tv_micro	= 0;

	ClrSignal(SIG_TIMER);
	SendIO(TimeRequest);

	Signals = Wait(SIG_SERREAD | SIG_SERWRITE | SIG_TIMER);

	if(!(Signals & SIG_TIMER))
	{
		if(!CheckIO(TimeRequest))
			AbortIO(TimeRequest);
	}

	WaitIO(TimeRequest);

	if(Signals & SIG_SERREAD)
	{
		WaitIO(ReadBuffer -> SerialRequest);

		ReadBuffer -> IsBusy		= FALSE;
		ReadBuffer -> SerialFilled	= ReadBuffer -> SerialRequest -> IOSer . io_Actual;
		ReadBuffer -> SerialIndex	= ReadBuffer -> SerialBuffer;

		ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_QUERY;

		DoIO(ReadBuffer -> SerialRequest);

		if(ReadBuffer -> SerialRequest -> IOSer . io_Actual)
		{
			LONG Size = ReadBuffer -> SerialSize - ReadBuffer -> SerialFilled;

			if(Size > 0)
			{
				if(Size > ReadBuffer -> SerialRequest -> IOSer . io_Actual)
					Size = ReadBuffer -> SerialRequest -> IOSer . io_Actual;

				ReadBuffer -> SerialRequest -> IOSer . io_Command	= CMD_READ;
				ReadBuffer -> SerialRequest -> IOSer . io_Data		= ReadBuffer -> SerialBuffer + ReadBuffer -> SerialFilled;
				ReadBuffer -> SerialRequest -> IOSer . io_Length	= Size;

				DoIO(ReadBuffer -> SerialRequest);

				ReadBuffer -> SerialFilled += ReadBuffer -> SerialRequest -> IOSer . io_Actual;
			}
		}
	}

	if(Signals & SIG_SERWRITE)
	{
		struct SerialBuffer *Swap = ThisBuffer;

		WaitIO(ThisBuffer -> SerialRequest);

		ThisBuffer -> IsBusy		= FALSE;
		ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;

		ThisBuffer = NextBuffer;
		NextBuffer = Swap;
	}

}

	/* com_outfull(VOID):
	 *
	 *	Return number of bytes still to be transferred.
	 */

int
com_outfull(VOID)
{
	return(ThisBuffer -> SerialIndex - ThisBuffer -> SerialBuffer + NextBuffer -> SerialIndex - NextBuffer -> SerialBuffer);
}

	/* carrier(VOID):
	 *
	 *	Return current carrier status.
	 */

int
carrier(VOID)
{
	if(nocarrier)
		return(1);
	else
	{
		if(!ThisBuffer -> IsBusy)
		{
			ThisBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_QUERY;

			DoIO(ThisBuffer -> SerialRequest);

			if(ThisBuffer -> SerialRequest -> io_Status & CIAF_COMCD)
				return(0);
			else
				return(1);
		}
		else
		{
			NextBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_QUERY;

			DoIO(NextBuffer -> SerialRequest);

			if(NextBuffer -> SerialRequest -> io_Status & CIAF_COMCD)
				return(0);
			else
				return(1);
		}
	}
}

	/* com_flush(VOID):
	 *
	 *	Make sure all pending data gets written.
	 */

VOID
com_flush(VOID)
{
	if(ThisBuffer -> IsBusy)
	{
		WaitIO(ThisBuffer -> SerialRequest);

		ThisBuffer -> IsBusy		= FALSE;
		ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;
	}

	if(NextBuffer -> SerialIndex > NextBuffer -> SerialBuffer)
	{
		NextBuffer -> SerialRequest -> IOSer . io_Command	= CMD_WRITE;
		NextBuffer -> SerialRequest -> IOSer . io_Data		= NextBuffer -> SerialBuffer;
		NextBuffer -> SerialRequest -> IOSer . io_Length	= NextBuffer -> SerialIndex - NextBuffer -> SerialBuffer;

		DoIO(NextBuffer -> SerialRequest);

		NextBuffer -> SerialIndex = NextBuffer -> SerialBuffer;
	}
}

	/* com_putbyte(byte c):
	 *
	 *	Transmit a single byte, queueing it if necessary.
	 */

VOID
com_putbyte(byte c)
{
	if(ThisBuffer -> IsBusy)
	{
		if(NextBuffer -> SerialIndex + 1 >= NextBuffer -> SerialTop)
		{
			struct SerialBuffer *Swap = ThisBuffer;

			WaitIO(ThisBuffer -> SerialRequest);

			ThisBuffer -> IsBusy		= FALSE;
			ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;

			ThisBuffer = NextBuffer;
			NextBuffer = Swap;

			ThisBuffer -> IsBusy					= TRUE;
			ThisBuffer -> SerialRequest -> IOSer . io_Command	= CMD_WRITE;
			ThisBuffer -> SerialRequest -> IOSer . io_Data		= ThisBuffer -> SerialBuffer;
			ThisBuffer -> SerialRequest -> IOSer . io_Length	= ThisBuffer -> SerialIndex - ThisBuffer -> SerialBuffer;

			ClrSignal(SIG_SERWRITE);
			SendIO(ThisBuffer -> SerialRequest);
		}

		*NextBuffer -> SerialIndex++ = c;
	}
	else
	{
		if(ThisBuffer -> SerialIndex + 1 < ThisBuffer -> SerialTop)
		{
			*ThisBuffer -> SerialIndex++ = c;

			ThisBuffer -> IsBusy					= TRUE;
			ThisBuffer -> SerialRequest -> IOSer . io_Command	= CMD_WRITE;
			ThisBuffer -> SerialRequest -> IOSer . io_Data		= ThisBuffer -> SerialBuffer;
			ThisBuffer -> SerialRequest -> IOSer . io_Length	= 1;

			ClrSignal(SIG_SERWRITE);
			SendIO(ThisBuffer -> SerialRequest);
		}
		else
		{
			ThisBuffer -> IsBusy					= TRUE;
			ThisBuffer -> SerialRequest -> IOSer . io_Command	= CMD_WRITE;
			ThisBuffer -> SerialRequest -> IOSer . io_Data		= ThisBuffer -> SerialBuffer;
			ThisBuffer -> SerialRequest -> IOSer . io_Length	= ThisBuffer -> SerialIndex - ThisBuffer -> SerialBuffer;

			ClrSignal(SIG_SERWRITE);
			SendIO(ThisBuffer -> SerialRequest);

			*NextBuffer -> SerialIndex++ = c;
		}
	}
}

	/* com_purge(VOID):
	 *
	 *	Clear the read/write buffers.
	 */

VOID
com_purge(VOID)
{
	if(ThisBuffer -> IsBusy)
	{
		if(!CheckIO(ThisBuffer -> SerialRequest))
			AbortIO(ThisBuffer -> SerialRequest);

		WaitIO(ThisBuffer -> SerialRequest);
	}

	ThisBuffer -> IsBusy		= FALSE;
	ThisBuffer -> SerialIndex	= ThisBuffer -> SerialBuffer;

	NextBuffer -> IsBusy		= FALSE;
	NextBuffer -> SerialIndex	= NextBuffer -> SerialBuffer;

	if(ReadBuffer -> IsBusy)
	{
		if(!CheckIO(ReadBuffer -> SerialRequest))
			AbortIO(ReadBuffer -> SerialRequest);

		WaitIO(ReadBuffer -> SerialRequest);
	}

	ReadBuffer -> IsBusy		= FALSE;
	ReadBuffer -> SerialIndex	= ReadBuffer -> SerialBuffer;
	ReadBuffer -> SerialFilled	= 0;

	ThisBuffer -> SerialRequest -> IOSer . io_Command = CMD_CLEAR;
	DoIO(ThisBuffer -> SerialRequest);
}

	/* com_dump(VOID):
	 *
	 *	Wait for asynchronous write request to terminate.
	 */

VOID
com_dump(VOID)
{
	com_flush();
}

	/* com_getbyte(VOID):
	 *
	 *	Read a single byte from the serial line. If not available,
	 *	return EOF.
	 */

int
com_getbyte(VOID)
{
	int Result;

	if(ReadBuffer -> SerialFilled <= 0)
	{
		if(ReadBuffer -> IsBusy)
		{
			if(!CheckIO(ReadBuffer -> SerialRequest))
				return(EOF);
			else
				WaitIO(ReadBuffer -> SerialRequest);

			ReadBuffer -> IsBusy		= FALSE;
			ReadBuffer -> SerialFilled	= ReadBuffer -> SerialRequest -> IOSer . io_Actual;
			ReadBuffer -> SerialIndex	= ReadBuffer -> SerialBuffer;

			ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_QUERY;

			DoIO(ReadBuffer -> SerialRequest);

			if(ReadBuffer -> SerialRequest -> IOSer . io_Actual)
			{
				LONG Size = ReadBuffer -> SerialSize - ReadBuffer -> SerialFilled;

				if(Size > 0)
				{
					if(Size > ReadBuffer -> SerialRequest -> IOSer . io_Actual)
						Size = ReadBuffer -> SerialRequest -> IOSer . io_Actual;

					ReadBuffer -> SerialRequest -> IOSer . io_Command	= CMD_READ;
					ReadBuffer -> SerialRequest -> IOSer . io_Data		= ReadBuffer -> SerialBuffer + ReadBuffer -> SerialFilled;
					ReadBuffer -> SerialRequest -> IOSer . io_Length	= Size;

					DoIO(ReadBuffer -> SerialRequest);

					ReadBuffer -> SerialFilled += ReadBuffer -> SerialRequest -> IOSer . io_Actual;
				}
			}
		}
		else
		{
			ReadBuffer -> SerialRequest -> IOSer . io_Command = SDCMD_QUERY;

			DoIO(ReadBuffer -> SerialRequest);

			if(!ReadBuffer -> SerialRequest -> IOSer . io_Actual)
				return(EOF);
			else
			{
				ReadBuffer -> SerialRequest -> IOSer . io_Command	= CMD_READ;
				ReadBuffer -> SerialRequest -> IOSer . io_Data		= ReadBuffer -> SerialBuffer;
				ReadBuffer -> SerialRequest -> IOSer . io_Length	= MIN(ReadBuffer -> SerialRequest -> IOSer . io_Actual,ReadBuffer -> SerialSize);

				DoIO(ReadBuffer -> SerialRequest);

				if(!ReadBuffer -> SerialRequest -> IOSer . io_Actual)
					return(EOF);
				else
				{
					ReadBuffer -> SerialFilled	= ReadBuffer -> SerialRequest -> IOSer . io_Actual;
					ReadBuffer -> SerialIndex	= ReadBuffer -> SerialBuffer;
				}
			}
		}
	}

	Result = *ReadBuffer -> SerialIndex++;

	ReadBuffer -> SerialFilled--;

	if(ReadBuffer -> SerialFilled <= 0)
	{
		ReadBuffer -> IsBusy					= TRUE;
		ReadBuffer -> SerialIndex				= ReadBuffer -> SerialBuffer;

		ReadBuffer -> SerialRequest -> IOSer . io_Command	= CMD_READ;
		ReadBuffer -> SerialRequest -> IOSer . io_Data		= ReadBuffer -> SerialBuffer;
		ReadBuffer -> SerialRequest -> IOSer . io_Length	= 1;

		ClrSignal(SIG_SERREAD);
		SendIO(ReadBuffer -> SerialRequest);
	}

	return(Result);
}

	/* setstamp(STRPTR Name,LONG Time):
	 *
	 *	Set time/date of a file.
	 */

VOID
setstamp(char *Name,long Time)
{
        struct tm *t;
	struct ClockData ClockData;
	ULONG Seconds;
	struct DateStamp Date;

        t = localtime(&Time);

	ClockData . sec		= t -> tm_sec;
	ClockData . min		= t -> tm_min;
	ClockData . hour	= t -> tm_hour;
	ClockData . mday	= t -> tm_mday;
	ClockData . month	= t -> tm_mon;
	ClockData . year	= t -> tm_year + 1900;

	Seconds = Date2Amiga(&ClockData);

	Date . ds_Days		= Seconds / (24 * 60 * 60);
	Date . ds_Minute	= (Seconds % (24 * 60 * 60)) / 60;
	Date . ds_Tick		= (Seconds % 60) * TICKS_PER_SECOND;

	SetFileDate(Name,&Date);
}

	/* freespace(STRPTR DrivePath):
	 *
	 *	Get free disk space for specified drive.
	 */

long
freespace(char *DrivePath)
{
	struct DevProc	*DevProc = GetDeviceProc(DrivePath,NULL);
	struct DosList	*DosList;
	BOOL		 GoodDevice = FALSE;
	LONG		 Size = (LONG)((ULONG)~0 >> 2);

	if(DosList = LockDosList(LDF_DEVICES | LDF_READ))
	{
		while(DosList = NextDosEntry(DosList,LDF_DEVICES))
		{
			if(DosList -> dol_Task == DevProc -> dvp_Port)
			{
				struct FileSysStartupMsg *FSSM = (struct FileSysStartupMsg *)BADDR(DosList -> dol_misc . dol_handler . dol_Startup);

				if(TypeOfMem(FSSM))
				{
					struct DosEnvec *DosEnvec = (struct DosEnvec *)BADDR(FSSM -> fssm_Environ);
					STRPTR Name = (STRPTR)BADDR(FSSM -> fssm_Device);

					if(TypeOfMem(DosEnvec) && TypeOfMem(Name))
					{
						if(Name[0] > 0 && !Name[(WORD)Name[0] + 1])
						{
							struct IOStdReq __aligned IORequest;

							if(!OpenDevice(Name + 1,FSSM -> fssm_Unit,&IORequest,FSSM -> fssm_Unit))
							{
								CloseDevice(&IORequest);

								if(DosEnvec -> de_TableSize > 0 && DosEnvec -> de_LowCyl <= DosEnvec -> de_HighCyl)
									GoodDevice = TRUE;
							}
						}
					}
				}
			}
		}

		UnLockDosList(LDF_DEVICES | LDF_READ);
	}

	FreeDeviceProc(DevProc);

	if(GoodDevice)
	{
		struct InfoData *InfoData;

		if(InfoData = (struct InfoData *)AllocVec(sizeof(struct InfoData),MEMF_ANY | MEMF_PUBLIC))
		{
			UBYTE NewName[256],*Index;
			BPTR FileLock;

			memcpy(NewName,DrivePath,255);

			NewName[255] = 0;

			Index = PathPart(NewName);

			*Index = 0;

			FileLock = Lock(NewName,ACCESS_READ);

			if(FileLock)
			{
				if(Info(FileLock,InfoData))
					Size = InfoData -> id_BytesPerBlock * (InfoData -> id_NumBlocks - InfoData -> id_NumBlocksUsed);

				UnLock(FileLock);
			}

			FreeVec(InfoData);
		}
	}

	return(Size);
}

	/* ffirst(char *FileSpec):
	 *
	 *	Return name of first file matching the given specs.
	 */

char *
ffirst(char *filespec)
{
	AnchorUsed = TRUE;

	if(MatchFirst(filespec,Anchor))
		return(NULL);
	else
		return((char *)Anchor -> ap_Buf);
}

	/* fnext(VOID):
	 *
	 *	Return name of next file matching the given specs.
	 */

char *
fnext(VOID)
{
	AnchorUsed = TRUE;

	if(MatchNext(Anchor))
		return(NULL);
	else
		return((char *)Anchor -> ap_Buf);
}
