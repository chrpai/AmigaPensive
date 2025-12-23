#include <exec/types.h>
#include <exec/memory.h>
#include <stdlib.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <dos/dos.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <novia/novia_nsl.h>
#include <pragma/noviasys_lib.h>
#include <novia/novia_portdata.h>
#include <novia/novia_userdata.h>

#ifndef NOVIA_REGISTERS_H
#include <novia/novia_registers.h>
#endif

//#define SCREENWIDTH cport->LocalUser.TermWidth
#define SCREENWIDTH 80

extern char editfield(char *bp, char *src, char maxlen, char fieldlen, ULONG flags, ULONG sigs);
extern MainPortConfig *mainportconfig;

BPTR mulder;					// filehandle
char buffer[256];
char *ptr;
char programend = FALSE;	// global exit
ULONG linecounter	= 0;

List variablelist;
List structurelist;;

__saveds ASM LONG nslib_execute_nsl(register __a0 char *name);

int LoadStructureKeys(struct StructureList *sl, char *filename);
void initglobal();
void freeglobal();

LONG getcommandid(char *string);
int nslcmd_print(char *string);
struct NSLVariable *getvariable(char *name);
struct NSLVariable *addvariable(char *name);
StructureEntry *searchuserkey(StructureList *sl, char *name);
int myisalpha(char c);
struct NSLMenu *CreateMenu(NSLVariable *var);

struct NSLMenuItem *CreateMenuEntry_PRINT();
struct NSLMenuItem *CreateMenuEntry_NULL();
struct NSLMenuItem *CreateMenuEntry_FIELD();
struct NSLMenuItem *CreateMenuEntry_TEXT();
struct NSLMenuItem *CreateMenuEntry_LINK();
struct NSLMenuItem *CreateMenuEntry_RETURN();

BOOL islink(ULONG nsltype);
void jump_menu(NSLMenu *);
void update_item_data(NSLMenuItem *item);
void build_menu(NSLMenu *nslmenu);

LONG returncode = NULL;

int GetNextGraph()
{
	int result = FALSE;
	char quit = FALSE;
	while (!quit && !programend)
	{
		if (!*ptr | *ptr == 10 | *ptr == 13)		// next line;
		{
			if (FGets(mulder, buffer, 256))
			{
				linecounter++;
				ptr = buffer;
				while ( *ptr && !isgraph(*ptr) && *ptr != 10 && *ptr != 13)
					ptr++;
				if (isgraph(*ptr))
					quit = TRUE;
			}
			else
				programend = TRUE;
		}
		else
		{
			while (isgraph(*ptr) && *ptr && *ptr != 10 && *ptr != 13)
				ptr++;

			if (isgraph(*ptr))
			{
				if (*ptr == ';')
				{
					*ptr = 0;			// comment. jump to next line;
				}
				else
				{
					quit = TRUE;
				}
			}
			else
			{
				while (!isgraph(*ptr) && *ptr && *ptr != 10 && *ptr != 13)
					ptr++;
				if (isgraph(*ptr))
				{
					quit = TRUE;
				}
				else
				{
					*ptr = 0;
				}
			}
		}
	}
	if (!programend)
		result = TRUE;
	return result;
}


