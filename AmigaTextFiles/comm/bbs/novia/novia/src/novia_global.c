#include <time.h>
#include <devices/timer.h>
#include <pragma/timer_lib.h>
#include <clib/timer_protos.h>
#include <exec/types.h>
#include <exec/io.h>
#include <exec/memory.h>
#include <devices/conunit.h>
#include <intuition/intuition.h>
#include <dos/dos.h>
#include <libraries/fifo.h>
#include <pragma/exec_lib.h>
#include <pragma/graphics_lib.h>
#include <pragma/dos_lib.h>
#include <pragma/diskfont_lib.h>
#include <pragma/intuition_lib.h>
#include <pragma/gadtools_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics/text.h>
#include <utility/date.h>
#include <novia/novia_global.h>
#include <novia/novia_misc.h>
#include <novia/novia_funcs.h>
#include <novia/novia_gui.h>
#include <novia/novia_registers.h>
#include <pragma/noviasys_lib.h>



struct TextAttr textattr[]={"cnet21",8,0,FPF_DISKFONT};

extern struct Task *MainTask;
struct Library *noviasysbase;


extern struct MainPortConfig *mainportconfig;

void GetArg(char *string);
void FreeAllMem();
void ClearMemQuick(APTR start,ULONG len);
void ItemToViewItem(APTR itemaddress,ViewItem *viewitem);
BOOL ArgToLong(const char *string,ULONG *from,ULONG *to);
BOOL DelItem(const char *path,ULONG start,ULONG end);
BOOL AddItem(struct ItemHeader *itemheader,const char *path);
void NewShell();
void CloseShell();
ULONG StrToIP(char *string);
void IPToStr(char *string,ULONG IPNumber);
void ClearList(struct List *list);
BOOL ChangeWhere(char *where);

void SelectFromTo(UBYTE *table, ULONG tablesize, char *string);
extern char StdCommand(ULONG cmdid);

extern void SendOnlineMessage(PortData *cport);
//extern struct Item *GetItem_byIP(ULONG dirIP,ULONG itemIP,struct Item *item,ULONG size);

extern int GetINETMail(void);

extern List *client_list;
extern struct GUIFrontend guifrontend;
extern struct Task *GUITask;

void GetArg(char *string)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		UBYTE len = strlen(string);
		UBYTE counter1;
		UBYTE counter2;
		char quit = FALSE;
		char *ptr;
		char *strptr;
		cport->argcounter = 0;
		for (counter1 = 0; counter1 < MAXARGS; counter1++)	// first, clearing old arg's
		{
			for (counter2 = 0; counter2 < MAXARGLEN; counter2++)
				cport->argstring[counter1][counter2] = 0;
		}

		/* next part. counter1 is the offset in string.
			     	  counter2 is the offset in cport->argstring. */

		strptr = string;
		while (!quit && cport->argcounter < MAXARGLEN)
		{
			while (isspace(*strptr))
				*strptr++;
			if (*strptr)
			{
				ptr = cport->argstring[cport->argcounter];
				if (*strptr == '"')
				{
					*strptr++;
					while (*strptr && *strptr != '"')
						*ptr++ = *strptr++;
					if (*strptr == '"')
						strptr++;
				}
				else
				{
					while (*strptr && isgraph(*strptr) && *strptr != '"')
						*ptr++ = *strptr++;
				}
				*ptr = 0;
				cport->argcounter++;
			}
			else
				quit = TRUE;
			
		}
		cport->argptr = cport->inputbuffer;
		while (*cport->argptr && *cport->argptr!=' ')
			cport->argptr++;
		while (*cport->argptr && *cport->argptr==' ')
			cport->argptr++;
	}
}


void FreeAllMem()
{
/*	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		MyMemEntry *entry;
		APTR address;
		if (alloc_mem_list)
		{
			entry=(MyMemEntry *)alloc_mem_list->lh_Head;
			while (!IsListEmpty(alloc_mem_list))
			{
				Remove((Node *)entry);
				address=entry->address;
				FreeVec(address);
				FreeVec(entry);
				entry=(MyMemEntry *)alloc_mem_list->lh_Head;
			}
			FreeVec(alloc_mem_list);
		}
	}*/
}

