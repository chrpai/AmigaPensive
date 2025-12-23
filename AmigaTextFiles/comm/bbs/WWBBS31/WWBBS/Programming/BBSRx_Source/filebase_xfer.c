#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/wwbbs.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/rexxsyslib.h>
#include <proto/wwbbs.h>

#include "rx.h"

#include "filebase.h"

void Upload(BYTE *id)
	{
		BOOL local=FALSE;
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,STTAG_Local,&local,TAG_END);
		if(strlen(user))
			{
				if(!local)
					{
						if(strlen(file_area))
							{
								BYTE protocol[33];
								strcpy(protocol,"");
								GetUserTags(USRTAG_Name,user,USRTAG_Protocol,protocol,TAG_END);
								if(strlen(protocol))
									{
										BYTE cmd[256];
										BOOL batch=FALSE,bidirectional=FALSE;
										strcpy(cmd,"");
										if(GetConfigTags(CFGTAG_Path,"Protocols",CFGTAG_Name,protocol,PRTAG_ReceiveCommand,cmd,PRTAG_Batch,&batch,PRTAG_Bidirectional,&bidirectional,TAG_END))
											{
												ULONG dsr=0,dfs=0;
												BOOL readonly=FALSE;
												BYTE directory[256];
												strcpy(directory,"");
												if(GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Directory,directory,FBTAG_DiskSpaceRequired,&dsr,FBTAG_ReadOnly,&readonly,TAG_END))
													{
														if(!readonly)
															{
																{
																	BPTR lock;
																	if(lock=Lock(directory,SHARED_LOCK))
																		{
																			struct InfoData id;
																			if(Info(lock,&id))
																				dfs=((id.id_NumBlocks-id.id_NumBlocksUsed)*id.id_BytesPerBlock)/1024;
																			UnLock(lock);
																		}
																}
																printf("~s\nFree disk space: %ldK\n",dfs);
																if(!dsr || dfs>=dsr)
																	{
																		BOOL ok=TRUE;
																		BYTE filename[33];
																		strcpy(filename,"");
																		if(!batch)
																			{
																				BYTE buff[33];
																				printf("~p\nEnter file name.\n: ");
																				if(GetLine(buff,32,NULL))
																					strcpy(filename,FilePart(buff));
																				else
																					ok=FALSE;
																			}
																		if(bidirectional)
																			CreateFileList(id);
																		if(ok)
																			{
																				printf("~s\nReady to receive files...\n~r");
																				DoTransfer(id,cmd,filename);
																				ProcessLogFile(id);
																				GetFileDescriptions(id);
																			}
																		if(bidirectional)
																			DeleteFileList(id);
																	}
																else
																	printf("~s\nThere is not enough disk space for uploads.\n");
															}
														else
															printf("~s\nThis file area is read only.\n");
													}
											}
										else
											printf("~s\nUnable to get config for `%s' protocol.\n",protocol);
									}
								else
									printf("~s\nPlease select a protocol first.\n");
							}
						else
							printf("~s\nPlease enter a file area first.\n");
					}
				else
					UploadFromDisk(id);
			}
	}

