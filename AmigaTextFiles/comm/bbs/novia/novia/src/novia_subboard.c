#include <exec/types.h>
#include <stdlib.h>
#include <intuition/screens.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <novia/novia_UserList.h>
#include <novia/novia_PortData.h>
#include <novia/novia_config.h>
#include <novia/novia_mail.h>
#include <novia/novia_subboard.h>
#include <novia/novia_types.h>
#include <novia/novia_misc.h>
#include <novia/novia_message.h>
#include <exec/memory.h>
#include <dos/stdio.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <novia/novia_funcs.h>

void GoBase();
void RepairBase();
struct Item *GetItem_byIP(ULONG dirIP,ULONG itemIP,struct Item *item,ULONG size);
struct Subboard *GetSubboard_byIP(ULONG serialIP,struct Subboard *subboard,ULONG size);

struct MyNode {
	struct MyNode 	*ln_Succ;
	struct MyNode	*ln_Pred;
	UBYTE			ln_Type;
	BYTE			ln_Pri;
	BYTE 			*ln_Name;
	ULONG			IPNumber;
};


void GoBase()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	char file_index[]=".index";
	char file_data[]=".data";
	char buffer[256],Quit=FALSE,Quit2=FALSE,habschon;
	char root[70];
	char path[256];
	char odir[256];
	List dirlist;
	char nostdcmd;
	long cmdid=0;
	ULONG start;
	ULONG end;
	ULONG entrys=0;
	APTR aptr;
	char *pointer;
	BPTR index_mulder=NULL,
		 data_mulder=NULL,
		 mulder=NULL,
		 lock=NULL,
		 lock2=NULL;
	ULONG	index_size,
			data_size,
			ulong,
			counter;
	struct Subboard *subboard;
	struct ItemHeader *itemheader;
	struct Item *item;
	struct FileInfoBlock *fib;
	struct ViewItem *viewitem=0;
	struct ViewList vlist;
	struct ViewTable viewtable;
	struct IndexEntry *indexentry;
	struct IndexEntry *indexentry2;
	struct IndexHeader indexheader;
	struct Reply *reply;
	struct MyNode *currentdir=0;
	struct MyNode *newdir=0;
	struct SelectItem *select;
	ULONG dircounter=0;
	ULONG itemcounter=0;
	BOOL readnew=TRUE;

	if (!cport)return;

	NewList(&dirlist);

	sprintf(root,"%sroot/",cport->sysdirs->sysdata);
	ClearMemQuick(&viewtable,sizeof(ViewTable));

	if (!(fib=AllocDosObject(DOS_FIB,NULL)))Quit=TRUE;
	if (!(item=AllocVec(sizeof(Item),MEMF_ANY|MEMF_CLEAR)))Quit=TRUE;
