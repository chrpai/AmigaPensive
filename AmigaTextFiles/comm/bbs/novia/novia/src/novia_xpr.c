#include <exec/types.h>

#include <novia/novia_global.h>
#include <novia/novia_misc.h>
#include <novia/novia_funcs.h>

#include <pragma/xpr_lib.h>
#include <pragma/gadtools_lib.h>
#include <graphics/gfxmacros.h>
#include <libraries/xprzmodem.h>
#include <libraries/xproto.h>

struct Library *XProtocolBase;
XPR_IO xprio;
LONG warnung;
UWORD lastgs;
void InitXPrReq();

VOID wprintf(struct RastPort *rp, UWORD x, UWORD y, UBYTE *string, ...)
{
	UBYTE buffer[256];

	vsprintf(buffer, string, (va_list)(&string + 1));

	Move(rp, x, y+6);
	Text(rp, buffer, strlen(buffer));
}


void settimer(long micros)
{
	struct PortData *cport;
	if ((cport = (PortData *) FindTask(NULL)->tc_UserData))
	{
		int secs=0;
		if (micros > 1000000)
		{
			secs	= micros / 1000000;
			micros = micros % 1000000;
		}
		if (!CheckIO((IORequest *)cport->TimerReq))
		{
			AbortIO((IORequest *)cport->TimerReq);
			WaitIO((IORequest *)cport->TimerReq);
		}
		cport->TimerReq->tr_time.tv_micro=micros; 
		cport->TimerReq->tr_time.tv_secs=secs;
		cport->TimerReq->tr_node.io_Command=TR_ADDREQUEST;
		SendIO((IORequest *)cport->TimerReq);
	}
}

