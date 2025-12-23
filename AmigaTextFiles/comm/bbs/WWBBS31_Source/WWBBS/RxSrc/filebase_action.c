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

void ViewArchive(BYTE *id)
	{
		if(strlen(file_area))
			{
				if(file_current)
					{
						BYTE file[256];
						strcpy(file,"");
						if(GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Directory,file,TAG_END))
							{
								APTR group;
								if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
									{
										BYTE *name=NULL;
										if(GetFileTags(group,FILTAG_ID,file_current,FILTAG_Name,&name,TAG_END))
											{
												AddPart(file,name,255);
												{
													BYTE *file_extension=NULL;
													file_extension=FilePart(file);
													{
														BYTE *p;
														p=file_extension;
														while(p=strchr(p,'.'))
															{
																p++;
																file_extension=p;
															}
													}
													{
														BYTE archiver[33];
														strcpy(archiver,"");
														{
															ULONG next=0;
															BYTE *arc_name=NULL,arc_extension[4];
															strcpy(arc_extension,"");
															while(next=GetConfigTags(CFGTAG_Path,"Archivers",CFGTAG_Name,&arc_name,CFGTAG_Next,next,ARCTAG_Extension,arc_extension,TAG_END))
																{
																	if(!stricmp(arc_extension,file_extension))
																		strcpy(archiver,arc_name);
																}
														}
														if(strlen(archiver))
															{
																BYTE arc_viewcommand[512];
																strcpy(arc_viewcommand,"");
																if(GetConfigTags(CFGTAG_Path,"Archivers",CFGTAG_Name,archiver,ARCTAG_ViewCommand,arc_viewcommand,TAG_END))
																	{
																		BYTE cmd[256];
																		sprintf(cmd,arc_viewcommand,file);
																		printf("~r\n");
																		SetMode(Input(),0);
																		SystemTags(cmd,TAG_END);
																		SetMode(Input(),1);
																	}
															}
														else
															printf("~s\nUnknown archive format.\n");
													}
												}
											}
										CloseFileGroup(group);
									}
							}
					}
				else
					printf("~s\nCurrent file does not exist.\n");
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

void ReadTextFile(BYTE *id)
	{
		if(strlen(file_area))
			{
				if(file_current)
					{
						BYTE file[256];
						strcpy(file,"");
						if(GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Directory,file,TAG_END))
							{
								APTR group;
								if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
									{
										BYTE *name=NULL;
										ULONG size=0;
										if(GetFileTags(group,FILTAG_ID,file_current,FILTAG_Name,&name,FILTAG_Size,&size,TAG_END))
											{
												AddPart(file,name,255);
												{
													BPTR fh;
													LONG c;
													BOOL ok=TRUE;
													int i;
													if(fh=Open(file,MODE_OLDFILE))
														{
															for(i=0;i<(size<1024) ? size : 1024;i++)
																{
																	if((c=FGetC(fh))!=EOF)
																		{
																			if(c<0)
																				{
																					ok=FALSE;
																					break;
																				}
																		}
																	else
																		break;
																}
															Close(fh);
														}
													if(ok)
														ShowText(file);
													else
														printf("~s\n`%s' is not a text file.\n",FilePart(file));
												}
											}
										CloseFileGroup(group);
									}
							}
					}
				else
					printf("~s\nCurrent file does not exist.\n");
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}
