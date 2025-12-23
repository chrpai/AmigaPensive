#include <exec/types.h>
#include <stdlib.h>
#include <exec/ports.h>
#include <exec/io.h>
#include <exec/tasks.h>
#include <exec/lists.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <pragma/intuition_lib.h>
#include <pragma/gadtools_lib.h>
#include <novia/novia_UserList.h>
#include <novia/novia_PortData.h>
#include <novia/novia_config.h>
#include <novia/novia_vde.h>
#include <novia/novia_mail.h>
#include <novia/novia_subboard.h>
#include <novia/novia_types.h>
#include <novia/novia_misc.h>
#include <novia/novia_message.h>
#include <novia/novia_gui.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <string.h>
#include <stdio.h>
#include <intuition/intuition.h>
#include <rexx/storage.h>
#include <clib/rexxsyslib_protos.h>
#include <novia/novia_funcs.h>
#include <pragma/noviasys_lib.h>

//#define DEBUG

/* **** NOVIA_MAIN.C IMPORTS **** */

extern struct Task *MainTask;
extern struct MsgPort *GUIPort;
extern struct MsgPort *MainGUIPort;
extern struct MsgPort *main_eventport;
extern struct MainPortConfig *mainportconfig;
extern struct List *client_list;


/* **** PROTOTYPES **** */

BOOL SN_Open();
void SN_Close();
BOOL SN_CheckNetworkStatus();
LONG SN_Send(APTR data, ULONG len);
ULONG SN_Recv(char *string, char len, ULONG BreakBitMask);
void SN_HangingUP();
void SN_WaitConnect();


/* ************************** */

#define PORTTYPE_MAIN 1
#define PORTTYPE_CLIENT 0


