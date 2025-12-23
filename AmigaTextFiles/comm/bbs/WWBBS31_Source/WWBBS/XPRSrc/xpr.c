#include <exec/types.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <devices/serial.h>
#include <devices/timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>

#include "Data:Programs/Include/xproto.h"

#include "xpr_rev.h"

char *version=VERSTAG;

struct Library *XProtocolBase;

long __stack=10000;

BOOL panic=FALSE;

void __regargs _CXBRK(void);

void __regargs _CXBRK(void)
	{
		panic=TRUE;
	}

/* Vars */
BYTE *Template="Command/A,D=Device/K,U=Unit/N,L=Library/K/R,I=Init/K,Log/K,FL=FileList/K,FN=FileName/K,ID=UserID/K,RES=Resume/S";
LONG ArgArray[]={NULL,(LONG) "serial.device",0,NULL,NULL,NULL,NULL,NULL,NULL,FALSE};
struct RDArgs *RDArgs;
struct MsgPort *SerialMP;
struct IOExtSer *SerialIO;
BOOL SerialIsOpen;
struct MsgPort *TimerMP;
struct timerequest *TimerIO;
BOOL TimerIsOpen;
struct XPR_IO *XprIO;
BPTR FileListFH;
ULONG XprFlags;
BOOL XprSuccess;

/* Settings */
BYTE *Command;
BYTE *Device;
ULONG Unit;
BYTE *XprLibrary;
BYTE *XprInit;
BYTE *LogFile;
BYTE *FileList;
BYTE *FileName;
BYTE *UserID;
BOOL Resume;

/* Xpr */
BOOL XprIsFile;
BYTE XprFileName[256];
BOOL XprResume;
LONG XprBytes;
LONG XprFileSize;
LONG XprBlockSize;
LONG XprCPS;
LONG XprErrors;
LONG XprTimeouts;

/* Protos */
void close_all(int);
LONG __asm __saveds xpr_fopen(register __a0 char *,register __a1 char *);
LONG __asm __saveds xpr_fclose(register __a0 FILE *);
LONG __asm __saveds xpr_fread(register __a0 char *,register __d0 long,register __d1 long,register __a1 FILE *);
LONG __asm __saveds xpr_fwrite(register __a0 char *,register __d0 long,register __d1 long,register __a1 FILE *);
LONG __asm __saveds xpr_sread(register __a0 char *,register __d0 long,register __d1 long);
LONG __asm __saveds xpr_swrite(register __a0 char *,register __d0 long);
LONG __asm __saveds xpr_sflush(void);
LONG __asm __saveds xpr_update(register __a0 struct XPR_UPDATE *);
LONG __asm __saveds xpr_chkabort(void);
LONG __asm __saveds xpr_chkmisc(void);
LONG __asm __saveds xpr_gets(void);
LONG __asm __saveds xpr_setserial(register __d0 long);
LONG __asm __saveds xpr_ffirst(register __a0 char *,register __a1 char *);
LONG __asm __saveds xpr_fnext(register __d0 long,register __a0 char *,register __a1 char *);
LONG __asm __saveds xpr_finfo(register __a0 char *,register __d0 long);
LONG __asm __saveds xpr_fseek(register __a0 FILE *,register __d0 long,register __d1 long);
LONG __asm __saveds xpr_options(register __d0 long,register __a0 struct xpr_option **);
LONG __asm __saveds xpr_unlink(register __a0 char *);
LONG __asm __saveds xpr_squery(void);
LONG __asm __saveds xpr_getptr(register __d0 long);