__saveds ASM LONG nslib_execute_nsl(register __a0 char *name)
{
	struct PortData *cport = (PortData *)FindTask(NULL)->tc_UserData;
	char acmd[256];				// actualcommand
	char *acmdptr;					// pointer for line tracing
	struct NSLVariable *avar;	// actualvariable
	char quit;
	ULONG counter;

	programend = FALSE;	// global exit
	linecounter	= 0;

	ioprintf("inin\n");
	initglobal();
	if ((mulder = Open(name, MODE_OLDFILE)))
	{
		NSLVariable *var;
		ptr = buffer;
		*ptr = 0;

		while (!programend && !cport->ProgramClose && cport->network.online)
		{
			while (GetNextGraph())
			{
				quit	= FALSE;
				*acmd = 0;
				if (myisalpha(*ptr))							// check for keyword
				{
					acmdptr = acmd;
					while (myisalpha(*ptr))
						*acmdptr++ = *ptr++;
					*acmdptr = 0;
					switch(getcommandid(acmd))
					{
						case NSL_TYPE_COMMENT:
							while (*ptr && *ptr != 10 && *ptr != 13)
								ptr++;
							break;
						case NSLCOMMAND_PRINT:
							ptr = ptr + nslcmd_print(ptr);
							break;
						case NSLCOMMAND_EXIT:
							programend = TRUE;
							break;
						case NSLCOMMAND_MENU:
							break;
						case NSLCOMMAND_JUMP:
							acmdptr = acmd;
							while (*ptr && !myisalpha(*ptr))
								ptr++;
							while (myisalpha(*ptr))
								*acmdptr++ = *ptr++;
							*acmdptr = 0;
							if ((var = getvariable(acmd)))
							{
								if (var->nsltype == NSL_TYPE_MENU && var->data)
									jump_menu((NSLMenu *)var->data);
							}
							break;
						case NSLCOMMAND_IF:
							acmdptr = acmd;
							while (*ptr && !myisalpha(*ptr)) // jump to variable
								ptr++;

							while (myisalpha(*ptr))
								*acmdptr++ = *ptr++;
							*acmdptr = 0;

							if ((var = getvariable(acmd)))
							{
								ioprintf("ptr2:%s\n",ptr);
								while (isspace(*ptr))
									ptr++;
								ioprintf("ptr2:%s\n",ptr);
								if (*ptr == '=' && isspace(ptr[1]))
								{
									ptr++;
									while (*ptr && !isgraph(*ptr))
										ptr++;
									acmdptr = acmd;
									while (isgraph(*ptr))
										*acmdptr++ = *ptr++;
									*acmdptr = 0;
									if (getcommandid(acmd) == NSL_RESULT_TRUE)
									{
										if (var->nsltype == NSL_TYPE_MENU)
										{
											if (var->result)
											{
												while (isgraph(*ptr))
													ptr++;
												while (isspace(*ptr))
													ptr++;
												acmdptr = acmd;
												while (isgraph(*ptr))
													*acmdptr++ = *ptr++;
												*acmdptr = 0;
												if (getcommandid(acmd) == NSLCOMMAND_THEN)
												{
													while (isgraph(*ptr))
														ptr++;
													while (isspace(*ptr))
														ptr++;
													acmdptr = acmd;
													while (*ptr && *ptr != '(')
														*acmdptr++ = *ptr++;
													*acmdptr = 0;
													if (getcommandid(acmd) == NSLCOMMAND_SAVECOMMANDENTRY)
													{
														while (isgraph(*ptr) && *ptr != '(')
															ptr++;
														if (*ptr == '(')
														{
															ptr++;
															acmdptr = acmd;
															while (*ptr && *ptr != ')')
																*acmdptr++ = *ptr++;
															*acmdptr = 0;
															if ((var = getvariable(acmd)))
															{
																if (var->nsltype == NSL_TYPE_STRUCT)
																{
																	if (!(ndos_save_ndos_object(cport->argstring[1], (APTR)var->data)))
																	{
																	}
																	else
																	{
																		programend = TRUE;
																		ioprintf("programerror 23 in line: %d <%s>.\n",linecounter,ptr);
																	}
																}
																else
																{
																	programend = TRUE;
																	ioprintf("programerror 23.1 in line: %d <%s>.\n",linecounter,ptr);
																}
															}
															else
															{
																programend = TRUE;
																ioprintf("programerror 23.2 in line: %d <%s>.\n",linecounter,ptr);
															}
														}
														else
														{
															programend = TRUE;
															ioprintf("programerror 23.3 in line: %d <%s>.\n",linecounter,ptr);
														}
													}
													else if (getcommandid(acmd) == NSLCOMMAND_SAVEUSER)
													{
														while (isgraph(*ptr) && *ptr != '(')
															ptr++;
														if (*ptr == '(')
														{
															ptr++;
															acmdptr = acmd;
															while (*ptr && *ptr != ')')
																*acmdptr++ = *ptr++;
															*acmdptr = 0;
															if ((var = getvariable(acmd)))
															{
																if (var->nsltype == NSL_TYPE_STRUCT)
																{
																	SaveUser(&cport->EditUser);
																}
																else
																{
																	programend = TRUE;
																	ioprintf("programerror 23.1 in line: %d <%s>.\n",linecounter,ptr);
																}
															}
															else
															{
																programend = TRUE;
																ioprintf("programerror 23.2 in line: %d <%s>.\n",linecounter,ptr);
															}
														}
														else
														{
															programend = TRUE;
															ioprintf("programerror 23.3 in line: %d <%s>.\n",linecounter,ptr);
														}
													}
													else
													{
														programend = TRUE;
														ioprintf("programerror 23.4 in line: %d <%s>.\n",linecounter,ptr);
													}
												}
												else
												{
													programend = TRUE;
													ioprintf("programerror 23.5 in line: %d <%s>.\n",linecounter,ptr);
												}
											}
											else
												{
												programend = TRUE;
												ioprintf("programerror 23.6 in line: %d <%s>.\n",linecounter,ptr);
											}
										}
										else
										{
											programend = TRUE;
											ioprintf("programerror 23.7 in line: %d <%s>.\n",linecounter,ptr);
										}
									}
									else
									{
										programend = TRUE;
										ioprintf("programerror 23.8 in line: %d <%s>.\n",linecounter,ptr);
									}
								}
								else
								{
									programend = TRUE;
									ioprintf("programerror 23.9 in line: %d <%s>.\n",linecounter,ptr);
								}
							}
							else
								programend = TRUE;
							break;
						case NSLCOMMAND_UNDEFINED:
							{
								struct NSLVariable *var;
								var = getvariable(acmd);
								while (*ptr && *ptr != 10 && *ptr != 13 && *ptr != ':' && *ptr !='=')
									*ptr++;
								if (*ptr == ':')							// Label
								{
									if (var)
									{
										ioprintf("ERROR 100, in line: %d -> Label %s already defined.\n",linecounter,acmd);
									}
									else
									{
										if ((var = addvariable(acmd)))
										{
											ioprintf("addviarable: %s\n",acmd);
											var->nsltype	= NSL_TYPE_LABEL;
											var->data		= linecounter;
										}
									}
								}
								else
								{
									if (isgraph(*ptr))
									{
										if (*ptr == '=')
										{
											while (isgraph(*ptr))
												ptr++;
											if (!var)
												var = addvariable(acmd);
											if (var)
											{
												while (*ptr && *ptr != 10 && *ptr != 13 && !isgraph(*ptr))
													ptr++;
												if (*ptr && *ptr != 10 && *ptr != 13)
												{
													if (myisalpha(*ptr))
													{
														StructureList *sl;
														NSLMenu *nslmenu;
														acmdptr = acmd;
														while (myisalpha(*ptr))
															*acmdptr++ = *ptr++;
														*acmdptr = 0;
														switch (getcommandid(acmd))
														{
															case NSLCOMMAND_MENU:
																ioprintf("Create Menu: %d\n",linecounter);
																var->nsltype = NSL_TYPE_MENU;
																if (nslmenu = CreateMenu(var))
																{
																	nslmenu->title = var->name;
																}
																break;
															case NSLCOMMAND_LOADUSER:
																var->nsltype = NSL_TYPE_STRUCT;
																sl = (StructureList *)&structurelist.lh_Head;
																while (sl && sl->ln_Succ)
																{
																	if (sl->sl_id == NSL_SLID_USER)
																		var->sl = sl;
																	sl = sl->ln_Succ;
																}
																while (*ptr && (*ptr == '(' | isspace(*ptr)))
																	ptr++;
																if (myisalpha(*ptr))
																{
																	acmdptr = acmd;
																	while (*ptr && myisalpha(*ptr))
																		*acmdptr++ = *ptr++;
																	*acmdptr = 0;
																	if (getcommandid(acmd) == NSL_TYPE_ARGSTRING)
																	{
																		ULONG userid = NULL;
																		if ((userid = FindUser(cport->argstring[1])))
																		{
																			if (LoadUser(&cport->EditUser,userid))
																			{
																				var->data = (ULONG) &cport->EditUser;
																			}
																			else
																			{
																				programend = TRUE;
																				ioprintf("programerror 33 in line: %d, can't load user\n",linecounter);
																			}
																		}
																		else
																		{
																			programend = TRUE;
																			ioprintf("programerror 34 in line: %d, can't get userid\n",linecounter);
																		}
																	}
																	else
																	{
																		programend = TRUE;
																		ioprintf("programerror 35 in line: %d, empty argument string\n",linecounter);
																	}
																}
																break;
															case NSLCOMMAND_LOADMAINCONFIG:
																var->nsltype = NSL_TYPE_STRUCT;
																sl = (StructureList *)&structurelist.lh_Head;
																while (sl && sl->ln_Succ)
																{
																	if (sl->sl_id == NSL_SLID_MAINCONFIG)
																		var->sl = sl;
																	sl = sl->ln_Succ;
																}
																while (*ptr && (*ptr == ')' | isspace(*ptr)))
																	ptr++;
																var->data = (ULONG) mainportconfig;
																break;

															case NSLCOMMAND_LOADCOMMANDENTRY:

																var->nsltype = NSL_TYPE_STRUCT;
																sl = (StructureList *)&structurelist.lh_Head;
																while (sl && sl->ln_Succ)
																{
																	if (sl->sl_id == NSL_SLID_COMMANDENTRY)
																		var->sl = sl;
																	sl = sl->ln_Succ;
																}
																while (*ptr && (*ptr == '(' | isspace(*ptr)))
																	ptr++;
																if (myisalpha(*ptr))
																{
																	acmdptr = acmd;
																	while (*ptr && myisalpha(*ptr))
																		*acmdptr++ = *ptr++;
																	*acmdptr = 0;
																	if (getcommandid(acmd) == NSL_TYPE_ARGSTRING)
																	{
																		struct CommandEntry *ce;
																		if ((ce = AllocVec(sizeof(CommandEntry), MEMF_ANY | MEMF_CLEAR)))
																		{
																			LONG doserror;
																			if (!(doserror = ndos_load_ndos_object(cport->argstring[1], ce)))
																			{
																				printf("command entry->structure: %x\n",ce);
																				var->data = (ULONG)ce;
																			}
																			else
																			{
																				programend = TRUE;
																				ioprintf("programerror 23 in line: %d, %s.\n",linecounter,getstr(150,(UWORD)doserror));
																			}
																		}
																		else
																		{
																			programend = TRUE;
																			ioprintf("programerror 24 in line: %d, out of memory\n",linecounter);
																		}
																	}
																	else
																	{
																		programend = TRUE;
																		ioprintf("programerror 25 in line: %d, empty argument string\n",linecounter);
																	}
																}
																break;
														}
													}
												}
											}
										}
										else
										{
											ioprintf("ptr: <%s>\n",ptr);
										}
									}
									else
									{
										ioprintf("programerror 26 in line: %d, <%s>\n",linecounter,ptr);
										programend = TRUE;
									}
								}
							}
							break;
					}
				}
				else
					ioprintf("warning dfgfd in line: %d\n",linecounter);
			}
			linecounter++;
		}
		Close(mulder);
	}
	else
	{
		returncode = ERROR_OBJECT_NOT_FOUND;
		ioprintf("can't open nsl-program: %s\n",name);
	}
	freeglobal();
	ioprintf("c3z0\n");
	return(returncode);
}