BOOL SN_Open()
{
	char error				= FALSE;

	struct PortData		*cport;

	struct SerialNetwork	*sn = NULL;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (!cport->network.isopen)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((cport->network.networkdata = AllocVec(sizeof(SerialNetwork),MEMF_ANY|MEMF_CLEAR)))
				{
					sn = cport->network.networkdata;
					sn->SerWritePort	= CreateMsgPort();
					sn->SerReadPort	= CreateMsgPort();
					sn->SerQueryPort	= CreateMsgPort();
				
					sn->SerWrite		= NULL;
					sn->SerRead			= NULL;
					sn->SerQuery		= NULL;
				
					if (sn->SerReadPort && sn->SerWritePort && sn->SerQueryPort)
					{
						sn->SerRead		= CreateIORequest(sn->SerReadPort,sizeof(IOExtSer));
						sn->SerQuery	= CreateIORequest(sn->SerQueryPort,sizeof(IOExtSer));
						sn->SerWrite	= CreateIORequest(sn->SerWritePort,sizeof(IOExtSer));
						if (sn->SerWrite && sn->SerWrite && sn->SerQuery)
						{
				
						   sn->SerWrite->IOSer.io_Message.mn_ReplyPort = sn->SerWritePort;
				
							if (OpenDevice(cport->clientconfig.SerialData.Device_Name,cport->clientconfig.SerialData.Unit,(IORequest *)sn->SerWrite,0))
							{
								ioprintf("can't open serial.device. Device-Errorcode: %d\n",sn->SerWrite->IOSer.io_Error);
								error = TRUE;
							}
							else
							{
								cport->network.isopen = TRUE;
				
								/*** SET SERIAL PARAMS ***/
				
								sn->SerWrite->io_RBufLen			= 20;														// Größe des Eingangspuffers; ein Vielfaches von 64 Bytes
								sn->SerWrite->io_Baud				= cport->clientconfig.SerialData.OnlineBaud;
								sn->SerWrite->io_BrkTime			= 100;													// Dauer des Unterbrechungssignals in Microsekunden.
								sn->SerWrite->io_ReadLen			= cport->clientconfig.SerialData.DataBits;
								sn->SerWrite->io_WriteLen			= cport->clientconfig.SerialData.DataBits;
								sn->SerWrite->io_StopBits			= cport->clientconfig.SerialData.StopBits;
								sn->SerWrite->io_SerFlags			= SERF_XDISABLED|SERF_7WIRE;
								sn->SerWrite->IOSer.io_Command	= SDCMD_SETPARAMS;
								DoIO((IORequest *)sn->SerWrite);																// Set parameters
				
								*sn->SerRead = *sn->SerWrite;																// Copy SerWrite in SerRead
								sn->SerRead->IOSer.io_Message.mn_ReplyPort  = sn->SerReadPort;					// change correct port address
				
								*sn->SerQuery = *sn->SerWrite;
								sn->SerQuery->IOSer.io_Message.mn_ReplyPort = sn->SerQueryPort;
				
								sn->SerWrite->IOSer.io_Command = CMD_WRITE;
								sn->SerWrite->IOSer.io_Flags = IOF_QUICK;
				
								sn->SerWrite->IOSer.io_Data    = "ATZ\r";
								sn->SerWrite->IOSer.io_Length  = 4;
								sn->SerWrite->IOSer.io_Flags   = IOF_QUICK;
								sn->SerWrite->IOSer.io_Command = CMD_WRITE;
								DoIO((IORequest *)sn->SerWrite);
				
								sn->SerQuery->IOSer.io_Command = SDCMD_QUERY;
								sn->SerQuery->IOSer.io_Data    = (APTR) cport->network.ReadBuf;
								sn->SerQuery->IOSer.io_Length  = 0;
							}
						}
						else
						{
							ioprintf("can't open serial.device, out of memory.\n");
							error = TRUE;
						}
					}
					else
					{
						ioprintf("can't open serial.device, out of memory.\n");
						error = TRUE;
					}
				}
				else
				{
					ioprintf("internal error, can't find networkdate-structure.\n");
					error = TRUE;
				}
			}
			else
			{
				ioprintf("internal error network is not a serial-device.\n");
				error = TRUE;
			}
		}
		else
		{
			ioprintf("serial.device is already open.\n");
			error = TRUE;
		}
	}

	if (error && sn)
	{
		if (sn->SerWritePort)
		{
			if (sn->SerWrite)
			{
				DeleteIORequest((IORequest *)sn->SerWrite);
				sn->SerWrite = NULL;
			}
			DeleteMsgPort(sn->SerWritePort);
			sn->SerWritePort = NULL;
		}
		if (sn->SerReadPort)
		{
			if (sn->SerRead)
			{
				DeleteIORequest((IORequest *)sn->SerRead);
				sn->SerRead = NULL;
			}
			DeleteMsgPort(sn->SerReadPort);
			sn->SerReadPort = NULL;
		}
		if (sn->SerQueryPort)
		{
			if (sn->SerQuery)
			{
				DeleteIORequest((IORequest *)sn->SerQuery);
				sn->SerQuery = NULL;
			}
			DeleteMsgPort(sn->SerQueryPort);
			sn->SerQueryPort = NULL;
		}
	}
	if (!error)
		return TRUE;
}






void SN_Close()
{
	struct PortData		*cport;

	struct SerialNetwork	*sn = NULL;

	if (cport = (PortData *)FindTask(NULL)->tc_UserData)
	{
		if (cport->network.networktype == NETWORKTYPE_SERIAL)
		{
			if ((sn = cport->network.networkdata))
			{
				if (cport->network.isopen)
				{
					AbortIO((IORequest *)sn->SerRead);
					WaitIO((IORequest *)sn->SerRead);
					sn->SerRead->IOSer.io_Command	=	CMD_READ;
					sn->SerRead->IOSer.io_Data		=	cport->network.ReadBuf;
					sn->SerRead->IOSer.io_Length	=	1;
					while(CheckIO((IORequest *)sn->SerRead))
					{
						WaitIO((IORequest *)sn->SerRead);
						SendIO((IORequest *)sn->SerRead);
					}

					AbortIO((IORequest *)sn->SerWrite);
					WaitIO((IORequest *)sn->SerWrite);

					AbortIO((IORequest *)sn->SerQuery);
					WaitIO((IORequest *)sn->SerQuery);

					CloseDevice((IORequest *)sn->SerWrite);
				}

				DeleteIORequest((IORequest *)sn->SerWrite);
				sn->SerWrite=NULL;

				DeleteIORequest((IORequest *)sn->SerRead);
				sn->SerRead=NULL;

				DeleteIORequest((IORequest *)sn->SerQuery);
				sn->SerQuery=NULL;

				DeleteMsgPort(sn->SerWritePort);
				sn->SerWritePort=NULL;

				DeleteMsgPort(sn->SerReadPort);
				sn->SerReadPort=NULL;

				DeleteMsgPort(sn->SerQueryPort);
				sn->SerQueryPort=NULL;

				FreeVec(sn);
				cport->network.networkdata = NULL;
			}
			else
				ioprintf("internal error: can't find networkdate-structure.\n");
		}
	}
	else
		ioprintf("internal error: can't find PortData-structure.\n");
}



