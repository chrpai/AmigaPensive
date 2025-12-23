
void UpdateBaselist(char *filename)
{
	char Quit=FALSE,Quit2=FALSE,habschon;
	char path[256];
	BPTR index_mulder=NULL,
		 data_mulder=NULL;
	ULONG	index_size,
			ulong,
			counter;
	struct ItemHeader *itemheader;
	struct IndexEntry indexentry;
	struct IndexHeader indexheader;

	ClearMemQuick(&indexheader,sizeof(IndexHeader));
	ClearMemQuick(&indexentry,sizeof(IndexEntry));

	itemcounter=0;
	dircounter=0;

	if ((currentdir=AllocVec(sizeof(MyNode),MEMF_ANY|MEMF_CLEAR)))
	{
		if ((currentdir->ln_Name=AllocVec(strlen(filename)+1,MEMF_ANY|MEMF_CLEAR)))
		{
			strcpy(currentdir->ln_Name,filename);
			AddTail(&dirlist,(Node *)currentdir);

			NewList(&itemlist);

			strcpy(path,filename);
			strcat(path,"/data/.index");
			if (index_mulder=Open(path,MODE_READWRITE))
			{
				Seek(index_mulder,0,OFFSET_BEGINNING);
				Read(index_mulder,&indexheader,sizeof(IndexHeader));
			}
			else
			{
				printf("can't open %s.(%s)\n",path,filename);
			}
			if (indexheader.Entrys)
			{
				strcpy(path,currentdir->ln_Name);
				strcat(path,"/data/.data");
				data_mulder=Open(path,MODE_READWRITE);
				Quit2=FALSE;
				counter=0;
				while (!Quit2)
				{
					if (Read(index_mulder,&indexentry,sizeof(IndexEntry))<sizeof(IndexEntry))Quit2=TRUE;
					if (!Quit2)
					{
						if (indexentry.type==ITEMTYPE_ITEM)
						{
							index_size=sizeof(Item);
						}
						else
							index_size=sizeof(ItemHeader);
						if ((itemheader=AllocVec(index_size,MEMF_ANY)))
						{
							if (Seek(data_mulder,indexentry.offset,OFFSET_BEGINNING)>-1)
							{
								if (indexentry.type==ITEMTYPE_SUBBOARD)
								{
									if (Read(data_mulder,itemheader,index_size)==index_size)
									{
										dircounter++;
									}
									else
									{
										Quit2=TRUE;
									}
								}
								else
								{
									if (Read(data_mulder,itemheader,index_size)==index_size)
									{
										itemcounter++;
									}
									else
									{
										Quit2=TRUE;
									}
								}
								AddTail(&itemlist,(Node *)itemheader);
								DoMethod(App->LV_SS_BASEVIEW, MUIM_List_Insert, &itemheader, 1, MUIV_List_Insert_Bottom);
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
		}
	}

	sprintf(Baseviewstring,"\33I5:yam:icons/status_send\33bDirectorys: \33n%d	\33bItems: \33n%d",dircounter,itemcounter);
	set(App->TX_SS_BASEVIEW,MUIA_Text_Contents,Baseviewstring);

	if (data_mulder)Close(data_mulder);
	if (index_mulder)Close(index_mulder);
}








/*					strcpy(path,currentdir->ln_Name);
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
*/






void ParrentBaselist()
{
	char Quit=FALSE,Quit2=FALSE,habschon;
	char path[256];
	BPTR index_mulder=NULL,
		 data_mulder=NULL;
	ULONG	index_size,
			ulong,
			counter;
	struct ItemHeader *itemheader;
	struct IndexEntry indexentry;
	struct IndexHeader indexheader;

	ClearMemQuick(&indexheader,sizeof(IndexHeader));
	ClearMemQuick(&indexentry,sizeof(IndexEntry));

	itemcounter=0;
	dircounter=0;

	if (currentdir->ln_Pred->ln_Pred)
	{
		struct MyNode *parrentdir=currentdir->ln_Pred;
		if (currentdir->ln_Name)
			FreeVec(currentdir->ln_Name);
		Remove((Node *)currentdir);
		FreeVec(currentdir);
		currentdir=parrentdir;
		DoMethod(App->LV_SS_BASEVIEW, MUIM_List_Clear);

		NewList(&itemlist);

		strcpy(path,currentdir->ln_Name);
		strcat(path,"/data/.index");

		if (index_mulder=Open(path,MODE_READWRITE))
		{
			Seek(index_mulder,0,OFFSET_BEGINNING);
			Read(index_mulder,&indexheader,sizeof(IndexHeader));
		}
		else
		{
			printf("can't open %s.\n",path);
		}
		if (indexheader.Entrys)
		{
			strcpy(path,currentdir->ln_Name);
			strcat(path,"/data/.data");
			data_mulder=Open(path,MODE_READWRITE);
			Quit2=FALSE;
			counter=0;
			while (!Quit2)
			{
				if (Read(index_mulder,&indexentry,sizeof(IndexEntry))<sizeof(IndexEntry))Quit2=TRUE;
				if (!Quit2)
				{
					if (indexentry.type==ITEMTYPE_ITEM)
					{
						index_size=sizeof(Item);
					}
					else
						index_size=sizeof(ItemHeader);
					if ((itemheader=AllocVec(index_size,MEMF_ANY)))
					{
						if (Seek(data_mulder,indexentry.offset,OFFSET_BEGINNING)>-1)
						{
							if (indexentry.type==ITEMTYPE_SUBBOARD)
							{
								if (Read(data_mulder,itemheader,index_size)==index_size)
								{
									dircounter++;
								}
								else
								{
									Quit2=TRUE;
								}
							}
							else
							{
								if (Read(data_mulder,itemheader,index_size)==index_size)
								{
									itemcounter++;
								}
								else
								{
									Quit2=TRUE;
								}
							}
							AddTail(&itemlist,(Node *)itemheader);
							DoMethod(App->LV_SS_BASEVIEW, MUIM_List_Insert, &itemheader, 1, MUIV_List_Insert_Bottom);
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
		if (index_mulder)Close(index_mulder);
	}
	sprintf(Baseviewstring,"\33bDirectorys: \33n%d	\33bDirectorys: \33n%d",dircounter,itemcounter);
	set(App->TX_SS_BASEVIEW,MUIA_Text_Contents,Baseviewstring);
}




void Read_Subboard(ItemHeader *header)
{
	char	readnew=TRUE,quit=FALSE;
	char	path[256];
	char	buffer[256];
	BPTR	text_mulder=NULL;
	ULONG	index_size,
			data_size,
			ulong,
			counter;
	struct ItemHeader *itemheader;
	struct IndexEntry indexentry;
	struct Reply reply;
	char	*text=NULL;

	BOOL	running = TRUE;
	ULONG	signal;
	ULONG	result=0;

	ULONG replypos=0;

	struct Item *item;

	struct BaseViewApp *baseviewapp=NULL;

	itemheader=header;
	printf("Title   : %s\n",header->Title);
	printf("IDNumber: 0x%x\n",header->IDNumber);
	printf("IPNumber: 0x%x\n",header->IPNumber);
	if ((baseviewapp=CreateBaseViewApp(text)))
	{
		while (!quit)
		{
			if (readnew)
			{
				if (text)FreeVec(text);text=NULL;
				set(baseviewapp->MAILTEXT,MUIA_Mailtext_Text,NULL);
				DoMethod(baseviewapp->LV_BASEVIEW_HEADER, MUIM_List_Clear);

				item=(Item *)itemheader;

				sprintf(path,"%s/data/%s.text",currentdir->ln_Name,itemheader->Path);
				if ((text_mulder=Open(path,MODE_READWRITE)))
				{
					if (replypos==0)
					{
						if ((text=AllocVec(item->length+1,MEMF_ANY|MEMF_CLEAR)))
						{
							Seek(text_mulder,item->seek,OFFSET_BEGINNING);
							if (Read(text_mulder,text,item->length)==item->length)
							{
								sprintf(buffer,"%d of %d (%d replys)",itemheader->IDNumber,itemcounter,item->replys);
								InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Item:",buffer);
								InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Subject:",itemheader->Title);
								sprintf(buffer,"%s (%s)",item->From,item->FromRealName);
								InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"From:",buffer);
								sprintf(buffer,"%s (%s)",item->To,item->ToRealName);
								InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"To:",buffer);
								DateToString(buffer,&itemheader->CreateDate,0);
								InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Date:",buffer);
								if (item->SendDate.year)
								{
									DateToString(buffer,&item->SendDate,0);
									InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Receive:",buffer);
								}
								sprintf(buffer,"%05d.%05d.00001\n",item->DirIP,itemheader->IPNumber);
								InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Message-ID:",buffer);
								set(baseviewapp->MAILTEXT,MUIA_Mailtext_Text,text);
							}
						}
					}
					else
					{
						BPTR reply_mulder;

						sprintf(path,"%s/data/%s.replys",currentdir->ln_Name,itemheader->Path);

						if ((reply_mulder=Open(path,MODE_OLDFILE)))
						{
							Seek(reply_mulder,(replypos-1)*sizeof(Reply),OFFSET_BEGINNING);
							if (Read(reply_mulder,&reply,sizeof(Reply))==sizeof(Reply))
							{
								if ((text=AllocVec(reply.length+1,MEMF_ANY|MEMF_CLEAR)))
								{
									Seek(text_mulder,reply.seek,OFFSET_BEGINNING);
									if (Read(text_mulder,text,reply.length)==reply.length)
									{
										sprintf(buffer,"%d of %d",replypos,item->replys);
										InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Reply:",buffer);
										InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Subject:",itemheader->Title);
										sprintf(buffer,"%s (%s)",reply.From,reply.FromRealName);
										InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"From:",buffer);
										sprintf(buffer,"%s (%s)",reply.To,reply.ToRealName);
										InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"To:",buffer);
										DateToString(buffer,&reply.CreateDate,0);
										InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Date:",buffer);
										{
											DateToString(buffer,&reply.SendDate,0);
											InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Receive:",buffer);
										}
										sprintf(buffer,"%05d.%05d.%05d\n",item->DirIP,itemheader->IPNumber,replypos+1);
										InsertMessageHeader(baseviewapp->LV_BASEVIEW_HEADER,"Message-ID:",buffer);
										set(baseviewapp->MAILTEXT,MUIA_Mailtext_Text,text);
									}
								}
							}
							Close(reply_mulder);
						}
					}

					Close(text_mulder);
				}

				readnew=FALSE;
			}
			while (running && !quit && !readnew)
			{
				if ((result=DoMethod(baseviewapp->App,MUIM_Application_Input,&signal)))
				{
					printf("result: %d\n",result);
					switch (result)
					{
						case MUIV_Application_ReturnID_Quit:
							running=FALSE;
							quit=TRUE;
							break;
						case GUIID_BASEVIEWER_EDIT:
							break;
						case GUIID_BASEVIEWER_NEW:
							break;
						case GUIID_BASEVIEWER_POST_NEXT:
							if (itemheader->ln_Succ->ln_Succ)
							{
								replypos=0;
								itemheader=itemheader->ln_Succ;
								readnew=TRUE;
							}
							break;
						case GUIID_BASEVIEWER_POST_PREV:
							if (itemheader->ln_Pred->ln_Pred)
							{
								replypos=0;
								itemheader=itemheader->ln_Pred;
								readnew=TRUE;
							}
							break;
						case GUIID_BASEVIEWER_REPLY_REPLY:
							if (replypos>0)
							{
								replypos--;
								readnew=TRUE;
							}
							else
							{
								if (itemheader->ln_Pred->ln_Pred)
								{
									itemheader=itemheader->ln_Pred;
									replypos=((Item *)itemheader)->replys;
									readnew=TRUE;
								}								
							}
							break;
						case GUIID_BASEVIEWER_REPLY_NEXT:
							if (replypos<item->replys)
							{
								replypos++;
								readnew=TRUE;
							}
							else
							{
								if (itemheader->ln_Succ->ln_Succ)
								{
									itemheader=itemheader->ln_Succ;
									replypos=0;
									readnew=TRUE;
								}
							}
							break;
						case GUIID_BASEVIEWER_REPLY_REMOVE:
							break;
						case GUIID_BASEVIEWER_POST_REMOVE:
							break;
					}
				}
				if(running && signal) Wait(signal);
			}
		}
		if (baseviewapp)DisposeBaseViewApp(baseviewapp);
		if (text)FreeVec(text);
	}
}