void Download(BYTE *id)
	{
		BOOL local=FALSE;
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,STTAG_Local,&local,TAG_END);
		if(strlen(user))
			{
				if(!local)
					{
						BYTE protocol[33];
						strcpy(protocol,"");
						GetUserTags(USRTAG_Name,user,USRTAG_Protocol,protocol,TAG_END);
						if(strlen(protocol))
							{
								BYTE cmd[256];
								BOOL batch=FALSE,bidirectional=FALSE;
								strcpy(cmd,"");
								if(GetConfigTags(CFGTAG_Path,"Protocols",CFGTAG_Name,protocol,PRTAG_SendCommand,cmd,PRTAG_Batch,&batch,PRTAG_Bidirectional,&bidirectional,TAG_END))
									{
										if(batch && CreateFileList(id))
											{
												printf("~s\nSending files...\n~r");
												DoTransfer(id,cmd,"");
												ProcessLogFile(id);
												if(bidirectional)
													GetFileDescriptions(id);
												DeleteFileList(id);
											}
										else
											{
												if(strlen(file_area))
													{
														if(file_current)
															{
																if(Ask("Download current file",TRUE))
																	{
																		APTR group;
																		BYTE filename[33];
																		ULONG filesize=0;
																		strcpy(filename,"");
																		if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
																			{
																				BYTE *p=NULL;
																				if(GetFileTags(group,FILTAG_ID,file_current,FILTAG_Name,&p,FILTAG_Size,&filesize,TAG_END))
																					strcpy(filename,p);
																				CloseFileGroup(group);
																			}
																		if(strlen(filename))
																			{
																				BYTE baud[11];
																				ULONG creditsfile=0,creditskilo=0;
																				UBYTE ratiofile=0,ratiobyte=0;
																				UWORD timeremaining=0;
																				strcpy(baud,"");
																				if((GetVar("BAUD",baud,10,NULL)!=-1) && atol(baud))
																					{
																						GetStatusTags(STTAG_Name,id,STTAG_TimeRemaining,&timeremaining,TAG_END);
																						GetUserTags(USRTAG_Name,user,USRTAG_RatioFile,&ratiofile,USRTAG_RatioByte,&ratiobyte,USRTAG_FilesCredits,&creditsfile,USRTAG_KilosCredits,&creditskilo,TAG_END);
																						if(!ratiofile || creditsfile)
																							{
																								if(!ratiobyte || (creditskilo>=filesize/1024))
																									{
																										if(timeremaining*60>=filesize/(atol(baud)/10))
																											{
																												BYTE directory[256];
																												BOOL readonly=FALSE;
																												strcpy(directory,"");
																												GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Directory,directory,FBTAG_ReadOnly,&readonly,TAG_END);
																												if(!bidirectional || (strlen(file_area) && !readonly))
																													{
																														BYTE filebuff[256];
																														strcpy(filebuff,directory);
																														AddPart(filebuff,filename,255);
																														if(batch)
																															{
																																FILE *fp;
																																BYTE filelist_file[64];
																																sprintf(filelist_file,"T:wwbbs_filelist.%s",id);
																																if(fp=fopen(filelist_file,"w"))
																																	{
																																		fprintf(fp,"%s\n",filebuff);
																																		fclose(fp);
																																	}
																															}
																														printf("~s\nSending file...\n~r");
																														DoTransfer(id,cmd,(batch) ? "" : filebuff);
																														ProcessLogFile(id);
																														if(bidirectional)
																															GetFileDescriptions(id);
																														if(batch)
																															DeleteFileList(id);
																													}
																												else
																													printf("~s\nSince you are using a bidirectional protocol, you must change directories to\nensure that you do not upload to an invalid directory.\n");
																											}
																										else
																											printf("~s\nSorry, not enough time remaining.\n");
																									}
																								else
																									printf("~s\nSorry, not enough kilo credits.\n");
																							}
																						else
																							printf("~s\nSorry, not enough file credits.\n");
																					}
																			}
																		else
																			printf("~s\nUnable to access file.\n");
																	}
															}
														else
															printf("~s\nCurrent file does not exist.\n");
													}
												else
													printf("~s\nPlease enter a file area first.\n");
											}
									}
								else
									printf("~s\nUnable to get config for protocol `%s'.\n",protocol);
							}
						else
							printf("~s\nPlease select a protocol first.\n");
					}
				else
					printf("~s\nFor some reason, downloading from a local node is not allowed.\n");
			}
	}