void main()
	{
		/* Read Args */
		if(!(RDArgs=ReadArgs(Template,ArgArray,NULL)))
			close_all(RETURN_FAIL);
		Command=(BYTE *) ArgArray[0];
		Device=(BYTE *) ArgArray[1];
		Unit=(ULONG) *((LONG *) ArgArray[2]);
		XprLibrary=(BYTE *) ArgArray[3];
		XprInit=(BYTE *) ArgArray[4];
		LogFile=(BYTE *) ArgArray[5];
		FileList=(BYTE *) ArgArray[6];
		FileName=(BYTE *) ArgArray[7];
		UserID=(BYTE *) ArgArray[8];
		Resume=(BOOL) ArgArray[9];

		/* Open Serial */
		if(!(SerialMP=CreateMsgPort()))
			close_all(RETURN_FAIL);
		if(!(SerialIO=(struct IOExtSer *) CreateIORequest(SerialMP,sizeof(struct IOExtSer))))
			close_all(RETURN_FAIL);
		SerialIO->io_SerFlags|=SERF_SHARED;
		if(OpenDevice(Device,Unit,(struct IORequest *) SerialIO,0))
			close_all(RETURN_FAIL);
		SerialIsOpen=TRUE;

		/* Open Timer */
		if(!(TimerMP=CreateMsgPort()))
			close_all(RETURN_FAIL);
		if(!(TimerIO=(struct timerequest *) CreateIORequest(TimerMP,sizeof(struct timerequest))))
			close_all(RETURN_FAIL);
		if(OpenDevice(TIMERNAME,UNIT_VBLANK,(struct IORequest *) TimerIO,0))
			close_all(RETURN_FAIL);
		TimerIsOpen=TRUE;

		/* Open Xpr Library */
		if(!(XProtocolBase=OpenLibrary(XprLibrary,0)))
			close_all(RETURN_FAIL);
		if(!(XprIO=AllocMem(sizeof(struct XPR_IO),MEMF_CLEAR)))
			close_all(RETURN_FAIL);

		/* Delete Old Log File */
		if(LogFile)
			DeleteFile(LogFile);

		/* Open File List */
		if(FileList)
			{
				if(!(FileListFH=Open(FileList,MODE_OLDFILE)))
					close_all(RETURN_FAIL);
			}

		/* Init Xpr */
		{
			char buffer[256];
			if(!XprInit)
				{
					char name[256];
					strcpy(name,XprLibrary);
					{
						char *p;
						if(p=strchr(name,'.'))
							*p=NULL;
					}
					strcpy(buffer,"");
					GetVar(name,buffer,255,NULL);
				}
			XprIO->xpr_filename=(XprInit) ? XprInit : buffer;
			XprIO->xpr_fopen=xpr_fopen;
			XprIO->xpr_fclose=xpr_fclose;
			XprIO->xpr_fread=xpr_fread;
			XprIO->xpr_fwrite=xpr_fwrite;
			XprIO->xpr_sread=xpr_sread;
			XprIO->xpr_swrite=xpr_swrite;
			XprIO->xpr_sflush=xpr_sflush;
			XprIO->xpr_update=xpr_update;
			XprIO->xpr_chkabort=xpr_chkabort;
			XprIO->xpr_chkmisc=xpr_chkmisc;
			XprIO->xpr_gets=xpr_gets;
			XprIO->xpr_setserial=xpr_setserial;
			XprIO->xpr_ffirst=xpr_ffirst;
			XprIO->xpr_fnext=xpr_fnext;
			XprIO->xpr_finfo=xpr_finfo;
			XprIO->xpr_fseek=xpr_fseek;
			XprIO->xpr_extension=4;
			XprIO->xpr_options=xpr_options;
			XprIO->xpr_unlink=xpr_unlink;
			XprIO->xpr_squery=xpr_squery;
			XprIO->xpr_getptr=xpr_getptr;
			XprFlags=XProtocolSetup(XprIO);
		}

		/* Do Transfer */
		switch(Command[0])
			{
				case 'R':
				case 'r':
					XprIO->xpr_filename=FileName;
					XprSuccess=XProtocolReceive(XprIO);
					break;
				case 'S':
				case 's':
					XprIO->xpr_filename=FileName;
					XprSuccess=XProtocolSend(XprIO);
					break;
			}

		/* If Non-Batch Receive, Write Log... */
		if(!(XprFlags & XPRS_NORECREQ))
			{
				if(XprIsFile)
					{
						if(XprSuccess || (XprBytes && XprFileSize && (XprBytes>=XprFileSize)))
							{
								if(LogFile)
									{
										FILE *logfp;
										if(logfp=fopen(LogFile,"w"))
											{
												switch(Command[0])
													{
														case 'R':
														case 'r':
															fprintf(logfp,"R %ld 0 bps %ld cps %ld errors %ld %ld %s 0\n",XprBytes,XprCPS,XprErrors,XprTimeouts,XprBlockSize,XprFileName);
															break;
														case 'S':
														case 's':
															fprintf(logfp,"H %ld 0 bps %ld cps %ld errors %ld %ld %s 0\n",XprBytes,XprCPS,XprErrors,XprTimeouts,XprBlockSize,XprFileName);
															break;
													}
												fclose(logfp);
											}
									}
							}
						else
							{
								switch(Command[0])
									{
										case 'R':
										case 'r':
											DeleteFile(XprFileName);
											break;
									}
							}
						XprIsFile=FALSE;
					}
			}

		/* Done */
		close_all((XprSuccess) ? RETURN_OK : RETURN_WARN);
	}

