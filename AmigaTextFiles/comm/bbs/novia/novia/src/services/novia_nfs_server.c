#include <exec/types.h>
#include <stdlib.h>
#include <intuition/screens.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <novia/novia_UserList.h>
#include <novia/novia_PortData.h>
#include <novia/novia_config.h>
#include <novia/novia_mail.h>
#include <novia/novia_types.h>
#include <novia/novia_misc.h>
#include <novia/novia_message.h>
#include <novia/novia_dos.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <pragma/noviasys_lib.h>
#include <exec/memory.h>
#include <dos/stdio.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <novia/novia_funcs.h>
#include <novia/novia_registers.h>
//#include "/protos/novia_dos_protos.h"
#include <pragma/noviasys_lib.h>
#include <exec/semaphores.h>
#include <utility/tagitem.h>
#include <dos/dostags.h>

extern struct MainPortConfig *mainportconfig;
extern Library *NoviaSysBase;
extern struct NFSServer *nfsserver;
extern struct List *client_list;
extern struct Task *MainTask;
extern struct SignalSemaphore *rt_semaphore;

/*** PROTOTYPES ***/

Directory * nfs_support_mkdir(char *name, PortData *cport);
LONG nfs_support_closeroot( PortData *cport);
LONG nfs_support_openroot( PortData *cport);
LONG nfs_support_parrentdir( PortData *cport);
LONG nfs_support_changedir( char *name,  PortData *cport);
LONG nfs_support_delete( char *name,  PortData *cport);
LONG nfs_support_addmail( struct NewItem *item,  PortData *cport);
LONG nfs_support_addreply( char *itemname,  char *replyname, PortData *cport);
LONG nfs_support_loadselectlist( List *select_list,  PortData *cport);
LONG nfs_support_saveselectlist(List *select_list,  PortData *cport);
LONG nfs_support_getlist( PortData *cport);
LONG nfs_support_additementry( struct ItemEntry *itementry,  PortData *cport);
LONG nfs_support_additementry2dir(struct Directory *dir,			struct ItemEntry *itementry,  PortData *cport);
LONG nfs_support_addmail2dir(Directory *dir,	struct NewItem *item, PortData *cport);
LONG nfs_support_closedir(Directory *dir, PortData *cport);
LONG nfs_support_sendmail(NewMail *newmail, PortData *cport);
LONG nfs_support_rename(char *oldname, char *newname, PortData *cport);
LONG nfs_support_uucp_import_mail(char *filename, struct PortData *cport);
struct ItemEntryBlock *nfs_support_get_ieb(char *name, PortData *cport);
LONG nfs_support_lowlevel_makefilesystem(struct PortData *cport);
LONG nfs_support_lowlevel_makeroot(struct PortData *cport);
LONG nfs_support_lowlevel_adddirectory(char *name, struct PortData *cport);
LONG nfs_support_lowlevel_addlink(char *name, struct PortData *cport);
LONG nfs_support_lowlevel_addcommand(char *name, struct PortData *cport);


/* tools */


extern int mystrcmp(char *str1, char *str2);
void getuser_from_host(char *src, char *username);
struct INet_User_Address *Create_INet_User(char *buffer);

extern char uucp_char[];

/* perform a NFSPacket and wait for completion */

void SetIPCounter64(ULONG *ptr);