void ClearMemQuick(APTR start,ULONG len)
{
	ULONG *start2=(ULONG *)start;
	ULONG *end=start2+(len>>2);
	while (start2<end)
		*start2++=0;
}

void ClearMem(APTR start,ULONG len)
{
	UBYTE *start2=(UBYTE *)start;
	UBYTE *end=start2+len;
	while (start2<end)
		*start2++ = 0;
}

void ItemToViewItem(APTR itemaddress,ViewItem *viewitem)
{
	struct ItemHeader *itemheader=itemaddress;
	struct Item *item=itemaddress;
	struct Subboard *subboard=itemaddress;
	if (itemheader->ItemType==ITEMTYPE_SUBBOARD)
	{
		if (!strlen(subboard->Scanfilter))strcpy(subboard->Scanfilter,"dir");
		sprintf(viewitem->Title," c3%4d       c3%s                                                    ",viewitem->ListID+1,itemheader->Title);
		sprintf(&viewitem->Title[61],"c7(c3%3sc7)  -  c1[%02d-%02d-%02d - %02d:%02d:%02d]",subboard->Scanfilter,
			itemheader->LastChange.day,
			itemheader->LastChange.month,
			itemheader->LastChange.year,
			itemheader->LastChange.hours,
			itemheader->LastChange.mins,
			itemheader->LastChange.secs);
	}
	else
	{
		sprintf(viewitem->Title," c3%4d       c7%s                                                     ",viewitem->ListID+1,itemheader->Title);
		sprintf(&viewitem->Title[50],"c3%10dc7 bytes, %3d responds  -  c1[%02d-%02d-%02d - %02d:%02d:%02d]",item->Size,item->replys,
				itemheader->LastChange.day,
				itemheader->LastChange.month,
				itemheader->LastChange.year,
				itemheader->LastChange.hours,				itemheader->LastChange.mins,
				itemheader->LastChange.secs);
	}
	if (viewitem->select)
		*viewitem->Title='*';
}

BOOL ArgToLong(const char *string,ULONG *from,ULONG *to)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		UBYTE counter;
		UBYTE counter2=0;
		LONG ulong;
		char  string2[25];
		if (!stricmp(string,"ALL"))
		{
			*from=0;
			*to=0xFFFFFFFF;
			return TRUE;
		}
		if (StrToLong((STRPTR)string,&ulong)==-1|strlen(string)==0)return FALSE;
		for (counter=0;(counter<12 && string[counter]>='0' && string[counter]<='9');counter++)
		{
			string2[counter]=string[counter];
		}
		StrToLong(string2,(LONG *)from);
		counter++;
		string2[0]=0;
		for (;(counter<22 && string[counter]>='0' && string[counter]<='9');counter++)
		{
			string2[counter2]=string[counter];
			counter2++;
		}
		string2[counter2]=0;
		if (StrToLong(string2,(LONG *)to)==-1)
		{
			if (string[counter-1]=='-')
			{
				*to=0xFFFFFFFF;
			}
			else
			{
				*to=*from;
			}
		}
	}
	return TRUE;
}