void close_all(int exitcode)
	{
		if(XprFlags & XPRS_SUCCESS) XProtocolCleanup(XprIO);
		if(FileListFH) Close(FileListFH);
		if(XprIO) FreeMem(XprIO,sizeof(struct XPR_IO));
		if(XProtocolBase) CloseLibrary(XProtocolBase);
		if(TimerIsOpen) CloseDevice((struct IORequest *) TimerIO);
		if(TimerIO) DeleteIORequest((struct IORequest *) TimerIO);
		if(TimerMP) DeleteMsgPort(TimerMP);
		if(SerialIsOpen) CloseDevice((struct IORequest *) SerialIO);
		if(SerialIO) DeleteIORequest((struct IORequest *) SerialIO);
		if(SerialMP) DeleteMsgPort(SerialMP);
		if(RDArgs) FreeArgs(RDArgs);
		exit(exitcode);
	}

LONG __asm __saveds
xpr_fopen(register __a0 char *filename,register __a1 char *accessmode)
	{
		LONG ret=0;
		int unfinished=FALSE,ok=TRUE;
#ifdef DEBUG
printf("xpr_fopen(%s,%s)",filename,accessmode);
#endif
		switch(Command[0])
			{
				case 'R':
				case 'r':
					if(UserID)
						{
							{
								BPTR fh;
								char s[256];
								sprintf(s,"XPR: %s Unfinished File",UserID);
								if(fh=Open(filename,MODE_OLDFILE))
									{
										__aligned struct FileInfoBlock fib;
										if(ExamineFH(fh,&fib))
											{
												if(!strcmp(fib.fib_Comment,s))
													unfinished=TRUE;
												else	
													ok=FALSE;
											}
										Close(fh);
									}
							}
						}
					break;
			}
		if(ok)
			{
				switch(Command[0])
					{
						case 'S':
						case 's':
							ret=(LONG) fopen(filename,accessmode);
							if(ret)
								{
									XprIsFile=TRUE;
									strcpy(XprFileName,filename);
									XprResume=(unfinished) ? TRUE : FALSE;
									XprBytes=0;
									XprFileSize=0;
									XprBlockSize=1024;
									XprCPS=9999;
									XprErrors=0;
									XprTimeouts=0;
								}
							break;
						case 'R':
						case 'r':
							ret=(LONG) fopen(FilePart(filename),accessmode);
							if(ret)
								{
									XprIsFile=TRUE;
									strcpy(XprFileName,FilePart(filename));
									XprResume=(unfinished) ? TRUE : FALSE;
									XprBytes=0;
									XprFileSize=0;
									XprBlockSize=1024;
									XprCPS=9999;
									XprErrors=0;
									XprTimeouts=0;
								}
							break;
					}
			}
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_fclose(register __a0 FILE *fp)
	{
		LONG ret;
#ifdef DEBUG
printf("xpr_fclose(%p)",fp);
#endif
		ret=fclose(fp);
		if(XprFlags & XPRS_NORECREQ)
			{
				if(XprIsFile)
					{
						if(XprBytes && XprFileSize && (XprBytes>=XprFileSize))
							{
								switch(Command[0])
									{
										case 'R':
										case 'r':
											if(XprResume)
												SetComment(XprFileName,"");
											break;
									}
								if(LogFile)
									{
										FILE *logfp;
										if(logfp=fopen(LogFile,"a"))
											{
												switch(Command[0])
													{
														case 'R':
														case 'r':
															fprintf(logfp,"R %ld 0 bps %ld cps %ld errors %ld %ld %s 0\n",XprBytes,XprCPS,XprErrors,XprTimeouts,XprBlockSize,XprFileName);
															break;
														case 'S':
														case 's':
															fprintf(logfp,"H %ld 0 bps %ld cps %ld errors %ld %ld %s 0\n",XprBytes,XprCPS,XprErrors,XprTimeouts,XprBlockSize,XprFileName);
															break;
													}
												fclose(logfp);
											}
									}
							}
						else
							{
								switch(Command[0])
									{
										case 'R':
										case 'r':
											if(Resume)
												{
													if(UserID)
														{
															char s[256];
															sprintf(s,"XPR: %s Unfinished File",UserID);
															SetComment(XprFileName,s);
														}
												}
											else
												DeleteFile(XprFileName);
											break;
									}
							}
						XprIsFile=FALSE;
					}
			}
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_fread(register __a0 char *buffer,register __d0 long size,register __d1 long count,register __a1 FILE *fp)
	{
		LONG ret;
#ifdef DEBUG
printf("xpr_fread(%p,%ld,%ld,%p)",buffer,size,count,fp);
#endif
		ret=(LONG) fread(buffer,size,count,fp);
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_fwrite(register __a0 char *buffer,register __d0 long size,register __d1 long count,register __a1 FILE *fp)
	{
		LONG ret;
#ifdef DEBUG
printf("xpr_fwrite(%p,%ld,%ld,%p)",buffer,size,count,fp);
#endif
		ret=(LONG) fwrite(buffer,size,count,fp);
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_sread(register __a0 char *buffer,register __d0 long size,register __d1 long timeout)
	{
		ULONG UnreadCount;
		BOOL Carrier;
		LONG ret=0;
#ifdef DEBUG
printf("xpr_sread(%p,%ld,%ld)",buffer,size,timeout);
#endif
		SerialIO->IOSer.io_Command=SDCMD_QUERY;
		DoIO((struct IORequest *) SerialIO);
		UnreadCount=SerialIO->IOSer.io_Actual;
		Carrier=(SerialIO->io_Status & (1L << 5)) ? FALSE : TRUE;
		if(!Carrier)
			ret=-1;
		else
			{
				if(size>0)
					{
						if(timeout>0)
							{
								if(UnreadCount>=size)
									{
										SerialIO->IOSer.io_Length=size;
										SerialIO->IOSer.io_Data=buffer;
										SerialIO->IOSer.io_Command=CMD_READ;
										DoIO((struct IORequest *) SerialIO);
										ret=SerialIO->IOSer.io_Actual;
									}
								else
									{
										SerialIO->IOSer.io_Length=size;
										SerialIO->IOSer.io_Data=buffer;
										SerialIO->IOSer.io_Command=CMD_READ;
										SendIO((struct IORequest *) SerialIO);
										TimerIO->tr_time.tv_secs=timeout/1000000;
										TimerIO->tr_time.tv_micro=timeout%1000000;
										TimerIO->tr_node.io_Command=TR_ADDREQUEST;
										SendIO((struct IORequest *) TimerIO);
										{
											int kg=TRUE;
											while(kg)
												{
													Wait((1L << SerialMP->mp_SigBit) | (1L << TimerMP->mp_SigBit));
													if(CheckIO((struct IORequest *) SerialIO))
														kg=FALSE;
													if(CheckIO((struct IORequest *) TimerIO))
														kg=FALSE;
												}
										}
										if(!CheckIO((struct IORequest *) SerialIO))
											AbortIO((struct IORequest *) SerialIO);
										WaitIO((struct IORequest *) SerialIO);
										if(!CheckIO((struct IORequest *) TimerIO))
											AbortIO((struct IORequest *) TimerIO);
										WaitIO((struct IORequest *) TimerIO);
										ret=SerialIO->IOSer.io_Actual;
									}
							}
						else
							{
								SerialIO->IOSer.io_Length=(UnreadCount>size) ? size : UnreadCount;
								SerialIO->IOSer.io_Data=buffer;
								SerialIO->IOSer.io_Command=CMD_READ;
								DoIO((struct IORequest *) SerialIO);
								ret=SerialIO->IOSer.io_Actual;
							}
					}
			}
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_swrite(register __a0 char *buffer,register __d0 long size)
	{
#ifdef DEBUG
printf("xpr_swrite(%p,%ld)",buffer,size);
#endif
		SerialIO->IOSer.io_Length=size;
		SerialIO->IOSer.io_Data=buffer;
		SerialIO->IOSer.io_Command=CMD_WRITE;
		DoIO((struct IORequest *) SerialIO);
#ifdef DEBUG
printf(" -> 0\n");
#endif
		return(0);
	}

LONG __asm __saveds
xpr_sflush()
	{
#ifdef DEBUG
printf("xpr_sflush()");
#endif
		SerialIO->IOSer.io_Command=CMD_FLUSH;
		DoIO((struct IORequest *) SerialIO);
#ifdef DEBUG
printf(" -> 0\n");
#endif
		return(0);
	}

LONG __asm __saveds
xpr_update(register __a0 struct XPR_UPDATE *updatestruct)
	{
#ifdef DEBUG
printf("xpr_update(%p)",updatestruct);
#endif
		if(XprIsFile)
			{
				if(updatestruct->xpru_updatemask & XPRU_BYTES)
					XprBytes=updatestruct->xpru_bytes;
				if(updatestruct->xpru_updatemask & XPRU_FILESIZE)
					XprFileSize=updatestruct->xpru_filesize;
				if(updatestruct->xpru_updatemask & XPRU_BLOCKSIZE)
					XprBlockSize=updatestruct->xpru_blocksize;
				if(updatestruct->xpru_updatemask & XPRU_DATARATE)
					XprCPS=updatestruct->xpru_datarate;
				if(updatestruct->xpru_updatemask & XPRU_ERRORS)
					XprErrors=updatestruct->xpru_errors;
				if(updatestruct->xpru_updatemask & XPRU_TIMEOUTS)
					XprTimeouts=updatestruct->xpru_timeouts;
			}
#ifdef DEBUG
printf(" -> 0\n");
#endif
		return(0);
	}

LONG __asm __saveds
xpr_chkabort()
	{
		LONG ret=0;
		BOOL Carrier;
#ifdef DEBUG
printf("xpr_chkabort()");
#endif
		SerialIO->IOSer.io_Command=SDCMD_QUERY;
		DoIO((struct IORequest *) SerialIO);
		Carrier=(SerialIO->io_Status & (1L << 5)) ? FALSE : TRUE;
		if(!Carrier)
			ret=-1;
		if(panic)
			ret=-1;
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_chkmisc()
	{
#ifdef DEBUG
printf("xpr_chkmisc -> 0\n");
#endif
		return(0);
	}

LONG __asm __saveds
xpr_gets()
	{
#ifdef DEBUG
printf("xpr_gets -> 0\n");
#endif
		return(0);
	}

LONG __asm __saveds
xpr_setserial(register __d0 long newstatus)
	{
#ifdef DEBUG
printf("xpr_setserial -> -1\n");
#endif
		return(-1);
	}

LONG __asm __saveds
xpr_ffirst(register __a0 char *buffer,register __a1 char *pattern)
	{
		LONG ret=0;
		BYTE file[256];
#ifdef DEBUG
printf("xpr_ffirst(%p,%s)",buffer,pattern);
#endif
		if(FileList)
			{
				Seek(FileListFH,0,OFFSET_BEGINNING);
				if(FGets(FileListFH,file,255))
					{
						{
							char *p;
							if(p=strchr(file,'\n'))
								*p=NULL;
						}
						strcpy(buffer,file);
						ret=1;
					}
			}
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_fnext(register __d0 long oldstate,register __a0 char *buffer,register __a1 char *pattern)
	{
		LONG ret=0;
		BYTE file[256];
#ifdef DEBUG
printf("xpr_fnext(%p,%s)",buffer,pattern);
#endif
		if(FileList)
			{
				if(FGets(FileListFH,file,255))
					{
						{
							char *p;
							if(p=strchr(file,'\n'))
								*p=NULL;
						}
						strcpy(buffer,file);
						ret=1;
					}
			}
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_finfo(register __a0 char *filename,register __d0 long typeofinfo)
	{
		LONG ret=0;
#ifdef DEBUG
printf("xpr_finfo(%s,%ld)",filename,typeofinfo);
#endif
		switch(typeofinfo)
			{
				case 1:
					switch(Command[0])
						{
							case 'S':
							case 's':
								{
									BPTR fh;
									if(fh=Open(filename,MODE_OLDFILE))
										{
											__aligned struct FileInfoBlock fib;
											if(ExamineFH(fh,&fib))
												ret=fib.fib_Size;
											Close(fh);
										}
								}
								break;
							case 'R':
							case 'r':
								{
									BPTR fh;
									if(fh=Open(FilePart(filename),MODE_OLDFILE))
										{
											__aligned struct FileInfoBlock fib;
											if(ExamineFH(fh,&fib))
												ret=fib.fib_Size;
											Close(fh);
										}
								}
								break;
						}
					break;
				case 2:
					ret=1;
					break;
			}
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_fseek(register __a0 FILE *fp,register __d0 long offset,register __d1 long origin)
	{
		LONG ret;
#ifdef DEBUG
printf("xpr_fseek(%p,%ld,%ld)",fp,offset,origin);
#endif
		ret=(LONG) fseek(fp,offset,origin);
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_options(register __d0 long numopts,register __a0 struct xpr_option **opts)
	{
#ifdef DEBUG
printf("xpr_options() -> 0\n");
#endif
		return(0);
	}

LONG __asm __saveds
xpr_unlink(register __a0 char *filename)
	{
		LONG ret=-1;
#ifdef DEBUG
printf("xpr_unlink(%s)",filename);
#endif
		if(DeleteFile(filename))
			ret=0;
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_squery()
	{
		LONG ret=0;
#ifdef DEBUG
printf("xpr_squery()");
#endif
		SerialIO->IOSer.io_Command=SDCMD_QUERY;
		DoIO((struct IORequest *) SerialIO);
		ret=SerialIO->IOSer.io_Actual;
#ifdef DEBUG
printf(" -> %ld\n",ret);
#endif
		return(ret);
	}

LONG __asm __saveds
xpr_getptr(register __d0 long infotype)
	{
#ifdef DEBUG
printf("xpr_getptr(%ld) -> -1\n",infotype);
#endif
		return(-1);
	}
