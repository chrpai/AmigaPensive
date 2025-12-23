#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/param.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>

#include <pragma/exec_lib.h>
#include <pragma/socket_lib.h>
#include <pragma/noviasys_lib.h>
#include <novia/novia_portdata.h>
#include <exec/tasks.h>
#include <devices/timer.h>
#include <novia/novia_config.h>
#include <novia/novia_dos.h>
#include <novia/novia_registers.h>

#define PORTADDRESS 110

#include "novia_tcpip_api.h"

extern int VerboseLevel;   /* prepare use of lprintf */
extern MainPortConfig *mainportconfig;

struct Library *SocketBase = NULL;

int recvExactlynbytes(int socket, char *buffer, int numbytestoreceive, int flags );
void ht ( void );
extern void ClearMem(APTR start,ULONG len);
char *SendKey(int socket, char *string);
void bzero(APTR start, ULONG len);
int readsocket(int socket, char *ptr, ULONG len);

void __saveds ASM nslib_CopyUUCPName(register __a0 char *string, register __a1 char *RealName, register __a2 char *Name);
UBYTE __saveds ASM nslib_GetUUCPMonth(register __a0 char *string);
void __saveds ASM nslib_ConvertUUCPDate(register __a0 char *string, register __a1 struct Date *date);

struct timeval tv;
int retval;
extern struct Date actualdate;

char uucp_char[] = { 0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
1,
0,
0,
0,
1,
1,
0,
1,
1,
1,
1,
0,
1,
1,
0,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
0,
0,
0,
1,
0,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
0,
0,
0,
0,
0,
1,
0,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1};