BOOL DelItem(const char *path,ULONG start,ULONG end)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		BPTR index_mulder;
		BPTR data_mulder;
		char *buffer;
		APTR pointer;
		ULONG ulong;
	
		struct IndexHeader indexheader;
	
		if (buffer=AllocVec(256,MEMF_ANY))
		{
			strcpy(buffer,path);
			strcat(buffer,"/data/.data");
			if (index_mulder=Open(buffer,MODE_READWRITE))
			{
				if (Read(index_mulder,&indexheader,sizeof(IndexHeader))==sizeof(IndexHeader))
				{
					if (end>indexheader.Entrys-1)end=indexheader.Entrys-1;
					if (start>end)
					{
						ulong=end;
						end=start;
						start=ulong;
					}
					strcpy(buffer,path);
					strcat(buffer,"/data/.data");
					if (data_mulder=Open(buffer,MODE_READWRITE))
					{
	
						/*** End of .data-file is the offset for new subboard-structure ***/
	
						Seek(data_mulder,0,OFFSET_END);
						ulong=Seek(data_mulder,0,OFFSET_END);	// get offset for .index-entry
	
						if (pointer=AllocVec(ulong,MEMF_ANY))
						{	
							Seek(index_mulder,12,OFFSET_BEGINNING);
							Seek(index_mulder,sizeof(IndexEntry)*(end+1),OFFSET_CURRENT);
							if (end+1<indexheader.Entrys)
							{
								FRead(index_mulder,pointer,(indexheader.Entrys-end-1)*sizeof(IndexEntry),1);
								Seek(index_mulder,12,OFFSET_BEGINNING);
								Seek(index_mulder,sizeof(IndexEntry)*start,OFFSET_CURRENT);
								FWrite(index_mulder,pointer,(indexheader.Entrys-end-1)*sizeof(IndexEntry),1);
							}
							Seek(index_mulder,8,OFFSET_BEGINNING);
							indexheader.Entrys=indexheader.Entrys-(end-start+1);
							FWrite(index_mulder,&indexheader.Entrys,4,1);
							SetFileSize(index_mulder,12+indexheader.Entrys*sizeof(IndexEntry),OFFSET_BEGINNING);
							cport->readnew=TRUE;
							FreeVec(pointer);
						}
						Close(data_mulder);
					}
					else
					{
						ioprintf("\nCan't open %s. ",path);
						PrintDosError(IoErr());
					}
					Close(index_mulder);
				}
				else
				{
					ioprintf("\nCan't read %s. ",path);
					PrintDosError(IoErr());
				}
			}
			else
			{
				ioprintf("\nCan't open %s. ",path);
				PrintDosError(IoErr());
				return FALSE;
			}
		}
	}
}
	
BOOL AddItem(struct ItemHeader *itemheader,const char *path)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		BPTR index_mulder=NULL;
		BPTR data_mulder=NULL;
		BPTR lock;
		BPTR lock2;
		char *buffer;
		APTR pointer;
		ULONG ulong;
		char validate=FALSE;
		struct Item *item=(Item *)itemheader;
		struct Subboard *subboard=(Subboard *)itemheader;
		struct IndexEntry *indexentry;
		struct IndexHeader indexheader;
	
		if (!(indexentry=AllocVec(sizeof(IndexEntry),MEMF_ANY)))
		{
			return FALSE;
		}
	
		if (buffer=AllocVec(256,MEMF_ANY))
		{
			if (itemheader->ItemType=ITEMTYPE_SUBBOARD)
			{
				if (!(lock=Lock(itemheader->Path,SHARED_LOCK)))	// First locking of exists directory
				{
					if (!(lock=CreateDir(itemheader->Path)))			// create directory.
					{
						ioprintf("\nCan't create directory. %s,",itemheader->Path);
						PrintDosError(IoErr());
					}
				}
				if (lock)
				{
					UnLock(lock);
					strcpy(buffer,itemheader->Path);
					strcat(buffer,"/data");
					if (!(lock=Lock(buffer,SHARED_LOCK)))
					{
						if (!(lock=CreateDir(buffer)))
						{
							ioprintf("\nCan't Create %s. ",buffer);
							PrintDosError(IoErr());
						}
					}
				}
				if (lock)
				{
					UnLock(lock);
					validate=TRUE;
				}
			}
			else
			{
				validate=TRUE;
			}
			if (validate)
			{
				strcpy(buffer,path);
				strcat(buffer,"/data/.index");
				if (index_mulder=Open(buffer,MODE_READWRITE))
				{
					strcpy(buffer,path);
					strcat(buffer,"/data/.data");
					if (data_mulder=Open(buffer,MODE_READWRITE))
					{
						if (Read(index_mulder,&indexheader,sizeof(IndexHeader))<sizeof(IndexHeader))
						{
							ClearMemQuick(&indexheader,sizeof(IndexHeader));
							Seek(index_mulder,0,OFFSET_BEGINNING);
						}
	
						/*** Adding Username & ID of Creater ***/
	
						item->FromUser=cport->LocalUser.IDNumber;
						item->ItemType=1;
						strcpy(item->Name,cport->LocalUser.Handle);
						strcpy(item->FromRealName,cport->LocalUser.RealName);
						strcpy(item->Filename,cport->LocalUser.Handle);
						GetTime(&itemheader->CreateDate);
						GetTime(&itemheader->LastChange);
	
						/*** End of .data-file is the offset for new subboard-structure ***/
	
						ClearMemQuick(indexentry,sizeof(IndexEntry));
						Seek(data_mulder,0,OFFSET_END);
						indexentry->offset=Seek(data_mulder,0,OFFSET_END);
						itemheader->IndexOffset=indexentry->offset;
	
						/*** Calculating IDNumber of subboard ***/
	
						itemheader->IDNumber=indexheader.idcounter++;
						indexheader.Entrys++;
						Seek(index_mulder,0,OFFSET_BEGINNING);
						Write(index_mulder,&indexheader,sizeof(IndexHeader));
	
						/*** Adding subboard-Entry in .index ***/
	
						indexentry->type=itemheader->ItemType;
						Seek(index_mulder,0,OFFSET_END);
						Write(index_mulder,indexentry,sizeof(IndexEntry));
	
						/*** Saveing Subdir-Structure in .data and closing X-Files ***/
	
						if (itemheader->ItemType==ITEMTYPE_SUBBOARD)
						{
							Write(data_mulder,itemheader,sizeof(Subboard));
						}
						else
						{
							Write(data_mulder,itemheader,sizeof(Item));
						}
	
						Close(data_mulder);
					}
					else
					{
						ioprintf("\nCan't open %s. ",path);
						PrintDosError(IoErr());
						return FALSE;
					}
					Close(index_mulder);
				}
				else
				{
					ioprintf("\nCan't open %s. ",path);
					PrintDosError(IoErr());
					return FALSE;
				}
			}
			FreeVec(buffer);
			return TRUE;
		}
		return FALSE;
	}
}
	