BOOL OpenXPrWindow()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct Gadget		*glist, *Gadgets[2], *g;
	struct NewGadget	ng;

	if(cport->xprwin)	return(TRUE);

	if(cport->xprwin = OpenWindowTags(NULL,
		WA_Width,			600,
		WA_Height,			184,
		WA_Left,				(cport->win->WScreen->Width - 600) >> 1,
		WA_Top,				(cport->win->WScreen->Height - 184) >> 1,
		WA_Activate,		FALSE,
		WA_DragBar,			TRUE,
		WA_DepthGadget,	TRUE,
		WA_CloseGadget,	TRUE,
		WA_RMBTrap,			TRUE,
		WA_IDCMP,			IDCMP_CLOSEWINDOW | IDCMP_GADGETUP,
		WA_Title,			"NOViA ZModem Transfer Window",
//		WA_CustomScreen,	cport->win->WScreen,
		TAG_DONE))
	{
//		BlockWindow(Win1);
//		BlockWindow(Win2);

#define LEFTX 15
#define RIGHTX 310

		SetAPen(cport->xprwin->RPort, (cport->clientconfig.Colors == 1 ? 1 : 2));

		wprintf(cport->xprwin->RPort, LEFTX,		18,	"Selected Protocol.:");
		wprintf(cport->xprwin->RPort, LEFTX,		31,	"Last XPr-Message..:");
		wprintf(cport->xprwin->RPort, LEFTX,		41,	"Last XPr-Error....:");
		wprintf(cport->xprwin->RPort, LEFTX,		54,	"Current Filename..:");
		wprintf(cport->xprwin->RPort, LEFTX,		64,	"Current Filesize..:");
		wprintf(cport->xprwin->RPort, LEFTX,		81,	"Bytes Transferred.:");
		wprintf(cport->xprwin->RPort, LEFTX,		91,	"Blocks Transferred:");
		wprintf(cport->xprwin->RPort, LEFTX,		101,	"Block Size........:");
		wprintf(cport->xprwin->RPort, LEFTX,		111,	"Block Check Type..:");
		wprintf(cport->xprwin->RPort, LEFTX,		127,	"Estimated Time....:");
		wprintf(cport->xprwin->RPort, LEFTX,		137,	"Elapsed Time......:");
		wprintf(cport->xprwin->RPort, RIGHTX,	81,	"Data Rate [CPS]...:");
		wprintf(cport->xprwin->RPort, RIGHTX,	91,	"Charakter Delay...:");
		wprintf(cport->xprwin->RPort, RIGHTX,	101,	"Packet Delay......:");
		wprintf(cport->xprwin->RPort, RIGHTX,	111,	"Packet Type.......:");
		wprintf(cport->xprwin->RPort, RIGHTX,	127,	"Number Of Errors..:");
		wprintf(cport->xprwin->RPort, RIGHTX,	137,	"Number Of Timeouts:");

#define XLEFT	178
#define XRIGHT	472

		SetAPen(cport->xprwin->RPort, 7);

		wprintf(cport->xprwin->RPort, XLEFT,		18,	"- unknown -");
		wprintf(cport->xprwin->RPort, XLEFT,		31,	"- none -");
		wprintf(cport->xprwin->RPort, XLEFT,		41,	"- none -");
		wprintf(cport->xprwin->RPort, XLEFT,		54,	"- unknown -");
		wprintf(cport->xprwin->RPort, XLEFT,		64,	"-");
		wprintf(cport->xprwin->RPort, XLEFT,		81,	"-");
		wprintf(cport->xprwin->RPort, XLEFT,		91,	"-");
		wprintf(cport->xprwin->RPort, XLEFT,		101,	"-");
		wprintf(cport->xprwin->RPort, XLEFT,		111,	"--");
		wprintf(cport->xprwin->RPort, XLEFT,		127,	"--:--:--");
		wprintf(cport->xprwin->RPort, XLEFT,		137,	"--:--:--");
		wprintf(cport->xprwin->RPort, XRIGHT,	81,	"-");
		wprintf(cport->xprwin->RPort, XRIGHT,	91,	"-");
		wprintf(cport->xprwin->RPort, XRIGHT,	101,	"-");
		wprintf(cport->xprwin->RPort, XRIGHT,	111,	"--");
		wprintf(cport->xprwin->RPort, XRIGHT,	127,	"-");
		wprintf(cport->xprwin->RPort, XRIGHT,	137,	"-");

		DrawBevelBox(cport->xprwin->RPort, XLEFT - 8, 15, 416, 59,
			GTBB_Recessed,	TRUE,
			GT_VisualInfo,	cport->vinfo,
			TAG_DONE );
		DrawBevelBox(cport->xprwin->RPort, XLEFT - 8, 78, 122, 43,
			GTBB_Recessed,	TRUE,
			GT_VisualInfo,	cport->vinfo,
			TAG_DONE );
		DrawBevelBox(cport->xprwin->RPort, XRIGHT - 8, 78, 122, 43,
			GTBB_Recessed,	TRUE,
			GT_VisualInfo,	cport->vinfo,
			TAG_DONE );
		DrawBevelBox(cport->xprwin->RPort, XLEFT - 8, 124, 122, 23,
			GTBB_Recessed,	TRUE,
			GT_VisualInfo,	cport->vinfo,
			TAG_DONE );
		DrawBevelBox(cport->xprwin->RPort, XRIGHT - 8, 124, 122, 23,
			GTBB_Recessed,	TRUE,
			GT_VisualInfo,	cport->vinfo,
			TAG_DONE );

		DrawBevelBox(cport->xprwin->RPort, LEFTX, 152, 571, 10,
			GTBB_Recessed,	TRUE,
			GT_VisualInfo,	cport->vinfo,
			TAG_DONE );

		SetAPen(cport->xprwin->RPort, 0);
		RectFill(cport->xprwin->RPort, LEFTX + 2, 153, 564 + LEFTX + 2, 160);
		SetAPen(cport->xprwin->RPort, 7);

/*		if(g = CreateContext(&glist))
		{
			ng.ng_LeftEdge		= LEFTX + 24;
			ng.ng_TopEdge		= 167;
			ng.ng_Width			= 200;
			ng.ng_Height		= 11;
			ng.ng_GadgetText	= "Abort Entire Transfer";
//			ng.ng_TextAttr		= &FontAttr;
			ng.ng_GadgetID		= 1;
			ng.ng_Flags			= PLACETEXT_IN;
			ng.ng_VisualInfo	= cport->vinfo;

			Gadgets[0] =
			g = CreateGadget(BUTTON_KIND, g, &ng,
					TAG_DONE);

			ng.ng_LeftEdge		= LEFTX + 344;
			ng.ng_GadgetText	= "Skip Current File";
			ng.ng_GadgetID		= 2;

			Gadgets[1] =
			g = CreateGadget(BUTTON_KIND, g, &ng,
					TAG_DONE);

			AddGList(cport->xprwin, glist, 0, -1, NULL);
			RefreshGList(glist, cport->xprwin, NULL, -1);
			GT_RefreshWindow(cport->xprwin, NULL);
		}*/

		return(TRUE);
	}

	return(FALSE);
}