LONG getcommandid(char *string)
{
	if (!strcmp(string,"FALSE"))									return NSL_RESULT_FALSE;
	if (!strcmp(string,"TRUE"))									return NSL_RESULT_TRUE;

	if (*string == ';')												return NSL_TYPE_COMMENT;

	if (!strcmp(string,"PRINT"))									return NSLCOMMAND_PRINT;
	if (!strcmp(string,"EXIT"))									return NSLCOMMAND_EXIT;
	if (!strcmp(string,"MENU"))									return NSLCOMMAND_MENU;
	if (!strcmp(string,"JUMP"))									return NSLCOMMAND_JUMP;
	if (!strcmp(string,"PROCEDURE"))								return NSLCOMMAND_PROCEDURE;
	if (!strcmp(string,"IF"))										return NSLCOMMAND_IF;
	if (!strcmp(string,"THEN"))									return NSLCOMMAND_THEN;
	if (!strcmp(string,"ELSE"))									return NSLCOMMAND_ELSE;
	if (!strcmp(string,"LOADUSER"))								return NSLCOMMAND_LOADUSER;
	if (!strcmp(string,"SAVEUSER"))								return NSLCOMMAND_SAVEUSER;
	if (!strcmp(string,"LOADMAINCONFIG"))						return NSLCOMMAND_LOADMAINCONFIG;
	if (!strcmp(string,"LOADCOMMANDENTRY"))					return NSLCOMMAND_LOADCOMMANDENTRY;
	if (!strcmp(string,"SAVECOMMANDENTRY"))					return NSLCOMMAND_SAVECOMMANDENTRY;
	if (!strcmp(string,"MENUTAG_COLS"))							return NSL_MENUTAG_COLS;
	if (!strcmp(string,"MENUTAG_AUTOFORMAT"))					return NSL_MENUTAG_AUTOFORMAT;
	if (!strcmp(string,"FIELD"))									return NSL_TYPE_FIELD;
	if (!strcmp(string,"TEXT"))									return NSL_TYPE_TEXT;
	if (!strcmp(string,"LINK"))									return NSL_TYPE_LINK;
	if (!strcmp(string,"RETURN"))									return NSL_TYPE_RETURN;
	if (!strcmp(string,"NULL"))									return NSL_TYPE_NULL;
	if (!strcmp(string,"ARGSTRING"))								return NSL_TYPE_ARGSTRING;

	if (!strcmp(string,"NSL_TYPE_ACCESSGROUP"))				return NSL_TYPE_ACCESSGROUP;
	if (!strcmp(string,"NSL_TYPE_ANSICOLORS"))				return NSL_TYPE_ANSICOLORS;
	if (!strcmp(string,"NSL_TYPE_ANSIMODE"))					return NSL_TYPE_ANSIMODE;
	if (!strcmp(string,"NSL_TYPE_ANSITABS"))					return NSL_TYPE_ANSITABS;
	if (!strcmp(string,"NSL_TYPE_ARCMETHOD"))					return NSL_TYPE_ARCMETHOD;
	if (!strcmp(string,"NSL_TYPE_BOOL"))						return NSL_TYPE_BOOL;
	if (!strcmp(string,"NSL_TYPE_CHARSET"))					return NSL_TYPE_CHARSET;
	if (!strcmp(string,"NSL_TYPE_COMPUTERTYPE"))				return NSL_TYPE_COMPUTERTYPE;
	if (!strcmp(string,"NSL_TYPE_COUNTRY"))					return NSL_TYPE_COUNTRY;
	if (!strcmp(string,"NSL_TYPE_DATE"))						return NSL_TYPE_DATE;
	if (!strcmp(string,"NSL_TYPE_DIRECTORYPATH"))			return NSL_TYPE_DIRECTORYPATH;
	if (!strcmp(string,"NSL_TYPE_EOLSEQUENCE"))				return NSL_TYPE_EOLSEQUENCE;
	if (!strcmp(string,"NSL_TYPE_GENDER"))						return NSL_TYPE_GENDER;
	if (!strcmp(string,"NSL_TYPE_HELPLEVEL"))					return NSL_TYPE_HELPLEVEL;
	if (!strcmp(string,"NSL_TYPE_LANGUAGE"))					return NSL_TYPE_LANGUAGE;
	if (!strcmp(string,"NSL_TYPE_LINEFEEDS"))					return NSL_TYPE_LINEFEEDS;
	if (!strcmp(string,"NSL_TYPE_MOREMODE"))					return NSL_TYPE_MOREMODE;
	if (!strcmp(string,"NSL_TYPE_PHONENUMBER"))				return NSL_TYPE_PHONENUMBER;
	if (!strcmp(string,"NSL_TYPE_PHONEVERIFICATION"))		return NSL_TYPE_PHONEVERIFICATION;
	if (!strcmp(string,"NSL_TYPE_PROTOCOL"))					return NSL_TYPE_PROTOCOL;
	if (!strcmp(string,"NSL_TYPE_STRING"))						return NSL_TYPE_STRING;
	if (!strcmp(string,"NSL_TYPE_TIMEFORMAT"))				return NSL_TYPE_TIMEFORMAT;
	if (!strcmp(string,"NSL_TYPE_TIMEZONE"))					return NSL_TYPE_TIMEZONE;
	if (!strcmp(string,"NSL_TYPE_UBYTE"))						return NSL_TYPE_UBYTE;
	if (!strcmp(string,"NSL_TYPE_ULONG"))						return NSL_TYPE_ULONG;
	if (!strcmp(string,"NSL_TYPE_USERID"))						return NSL_TYPE_USERID;
	if (!strcmp(string,"NSL_TYPE_UWORD"))						return NSL_TYPE_UWORD;
	if (!strcmp(string,"NSL_TYPE_GROUPBOOL"))					return NSL_TYPE_GROUPBOOL;
	if (!strcmp(string,"NSL_TYPE_DATEFORMAT"))				return NSL_TYPE_DATEFORMAT;
	if (!strcmp(string,"NSL_TYPE_STRUCT"))						return NSL_TYPE_STRUCT;
	if (!strcmp(string,"NSL_TYPE_FILENAME"))					return NSL_TYPE_FILENAME;
	if (!strcmp(string,"NSL_TYPE_DOMAIN"))						return NSL_TYPE_DOMAIN;
	if (!strcmp(string,"NSL_TYPE_INETADDRESS"))				return NSL_TYPE_INETADDRESS;
	if (!strcmp(string,"NSL_TYPE_INETSERVICEPORT"))			return NSL_TYPE_INETSERVICEPORT;
	if (!strcmp(string,"NSL_TYPE_INETUSERLIST"))				return NSL_TYPE_INETUSERLIST;
	if (!strcmp(string,"NSL_TYPE_INETSERVERSTATUS"))		return NSL_TYPE_INETSERVERSTATUS;
	if (!strcmp(string,"NSL_TYPE_INETMAILUSERTYPE"))		return NSL_TYPE_INETMAILUSERTYPE;
	if	(!strcmp(string,"NSL_TYPE_INETUSERNAME"))				return NSL_TYPE_INETUSERNAME;
	if	(!strcmp(string,"NSL_TYPE_IPADDRESS"))					return NSL_TYPE_IPADDRESS;
	if	(!strcmp(string,"NSL_TYPE_SCREENMODE"))				return NSL_TYPE_SCREENMODE;
	if	(!strcmp(string,"NSL_TYPE_FONTNAME"))					return NSL_TYPE_FONTNAME;
	if	(!strcmp(string,"NSL_TYPE_CLIENTLIST"))				return NSL_TYPE_CLIENTLIST;
	if	(!strcmp(string,"NSL_TYPE_LOCALMAILSERVERLIST"))	return NSL_TYPE_LOCALMAILSERVERLIST;
	if	(!strcmp(string,"NSL_TYPE_POP3SERVERLIST"))			return NSL_TYPE_POP3SERVERLIST;
	if	(!strcmp(string,"NSL_TYPE_EVENTLIST"))					return NSL_TYPE_EVENTLIST;
	if (!strcmp(string,"NSL_TYPE_USERIP"))						return NSL_TYPE_USERIP;
	if (!strcmp(string,"NSL_TYPE_ITEMTYPE"))					return NSL_TYPE_ITEMTYPE;
	if (!strcmp(string,"NSL_TYPE_PROTECTION"))				return NSL_TYPE_PROTECTION;

//	if (!strcmp(string,""))						return
	return NSLCOMMAND_UNDEFINED;
}


int nslcmd_print(char *string)
{
	char *ptr = string;
	char quit = FALSE;
	char iobuffer[256];
	char *ioptr = iobuffer;
	while (!quit)
	{
		if (*ptr == '"')															// detected "string"
		{
			*ptr++;
			while (*ptr && *ptr != 10 && *ptr != 13 && *ptr != '"')	// wait for second " or lineend
				*ioptr++ = *ptr++;												// copy string in iobuffer

			if (*ptr == '"')														// check of second "
				*ptr++;
			*ioptr++ = ' ';
		}
		else
		{
			if (isdigit(*ptr))													// check for integer
			{
				while (isdigit(*ptr) | *ptr == '.' | *ptr == 44)		// ascii for , is 44
				{
					if (*ptr == ',')
					{
						*ioptr++ = '.';											// convert , in .
						*ptr++;
					}
					else
						*ioptr++ = *ptr++;
				}
				*ioptr++ = ' ';
			}
			else
			{
				if (!*ptr | *ptr == 10 | *ptr == 13 | *ptr == ';')		// check end of line or next command
					quit = TRUE;
				else
				{
					if (myisalpha(*ptr))
					{
						char varname[100];
						char *varnameptr = varname;
						struct NSLVariable *var;
						while (myisalpha(*ptr))
							*varnameptr++ = *ptr++;
						*varnameptr = 0;
						var = getvariable(varname);
						if (var)
						{
							switch (var->nsltype)
							{
								case NSL_TYPE_ULONG: case NSL_TYPE_UWORD: case NSL_TYPE_UBYTE:
									sprintf(ioptr,"%u",var->data);
									ioptr = ioptr + strlen(ioptr);
									break;
								case NSL_TYPE_NOTDEFINED:
									strcat(ioptr,"<>");
									ioptr++;
									break;
								case NSL_TYPE_STRING:
									strcpy(ioptr,(char *)var->data);
									ioptr = ioptr + strlen(ioptr);
									break;
							}
						}
					}
					else
					{
						*ptr++;
					}
				}
			}
		}
	}
	*ioptr = 0;
	ioprintf("stdout: %s\n",iobuffer);
	return(ptr - string);
}


struct NSLVariable *getvariable(char *name)
{
	struct NSLVariable *result = NULL;
	struct NSLVariable *var;

	var = (NSLVariable *)variablelist.lh_Head;		// get first variable

	while (var->ln_Succ && !result)
	{
		if (!strcmp(var->name, name))
		{
			result = var;
		}
		else
			var = var->ln_Succ;
	}
	if (result)
	{
		if (!strcmp(name, "TIME"))
		{
			char *ctimeout;
			time_t t = time(0);
			ctimeout = ctime(&t);
			if ((result->data = (ULONG) AllocVec(strlen(ctimeout), MEMF_ANY|MEMF_CLEAR))) // allocate memory for var->data
			{
				/* copy global-timestring into var->data. ctime hangs a CR on the global timestring.
					Is this normaly ?? Or a nice bug of my MaxonC++ Compiler ?? */

				strncpy((APTR)result->data, ctimeout, strlen(ctimeout) - 1);
			}
		}
	}

	return result;
}

