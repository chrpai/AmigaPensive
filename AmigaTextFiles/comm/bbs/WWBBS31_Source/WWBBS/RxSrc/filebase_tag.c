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

void ClearFileTagList(BYTE *id)
	{
		if(Ask("Clear file tag list",FALSE))
			{
				struct Node *node;
				while(node=RemHead(file_taglist))
					FreeVec(node);
				printf("~s\nFile tag list cleared.\n");
			}
	}

void EditFileTagList(BYTE *id)
	{
		BOOL kg=TRUE;
		int count;
		while(!panic && kg)
			{
				count=0;
				{
					struct Node *node;
					for(node=file_taglist->lh_Head;node->ln_Succ;node=node->ln_Succ)
						count++;
				}
				if(count)
					{
						{
							int width,i=0;
							width=((int) (log10((double) count)+1)>3) ? (int) (log10((double) count)+1) : 3;
							printf("\n~h");
							printf("%*sNum File                             Size      \n",width-3,"");
							printf("~o");
							{
								struct FileTagNode *node;
								for(node=(struct FileTagNode *) file_taglist->lh_Head;node->ftn_Node.ln_Succ;node=(struct FileTagNode *) node->ftn_Node.ln_Succ)
									{
										if(WaitForChar(Input(),0))
											{
												getchar();
												break;
											}
										printf("%*d %-32s %ld\n",width,i+1,FilePart(node->ftn_Name),node->ftn_Size);
										i++;
									}
							}
						}
						{
							char buff[11];
							printf("~p\nEdit File Tag List: ");
							if(GetLine(buff,10,GLFLG_NoChars))
								{
									struct FileTagNode *wn,*nn;
									int i=0;
									wn=(struct FileTagNode *) file_taglist->lh_Head;
									while(nn=(struct FileTagNode *) wn->ftn_Node.ln_Succ)
										{
											if(IsRange(buff,i+1))
												{
													Remove((struct Node *) wn);
													FreeVec(wn);
												}
											wn=nn;
											i++;
										}
								}
							else
								kg=FALSE;
						}
					}
				else
					{
						printf("~s\nFile tag list is empty.\n");
						kg=FALSE;
					}
			}
	}

void TagFile(BYTE *id)
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
												if(FindName(file_taglist,file))
													{
														if(DeleteFileTag(file_taglist,file))
															printf("~s\nRemoved `%s' from file tag list.\n",name);
														else
															printf("~s\nUnable to remove `%s' from file tag list.\n",name);
													}
												else
													{
														if(AddFileTag(file_taglist,file_path,file_area,file_current,file,size))
															printf("~s\nAdded `%s' to file tag list.\n",name);
														else
															printf("~s\nUnable to add `%s' to file tag list.\n",name);
													}
											}
										else
											printf("~s\nUnable to get current file.\n");
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