//	if (!(viewtable.item=AllocVec(viewtable.maxitems*sizeof(ViewItem),MEMF_ANY)))Quit=TRUE;
	if (!(indexentry=AllocVec(sizeof(IndexEntry),MEMF_ANY)))Quit=TRUE;
	if (!(reply=AllocVec(sizeof(Reply),MEMF_ANY)))Quit=TRUE;

	if (Quit)
	{
		if (fib)FreeDosObject(DOS_FIB,fib);
		if (item)FreeVec(item);
		if (viewtable.item)FreeVec(viewtable.item);
		if (indexentry)FreeVec(indexentry);
		if (reply)FreeVec(reply);
		return;
	}

	subboard=(Subboard *)item;
	itemheader=(ItemHeader *)item;

	if (!(lock=Lock(root,SHARED_LOCK)))
	{
		if (!(lock=CreateDir(root)))
			Quit=TRUE;
		else
			ChangeMode(CHANGE_LOCK,lock,SHARED_LOCK);
	}
	if (lock)
	{
		if (NameFromLock(lock,buffer,255))
		{
			strcpy(path,buffer);
			strcat(path,"/data/.index");
			if (currentdir=AllocVec(sizeof(Node),MEMF_ANY|MEMF_CLEAR))
			{
				if (currentdir->ln_Name=AllocVec(strlen(buffer)+1,MEMF_ANY))
				{
					strcpy(currentdir->ln_Name,buffer);
					AddTail(&dirlist,(Node *)currentdir);
//					sprintf(path,"%s/data",buffer);
					if (!(index_mulder=Open(path,MODE_READWRITE)))
					{
						if (lock2=CreateDir(path))
						{
							UnLock(lock2);
							strcat(path,"/.index");
							if (!(index_mulder=Open(path,MODE_READWRITE)))
							{
								Quit=TRUE;
								ioprintf("can't create root.index.\n");
							}
						}
						else
						{
							ioprintf("can't create root/data directory.\n");
							Quit=TRUE;
						}
					}
				}
				else
				{
					FreeVec(currentdir);
					Quit=TRUE;
				}
			}
		}
		UnLock(lock);
		if (index_mulder)
		{
			Seek(index_mulder,0,OFFSET_END);
			index_size=Seek(index_mulder,0,OFFSET_END);
			if (Seek(index_mulder,0,OFFSET_BEGINNING)>=sizeof(IndexHeader))
			{
				Read(index_mulder,&indexheader,sizeof(IndexHeader));
			}
			else
			{
				ClearMemQuick(&indexheader,sizeof(IndexHeader));
				Write(index_mulder,&indexheader.Entrys,4);
				ioprintf("index mulder is empty\n");
			}
			Close(index_mulder);
		}
		else
		{
			Quit=TRUE;
			ioprintf("can't open %s\n",path);
		}
	}
	while (!Quit && !cport->ProgramClose && cport->network.online)
	{
		if (readnew)
		{
			if (viewtable.item)FreeVec(viewtable.item);
			ClearMemQuick(&indexheader,sizeof(IndexHeader));
			viewtable.item=0;
			itemcounter=0;
			dircounter=0;
			viewtable.items=0;
			viewtable.maxitems=0;
			strcpy(path,currentdir->ln_Name);
			strcat(path,"/data/.index");
			if (index_mulder=Open(path,MODE_READWRITE))
			{
				Seek(index_mulder,0,OFFSET_BEGINNING);
				Read(index_mulder,&indexheader,sizeof(IndexHeader));
			}
			else
			{
				ioprintf("can't open %s.\n",path);
			}
			if (indexheader.Entrys)
			{
				strcpy(path,currentdir->ln_Name);
				strcat(path,"/data/.data");
				data_mulder=Open(path,MODE_READWRITE);
				Quit2=FALSE;
				counter=0;
				ClearMemQuick(&viewtable,sizeof(ViewTable));
				if (viewtable.item=AllocVec(sizeof(ViewItem)*(indexheader.Entrys+200),MEMF_ANY|MEMF_CLEAR))
				{
					viewtable.maxitems=indexheader.Entrys+200;
					while (!Quit2)
					{
						if (Read(index_mulder,indexentry,sizeof(IndexEntry))<sizeof(IndexEntry))Quit2=TRUE;
						if (!Quit2)
						{
							if (Seek(data_mulder,indexentry->offset,OFFSET_BEGINNING)>-1)
							{
								viewtable.maxitems=indexheader.Entrys+200;
								if (indexentry->type==ITEMTYPE_SUBBOARD)
								{
									if (Read(data_mulder,itemheader,sizeof(Subboard))==sizeof(Subboard))
									{
										select=(SelectItem *)cport->select_list.mlh_Head;
										while (select->ln_Succ)
										{
											if (select->dirIP==currentdir->IPNumber && select->itemIP==itemheader->IPNumber)
												viewtable.item[viewtable.items].select=TRUE;
											select=select->ln_Succ;
										}
										dircounter++;
										viewtable.item[viewtable.items].ItemType=indexentry->type;
										viewtable.item[viewtable.items].Seek=indexentry->offset;
										viewtable.item[viewtable.items].ListID=counter;
										viewtable.item[viewtable.items].IndexID=counter++;
										viewtable.item[viewtable.items].ItemIP=itemheader->IPNumber;
										ItemToViewItem(itemheader,&viewtable.item[viewtable.items]);
										viewtable.items++;
									}
									else
									{
										Quit2=TRUE;
									}
								}
								else
								{
									if (Read(data_mulder,itemheader,sizeof(Item))==sizeof(Item))
									{
										select=(SelectItem *)cport->select_list.mlh_Head;
										while (select->ln_Succ)
										{
											if (select->dirIP==currentdir->IPNumber && select->itemIP==itemheader->IPNumber)
												viewtable.item[viewtable.items].select=TRUE;
											select=select->ln_Succ;
										}
										itemcounter++;
										viewtable.item[viewtable.items].ItemType=indexentry->type;
										viewtable.item[viewtable.items].Seek=indexentry->offset;
										viewtable.item[viewtable.items].ListID=counter;
										viewtable.item[viewtable.items].ItemIP=itemheader->IPNumber;
										viewtable.item[viewtable.items].IndexID=counter++;
										ItemToViewItem(itemheader,&viewtable.item[viewtable.items]);
										viewtable.items++;
									}
									else
									{
										Quit2=TRUE;
									}
								}
							}
							else
							{
								Quit2=TRUE;
							}
						}
						else
						{
							Quit2=TRUE;
						}
					}
				}
				if (data_mulder)Close(data_mulder);
			}
			readnew=FALSE;
			if (index_mulder)Close(index_mulder);
			if (indexheader.Entrys)
			{
				ioprintf("Subboardname: c7%s c2SerialIP: %05d\n",currentdir->ln_Name,currentdir->IPNumber);
				LF();
				for (counter=0; counter<indexheader.Entrys; counter++)
					ioprintf("%s\n",viewtable.item[counter].Title);
				ioprintf("c7%d c3directories - c7%d c3items\n",dircounter,itemcounter);
			}
			else
			{
				ioprintf("\nDirectory is empty\n");
			}
		}
		ioprintf("\nc7%d.%s>",cport->clientconfig.IDNumber,currentdir->ln_Name);
		Getstring(buffer,0,50,0,0);
		GetArg(buffer);
		nostdcmd=TRUE;
		switch(cmdid=GetCommandID(cport->argstring[0]))
		{
			/****************************************************************************/
			case CMD_REPAIR:
				RepairBase();
				nostdcmd=FALSE;				
				break;
			case CMD_EDIT:
				StrToLong(cport->argstring[1],(LONG *)&ulong);ulong--;
				if (ulong>=0 && ulong<indexheader.Entrys)
				{
					strcpy(path,currentdir->ln_Name);
					strcat(path,"/data/.data");
					if (data_mulder=Open(path,MODE_READWRITE))
					{
						Seek(data_mulder,viewtable.item[ulong].Seek,OFFSET_BEGINNING);
						if (viewtable.item[ulong].ItemType==ITEMTYPE_SUBBOARD)
						{
							if (FRead(data_mulder,subboard,sizeof(Subboard),1))
							{
								if (VDE(VDEID_SUBBOARD,(char *)subboard)==TRUE)
								{
									Seek(data_mulder,viewtable.item[ulong].Seek,OFFSET_BEGINNING);
									FWrite(data_mulder,subboard,sizeof(Subboard),1);
									readnew=TRUE;
								}
							}
						}
						else
						{
							if (FRead(data_mulder,item,sizeof(Item),1))
							{
							}
						}
						Close(data_mulder);
					}
				}
				else
				{
					ioprintf("invalid Itemnumber: %d\n",ulong);
				}
				nostdcmd=FALSE;
				break;
			case CMD_ADOPT_ORPHANS:
				ioprintf("\nEnter directory :> ",-1);
				Getstring(buffer,0,50,0,0);
				ioprintf("\n");
				if (lock2=Lock(buffer,SHARED_LOCK))
				{
					Examine(lock2,fib);
					Quit2=FALSE;
					NewList((List *)&vlist);
					vlist.entrys=0;
					NameFromLock(lock2,odir,256);
					while (ExNext(lock2,fib) && !Quit2)
					{
						if (fib->fib_DirEntryType==ST_FILE)
						{
							sprintf(buffer,"c7Add %s ? [c1Yc7es,No,Stop,Cancel]>",fib->fib_FileName);
							ioprintf("\n");
							Writeio(buffer,-1);
							switch (GetChar("",0))
							{
								case 'Y':case 'y':
									Writeio("Yes",-1);
									if (viewitem=AllocVec(sizeof(ViewItem),MEMF_ANY|MEMF_CLEAR))
									{
										strcpy(viewitem->Title,fib->fib_FileName);
										AddTail((List *)&vlist,(Node *)viewitem);
										viewitem->Size=(LONG)fib->fib_Size;
										vlist.entrys++;
									}
									break;
								case 'N':case 'n':
									Writeio("No",-1);
									break;
								case 'S':case 's':
									Writeio("Stop",-1);
									Quit2=TRUE;
									break;
								case 'C':case 'c':
									Writeio("Cancel",-1);
									while (!IsListEmpty((List *)&vlist))
									{
										viewitem=vlist.lh_Head;
										Remove((Node *)viewitem);
										FreeVec(viewitem);
									}
									Quit2=TRUE;
									break;
							}
						}
					}
					UnLock(lock2);
					ioprintf("\n");
					strcpy(path,currentdir->ln_Name);
					strcat(path,"/data/.index");
					if (index_mulder=Open(path,MODE_READWRITE))
					{
						strcpy(path,currentdir->ln_Name);
						strcat(path,"/data/.data");
						if (data_mulder=Open(path,MODE_READWRITE))
						{
							if (indexheader.Entrys+vlist.entrys>=viewtable.maxitems)
							{
								if (viewitem=AllocVec(sizeof(ViewItem)*(indexheader.Entrys+vlist.entrys+200),MEMF_ANY|MEMF_CLEAR))
								{
									if (viewtable.item)
									{
										FreeVec(viewtable.item);
										CopyMemQuick(viewitem,viewtable.item,indexheader.Entrys*sizeof(ViewItem));
									}
									viewtable.maxitems = indexheader.Entrys+vlist.entrys+200;
									viewtable.item		 = viewitem;
								}
							}
							ioprintf("\nalloc: %u - entrys %u size: %u\n",sizeof(IndexEntry)*vlist.entrys,vlist.entrys,sizeof(IndexEntry));
							if (indexentry2=AllocVec(sizeof(IndexEntry)*vlist.entrys,MEMF_ANY|MEMF_CLEAR))
							{
								if (item=AllocVec(sizeof(Item)*vlist.entrys,MEMF_ANY|MEMF_CLEAR))
								{
									Seek(data_mulder,0,OFFSET_END);
									ulong=Seek(data_mulder,0,OFFSET_END);	// get offset for .index-entry
									counter=0;
									while (!IsListEmpty((List *)&vlist))
									{
										/*** Get a entrys from edit ViewList (vlist) ***/
											viewitem=vlist.lh_Head;
	
										/*** init itemstructure, adding user, itemtype, time etc ***/
	
										item[counter].itemheader.ItemType=ITEMTYPE_FILEITEM;
										strcpy(item[counter].itemheader.Title,viewitem->Title);
										strcpy(item[counter].itemheader.Path,currentdir->ln_Name);
										strcat(item[counter].itemheader.Path,viewitem->Title);
										item[counter].FromUser=cport->LocalUser.IDNumber;
										item[counter].ItemType=1;
										strcpy(item[counter].Name,cport->LocalUser.Handle);
										strcpy(item[counter].FromRealName,cport->LocalUser.RealName);
										sprintf(item[counter].Filename,"%s/%s",odir,viewitem->Title);
										GetTime(&item[counter].SendDate);
										GetTime(&item[counter].itemheader.CreateDate);	
										GetTime(&item[counter].itemheader.LastChange);
										item[counter].Size=viewitem->Size;
	
										/*** End of .data-file is the offset for new subboard-structure ***/
	
										indexentry2[counter].offset=ulong;
										ulong=ulong+sizeof(Item);

										/*** Calculating IDNumber of subboard ***/
	
										item[counter].itemheader.IDNumber=indexheader.idcounter++;
	
										/*** Adding item-entry ***/
	
										indexentry2[counter].type=item[counter].itemheader.ItemType;

										/*** Remove Entry from edit viewlist and Add Entry to System-Viewlist ***/
	
										Remove((Node *)viewitem);
										if (viewtable.items<viewtable.maxitems)
										{
											itemcounter++;
											viewitem->ItemType=item[counter].itemheader.ItemType;
											viewitem->Seek=indexentry2[counter].offset;
											viewitem->ListID=indexheader.Entrys;
											viewitem->IndexID=indexheader.Entrys;
											ItemToViewItem(&item[counter].itemheader,viewitem);				// Fill viewitem->Title with item-datas
											CopyMemQuick(viewitem,&viewtable.item[viewtable.items],sizeof(ViewItem));
											viewtable.items++;
										}
										FreeVec(viewitem);
										indexheader.Entrys++;
										counter++;
									}
									Seek(index_mulder,0,OFFSET_BEGINNING);
									Write(index_mulder,&indexheader,sizeof(IndexHeader));
									Seek(index_mulder,0,OFFSET_END);
									Write(index_mulder,indexentry2,sizeof(IndexEntry)*vlist.entrys);
									Seek(data_mulder,0,OFFSET_END);
									Write(data_mulder,item,sizeof(Item)*vlist.entrys);
									FreeVec(item);
									item=(Item *)subboard;
									LF();
									for (counter=0; counter<indexheader.Entrys; counter++)
										ioprintf("%s\n",viewtable.item[counter].Title);
									ioprintf("c7%d c3directories - c7%d c3items\n",dircounter,itemcounter);
								}
								else
								{
									ioprintf("\nCan't alloc mem for item.\n");
								}
								FreeVec(indexentry2);
							}
							else
							{
								ioprintf("\nCan't alloc mem for indexentry\n");
							}
							Close(index_mulder);
							Close(data_mulder);
							}
						else
						{
							Close(index_mulder);
						}
					}
				}
				nostdcmd=FALSE;
				break;
				/**************************************************************************/
				case CMD_SUBBOARD_NEW:
					ClearMemQuick(subboard,sizeof(Subboard));
				ioprintf("\nc7Please Enter the name of subboard\n :> ");
				Getstring(itemheader->Title,0,50,0,0);
				if (strlen(itemheader->Title)>=3)
				{
					ioprintf("\nc7Please Enter the pathname (without /\/././\/./\.).\n:> ");
					sprintf(buffer,"%s/%s",currentdir->ln_Name,itemheader->Title);
					Getstring(itemheader->Path,buffer,70,0,0);
					if (!strlen(itemheader->Path))
					{
						strcpy(itemheader->Path,itemheader->Title);
						Writeio(itemheader->Path,-1);
					}
					if (strlen(itemheader->Path)>=3)
					{
						if (VDE(VDEID_SUBBOARD,(char *)subboard)==TRUE)
						{
							itemheader->ItemType=ITEMTYPE_SUBBOARD;
							if (AddItem(itemheader,(char *)currentdir->ln_Name))
							{
									strcpy(currentdir->ln_Name,itemheader->Path);
								/*** Adding directory to list ***/
									if (viewtable.items+1>=viewtable.maxitems)
								{
									if (viewitem=AllocVec(sizeof(ViewItem)*(viewtable.maxitems+200),MEMF_ANY|MEMF_CLEAR))
									{
										if (viewtable.item)
										{
											FreeVec(viewtable.item);
											CopyMemQuick(viewtable.item,viewitem,viewtable.items*sizeof(ViewItem));
										}
										viewtable.item=viewitem;
										viewtable.maxitems=viewtable.maxitems+200;
									}
								}
								if (viewtable.items<viewtable.maxitems)
								{
									dircounter++;
									viewtable.item[viewtable.items].ItemType=itemheader->ItemType;
									viewtable.item[viewtable.items].Seek=itemheader->IndexOffset;
									viewtable.item[viewtable.items].ListID=indexheader.Entrys;
									viewtable.item[viewtable.items].IndexID=indexheader.Entrys++;
									ItemToViewItem(itemheader,&viewtable.item[viewtable.items]);
									viewtable.items++
								}
								else
								{
									ErrorMessage("Can't add directory to displaylist, not anouth memory.");
								}
							}
						}
					}
					else
					{
						Writeio("the minimum length of path must be 3 chars!",-1);
					}
				}
				else
				{
					Writeio("the minimum length of title must be 3 chars!",-1);
				}
				nostdcmd=FALSE;
				break;
			/****************************************************************************/
			case CMD_LIST: case CMD_DIR: case CMD_BROWSE:
				LF();
				for (counter=0; counter<indexheader.Entrys; counter++)
					ioprintf("%s\n",viewtable.item[counter].Title);
				ioprintf("c7%d c3directories - c7%d c3items\n",dircounter,itemcounter);
				nostdcmd=FALSE;
				break;
			case CMD_DEL:
				if (ArgToLong(cport->argstring[1],&start,&end))
				{
					start--;end--;
					DelItem(currentdir->ln_Name,start,end);
				}
				else
				{
					ioprintf("\nUSAGE: delete [FROM]-[TO] | [ALL]\n");
				}
				nostdcmd=FALSE;
				break;
			case CMD_PARRENT:
				if (currentdir->ln_Pred->ln_Pred)
				{
					ioprintf("search select items \n");
					for (counter=0; counter<indexheader.Entrys; counter++)
					{
						ioprintf("counting %d\n",counter);
						if (viewtable.item[counter].ItemIP)
						{
							char q=FALSE;
							ioprintf("select: %d\n",viewtable.item[counter].ItemIP);
							select=(SelectItem *)&cport->select_list.mlh_Head;
							while (!q && select->ln_Succ)
							{
								if (select->dirIP==currentdir->IPNumber &&
									select->itemIP==viewtable.item[counter].ItemIP)q=TRUE;
								if (!q)select=select->ln_Succ;
							}
							if (viewtable.item[counter].select)
							{
								if (!q)
								{
									if (select=AllocVec(sizeof(SelectItem),MEMF_ANY|MEMF_CLEAR))
									{
										select->dirIP=currentdir->IPNumber;
										select->itemIP=viewtable.item[counter].ItemIP;
										AddTail((List *)&cport->select_list,(Node *)select);
									}
								}
							}
							else
							{
								if (q && select)
								{
									Remove((Node *)select);
									FreeVec(select);
								}
							}
						}
					}					
					newdir=currentdir->ln_Pred;
					Remove((Node *)currentdir);
					if (currentdir->ln_Name)
						FreeVec(currentdir->ln_Name);
					FreeVec(currentdir);
					currentdir=newdir;
					readnew=TRUE;
				}				
				else
				{
					Quit=TRUE;
				}				
				nostdcmd=FALSE;
				break;
			case CMD_POST:
				ClearMemQuick(item,sizeof(Item));
				ioprintf("\nc7Subject:> ");
				Getstring(itemheader->Title,0,50,0,0);
				if (strlen(itemheader->Title)>=3)
				{
					if (VisualEditor("ram:temp"))
					{
						if (mulder=Open("ram:temp",MODE_OLDFILE))
						{
							Seek(mulder,0,OFFSET_END);
							if (ulong=Seek(mulder,0,OFFSET_BEGINNING))
							{
								aptr=AllocVec(ulong,MEMF_ANY);
								FRead(mulder,aptr,ulong,1);
									strcpy(path,currentdir->ln_Name);
								strcat(path,"/data/.index");
								if (index_mulder=Open(path,MODE_READWRITE))
								{
									sprintf(path,"%s/data/%x.text",currentdir->ln_Name,indexheader.idcounter+1);
									if (data_mulder=Open(path,MODE_NEWFILE))
									{
										FWrite(data_mulder,aptr,ulong,1);
										item->seek=0;
										item->length=Seek(data_mulder,0,OFFSET_END);
										Close(data_mulder);
											sprintf(item->itemheader.Path,"%x",indexheader.idcounter+1);
								
										strcpy(path,currentdir->ln_Name);
										strcat(path,"/data/.data");
										if (data_mulder=Open(path,MODE_READWRITE))
										{
											/*** Adding Username & ID of Creater ***/
	
											itemheader->ItemType=ITEMTYPE_ITEM;
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
		
											/*** Calculating IDNumber of subboard ***/
	
											itemheader->IDNumber=indexheader.idcounter++;
											Seek(index_mulder,0,OFFSET_BEGINNING);
											indexheader.Entrys++;
											Write(index_mulder,&indexheader,sizeof(IndexHeader));
	
											/*** Adding subboard-Entry in .index ***/
	
											indexentry->type=itemheader->ItemType;
											Seek(index_mulder,0,OFFSET_END);
											Write(index_mulder,indexentry,sizeof(IndexEntry));	// fill up to 20 Bytes
	
											/*** Saveing Subdir-Structure in .data and closing X-Files ***/
	
											Write(data_mulder,item,sizeof(Item));
											Close(data_mulder);
											Close(index_mulder);		
	
											/*** Adding Item to list ***/
	
											if ((viewtable.items+1)>=viewtable.maxitems)
											{
												if (viewitem=AllocVec(sizeof(ViewItem)*(viewtable.maxitems+200),MEMF_ANY|MEMF_CLEAR))
												{
													if (viewtable.maxitems)CopyMemQuick(viewtable.item,viewitem,(viewtable.maxitems+200)*sizeof(ViewItem));
													viewtable.maxitems=viewtable.maxitems+200;
													FreeVec(viewtable.item);
													viewtable.item=viewitem;
												}
											}
											if (viewtable.items<viewtable.maxitems)
											{
												itemcounter++;
												viewtable.item[viewtable.items].ItemType=itemheader->ItemType;
												viewtable.item[viewtable.items].Seek=indexentry->offset;
												viewtable.item[viewtable.items].ListID=indexheader.Entrys;
												viewtable.item[viewtable.items].IndexID=indexheader.Entrys++;
												ItemToViewItem(itemheader,&viewtable.item[viewtable.items]);
												viewtable.items++
											}
											else
											{
												ErrorMessage("Can't add directory to displaylist, out of memory.");
											}
										}
										else
										{
											Close(index_mulder);
											ioprintf("\nCan't open %s. ",path);
											PrintDosError(IoErr());
										}
									}
									else
									{
										ulong=(LONG)IoErr();
										ioprintf("\nCan't open %s. ",path);
										PrintDosError(IoErr());
									}
								}
								else
								{
									ulong=(LONG)IoErr();
									ioprintf("Can't open %s. ",path);
									PrintDosError(IoErr());
								}
								FreeVec(aptr);
							}
							else
							{
								ioprintf("\nral:temp is empty.Tn");
							}
							Close(mulder);
						}
						else
						{
							ioprintf("\nCan't open ram:temp\n");
						}
					}
				}
				nostdcmd=FALSE;
				break;
			case CMD_HELP:
				sprintf(buffer,"%ssys.base.text",cport->sysdirs->systext);
				ShowText(buffer);LF();
				nostdcmd=FALSE;
				break;
			case CMD_QUIT:
				Quit=TRUE;
				nostdcmd=FALSE;
				break;
			case CMD_DOWNLOAD_DIRECT:
				{
					struct SelectItem *select;
					ULONG start = 0,
							end	= 0;
					if (ArgToLong(cport->argstring[1],&start,&end))
					{
						char quit=FALSE;
						start--;end--;
						if (end > indexheader.Entrys)
							end = indexheader.Entrys-1;

						for (start=start; start<end; start++)
						{
							if (select=AllocVec(sizeof(SelectItem),MEMF_ANY|MEMF_CLEAR))
							{
								select->dirIP	= currentdir->IPNumber;
								select->itemIP	= viewtable.item[start].ItemIP;
								select->downloadnow=2;											// 2, to find direct download files
								AddTail((List *)&cport->select_list,(Node *)select);
							}
						}

						select=(SelectItem *)cport->select_list.mlh_Head;
						cport->actual = NULL;													// reset actual download file
						while (select->ln_Succ && !quit)
						{
							if (select->downloadnow == 2)										// searching for first direct download file
							{
								cport->actual = select;											// set first file to download
								quit = TRUE;
							}
							select=select->ln_Succ;
						}
						if (quit)
							DoTransfer();															// if quit == TRUE, selected files was ok.
					}
					else
					{
						ioprintf("\nUSAGE: delete [FROM]-[TO] | [ALL]\n");
					}
				}
				nostdcmd=FALSE;
				break;

		}
		if (nostdcmd)
		{
			if (!StdCommand(cmdid) && *buffer)
			{
				StrToLong(cport->argstring[0],(long *)&ulong);
				ulong--;
				if (ulong>=0 && ulong<indexheader.Entrys)
				{
					strcpy(path,currentdir->ln_Name);
					strcat(path,"/data/.data");
					if (data_mulder=Open(path,MODE_OLDFILE))
					{
						Seek(data_mulder,viewtable.item[ulong].Seek,OFFSET_BEGINNING);
						if (viewtable.item[ulong].ItemType==ITEMTYPE_SUBBOARD)
						{
							if (FRead(data_mulder,subboard,sizeof(Subboard),1))
							{
								if (newdir=AllocVec(sizeof(Node),MEMF_ANY|MEMF_CLEAR))
								{
									if (newdir->ln_Name=AllocVec(strlen(itemheader->Path)+1,MEMF_ANY|MEMF_CLEAR))
									{
										for (counter=0; counter<indexheader.Entrys; counter++)
										{
											if (viewtable.item[counter].ItemIP)
											{
												char q=FALSE;
												select=(SelectItem *)&cport->select_list.mlh_Head;
												while (!q && select->ln_Succ)
												{
													if (select->dirIP==currentdir->IPNumber &&
														select->itemIP==viewtable.item[counter].ItemIP)q=TRUE;
													if (!q)select=select->ln_Succ;
												}
												if (viewtable.item[counter].select)
												{
													if (!q)
													{
														if (select=AllocVec(sizeof(SelectItem),MEMF_ANY|MEMF_CLEAR))
														{
															select->dirIP=currentdir->IPNumber;
															select->itemIP=viewtable.item[counter].ItemIP;
															AddTail((List *)&cport->select_list,(Node *)select);
														}
													}
												}
												else
												{
													if (q && select)
													{
														Remove((Node *)select);
														FreeVec(select);
													}
												}
											}
										}
										strcpy(newdir->ln_Name,itemheader->Path);
										AddTail(&dirlist,(Node *)newdir);
										currentdir=newdir;
										currentdir->IPNumber=itemheader->IPNumber;
										readnew=TRUE;
									}
									else
									{
										ioprintf("Can't change directory, out of memory\n");
										FreeVec(newdir);
									}
								}
							}
						}
						else
						{
							if (FRead(data_mulder,item,sizeof(Item),1))
							{
								ReadSubboard(item,currentdir->ln_Name);
								readnew=TRUE;
							}
						}
						Close(data_mulder);
					}
					else
						ioprintf("can't open: %s\n",path);
				}
				else
				{
					if (strlen(cport->argstring[0]))
					{
						if (*cport->argstring[0]=='*')
						{
							{
								ULONG from=0;
								ULONG to=0;
								char *pt=cport->argstring[0];
								while (*pt)
								{
									if (*pt>=48 && *pt<=57)
									{
										StrToLong(pt,(long *)&from);
										while (*pt>=48 && *pt<=57)
											pt++;
										if (*pt=='-')
										{
											while (*pt && (*pt<48|*pt>57))
												pt++;
											if (*pt>=48 && *pt<=57)
											{
												StrToLong(pt,(long *)&to);
												while (*pt>=48 && *pt<=57)pt++;
												if (from>to)
												{
													ulong=to;
													to=from;
													from=ulong;
												}
												if (!from)from=1;
												for (ulong=from;(ulong<=indexheader.Entrys && ulong<=to);ulong++)
												{
													if (viewtable.item[ulong-1].select)
													{
														viewtable.item[ulong-1].select=FALSE;
														ioprintf("c3item c7%d c3is now deselected\n",ulong);
													}
													else
													{
														viewtable.item[ulong-1].select=TRUE;
														ioprintf("c3item c7%d c3is now selected\n",ulong);
													}
												}
											}
											else
											{
												if (from && from<=indexheader.Entrys)
												{
													for (ulong=from-1;ulong<indexheader.Entrys;ulong++)
													{
														if (viewtable.item[ulong].select)
														{
															viewtable.item[ulong].select=FALSE;
															ioprintf("c3item c7%d c3is now deselected\n",ulong);
														}
														else
														{
															viewtable.item[ulong].select=TRUE;
															ioprintf("c3item c7%d c3is now selected\n",ulong);
														}
													}
												}
											}
										}
										else
										{
											if (from>0 && from<=indexheader.Entrys)
											{
												if (viewtable.item[from-1].select)
												{
													viewtable.item[from-1].select=FALSE;
													ioprintf("c3item c7%d c3is now deselected\n",from);
												}
												else
												{
													viewtable.item[from-1].select=TRUE;
													ioprintf("c3item c7%d c3is now selected\n",from);
												}
											}
										}
									}
									else
									{
										if (*pt=='-')
										{
											while (*pt && (*pt<48|*pt>57))
												pt++;
											if (*pt>=48 && *pt<=57)
											{
												StrToLong(pt,(long *)&to);
												if (to>indexheader.Entrys)to=indexheader.Entrys;
												for (ulong=0;ulong<to;ulong++)
												{
													if (viewtable.item[ulong].select)
													{
														viewtable.item[ulong].select=FALSE;
														ioprintf("c3item c7%d c3is now deselected\n",ulong+1);
													}
													else
													{
														viewtable.item[ulong].select=TRUE;
														ioprintf("c3item c7%d c3is now selected\n",ulong+1);
													}
												}
											}
										}
									}
									if(*pt)pt++;
								}
							}
						}
						else
						{
							if (ulong>0 && ulong<0xffffffff)
								ioprintf("\n%d: invalid Itemnumber",ulong);
							else
							{
								ioprintf("\n%s: Unknown command\n",cport->argstring[0]);
								sprintf(buffer,"%ssys.base.text",cport->sysdirs->systext);
								ShowText(buffer);LF();
							}
						}
					}
				}
			}
		}
	}
	if (indexheader.Entrys)
	{
		for (counter=0; counter<indexheader.Entrys; counter++)
		{
			if (viewtable.item[counter].ItemIP)
			{
				char q=FALSE;
				select=(SelectItem *)&cport->select_list.mlh_Head;
				while (!q && select->ln_Succ)
				{
					if (select->dirIP==currentdir->IPNumber &&
						select->itemIP==viewtable.item[counter].ItemIP)q=TRUE;
					if (!q)select=select->ln_Succ;
				}
				if (viewtable.item[counter].select)
				{
					if (!q)
					{
						if (select=AllocVec(sizeof(SelectItem),MEMF_ANY|MEMF_CLEAR))
						{
							select->dirIP=currentdir->IPNumber;
							select->itemIP=viewtable.item[counter].ItemIP;
							AddTail((List *)&cport->select_list,(Node *)select);
						}
					}
				}
				else
				{
					if (q && select)
					{
						Remove((Node *)select);
						FreeVec(select);
					}
				}
			}
		}					
	}
	while (!IsListEmpty(&dirlist))
	{
		newdir=(MyNode *)RemHead(&dirlist);
		if (newdir->ln_Name)
			FreeVec(newdir->ln_Name);
		FreeVec(newdir);
	}
	if (fib)FreeDosObject(DOS_FIB,fib);
	if (item)FreeVec(item);
	if (viewtable.item)FreeVec(viewtable.item);
	if (indexentry)FreeVec(indexentry);
	if (reply)FreeVec(reply);
}	