BOOL SN_CheckNetworkStatus()
{
	struct PortData *cport;
	if (cport = (PortData *)FindTask(NULL)->tc_UserData)
	{
		if (cport->network.isopen)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				struct SerialNetwork *sn = cport->network.networkdata;
				if (!(DoIO((IORequest *)sn->SerQuery)))
				{
					if (!(sn->SerQuery->io_Status & (1<<5)))		// Check Carrier
					{
						cport->network.online=TRUE;
					}
					else
					{
						if (cport->network.online)
						{
							cport->network.online=FALSE;
							SendMsg(cport->MainPort,cport,0,MSG_PORT_LOSTCARRIER,NULL);
						}
					}
				}
			}
		}
	}
	return ((BOOL)cport->network.online );
}


LONG SN_Send(APTR data, ULONG len)
{
	struct	PortData			*cport			= (PortData *)FindTask(NULL)->tc_UserData;
	char							SerWriteFinish	= FALSE;
	ULONG 						signals			= (1<<cport->TimePort->mp_SigBit);
	struct	SerialNetwork 	*sn;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{
					signals	=	signals | (1<<sn->SerWritePort->mp_SigBit);
					sn->SerWrite->IOSer.io_Flags		= IOF_QUICK;
					sn->SerWrite->IOSer.io_Data		= data;
					sn->SerWrite->IOSer.io_Length		= len;
				  	sn->SerWrite->IOSer.io_Command	= CMD_WRITE;
					SendIO((IORequest *)sn->SerWrite);

					if (!(CheckIO((IORequest *)cport->TimerReq)))
					{
						AbortIO((IORequest *)cport->TimerReq);
						WaitIO((IORequest *)cport->TimerReq);
					}
	
					cport->TimerReq->tr_time.tv_secs		= 1;
					cport->TimerReq->tr_time.tv_micro	= 0;
					cport->TimerReq->tr_node.io_Command	= TR_ADDREQUEST;
					SendIO((IORequest *)cport->TimerReq);
	
					while (SerWriteFinish == FALSE)
					{
						if (CheckIO((IORequest *)sn->SerWrite))
						{
							WaitIO((IORequest *)sn->SerWrite);			// reply IORequest-Message to serial.device
							SerWriteFinish = TRUE;
						}
						if (CheckIO((IORequest *)cport->TimerReq))
						{
							WaitIO((IORequest *)cport->TimerReq);			// first step, reply to timer.device
							SerWriteFinish = TRUE;
						}
						if (SerWriteFinish == NULL )
						Wait(signals);
					}
	
					AbortIO((IORequest *)cport->TimerReq);				/* Abort timerequest */
					WaitIO((IORequest *)cport->TimerReq);
	
	
					if (!(CheckIO((IORequest *)sn->SerWrite)))
						AbortIO((IORequest *)sn->SerWrite);
					WaitIO((IORequest *)sn->SerWrite);
					if (!cport->connect_seq)
						SN_CheckNetworkStatus();
				}
			}
		}
	}
	return TRUE;
}