struct NSLVariable *addvariable(char *name)
{
	struct NSLVariable *var = NULL;
	if ((var = AllocVec(sizeof(struct NSLVariable), MEMF_ANY | MEMF_CLEAR)))
	{
		if ((var->name = AllocVec(strlen(name) + 1, MEMF_ANY | MEMF_CLEAR)))
		{
			strcpy(var->name, name);
			AddTail(&variablelist, (Node *)var);
		}
		else
		{
			FreeVec(var);
			var = NULL;
		}
	}
	return var;
}

int myisalpha(char c)
{
	if (isalpha(c) |  c == '_')
		return TRUE;
	else
		return FALSE;
}


int LoadStructureKeys(struct StructureList *sl, char *filename)
{
	int result = NULL;
	BPTR fh;
	NewList((List *)&sl->se_list);
	if ((fh = Open(filename, MODE_OLDFILE)))
	{
		char line[256];
		char *lineptr;
		char *septr;
		struct StructureEntry *se;
		while (FGets(fh, line, 256) && !programend)
		{
			if ((se = AllocVec(sizeof(StructureEntry), MEMF_ANY | MEMF_CLEAR)))
			{
				lineptr	= line;
				septr		= se->se_name;
				
				while (!isgraph(*lineptr))
					lineptr++;

				/* get se_name */

				while (*lineptr && isgraph(*lineptr))
					*septr++ = *lineptr++;

				/* jump to offset */

				while (*lineptr && !isdigit(*lineptr))
					lineptr++;

				/* test for offset */

				if (isdigit(*lineptr))
				{

					/* get offset */

					lineptr = lineptr + StrToLong(lineptr, (LONG *)&se->se_offset);

					/* als naechstes offseteintrag (isdigit) überspringen und zur fieldlen springen.
						wegen eines Bugs in StrToLong liefert StringToLong vor AmigaOS V39 nicht die Anzahl
						der konvertierten zahlen zurück, sondern die tabulatoren. */

					while (isdigit(*lineptr))					// jump to end of offset-digit-string.
						lineptr++;

					while (*lineptr && !isdigit(*lineptr))	// jump to first digit of fieldlen
						lineptr++;

					if (isdigit(*lineptr))						// teste ob das naechste Zeichen eine Zahl ist.
					{
						lineptr = lineptr + StrToLong(lineptr, (LONG *)&se->se_length);	// get fieldlen

						/* springe zum ersten buchstaben des ItemTypes */

						while (*lineptr && !isalpha(*lineptr))
							lineptr++;

						if (myisalpha(*lineptr))				// teste auf alphabetischen Zeichen.
						{

							/* kopiere itemtype bis zum ende der zeile */

							char *lineptr2 = lineptr;
							while (*lineptr2)
							{
								if (*lineptr2 == 10 | *lineptr2 == 13)
									*lineptr2 = 0;
								lineptr2++;
							}
							se->se_type = getcommandid(lineptr);	// get ItemType
							AddTail((List *)&sl->se_list, (Node *)se);
							result++;
						}
						else
						{
							programend = TRUE;
							ioprintf("add us failat 3:<%s> <%s>\n",lineptr,sl->sl_name);
						}
					}
					else
					{
						programend = TRUE;
						ioprintf("add us failat 2:<%s> <%s>\n",lineptr,sl->sl_name);
					}
				}
				else
				{
					programend = TRUE;
					ioprintf("add us failat 1:<%s> <%s>\n",lineptr,sl->sl_name);
				}
			}
			else
			{
				programend = TRUE;
				ioprintf("add us failat 0:<%s> <%s>\n",lineptr,sl->sl_name);
			}
		}
		Close(fh);
	}
	else
	{
		ioprintf("(failed)\ncan't open userkeys\n");
		programend = TRUE;
	}
	return result;
}

void initglobal()
{
	struct NSLVariable *var;
	struct StructureList *sl;

	NewList(&structurelist);
	NewList(&variablelist);

	if ((var = addvariable("TIME")))
	{
		var->nsltype = NSL_TYPE_STRING;
	}

	if ((sl = AllocVec(sizeof(StructureList), MEMF_ANY|MEMF_CLEAR)))
	{
		strcpy(sl->sl_name,"USER");
		sl->sl_id = NSL_SLID_USER;
		if (LoadStructureKeys(sl, "novia:sysdata/nsl/structures/user"))
		{
			AddTail((List *)&structurelist, (Node *)sl);
		}
		else
		{
			FreeVec(sl);
			programend = TRUE;
			ioprintf("can't open novia:sysdata/nsl/structures/user");
		}
	}

	if ((sl = AllocVec(sizeof(StructureList), MEMF_ANY|MEMF_CLEAR)))
	{
		strcpy(sl->sl_name,"MAINCONFIG");
		sl->sl_id = NSL_SLID_MAINCONFIG;
		if (LoadStructureKeys(sl, "novia:sysdata/nsl/structures/mainconfig"))
		{
			AddTail((List *)&structurelist, (Node *)sl);
		}
		else
		{
			FreeVec(sl);
			programend = TRUE;
			ioprintf("can't open novia:sysdata/nsl/structures/mainconfig");
		}
	}

	if ((sl = AllocVec(sizeof(StructureList), MEMF_ANY|MEMF_CLEAR)))
	{
		strcpy(sl->sl_name,"INETCONFIG");
		sl->sl_id = NSL_SLID_INETCONFIG;
		if (LoadStructureKeys(sl, "novia:sysdata/nsl/structures/inetconfig"))
		{
			AddTail((List *)&structurelist, (Node *)sl);
		}
		else
		{
			FreeVec(sl);
			programend = TRUE;
			ioprintf("can't open novia:sysdata/nsl/structures/inetconfig");
		}
	}

	if ((sl = AllocVec(sizeof(StructureList), MEMF_ANY|MEMF_CLEAR)))
	{
		strcpy(sl->sl_name,"LOCALMAILSERVER");
		sl->sl_id = NSL_SLID_LOCALMAILSERVER;
		if (LoadStructureKeys(sl, "novia:sysdata/nsl/structures/localmailserver"))
		{
			AddTail((List *)&structurelist, (Node *)sl);
		}
		else
		{
			FreeVec(sl);
			programend = TRUE;
			ioprintf("can't open novia:sysdata/nsl/structures/localmailserver");
		}
	}

	if ((sl = AllocVec(sizeof(StructureList), MEMF_ANY|MEMF_CLEAR)))
	{
		strcpy(sl->sl_name,"POP3SERVER");
		sl->sl_id = NSL_SLID_POP3SERVER;
		if (LoadStructureKeys(sl, "novia:sysdata/nsl/structures/pop3server"))
		{
			AddTail((List *)&structurelist, (Node *)sl);
		}
		else
		{
			FreeVec(sl);
			programend = TRUE;
			ioprintf("can't open novia:sysdata/nsl/structures/pop3server");
		}
	}

	if ((sl = AllocVec(sizeof(StructureList), MEMF_ANY|MEMF_CLEAR)))
	{
		strcpy(sl->sl_name,"INETMAILUSER");
		sl->sl_id = NSL_SLID_INETMAILUSER;
		if (LoadStructureKeys(sl, "novia:sysdata/nsl/structures/inetmailuser"))
		{
			AddTail((List *)&structurelist, (Node *)sl);
		}
		else
		{
			FreeVec(sl);
			programend = TRUE;
			ioprintf("can't open novia:sysdata/nsl/structures/inetmailuser");
		}
	}

	if ((sl = AllocVec(sizeof(StructureList), MEMF_ANY|MEMF_CLEAR)))
	{
		strcpy(sl->sl_name,"COMMANDENTRY");
		sl->sl_id = NSL_SLID_COMMANDENTRY;
		if (LoadStructureKeys(sl, "novia:sysdata/nsl/structures/commandentry"))
		{
			AddTail((List *)&structurelist, (Node *)sl);
		}
		else
		{
			FreeVec(sl);
			programend = TRUE;
			ioprintf("can't open novia:sysdata/nsl/structures/commandentry");
		}
	}
}

void freeglobal()
{
	struct NSLVariable *var;
	struct StructureEntry *se;
	struct StructureList  *sl;
	while (!IsListEmpty(&variablelist))
	{
		var = (NSLVariable *)variablelist.lh_Head;
		if (var->ln_Succ)
		{
			if (var->name)
				FreeVec(var->name);
			Remove((Node *)var);
			FreeVec(var);
		}
		var = var->ln_Succ;
	}

	while (!IsListEmpty(&structurelist))
	{
		sl = (StructureList *)structurelist.lh_Head;
		Remove((Node *)sl);
		while (!IsListEmpty((List *)&sl->se_list))
		{
			se = (StructureEntry *)sl->se_list.mlh_Head;
			Remove((Node *)se);
			FreeVec(se);
		}
		FreeVec(sl);
	}
}

struct NSLMenu *CreateMenu(NSLVariable *var)
{
	struct PortData *cport	= (struct PortData *)FindTask(NULL)->tc_UserData;
	char acmd[256];				// actualcommand
	char *acmdptr;					// pointer for line tracing
	char varname[100];
	char *varnameptr = varname;
	char spalte = 1;
	char line	= 1;
	struct NSLMenu  *nslmenu;
	char quit = FALSE;