struct smulder {
	struct smulder *ln_Succ;
	struct smulder *ln_Pred;
	UBYTE			ln_Type;
	BYTE			ln_Pri;
	BYTE 			*ln_Name;
	BPTR			index_mulder;
	BPTR			data_mulder;
	BPTR			serial_mulder;
};

void RepairBase()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	char buffer[256],quit=FALSE;
	List dirlist;
	BPTR index_mulder=NULL,
		 data_mulder=NULL,
		 mulder=NULL,
		 lock=NULL,
		 lock2=NULL,
		 serial_mulder=NULL,
		 serial_names=NULL;
	struct Subboard *subboard;
	struct ItemHeader *itemheader;
	struct Item *item;
	struct IndexEntry *indexentry;
	struct IndexEntry *indexentry2;
	struct IndexHeader indexheader;
	struct smulder *currentdir=0;
	struct smulder *newdir=0;
	struct DirectoryIndexHeader dih;
	struct DirIndexEntry		die;
	struct ItemIndexHeader		iih;
	struct ItemIndexEntry 		iie;
	char Quit2=FALSE;
	char Quit=FALSE;
	ULONG dircounter=0;

	NewList(&dirlist);
	sprintf(buffer,"%sroot/",cport->sysdirs->sysdata);

	if (!(item=AllocVec(sizeof(Item),MEMF_ANY|MEMF_CLEAR)))Quit=TRUE;
	if (!(indexentry=AllocVec(sizeof(IndexEntry),MEMF_ANY)))Quit=TRUE;

	subboard=(Subboard *)item;
	itemheader=(ItemHeader *)item;

	if (!(lock=Lock(buffer,SHARED_LOCK)))
	{
		if (!(lock=CreateDir(buffer))) 
			Quit=TRUE;
		else
			ChangeMode(CHANGE_LOCK,lock,SHARED_LOCK);
	}
	if (lock)
	{
		if (NameFromLock(lock,buffer,255))
		{
			if (currentdir=AllocVec(sizeof(smulder),MEMF_ANY|MEMF_CLEAR))
			{
				if (currentdir->ln_Name=AllocVec(strlen(buffer)+1,MEMF_ANY))
				{
					strcpy(currentdir->ln_Name,buffer);
					AddTail(&dirlist,(Node *)currentdir);
					sprintf(buffer,"%s/data/.index",currentdir->ln_Name);
					if (!(index_mulder=Open(buffer,MODE_READWRITE)))
					{
						if (lock2=CreateDir(buffer))
						{
							UnLock(lock2);
							if (!(index_mulder=Open(buffer,MODE_READWRITE)))
							{
								Quit=TRUE;
								ioprintf("can't create root.index.\n");
							}
						}
						else
						{
							ioprintf("can't create root/data directory.\n");
							Quit=TRUE;
						}
					}
					sprintf(buffer,"%ssys.dirtable.index",cport->sysdirs->sysdata);
					if (serial_mulder=Open(buffer,MODE_READWRITE))
					{
						ClearMemQuick(&dih,sizeof(DirectoryIndexHeader));
						Read(serial_mulder,&dih,sizeof(DirectoryIndexHeader));
						Seek(serial_mulder,0,OFFSET_BEGINNING);
						Write(serial_mulder,&dih,sizeof(DirectoryIndexHeader));
					}
					else
						Quit=TRUE;
					sprintf(buffer,"%ssys.dirtable.names",cport->sysdirs->sysdata);
					if (serial_names=Open(buffer,MODE_READWRITE))
						Seek(serial_names,0,OFFSET_END);
					else
						Quit=TRUE;
				}
				else
				{
					FreeVec(currentdir);
					Quit=TRUE;
				}
			}
		}
		UnLock(lock);
		if (index_mulder)
		{
			Seek(index_mulder,0,OFFSET_END);
			if (Seek(index_mulder,0,OFFSET_BEGINNING)>=sizeof(IndexHeader))
			{
				Read(index_mulder,&indexheader,sizeof(IndexHeader));
			}
			else
			{
				ClearMemQuick(&indexheader,sizeof(IndexHeader));
				Write(index_mulder,&indexheader.Entrys,4);
				ioprintf("index mulder is empty\n");
			}
			Close(index_mulder);
		}
		else
		{
			Quit=TRUE;
			ioprintf("can't open %s\n",buffer);
		}
		currentdir->index_mulder=0;
		currentdir->data_mulder=0;
	}
	while (!Quit && !cport->ProgramClose && cport->network.online)
	{
		ClearMemQuick(&indexheader,sizeof(IndexHeader));
		if (!currentdir->index_mulder)
		{
			sprintf(buffer,"%s/data/.index",currentdir->ln_Name);
			ioprintf("index open: %s\n",buffer);
			currentdir->index_mulder=Open(buffer,MODE_READWRITE);
		}
		if (currentdir->index_mulder)
		{
				if (!currentdir->data_mulder)
				{
					Read(currentdir->index_mulder,&indexheader,sizeof(IndexHeader));
					sprintf(buffer,"%s/data/.data",currentdir->ln_Name);
					currentdir->data_mulder=Open(buffer,MODE_READWRITE);
					ioprintf("data open: %s\n",buffer);
				}
				if (currentdir->data_mulder)
				{
					Quit2=FALSE;
					while (!Quit2)
					{
						if (Read(currentdir->index_mulder,indexentry,sizeof(IndexEntry)))
						{
							if (Seek(currentdir->data_mulder,indexentry->offset,OFFSET_BEGINNING)>-1)
							{
								if (indexentry->type==ITEMTYPE_SUBBOARD)
								{
									if (Read(currentdir->data_mulder,itemheader,sizeof(Subboard))==sizeof(Subboard))
									{
										UWORD len=strlen(currentdir->ln_Name)+1;
										Seek(serial_names,0,OFFSET_END);
										die.seek=Seek(serial_names,0,OFFSET_END);
										Write(serial_names,&len,sizeof(UWORD));
										Write(serial_names,currentdir->ln_Name,len);

										Seek(serial_mulder,0,OFFSET_END);
										
										die.IPNumber64=0;
										die.IPNumber=dih.counter+1;
										die.dataseek=indexentry->offset;
										die.checksum=die.IPNumber+die.IPNumber64+die.seek+die.dataseek;
										Write(serial_mulder,&die,sizeof(DirIndexEntry));

										dih.counter++;
										dih.entrys++;

										itemheader->IPNumber=die.IPNumber;

										Seek(currentdir->data_mulder,indexentry->offset,OFFSET_BEGINNING);
										Write(currentdir->data_mulder,itemheader,sizeof(Subboard));
																				
										ioprintf("dir: %s\n",itemheader->Title);
										if (newdir=AllocVec(sizeof(smulder),MEMF_ANY|MEMF_CLEAR))
										{
											if (newdir->ln_Name=AllocVec(strlen(itemheader->Path)+1,MEMF_ANY|MEMF_CLEAR))
											{
												strcpy(newdir->ln_Name,itemheader->Path);
												sprintf(buffer,"%s/data/.index",itemheader->Path);
												if (mulder=Open(buffer,MODE_OLDFILE))
												{
													Close(mulder);
													sprintf(buffer,"%s/data/.data",itemheader->Path);
													if (mulder=Open(buffer,MODE_OLDFILE))
													{
														Close(mulder);
														AddTail(&dirlist,(Node *)newdir);
														currentdir=newdir;
														Quit2=TRUE;
													}
												}
												if (!Quit2)
												{
													ioprintf("error: %s\n",buffer);
													FreeVec(newdir->ln_Name);
													FreeVec(newdir);
												}
											}
											else
											{
												ioprintf("Can't change directory, out of memory\n");
												FreeVec(newdir);
											}
										}

									}
								}
								else
								{
									if (Read(currentdir->data_mulder,itemheader,sizeof(Item))==sizeof(Item))
									{
										BPTR itemserial_mulder;
										sprintf(buffer,"%s/data/.item.serials",currentdir->ln_Name);
										if (itemserial_mulder=Open(buffer,MODE_READWRITE))
										{
											if (Read(itemserial_mulder,&iih,sizeof(ItemIndexHeader))<sizeof(ItemIndexHeader))
											{
												ClearMemQuick(&iih,sizeof(ItemIndexHeader));
												Seek(itemserial_mulder,0,OFFSET_BEGINNING);
												Write(itemserial_mulder,&iih,sizeof(ItemIndexHeader));
											}
											Seek(itemserial_mulder,0,OFFSET_END);
											iie.IPNumber64=0;
											iie.IPNumber=iih.counter+1;
											iie.seek=indexentry->offset;
											iie.checksum=iie.IPNumber+iie.IPNumber64+iie.seek;
											iih.version=NOVIA_SUBBOARD_VER;
											iih.counter++;
											iih.entrys++;
											itemheader->IPNumber=iie.IPNumber;
											item->DirIP=die.IPNumber;

											Write(itemserial_mulder,&iie,sizeof(ItemIndexEntry));
											Seek(itemserial_mulder,0,OFFSET_BEGINNING);
											Write(itemserial_mulder,&iih,sizeof(ItemIndexHeader));
											Close(itemserial_mulder);
	
											Seek(currentdir->data_mulder,indexentry->offset,OFFSET_BEGINNING);
											Write(currentdir->data_mulder,itemheader,sizeof(Item));
	
										}
										else
											ioprintf("can't open %s\n",buffer);
										ioprintf("item: %s\n",itemheader->Title);
									}
								}
							}
						}
						else
						{
							if (currentdir->ln_Pred->ln_Pred)
							{
								newdir=currentdir;
								currentdir=currentdir->ln_Pred;
								Remove((Node *)newdir);
								if (newdir->ln_Name)FreeVec(newdir->ln_Name);
								if (newdir->index_mulder)Close(newdir->index_mulder);
								if (newdir->data_mulder)Close(newdir->data_mulder);
								FreeVec(newdir);
								ioprintf("Q2\n");
							}
							else
							{
								if (dircounter==0)Quit=TRUE;
								Quit2=TRUE;
							}
						}
					}
				}
		}
		else
		{
			if (currentdir->ln_Pred->ln_Pred)
			{
				newdir=currentdir;
				currentdir=currentdir->ln_Pred;
				Remove((Node *)newdir);
				if (newdir->ln_Name)FreeVec(newdir->ln_Name);
				if (newdir->index_mulder)Close(newdir->index_mulder);
				if (newdir->data_mulder)Close(newdir->data_mulder);
				FreeVec(newdir);
				dircounter--;
				ioprintf("Q4\n");
			}
			else
			{
				ioprintf("E4\n");
				Quit=TRUE;
			}
		}

		if (IsListEmpty(&dirlist))Quit=TRUE;
	}
	if (item)FreeVec(item);
	if (indexentry)FreeVec(indexentry);
	if (serial_mulder)
	{
		Seek(serial_mulder,0,OFFSET_BEGINNING);
		Write(serial_mulder,&dih,sizeof(DirectoryIndexHeader));
		Close(serial_mulder);
	}
	if (serial_names)
		Close(serial_names);
		
}