void CloseXPrWindow()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if(cport->xprwin)
	{
//		FreeGadgets(cport->xprwin->FirstGadget);

//		ReleaseWindow(Win1);
//		ReleaseWindow(Win2);

		CloseWindow(cport->xprwin);
		cport->xprwin = NULL;
	}
}



extern "Asm" LONG xpr_fopen(register __a3 struct XPR_IO *io,register __a0 char *filename,register __a1 char *accessmode)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	BPTR fh		= NULL;		// filehandle of opening file
	BPTR lock	= NULL;		// test lock
	if (cport->xprwin) EraseRect(cport->xprwin->RPort,21,91,499,100); lastgs=0;
	switch(*accessmode)
	{
		case 'r':								// Open as read
			if (accessmode[1]=='+')
				fh = Open(filename, MODE_READWRITE);
			else
				fh = Open(filename, MODE_OLDFILE);
			break;
		case 'w':								// Open as write
			if(accessmode[1] == '+')
			{
				// first, locking for exist file
				if(lock = Lock(filename, ACCESS_WRITE))
				{
					UnLock(lock);				// unlock and delete old file
					DeleteFile(filename);
				}
				fh = Open(filename, MODE_READWRITE);
			}
			else
				fh = Open(filename, MODE_NEWFILE);
			break;
		case 'a':								// Open file as writewrite (to add data)
			if(lock = Lock(filename, ACCESS_WRITE))
			{
				UnLock(lock);
				if(fh = Open(filename, MODE_READWRITE))
				{
					if(Seek(fh, 0, OFFSET_END) == -1)
					{
						Close(fh);
						fh = NULL;
					}
				}
			}
			else
				fh = Open(filename, MODE_NEWFILE);
			break;
	}
	return((LONG)fh);
}


extern "Asm" LONG xpr_fclose(register __a3 struct XPR_IO *io,register __a0 void *fileptr)		/* this function close the file */
{
	if(fileptr) Close((BPTR)fileptr);
	return(0);
}




/*** next function is to read data from file ***/

extern "Asm" LONG xpr_fread(register __a3 struct XPR_IO *io,register __a0 char *buffer,register __d0 LONG size,register __d1 LONG count,register __a1 void *fileptr)
{
	return(Read((BPTR)fileptr, buffer, size * count));   // ??? /size ???
}




/*** this functions write data in file ***/

extern "Asm" LONG xpr_fwrite(register __a3 struct XPR_IO *io,register __a0 char *buffer,register __d0 LONG size,register __d1 LONG count,register __a1 void *fileptr)
{
	return(Write((BPTR)fileptr, buffer, size * count));  // ??? /size ???
}





/*** this function read data from serial.device ***/