LONG __saveds ASM nslib_uucp_sendmail(register __a0 INet_POP3Server *server, register __a1 INet_Mail *mail)
{
	struct PortData *cport = (struct PortData *)FindTask(NULL)->tc_UserData;
	int failat = 0;
	List headerlist;
	if ((SocketBase = OpenLibrary("bsdsocket.library",0)))
	{	
		struct hostent *hp;
		struct sockaddr_in sa;
		int s;
		char buf[200];
		ULONG counter = 0;
	
		/*********************************************************\
		*                                                         *
		*                       SEND MAIL                         *
		*                                                         *
		\*********************************************************/


		if (server)
		{
			printf("NOViA SMTP-Mailer (RFC-821)\n");
			printf("---------------------------\n");
			printf("servername: %s\n",server->Name);
			printf("domain    : %s\n",server->Domain);
			printf("SMTPHost  : %s\n",server->SMTPHost);
			printf("---------------------------\n");
			printf("connecting to %s ...\n",server->SMTPHost);
			if((hp = gethostbyname(server->SMTPHost)))
			{
				printf("get sock ok\n");
				ClearMem(&sa, sizeof(sa));
				CopyMem(hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
	
				sa.sin_family = hp->h_addrtype;
				sa.sin_port	  = htons((u_short)25);
			
				if ((s = socket(hp->h_addrtype,SOCK_STREAM,0)) >= 0)
				{
					if (connect(s,(struct sockaddr *) &sa,sizeof(sa))>= 0)
					{
						struct Node *node = (Node *)mail->to_list.mlh_Head;
						char *buffer;
						LONG erg;
						fd_set readable;
						FD_ZERO(&readable);
						FD_SET(0, &readable);
						tv.tv_secs	= 30;
						tv.tv_micro	= 0;
						ClearMem(buf,200);
						if (WaitSelect(1, &readable, NULL, NULL, &tv, 0))
						{
							if ((erg = recv(s,buf,199,0)) != -1)
							{
								StrToLong(buf,&erg);
								if (erg != 220)
								{
									printf("server connecting failat: %s",buf);
									failat = TRUE;
								}
								else
								{
									printf("connected to %s\n",server->SMTPHost);
								}
							}
							else
							{
								printf("no reply message: %d\n",erg);
								failat = TRUE;
							}
						}
						else
						{
							printf("Timeout!!!\n");
							failat = TRUE;
						}

				/*** HELLO ***/

						if (!failat)
						{
							sprintf(buf,"HELO %s\r\n",server->Username);
							if ((buffer = SendKey(s,buf)))
							{
								StrToLong(buffer, &erg);
								if (erg != 250)
								{
									printf("Logon procedure failat: %s",buffer);
									failat = TRUE;
								}
								else
									printf("logged in.\n");
							}
							else
								failat = TRUE;
						}

				/*** MAIL FROM: ***/

						if (!failat)
						{
							sprintf(buf,"MAIL FROM:<%s>\r\n",mail->from);
							if ((buffer = SendKey(s,buf)))
							{
								StrToLong(buffer, &erg);
								if (erg < 250 | erg > 251)
								{
									printf("FROM failat: %s",buffer);
									failat = TRUE;
								}
								else
								{
									printf("Sender ok.\n");
								}
							}
							else
								failat = TRUE;
						}

				/*** RCPT TO: ***/

						while (!failat && node->ln_Succ)
						{
							sprintf(buf,"RCPT TO:<%s>\r\n",node->ln_Name);
							if ((buffer = SendKey(s,buf)))
							{
								StrToLong(buffer, &erg);
								if (erg < 250 | erg > 251)
								{
									printf("RCPT failat: %s",buffer);
									failat = TRUE;
								}
								else
									printf("Empfänger ok.\n");
							}
							else
								failat = TRUE;
							node = node->ln_Succ;
						}

				/*** DATA ***/

						if (!failat)
						{
							sprintf(buf,"DATA\r\n");
							if ((buffer = SendKey(s,buf)))
							{
								StrToLong(buffer, &erg);
								if (erg != 354)
								{
									printf("failat to sending DATA: %s",buffer);
									failat = TRUE;
								}
								else
								{
									if (send(s, mail->text, strlen(mail->text), 0) != -1)
									{
										fd_set readable;
										ULONG mask;
										FD_ZERO(&readable);
										FD_SET(0, &readable);
										tv.tv_secs	= 20;
										tv.tv_micro	= 0;
										if ((mask = WaitSelect(1, &readable, NULL, NULL, &tv, 0)))
										{
											if (recv(s, buffer, 1023, 0) == -1)
											{
												failat = TRUE;
												printf("send mail failat: %s\n",buffer);
											}
											else
												printf("read ok: %s\n",buffer);
										}
										else
										{
											failat = TRUE;
											printf("send mail failat.\n");
										}
									}
								}
								FreeVec(buffer);
							}
							else
								failat = TRUE;
						}


						if (!failat)
						{
							sprintf(buf,"\r\n.\r\n");
							if ((buffer = SendKey(s,buf)))
							{
								FreeVec(buffer);
							}
							else
								failat = TRUE;
						}


				/*** QUIT ***/

						if (!failat)
						{
							sprintf(buf,"QUIT\r\n");
							if ((buffer = SendKey(s,buf)))
							{
								StrToLong(buffer, &erg);
								if (erg != 221)
								{
									printf("close connecting failat, break socket connection: %s",buffer);
									failat = TRUE;
								}
								else
								{
									printf("connecting closed.\n");
								}
							}
							else
								failat = TRUE;
						}
					}
				}
			}
//			cleanup_sockets();					
		}
	}
	else
		printf("can't open bsdsocket.library\n");
	return failat;
}

LONG __saveds ASM nslib_uucp_getmails(register __a0 INet_POP3Server *server)
{
	struct PortData *cport = (struct PortData *)FindTask(NULL)->tc_UserData;
	LONG result	= 0;
	List savedlist;
	List serverlist;
	BPTR fh;
	char path[120];

	char	*fullpath	= server->InboundPath;
	ULONG length_fp	= strlen(fullpath);

	if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
		sprintf(path,"%s.index",fullpath);
	else
		sprintf(path,"%s/.index",fullpath);

	NewList(&savedlist);
	NewList(&serverlist);


	if ((fh = Open(path, MODE_READWRITE)))
	{
		char quit = FALSE;
		struct INet_MailHeader *header;
		while (!quit)
		{
			if ((header = AllocVec(sizeof(INet_MailHeader), MEMF_ANY | MEMF_CLEAR)))
			{
				if ((Read(fh, header, sizeof(INet_MailHeader)) == sizeof(INet_MailHeader)))
				{
					AddTail((List *)&savedlist, (Node *)header);
				}
				else
				{
					FreeVec(header);
					quit = TRUE;
				}
			}
		}

		if ((SocketBase = OpenLibrary("bsdsocket.library",0)))
		{	
			struct hostent *hp;
			struct sockaddr_in sa;
			int s;
			char buf[200];
			ULONG counter = 0;
		
			/*********************************************************\
			*                                                         *
			*                         GET MAIL                        *
			*                                                         *
			\*********************************************************/
	
	
			while (server && server->ln_Succ)
			{
				printf("NOViA POP3 Client\n");
				printf("---------------------------\n");
				printf("servername: %s\n",server->Name);
				printf("domain    : %s\n",server->Domain);
				printf("SMTPHost  : %s\n",server->POP3Host);
				printf("---------------------------\n");
				printf("connecting to %s ...\n",server->POP3Host);

			/*** get ip-address from Domain Name Server ***/

				if((hp = gethostbyname(server->POP3Host)))
				{
					ClearMem(&sa, sizeof(sa));
					CopyMem(hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
		
					sa.sin_family = hp->h_addrtype;
					sa.sin_port	  = htons((u_short) 110);
				
			/*** create socket ***/

					if ((s = socket(hp->h_addrtype, SOCK_STREAM, 0)) >= 0)
					{

			/*** connect POP Server ***/

						if (connect(s,(struct sockaddr *) &sa, sizeof(sa))>= 0)
						{
							char *buffer;
							LONG erg;
							LONG Mails = 0;
							LONG size  = 0;
							char quit  = FALSE;
							fd_set readable;
							FD_ZERO(&readable);
							FD_SET(0, &readable);
							tv.tv_secs	= 30;
							tv.tv_micro	= 0;
							ClearMem(buf,200);
							if (WaitSelect(1, &readable, NULL, NULL, &tv, 0))
							{
								if ((recv(s,buf,199,0)) != -1)
								{
									if (buf[0] == '+' && buf[1] == 'O' && buf[2] == 'K')
									{
										printf("connected to %s %s\n",server->POP3Host,buf);
									}
									else
									{
										result = TRUE;
										printf("server connecting failat: %s",buf);
									}
								}
								else
								{
									printf("no reply message: %d\n",erg);
									result = TRUE;
								}
							}
							else
							{
								printf("Timeout!!!\n");
								result = TRUE;
							}


			/*** USER <> ***/

							if (!result)
							{
								sprintf(buf,"USER %s\r\n",server->Username);
								if ((buffer = SendKey(s,buf)))
								{
									if (buffer[0] == '+' && buffer[1] == 'O' && buffer[2] == 'K')
									{
										printf("logged in.\n");
									}
									else
									{
										printf("Logon procedure failat: %s",buffer);
										result = TRUE;
									}
									FreeVec(buffer);
								}
								else
									result = TRUE;
							}


			/*** PASS <> ***/

							if (!result)
							{
								sprintf(buf,"PASS %s\r\n",server->Password);
								printf("%s",buf);
								if ((buffer = SendKey(s,buf)))
								{
									if (buffer[0] == '+' && buffer[1] == 'O' && buffer[2] == 'K')
									{
										printf("password ok. logged in.\n");
									}
									else
									{
										printf("wrong password: %s",buffer);
										result = TRUE;
									}
									FreeVec(buffer);
								}
								else
								{
									result = TRUE;
								}
							}

				/*** STAT <> ***/

							if (!result)
							{
								sprintf(buf,"STAT\r\n");
								if ((buffer = SendKey(s,buf)))
								{
									if (buffer[0] == '+' && buffer[1] == 'O' && buffer[2] == 'K')
									{
										char *ptr = buffer;
										printf("Get Statistik ok.\n");
										while (*ptr && *ptr != ' ')
											ptr++;
										while (*ptr && !isdigit(*ptr))
											ptr++;
										StrToLong(ptr, &Mails);
										while (*ptr && *ptr != ' ')
											ptr++;
										while (*ptr && !isdigit(*ptr))
											ptr++;
										StrToLong(ptr, &size);
										printf("Mails: %d\n",Mails);
										printf("size : %d\n",size);
									}
									else
									{
										printf("STAT command failat. %s",buffer);
										result = TRUE;
									}
									FreeVec(buffer);
								}
								else
									result = TRUE;
							}


				/*** LIST <> ***/

							if (!result)
							{
								char *ptr;
								char *ptr2;
								char lineok;
								sprintf(buf,"LIST\r\n");
								printf("%s",buf);
								if ((buffer = SendKey(s,buf)))
								{
									if (buffer[0] == '+' && buffer[1] == 'O' && buffer[2] == 'K')
									{
										printf("LIST.\n%s",buffer);
										ptr = buffer;
										while (*ptr && *ptr != 10 && *ptr != 13)		// jump to next line
											ptr++;
										while (*ptr && (*ptr == 10 && *ptr ==13))
											ptr++;
									}
									else
									{
										printf("wrong password: %s",buffer);
										result = TRUE;
									}
								}
								else
								{
									result = TRUE;
								}
								while (!quit && !result)
								{
									ptr2 = ptr;
									lineok = FALSE;
									while (*ptr2 && *ptr2 != 10 && *ptr2 != 13)	// checking for LF/CR on end of line
										ptr2++;
									if (*ptr2 == 13 | *ptr2 == 10)					// if CR or LF line is ok
									{
										lineok = TRUE;
									}
									else														// broken line, get more bytes from socket.
									{
										char *newbuf = AllocVec(1024, MEMF_ANY|MEMF_CLEAR);
										if (ptr2 - ptr < 1024)
										{
											CopyMem(ptr, newbuf, ptr2 - ptr);		// copy first segment of line into new buffer.
											ptr = newbuf + (ptr2 - ptr);				// start for next segment
											if (readsocket(s, ptr, 1023 - (ptr2 - ptr)) == -1) // failat on readsocket ??
											{
												printf("readsocket failat\n");
												quit = TRUE;
											}
											ptr = newbuf;
											lineok = TRUE;
										}
										else
										{
											printf("unexpected error\n");			// is line bigger as 1024 bytes ?????
											quit = TRUE;
										}
									}
									if (!quit)
									{
										if ((header = AllocVec(sizeof(INet_MailHeader), MEMF_ANY|MEMF_CLEAR)))
										{
											while (*ptr && !isdigit(*ptr))
												ptr++;
											StrToLong(ptr, (LONG *)&header->id);		// copy number of mail
											printf("id: %d\n",header->id);
											while (isdigit(*ptr))
												ptr++;
											while (*ptr && !isdigit(*ptr))
												ptr++;
											StrToLong(ptr, (LONG *)&header->size);	// copy size of message
											printf("size: %d\n",header->size);
											if (!header->id | !header->size)			// check of valid header
												FreeVec(header);
											else
												AddTail(&serverlist, (Node *)header);
										}
										else
										{
											printf("can't alloc mem\n");
											quit = TRUE;
										}
										while (*ptr && *ptr != 13 && *ptr != 10)	// set ptr to end of line
											ptr++;
										while (*ptr && (*ptr == 10 | *ptr == 13))
											ptr++;
									}
								}
								if (buffer)
									FreeVec(buffer);
							}
							if (IsListEmpty(&serverlist))
							{
								result = TRUE;
								printf("no mails on server\n");
							}

				/*** UIDL <> ***/

							if (!result)
							{
								char *ptr;
								char *ptr2;
								char lineok;
								INet_MailHeader *header;
								quit = FALSE;
								sprintf(buf,"UIDL\r\n");
								printf("%s",buf);
								if ((buffer = SendKey(s,buf)))
								{
									if (buffer[0] == '+' && buffer[1] == 'O' && buffer[2] == 'K')
									{
										printf("UIDL.\n%s",buffer);
										ptr = buffer;
										while (*ptr && *ptr != 10 && *ptr != 13)		// jump to next line
											ptr++;
										while (*ptr && (*ptr == 10 && *ptr ==13))
											ptr++;
									}
									else
									{
										printf("wrong password: %s",buffer);
										result = TRUE;
									}
								}
								else
								{
									result = TRUE;
								}
								header = (struct INet_MailHeader *)serverlist.lh_Head;
								if (!header->ln_Succ | !header->ln_Pred)
									result = TRUE;
								while (!quit && !result)
								{
									ptr2 = ptr;
									lineok = FALSE;
									while (*ptr2 && *ptr2 != 10 && *ptr2 != 13)	// checking for LF/CR on end of line
										ptr2++;
									if (*ptr2 == 13 | *ptr2 == 10)					// if CR or LF line is ok
									{
										lineok = TRUE;
									}
									else														// broken line, get more bytes from socket.
									{
										char *newbuf = AllocVec(1024, MEMF_ANY|MEMF_CLEAR);
										if (ptr2 - ptr < 1024)
										{
											CopyMem(ptr, newbuf, ptr2 - ptr);		// copy first segment of line into new buffer.
											ptr = newbuf + (ptr2 - ptr);				// start for next segment
											if (readsocket(s, ptr, 1023 - (ptr2 - ptr)) == -1) // failat on readsocket ??
											{
												printf("readsocket result\n");
												quit = TRUE;
											}
											ptr = newbuf;
											lineok = TRUE;
										}
										else
										{
											printf("unexpected error\n");			// is line bigger as 1024 bytes ?????
											quit = TRUE;
										}
									}
									if (!quit)
									{
										ULONG id;
										ULONG UID_size = 0;
										char  *uidptr;
										while (*ptr && !isdigit(*ptr))
											ptr++;
										StrToLong(ptr, (LONG *)&id);		// copy number of mail
										printf("get id: %d\n",id);
										while (header->ln_Succ && header->id != id)
										{
											header = header->ln_Succ;
										}
										if (header->ln_Succ && header->id == id)
										{
											uidptr = header->uid;
											while (isdigit(*ptr))
												ptr++;
											while (*ptr && !isgraph(*ptr))
												ptr++;
											while (isgraph(*ptr) && UID_size++ < 80)
												*uidptr++ = *ptr++;
											*uidptr = 0;
											printf("uid: %d %s\n",header->id,header->uid);
										}
										else
										{
											printf("can't find message id. internal failat\n");
											quit = TRUE;
										}
										while (*ptr && *ptr != 13 && *ptr != 10)	// set ptr to end of line
											ptr++;
										while (*ptr && (*ptr == 10 | *ptr == 13))
											ptr++;
									}
								}
								if (buffer)
									FreeVec(buffer);
							}

				/*** RETR <> ***/

							printf("check duplicate messages\n");
							if (!result)
							{
								if (!IsListEmpty((List *)&savedlist))
								{
									struct INet_MailHeader *savedheader  = (struct INet_MailHeader *)savedlist.lh_Head;
									struct INet_MailHeader *serverheader;
									char found;
									while (savedheader->ln_Succ)
									{
										serverheader = (struct INet_MailHeader *)serverlist.lh_Head;
										found = FALSE;
										while (!found && serverheader->ln_Succ)
										{
											if (!strcmp(serverheader->uid,savedheader->uid))
											{
												found = TRUE;
												printf("%s alread loaded.\n",serverheader->uid);
												Remove((Node *)serverheader);
												FreeVec(serverheader);
											}
											serverheader = serverheader->ln_Succ;
										}
										savedheader  = savedheader->ln_Succ;
									}
								}
								if (IsListEmpty((List *)&serverlist))
								{
									printf("all messages already loaded.\n");
									result = TRUE;
								}
							}

							if (!result)
							{
								ULONG readsize;
								ULONG getsize;
								BPTR  text_fh;
								INet_MailHeader *header = (INet_MailHeader *)serverlist.lh_Head;
								while (header->ln_Succ)
								{
									strncpy(header->filename, header->uid, 20);


									if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
										sprintf(path,"%s%s",fullpath,header->filename);
									else
										sprintf(path,"%s/%s",fullpath,header->filename);

									if ((text_fh = Open(path, MODE_NEWFILE)))
									{
										printf("GET MAIL: %d %d\n",header->id,header->size);
										sprintf(buf,"RETR %d\r\n",header->id);
										if ((buffer = SendKey(s,buf)))
										{
											if (buffer[0] == '+' && buffer[1] == 'O' && buffer[2] == 'K')
											{
												readsize = 0;
												printf("mail:>\n%s",buffer);
												while (!result && readsize < header->size)
												{
													ClearMem(buffer,1024);
													if ((header->size - readsize) > 1023)
														getsize = recv(s,buffer,1023,0);
													else
														getsize = recv(s,buffer,header->size - readsize,0);
													if (getsize != -1)
													{
//														printf("%s",buffer);
														Write(text_fh,buffer,getsize);
														readsize = readsize + getsize;
													}
													else
													{
														printf("readsocket failat: %d\n",getsize);
														result = TRUE;
													}
												}
												recv(s,buffer,1023,0);	// end-block 'CR LF . CR LF sequence
											}
											else
											{
												printf("wrong password: %s",buffer);
												result = TRUE;
											}
											FreeVec(buffer);
										}
										else
											result = TRUE;
										Close(text_fh);
										if (!result)
										{
											Write(fh, header, sizeof(INet_MailHeader));
										}
										else
										{
//											DeleteFile(filename);
										}
										
									}
									header = header->ln_Succ;
								}
							}
//							if (!result)
							{
								sprintf(buf,"QUIT\r\n");
								if ((buffer = SendKey(s,buf)))
								{
									if (buffer[0] == '+' && buffer[1] == 'O' && buffer[2] == 'K')
									{
										printf("logged out.\n");
									}
									else
									{
										printf("logout failat: %s",buffer);
										result = TRUE;
									}
									FreeVec(buffer);
								}
								else
									result = TRUE;
							}
	
						}
					}
				}
				server = server->ln_Succ;	
		//		cleanup_sockets();					
			}
	
	
			CloseLibrary(SocketBase);
		}
		else
			printf("can't open bsdsocket.library\n");
		Close(fh);
	}
	else
		printf("can't open .index\n");
	return(result);
}

char *SendKey(int socket, char *string)
{
	char *buf = NULL;
	if (string)
	{
		if ((buf = AllocVec(1024, MEMF_ANY|MEMF_CLEAR)))
		{
			if (send(socket,string,strlen(string),0) != -1)
			{
				fd_set readable;
				ULONG mask;
				FD_ZERO(&readable);
				FD_SET(0, &readable);
				tv.tv_secs	= 20;
				tv.tv_micro	= 0;
				if ((mask = WaitSelect(1, &readable, NULL, NULL, &tv, 0)))
				{
					if (recv(socket,buf,1023,0) == -1)
					{
						FreeVec(buf);
						buf = 0;
					}
					else
						printf("read ok: %s\n",buf);
				}
				else
				{
					FreeVec(buf);
					buf = 0;
				}
			}
			else
			{
				FreeVec(buf);
				buf = 0;
			}
		}
	}
	return buf;
}

int readsocket(int socket, char *ptr, ULONG len)
{
	int result = -1;
	if (socket && ptr && len)
	{
		fd_set readable;
		ULONG mask;
		FD_ZERO(&readable);
		FD_SET(0, &readable);
		tv.tv_secs	= 5;
		tv.tv_micro	= 0;
		if ((mask = WaitSelect(1, &readable, NULL, NULL, &tv, 0)))
		{
			result = recv(socket,ptr,len,0);
		}
	}
	return result;
}


void bzero(APTR start, ULONG len)
{
	ClearMem(start, len);
}

int mystrcmp(char *str1, char *str2)
{
	char *ptr1 = str1;
	char *ptr2 = str2;
	int result = TRUE;
	while (isspace(*ptr1))
		ptr1++;
	while (result && *ptr1 && *ptr2)
	{
		if (toupper(*ptr1) == toupper(*ptr2))
		{
			ptr1++; ptr2++;
		}
		else
		{
			if (!isspace(*ptr1) | *ptr2 != 0)
				result = FALSE;
		}
	}
	return(result);
}


void __saveds ASM nslib_CopyUUCPName(register __a0 char *string, register __a1 char *RealName, register __a2 char *Name)
{
	char *ptr		= string;
	char *ptr2;
	char *realname	= RealName;
	char *name		= Name;
	char *endrealname;

	while (*ptr && *ptr != ':')	// jump to :
		ptr++;
	if (*ptr == ':')
		ptr++;
	while (isspace(*ptr))			// jump to first char of name
		ptr++;
	ptr2 = ptr;
	while (*ptr2 && *ptr2 != '@')	// suche @
		ptr2++;
	if (*ptr2 == '@')
	{
		ptr2--;
		while (uucp_char[*ptr2])
			ptr2--;
	}
	endrealname = ptr2;
	ptr2++;
	while (*ptr2 && uucp_char[*ptr2])
		*name++ = *ptr2++;

	*name = 0;
	while (*ptr && ptr < endrealname)
	{
		if (uucp_char[*ptr] | isspace(*ptr))
			*realname++ = *ptr++;
		else
			ptr++;
	}
	while (isspace(realname[-1]))
		realname--;
	*realname = 0;
}

UBYTE __saveds ASM nslib_GetUUCPMonth(register __a0 char *string)
{
	char date[4];
	date[0] = string[0];
	date[1] = string[1];
	date[2] = string[2];
	date[3] = 0;
	if (!strcmp(date, "Jan"))
		return 1;
	else if (!strcmp(date, "Feb"))
		return 2;
	else if (!strcmp(date, "Mar"))
		return 3;
	else if (!strcmp(date, "Apr"))
		return 4;
	else if (!strcmp(date, "May"))
		return 5;
	else if (!strcmp(date, "Jun"))
		return 6;
	else if (!strcmp(date, "Jul"))
		return 7;
	else if (!strcmp(date, "Aug"))
		return 8;
	else if (!strcmp(date, "Sep"))
		return 9;
	else if (!strcmp(date, "Oct"))
		return 10;
	else if (!strcmp(date, "Nov"))
		return 11;
	else if (!strcmp(date, "Dec"))
		return 12;
	else
		return 0;
}

void __saveds ASM nslib_ConvertUUCPDate(register __a0 char *string, register __a1 struct Date *date)
{
	//	Tue, 07 Sep 1999 18:21:00 +0100
	char *ptr = string;
	LONG result;

	while (*ptr && *ptr != ':')
		ptr++;
	if (*ptr == ':')
		ptr++;

	while (*ptr && !isdigit(*ptr))
		ptr++;
	if (*ptr)
	{
		StrToLong(ptr,&result);
		date->day = (UBYTE) result;
		
		while (*ptr && !isalpha(*ptr))
			ptr++;
	
		date->month = GetUUCPMonth(ptr);
		
		while (*ptr && isgraph(*ptr))
			ptr++;
		
		if (*ptr)
		{
			while (*ptr && !isdigit(*ptr))
				ptr++;
			StrToLong(ptr,&result);
			if (result < 100)								// Y2K Correcting ( 19xx or 20xx ???)
			{
				date->year = (UWORD)result + 1900;
			}
			else
				date->year = (UWORD)result;
			while (isdigit(*ptr))
				ptr++;
			if (*ptr)
			{
				while (*ptr && !isdigit(*ptr))
					ptr++;
				if (*ptr)
				{
					StrToLong(ptr, &result);
					date->hours = (UBYTE)result;
					ptr = ptr + 3;
					StrToLong(ptr, &result);
					date->mins = (UBYTE)result;
					ptr = ptr + 3;
					StrToLong(ptr, &result);
					date->secs = (UBYTE)result;
					ptr = ptr + 3;
					CalcDatecode(date);
				}
			}
		}
	}
}