	if ((nslmenu = AllocVec(sizeof(struct NSLMenu), MEMF_ANY | MEMF_CLEAR)))
	{
		NewList((List *)&nslmenu->itemlist);
		var->data		= (ULONG) nslmenu;
		nslmenu->var	= var;
		while (!isgraph(*ptr) && *ptr && *ptr != 10 && *ptr != 13)
			ptr++;
		while (!quit && !programend)
		{
			if (*ptr == '(')
			{
				ptr++;
				quit = TRUE;
			}
			else
				GetNextGraph();
		}

		if (!programend)
			quit = FALSE;
		while (!quit && !programend)
		{
			if (!isgraph(*ptr))
			{
				if (!GetNextGraph())
				{
					quit = TRUE;
					programend = TRUE;
					ioprintf("unexcpected program error in line: %d\n",linecounter);
				}
				else
				{
					ioprintf("getnextgraph error\n");
					quit = TRUE;
					programend = TRUE;
				}

			}
			if (isgraph(*ptr))
			{
				acmdptr = acmd;
				while (myisalpha(*ptr))
					*acmdptr++ = *ptr++;
				*acmdptr = 0;
				switch (getcommandid(acmd))
				{
					case NSL_MENUTAG_COLS:
						if (!nslmenu->colums)
						{
							char int_found = FALSE;
							while (!int_found)
							{
								while (*ptr && *ptr != 10 && *ptr != 13 && !isdigit(*ptr))
									*ptr++;
								if (isdigit(*ptr))
								{
									nslmenu->colums = atoi(ptr);
									if (nslmenu->colums == 0 | nslmenu->colums> 10)
									{
										quit			= TRUE;
										programend	= TRUE;
										ioprintf("create MENU-Error: unvalid colums %d\n",nslmenu->colums);
									}
									int_found	= TRUE;
								}
								else
								{
									if (!GetNextGraph())
									{
										int_found	= TRUE;
										quit			= TRUE;
									}
								}
							}
						}
						else
						{
							ioprintf("menu colums already defined");
							programend = TRUE;
						}
						break;
					case NSL_MENUTAG_AUTOFORMAT:
//						ioprintf("MENU AUTOFORMAT\n");
						break;
					case NSLCOMMAND_PRINT:
						{
							struct NSLMenuItem *item = CreateMenuEntry_PRINT();
							if (item)
							{
								item->X = spalte;
								item->Y = line;
								if (spalte == nslmenu->colums)
								{
									spalte = 1;
									line++;
								}
								else
								{
									spalte++;
								}
								AddTail((List *)&nslmenu->itemlist, (Node *)item);
								item->nsltype = NSL_TYPE_TEXT;
								nslmenu->entrys++;
							}
						}
						break;
					case NSL_TYPE_NULL:
						{
							if (spalte == nslmenu->colums)
							{
								spalte = 1;
								line++;
							}
							else
							{
								spalte++;
							}
						}
						break;
					case NSL_TYPE_FIELD:
						{
							struct NSLMenuItem *item = CreateMenuEntry_PRINT();
							if (item)
							{
								item->X = spalte;
								item->Y = line;
								if (spalte == nslmenu->colums)
								{
									spalte = 1;
									line++;
								}
								else
								{
									spalte++;
								}
								AddTail((List *)&nslmenu->itemlist, (Node *)item);
								item->nsltype = NSL_TYPE_FIELD;
								nslmenu->entrys++;
							}
						}
						break;
					case NSL_TYPE_TEXT:
						{
							struct NSLMenuItem *item = CreateMenuEntry_TEXT();
							if (item)
							{
								item->X = spalte;
								item->Y = line;
								if (spalte == nslmenu->colums)
								{
									spalte = 1;
									line++;
								}
								else
								{
									spalte++;
								}
								AddTail((List *)&nslmenu->itemlist, (Node *)item);
								nslmenu->entrys++;
							}
						}
						break;
					case NSL_TYPE_LINK:
						{
							struct NSLMenuItem *item = CreateMenuEntry_LINK();
							if (item)
							{
								item->X = spalte;
								item->Y = line;
								if (spalte == nslmenu->colums)
								{
									spalte = 1;
									line++;
								}
								else
								{
									spalte++;
								}
								AddTail((List *)&nslmenu->itemlist, (Node *)item);
								item->nsltype = NSL_TYPE_LINK;
								nslmenu->entrys++;
							}
						}
						break;
					case NSL_TYPE_RETURN:
						{
							struct NSLMenuItem *item = CreateMenuEntry_RETURN();
							if (item)
							{
								ioprintf("create return\n");
								item->nsltype = NSL_TYPE_RETURN;
								item->X = spalte;
								item->Y = line;
								if (spalte == nslmenu->colums)
								{
									spalte = 1;
									line++;
								}
								else
								{
									spalte++;
								}
								AddTail((List *)&nslmenu->itemlist, (Node *)item);
								nslmenu->entrys++;
							}
						}
						break;
					case NSLCOMMAND_UNDEFINED:
						if (*ptr == ')')
						{
							ptr++;
							quit = TRUE;
						}
						break;
				}
				if (!quit)
				{
					if (!GetNextGraph())
					{
						ioprintf("program errrorrr in line: %d\n",linecounter);
						programend = TRUE;
						quit = TRUE;
					}
				}
			}
			else
			{
				ioprintf("error ???\n");
			}
		}
		if (nslmenu->colums <= 0 | nslmenu->colums >= 10)
			programend = TRUE;
		if (!programend)
		{
			char collen[10];
			char xpos[10];
			char length;
			char counter;
			char multiplicator = SCREENWIDTH / nslmenu->colums;
			char space = 0;
			char *newtitle;
			char *newptr;
			char *oldptr;
			NSLMenuItem *item = (NSLMenuItem *)nslmenu->itemlist.mlh_Head;
			for (counter = 9; (counter--);)
				collen[counter] = 0;
			while (item->ln_Succ)
			{
				length = strlen(item->title);
				if (length > collen[item->X])
					collen[item->X] = length;
				item = item->ln_Succ;
			}
			for (counter = 1; counter <= nslmenu->colums; counter++)
			{
				space = space + collen[counter] + 1;
			}
			space = (SCREENWIDTH - space + 1) / nslmenu->colums;
			xpos[1] = 1;
			for (counter = 2; counter <= nslmenu->colums; counter++)
			{
				xpos[counter] = xpos[counter - 1] + collen[counter - 1] + space + 1;
			}
			item = (NSLMenuItem *)nslmenu->itemlist.mlh_Head;
			while (item->ln_Succ)
			{
				item->titlelen = collen[item->X] + 1;
				if (item->title)
				{
					if ((newtitle = AllocVec(100, MEMF_ANY|MEMF_CLEAR)))
					{
						newptr = newtitle;
						oldptr = item->title;
						while (*oldptr)
							*newptr++ = *oldptr++;
						for (counter = strlen(item->title); counter < collen[item->X]; counter++)
							*newptr++ = ' ';
						if (item->nsltype == NSL_TYPE_LINK)
							*newptr++ = '>';
						else if (item->nsltype != NSL_TYPE_RETURN)
							*newptr++ = ':';
						FreeVec(item->title);
						item->title = newtitle;
					}
				}
				item->X = xpos[item->X];
				if (item->ln_Succ->ln_Succ)
				{
					if (item->ln_Succ->Y == item->Y)
					{
						item->fieldlen = space;
					}
					else
					{
						item->fieldlen = SCREENWIDTH - item->X - item->titlelen + 1;
					}
				}
				else
					item->fieldlen = SCREENWIDTH - item->X - item->titlelen + 1;
				item = item->ln_Succ;
			}			
		}
	}
	return nslmenu;
}