extern "Asm" LONG xpr_sread(register __a3 struct XPR_IO *io,register __a0 char *buffer,register __d0 LONG size,register __d1 LONG timeout)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;		// get cport structure
	ULONG	signalset,													// Recieve SignalMask
			sersig,														// SerRead SignalBit
			winsig,														// SigBit for IDCMP-Messages
			timesig;														// TimeReq SignalBit
	ULONG	length = 0;													// datalength in seralbuffer

	struct SerialNetwork *sn;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen && cport->network.online)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{
					if (!timeout)														// == no timeout
					{
						sn->SerQuery->IOSer.io_Command	= SDCMD_QUERY;
						DoIO((IORequest *)sn->SerQuery);								// read device status
				
						if ((length = sn->SerQuery->IOSer.io_Actual))				// check for data in seria-buffer ?
						{
							if (length > (ULONG)size)	length = (ULONG)size;
							sn->SerRead->IOSer.io_Length		= length;
							sn->SerRead->IOSer.io_Data		= buffer;
							sn->SerRead->IOSer.io_Command	= CMD_READ;
							DoIO((IORequest *)sn->SerRead);							// copy serialdate to buffer
							length = sn->SerRead->IOSer.io_Actual;
						}
						
					}
					else																// timeout ?
					{
						char quit=FALSE;
						settimer(timeout);												// reset timer
				
						sn->SerRead->IOSer.io_Command	= CMD_READ;
						sn->SerRead->IOSer.io_Data		= buffer;
						sn->SerRead->IOSer.io_Length		= size;
				
						sersig	= 1 << sn->SerReadPort->mp_SigBit;					// Get SerRead SigBit
						timesig	= 1 << cport->TimePort->mp_SigBit;						// Get TimeReq SigBit
						if (cport->xprwin)
							winsig= 1 << cport->xprwin->UserPort->mp_SigBit;			// Get Window UserPort SigBit
						else
							winsig=0;
						SetSignal(0, sersig | timesig);
				
						SendIO((IORequest *)sn->SerRead);
				
						signalset = Wait(sersig | timesig | winsig);
				
						if (cport->ProgramClose)
						{
							AbortIO((IORequest *)sn->SerRead);
							WaitIO((IORequest *)sn->SerRead);
							AbortIO((IORequest *)cport->TimerReq);
							WaitIO((IORequest *)cport->TimerReq);
							return (-1);
						}
						else
						{
							if (sersig & signalset)
							{
								length=sn->SerRead->IOSer.io_Actual;
				//				if (!(CheckIO((IORequest *)cport->TimerReq)))		// Abort TimeReq
								{
									AbortIO((IORequest *)cport->TimerReq);			
									WaitIO((IORequest *)cport->TimerReq);
								}
							}
							if (timesig & signalset)
							{
				//				if (!(CheckIO((IORequest *)sn->SerRead)))
								{
									AbortIO((IORequest *)sn->SerRead);
									WaitIO((IORequest *)sn->SerRead);
								}
								length = sn->SerRead->IOSer.io_Actual;
							}
						}
					}
				}
			}
		}
	}
	return length;
}



/*** function send buffer to serial.device ***/

extern "Asm" LONG xpr_swrite(register __a3 struct XPR_IO *io,register __a0 char *buffer,register __d0 LONG size)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;		// get userdate

	struct SerialNetwork *sn;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen && cport->network.online)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{
					if (size)															// check size
					{
						sn->SerWrite->IOSer.io_Length  = size;
						sn->SerWrite->IOSer.io_Data    = buffer;
						sn->SerWrite->IOSer.io_Command = CMD_WRITE;

						DoIO((struct IORequest *)sn->SerWrite);							// Send buffer
						return((long)sn->SerWrite->IOSer.io_Error);
					}
				}
			}
		}
	}
	else
		return (-1);
}


/*** function to flush serial.device ***/

extern "Asm" LONG xpr_sflush(register __a3 struct XPR_IO *io)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct SerialNetwork *sn;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen && cport->network.online)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{

					sn->SerQuery->IOSer.io_Command = CMD_FLUSH;
					DoIO((struct IORequest *)sn->SerQuery);
					return((long)sn->SerQuery->IOSer.io_Error);
				}
			}
		}
	}
}


/*** this function update the xpr-window ***/