struct Subboard *GetSubboard_byIP(ULONG serialIP,struct Subboard *subboard,ULONG size)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	BPTR index_mulder,names_mulder;
	char buffer[256];
	struct Subboard *result=0;
	sprintf(buffer,"%ssys.dirtable.index",cport->sysdirs->sysdata);
	if (index_mulder=Open(buffer,MODE_OLDFILE))
	{
		sprintf(buffer,"%ssys.dirtable.names",cport->sysdirs->sysdata);
		if (names_mulder=Open(buffer,MODE_OLDFILE))
		{
			ULONG ulong;
			struct DirectoryIndexHeader *dih;
			struct DirIndexEntry  *die;
			char *names;
			BPTR mulder;
			char *pt;
			ULONG datasize;
			Seek(index_mulder,0,OFFSET_END);
			datasize=Seek(index_mulder,0,OFFSET_BEGINNING);

			Seek(names_mulder,0,OFFSET_END);
			ulong=Seek(names_mulder,0,OFFSET_BEGINNING);

			names=AllocVec(ulong,MEMF_ANY);
			Read(names_mulder,names,ulong);
			if (pt=AllocVec(datasize,MEMF_ANY))
			{
				ULONG counter;

				Read(index_mulder,pt,datasize);

				dih=(DirectoryIndexHeader *)pt;
				die=(DirIndexEntry *)&pt[sizeof(DirectoryIndexHeader)];

				for (counter=0;((counter<dih->entrys) && (!(die[counter].IPNumber==serialIP)));counter++);
				if (die[counter].IPNumber==serialIP)
				{
					sprintf(buffer,"%s/data/.data",&names[die[counter].seek+2]);
					if (mulder=Open(buffer,MODE_OLDFILE))
					{
						if (Seek(mulder,die[counter].dataseek,OFFSET_BEGINNING)!=ENDSTREAMCH)
							if (Read(mulder,subboard,size))
								result=subboard;
						Close(mulder);
					}
				}
				FreeVec(pt);
			}
			FreeVec(names);
			Close(names_mulder);
		}
		Close(index_mulder);
	}
	return result;
}