struct NSLMenuItem *CreateMenuEntry_PRINT()
{
	struct NSLMenuItem *result = AllocVec(sizeof(NSLMenuItem), MEMF_ANY|MEMF_CLEAR);
	ULONG counter = 0;
	char  *ptr2;
	if (result)
	{
		if (!isgraph(*ptr))
			GetNextGraph();
		if (*ptr == '(')		// Open found
		{
			ptr++;
			if (!isgraph(*ptr))
				GetNextGraph();
			if (*ptr == '"')	// first " found
			{
				ptr++;
				ptr2 = ptr;
				while (*ptr2 && *ptr2 != '"')		// search second "
				{
					counter++;
					ptr2++;
				}
				if (*ptr2 == '"')
				{
					if ((result->title = AllocVec(counter + 2, MEMF_ANY | MEMF_CLEAR)))
					{
						ptr2 = result->title;
						while (*ptr && *ptr != '"')		// copy title 'nd wait for second "
							*ptr2++ = *ptr++;
						if (*ptr)
						{
							ptr++;									// jump to next character after "
							while(*ptr && !myisalpha(*ptr))	// space over
								ptr++;
							if (myisalpha(*ptr))
							{
								char name[100];
								char *nameptr = name;
								while (*ptr && myisalpha(*ptr))	// komma weg
									*nameptr++ = *ptr++;
								*nameptr = 0;
								if (!(result->var = getvariable(name)))
								{
									if ((result->var = addvariable(name)))
									{
										result->var->nsltype = NSL_TYPE_STRUCT;
									}
								}
								if (result->var)
								{
									while (*ptr && !isgraph(*ptr))
										ptr++;
									if (isgraph(*ptr))
									{
										if (*ptr == '-' && ptr[1] == '>')
										{
											ptr = ptr + 2;
											while (*ptr && !isgraph(*ptr))
												ptr++;
											if (result->var->nsltype == NSL_TYPE_NOTDEFINED)
												result->var->nsltype = NSL_TYPE_STRUCT;
											if (result->var->nsltype == NSL_TYPE_STRUCT)
											{
												StructureEntry *se;
												nameptr = name;
												while (*ptr && myisalpha(*ptr))
													*nameptr++ = *ptr++;
												*nameptr = 0;
												if (result->var->sl)
												{
													if ((se = searchuserkey(result->var->sl,name)))
													{
														result->se_offset	= se->se_offset;
														result->se_length	= se->se_length;
														result->se_type	= se->se_type;
													}
													else
													{
													}
												}
												else
													ioprintf("can't find structure\n");
											}
											else
											{
												FreeVec(result->title);
												FreeVec(result);
												result		= NULL;
												programend	= TRUE;
												ioprintf("error in line. %d. variable type of %s alread def.\n",linecounter,result->var->name);
											}
										}
										else
										{
											ioprintf("warning 3 <%s>\n",ptr);
										}
									}
									else
										ioprintf("warning 2\n");
								}
								while (*ptr && *ptr != ')')
									ptr++;
							}
							else
							{
								FreeVec(result->title);
								FreeVec(result);
								result		= NULL;
								programend	= TRUE;
								ioprintf("nsl program error.-456.5\n");
							}
							
						}
						else
						{
							FreeVec(result->title);
							FreeVec(result);
							result		= NULL;
							programend	= TRUE;
							ioprintf("nsl program error.-456.4\n");
						}						
					}
					else
					{
						FreeVec(result);
						result		= NULL;
						programend	= TRUE;
						ioprintf("nsl program error.-456.2\n");
					}
				}
				else
				{
					FreeVec(result);
					result		= NULL;
					programend	= TRUE;
					ioprintf("nsl program error.-456.1\n");
				}
			}
		}
	}
	return result;
}

StructureEntry *searchuserkey(StructureList *sl, char *name)
{
	StructureEntry *result	= NULL;
	StructureEntry *se;

	se = (StructureEntry *)sl->se_list.mlh_Head;		// get first variable

	while (se->ln_Succ && !result)
	{
		if (!strcmp(se->se_name, name))
		{
			result = se;
		}
		else
			se = se->ln_Succ;
	}

	return result;
}


struct NSLMenuItem *CreateMenuEntry_TEXT()
{
	struct NSLMenuItem *result = AllocVec(sizeof(NSLMenuItem), MEMF_ANY|MEMF_CLEAR);
	ULONG counter = 0;
	char  *ptr2;
	if (result)
	{
		if (!isgraph(*ptr))
			GetNextGraph();
		if (*ptr == '(')		// Open found
		{
			ptr++;
			if (!isgraph(*ptr))
				GetNextGraph();
			if (*ptr == '"')	// first " found
			{
				ptr++;
				ptr2 = ptr;
				while (*ptr2 && *ptr2 != '"')		// search second "
				{
					counter++;
					ptr2++;
				}
				if (*ptr2 == '"')
				{
					if ((result->title = AllocVec(counter + 2, MEMF_ANY | MEMF_CLEAR)))
					{
						ptr2 = result->title;
						while (*ptr && *ptr != '"')
							*ptr2++ = *ptr++;
						while (*ptr && *ptr != ')')
							ptr++;
						if (!*ptr)
						{
							FreeVec(result);
							result		= NULL;
							programend	= TRUE;
							ioprintf("nsl program error.-456.3\n");
						}
					}
					else
					{
						FreeVec(result);
						result		= NULL;
						programend	= TRUE;
						ioprintf("nsl program error.-456.2\n");
					}
				}
				else
				{
					FreeVec(result);
					result		= NULL;
					programend	= TRUE;
					ioprintf("nsl program error.-456.1\n");
				}
			}
		}
	}
	return result;
}

struct NSLMenuItem *CreateMenuEntry_FIELD()
{
	struct NSLMenuItem *result = AllocVec(sizeof(NSLMenuItem), MEMF_ANY|MEMF_CLEAR);
	if (result)
	{
	}
	return result;
}

struct NSLMenuItem *CreateMenuEntry_LINK()
{
	struct NSLMenuItem *result = AllocVec(sizeof(NSLMenuItem), MEMF_ANY|MEMF_CLEAR);
	ULONG counter = 0;
	char  *ptr2;
	if (result)
	{
		if (!isgraph(*ptr))
			GetNextGraph();
		if (*ptr == '(')		// Open found
		{
			ptr++;
			if (!isgraph(*ptr))
				GetNextGraph();
			if (*ptr == '"')	// first " found
			{
				ptr++;
				ptr2 = ptr;
				while (*ptr2 && *ptr2 != '"')		// search second "
				{
					counter++;
					ptr2++;
				}
				if (*ptr2 == '"')
				{
					if ((result->title = AllocVec(counter + 2, MEMF_ANY | MEMF_CLEAR)))
					{
						ptr2 = result->title;
						while (*ptr && *ptr != '"')
							*ptr2++ = *ptr++;
						if (*ptr)
						{
							ptr++;
							if (*ptr)
							{
								ptr++;
								while(*ptr && !myisalpha(*ptr))
									ptr++;
								if (myisalpha(*ptr))
								{
									char name[100];
									char *nameptr = name;
									while (*ptr && myisalpha(*ptr))
										*nameptr++ = *ptr++;
									*nameptr = 0;
									if (!(result->var = getvariable(name)))
									{
										if ((result->var = addvariable(name)))
										{
											result->var->nsltype = NSL_TYPE_LINK;
										}
									}
									if (result->var)
									{
										while (*ptr && !isgraph(*ptr))
											ptr++;
										if (isgraph(*ptr))
										{
											while (*ptr && !isgraph(*ptr))
												ptr++;
											if (result->var->nsltype == NSL_TYPE_NOTDEFINED)
												result->var->nsltype = NSL_TYPE_LINK;
											if (islink(result->var->nsltype))
											{
//												ioprintf("add link: <%s><%s>\n",result->title,result->var->name);
											}
											else
											{
												FreeVec(result->title);
												FreeVec(result);
												result		= NULL;
												programend	= TRUE;
												ioprintf("error in line. %d. variable %s has wrong type.\n",linecounter,result->var->name);
											}
										}
										else
											ioprintf("warning 2\n");
									}
									while (*ptr && *ptr != ')')
									ptr++;
								}
								else
								{
									FreeVec(result->title);
									FreeVec(result);
									result		= NULL;
									programend	= TRUE;
									ioprintf("nsl program error.-456.5\n");
								}
								
							}
							else
							{
								FreeVec(result->title);
								FreeVec(result);
								result		= NULL;
								programend	= TRUE;
								ioprintf("nsl program error.-456.4\n");
							}						
						}
						else
						{
							FreeVec(result->title);
							FreeVec(result);
							result		= NULL;
							programend	= TRUE;
							ioprintf("nsl program error.-456.3\n");
						}
					}
					else
					{
						FreeVec(result);
						result		= NULL;
						programend	= TRUE;
						ioprintf("nsl program error.-456.2\n");
					}
				}
				else
				{
					FreeVec(result);
					result		= NULL;
					programend	= TRUE;
					ioprintf("nsl program error.-456.1\n");
				}
			}
		}
	}
	return result;
}

struct NSLMenuItem *CreateMenuEntry_RETURN()
{
	struct NSLMenuItem *result = AllocVec(sizeof(NSLMenuItem), MEMF_ANY|MEMF_CLEAR);
	ULONG counter = 0;
	char  *ptr2;
	printf("return:\n");
	if (result)
	{
		if (!isgraph(*ptr))
			GetNextGraph();
		if (*ptr == '(')		// Open found
		{
			ptr++;
			if (!isgraph(*ptr))
				GetNextGraph();
			if (*ptr == '"')	// first " found
			{
				ptr++;
				ptr2 = ptr;
				while (*ptr2 && *ptr2 != '"')		// search second "
				{
					counter++;
					ptr2++;
				}
				if (*ptr2 == '"')
				{
					if ((result->title = AllocVec(counter + 2, MEMF_ANY | MEMF_CLEAR)))
					{
						ptr2 = result->title;
						while (*ptr && *ptr != '"')
							*ptr2++ = *ptr++;
						if (*ptr)
						{
							ptr++;
							if (*ptr)
							{
								ptr++;
								while(*ptr && !myisalpha(*ptr))
									ptr++;
								if (myisalpha(*ptr))
								{
									char name[100];
									char *nameptr = name;
									while (*ptr && myisalpha(*ptr))
										*nameptr++ = *ptr++;
									*nameptr = 0;
									if (!strcmp(name,"TRUE"))
									{
										result->se_type = NSL_RESULT_TRUE;
									}
									else
									{
										if (!strcmp(name,"FALSE"))
										{
											result->se_type = NSL_RESULT_FALSE;
										}
										else
											programend = TRUE;
									}
									while (*ptr && *ptr != ')')
										ptr++;
								}
								else
								{
									FreeVec(result->title);
									FreeVec(result);
									result		= NULL;
									programend	= TRUE;
									ioprintf("nsl program error.-456.5\n");
								}
								
							}
							else
							{
								FreeVec(result->title);
								FreeVec(result);
								result		= NULL;
								programend	= TRUE;
								ioprintf("nsl program error.-456.4\n");
							}						
						}
						else
						{
							FreeVec(result->title);
							FreeVec(result);
							result		= NULL;
							programend	= TRUE;
							ioprintf("nsl program error.-456.3\n");
						}
					}
					else
					{
						FreeVec(result);
						result		= NULL;
						programend	= TRUE;
						ioprintf("nsl program error.-456.2\n");
					}
				}
				else
				{
					FreeVec(result);
					result		= NULL;
					programend	= TRUE;
					ioprintf("nsl program error.-456.1\n");
				}
			}
		}
	}
	return result;
}

