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

BOOL GetNextArea(BYTE *path,BYTE *name)
	{
		BOOL ret=FALSE;
		if(strlen(name))
			{
				BYTE next[33];
				strcpy(next,"");
				GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_GetNext,next,TAG_END);
				if(strlen(next))
					{
						BOOL child=FALSE;
						GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,next,CFGTAG_Child,&child,TAG_END);
						if(child)
							{
								AddPart(path,next,255);
								strcpy(name,"");
								ret=GetNextArea(path,name);
							}
						else
							{
								strcpy(name,next);
								ret=TRUE;
							}
					}
				else
					{
						if(strchr(path,'/'))
							{
								char *p;
								strcpy(name,FilePart(path));
								p=PathPart(path);
								*p=NULL;
								ret=GetNextArea(path,name);
							}
					}
			}
		else
			{
				BYTE first[33];
				strcpy(first,"");
				GetConfigTags(CFGTAG_Path,path,CFGTAG_GetFirst,first,TAG_END);
				if(strlen(first))
					{
						BOOL child=FALSE;
						GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,first,CFGTAG_Child,&child,TAG_END);
						if(child)
							{
								AddPart(path,first,255);
								ret=GetNextArea(path,name);
							}
						else
							{
								strcpy(name,first);
								ret=TRUE;
							}
					}
			}
		return(ret);
	}

BOOL GetPreviousArea(BYTE *path,BYTE *name)
	{
		BOOL ret=FALSE;
		if(strlen(name))
			{
				BYTE previous[33];
				strcpy(previous,"");
				GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_GetPrevious,previous,TAG_END);
				if(strlen(previous))
					{
						BOOL child=FALSE;
						GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,previous,CFGTAG_Child,&child,TAG_END);
						if(child)
							{
								AddPart(path,previous,255);
								strcpy(name,"");
								ret=GetPreviousArea(path,name);
							}
						else
							{
								strcpy(name,previous);
								ret=TRUE;
							}
					}
				else
					{
						if(strchr(path,'/'))
							{
								char *p;
								strcpy(name,FilePart(path));
								p=PathPart(path);
								*p=NULL;
								ret=GetPreviousArea(path,name);
							}
					}
			}
		else
			{
				BYTE last[33];
				strcpy(last,"");
				GetConfigTags(CFGTAG_Path,path,CFGTAG_GetLast,last,TAG_END);
				if(strlen(last))
					{
						BOOL child=FALSE;
						GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,last,CFGTAG_Child,&child,TAG_END);
						if(child)
							{
								AddPart(path,last,255);
								ret=GetPreviousArea(path,name);
							}
						else
							{
								strcpy(name,last);
								ret=TRUE;
							}
					}
			}
		return(ret);
	}