struct Item *GetItem_byIP(ULONG dirIP,ULONG itemIP,struct Item *item,ULONG size)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	BPTR index_mulder,names_mulder;
	char buffer[200];
	struct ItemHeader itemheader;
	struct Item *result=0;
	sprintf(buffer,"%ssys.dirtable.index",cport->sysdirs->sysdata);
	if (index_mulder=Open(buffer,MODE_OLDFILE))
	{
		sprintf(buffer,"%ssys.dirtable.names",cport->sysdirs->sysdata);
		if (names_mulder=Open(buffer,MODE_OLDFILE))
		{
			ULONG ulong;
			struct DirectoryIndexHeader *dih;
			struct DirIndexEntry  *die;
			char *names;
			BPTR mulder;
			char *pt;
			ULONG datasize;
			Seek(index_mulder,0,OFFSET_END);
			datasize=Seek(index_mulder,0,OFFSET_BEGINNING);
			Seek(names_mulder,0,OFFSET_END);
			ulong=Seek(names_mulder,0,OFFSET_BEGINNING);
			names=AllocVec(ulong,MEMF_ANY);
			Read(names_mulder,names,ulong);
			if (pt=AllocVec(datasize,MEMF_ANY))
			{
				ULONG counter;
				Read(index_mulder,pt,datasize);
				dih=(DirectoryIndexHeader *)pt;
				die=(DirIndexEntry *)&pt[sizeof(DirectoryIndexHeader)];
				for (counter=0;((counter<dih->entrys) && (!(die[counter].IPNumber==dirIP)));counter++);
				if (die[counter].IPNumber==dirIP)
				{
					sprintf(buffer,"%s/data/.data",&names[die[counter].seek+2]);
					if (mulder=Open(buffer,MODE_OLDFILE))
					{
						if (Seek(mulder,die[counter].dataseek,OFFSET_BEGINNING)!=ENDSTREAMCH)
						{
							if (Read(mulder,&itemheader,sizeof(ItemHeader)))
							{
								BPTR serial_mulder,item_mulder;
								sprintf(buffer,"%s/data/.item.serials",itemheader.Path);
								if (serial_mulder=Open(buffer,MODE_OLDFILE))
								{
									sprintf(buffer,"%s/data/.data",itemheader.Path);
									if (item_mulder=Open(buffer,MODE_OLDFILE))
									{
										char *pt2;
										Seek(serial_mulder,0,OFFSET_END);
										ulong=Seek(serial_mulder,0,OFFSET_BEGINNING);
										if (pt2=AllocVec(ulong,MEMF_ANY))
										{
											if (Read(serial_mulder,pt2,ulong)!=ENDSTREAMCH)
											{
												ItemIndexHeader *iih=(ItemIndexHeader *)pt2;
												ItemIndexEntry *iie=(ItemIndexEntry *)iih+1;
												for (counter=0;((counter<iih->entrys) && (!(iie[counter].IPNumber==itemIP)));counter++);
													if (iie[counter].IPNumber==itemIP)
														if (Seek(item_mulder,iie[counter].seek,OFFSET_BEGINNING)!=ENDSTREAMCH)
															if (Read(item_mulder,item,size))
																result=item;
													
											}
											FreeVec(pt2);
										}	
										result=item;
										Close(item_mulder);
									}
									Close(serial_mulder);
								}
							}
						}
						Close(mulder);
					}
				}
				FreeVec(pt);
			}
			FreeVec(names);
			Close(names_mulder);
		}
		Close(index_mulder);
	}
	return result;
}