struct NSLMenuItem *CreateMenuEntry_NULL()
{
	struct NSLMenuItem *result = AllocVec(sizeof(NSLMenuItem), MEMF_ANY|MEMF_CLEAR);
	return result;
}

BOOL islink(ULONG nsltype)
{
	BOOL result = FALSE;
	if (nsltype == NSL_TYPE_LINK | nsltype == NSL_TYPE_MENU | nsltype == NSL_TYPE_LABEL)
		result = TRUE;
	return result;
}


void jump_menu(NSLMenu *nslmenu)
{
	struct PortData *cport	= (struct PortData *)FindTask(NULL)->tc_UserData;
	char rebuild				= TRUE;
	char buffer[10];
	ULONG *longpt;
	ULONG ulong;
	UBYTE ubyte;
	char  quit = FALSE;
	NSLMenuItem	*item			= (NSLMenuItem *)nslmenu->itemlist.mlh_Head;;
	char *structure;

		while (!quit && !programend && !cport->ProgramClose && cport->network.online)
		{
			if (rebuild==TRUE)
			{
				build_menu(nslmenu);
				if (!item)
					item		= (NSLMenuItem *)nslmenu->itemlist.mlh_Head;
				rebuild	= FALSE;
			}
			if (item)
			{
				if (item->ln_Succ && item->ln_Pred)
				{
					JumpXY(item->X,item->Y);
					ioprintf("[47m[34m%s",item->title);
				}
			}
			structure = (char *)item->var->data;
			Conread(buffer,1,0);
			switch (*buffer)
			{
				case 13:
					switch (item->nsltype)
					{
						case NSL_TYPE_FIELD:
							switch (item->se_type)
							{
								case NSL_TYPE_ACCESSGROUP:
									{
										char buffer[15];
										char src[15];
										LONG dest;
										if (*(structure + item->se_offset))
											sprintf(src,"%u",*(structure + item->se_offset));
										editfield(buffer,src, 10, item->fieldlen, 0, 0);
										StrToLong(buffer,&dest);
										*(structure + item->se_offset) = (ULONG)dest;
									}
									break;
								case NSL_TYPE_ANSICOLORS:
									switch (*(structure + item->se_offset))
									{
										case 0: case 1: case 16:
											*(structure + item->se_offset) = 2;
											break;
										case 2:
											*(structure + item->se_offset) = 4;
											break;
										case 4:
											*(structure + item->se_offset) = 8;
											break;
										case 8:
											*(structure + item->se_offset) = 16;
											break;
									}
									update_item_data(item);
									break;
								case NSL_TYPE_ANSIMODE:
									if (*(structure + item->se_offset) == TRUE)
										*(structure + item->se_offset) = FALSE;
									else
										*(structure + item->se_offset) = TRUE;
									update_item_data(item);
									break;
								case NSL_TYPE_ANSITABS:
									if (*(structure + item->se_offset) == TRUE)
										*(structure + item->se_offset) = FALSE;
									else
										*(structure + item->se_offset) = TRUE;
									update_item_data(item);
									break;
								case NSL_TYPE_ARCMETHOD:
									if (*(structure + item->se_offset) == TRUE)
										*(structure + item->se_offset) = FALSE;
									else
										*(structure + item->se_offset) = TRUE;
									update_item_data(item);
									break;
								case NSL_TYPE_BOOL:
									if (*(structure + item->se_offset) == TRUE)
										*(structure + item->se_offset) = FALSE;
									else
										*(structure + item->se_offset) = TRUE;
									update_item_data(item);
									break;
								case NSL_TYPE_CHARSET:
									if (*(structure + item->se_offset) < 2)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_COMPUTERTYPE:
									if (*(structure + item->se_offset) < 4)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_COUNTRY:
									if (*(structure + item->se_offset) < 13)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_DATE:
									break;
								case NSL_TYPE_DIRECTORYPATH:
									{
										char buffer[80];
										editfield(buffer,structure + item->se_offset, item->se_length, item->fieldlen, 0, 0);
										strncpy(structure + item->se_offset,buffer,item->se_length - 1);
										update_item_data(item);
									}
									break;
								case NSL_TYPE_EOLSEQUENCE:
									break;
								case NSL_TYPE_FILENAME:
									{
										char buffer[80];
										editfield(buffer,structure + item->se_offset, item->se_length, item->fieldlen, 0, 0);
										strncpy(structure + item->se_offset,buffer,item->se_length - 1);
										update_item_data(item);
									}
									break;
								case NSL_TYPE_GENDER:
									if (*(structure + item->se_offset) == 1)
										*(structure + item->se_offset) = 0;
									else
										*(structure + item->se_offset) = 1;
									update_item_data(item);
									break;
								case NSL_TYPE_HELPLEVEL:
									if (*(structure + item->se_offset) < 2)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_ITEMTYPE:
									if (*(structure + item->se_offset) < 10)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_LANGUAGE:
									if (*(structure + item->se_offset) < 13)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_LINEFEEDS:
									if (*(structure + item->se_offset) < 2)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_MOREMODE:
									if (*(structure + item->se_offset) < 2)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_PHONENUMBER:
									break;
								case NSL_TYPE_PHONEVERIFICATION:
									if (*(structure + item->se_offset) < 3)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_PROTOCOL:
									if (*(structure + item->se_offset) < 3)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_STRING:
									{
										char buffer[80];
										editfield(buffer,structure + item->se_offset, item->se_length, item->fieldlen, 0, 0);
										strncpy(structure + item->se_offset,buffer,item->se_length - 1);
										update_item_data(item);
									}
									break;
								case NSL_TYPE_TIMEFORMAT:
									if (*(structure + item->se_offset) < 3)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_DATEFORMAT:
									if (*(structure + item->se_offset) < 2)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_TIMEZONE:
									if (*(structure + item->se_offset) < 24)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_UBYTE:
									{
										char buffer[10];
										char src[10];
										LONG dest;
										if (*(structure + item->se_offset))
											sprintf(src,"%u",*(structure + item->se_offset));
										editfield(buffer,src, 3, item->fieldlen, 0, 0);
										StrToLong(buffer,&dest);
										*(structure + item->se_offset) = (UBYTE)dest;
									}
									break;
								case NSL_TYPE_ULONG:
									{
										char buffer[15];
										char src[15];
										LONG dest;
										if (*(structure + item->se_offset))
											sprintf(src,"%u",*(structure + item->se_offset));
										editfield(buffer,src, 10, item->fieldlen, 0, 0);
										StrToLong(buffer,&dest);
										*(structure + item->se_offset) = (UBYTE)dest;
									}
									break;
								case NSL_TYPE_USERID:
								case NSL_TYPE_USERIP:
									{
										char buffer[15];
										char src[15];
										LONG dest;
										if (*(structure + item->se_offset))
											sprintf(src,"%u",*(structure + item->se_offset));
										editfield(buffer,src, 10, item->fieldlen, 0, 0);
										StrToLong(buffer,&dest);
										*(structure + item->se_offset) = (ULONG)dest;
									}
									break;
								case NSL_TYPE_UWORD:
									{
										char buffer[15];
										char src[15];
										LONG dest;
										if (*(structure + item->se_offset))
											sprintf(src,"%u",*(structure + item->se_offset));
										editfield(buffer,src, 5, item->fieldlen, 0, 0);
										StrToLong(buffer,&dest);
										*(structure + item->se_offset) = (UWORD)dest;
									}
									break;
								case NSL_TYPE_GROUPBOOL:
									if (*(structure + item->se_offset) < 2)
										*(structure + item->se_offset) = *(structure + item->se_offset) + 1;
									else
										*(structure + item->se_offset) = 0;
									update_item_data(item);
									break;
								case NSL_TYPE_STRUCT:
									break;
							}
							break;
						case NSL_TYPE_LINK:
							switch (item->se_type)
							{
								case NSL_TYPE_NOTDEFINED:
									if (item->var->nsltype == NSL_TYPE_LINK)
									{
									}
									else
									{
										if (item->var->nsltype == NSL_TYPE_MENU)
										{
											nslmenu->last_item = item;
											nslmenu = (NSLMenu *)item->var->data;
											item = nslmenu->last_item;
											rebuild = TRUE;
										}
									}			
									break;
							}
							break;
						case NSL_TYPE_RETURN:
							if (item->se_type == TRUE)
								nslmenu->var->result = TRUE;
							else
								nslmenu->var->result = FALSE;
							quit = TRUE;
							break;
					}
					break;
				case -101:
					Conread(buffer,1,0);
					switch(*buffer)
					{
						case 'D':														// CURSER LEFT
							JumpXY(item->X,item->Y);
							ioprintf("[44m[37m%s",item->title);
							if (item->ln_Pred && item->ln_Pred->ln_Pred)
							{
								item = item->ln_Pred;
								JumpXY(item->X,item->Y);
								ioprintf("[47m[34m%s",item->title);
							}
							else
							{
								if (nslmenu->itemlist.mlh_TailPred)
								{
									item = (NSLMenuItem *)nslmenu->itemlist.mlh_TailPred;
									JumpXY(item->X,item->Y);
									ioprintf("[47m[34m%s",item->title);
								}
							}
							break;
						case 'C':														// CURSER RIGHT
							JumpXY(item->X,item->Y);
							ioprintf("[44m[37m%s",item->title);
							if (item->ln_Succ && item->ln_Succ->ln_Succ)
							{
								item = item->ln_Succ;
								JumpXY(item->X,item->Y);
								ioprintf("[47m[34m%s",item->title);
							}
							else
							{
								if (nslmenu->itemlist.mlh_Head)
								{
									item = (NSLMenuItem *)nslmenu->itemlist.mlh_Head;
									JumpXY(item->X,item->Y);
									ioprintf("[47m[34m%s",item->title);
								}
							}
							break;
						case 'A':														// CURSER UP
							if (item->ln_Pred)
							{
								struct NSLMenuItem *new_item = item->ln_Pred;
								while (new_item->ln_Pred && new_item->X != item->X)
									new_item = new_item->ln_Pred;
								if (!new_item->ln_Pred | new_item->X != item->X)
								{
									new_item = (NSLMenuItem *)nslmenu->itemlist.mlh_TailPred;
									while (new_item->ln_Pred && new_item->X != item->X)
										new_item = new_item->ln_Pred;
								}
								if (new_item)
								{
									if (new_item->ln_Pred && new_item->ln_Succ)
									{
										JumpXY(item->X,item->Y);
										ioprintf("[44m[37m%s",item->title);
										item = new_item;
										JumpXY(item->X,item->Y);
										ioprintf("[47m[34m%s",item->title);
									}
								}
							}
							break;
						case 'B': 														// CURSER DOWN
							if (item->ln_Succ)
							{
								struct NSLMenuItem *new_item = item->ln_Succ;
								while (new_item->ln_Succ && new_item->X != item->X)
									new_item = new_item->ln_Succ;
								if (!new_item->ln_Succ | new_item->X != item->X)
								{
									new_item = (NSLMenuItem *)nslmenu->itemlist.mlh_Head;
									while (new_item->ln_Succ && new_item->X != item->X)
										new_item = new_item->ln_Succ;
								}
								if (new_item)
								{
									if (new_item->ln_Pred && new_item->ln_Succ)
									{
										JumpXY(item->X,item->Y);
										ioprintf("[44m[37m%s",item->title);
										item = new_item;
										JumpXY(item->X,item->Y);
										ioprintf("[47m[34m%s",item->title);
									}
								}
							}
							break;
				 	}
					break;
			}
		}
}