extern "Asm" LONG xpr_update(register __a3 struct XPR_IO *io,register __a0 struct XPR_UPDATE *Update)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if(!cport->xprwin)	return(0);

	if(Update)
	{
		if(Update->xpru_updatemask & XPRU_PROTOCOL)
			wprintf(cport->xprwin->RPort, XLEFT, 18, "%-50.50ls", Update->xpru_protocol);

		if(Update->xpru_updatemask & XPRU_MSG)
			wprintf(cport->xprwin->RPort, XLEFT, 31, "%-50.50ls", Update->xpru_msg);

		if(Update->xpru_updatemask & XPRU_ERRORMSG)
			wprintf(cport->xprwin->RPort, XLEFT, 41, "%-50.50ls", Update->xpru_errormsg);

		if(Update->xpru_updatemask & XPRU_FILENAME)
			wprintf(cport->xprwin->RPort, XLEFT, 54, "%-50.50ls", Update->xpru_filename);

		if(Update->xpru_updatemask & XPRU_FILESIZE)
		{
			wprintf(cport->xprwin->RPort, XLEFT, 64, "%-50.50ld", Update->xpru_filesize);
			warnung = Update->xpru_filesize;
		}

		if(Update->xpru_updatemask & XPRU_BYTES)
		{
			wprintf(cport->xprwin->RPort, XLEFT, 81, "%-13.13ld", Update->xpru_bytes);
			if(warnung > 0)
			{
				ULONG Check = 0x5555AAAA;			// __chip

				SetAfPt(cport->xprwin->RPort, (UWORD *)&Check, 1);
				RectFill(cport->xprwin->RPort, LEFTX + 2, 153, ((Update->xpru_bytes * 563) / warnung) + LEFTX + 3, 160);
				SetAfPt(cport->xprwin->RPort, NULL, 0);
			}
		}

		if(Update->xpru_updatemask & XPRU_BLOCKS)
			wprintf(cport->xprwin->RPort, XLEFT, 91, "%-13.13ld", Update->xpru_blocks);

		if(Update->xpru_updatemask & XPRU_BLOCKSIZE)
			wprintf(cport->xprwin->RPort, XLEFT, 101, "%-13.13ld", Update->xpru_blocksize);

		if(Update->xpru_updatemask & XPRU_BLOCKCHECK)
			wprintf(cport->xprwin->RPort, XLEFT, 111, "%-13.13ls", Update->xpru_blockcheck);

		if(Update->xpru_updatemask & XPRU_EXPECTTIME)
			wprintf(cport->xprwin->RPort, XLEFT, 127, "%-13.13ls", Update->xpru_expecttime);

		if(Update->xpru_updatemask & XPRU_ELAPSEDTIME)
			wprintf(cport->xprwin->RPort, XLEFT, 137, "%-13.13ls", Update->xpru_elapsedtime);

		if(Update->xpru_updatemask & XPRU_DATARATE)
			wprintf(cport->xprwin->RPort, XRIGHT, 81, "%-13.13ld", Update->xpru_datarate);

		if(Update->xpru_updatemask & XPRU_CHARDELAY)
			wprintf(cport->xprwin->RPort, XRIGHT, 91, "%-13.13ld", Update->xpru_chardelay);

		if(Update->xpru_updatemask & XPRU_PACKETDELAY)
			wprintf(cport->xprwin->RPort, XRIGHT, 101, "%-13.13ld", Update->xpru_packetdelay);

		if(Update->xpru_updatemask & XPRU_PACKETTYPE)
			wprintf(cport->xprwin->RPort, XRIGHT, 111, "%-13.13ls", Update->xpru_packettype);

		if(Update->xpru_updatemask & XPRU_ERRORS)
			wprintf(cport->xprwin->RPort, XRIGHT, 127, "%-13.13ld", Update->xpru_errors);

		if(Update->xpru_updatemask & XPRU_TIMEOUTS)
			wprintf(cport->xprwin->RPort, XRIGHT, 137, "%-13.13ld", Update->xpru_timeouts);
	}

	return(1);
}


/*** function to check AbbortTransfer from XPR-Window Close Gadget ***/

extern "Asm" LONG xpr_chkabort(register __a3 struct XPR_IO *io)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct IntuiMessage *imsg;
	struct Gadget *gadget;
	ULONG class;

	if(cport->xprwin)
	{
		struct IntuiMessage	*imsg=NULL;
		while((imsg = (struct IntuiMessage *)GetMsg(cport->xprwin->UserPort)))
		{
			if (imsg->Class==IDCMP_MENUPICK)
			{
				return(-1);	/* Abort transfer */
//				return(1);	/* Skip file */
			}
			ReplyMsg((struct Message *)imsg);
		}
		if (cport->ProgramClose | !cport->network.online)
			return(-1);
	}
	return(0);	// no abort
}