void nfs_main()
{
	if ((nfsserver = AllocVec(sizeof(NFSServer),MEMF_ANY|MEMF_CLEAR)))
	{
		ULONG sigs;
		ULONG sb_nfs_port;
		ULONG	sb_controlport;
		BOOL quit = FALSE;
		struct NFSPacket *packet;

		/* Init Task */

		nfsserver->nfs_port		= CreateMsgPort();
		nfsserver->controlport	= CreateMsgPort();

		if (nfsserver->nfs_port && nfsserver->controlport)
		{
			sb_nfs_port		=	1 << nfsserver->nfs_port->mp_SigBit;
			sb_controlport	=	1 << nfsserver->controlport->mp_SigBit;
			nfsserver->nfstask		= FindTask(NULL);
			nfsserver->CloseServer	= FALSE;
			nfsserver->ServerOpen	= TRUE;
		}
		else
			nfsserver->CloseServer = TRUE;

		/* Wait for NFS-Packets */

		while (!nfsserver->CloseServer && nfsserver->ServerOpen)
		{
			sigs = Wait(sb_nfs_port);
			if ((sb_nfs_port & sigs))
			{
				while ((packet = (NFSPacket *)GetMsg(nfsserver->nfs_port)))
				{
					if (packet->nfs_msg.mn_ReplyPort == nfsserver->nfs_port)
					{
						if (packet->nfs_result1 == RESULT_NFS_TEXTNOTIFY)
							if (packet->nfs_result2)
								FreeVec(packet->nfs_result2);
						FreeVec(packet);
					}
					else
					{
						switch (packet->nfs_type)
						{
							case ACTION_NFS_CLOSEROOT:
								packet->nfs_result1 = nfs_support_closeroot((PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_OPENROOT:
								packet->nfs_result1 = nfs_support_openroot((PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_MAKEDIR:
								packet->nfs_result1 = (LONG)nfs_support_mkdir((char *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_PARENTDIR:
								packet->nfs_result1 = nfs_support_parrentdir((PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_CHANGEDIR:
								packet->nfs_result1 = nfs_support_changedir((char *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_DELETE:
								packet->nfs_result1 = nfs_support_delete((char *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_ADDMAIL:
								packet->nfs_result1 = nfs_support_addmail((NewItem *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_ADDREPLY:
								packet->nfs_result1 = nfs_support_addreply((char *)packet->nfs_arg1, (char *)packet->nfs_arg2, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_LOADSELECTLIST:
								packet->nfs_result1 = nfs_support_loadselectlist((List *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_SAVESELECTLIST:
								packet->nfs_result1 = nfs_support_saveselectlist((List *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_GETLIST:
								packet->nfs_result1 = nfs_support_getlist((PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_ADDITEMENTRY:
								packet->nfs_result1 = nfs_support_additementry((ItemEntry *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_ADDITEMENTRY2DIR:
								packet->nfs_result1 = nfs_support_additementry2dir((Directory *)packet->nfs_arg1, (ItemEntry *)packet->nfs_arg2,(PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_ADDMAIL2DIR:
								packet->nfs_result1 = nfs_support_addmail2dir((Directory *)packet->nfs_arg1, (NewItem *)packet->nfs_arg2,(PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_CLOSEDIR:
								packet->nfs_result1 = nfs_support_closedir((Directory *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_SENDMAIL:
								packet->nfs_result1 = nfs_support_sendmail((NewMail *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_UUCP_IMPORT_MAIL:
								packet->nfs_result1 = nfs_support_uucp_import_mail((char *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_RENAME:
								packet->nfs_result1 = nfs_support_rename((char *)packet->nfs_arg1, (char *)packet->nfs_arg2, (PortData *)packet->nfs_cport);
								break;
							case ACTION_NFS_GET_IEB:
								packet->nfs_result1 = (LONG)nfs_support_get_ieb((char *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_LOWLEVEL_MAKE_FILESYSTEM:
								packet->nfs_result1 = (LONG)nfs_support_lowlevel_makefilesystem((PortData *)packet->nfs_cport);
								break;
							case ACTION_LOWLEVEL_MAKE_ROOT:
								packet->nfs_result1 = (LONG)nfs_support_lowlevel_makeroot((PortData *)packet->nfs_cport);
								break;
							case ACTION_LOWLEVEL_ADD_DIRECTORY:
								packet->nfs_result1 = (LONG)nfs_support_lowlevel_adddirectory((char *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_LOWLEVEL_ADD_LINK:
								packet->nfs_result1 = (LONG)nfs_support_lowlevel_addlink((char *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
							case ACTION_LOWLEVEL_ADD_COMMAND:
								packet->nfs_result1 = (LONG)nfs_support_lowlevel_addcommand((char *)packet->nfs_arg1, (PortData *)packet->nfs_cport);
								break;
						}
						ReplyMsg((Message *)packet);
					}
				}
			}
		}

		/* remove resources */

		nfsserver->ServerOpen	= FALSE;
		nfsserver->CloseServer	= FALSE;

		if (nfsserver->nfs_port)		DeleteMsgPort((MsgPort *)nfsserver->nfs_port);
		if (nfsserver->controlport)DeleteMsgPort((MsgPort *)nfsserver->controlport);
		FreeVec(nfsserver);
		nfsserver = NULL;
	}
}




Directory * nfs_support_mkdir(char *name, PortData *cport)
{
	LONG   error  = FALSE;
	char  *buffer = NULL;
	struct Directory *dir = NULL;
	if (*name)
	{
		char *np = name;
		char *fullpath = cport->currentdir->iib.fullpath;
		ULONG length_fp = strlen(fullpath);
		while (*np)
		{
			if ((*np == '/') | (*np == ':'))
				*np = '_';
			np++;
		}
		if ((buffer = AllocVec(strlen(name) + length_fp + 10,MEMF_ANY)))	// for full path
		{
			printf("fullpath : <%s>\n",fullpath);
			printf("length_fp : <%d>\n",length_fp);
			if (fullpath)
			{
				if (length_fp)
				{
					BPTR dirlock;
					if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
						sprintf(buffer,"%s%s",fullpath,name);
					else
						sprintf(buffer,"%s/%s",fullpath,name);
					if ((dirlock = CreateDir(buffer)))
						UnLock(dirlock);
					else
					{
						printf("io errpr: %d <%s>\n",IoErr(),buffer);
						if (IoErr() != ERROR_OBJECT_EXISTS)
							error = ERROR_OBJECT_EXISTS;

					}
				}
				else
					error = TRUE;
			}
			else
				error = TRUE;

			if (!error)
			{
				if ((dir = AllocVec(sizeof(Directory),MEMF_ANY|MEMF_CLEAR)))
				{
					BPTR						fh			= NULL;							// filehandle for <name>.dir file
					if (name[strlen(name)-1] == '/')								// Remove '/' from dirname
						name[strlen(name)] == 0;

					strncpy(dir->iib.Title,		name, sizeof(dir->iib.Title) - 1);// copy subdir name into title
					strncpy(dir->iib.Filename,	name, sizeof(dir->iib.Filename) - 1);

					dir->iib.Protection = (FIBF_READ | FIBF_WRITE | FIBF_EXECUTE | FIBF_DELETE);

					dir->iib.ItemType=ITEMTYPE_SUBBOARD;						// set ItemType
					SetIPCounter64(&dir->iib.IPNumber64);
					dir->fh_messages	= NULL;
					dir->fh_files		= NULL;
					dir->fh_dirs		= NULL;
					dir->fh_text		= NULL;
					dir->fh_replys		= NULL;
					//dir->aminetdir	= TRUE;
					GetTime(&dir->iib.LastChange);								// Get actual systemtime.
					CopyMemQuick(&dir->iib.LastChange, &dir->iib.CreateDate, sizeof(Date)); // 'nd copy date to createtime

					if (cport->currentdir->fh_dirs)
					{
						fh = cport->currentdir->fh_dirs;
					}
					else
					{

						if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
							sprintf(buffer,"%s.dirs",fullpath);
						else
							sprintf(buffer,"%s/.dirs",fullpath);

						fh = Open(buffer,MODE_READWRITE);
						cport->currentdir->fh_dirs = fh;
					}	
					if (fh)
					{
						if (Seek(fh, 0, OFFSET_END) != -1)
						{
							if (Write(fh, dir, sizeof(Directory)) == sizeof(Directory))
							{
								struct OpenEntry *entry;
								if ((entry = AllocVec(sizeof(OpenEntry),MEMF_ANY)))
								{
									if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
										sprintf(buffer,"%s%s",fullpath,name);
									else
										sprintf(buffer,"%s/%s",fullpath,name);
									dir->iib.fullpath = buffer;
									dir->iib.OpenCounter = 1;
									NewList((List *)&dir->iib.OpenList);
									NewList((List *)&dir->subdirlist);
									AddTail((List *)&cport->currentdir->subdirlist,(Node *)dir);
									dir->parrentdir = cport->currentdir;
									entry->cport = cport;
									AddTail((List *)&dir->iib.OpenList, (Node *)entry);
									error = FALSE;
								}
								else
									printf("mkdir: can't alloc mem for entry\n");

							}
							else
								printf("mkdir: error by write\n");

						}
						else
							printf("mkdir: error by seek\n");
					}
					else
						printf("mkdir: can't open filehandle\n");
				}
				else
					printf("mkdir: can't create directory: <%s>\n",buffer);
			}
			else
			{
				printf("AmigaDOS Error: %d\n",IoErr());
			}
		}
		else
			printf("mkdir: can't alloc mem for buffer\n");
	}
	else
		printf("mkdir: invalid name: \n");
	if (error)
	{
		printf("mkdir: error %d\n",IoErr());
		if (buffer)FreeVec(buffer);
		if (dir)FreeVec(dir);
		dir = NULL;
	}
	return dir;
}

LONG nfs_support_closeroot(PortData *cport)
{
	LONG result = FALSE;
	{
		while (nfs_support_parrentdir(cport));
			result = TRUE;
		cport->currentdir->iib.OpenCounter--;
		if (cport->currentdir && cport->currentdir->iib.OpenCounter <= 0)
		{
			if (cport->currentdir->fh_messages) Close(cport->currentdir->fh_messages);
			if (cport->currentdir->fh_files) 	Close(cport->currentdir->fh_files);
			if (cport->currentdir->fh_dirs)		Close(cport->currentdir->fh_dirs);
			if (cport->currentdir->fh_text)		Close(cport->currentdir->fh_text);
			if (cport->currentdir->fh_replys)	Close(cport->currentdir->fh_replys);

			if (cport->currentdir->iib.fullpath)FreeVec(cport->currentdir->iib.fullpath);

			FreeVec(cport->currentdir);

			if (cport->currentdirname)FreeVec(cport->currentdirname);
			mainportconfig->rootdir = NULL;
		}
	} /* if (!cport->currentdir) */
	return result;
};

LONG nfs_support_openroot(PortData *cport)
{
	LONG result = FALSE;
	if (!cport->currentdir)
	{
		if (mainportconfig->rootdir)
		{
			struct OpenEntry *entry;
			if ((entry = AllocVec(sizeof(OpenEntry),MEMF_ANY)))
			{
				entry->cport = cport;
				AddTail((List *)&mainportconfig->rootdir->iib.OpenList, (Node *)entry);
				cport->currentdir = mainportconfig->rootdir;
				mainportconfig->rootdir->iib.OpenCounter++;
				if ((cport->currentdirname = AllocVec(strlen(mainportconfig->rootdir->iib.fullpath)+1,MEMF_ANY)))
					strcpy(cport->currentdirname, mainportconfig->rootdir->iib.fullpath);
				result = TRUE;
				nfs_support_getlist(cport);
			}
		}
		else
		{
			char 					*root		= NULL;
			struct Directory	*rootdir	= NULL;
			BPTR					 fh		= NULL;
			if ((root = AllocVec(strlen("novia:") + 2, MEMF_ANY | MEMF_CLEAR)))
			{
				if ((rootdir = AllocVec(sizeof(Directory), MEMF_ANY | MEMF_CLEAR)))
				{
					char *buffer;
					if ((buffer = AllocVec(strlen("novia:") + 2,MEMF_ANY)))
					{
						struct OpenEntry *entry;
						strcpy(root,"novia:");
						strcpy(buffer,"novia:");
						strncpy(rootdir->iib.Title,buffer,sizeof(buffer) - 1);
		
						NewList((List *)&rootdir->iib.OpenList);
						NewList((List *)&rootdir->subdirlist);
						NewList((List *)&rootdir->entrylist);

						rootdir->iib.ItemType = ITEMTYPE_SUBBOARD;					// set ItemType

						GetTime(&rootdir->iib.LastChange);								// Get actual systemtime.
						CopyMemQuick(&rootdir->iib.LastChange, &rootdir->iib.CreateDate, sizeof(Date)); // 'nd copy date to createtime

						if ((rootdir->iib.fullpath = AllocVec(strlen(buffer) + 1, MEMF_ANY | MEMF_CLEAR)))
							sprintf(rootdir->iib.fullpath, "novia:");

						rootdir->fh_messages	= NULL;
						rootdir->fh_files		= NULL;
						rootdir->fh_dirs		= NULL;
						rootdir->fh_text		= NULL;
						rootdir->fh_replys	= NULL;
						if ((entry = AllocVec(sizeof(OpenEntry), MEMF_ANY | MEMF_CLEAR)))
						{
							entry->cport = cport;
							AddTail((List *)&rootdir->iib.OpenList, (Node *)entry);
							cport->currentdir = rootdir;
							mainportconfig->rootdir = rootdir;
							rootdir->iib.OpenCounter++;
							strncpy(rootdir->iib.Filename, buffer, sizeof(rootdir->iib.Filename) - 1);
							strncpy(rootdir->iib.Title,    buffer, sizeof(rootdir->iib.Title) - 1);
							if ((cport->currentdirname = AllocVec(strlen(rootdir->iib.fullpath) + 1, MEMF_ANY)))
								strcpy(cport->currentdirname, rootdir->iib.fullpath);
							result = TRUE;
							nfs_support_getlist(cport);
						}
					}
				}
				FreeVec(root);
			}
		}
	}
	else
	{
		while (nfs_support_parrentdir(cport));
		result = TRUE;
	} /* if (!cport->currentdir) */
	return result;
}

LONG nfs_support_parrentdir(PortData *cport)
{
	LONG result = FALSE;
	if (cport->currentdir)
	{
		if (cport->currentdir->parrentdir)
		{
			struct Directory *parrentdir = cport->currentdir->parrentdir;
			struct OpenEntry *entry;
			cport->currentdir->iib.OpenCounter--;
			if ((entry = (OpenEntry *)cport->currentdir->iib.OpenList.mlh_Head))
			{
				while (entry->ln_Succ && entry->cport != cport)
					entry = entry->ln_Succ;
				if (entry->cport == cport)
				{
					Remove((Node *)entry);
					FreeVec(entry);
				}
			}
			if (cport->currentdir->iib.OpenCounter <= 0)
			{
				struct Directory *dir = (Directory *)cport->currentdir->parrentdir->subdirlist.mlh_Head;
				while (dir->iib.ln_Succ && dir != cport->currentdir)
					dir = (Directory *)dir->iib.ln_Succ;
				if (dir == cport->currentdir)
				{
					if (dir->fh_messages)	Close(dir->fh_messages);
					if (dir->fh_files)		Close(dir->fh_files);
					if (dir->fh_dirs)			Close(dir->fh_dirs);
					if (dir->fh_text)			Close(dir->fh_text);
					if (dir->fh_replys)		Close(dir->fh_replys);
					Remove((Node *)dir);
					if (dir->iib.fullpath)
						FreeVec(dir->iib.fullpath);
					FreeVec(dir);
				}
			}
			cport->currentdir = parrentdir;
			if (cport->currentdirname)
				FreeVec(cport->currentdirname);
			if ((cport->currentdirname = AllocVec(strlen(parrentdir->iib.fullpath)+1,MEMF_ANY)))
				strcpy(cport->currentdirname, cport->currentdir->iib.fullpath);
			result = TRUE;
			nfs_support_getlist(cport);
		}
	}
	return result;
}


LONG nfs_support_changedir(char *name, PortData *cport)
{
	LONG result = FALSE;
	if (cport->currentdir)
	{
		struct Directory *founded = NULL;
		char *fullpath;
		LONG length_fp;
		struct Directory *dir = (Directory *)cport->currentdir->subdirlist.mlh_Head;
		while (dir->iib.ln_Succ && !founded)
		{
			if (dir->iib.ItemType == ITEMTYPE_SUBBOARD)
			{
				if (!Stricmp(dir->iib.Title,name))
					founded = dir;
			}
			dir = (Directory *)dir->iib.ln_Succ;
		}

		if (founded)
		{
			struct OpenEntry *entry;
			dir = founded;
			if ((entry = AllocVec(sizeof(OpenEntry),MEMF_ANY)))
			{
				char *newname;
				entry->cport = cport;
				AddTail((List *)&dir->iib.OpenList, (Node *)entry);
				cport->currentdir = dir;
				dir->iib.OpenCounter++;

				fullpath		= dir->iib.fullpath;
				length_fp	= strlen(fullpath);

				if ((newname = AllocVec(length_fp + strlen(name) + 5, MEMF_ANY)))
				{
					if (fullpath)
					{
						if (length_fp)
						{
							BPTR dirlock;
							if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
								sprintf(newname,"%s%s",fullpath,name);
							else
								sprintf(newname,"%s/%s",fullpath,name);

							if (cport->currentdirname)
								FreeVec(cport->currentdirname);

							cport->currentdirname = newname;
							result = TRUE;
							nfs_support_getlist(cport);
						}
					}
				}
			}
		}
		else
		{
			BPTR fh;
			char *file;
			BOOL failat = FALSE;
			fullpath		= cport->currentdir->iib.fullpath;
			length_fp	= strlen(fullpath);
			if ((file = AllocVec(length_fp + strlen(name) + 10,MEMF_ANY)))
			{
				if ((dir = AllocVec(sizeof(Directory), MEMF_ANY | MEMF_CLEAR)))
				{
					if ((fh = cport->currentdir->fh_dirs))
						Seek(fh, 0, OFFSET_BEGINNING);
					else
					{
						if (fullpath)
						{
							if (length_fp)
							{
								if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
									sprintf(file,"%s.dirs",fullpath);
								else
									sprintf(file,"%s/.dirs",fullpath);
								
								fh = Open(file, MODE_READWRITE);
								cport->currentdir->fh_dirs = fh;
							}
						}
					}
					if (fh)
					{
						founded = NULL;
						while (!founded && Read(fh, dir, sizeof(Directory)) == sizeof(Directory))
						{
							if (!Stricmp(dir->iib.Title,name))
								founded = dir;
						}
						if (founded)
						{
							if (dir->localdir)
							{
								struct Directory	*localdir	= NULL;
								char					*path			= NULL;
								struct OpenEntry	*entry		= NULL;
								if ((localdir = AllocVec(sizeof(Directory),MEMF_ANY|MEMF_CLEAR)))
								{
									if ((path = AllocVec(strlen(cport->sysdirs->userdir)+100,MEMF_ANY|MEMF_CLEAR)))
									{
										if ((entry = AllocVec(sizeof(OpenEntry),MEMF_ANY)))
										{
											char *newname;
											sprintf(path,"novia:home/%s",cport->LocalUser.Handle);
											localdir->iib.fullpath = path;
											localdir->iib.OpenCounter = 1;
											NewList((List *)&localdir->iib.OpenList);
											NewList((List *)&localdir->subdirlist);
											localdir->fh_messages	= NULL;
											localdir->fh_files		= NULL;
											localdir->fh_dirs			= NULL;
											localdir->fh_text			= NULL;
											localdir->fh_replys		= NULL;
											localdir->parrentdir		= cport->currentdir;
											localdir->localdir = TRUE;
											entry->cport = cport;
											AddTail((List *)&localdir->iib.OpenList, (Node *)entry);
											cport->currentdir = localdir;
											if ((newname = AllocVec(strlen("local")+20,MEMF_ANY)))
												sprintf(newname,"local");
											if (cport->currentdirname)
												FreeVec(cport->currentdirname);
											cport->currentdirname = newname;
											result = TRUE;
											nfs_support_getlist(cport);
										}
										else
											failat = TRUE;
									}
									else
										failat = TRUE;
								}
								else
									failat = TRUE;
								if (failat)
								{
									if (localdir)
									{
										FreeVec(localdir);
										if (path)
										{
											FreeVec(path);
											if (entry) FreeVec(entry);
										}
									}
								}
							}
							else
							{
								if ((dir->iib.fullpath = AllocVec(length_fp + strlen(name) + 10, MEMF_ANY | MEMF_CLEAR)))
								{
									struct OpenEntry *entry;
									if ((entry = AllocVec(sizeof(OpenEntry),MEMF_ANY)))
									{
										char *newname;

										if (fullpath)
										{
											if (length_fp)
											{
												if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
													sprintf(dir->iib.fullpath,"%s%s",fullpath,name);
												else
													sprintf(dir->iib.fullpath,"%s/%s",fullpath,name);
											}
										}

										dir->iib.OpenCounter = 1;
										NewList((List *)&dir->iib.OpenList);
										NewList((List *)&dir->subdirlist);
										dir->fh_messages	= NULL;
										dir->fh_files		= NULL;
										dir->fh_dirs		= NULL;
										dir->fh_text		= NULL;
										dir->fh_replys		= NULL;
	
										AddTail((List *)&cport->currentdir->subdirlist, (Node *)dir);
										dir->parrentdir	= cport->currentdir;
										entry->cport		= cport;
										AddTail((List *)&dir->iib.OpenList, (Node *)entry);
										cport->currentdir = dir;
										if ((newname = AllocVec(length_fp + strlen(dir->iib.Filename) + 10,MEMF_ANY)))
											strcpy(newname, dir->iib.fullpath);
										if (cport->currentdirname)
											FreeVec(cport->currentdirname);
										cport->currentdirname = newname;
										result = TRUE;
										nfs_support_getlist(cport);
									}
									else
										failat = TRUE;
								}
								else
									failat = TRUE;
							}
						}
					}
					else
						failat = TRUE;
					if (failat)
					{
						if (dir->iib.fullpath)
							FreeVec(dir->iib.fullpath);
						FreeVec(dir);
					}
				}
				FreeVec(file);
			}
		}
	}
	return result;
}

LONG __saveds ASM nfs_support_delete(char *name, PortData *cport)
{
	LONG result = FALSE;
	if (cport && cport->currentdir)
	{
		char *fullpath = cport->currentdir->iib.fullpath;
		LONG length_fp = strlen(fullpath);
		struct ItemEntryBlock *ieb;
		char *path;
		if ((ieb = nfs_support_get_ieb(name, cport)))
		{
			if ((path = AllocVec(length_fp + strlen(name) + 30 , MEMF_ANY | MEMF_CLEAR)))
			{
				BPTR fh;
				switch (ieb->ItemType)
				{

					case ITEMTYPE_SUBBOARD:

						if (!cport->currentdir->fh_dirs)
						{
							if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
								sprintf(path,"%s.dirs",fullpath);
							else
								sprintf(path,"%s/.dirs",fullpath);
							cport->currentdir->fh_dirs = Open(path, MODE_READWRITE);
						}
						else
						{
							if (Seek(cport->currentdir->fh_dirs, 0, OFFSET_BEGINNING) == -1)
								result = IoErr();
						}
						if (!result && (fh = cport->currentdir->fh_dirs))
						{
							struct Directory *dir;
							if ((dir = AllocVec(sizeof(Directory), MEMF_ANY)))
							{
								char founded = FALSE;
								while (!founded && Read(fh, dir, sizeof(Directory)) == sizeof(Directory))
								{
									if (ieb->IPNumber64 == dir->iib.IPNumber64 && ieb->IPNumber == dir->iib.IPNumber)
									{
										if (Seek(fh, - sizeof(Directory), OFFSET_CURRENT))
										{
											dir->iib.Deleted = TRUE;
											if (Write(fh, dir, sizeof(Directory)) == sizeof(Directory))
											{
												if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
													sprintf(path, "delete %s%s ALL QUIET <>NIL:",fullpath, name);
												else
													sprintf(path, "delete %s/%s ALL QUIET <>NIL:",fullpath, name);
											
												if (!Execute(path, NULL, NULL))
													result = IoErr();
											}
											else
												result = IoErr();
										}
										founded = TRUE;
									}
								}
								if (!founded)
								{
									result = ERROR_OBJECT_NOT_FOUND;
								}
								FreeVec(dir);
							}
							else
								result = ERROR_OUT_OF_MEMORY;
						}
						else
						{
							if (!result)
								result = ERROR_DIR_NOT_FOUND;
						}
						break;

					case ITEMTYPE_FILEARCHIV: case ITEMTYPE_COMMAND: case ITEMTYPE_MESSAGE:

						if (!cport->currentdir->fh_files)
						{
							if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
								sprintf(path,"%s.files",fullpath);
							else
								sprintf(path,"%s/.files",fullpath);
							cport->currentdir->fh_files = Open(path, MODE_READWRITE);
						}
						else
						{
							if (Seek(cport->currentdir->fh_files, 0, OFFSET_BEGINNING) == -1)
								result = IoErr();
						}
						if (!result && (fh = cport->currentdir->fh_files))
						{
							struct ItemEntry *item;
							if ((item = AllocVec(sizeof(ItemEntry), MEMF_ANY)))
							{
								char founded = FALSE;
								while (!founded && Read(fh, item, sizeof(ItemEntry)) == sizeof(ItemEntry))
								{
									if (ieb->IPNumber64 == item->iib.IPNumber64 && ieb->IPNumber == item->iib.IPNumber)
									{
										if (Seek(fh, - sizeof(ItemEntry), OFFSET_CURRENT))
										{
											item->iib.Deleted = TRUE;
											if (Write(fh, item, sizeof(ItemEntry)) == sizeof(ItemEntry))
											{
												if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
													sprintf(path, "delete %s%s ALL QUIET <>NIL:",fullpath, name);
												else
													sprintf(path, "delete %s/%s ALL QUIET <>NIL:",fullpath, name);
											
												if (!Execute(path, NULL, NULL))
													result = IoErr();
											}
											else
												result = IoErr();
										}
										founded = TRUE;
									}
								}
								if (!founded)
								{
									result = ERROR_OBJECT_NOT_FOUND;
								}
								FreeVec(item);
							}
							else
								result = ERROR_OUT_OF_MEMORY;
						}
						else
						{
							if (!result)
								result = ERROR_DIR_NOT_FOUND;
						}
						break;

				}
				FreeVec(path);
			}
			else
				result = ERROR_OUT_OF_MEMORY;
		}
		else
			result = ERROR_OBJECT_NOT_FOUND;
	}
	else
		result = ERROR_NOT_A_NOVIA_DOS_PROGRAMM;
	return result;
}

LONG nfs_support_addmail(struct NewItem *item, PortData *cport)
{
	LONG					result = 0;
	struct ItemEntry  *itementry;
	if (cport && item)
	{
		char *buffer;
		if ((buffer = AllocVec(strlen(cport->currentdir->iib.fullpath)+strlen(item->Subject)+30,MEMF_ANY)))	// for full path
		{
			if ((itementry = AllocVec(sizeof(ItemEntry),MEMF_ANY|MEMF_CLEAR)))
			{
				BPTR						fh;											// FileHandle for <Subject>.msg file
				BPTR						replylock;									// FileLock   for <Subject>.replys directory
				BPTR						txtfh;										// FileHandle for <Subject>.text file
				if (!((fh = cport->currentdir->fh_files)))
				{
					sprintf(buffer,"%s/.messages",cport->currentdir->iib.fullpath);
					if ((fh = Open(buffer,MODE_READWRITE)))
						cport->currentdir->fh_files = fh;
				}
				if (fh)
				{
					if (!((txtfh = cport->currentdir->fh_files)))
					{
						sprintf(buffer,"%s/.text",cport->currentdir->iib.fullpath);
						if ((txtfh = Open(buffer,MODE_READWRITE)))
							cport->currentdir->fh_text = txtfh;
					}
					if (txtfh)
					{
						SetIPCounter64(&itementry->iib.IPNumber64);

						strncpy(itementry->iib.Title,item->Subject,sizeof(itementry->iib.Title)-1);								// copy subdir name into title
						sprintf(itementry->iib.Filename,".text");

						itementry->iib.ItemType		= item->ItemType;										// normaly ITEMTYPE_MESSAGE

						CopyMemQuick(&item->CreateDate,&itementry->iib.CreateDate,sizeof(Date));	// true writing date
						CopyMemQuick(&item->CreateDate,&itementry->iib.LastChange,sizeof(Date));

						itementry->iib.Size			= strlen(item->text);
						itementry->iib.Protection	= item->Protection;

						itementry->iib.CreateUserIP= item->CreateUserIP;

						GetTime(&itementry->SendDate);														// import date in database 
						itementry->ItemType			= 0;														// 0 - mail / 1 - mail

						itementry->ToUser				= item->ToUser;
						itementry->FromUser			= item->FromUser;
						itementry->RealUser			= item->FromUser;

						strncpy(itementry->FromRealName,	item->FromRealName,	sizeof(itementry->FromRealName)-1);
						strncpy(itementry->From, 			item->From,				sizeof(itementry->From)-1);
						strncpy(itementry->ToRealName, 	item->ToRealName,		sizeof(itementry->ToRealName)-1);
						strncpy(itementry->To, 				item->To,				sizeof(itementry->To)-1);
						strncpy(itementry->Alias,			item->Alias,			sizeof(itementry->Alias)-1);

						itementry->anonymous			= item->anonymous;

						itementry->length				= itementry->iib.Size;
						itementry->DirIP				= cport->currentdir->iib.IPNumber;				// serialIP of directory

						Seek(txtfh, 0, OFFSET_END);
						itementry->text_seek = Seek(txtfh, 0, OFFSET_END);
						Seek(fh, 0, OFFSET_END);
						Write(fh,	 itementry, sizeof(ItemEntry));
						Write(txtfh, item->text, itementry->length);
					}
					else
					{
						ioprintf("can't open %s\n",buffer);
						result = IoErr();
					}
				}
				else
				{
					ioprintf("can't open %s\n",buffer);
					result = IoErr();
				}
				FreeVec(itementry);
			}
			else
				result = ERROR_OUT_OF_MEMORY;
			FreeVec(buffer);
		}
		else
			result = ERROR_OUT_OF_MEMORY;
	}
	return result;
}

LONG nfs_support_addreply(char *itemname, char *replyname, PortData *cport)
{
	LONG result = 0;
	return result;
}

LONG nfs_support_loadselectlist(List *select_list, PortData *cport)
{
	char *buffer;
	LONG result = FALSE;
	BPTR mulder;
	Select *select;

	while ((select = (Select *)select_list->lh_Head)->ln_Succ)
	{
		Remove((Node *)select);
		if (select->dirpath)FreeVec(select->dirpath);
		if (select->filepath)FreeVec(select->filepath);
		FreeVec(select);
	}
	NewList(&cport->select_list);
	if ((buffer = AllocVec(strlen(cport->sysdirs->userdir)+100,MEMF_ANY)))
	{
		if (cport->LocalUser.Data_dir && (strlen(cport->LocalUser.Data_dir)>1))
			sprintf(buffer,"novia:home/%s/sys.selectlist",cport->LocalUser.Handle);
		else
			sprintf(buffer,"novia:home/%s/sys.selectlist",cport->LocalUser.Handle);
		if ((mulder = Open(buffer,MODE_OLDFILE)))
		{
			struct SelectListFileHeader	header;
			ULONG									ulong;
			ULONG									dirlength;
			ULONG									filelength;
			BOOL									quit = FALSE;
			if (Read(mulder,&header,sizeof(SelectListFileHeader)) == sizeof(SelectListFileHeader))			// get fileheader
			{
				while (!quit)
				{
					if ((select = AllocVec(sizeof(Select),MEMF_ANY|MEMF_CLEAR)))
					{
						if (Read(mulder,select,sizeof(Select)-8) == sizeof(Select)-8)
						{
							select->dirpath  = 0;
							select->filepath = 0;
							select->ln_Succ  = 0;
							select->ln_Pred  = 0;
							dirlength	= 0;
							filelength	= 0;
							Read(mulder,&dirlength,4);
							Read(mulder,&filelength,4);
							if (dirlength)
							{
								if ((select->dirpath = AllocVec(dirlength+1,MEMF_ANY|MEMF_CLEAR)))
								{
									if (Read(mulder,select->dirpath,dirlength) != dirlength)
										quit = TRUE;
								}
								else
									quit = TRUE;
							}
							if (filelength)
							{
								if ((select->filepath = AllocVec(filelength+1,MEMF_ANY|MEMF_CLEAR)))
								{
									if (Read(mulder,select->filepath,filelength) != filelength)
										quit = TRUE;
								}
								else
									quit = TRUE;
							}
							AddTail(select_list,(Node *)select);
						}
						else
							quit = TRUE;
					}
					else
						quit = TRUE;
				}
			}
			Close(mulder);
		}
		else
			ioprintf("can't open selectlist: %s\n",buffer);
	}
	return result;
}

LONG nfs_support_saveselectlist(List *select_list, PortData *cport)
{
	char *buffer;
	LONG result = FALSE;
	BPTR mulder;

	if ((buffer = AllocVec(strlen(cport->sysdirs->userdir)+200,MEMF_ANY)))
	{
		if (cport->LocalUser.Data_dir && (strlen(cport->LocalUser.Data_dir)>1))
			sprintf(buffer,"novia:home/%s/sys.selectlist",cport->LocalUser.Handle);
		else
			sprintf(buffer,"novia:home/%s/sys.selectlist",cport->LocalUser.Handle);
		if ((mulder = Open(buffer,MODE_NEWFILE)))
		{
			struct SelectListFileHeader	header;
			struct Select						*select;
			ULONG									dirlength;
			ULONG									filelength;
			BOOL									quit = FALSE;
			ClearMem(&header, sizeof(SelectListFileHeader));
			select = (Select *)select_list->lh_Head;
			while (select->ln_Succ)
			{
				header.entrys++;
				select = select->ln_Succ;
			}
			if (Write(mulder,&header,sizeof(SelectListFileHeader)) == sizeof(SelectListFileHeader))			// get fileheader
			{
				select = (Select *)select_list->lh_Head;
				while (!quit && select->ln_Succ)
				{
					if (Write(mulder,select,sizeof(Select) - 8) == sizeof(Select) - 8)
					{
						if (select->dirpath)
							dirlength = strlen(select->dirpath);
						else
							dirlength = 0;

						if (select->filepath)
							filelength = strlen(select->filepath);
						else
							filelength = 0;

						Write(mulder,&dirlength,4);
						Write(mulder,&filelength,4);
						if (dirlength)Write(mulder,select->dirpath,dirlength);
						if (filelength)Write(mulder,select->filepath,filelength);

					}
					select = select->ln_Succ;
				}
			}
			Close(mulder);
		}
		else
			ioprintf("can't write selectlist: %s\n",buffer);
	}
	return result;
}

LONG nfs_support_getlist(PortData *cport)
{
	LONG			  result = FALSE;
	BPTR			  fh		= FALSE;
	char 			  *pos;
	char			  filename[128];
	char			  *filenamepos;
	char			  *file;
	char			  quit = FALSE;
	char			  buf[150];
	ItemInfoBlock *iib;
	Directory	  *dir;
	MailEntry	  *mail;
	INet_EMail_Entry *inet_email_entry;
	ItemEntry	  *item;
	ItemEntryBlock *ieb;
	Select		  *select;

	while ((ieb = cport->itementrylist.lh_Head)->ln_Succ)
	{
		Remove((Node *)ieb);
		if (ieb->Title)FreeVec(ieb->Title);
		if (ieb->Comment)FreeVec(ieb->Comment);
		if (ieb->filename)FreeVec(ieb->filename);
		FreeVec(ieb);
	}

	if (cport->currentdir)
	{
		char *fullpath = cport->currentdir->iib.fullpath;
		ULONG length_fp = strlen(fullpath);
		cport->itementrylist.files			= 0;
		cport->itementrylist.dirs			= 0;
		cport->itementrylist.size			= 0;
		cport->itementrylist.IDCounter	= 1;
		if ((file = AllocVec(strlen(cport->currentdir->iib.fullpath) + 128, MEMF_ANY)))
		{
			if ((dir = AllocVec(sizeof(Directory), MEMF_ANY)))
			{
				iib = (ItemInfoBlock *)dir;
				if ((fh = cport->currentdir->fh_dirs))
					Seek(fh, 0, OFFSET_BEGINNING);
				else
				{
					if (fullpath)
					{
						if (length_fp)
						{
							if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
								sprintf(file,"%s.dirs",fullpath);
							else
								sprintf(file,"%s/.dirs",fullpath);
							if ((fh = Open(file,MODE_READWRITE)))
							{
								cport->currentdir->fh_dirs = fh;
							}
						}
					}
				}
				if (fh)
				{
					while (Read(fh, dir, sizeof(Directory)) == sizeof(Directory))
					{
						if (!iib->Deleted)
						{
							if ((ieb = AllocVec(sizeof(ItemEntryBlock),MEMF_ANY|MEMF_CLEAR)))
							{
								ULONG length;
								ieb->IDNumber			= cport->itementrylist.IDCounter++;
								ieb->IPNumber64		= iib->IPNumber64;
								ieb->IPNumber			= iib->IPNumber;
								ieb->ItemType			= iib->ItemType;
								CopyMemQuick(&iib->LastChange, &ieb->LastChange, 12);
								CopyMemQuick(&iib->CreateDate, &ieb->CreateDate, 12);
								ieb->Size64				= iib->Size64;
								ieb->Size				= iib->Size;
								ieb->NumBlocks64		= iib->NumBlocks64;
								ieb->NumBlocks			= iib->NumBlocks;
								ieb->Protection		= iib->Protection;
								ieb->UID					= iib->UID;
								ieb->GID					= iib->GID;
								ieb->AccessLevel		= iib->AccessLevel;
								ieb->PostAccess		= iib->PostAccess;
								ieb->UploadAccess		= iib->UploadAccess;
								ieb->DownloadAccess	= iib->DownloadAccess;
								ieb->ComputerTypes	= iib->ComputerTypes;
								ieb->ReplyAccess		= iib->ReplyAccess;
								ieb->CreateUserIP		= iib->CreateUserIP;
//								ieb->seleted			= iib->;
								ieb->NetworkType		= dir->NetworkType;
								ieb->ItemEntrys		= dir->ItemEntrys;
								ieb->DirEntrys			= dir->DirEntrys;
								cport->itementrylist.dirs++;
								quit = FALSE;
								select = (Select *)cport->select_list.mlh_Head;
								while (!quit && select->ln_Succ)
								{
									if (select->directoryIP64 == iib->IPNumber64 && select->directoryIP == iib->IPNumber && select->itemIP == 0 && select->itemIP64 == 0)
									{
										ieb->selected = TRUE;
										quit = TRUE;
									}
									else
										select=select->ln_Succ;
								}
								if ((length = strlen(iib->Title)))
								{
									if ((ieb->Title = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->Title, iib->Title);
								}
								if ((length = strlen(iib->Comment)))
								{
									if ((ieb->Comment = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->Comment, iib->Comment);
								}
								if ((length = strlen(iib->Filename)))
								{
									if ((ieb->filename = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->filename, iib->Filename);
								}
								AddTail((List *)&cport->itementrylist,(Node *)ieb);
							}
						}
					}
				}
				FreeVec(dir);
			}

			if ((item = AllocVec(sizeof(ItemEntry),MEMF_ANY)))
			{
				iib = (ItemInfoBlock *)item;
				if ((fh = cport->currentdir->fh_files))
					Seek(fh, 0, OFFSET_BEGINNING);
				else
				{
					if (fullpath)
					{
						if (length_fp)
						{
							if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
								sprintf(file,"%s.files",fullpath);
							else
								sprintf(file,"%s/.files",fullpath);
							if ((fh = Open(file,MODE_READWRITE)))
								cport->currentdir->fh_files = fh;
						}
					}
				}
				if (fh)
				{
					while (Read(fh, item, sizeof(ItemEntry)) == sizeof(ItemEntry))
					{
						if (!iib->Deleted)
						{
							if ((ieb = AllocVec(sizeof(ItemEntryBlock),MEMF_ANY|MEMF_CLEAR)))
							{
								ULONG length;
								ieb->IDNumber				= cport->itementrylist.IDCounter++;
								ieb->IPNumber64			= iib->IPNumber64;
								ieb->IPNumber				= iib->IPNumber;
								ieb->ItemType				= iib->ItemType;
								CopyMemQuick(&iib->LastChange,&ieb->LastChange,12);
								CopyMemQuick(&iib->CreateDate,&ieb->CreateDate,12);
								ieb->Size64				= iib->Size64;
								ieb->Size				= iib->Size;
								ieb->NumBlocks64		= iib->NumBlocks64;
								ieb->NumBlocks			= iib->NumBlocks;
								ieb->Protection		= iib->Protection;
								ieb->UID					= iib->UID;
								ieb->GID					= iib->GID;
								ieb->AccessLevel		= iib->AccessLevel;
								ieb->PostAccess		= iib->PostAccess;
								ieb->UploadAccess		= iib->UploadAccess;
								ieb->DownloadAccess	= iib->DownloadAccess;
								ieb->ComputerTypes	= iib->ComputerTypes;
								ieb->ReplyAccess		= iib->ReplyAccess;
								ieb->CreateUserIP		= iib->CreateUserIP;
	//							ieb->seleted			= iib->;
								ieb->NetworkType		= item->NetworkType;
								ieb->FileType  		= item->ItemType;
								ieb->replys				= item->replys;
								ieb->FileVersion		= item->FileVersion;
								ieb->besttransfer		= item->besttransfer;
								ieb->downloads			= item->downloads;
								if (!ieb->Size)										// is't files ?
									ieb->Size			= item->length;
								else
									ieb->FileType		= 1;
								cport->itementrylist.size = cport->itementrylist.size + ieb->Size;
								quit = FALSE;
								cport->itementrylist.files++;
								select = (Select *)cport->select_list.mlh_Head;
								while (!quit && select->ln_Succ)
								{
									if (select->directoryIP == item->DirIP && select->itemIP64 == iib->IPNumber64 && select->itemIP == iib->IPNumber)
									{
										ieb->selected = TRUE;
										quit = TRUE;
									}
									else
										select=select->ln_Succ;
								}
								if ((length = strlen(iib->Title)))
								{
									if ((ieb->Title = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->Title,iib->Title);
								}
								if ((length = strlen(iib->Comment)))
								{
									if ((ieb->Comment = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->Comment,iib->Comment);
								}
								if ((length = strlen(iib->Filename)))
								{
									if ((ieb->filename = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->filename,iib->Filename);
								}
								if (iib->ItemType == ITEMTYPE_MESSAGE | iib->ItemType == ITEMTYPE_FILEARCHIV)
								{
									strncpy(ieb->FromRealName,	item->FromRealName,	sizeof(ieb->FromRealName)-1);
									strncpy(ieb->From,			item->From,				sizeof(ieb->From)-1);
									strncpy(ieb->ToRealName,	item->ToRealName,		sizeof(ieb->ToRealName)-1);
									strncpy(ieb->To,				item->To,				sizeof(ieb->To)-1);
								}
								AddTail((List *)&cport->itementrylist,(Node *)ieb);
							}
						}
					}
				}
				FreeVec(item);
			}

			if ((mail = AllocVec(sizeof(MailEntry),MEMF_ANY)))
			{
				iib = (ItemInfoBlock *)mail;
				if (cport->currentdir->fh_messages)
					Seek(cport->currentdir->fh_messages, 0, OFFSET_BEGINNING);
				else
				{
					if (fullpath)
					{
						if (length_fp)
						{
							if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
								sprintf(file,"%s.messages",fullpath);
							else
								sprintf(file,"%s/.messages",fullpath);
							if ((fh = Open(file,MODE_READWRITE)))
								cport->currentdir->fh_messages = fh;
						}
					}
				}
				if (cport->currentdir->fh_messages)
				{
					while (Read(cport->currentdir->fh_messages, mail, sizeof(MailEntry)) == sizeof(MailEntry))
					{
						if (!iib->Deleted)
						{
							if ((ieb = AllocVec(sizeof(ItemEntryBlock),MEMF_ANY|MEMF_CLEAR)))
							{
								ULONG length;
								ieb->IDNumber				= cport->itementrylist.IDCounter++;
								ieb->IPNumber64			= iib->IPNumber64;
								ieb->IPNumber				= iib->IPNumber;
								ieb->ItemType				= iib->ItemType;
								CopyMemQuick(&iib->LastChange,&ieb->LastChange,12);
								CopyMemQuick(&iib->CreateDate,&ieb->CreateDate,12);
								ieb->Size64				= iib->Size64;
								ieb->Size				= iib->Size;
								ieb->NumBlocks64		= iib->NumBlocks64;
								ieb->NumBlocks			= iib->NumBlocks;
								ieb->Protection		= iib->Protection;
								ieb->UID					= iib->UID;
								ieb->GID					= iib->GID;
								ieb->AccessLevel		= iib->AccessLevel;
								ieb->PostAccess		= iib->PostAccess;
								ieb->UploadAccess		= iib->UploadAccess;
								ieb->DownloadAccess	= iib->DownloadAccess;
								ieb->ComputerTypes	= iib->ComputerTypes;
								ieb->ReplyAccess		= iib->ReplyAccess;
								ieb->CreateUserIP		= iib->CreateUserIP;
	//							ieb->seleted			= iib->;
								ieb->NetworkType		= mail->NetworkType;
								ieb->FileType  		= mail->ItemType;
								ieb->replys				= mail->replys;
								ieb->FileVersion		= mail->FileVersion;
								ieb->besttransfer		= mail->besttransfer;
								ieb->downloads			= mail->downloads;
								if (!ieb->Size)										// is't files ?
									ieb->Size			= mail->length;
								else
									ieb->FileType		= 1;
								cport->itementrylist.size = cport->itementrylist.size + ieb->Size;
								quit = FALSE;
								cport->itementrylist.files++;
								select = (Select *)cport->select_list.mlh_Head;
								while (!quit && select->ln_Succ)
								{
									if (select->directoryIP == mail->DirIP && select->itemIP64 == iib->IPNumber64 && select->itemIP == iib->IPNumber)
									{
										ieb->selected = TRUE;
										quit = TRUE;
									}
									else
										select=select->ln_Succ;
								}
								if ((length = strlen(iib->Title)))
								{
									if ((ieb->Title = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->Title,iib->Title);
								}
								if ((length = strlen(iib->Comment)))
								{
									if ((ieb->Comment = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->Comment,iib->Comment);
								}
								if ((length = strlen(iib->Filename)))
								{
									if ((ieb->filename = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->filename,iib->Filename);
								}
								strncpy(ieb->FromRealName	, mail->FromRealName,sizeof(ieb->FromRealName)-1);
								strncpy(ieb->From				, mail->From,sizeof(ieb->From)-1);
								strncpy(ieb->ToRealName		, mail->ToRealName,sizeof(ieb->ToRealName)-1);
								strncpy(ieb->To				, mail->To,sizeof(ieb->To)-1);
								AddTail((List *)&cport->itementrylist,(Node *)ieb);
							}
						}
					}
				}
				FreeVec(mail);
			}

			if ((inet_email_entry = AllocVec(sizeof(INet_EMail_Entry),MEMF_ANY)))
			{
				BPTR fh_mails  = NULL;
				BPTR fh_fields = NULL;
				ULONG length;
				iib = (ItemInfoBlock *)inet_email_entry;
				if (fullpath)
				{
					if (length_fp)
					{
						if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
						{
							sprintf(file,"%s.inet_messages",fullpath);
							fh_mails = Open(file,MODE_READWRITE);
							sprintf(file,"%s.inet_field_entrys",fullpath);
							fh_fields = Open(file,MODE_READWRITE);
						}
						else
						{
							sprintf(file,"%s/.inet_messages",fullpath);
							fh_mails = Open(file,MODE_READWRITE);
							sprintf(file,"%s/.inet_field_entrys",fullpath);
							fh_fields = Open(file,MODE_READWRITE);
						}
					}
					if (fh_mails && fh_fields)
					{
						char *fieldbuffer;
						while (Read(fh_mails, inet_email_entry, sizeof(INet_EMail_Entry)) == sizeof(INet_EMail_Entry))
						{
							if ((ieb = AllocVec(sizeof(ItemEntryBlock), MEMF_ANY | MEMF_CLEAR)))
							{
								ieb->IDNumber				= cport->itementrylist.IDCounter++;
								ieb->IPNumber64			= iib->IPNumber64;
								ieb->IPNumber				= iib->IPNumber;
								ieb->ItemType				= iib->ItemType;
								CopyMemQuick(&iib->LastChange,&ieb->LastChange,12);
								CopyMemQuick(&iib->CreateDate,&ieb->CreateDate,12);
								ieb->Size64					= iib->Size64;
								ieb->Size					= iib->Size;
								ieb->NumBlocks64			= iib->NumBlocks64;
								ieb->NumBlocks				= iib->NumBlocks;
								ieb->Protection			= iib->Protection;
								ieb->UID						= iib->UID;
								ieb->GID						= iib->GID;
								ieb->AccessLevel			= iib->AccessLevel;
								ieb->PostAccess			= iib->PostAccess;
								ieb->UploadAccess			= iib->UploadAccess;
								ieb->DownloadAccess		= iib->DownloadAccess;
								ieb->ComputerTypes		= iib->ComputerTypes;
								ieb->ReplyAccess			= iib->ReplyAccess;
								ieb->CreateUserIP			= iib->CreateUserIP;
								ieb->NetworkType			= 1;
								ieb->FileType  			= ITEMTYPE_EMAIL;
								ieb->replys					= inet_email_entry->replys;
								cport->itementrylist.size = cport->itementrylist.size + inet_email_entry->textsize;
								cport->itementrylist.files++;
								if ((length = strlen(iib->Title)))
								{
									if ((ieb->Title = AllocVec(length + 1,MEMF_ANY)))
										strcpy(ieb->Title,iib->Title);
								}

								if (inet_email_entry->FieldSize)
								{
									if ((fieldbuffer = AllocVec(inet_email_entry->FieldSize, MEMF_ANY)))
									{
										if (Seek(fh_fields, inet_email_entry->FirstField, OFFSET_BEGINNING) != -1)
										{
											if (Read(fh_fields, fieldbuffer, inet_email_entry->FieldSize) == inet_email_entry->FieldSize)
											{
												char *ptr = fieldbuffer;
												char quit = FALSE;
												ULONG fieldid;
												ULONG length;
												ULONG fieldlength;
												while (!quit)
												{
													CopyMem(ptr, &fieldid, 4);
													ptr = ptr + 4;
													fieldlength = (ULONG)*ptr;
													ptr++;
													if (fieldid)
													{
														switch(fieldid)
														{
															case INET_FIELD_ID_X_ENVELOPE_TO:
																length = (ULONG)*ptr;
																ptr++;
																strncpy(ieb->ToRealName, ptr, 40);
																ptr = ptr + length;
	
																length = (ULONG)*ptr;
																ptr++;
																strncpy(ieb->To, ptr, 80);
																ptr = ptr + length;
																break;
															case INET_FIELD_ID_FROM:
																length = (ULONG)*ptr;
																ptr++;
																strncpy(ieb->FromRealName, ptr, 40);
																ptr = ptr + length;
	
																length = (ULONG)*ptr;
																ptr++;
																strncpy(ieb->From, ptr, 80);
																ptr = ptr + length;
																break;
															case INET_FIELD_ID_SUBJECT:
																if ((ieb->Title = AllocVec(fieldlength, MEMF_ANY)))
																	strncpy(ieb->Title, ptr, fieldlength-1);
																ptr = ptr + fieldlength;												
																break;
															ptr = ptr + fieldlength;
														}
													}
													else
														quit = TRUE;
												}
											}
										}
									}
								}
								AddTail((List *)&cport->itementrylist,(Node *)ieb);
							}
						}
					}
					if (fh_mails)
						Close(fh_mails);
					if (fh_fields)
						Close(fh_fields);
				}
				FreeVec(inet_email_entry);
			}
			FreeVec(file);
		}
	}
	return FALSE;
}


LONG nfs_support_additementry(struct ItemEntry *itementry,  PortData *cport)
{
	LONG					result = 0;
	if (cport && itementry)
	{
		char *buffer;
		if ((buffer = AllocVec(strlen(cport->currentdir->iib.fullpath)+50,MEMF_ANY)))	// for full path
		{
			BPTR						fh;											// FileHandle for <Subject>.msg file
			BPTR						replylock;									// FileLock   for <Subject>.replys directory
			BPTR						txtfh;										// FileHandle for <Subject>.text file
			char						filename[50];
			if (!((fh = cport->currentdir->fh_files)))
			{	
				sprintf(buffer,"%s/.files",cport->currentdir->iib.fullpath);
				if ((fh = Open(buffer,MODE_READWRITE)))						// create real directory
					cport->currentdir->fh_files = fh;
			}
			if (fh)
			{
				if (!((txtfh = cport->currentdir->fh_text)))
				{
					sprintf(buffer,"%s/.text",cport->currentdir->iib.fullpath);
					if ((txtfh = Open(buffer,MODE_READWRITE)))
						cport->currentdir->fh_text = txtfh;
				}
				if (txtfh)
				{
					SetIPCounter64(&itementry->iib.IPNumber64);

					itementry->iib.Protection = (FIBF_READ | FIBF_WRITE | FIBF_EXECUTE | FIBF_DELETE);

					strncpy(itementry->iib.Filename,filename,sizeof(itementry->iib.Filename)-1);

					GetTime(&itementry->SendDate);														// import date in database 
					itementry->ItemType			= 0;														// 0 - mail / 1 - mail

					itementry->DirIP				= cport->currentdir->iib.IPNumber;				// serialIP of directory

					Seek(fh, 0, OFFSET_END);
					Seek(txtfh, 0, OFFSET_END);
					itementry->text_seek = Seek(txtfh, 0, OFFSET_END);
					Write(fh,	 itementry, 		sizeof(ItemEntry));
					if (itementry->text)
						Write(txtfh, itementry->text, itementry->length);
				}
				else
				{
					ioprintf("can't open %s\n",buffer);
					result = IoErr();
				}
			}
			else
			{
				ioprintf("can't open %s\n",buffer);
				result = IoErr();
			}
			FreeVec(buffer);
		}
		else
			result = ERROR_OUT_OF_MEMORY;
	}
	return result;
}

LONG nfs_support_additementry2dir(struct Directory *dir, struct ItemEntry *itementry, PortData *cport)
{
	LONG					result = 0;
	if (cport && itementry)
	{
		char *buffer;
		if ((buffer = AllocVec(strlen(dir->iib.fullpath)+50,MEMF_ANY)))	// for full path
		{
			BPTR						fh;											// FileHandle for <Subject>.msg file
			BPTR						txtfh;										// FileHandle for <Subject>.text file
			char						filename[50];
			if (!((fh = dir->fh_files)))
			{	
				sprintf(buffer,"%s/.files",dir->iib.fullpath);
				if ((fh = Open(buffer,MODE_READWRITE)))						// create real directory
					dir->fh_files = fh;
			}
			if (fh)
			{
				if (!((txtfh = dir->fh_text)))
				{
					sprintf(buffer,"%s/.text",dir->iib.fullpath);
					if ((txtfh = Open(buffer,MODE_READWRITE)))
						dir->fh_text = txtfh;
				}
				if (txtfh)
				{
					Seek(fh, 0, OFFSET_END);
					SetIPCounter64(&itementry->iib.IPNumber64);

					itementry->iib.Protection = (FIBF_READ | FIBF_WRITE | FIBF_EXECUTE | FIBF_DELETE);

					strncpy(itementry->iib.Filename,filename,sizeof(itementry->iib.Filename)-1);

					GetTime(&itementry->SendDate);														// import date in database 
					itementry->ItemType			= 0;														// 0 - mail / 1 - mail

					itementry->DirIP				= dir->iib.IPNumber;				// serialIP of directory

					Seek(txtfh, 0, OFFSET_END);
						itementry->text_seek = Seek(txtfh, 0, OFFSET_END);
					Write(fh,	 itementry, 	sizeof(ItemEntry));
					if (itementry->text)
						Write(txtfh, itementry->text, itementry->length);
				}
				else
				{
					ioprintf("can't open %s\n",buffer);
					result = IoErr();
				}
			}
			else
			{
				ioprintf("can't open %s\n",buffer);
				result = IoErr();
			}
			FreeVec(buffer);
		}
		else
			result = ERROR_OUT_OF_MEMORY;
	}
	return result;
}

LONG nfs_support_addmail2dir(Directory *dir, struct NewItem *item, PortData *cport)
{
	LONG					result = 0;
	struct ItemEntry  *itementry;
	if (cport && item)
	{
		char *buffer;
		if ((buffer = AllocVec(strlen(dir->iib.fullpath)+strlen(item->Subject)+30,MEMF_ANY)))	// for full path
		{
			if ((itementry = AllocVec(sizeof(ItemEntry),MEMF_ANY|MEMF_CLEAR)))
			{
				BPTR						fh;											// FileHandle for <Subject>.msg file
				BPTR						replylock;									// FileLock   for <Subject>.replys directory
				BPTR						txtfh;										// FileHandle for <Subject>.text file
				if (!((fh = dir->fh_files)))
				{	
					sprintf(buffer,"%s/.files",dir->iib.fullpath);
					if ((fh = Open(buffer,MODE_READWRITE)))						// create real directory
						dir->fh_files = fh;
				}
				if (fh)
				{
					if (!((txtfh = dir->fh_text)))
					{
						sprintf(buffer,"%s/.text",dir->iib.fullpath);
						if ((txtfh = Open(buffer,MODE_READWRITE)))
							dir->fh_text = txtfh;
					}
					if (txtfh)
					{
						Seek(fh, 0, OFFSET_END);
						SetIPCounter64(&itementry->iib.IPNumber64);

						strncpy(itementry->iib.Title,item->Subject,sizeof(itementry->iib.Title)-1);								// copy subdir name into title
						sprintf(itementry->iib.Filename,".text");

						itementry->iib.ItemType		= item->ItemType;										// normaly ITEMTYPE_MESSAGE

						CopyMemQuick(&item->CreateDate,&itementry->iib.CreateDate,sizeof(Date));	// true writing date
						CopyMemQuick(&item->CreateDate,&itementry->iib.LastChange,sizeof(Date));

						itementry->iib.Protection	= item->Protection;

						itementry->iib.CreateUserIP= item->CreateUserIP;

						GetTime(&itementry->SendDate);														// import date in database 
						itementry->ItemType			= 0;														// 0 - mail / 1 - mail

						itementry->ToUser				= item->ToUser;
						itementry->FromUser			= item->FromUser;
						itementry->RealUser			= item->FromUser;

						strncpy(itementry->FromRealName,	item->FromRealName,	sizeof(itementry->FromRealName)-1);
						strncpy(itementry->From, 			item->From,				sizeof(itementry->From)-1);
						strncpy(itementry->ToRealName, 	item->ToRealName,		sizeof(itementry->ToRealName)-1);
						strncpy(itementry->To, 				item->To,				sizeof(itementry->To)-1);
						strncpy(itementry->Alias,			item->Alias,			sizeof(itementry->Alias)-1);

						itementry->anonymous			= item->anonymous;

						itementry->DirIP				= dir->iib.IPNumber;				// serialIP of directory

						Seek(txtfh, 0, OFFSET_END);
						itementry->text_seek = Seek(txtfh, 0, OFFSET_END);
						Write(fh,	 itementry, sizeof(ItemEntry));
						Write(txtfh, item->text, itementry->length);
					}
					else
					{
						ioprintf("can't open %s\n",buffer);
						result = IoErr();
					}
				}
				else
				{
					ioprintf("can't open %s\n",buffer);
					result = IoErr();
				}
				FreeVec(itementry);
			}
			else
				result = ERROR_OUT_OF_MEMORY;
			FreeVec(buffer);
		}
		else
			result = ERROR_OUT_OF_MEMORY;
	}
	return result;
}

LONG nfs_support_closedir(Directory *dir, PortData *cport)
{
	LONG result = FALSE;
	if (dir)
	{
		if (dir->parrentdir)
		{
			struct Directory *parrentdir = dir->parrentdir;
			struct OpenEntry *entry;
			dir->iib.OpenCounter--;
			if ((entry = (OpenEntry *)dir->parrentdir->iib.OpenList.mlh_Head))
			{
				while (entry->ln_Succ && entry->cport != cport)
					entry = entry->ln_Succ;
				if (entry->cport == cport)
				{
					Remove((Node *)entry);
					FreeVec(entry);
				}
			}
			if (dir->iib.OpenCounter <= 0)
			{
				struct Directory *pdir = (Directory *)dir->parrentdir->subdirlist.mlh_Head;
				while (pdir->iib.ln_Succ && pdir != dir)
					pdir = (Directory *)pdir->iib.ln_Succ;
				if (pdir == dir)
				{
					if (dir->fh_messages)	Close(dir->fh_messages);
					if (dir->fh_files)		Close(dir->fh_files);
					if (dir->fh_dirs)			Close(dir->fh_dirs);
					if (dir->fh_text)			Close(dir->fh_text);
					if (dir->fh_replys)		Close(dir->fh_replys);
					
					Remove((Node *)pdir);
					if (pdir->iib.fullpath)
						FreeVec(pdir->iib.fullpath);
					FreeVec(pdir);
				}
			}
			result = TRUE;
		}
	}
	return result;
}


LONG nfs_support_sendmail(NewMail *newmail, PortData *cport)
{
	LONG result = FALSE;
	struct PortData *mport = (PortData *)MainTask->tc_UserData;
	if (cport && LoadUser(&cport->EditUser, newmail->ToUser))
	{
		char buffer [200];
		struct MailEntry *mailentry;
		struct Date actualdate;
		GetTime(&actualdate);
		if ((mailentry = AllocVec(sizeof(MailEntry),MEMF_ANY|MEMF_CLEAR)))
		{
			if (newmail->ToUser)
			{
				BPTR fh_mail;
				BPTR fh_text;
				BOOL error	 = FALSE;
				sprintf(buffer,"novia:home/%s/.files",cport->EditUser.Handle);
				if ((fh_mail = Open(buffer,MODE_READWRITE)))
				{
					sprintf(buffer,"novia:home/%s/.text",cport->EditUser.Handle);
					if ((fh_text = Open(buffer,MODE_READWRITE)))
					{
						if (!result)
						{
							if (Seek(fh_text, 0, OFFSET_END) != -1)
							{
								mailentry->text_seek = Seek(fh_text, 0, OFFSET_END);
								if (mailentry->text_seek == -1)
									result = IoErr();
								else
								{
									if (newmail->length)
									{
										if (Write(fh_text, newmail->text, newmail->length) != newmail->length)
										{
											result = IoErr();
											if (result == 0)
												result = -1;
										}
									}
								}
							}
							else
								result = IoErr();
						}
						if (!result)
						{
							NFSPacket			*packet;
							struct PortData2	*clientport;
//							mailentry->IDNumber = 1;
							SetIPCounter64(&mailentry->iib.IPNumber64);
							mailentry->FromUser				= cport->LocalUser.IDNumber;
							mailentry->ToUser					= newmail->ToUser;
							mailentry->CCUser					= newmail->CCUser;
							mailentry->BCCUser				= newmail->BCCUser;
							mailentry->ReplyToUser			= newmail->ReplyToUser;
							mailentry->ReturnToUser			= newmail->ReturnToUser;

							CopyMemQuick(&actualdate,					&mailentry->iib.LastChange,	sizeof(Date));
							CopyMemQuick(&actualdate,					&mailentry->iib.CreateDate,	sizeof(Date));
							CopyMemQuick(&newmail->ExpirationDate,	&mailentry->ExpirationDays,	sizeof(Date));

							mailentry->ExpirationDays		= newmail->ExpirationDays;
							mailentry->ItemType				= newmail->MailType;

							strncpy(mailentry->iib.Title,		newmail->subject, 			sizeof(mailentry->iib.Title)-1);
							strncpy(mailentry->From,			cport->LocalUser.Handle,	sizeof(mailentry->From)-1);
							strncpy(mailentry->FromRealName,	cport->LocalUser.RealName,	sizeof(mailentry->FromRealName)-1);
							if (strlen(newmail->Alias) > 1)
								strncpy(mailentry->Alias,		newmail->Alias,				sizeof(mailentry->Alias)-1);
								
							mailentry->length = newmail->length;

							mailentry->Receipt				= newmail->Receipt;
							mailentry->Return					= newmail->Return;
							mailentry->ItemType				= newmail->FileMail;
							mailentry->Anonymous				= newmail->Anonymous;
							mailentry->copy_own				= newmail->copy_own;
							mailentry->iib.ItemType		= ITEMTYPE_EMAIL;
							Seek(fh_mail, 0, OFFSET_END);
							Write(fh_mail, mailentry, sizeof(MailEntry));

							clientport = (PortData2 *)client_list->lh_Head;
							while (clientport->port_node.ln_Succ)
							{
								if (clientport->port_address->LocalUser.IDNumber == newmail->ToUser)
								{
									if (clientport->port_address->network.online && clientport->port_address->nfs_port)
									{
										if ((packet = AllocVec(sizeof(NFSPacket),MEMF_ANY|MEMF_CLEAR)))
										{
											char buf[200];
											sprintf(buf,"c3you have a new mail from c7%sc3.",cport->LocalUser.Handle);
											packet->nfs_msg.mn_ReplyPort	= nfsserver->nfs_port;
											packet->nfs_result1				= RESULT_NFS_TEXTNOTIFY;
											if ((packet->nfs_result2 = AllocVec(strlen(buf)+1,MEMF_ANY)))
											{
												printf("send notify to: %d\n",clientport->port_address->clientconfig.IDNumber);
												strcpy((char *)packet->nfs_result2, buf);
												packet->nfs_cport					= 0;
												packet->nfs_msg.mn_ReplyPort	= nfsserver->nfs_port;
												PutMsg(clientport->port_address->nfs_port, (Message *)packet);
												printf("notify sended to client: %d\n",clientport->port_address->clientconfig.IDNumber);
											}
											else
												FreeVec(packet);
										}
									}
								}
								clientport = (PortData2 *)clientport->port_node.ln_Succ;
							}
								printf("mportuser: %d %d %d\n",mport->clientconfig.IDNumber,mport->LocalUser.IDNumber,newmail->ToUser);
							if (mport->LocalUser.IDNumber == newmail->ToUser)
							{
								printf("mportuser: %d\n",mport->LocalUser.IDNumber);
								if (mport->network.online && mport->nfs_port)
								{
									printf("semnd packet\n");
									if ((packet = AllocVec(sizeof(NFSPacket),MEMF_ANY|MEMF_CLEAR)))
									{
										char buf[200];
										sprintf(buf,"c3you have a new mail from c7%sc3.",cport->LocalUser.Handle);
										packet->nfs_msg.mn_ReplyPort	= nfsserver->nfs_port;
										packet->nfs_result1				= RESULT_NFS_TEXTNOTIFY;
										if ((packet->nfs_result2 = AllocVec(strlen(buf)+1,MEMF_ANY)))
										{
											printf("send notify to main\n");
											strcpy((char *)packet->nfs_result2, buf);
											packet->nfs_cport					= 0;
											packet->nfs_msg.mn_ReplyPort	= nfsserver->nfs_port;
											PutMsg(mport->nfs_port, (Message *)packet);
											printf("notify sended to main\n");
										}
										else
											FreeVec(packet);
									}
								}
							}

						}
						Close(fh_text);
					}
					else
						result = IoErr();
					Close(fh_mail);
				}
				else
					result = IoErr();
			}
			else						// * UUCP Mail *
			{
				BPTR fh_mail;
				BPTR fh_text;
				BOOL error	 = FALSE;
				sprintf(buffer,"novia:home/%s/.files",cport->EditUser.Handle);
				if ((fh_mail = Open(buffer,MODE_READWRITE)))
				{
					sprintf(buffer,"novia:home/%s/.text",cport->EditUser.Handle);
					if ((fh_text = Open(buffer,MODE_READWRITE)))
					{
						if (!result)
						{
							if (Seek(fh_text, 0, OFFSET_END) != -1)
							{
								mailentry->text_seek = Seek(fh_text, 0, OFFSET_END);
								if (mailentry->text_seek == -1)
									result = IoErr();
								else
								{
									if (newmail->length)
									{
										if (Write(fh_text, newmail->text, newmail->length) != newmail->length)
										{
											result = IoErr();
											if (result == 0)
												result = -1;
										}
									}
								}	
							}
							else
								result = IoErr();
						}
						if (!result)
						{
							SetIPCounter64(&mailentry->iib.IPNumber64);
							mailentry->FromUser				= cport->LocalUser.IDNumber;
							mailentry->ToUser					= newmail->ToUser;
							mailentry->CCUser					= newmail->CCUser;
							mailentry->BCCUser				= newmail->BCCUser;
							mailentry->ReturnToUser			= newmail->ReturnToUser;
							mailentry->ReplyToUser			= newmail->ReplyToUser;

							CopyMemQuick(&actualdate,					&mailentry->iib.LastChange,	sizeof(Date));
							CopyMemQuick(&actualdate,					&mailentry->iib.CreateDate,	sizeof(Date));
							CopyMemQuick(&newmail->ExpirationDate,	&mailentry->ExpirationDays,	sizeof(Date));

							mailentry->ExpirationDays		= newmail->ExpirationDays;
							mailentry->ItemType				= newmail->MailType;

							strncpy(mailentry->iib.Title,		newmail->subject, 			sizeof(mailentry->iib.Title)-1);
							strncpy(mailentry->From,			cport->LocalUser.Handle,	sizeof(mailentry->From)-1);
							strncpy(mailentry->FromRealName,	cport->LocalUser.RealName,	sizeof(mailentry->FromRealName)-1);
							if (strlen(newmail->Alias) > 1)
								strncpy(mailentry->Alias,		newmail->Alias,				sizeof(mailentry->Alias)-1);
								
							mailentry->length = newmail->length;

							mailentry->Receipt				= newmail->Receipt;
							mailentry->Return					= newmail->Return;
							mailentry->ItemType				= newmail->FileMail;
							mailentry->Anonymous				= newmail->Anonymous;
							mailentry->copy_own				= newmail->copy_own;
//							mailentry->LogID					= newmail->LogID;
//							mailentry->LogMail				= newmail->LogMail;

						}
						Close(fh_text);
					}
					else
						result = IoErr();
					Close(fh_mail);
				}
				else
					result = IoErr();
			}
			FreeVec(mailentry);
		}
		else
			result = -1;
	}
	else
		result = -1;
	return result;
}

LONG nfs_support_uucp_import_mail(char *filename, struct PortData *cport)
{
	LONG result = 0;
	ULONG length_fp	= strlen(filename);
	BPTR fh;
	struct INet_EMail_Entry *email;
	char *writebuffer;
	struct Date actualdate;
	ULONG userid = 0;
	GetTime(&actualdate);

	if (length_fp)
	{
		char *buffer;
		if ((buffer = AllocVec(1025, MEMF_ANY)))
		{
			if ((email = AllocVec(sizeof(INet_EMail_Entry), MEMF_ANY | MEMF_CLEAR)))
			{
				if ((writebuffer = AllocVec(2048, MEMF_ANY | MEMF_CLEAR)))
				{
					if ((fh = Open(filename, MODE_READWRITE)))
					{
						char *writeptr = writebuffer;
						ULONG writesize = 0;
						char *ptr = buffer;
						long rnd = clock() + (long)cport;	// zufallszahl;
						ULONG length;
						BPTR text_out;
						char headerend = FALSE;
						ULONG headersize = 0;
						char RealName[80];
						char UserName[128];
						struct INet_User_Address *inet_user_address;
						struct INet_String		 *inet_string;
						List fieldlist;
						NewList(&fieldlist);
						sprintf(buffer,"ram:%d",rnd);
						if (Seek(fh, 0, OFFSET_END) != -1)
							length = Seek(fh, 0, OFFSET_BEGINNING);
						else
							length = -1;
						if (length != -1)
						{
							length = 0;
							if ((text_out = Open(buffer, MODE_NEWFILE)))
							{
								while (!result && FGets(fh, buffer, 1024))
								{
									if (!headerend)
									{
										if (*buffer == 13 | *buffer == 10)
										{
											printf("headersize: %d\n",headersize);
											headerend = TRUE;
										}
										else if (mystrcmp(buffer, "X-Envelope-To:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_X_ENVELOPE_FROM;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
												if (!userid)
												{
													char buffer2[128];
													char *username = inet_user_address->UserName;
													ptr = buffer2;
													while (*username && *username != '@')
														*ptr++ = *username++;
													*ptr = 0;
													userid = SearchUser(buffer2);
												}
											}
										}
										else if (mystrcmp(buffer, "X-Envelope-From:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_X_ENVELOPE_TO;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "To:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_TO;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "From:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_FROM;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "Sender:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_SENDER;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "Reply-To:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_REPLY_TO;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "CC:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_CC;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "BCC:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_BCC;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "Return-Receipt-To:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_RETURN_RECEIPT_TO;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "Disposition-Notification-To:"))
										{
											if ((inet_user_address = Create_INet_User(buffer)))
											{
												inet_user_address->field.fieldid = INET_FIELD_ID_DISPOSITION_NOTIFICATION_TO;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_USER_ADDRESS;
												AddTail(&fieldlist, (Node *)inet_user_address);
											}
										}
										else if (mystrcmp(buffer, "Message-ID:"))
										{
											if ((inet_string = AllocVec(sizeof(struct INet_String), MEMF_ANY | MEMF_CLEAR)))
											{
												ptr = buffer;
												while (*ptr && *ptr != ':')
													ptr++;
												if (*ptr)
												{
													ptr++;
													while (isspace(*ptr))
														ptr++;
													if ((inet_string->string = AllocVec(strlen(ptr) + 3, MEMF_ANY | MEMF_CLEAR)))
													{
														char *org = inet_string->string;
														while (*ptr && *ptr != 13 && *ptr != 10)
															*org++ = *ptr++;
														*org = 0;
													}
												}
												inet_string->field.fieldid = INET_FIELD_ID_MESSAGE_ID;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_STRING;
												AddTail(&fieldlist, (Node *)inet_string);
											}
										}
										else if (mystrcmp(buffer, "Organization:"))
										{
											if ((inet_string = AllocVec(sizeof(struct INet_String), MEMF_ANY | MEMF_CLEAR)))
											{
												ptr = buffer;
												while (*ptr && *ptr != ':')
													ptr++;
												if (*ptr)
												{
													ptr++;
													while (isspace(*ptr))
														ptr++;
													if ((inet_string->string = AllocVec(strlen(ptr) + 3, MEMF_ANY | MEMF_CLEAR)))
													{
														char *org = inet_string->string;
														while (*ptr && *ptr != 13 && *ptr != 10)
															*org++ = *ptr++;
														*org = 0;
													}
												}
												inet_string->field.fieldid = INET_FIELD_ID_ORGANIZATION;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_STRING;
												AddTail(&fieldlist, (Node *)inet_string);
											}
										}
										else if (mystrcmp(buffer, "Date:"))
										{
											Date *date = &email->SendDate;
											ConvertUUCPDate(buffer, &email->SendDate);
										}
										else if (mystrcmp(buffer, "X-Mailer:"))
										{
											if ((inet_string = AllocVec(sizeof(struct INet_String), MEMF_ANY | MEMF_CLEAR)))
											{
												ptr = buffer;
												while (*ptr && *ptr != ':')
													ptr++;
												if (*ptr)
												{
													ptr++;
													while (isspace(*ptr))
														ptr++;
													if ((inet_string->string = AllocVec(strlen(ptr) + 3, MEMF_ANY | MEMF_CLEAR)))
													{
														char *org = inet_string->string;
														while (*ptr && *ptr != 13 && *ptr != 10)
															*org++ = *ptr++;
														*org = 0;
													}
												}
												inet_string->field.fieldid = INET_FIELD_ID_X_MAILER;
												inet_user_address->field.fieldtype = INET_FIELD_TYPE_STRING;
												AddTail(&fieldlist, (Node *)inet_string);
											}
										}
										else if (mystrcmp(buffer, "Subject:"))
										{
											if (!email->iib.Title[0])
											{
												char *org = email->iib.Title;
												ptr = buffer;
												while (*ptr && *ptr != ':')
													ptr++;
												if (*ptr)
												{
													ptr++;
													while (isspace(*ptr))
														ptr++;
													while (*ptr && *ptr != 10 && *ptr != 13)
														*org++ = *ptr++;
													*org = 0;
												}
											}
								
										}
									}
									ptr = buffer;
									if (writesize >= 1700)
									{
										if (Write(text_out, writebuffer, writesize) != writesize)
											result = IoErr();
										else
										{
											writeptr = writebuffer;
											writesize = 0;
										}
									}
									if (!result)
									{
										ptr = buffer;
										while (*ptr && *ptr != 10 && *ptr != 13 && writesize < 2000)
										{
											*writeptr++ = *ptr++;
											writesize++;
											length++;
											if (!headerend)
												headersize++;
										}
										*writeptr++ = 10;
										writesize++;
										length++;
										if (!headerend)
											headersize++;
									}
								}
								if (Write(text_out, writebuffer, writesize) != writesize)
									result = IoErr();
								if (!result)
								{
									if (userid)
									{
										if (LoadUser(&cport->EditUser, userid))
										{
											BPTR fh_mails;
											BPTR fh_fields;
											sprintf(buffer,"novia:home/%s/.inet_messages",cport->EditUser.Handle);
											if ((fh_mails = Open(buffer, MODE_READWRITE)))
											{
												sprintf(buffer,"novia:home/%s/.inet_field_entrys",cport->EditUser.Handle);
												if ((fh_fields = Open(buffer, MODE_READWRITE)))
												{
													SetIPCounter64(&email->iib.IPNumber64);
													CopyMemQuick(&actualdate,			&email->iib.LastChange,	sizeof(Date));
													CopyMemQuick(&actualdate,			&email->iib.CreateDate,	sizeof(Date));
						
													email->iib.ItemType					= ITEMTYPE_EMAIL;;
													email->textsize	= length;
													email->headersize = headersize;
					
													if (Seek(text_out, 0, OFFSET_BEGINNING) != -1)
													{
														BPTR fh_text;
														sprintf(buffer,"novia:home/%s/.text",cport->EditUser.Handle);
														if ((fh_text = Open(buffer, MODE_READWRITE)))
														{
															if (Seek(fh_text, 0, OFFSET_END) != -1)
															{
																if ((email->seek = Seek(fh_text, 0, OFFSET_END)) != -1)
																{
																	long readsize;
																	while (!result && (readsize = Read(text_out, writebuffer, 2048)))
																	{
																		if (readsize != -1)
																		{
																			if (Write(fh_text, writebuffer, readsize) == -1)
																				result = IoErr();
																		}
																		else
																			result = IoErr();
																	}
																	if (!result)
																	{
																		if (Seek(fh_mails, 0, OFFSET_END) != -1)
																		{
																			if (Seek(fh_fields, 0, OFFSET_END) != -1)
																			{
																				if ((email->FirstField = Seek(fh_fields, 0, OFFSET_END)) != -1)
																				{
																					char usernamesize, realnamesize, fieldsize;
																					struct INet_User_Address *address = (struct INet_User_Address *)fieldlist.lh_Head;
																					struct INet_String		 *string;
																					char *writebuffer = AllocVec(20000, MEMF_ANY | MEMF_CLEAR);
																					if (writebuffer)
																					{
																						char *ptr = writebuffer;
																						while (address->field.ln_Succ)
																						{
																							if (address->field.fieldtype == INET_FIELD_TYPE_USER_ADDRESS)
																							{
																								printf("fieldtype: %d\n",address->field.fieldid);
																								CopyMem(&address->field.fieldid, ptr, 4);
																								ptr = ptr + 4;
																								usernamesize = strlen(address->UserName) + 1;
																								realnamesize = strlen(address->RealName) + 1;
																								fieldsize    = usernamesize + realnamesize + 2;
																								*ptr++ = fieldsize;

																								*ptr++ = realnamesize;
																								CopyMem(address->RealName, ptr, realnamesize);
																								ptr = ptr + realnamesize;

																								*ptr++ = usernamesize;
																								CopyMem(address->UserName, ptr, usernamesize);
																								ptr = ptr + usernamesize;
																								email->FieldSize = email->FieldSize + fieldsize + 5;
																								email->FieldEntrys++;
																							}
																							else if (address->field.fieldtype == INET_FIELD_TYPE_STRING)
																							{
																								string = (INet_String *)address;
																								CopyMem(&string->field.fieldid, ptr, 4);
																								ptr = ptr + 4;
																								fieldsize    = strlen(string->string) + 1;
																								*ptr++ = fieldsize;

																								CopyMem(string->string, ptr, fieldsize);
																								ptr = ptr + fieldsize;

																								email->FieldSize = email->FieldSize + fieldsize + 5;
																								email->FieldEntrys++;
																							}
																							address = (INet_User_Address *)address->field.ln_Succ;
																						}
																						if (Write(fh_fields, writebuffer, email->FieldSize) == -1)
																							result = IoErr();
																						FreeVec(writebuffer);
																					}
																					else
																						result = ERROR_OUT_OF_MEMORY;
																				}
																				else
																					result = IoErr();
																			}
																			else
																				result = IoErr();
																			if (!result)
																			{
																				if (Write(fh_mails, email, sizeof(INet_EMail_Entry)) != sizeof(INet_EMail_Entry))
																					result = IoErr();
																			}
																			result = IoErr();
																		}
																		else
																			result = IoErr();
																	}
																}
																else
																	result = IoErr();
															}
															Close(fh_text);
														}
														else
															result = IoErr();
													}
													else
														result = IoErr();
													Close(fh_fields);
												}
												else
													result = IoErr();
												Close(fh_mails);
											}
											else
												result = IoErr();
										}
										else
										{
											result = ERROR_USER_NOT_FOUND;
										}
									}
									else
										result = ERROR_USER_NOT_FOUND;
								}
								Close(text_out);
								sprintf(buffer,"ram:%d",rnd);
								DeleteFile(buffer);
							}
							else
								result = IoErr();
						}
						else
							result = IoErr();
						Close(fh);
					}
					else
						result = IoErr();
				}
				else
					result = ERROR_OUT_OF_MEMORY;
			}
			else
				result = ERROR_OUT_OF_MEMORY;
		}
		else
			result = ERROR_OUT_OF_MEMORY;
	}
	else
		result = ERROR_INVALID_OBJECT_NAME;
	return(result);
}



LONG nfs_support_rename(char *oldname, char *newname, PortData *cport)
{
	LONG result = FALSE;
	if (cport->currentdir)
	{
		BOOL		founded	= FALSE;
		ULONG 	IDNumber	= 0;
		struct ItemEntryBlock *ieb;
		ieb = nfs_support_get_ieb(oldname, cport);
		if ((ieb = nfs_support_get_ieb(oldname, cport)))
		{
			BPTR fh;
			char *path;
			char *fullpath = cport->currentdir->iib.fullpath;
			ULONG length_fp = strlen(fullpath);
			if ((path = AllocVec(length_fp + 150, MEMF_ANY | MEMF_CLEAR)))
			{
				switch (ieb->ItemType)
				{
					case ITEMTYPE_SUBBOARD:

						if (!cport->currentdir->fh_dirs)
						{
							if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
								sprintf(path,"%s.dirs",fullpath);
							else
								sprintf(path,"%s/.dirs",fullpath);
							cport->currentdir->fh_dirs = Open(path,MODE_READWRITE);
						}
						if (cport->currentdir->fh_dirs)
						{
							ULONG entrys, counter;
							founded = FALSE;
							Seek(cport->currentdir->fh_dirs, 0, OFFSET_END);
							entrys = Seek(cport->currentdir->fh_dirs, 0, OFFSET_BEGINNING) / sizeof(Directory);
							if (entrys)
							{
								struct Directory *dir;
								if ((dir = AllocVec(sizeof(Directory), MEMF_ANY)))
								{
									while (!founded && Read(cport->currentdir->fh_dirs, dir, sizeof(Directory)) == sizeof(Directory))
									{
										if (dir->iib.IPNumber == ieb->IPNumber && dir->iib.IPNumber64 == ieb->IPNumber64)
										{
											if (Seek(cport->currentdir->fh_dirs, - sizeof(Directory), OFFSET_CURRENT) != -1)
											{
												char *newpath;
												if ((newpath = AllocVec(length_fp + 150, MEMF_ANY | MEMF_CLEAR)))
												{
													if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
													{
														sprintf(path,"%s%s",fullpath,oldname);
														sprintf(newpath,"%s%s",fullpath,newname);
													}
													else
													{
														sprintf(path,"%s/%s",fullpath,oldname);
														sprintf(newpath,"%s/%s",fullpath,newname);
													}
													strncpy(dir->iib.Title,		newname, sizeof(dir->iib.Title)-1);
													strncpy(dir->iib.Filename,	newname, sizeof(dir->iib.Filename)-1);

													if (Rename(path,newpath))
													{
														if (ieb->Title)
														{
															FreeVec(ieb->Title);
															if ((ieb->Title = AllocVec(strlen(newname) + 1,MEMF_ANY)))
																strcpy(ieb->Title,newname);
														}
														if (ieb->filename)
														{
															FreeVec(ieb->filename);
															if ((ieb->Title = AllocVec(strlen(newname) + 1,MEMF_ANY)))
																strcpy(ieb->filename,newname);
														}
														if (Write(cport->currentdir->fh_dirs, dir, sizeof(Directory)) != sizeof(Directory))
															result = IoErr();
													}
													else
														result = IoErr();
												}
												else
													result = ERROR_OUT_OF_MEMORY;
											}
											else
												result = IoErr();
											founded = TRUE;
										}
									}
									FreeVec(dir);
								}
								else
									result = ERROR_OUT_OF_MEMORY;
							}
							else
								result = ERROR_OBJECT_NOT_FOUND;
						}
						else
							result = ERROR_OBJECT_NOT_FOUND;
						FreeVec(path);
						break;

					case ITEMTYPE_MESSAGE: case ITEMTYPE_FILEARCHIV: case ITEMTYPE_COMMAND:

						if (!cport->currentdir->fh_files)
						{
							if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
								sprintf(path,"%s.files",fullpath);
							else
								sprintf(path,"%s/.files",fullpath);
							cport->currentdir->fh_files = Open(path,MODE_READWRITE);
						}
						if (cport->currentdir->fh_files)
						{
							ULONG entrys, counter;
							founded = FALSE;
							Seek(cport->currentdir->fh_files, 0, OFFSET_END);
							entrys = Seek(cport->currentdir->fh_files, 0, OFFSET_BEGINNING) / sizeof(ItemEntry);
							if (entrys)
							{
								struct ItemEntry *item;
								if ((item = AllocVec(sizeof(struct ItemEntry), MEMF_ANY)))
								{
									while (!founded && Read(cport->currentdir->fh_files, item, sizeof(ItemEntry)) == sizeof(ItemEntry))
									{
										if (item->iib.IPNumber == ieb->IPNumber && item->iib.IPNumber64 == ieb->IPNumber64)
										{
											if (Seek(cport->currentdir->fh_files, - sizeof(ItemEntry), OFFSET_CURRENT) != -1)
											{
												char *newpath;
												if ((newpath = AllocVec(length_fp + 150, MEMF_ANY | MEMF_CLEAR)))
												{
													if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
													{
														sprintf(path,"%s%s",fullpath,oldname);
														sprintf(newpath,"%s%s",fullpath,newname);
													}
													else
													{
														sprintf(path,"%s/%s",fullpath,oldname);
														sprintf(newpath,"%s/%s",fullpath,newname);
													}
													strncpy(item->iib.Title,		newname, sizeof(item->iib.Title)-1);
													strncpy(item->iib.Filename,	newname, sizeof(item->iib.Filename)-1);

													if (Rename(path,newpath))
													{
														if (ieb->Title)
														{
															FreeVec(ieb->Title);
															if ((ieb->Title = AllocVec(strlen(newname) + 2,MEMF_ANY | MEMF_CLEAR)))
																strcpy(ieb->Title,newname);
														}
														if (ieb->filename)
														{
															FreeVec(ieb->filename);
															if ((ieb->filename = AllocVec(strlen(newname) + 2,MEMF_ANY | MEMF_CLEAR)))
																strcpy(ieb->filename,newname);
														}
														if (Write(cport->currentdir->fh_files, item, sizeof(ItemEntry)) != sizeof(ItemEntry))
															result = IoErr();
													}
													else
														result = IoErr();
												}
												else
													result = ERROR_OUT_OF_MEMORY;
											}
											else
												result = IoErr();
											founded = TRUE;
										}
									}
									FreeVec(item);
								}
								else
									result = ERROR_OUT_OF_MEMORY;
							}
							else
								result = ERROR_OBJECT_NOT_FOUND;
						}
						else
							result = ERROR_OBJECT_NOT_FOUND;
						FreeVec(path);
						break;
					case 0:
						result = ERROR_INVALID_OBJECT_TYPE;
						break;
				}

			}
			else
				result = ERROR_OUT_OF_MEMORY;
		}
		else
			result = ERROR_OBJECT_NOT_FOUND;
	}
	else
		result = ERROR_NOT_A_NOVIA_DOS_PROGRAMM;
	return result;
}

struct ItemEntryBlock * __saveds ASM nfs_support_get_ieb(char *name, PortData *cport)
{
	ULONG IDNumber	= 0;
	struct ItemEntryBlock *ieb		= (ItemEntryBlock *)cport->itementrylist.lh_Head;
	struct ItemEntryBlock *result = FALSE;
	if (name && ieb->ln_Succ && ieb->ln_Pred && strlen(name))
	{
		StrToLong(name, (LONG *)&IDNumber);
		if (IDNumber)
		{
			while (!result && ieb->ln_Succ)
			{
				if (ieb->IDNumber == IDNumber)
					result = ieb;
				else
					ieb = ieb->ln_Succ;
			}
		}
		else
		{
			while (!result && ieb->ln_Succ)
			{
				if (ieb->Title)
				{
					if (!Stricmp(ieb->Title, name))
						result = ieb;
				}
				ieb = ieb->ln_Succ;
			}
		}
	}
	return (result);
}



/*
	#define ERROR_NO_FREE_STORE				103
	#define ERROR_TASK_TABLE_FULL				105
	#define ERROR_BAD_TEMPLATE					114
	#define ERROR_BAD_NUMBER					115
	#define ERROR_REQUIRED_ARG_MISSING		116
	#define ERROR_KEY_NEEDS_ARG				117
	#define ERROR_TOO_MANY_ARGS				118
	#define ERROR_UNMATCHED_QUOTES			119
	#define ERROR_LINE_TOO_LONG				120
	#define ERROR_FILE_NOT_OBJECT				121
	#define ERROR_INVALID_RESIDENT_LIBRARY	122
	#define ERROR_NO_DEFAULT_DIR				201
	#define ERROR_OBJECT_IN_USE				202
	#define ERROR_OBJECT_EXISTS				203
	#define ERROR_DIR_NOT_FOUND				204
	#define ERROR_OBJECT_NOT_FOUND			205
	#define ERROR_BAD_STREAM_NAME				206
	#define ERROR_OBJECT_TOO_LARGE			207
	#define ERROR_ACTION_NOT_KNOWN			209
	#define ERROR_INVALID_COMPONENT_NAME	210
	#define ERROR_INVALID_LOCK					211
	#define ERROR_OBJECT_WRONG_TYPE			212
	#define ERROR_DISK_NOT_VALIDATED			213
	#define ERROR_DISK_WRITE_PROTECTED		214
	#define ERROR_RENAME_ACROSS_DEVICES		215
	#define ERROR_DIRECTORY_NOT_EMPTY		216
	#define ERROR_TOO_MANY_LEVELS				217
	#define ERROR_DEVICE_NOT_MOUNTED			218
	#define ERROR_SEEK_ERROR					219
	#define ERROR_COMMENT_TOO_BIG				220
	#define ERROR_DISK_FULL						221
	#define ERROR_DELETE_PROTECTED			222
	#define ERROR_WRITE_PROTECTED				223
	#define ERROR_READ_PROTECTED				224
	#define ERROR_NOT_A_DOS_DISK				225
	#define ERROR_NO_DISK						226
	#define ERROR_NO_MORE_ENTRIES				232
	#define ERROR_IS_SOFT_LINK					233
	#define ERROR_OBJECT_LINKED				234
	#define ERROR_BAD_HUNK						235
	#define ERROR_NOT_IMPLEMENTED				236
	#define ERROR_RECORD_NOT_LOCKED			240
	#define ERROR_LOCK_COLLISION				241
	#define ERROR_LOCK_TIMEOUT					242
	#define ERROR_UNLOCK_ERROR					243
*/


LONG nfs_support_lowlevel_makefilesystem(struct PortData *cport)
{
	return(0);
}

LONG nfs_support_lowlevel_makeroot(struct PortData *cport)
{
	return(0);
}

LONG nfs_support_lowlevel_adddirectory(char *name, struct PortData *cport)
{
	return(0);
}

LONG nfs_support_lowlevel_addlink(char *name, struct PortData *cport)
{
	return(0);
}


LONG nfs_support_lowlevel_addcommand(char *name, struct PortData *cport)
{
	LONG result = 0;
	char *buffer;
	char *fullpath = cport->currentdir->iib.fullpath;
	ULONG length_fp = strlen(fullpath);

	if (name && strlen(name) && fullpath && length_fp)
	{
		if ((buffer = AllocVec(strlen(name) + length_fp + 10,MEMF_ANY)))	// for full path
		{
			BPTR fh;
			if (fullpath[length_fp - 1] == ':' | fullpath[length_fp - 1] == '/')
				sprintf(buffer,"%s%s",fullpath,name);
			else
				sprintf(buffer,"%s/%s",fullpath,name);
			if ((fh = Open(buffer, MODE_OLDFILE)))
			{
				struct CommandEntry *ce;
				if ((ce = AllocVec(sizeof(struct CommandEntry), MEMF_ANY|MEMF_CLEAR)))
				{
					SetIPCounter64(&ce->iib.IPNumber64);
					ce->iib.ItemType		= ITEMTYPE_COMMAND;
					GetTime(&ce->iib.LastChange);
					GetTime(&ce->iib.CreateDate);

					Seek(fh, 0, OFFSET_END);
					ce->iib.Size			= (ULONG) Seek(fh, 0, OFFSET_END);
					ce->iib.Protection	= (FIBF_READ | FIBF_WRITE | FIBF_EXECUTE | FIBF_DELETE);
					ce->iib.CreateUserIP		= cport->LocalUser.IPNumber;
					ce->iib.LastChangeUserIP= cport->LocalUser.IPNumber;
					strncpy(ce->iib.Title,		name, 127);
					strncpy(ce->iib.Filename,	name, 107);
					
					GetTime(&ce->SendDate);
					strncpy(ce->FromRealName,	cport->LocalUser.RealName, 39);
					strncpy(ce->From,				cport->LocalUser.Handle,	79);
					if (!cport->currentdir->fh_files)
					{
						sprintf(buffer,"%s/.files",fullpath);
						cport->currentdir->fh_files = Open(buffer,MODE_READWRITE);
					}
					if (cport->currentdir->fh_files)
					{
						Seek(cport->currentdir->fh_files, 0, OFFSET_END);
						Write(cport->currentdir->fh_files, ce, sizeof(CommandEntry));
					}
					FreeVec(ce);
				}
				else
					result = ERROR_OUT_OF_MEMORY;
				Close(fh);
			}
			else
				result = ERROR_OBJECT_NOT_FOUND;
			FreeVec(buffer);
		}
		else
			result = ERROR_OUT_OF_MEMORY;
	}
	else
		result = ERROR_INVALID_OBJECT_NAME;
	return(result);
}

void SetIPCounter64(ULONG *ptr)
{
	ULONG *counter64 = ptr;
	ULONG *counter	  = ptr + 1;
	BPTR fh;
	if (mainportconfig->MessageIPCounter < 0xFFFFFFFF)
	{
		mainportconfig->MessageIPCounter++;
	}
	else
	{
		mainportconfig->MessageIPCounter = 0;
		mainportconfig->MessageIPCounter64++;
	}
	*counter64	= mainportconfig->MessageIPCounter64;
	*counter		= mainportconfig->MessageIPCounter;
	if ((fh = Open("novia:sysdata/counter/ipcounter", MODE_READWRITE)))
	{
		Write(fh, &mainportconfig->MessageIPCounter64, 8);
		Close(fh);
	}
}


void getuser_from_host(char *src, char *username)
{
	char *ptr			= src;
	char *usernameptr	= username;
	while (*ptr && *ptr !='@')
		ptr++;
	if (*ptr)
	{
		while (ptr >= src && uucp_char[*ptr])
			ptr--;
		if (!uucp_char[*ptr])
			ptr++;
		while (*ptr && *ptr !='@')
			*usernameptr++ = *ptr++;
		*usernameptr = 0;
	}
}


struct INet_User_Address *Create_INet_User(char *buffer)
{
	struct INet_User_Address *user;
	if ((user = AllocVec(sizeof(INet_User_Address), MEMF_ANY | MEMF_CLEAR)))
	{
		char realname[80];
		char username[128];
		ULONG realnamesize, usernamesize;
		CopyUUCPName(buffer, realname, username);
		realnamesize = strlen(realname);
		usernamesize = strlen(username);
		if (realnamesize)
		{
			if ((user->RealName = AllocVec(realnamesize + 2, MEMF_ANY | MEMF_CLEAR)))
				strncpy(user->RealName, realname, realnamesize);
		}
		if (usernamesize)
		{
			if ((user->UserName = AllocVec(usernamesize + 2, MEMF_ANY | MEMF_CLEAR)))
				strncpy(user->UserName, username, usernamesize);
		}
	}
	return(user);
}