void update_item_data(NSLMenuItem *item)
{
	struct PortData *cport = (PortData *)FindTask(NULL)->tc_UserData;
	char buf[80];
//	if (item->se_offset)
	{
		int stringlen;
		char *structure = (char *)item->var->data;
		JumpXY(item->X + item->titlelen,item->Y);
		Writeio("[40m[33m", -1);
		*buf = 0;
		switch (item->se_type)
		{
			case NSL_TYPE_ACCESSGROUP:
				sprintf(buf,"%u",*((ULONG *)(structure+item->se_offset)));
				break;
			case NSL_TYPE_ANSICOLORS:
				sprintf(buf,"%s",getstr(29,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_ANSIMODE:
				sprintf(buf,"%s",getstr(28,*((UWORD *)(structure+item->se_offset))));
			case NSL_TYPE_ANSITABS:
				if (*(structure+item->se_offset))
					sprintf(buf,"yes");
				else
					sprintf(buf,"no");
				break;
			case NSL_TYPE_ARCMETHOD:
				sprintf(buf,"%u",*(structure+item->se_offset));
				break;
			case NSL_TYPE_BOOL:
				sprintf(buf,"%s",getstr(0,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_GROUPBOOL:
				sprintf(buf,"%s",getstr(28,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_CHARSET:
				sprintf(buf,"%s",getstr(22,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_COMPUTERTYPE:
				sprintf(buf,"%s",getstr(31,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_COUNTRY:
				sprintf(buf,"%s",getstr(21,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_DATE:
				DateToString(buf, (struct Date *)(structure+item->se_offset),0);
				break;
			case NSL_TYPE_DIRECTORYPATH:
				sprintf(buf,"%s",structure+item->se_offset);
				break;
			case NSL_TYPE_EOLSEQUENCE:
				sprintf(buf,"%u",*(structure+item->se_offset));
				break;
			case NSL_TYPE_FILENAME:
				sprintf(buf,"%s",structure+item->se_offset);
				break;				
			case NSL_TYPE_GENDER:
				sprintf(buf,"%s",getstr(20,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_HELPLEVEL:
				sprintf(buf,"%s",getstr(32,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_ITEMTYPE:
				sprintf(buf,"%s",getstr(96,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_LANGUAGE:
				sprintf(buf,"%s",getstr(21,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_LINEFEEDS:
				sprintf(buf,"%s",getstr(26,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_MOREMODE:
				sprintf(buf,"%s",getstr(34,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_PHONENUMBER:
				sprintf(buf,"%s",structure+item->se_offset);
				break;
			case NSL_TYPE_PHONEVERIFICATION:
				sprintf(buf,"%s",getstr(15,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_PROTECTION:
				{
					ULONG protection = (ULONG)*(structure+item->se_offset);
				strcpy(buf,"--------");
				if (protection & FIBF_SCRIPT)
					buf[1]='s';
				if (protection & FIBF_PURE)
					buf[2]='p';
				if (protection & FIBF_ARCHIVE)
					buf[3]='a';
//				if (protection & FIBF_READ)
					buf[4]='r';
//				if (protection & FIBF_WRITE)
					buf[5]='w';
//				if (protection & FIBF_EXECUTE)
					buf[6]='e';
//				if (protection & FIBF_DELETE)
					buf[7]='d';
				}
				break;
			case NSL_TYPE_PROTOCOL:
				sprintf(buf,"%s",getstr(33,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_STRING:
				sprintf(buf,"%s",structure+item->se_offset);
				break;
			case NSL_TYPE_TIMEFORMAT:
				sprintf(buf,"%s",getstr(24,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_TIMEZONE:
				sprintf(buf,"%s",getstr(16,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_DATEFORMAT:
				sprintf(buf,"%s",getstr(25,*((UWORD *)(structure+item->se_offset))));
				break;
			case NSL_TYPE_UBYTE:
				sprintf(buf,"%u",*(structure+item->se_offset));
				break;
			case NSL_TYPE_ULONG:
				sprintf(buf,"%u",*((ULONG *)(structure+item->se_offset)));
				break;
			case NSL_TYPE_USERID:
				sprintf(buf,"%u",*((ULONG *)(structure+item->se_offset)));
				break;
			case NSL_TYPE_USERIP:
				sprintf(buf,"%u",*((ULONG *)(structure+item->se_offset)));
				break;
			case NSL_TYPE_UWORD:
				sprintf(buf,"%u",*((UWORD *)(structure+item->se_offset)));
				break;
		}
		if ((stringlen = strlen(buf)))
		{
			if (stringlen < item->fieldlen)
			{
				char *bufptr = buf;
				int counter = 0;
				while (*bufptr)
					bufptr++;
				for (counter = stringlen; counter < item->fieldlen; counter++)
					*bufptr++ = ' ';
				*bufptr = 0;
				Writeio(buf,-1);
			}
			else
			{
				if (stringlen > item->fieldlen)
				{
					Writeio(buf,item->fieldlen);
				}
				else
					Writeio(buf,-1);
			}
		}
	}
}

void build_menu(NSLMenu *nslmenu)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct NSLMenuItem *item;
	char counter;
	CLS();
	item = (NSLMenuItem *)nslmenu->itemlist.mlh_Head;
	while (item->ln_Succ)
	{
		JumpXY(item->X,item->Y);
		ioprintf("[44m[37m%s",item->title);
		update_item_data(item);
		item = item->ln_Succ;
	}
}




