/***		the following functions are not implemented yet.

 extern "Asm" LONG xpr_chkmisc(register __a3 struct XPR_IO *io)
 extern "Asm" LONG xpr_gets(register __a3 struct XPR_IO *io,register __a0 char *prompt,register __a1 char *buffer)

***/

extern "Asm" LONG xpr_setserial(register __a3 struct XPR_IO *io,register __d0 LONG newstatus)
{
#define ST_PARTYON	(1 << 0)
#define ST_PARTYODD	(1 << 1)
#define ST_7WIRE	(1 << 2)
#define ST_QBREAK	(1 << 3)
#define ST_RADBOOGIE	(1 << 4)
#define ST_SHARED	(1 << 5)
#define ST_EOFMODE	(1 << 6)
#define ST_XDISABLED	(1 << 7)
#define ST_PARTYMARKON	(1 << 8)
#define ST_PARTYMARK	(1 << 9)
#define ST_2BITS	(1 << 10)
#define ST_READ7	(1 << 11)
#define ST_WRITE7	(1 << 12)
/*	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;

	LONG rtn, i;
	STATIC LONG XprBauds[12] = { 110, 300, 1200, 2400, 4800, 9600, 19200, 31250,
		38400, 57600, 76800, 115200 };


	sn->SerWrite->IOSer.io_Command = SDCMD_QUERY;
	DoIO((IORequest *)sn->SerWrite);

	rtn = sn->SerWrite->io_SerFlags & 0xFF;

	if(sn->SerWrite->io_ExtFlags & SEXTF_MSPON)	rtn |= ST_PARTYMARKON;
	if(sn->SerWrite->io_ExtFlags & SEXTF_MARK)		rtn |= ST_PARTYMARK;
	if(sn->SerWrite->io_StopBits	== 2)				rtn |= ST_2BITS;
	if(sn->SerWrite->io_ReadLen		== 7)				rtn |= ST_READ7;
	if(sn->SerWrite->io_WriteLen	== 7)				rtn |= ST_WRITE7;

	for(i = 0 ; i < 12 ; i++)
	{
		if(sn->SerWrite->io_Baud == XprBauds[i])
		{
			rtn |= (i << 16);
			break;
		}
	}

	if(Status != -1)
	{
		sn->SerWrite->IOSer.io_Command	= SDCMD_SETPARAMS;
		sn->SerWrite->io_SerFlags		= Status & 0xFF;
		sn->SerWrite->io_ExtFlags		= 0;

		if(Status & ST_PARTYMARKON)	sn->SerWrite->io_ExtFlags |= SEXTF_MSPON;
		if(Status & ST_PARTYMARK)		sn->SerWrite->io_ExtFlags |= SEXTF_MARK;
		if(Status & ST_2BITS)			sn->SerWrite->io_StopBits = 2;
		else									sn->SerWrite->io_StopBits = 1;
		if(Status & ST_READ7)			sn->SerWrite->io_ReadLen = 7;
		else									sn->SerWrite->io_ReadLen = 8;
		if(Status & ST_WRITE7)			sn->SerWrite->io_WriteLen = 7;
		else									sn->SerWrite->io_WriteLen = 8;

		DoIO((IORequest *)sn->SerWrite);

		sn->SerRead->io_SerFlags	= sn->SerWrite->io_SerFlags;
		sn->SerRead->io_ExtFlags	= sn->SerWrite->io_ExtFlags;
		sn->SerRead->io_StopBits	= sn->SerWrite->io_StopBits;
		sn->SerRead->io_ReadLen	= sn->SerWrite->io_ReadLen;
		sn->SerRead->io_WriteLen	= sn->SerWrite->io_WriteLen;
	}

	return(rtn);*/
	return NULL;
}