ULONG SN_Recv(char *string, char len, ULONG BreakBitMask)
{
	struct	PortData			*cport			= (PortData *)FindTask(NULL)->tc_UserData;
	char							SerWriteFinish	= FALSE;
	ULONG 						signals			= (1<<cport->TimePort->mp_SigBit);
	struct	SerialNetwork 	*sn;
	ULONG 						result			= 0;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{
					ULONG sb_SerReadPort=1<<sn->SerReadPort->mp_SigBit;
					BOOL quit = FALSE;

					AbortIO((IORequest *)sn->SerRead);
					WaitIO((IORequest *)sn->SerRead);

					sn->SerRead->IOSer.io_Data=(APTR)cport->network.ReadBuf;
					sn->SerRead->IOSer.io_Length=1;
			  		sn->SerRead->IOSer.io_Command = CMD_READ;

			  		SendIO((IORequest *)sn->SerRead);
					sn->SerReadSend=TRUE;

					*string=0;
					while (!cport->ProgramClose && ((cport->network.online)|cport->connect_seq) && !quit)
					{
						result=WaitSig(sb_SerReadPort | BreakBitMask);
						if (sb_SerReadPort & result)
						{
							if (cport->network.isopen && sn->SerRead && sn->SerReadSend)
							{
								if (CheckIO((IORequest *)sn->SerRead))
								{
									WaitIO((IORequest *)sn->SerRead);
								   if (sn->SerRead->IOSer.io_Actual > 0)
									{
										*string=*((char *)sn->SerRead->IOSer.io_Data);
										quit = TRUE;
										sn->SerReadSend=FALSE;
									}
								}
							}
						}
						if (BreakBitMask & result)
							quit = TRUE;
					} // while
				} // sn = cport->network.networkdata
			} // networktype
		} // isopen
	} // cport
	return result;
} // void ()


void SN_HangingUP()
{
	struct	PortData			*cport			= (PortData *)FindTask(NULL)->tc_UserData;
	char							SerWriteFinish	= FALSE;
	ULONG 						signals			= (1<<cport->TimePort->mp_SigBit);
	struct	SerialNetwork 	*sn;
	ULONG 						result			= 0;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{
					SN_CheckNetworkStatus();
					cport->network.isopen=FALSE;
					if (!(CheckIO((IORequest *)sn->SerWrite)))
					{			
						AbortIO((IORequest *)sn->SerWrite);
						WaitIO((IORequest *)sn->SerWrite);
					}

					/* clearing serial buffer */

					AbortIO((IORequest *)sn->SerRead);
					WaitIO((IORequest *)sn->SerRead);
					sn->SerRead->IOSer.io_Command	=	CMD_READ;
					sn->SerRead->IOSer.io_Data		=	cport->network.ReadBuf;
					sn->SerRead->IOSer.io_Length	=	1;
					while(CheckIO((IORequest *)sn->SerRead))
					{
						WaitIO((IORequest *)sn->SerRead);
						SendIO((IORequest *)sn->SerRead);
					}
	
					if (!(CheckIO((IORequest *)sn->SerRead)))
					{
						AbortIO((IORequest *)sn->SerRead);
						WaitIO((IORequest *)sn->SerRead);
					}

					CloseDevice((IORequest *)sn->SerWrite); /* Close device for hangup */
	
					Delay(5);
	
					if (!cport->ProgramClose)
					{
						if (OpenDevice(cport->clientconfig.SerialData.Device_Name,cport->clientconfig.SerialData.Unit,(IORequest *)sn->SerWrite, 0))
						{
							ioprintf("can't open serial.device!\n");
						}
						else
						{
							sn->SerWrite->io_RBufLen=20;	// Größe des Eingangspuffers; ein Vielfaches von 64 Bytes
							sn->SerWrite->io_Baud=cport->clientconfig.SerialData.OnlineBaud;
							sn->SerWrite->io_BrkTime=100;  // Dauer des Unterbrechungssignals in Microsekunden.
							sn->SerWrite->io_ReadLen=cport->clientconfig.SerialData.DataBits;;
							sn->SerWrite->io_WriteLen=cport->clientconfig.SerialData.DataBits;
							sn->SerWrite->io_StopBits=cport->clientconfig.SerialData.StopBits;
							sn->SerWrite->io_SerFlags=SERF_XDISABLED|SERF_7WIRE;
							sn->SerWrite->IOSer.io_Flags = IOF_QUICK;
							sn->SerWrite->IOSer.io_Command = SDCMD_SETPARAMS;
							DoIO((IORequest *)sn->SerWrite);

							SN_Send("ATZ\n",4);
		//					DoIO((IORequest *)sn->SerWrite);

							*sn->SerRead = *sn->SerWrite;
							sn->SerRead->IOSer.io_Message.mn_ReplyPort=sn->SerReadPort;	// change correct port address

							*sn->SerQuery = *sn->SerWrite;
							sn->SerQuery->IOSer.io_Message.mn_ReplyPort=sn->SerQueryPort;

							sn->SerQuery->IOSer.io_Command = SDCMD_QUERY;
							sn->SerQuery->IOSer.io_Data	 = (APTR)cport->network.ReadBuf;
							sn->SerQuery->IOSer.io_Length  = 0;

							sn->SerRead->IOSer.io_Command  = CMD_READ;
							sn->SerRead->IOSer.io_Data 	 = (APTR)cport->network.ReadBuf;
							sn->SerRead->IOSer.io_Length	 = 1;

		//					SendIO((IORequest *)sn->SerRead);
							sn->SerReadSend=FALSE;

							cport->network.isopen=TRUE;
						}
					}
				}
			}
		}
	}
}

