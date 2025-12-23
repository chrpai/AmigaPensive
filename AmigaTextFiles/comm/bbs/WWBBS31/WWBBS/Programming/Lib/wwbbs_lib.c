#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/wwbbs.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/wwbbs.h>

BOOL Ask(BYTE *prompt,BOOL def)
	{
		BOOL ret=FALSE,kg=TRUE;
		if(prompt)
			printf("~p\n%s (%s,%s)? ",prompt,(def) ? "[Y]" : "Y",(def) ? "N" : "[N]");
		{
			int c;
			while(kg)
				{
					c=getchar();
					if(c==EOF)
						break;
					switch(c)
						{
							case '\r':
								ret=def;
								kg=FALSE;
								break;
							case 'Y':
							case 'y':
								ret=TRUE;
								kg=FALSE;
								break;
							case 'N':
							case 'n':
								ret=FALSE;
								kg=FALSE;
								break;
						}
				}
		}
		if(!kg)
			{
				if(ret)
					printf("~iYes\n");
				else
					printf("~iNo\n");
			}
		return(ret);
	}

BOOL GetLine(BYTE *buff,UWORD length,ULONG flags)
	{
		BOOL ret=FALSE;
		UWORD index=0;
		printf("~i");
		if(flags & GLFLG_Edit)
			{
				index=strlen(buff);
				buff[index]=NULL;
				printf(buff);
			}
		else
			strcpy(buff,"");
		{
			BOOL kg=TRUE;
			int c;
			while(kg)
				{
					c=getchar();
					if(c==EOF)
						break;
					switch(c)
						{
							case '\b':
								if(index)
									{
										index--;
										buff[index]=NULL;
										printf("\b \b");
									}
								break;
							case '\r':
								if( !( !index && (flags & GLFLG_NoEmpty) ) )
									{
										printf("\n");
										if(index)
											ret=TRUE;
										kg=FALSE;
									}
								break;
							default:
								if(isprint(c) && index<length)
									{
										if(flags & GLFLG_Format)
											{
												if(isalpha(c))
													{
														if( (index && !isalpha(buff[index-1])) || !index)
															c=toupper(c);
														else
															c=tolower(c);
													}
											}
										if(flags & GLFLG_ToLower)
											{
												if(isalpha(c))
													c=tolower(c);
											}
										if(flags & GLFLG_ToUpper)
											{
												if(isalpha(c))
													c=toupper(c);
											}
										if(flags & GLFLG_Chars)
											{
												if(!isalpha(c))
													c=NULL;
											}
										if(flags & GLFLG_Digits)
											{
												if(!isdigit(c))
													c=NULL;
											}
										if(flags & GLFLG_NoChars)
											{
												if(isalpha(c))
													c=NULL;
											}
										if(flags & GLFLG_NoDigits)
											{
												if(isdigit(c))
													c=NULL;
											}
										if(flags & GLFLG_NoSpaces)
											{
												if(isspace(c))
													c=NULL;
											}
										if(flags & GLFLG_NoPunct)
											{
												if(ispunct(c))
													c=NULL;
											}
										if(flags & GLFLG_BeginChar)
											{
												if(!index && !isalpha(c))
													c=NULL;
											}
										if(flags & GLFLG_BeginDigit)
											{
												if(!index && !isdigit(c))
													c=NULL;
											}
										if(c)
											{
												sprintf(&buff[strlen(buff)],"%c",c);
												index++;
												if(flags & GLFLG_NoEcho)
													printf(".");
												else
													printf("%c",c);
											}
									}
								break;
						}
				}
		}
		return(ret);
	}

void ShowText(BYTE *file)
	{
		BYTE rows[4];
		strcpy(rows,"");
		if(GetVar("ROWS",rows,3,NULL)!=-1)
			{
				FILE *fp=NULL;
				BYTE file_node[256];
				int count=0;
				printf("~o");
				{
					BYTE node[33];
					strcpy(node,"");
					if(GetVar("NODE",node,32,NULL)!=-1)
						{
							sprintf(file_node,"%s.%s",file,node);
							fp=fopen(file_node,"r");
						}
				}
				if(fp || (fp=fopen(file,"r")))
					{
						BYTE buff[256];
						BOOL ansi_file_check=FALSE,is_ansi_file=FALSE;
						while(fgets(buff,255,fp))
							{
								if(!ansi_file_check)
									{
										if(!stricmp(buff,"@ANSI"))
											{
												is_ansi_file=TRUE;
												break;
											}
										ansi_file_check=TRUE;
									}
								if(WaitForChar(Input(),0))
									{
										getchar();
										break;
									}
								if(count==atoi(rows)-1)
									{
										printf("~pMore ([Y],N)? ");
										if(!Ask(NULL,TRUE))
											break;
										printf("~o");
										count=0;
									}
								printf(buff);
								count++;
							}
						if(is_ansi_file)
							{
								BYTE buff[64];
								int num;
								while(num=fread(buff,sizeof(BYTE),63,fp))
									{
										if(WaitForChar(Input(),0))
											{
												getchar();
												break;
											}
										printf("%.*s",num,buff);
									}
							}
						fclose(fp);
					}
			}
	}

BOOL RunEditor(BYTE *filename)
	{
		BOOL ret=FALSE;
		BYTE editor[256];
		strcpy(editor,"");
		if(GetVar("EDITOR",editor,255,NULL)!=-1)
			{
				if(strlen(editor))
					{
						BYTE cmd[256];
						BOOL insert_filename=FALSE;
						{
							char *p;
							if(p=strchr(editor,'%'))
								{
									p++;
									if(*p=='f')
										{
											insert_filename=TRUE;
											*p='s';
										}
								}
						}
						if(insert_filename)
							sprintf(cmd,editor,filename);
						else
							sprintf(cmd,"%s \"%s\"",editor,filename);
						printf("~r");
						SetMode(Input(),0);
						SystemTags(cmd,TAG_END);
						SetMode(Input(),1);
						{
							FILE *fp;
							if(fp=fopen(filename,"r"))
								{
									fclose(fp);
									if(Ask("Save message",TRUE))
										ret=TRUE;
									else
										DeleteFile(filename);
								}
						}
					}
				else
					printf("~s\nPlease select an editor first.\n");
			}
		return(ret);
	}

BOOL Pager(BYTE *text,UWORD lines)
	{
		BOOL ret=FALSE;
		BYTE rows[4];
		strcpy(rows,"");
		if(GetVar("ROWS",rows,3,NULL)!=-1)
			{
				char *p,*q;
				int count=0;
				p=text;
				count=lines;
				printf("~o");
				while(q=strchr(p,'\n'))
					{
						if(WaitForChar(Input(),0))
							{
								int c;
								c=getchar();
								if(c=='\r')
									ret=TRUE;
								break;
							}
						if(count==atoi(rows)-1)
							{
								printf("~pMore ([Y],N)? ");
								if(!Ask(NULL,TRUE))
									break;
								printf("~o");
								count=0;
							}
						q++;
						printf("%.*s",q-p,p);
						p=q;
						count++;
					}
			}
		return(ret);
	}
