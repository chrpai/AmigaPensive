#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/wwbbs.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/rexxsyslib.h>
#include <proto/wwbbs.h>

#include "rx.h"

#include "bbsrx_rev.h"

void ABBEREXXCommand(BYTE *id,UWORD cmd,BYTE *arg,char *buff,char **result,char **error,long *errorlevel)
	{
		switch(cmd)
			{
				case ABBEREXX_BBSIdentify:
					if(arg)
						{
							if(!stricmp(arg,"ABBEREXX"))
								*result="1.0";
							if(!stricmp(arg,"BBS"))
								{
									sprintf(buff,"%s (%s)",VERS,DATE);
									*result=buff;
								}
							if(!stricmp(arg,"EMULATION"))
								{
									BOOL ansi=FALSE;
									GetStatusTags(STTAG_Name,id,STTAG_ANSI,&ansi);
									*result=(ansi) ? "ANSI" : "ASCII";
								}
							if(!stricmp(arg,"NAME"))
								*result="World Wide BBS";
							if(!stricmp(arg,"SYSOP"))
								{
									strcpy(buff,"");
									GetUserTags(USRTAG_Name,"Sysop",USRTAG_UserName,buff,TAG_END);
									*result=buff;
								}
							if(!stricmp(arg,"TERM"))
								{
									BYTE baud[11],rows[4],columns[5];
									if((GetVar("BAUD",baud,10,NULL)!=-1) && (GetVar("ROWS",rows,3,NULL)!=-1) && (GetVar("COLUMNS",columns,4,NULL)!=-1))
										{
											sprintf(buff,"%s %s %s 0",baud,rows,columns);
											*result=buff;
										}
								}
							if(!stricmp(arg,"USER"))
								{
									BYTE name[33];
									strcpy(name,"");
									GetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
									if(strlen(name))
										{
											UBYTE accesslevel=0;
											BYTE *accessname=NULL,city[21],state[21],country[4];
											strcpy(city,"");
											strcpy(state,"");
											strcpy(country,"");
											GetUserTags(USRTAG_Name,name,
												USRTAG_AccessLevel,&accesslevel,
												USRTAG_City,city,
												USRTAG_State,state,
												USRTAG_Country,country,
												TAG_END);
											if(accesslevel==0)
												accessname="GUEST";
											if(accesslevel>0 && accesslevel<255)
												accessname="MEMBER";
											if(accesslevel==255)
												accessname="SYSOP";
											sprintf(buff,"\"%s\" \"%s, %s  %s\" %s",name,city,state,country,accessname);
											*result=buff;
										}
								}
						}
					break;
				case ABBEREXX_CheckAbort:
					if(WaitForChar(Input(),0))
						{
							int c;
							c=getchar();
							if(isspace(c))
								*result="1";
							else
								*result="0";
						}
					else
						*result="0";
					break;
				case ABBEREXX_CLS:
					printf("\f");
					break;
				case ABBEREXX_GetCarrier:
					{
						BOOL local=FALSE;
						GetStatusTags(STTAG_Name,id,STTAG_Local,&local,TAG_END);
						if(local || SerialCommandTags(id,SERCMD_Carrier,TAG_END))
							*result="1";
						else
							*result="0";
					}
					break;
				case ABBEREXX_GetChar:
					{
						int c;
						c=getchar();
						if(c==EOF)
							*result="###PANIC";
						else
							{
								sprintf(buff,"%c",c);
								*result=buff;
							}
					}
					break;
				case ABBEREXX_GetUser:
					if(arg)
						{
							switch(atoi(arg))
								{
									case 1: /* Name */
										if(GetStatusTags(STTAG_Name,id,STTAG_UserName,buff,TAG_END)) *result=buff;
										break;
									case 7: /* Time Remaining */
										{
											UWORD timeremaining=0;
											if(GetStatusTags(STTAG_Name,id,STTAG_TimeRemaining,&timeremaining,TAG_END))
												{
													sprintf(buff,"%d",timeremaining);
													*result=buff;
												}
										}
										break;
									case 15: /* Access group */
										*result="0";
										break;
									case 17: /* System maintenance */
										*result="0";
										break;
									case 18: /* Time used */
										{
											UWORD timeused=0;
											if(GetStatusTags(STTAG_Name,id,STTAG_TimeUsed,&timeused,TAG_END))
												{
													sprintf(buff,"%d",timeused);
													*result=buff;
												}
										}
										break;
									case 19: /* Screen clear code */
										*result="\f";
										break;
									case 23: /* Current port */
										*result="0";
										break;
									case 24: /* CPS */
										{
											BYTE baud[11];
											strcpy(baud,"");
											if(GetVar("BAUD",baud,10,NULL)!=-1)
												{
													sprintf(buff,"%ld",atol(baud)/10);
													*result=buff;
												}
										}
										break;
									case 27: /* Current line width */
										if(GetVar("ROWS",buff,3,NULL)!=-1)
											*result=buff;
										break;
									case 28: /* Terminal type */
										{
											BOOL ansi=FALSE;
											if(GetStatusTags(STTAG_Name,id,STTAG_ANSI,&ansi,TAG_END))
												*result=(ansi) ? "2" : "0";
										}
										break;
									case 41: /* ID */
										strcpy(buff,id);
										*result=buff;
										break;
								}
						}
					break;
				case ABBEREXX_HangUp:
					SetStatusTags(STTAG_Name,id,STTAG_Panic,TRUE,TAG_END);
					break;
				case ABBEREXX_IReady:
					{
						BOOL local=FALSE;
						GetStatusTags(STTAG_Name,id,STTAG_Local,&local,TAG_END);
						if(local)
							*result=(ConsoleCommandTags(id,CONCMD_Query,TAG_END)) ? "1" : "0";
						else
							*result=(SerialCommandTags(id,SERCMD_Query,TAG_END)) ? "1" : "0";
					}
					break;
				case ABBEREXX_MayGetChar:
					if(WaitForChar(Input(),0))
						{
							int c;
							c=getchar();
							sprintf(buff,"%c",c);
							*result=buff;
						}
					break;
				case ABBEREXX_NewLine:
					printf("\n");
					break;
				case ABBEREXX_Print:
					if(arg)
						printf(arg);
					printf("\n");
					break;
				case ABBEREXX_Prompt:
					if(arg)
						{
							char type[256],*s;
							int length;
							sscanf(arg,"%d %s",&length,type);
							if(s=strchr(arg,' '))
								{
									s++;
									if(s=strchr(s,' '))
										s++;
								}
							if(length>255)
								length=255;
							printf("~p%s",s);
							if(!stricmp(type,"NORMAL"))
								{
									GetLine(buff,length,NULL);
									*result=buff;
								}
							if(!stricmp(type,"HIDE"))
								{
									GetLine(buff,length,GLFLG_NoEcho);
									*result=buff;
								}
							if(!stricmp(type,"YESNO"))
								*result=(Ask(NULL,TRUE)) ? "Y": "N";
							if(!stricmp(type,"NOYES"))
								*result=(Ask(NULL,FALSE)) ? "Y": "N";
							printf("~r");
						}
					break;
				case ABBEREXX_Query:
					if(arg)
						printf("~p%s",arg);
					GetLine(buff,255,NULL);
					*result=buff;
					break;
				case ABBEREXX_Receive:
					GetLine(buff,255,NULL);
					*result=buff;
					break;
				case ABBEREXX_Send:
					if(arg)
						printf(arg);
					break;
				case ABBEREXX_SendFile:
					if(arg)
						ShowText(arg);
					break;
				case ABBEREXX_SetNodeLocation:
					break;
				case ABBEREXX_SysopLog:
					if(arg)
						{
							BYTE log[33];
							strcpy(log,"");
							GetStatusTags(STTAG_Name,id,STTAG_Location,log,TAG_END);
							if(strlen(log))
								{
									sprintf(buff,"%.255s",arg);
									LogEntry(id,log,buff);
								}
						}
					break;
			}
	}