ULONG CreateFileList(BYTE *id)
	{
		ULONG ret=0;
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				BYTE baud[11];
				strcpy(baud,"");
				if((GetVar("BAUD",baud,10,NULL)!=-1) && atol(baud))
					{
						ULONG files=0,kilos=0,creditsfile=0,creditskilo=0;
						UWORD timeremaining=0;
						UBYTE ratiofile=0,ratiobyte=0;
						GetStatusTags(STTAG_Name,id,STTAG_TimeRemaining,&timeremaining,TAG_END);
						GetUserTags(USRTAG_Name,user,USRTAG_RatioByte,&ratiobyte,USRTAG_RatioFile,&ratiofile,USRTAG_FilesCredits,&creditsfile,USRTAG_KilosCredits,&creditskilo,TAG_END);
						if(file_taglist->lh_Head->ln_Succ && Ask("Download tagged files",TRUE))
							{
								FILE *fp;
								BYTE filelist_file[64];
								sprintf(filelist_file,"T:wwbbs_filelist.%s",id);
								if(fp=fopen(filelist_file,"w"))
									{
										BOOL ok,nocreditsfile=FALSE,nocreditskilo=FALSE,notime=FALSE;
										printf("\n~hFile                             Size      \n~o");
										{
											struct FileTagNode *node;
											for(node=(struct FileTagNode *) file_taglist->lh_Head;node->ftn_Node.ln_Succ;node=(struct FileTagNode *) node->ftn_Node.ln_Succ)
												{
													ok=TRUE;
													if(ratiofile)
														{
															if(files+1>creditsfile)
																{
																	nocreditsfile=TRUE;
																	ok=FALSE;
																}
														}
													if(ratiobyte)
														{
															if(kilos+(node->ftn_Size/1024)>creditskilo)
																{
																	nocreditskilo=TRUE;
																	ok=FALSE;
																}
														}
													if(((kilos+(node->ftn_Size/1024))*1024)/(atol(baud)/10)>timeremaining*60)
														{
															notime=TRUE;
															ok=FALSE;
														}
													if(ok)
														{
															printf("%-32s %ld\n",FilePart(node->ftn_Name),node->ftn_Size);
															fprintf(fp,"%s\n",node->ftn_Name);
															files++;
															kilos+=node->ftn_Size/1024;
														}
												}
										}
										printf("\nTotals: %ld files, %ld kilos, %d minutes\n",files,kilos,((kilos*1024)/(atol(baud)/10))/60);
										if(nocreditsfile)
											printf("~s\nSorry, not enough file credits.\n");
										if(nocreditskilo)
											printf("~s\nSorry, not enough kilo credits.\n");
										if(notime)
											printf("~s\nSorry, not enough time remaining.\n");
										fclose(fp);
										ret=files;
									}
							}
					}
			}
		return(ret);
	}

void DeleteFileList(BYTE *id)
	{
		BYTE file[64];
		sprintf(file,"T:wwbbs_filelist.%s",id);
		DeleteFile(file);
	}

void DoTransfer(BYTE *id,BYTE *cmd,BYTE *filename)
	{
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				BPTR old_dir,new_dir;
				BYTE directory[256];
				BYTE log[64],filelist[64];
				strcpy(directory,"");
				if(strlen(file_area))
					GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Directory,directory,TAG_END);
				sprintf(log,"T:wwbbs_filelog.%s",id);
				sprintf(filelist,"T:wwbbs_filelist.%s",id);
				if(!strlen(file_area) || (new_dir=Lock(directory,SHARED_LOCK)))
					{
						if(strlen(file_area))
							old_dir=CurrentDir(new_dir);
						{
							char fullcmd[256],device[33],unit[11];
							ULONG unit_long=0;
							strcpy(fullcmd,"");
							strcpy(device,"");
							strcpy(unit,"");
							GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,id,NDTAG_Device,device,NDTAG_Unit,&unit_long,TAG_END);
							sprintf(unit,"%ld",unit_long);
							{
								char *p,*q[6],cmdbuff[256];
								int i;
								strcpy(cmdbuff,cmd);
								q[0]=q[1]=q[2]=q[3]=q[4]=q[5]=NULL;
								p=cmdbuff;
								i=0;
								while((i<6) && (p=strchr(p,'%')))
									{
										p++;
										if(*p)
											{
												switch(*p)
													{
														case 'D':
														case 'd':
															q[i]=device;
															i++;
															*p='s';
															break;
														case 'U':
														case 'u':
															q[i]=unit;
															i++;
															*p='s';
															break;
														case 'L':
														case 'l':
															q[i]=log;
															i++;
															*p='s';
															break;
														case 'F':
															q[i]=filelist;
															i++;
															*p='s';
															break;
														case 'f':
															q[i]=filename;
															i++;
															*p='s';
															break;
														case 'N':
														case 'n':
															q[i]=user;
															i++;
															*p='s';
															break;
													}
												p++;
											}
									}
								sprintf(fullcmd,cmdbuff,q[0],q[1],q[2],q[3],q[4],q[5]);
							}
							{
								LONG exitcode;
								exitcode=SystemTags(fullcmd,TAG_END);
								switch(exitcode)
									{
										case -1:
											printf("~s\nUnable to execute protocol command.\n");
											break;
										case 0:
											printf("~s\nTransfer successful.\n");
											break;
										case 1:
										case 2:
										case 3:
										case 4:
										case 5:
											printf("~s\nTransfer aborted.\n");
											break;
										default:
											printf("~s\nTransfer failed.\n");
											break;
									}
							}
						}
						if(strlen(file_area))
							{
								CurrentDir(old_dir);
								UnLock(new_dir);
							}
					}
			}
	}