void SN_WaitConnect()
{
	char 							cp[20];
	char 							buffer[200];
	WORD 							counter			= 0;
	ULONG 						ulong;
	struct	PortData			*cport			= (PortData *)FindTask(NULL)->tc_UserData;
	struct	SerialNetwork 	*sn;

	if ((cport = (PortData *)FindTask(NULL)->tc_UserData))
	{
		if (cport->network.isopen)
		{
			if (cport->network.networktype == NETWORKTYPE_SERIAL)
			{
				if ((sn = cport->network.networkdata))
				{
					cport->PortStatus		 = PORT_STATUS_WAITCALL;
					cport->network.online = FALSE;
					cport->connect_seq	 = TRUE;
					cport->network.can_write = FALSE;
					cport->network.can_read  = FALSE;

					ClearMemQuick(buffer,200);
		
					AbortIO((IORequest *)sn->SerWrite);
					WaitIO((IORequest *)sn->SerWrite);

					AbortIO((IORequest *)sn->SerRead);
					WaitIO((IORequest *)sn->SerRead);

					sn->SerReadSend=FALSE;

					while(CheckIO((IORequest *)sn->SerRead))
					{
						WaitIO((IORequest *)sn->SerRead);
						sn->SerRead->IOSer.io_Command	=	CMD_READ;
						sn->SerRead->IOSer.io_Data		=	cport->network.ReadBuf;
						sn->SerRead->IOSer.io_Length	=	1;
						SendIO((IORequest *)sn->SerRead);
					}

					while (cport->connect_seq && !cport->ProgramClose)
					{
						SN_Recv(cp,1,0);
						if (*cp)
						{
							Writeio(cp,1);
							if (*cp==10|*cp==13|counter>150)
							{
								if (counter)
								{
									if (strstr(buffer,"RING"))
									{
										Writeio("found RING\n",-1);
										if (CheckIO((IORequest *)sn->SerWrite))
										{
											AbortIO((IORequest *)sn->SerWrite);
											WaitIO((IORequest *)sn->SerWrite);
										}
										SN_Send("ATA\r",4);
									}
									if (strstr(buffer,"CID"))
									{
										Writeio("found CID:",-1);
										Writeio(&buffer[6],-1);
//										strncpy(cport->whostring,connect_string+6,30);
										SendMsg(cport->MainPort,cport,0,MSG_PORT_CID,NULL);
									}
									if (strstr(buffer,"CONNECT"))
									{
										cport->PortStatus        = PORT_STATUS_CONNECT;
										StrToLong(&buffer[9],(LONG *)ulong);
										SendMsg(cport->MainPort,cport,0,MSG_PORT_CHANGE,NULL);
										cport->connect_seq       = FALSE;
										cport->network.online    = TRUE;
										cport->network.can_write = TRUE;
										cport->network.can_read  = TRUE;										
									}
								}
								counter=0;
								ClearMemQuick(buffer,200);
							}
							else
							{
								buffer[counter]=*cp;
								counter++;
							}
						}
					}
				}
				else
				{
					cport->connect_seq       = FALSE;
					cport->network.online    = TRUE;
					cport->network.can_write = FALSE;
					cport->network.can_read  = FALSE;										
				}
			}
			else
			{
				cport->connect_seq       = FALSE;
				cport->network.online    = TRUE;
				cport->network.can_write = FALSE;
				cport->network.can_read  = FALSE;										
			}
		}
		else
		{
			cport->connect_seq       = FALSE;
			cport->network.online    = TRUE;
			cport->network.can_write = FALSE;
			cport->network.can_read  = FALSE;										
		}
	}
	else
	{
		cport->connect_seq       = FALSE;
		cport->network.online    = TRUE;
		cport->network.can_write = FALSE;
		cport->network.can_read  = FALSE;										
	}
}