/*** this function get the first filename from download list ***/

extern "Asm" LONG xpr_ffirst(register __a3 struct XPR_IO *io,register __a0 char *buffer,register __a1 char *pattern)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	printf("ffirst: %s\n",pattern);
	if (cport)
	{
		struct Item item;
		if (cport->actual)
		{
//			if (GetItem_byIP(cport->actual->dirIP,cport->actual->itemIP,(Item *)&item,sizeof(Item)))
			{
				if (item.itemheader.ItemType == ITEMTYPE_FILEITEM)
				{
					strcpy(buffer,item.Filename);
					printf("download : %s\n",buffer);
					warnung=item.Size;
					return (1);
				}
			}
		}
	}
	return(0);
}


extern "Asm" LONG xpr_fnext(register __a3 struct XPR_IO *io,register __d0 LONG oldstate,register __a0 char *buffer,register __a1 char *pattern)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;

	printf("fnext: (%d) %s\n",oldstate,pattern);

	SetAPen(cport->xprwin->RPort, 0);
	RectFill(cport->xprwin->RPort, LEFTX + 2, 153, 564 + LEFTX + 2, 160);
	SetAPen(cport->xprwin->RPort, 7);

	if (cport)
	{
		struct SelectItem *select=cport->actual->ln_Succ;
		char quit=FALSE;
		while (select->ln_Succ && !quit)
		{
			if (select->downloadnow)
			{
				cport->actual = select;
				quit = TRUE;
			}
			select=select->ln_Succ;
		}
		if (quit)
		{
			struct Item item;
			if (cport->actual)
			{
//				if (GetItem_byIP(cport->actual->dirIP,cport->actual->itemIP,(Item *)&item,sizeof(Item)))
				{
					if (item.itemheader.ItemType == ITEMTYPE_FILEITEM)
					{
						strcpy(buffer,item.Filename);
						printf("next download : %s\n",buffer);
						warnung=item.Size;
						return (1);
					}
				}
			}
		}
	}
	return(0);
}


extern "Asm" LONG xpr_finfo(register __a3 struct XPR_IO *io,register __a0 char *filename,register __d0 LONG typeofinfo)
{
	struct FileInfoBlock *fib;
	LONG result = NULL;
	if ((fib = AllocDosObject(DOS_FIB,NULL)))		// Alloc FileInfoBlock
	{
		BPTR lock;
		if ((lock = Lock(filename, SHARED_LOCK)))
		{
			if (Examine(lock, fib))
				result = fib->fib_Size;
			UnLock(lock);
			if (typeofinfo == 2)
				result=1;
			else
			{
				if(typeofinfo!=1)
					result = 0;
			}
		}
		FreeDosObject(DOS_FIB,fib);
	}
	return(result);
}


/*** file to set position of file ***/

extern "Asm" LONG xpr_fseek(register __a3 struct XPR_IO *io,register __a0 void *fileptr,register __d0 LONG offset,register __d1 LONG origin)
{
	LONG accessmode = NULL;
	switch(origin)
	{
		case 0:
			accessmode = OFFSET_BEGINNING;
			break;
		case 1:
			accessmode = OFFSET_CURRENT;
			break;
		case 2:
			accessmode = OFFSET_END;
			break;
	}

	if (Seek((BPTR)fileptr, offset, accessmode) == -1)
		return(-1);
	else
		return(0);
}

/***
  LONG__saveds __asm xpr_options(register __a3 struct XPR_IO *io,register __d0 LONG n,register __a0 struct xpr_option **opt)
***/


/*** function to delete a file ***/

extern "Asm" LONG xpr_unlink(register __a3 struct XPR_IO *io,register __a0 char *filename)
{
	return(DeleteFile(filename));
}



extern "Asm" LONG xpr_squery(register __a3 struct XPR_IO *io)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct SerialNetwork *sn;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen && cport->network.online)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{
					sn->SerQuery->IOSer.io_Command = SDCMD_QUERY;
					if (!(DoIO((IORequest *)sn->SerRead)))
						return (sn->SerRead->IOSer.io_Actual);
					else
						return(-1);
				}
			}
		}
	}
	return NULL;
}