void NewShell()
{
}

void CloseShell()
{
}

ULONG StrToIP(char *string)
{
	BYTE counter,counter2;
	ULONG IPNumber=0;
	BYTE *add=(BYTE *)&IPNumber;
	LONG erg;
	IPNumber=0;
	counter=0;
	for (counter2=0;counter2<4;counter2++)
	{
		StrToLong(&string[counter],(long *)&erg);
		add[counter2]=(BYTE)erg;
		for (counter=counter;string[counter]!='.';counter++)
		{
			if (!string[counter])return IPNumber;
		}
		counter++;
	}
	return IPNumber;
}

void IPToStr(char *string,ULONG IPNumber)
{
	ULONG ip=IPNumber;
	UBYTE *add=(char *)&ip;
	sprintf(string,"%0d.%0d.%0d.%0d",add[0],add[1],add[2],add[3]);
}


void ClearList(struct List *list)
{
	while (list->lh_Head->ln_Succ)
		FreeVec(RemHead(list));
}


BOOL ChangeWhere(char *where)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		strncpy(cport->Where,where,sizeof(cport->Where));
		return TRUE;
	}
	else
		return FALSE;
}


ULONG searchstr(char *str1, char *str2)
{
	char *pos1=str1;
	char *pos2=str2;
	if (*str1 && *str2)
	{
		while (*pos1 && *pos2)
		{
			if (tolower(*pos1)==tolower(*pos2))
				pos2++;
			else
				pos2=str2;
			pos1++;
		}
	}
	else
		return NULL;

	if (!*pos2)
		return ((pos1-str1)-(pos2-str2)+1);
	else
		return NULL;

}

ULONG searchkeyword(char *str1, char *str2)
{
	char *pos1=str1;
	char *pos2=str2;
	if (*str1 && *str2)
	{
		while (*pos1 && *pos2)
		{
			if (tolower(*pos1)==tolower(*pos2))
				pos2++;
			else
				pos2=str2;
			pos1++;
		}
	}
	else
		return NULL;

//	while (*pos1==':' | *pos1=='=' | *pos1==' ')

	if (!*pos2)
		return ((pos1-str1)-(pos2-str2)+1);
	else
		return NULL;

}