/*
extern "Asm" LONG xpr_getptr(register __a3 struct XPR_IO *io,register __d0 LONG type)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	return((type == 1 ? (LONG)cport->scr : -1));
}

*/

BOOL DoTransfer()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct Line *line;
	BPTR newlock, oldlock;
	LONG i;
	BOOL ok;
	char path[]="ram:";
	char buffer[200];
	struct SerialNetwork *sn;
	xprio.xpr_filename = 0;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen && cport->network.online)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{
					InitXPrReq();
					if (cport)
					{
						struct Item item;
						if (cport->actual)
						{
//							if (GetItem_byIP(cport->actual->dirIP,cport->actual->itemIP,(Item *)&item,sizeof(Item)))
							{
								if (item.itemheader.ItemType == ITEMTYPE_FILEITEM)
								{
									xprio.xpr_filename = item.Filename;
			
									printf("download : %s\n",buffer);
									warnung=item.Size;
									return (1);
								}
								printf(" no file \n");
							}
							printf(" get io failat %d , %d\n",cport->actual->dirIP,cport->actual->itemIP);
						}
						else
							printf(" no actual \n");
					}
			
			
					ok = FALSE;
					if(XProtocolBase = OpenLibrary("xprzmodem.library", 0))
					{
						ULONG		XPrFlags;
			
						XPrFlags = XProtocolSetup(&xprio);
						if(XPrFlags & XPRS_SUCCESS)
						{
							printf("sending \n");
							AbortIO((IORequest *)sn->SerRead);
							WaitIO((IORequest *)sn->SerRead);
							AbortIO((IORequest *)sn->SerWrite);
							WaitIO((IORequest *)sn->SerWrite);
				
							OpenXPrWindow();
				
				
							warnung = xpr_finfo(0,0,0);
							ok = XProtocolSend(&xprio);
				
				/*			warnung = 0;
							ok = XProtocolReceive(&xprio);
							warnung = i;*/
				
							printf("ok: %d i: %d\n",ok,i);
				
							CloseXPrWindow();
						}
						else
							printf("DoTransfer: XPrSetup() - Error\n");
				
						XProtocolCleanup(&xprio);
						CloseLibrary(XProtocolBase);
						XProtocolBase = NULL;
					}
					else
						printf("DoTransfer Konnte xpr.library nicht öffnen.\n");
				
					printf("Transfer Status : %ls\r\n", (ok ? "ok" : "failed"));
				
					printf("Transfer beendet");
				}
				return(FALSE);
			}
		}
	}
}


VOID InitXPrReq(VOID)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
   xprio.xpr_filename	= NULL;
	xprio.xpr_fopen		= xpr_fopen;
	xprio.xpr_fclose		= xpr_fclose;
	xprio.xpr_fread		= (long)xpr_fread;
	xprio.xpr_fwrite		= (long)xpr_fwrite;
	xprio.xpr_sread		= (long)xpr_sread;
	xprio.xpr_swrite		= (long)xpr_swrite;
	xprio.xpr_sflush		= (long)xpr_sflush;
	xprio.xpr_update		= (long)xpr_update;
	xprio.xpr_chkabort	= (long)xpr_chkabort;
	xprio.xpr_gets			= NULL;	 			// brauchen keine string-req
	xprio.xpr_setserial	= (long)NULL;
	xprio.xpr_ffirst		= (long)xpr_ffirst;
	xprio.xpr_fnext		= (long)xpr_fnext;
	xprio.xpr_finfo		= (long)xpr_finfo;
	xprio.xpr_fseek		= (long)xpr_fseek;
    xprio.xpr_data		= NULL;
	xprio.xpr_extension	= 4;
	xprio.xpr_options		= NULL;				// brauchen kein opt-window
	xprio.xpr_unlink		= (long)NULL;
	xprio.xpr_squery		= (long)NULL;
	xprio.xpr_getptr		= (long)NULL;		// Extension 4
	xprio.xpr_chkmisc		= NULL;				// brauchen das nicht...
}
