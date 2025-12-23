#include <exec/types.h>
#include <exec/exec.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>

#include "editor_rev.h"

char *ver=VERSTAG;

struct TextNode {
	struct Node tn_Node;
	char tn_Buffer[80];
};

BOOL ask(BYTE *,BOOL);

BOOL panic=FALSE;

void __regargs _CXBRK(void);

void __regargs _CXBRK(void)
	{
		panic=TRUE;
	}

void main(int argc,char **argv)
	{
		struct List TextList;
		struct TextNode *tnode=NULL;
		int x=0,y=0,abort=FALSE;
		if(argc!=2)
			exit(RETURN_FAIL);
		NewList(&TextList);
		printf("\n~hWorld Wide BBS Editor - Press CONTROL-P for Help\n~i");
		{
			BPTR fh;
			if(fh=Open(argv[1],MODE_OLDFILE))
				{
					BYTE buff[256];
					while(FGets(fh,buff,255))
						{
							{
								char *p;
								if(p=strchr(buff,'\n'))
									*p=NULL;
							}
							if(tnode=AllocMem(sizeof(struct TextNode),MEMF_CLEAR))
								{
									if(strlen(buff)>79)
										{
											strncpy(tnode->tn_Buffer,buff,79);
											tnode->tn_Buffer[79]=NULL;
										}
									else
										strcpy(tnode->tn_Buffer,buff);
									printf("%s\n",tnode->tn_Buffer);
									AddTail(&TextList,(struct Node *) tnode);
									y++;
								}
						}
					Close(fh);
					DeleteFile(argv[1]);
				}
		}
		{
			if(tnode=AllocMem(sizeof(struct TextNode),MEMF_CLEAR))
				{
					AddTail(&TextList,(struct Node *) tnode);
					printf("~p%d:\n~i",y+1);
				}
		}
		{
			int c;
			int kg=TRUE;
			while(!panic && kg)
				{
					c=getch();
					if(c==EOF)
						break;
					{
						int printprompt=FALSE;
						switch(c)
							{
								case 1: /* Abort */
									if(x)
										printf("\n");
									if(ask("Abort Message",FALSE))
										{
											abort=TRUE;
											kg=FALSE;
										}
									else
										{
											printf("\n~i");
											printprompt=TRUE;
										}
									break;
								case 12: /* List */
									if(y)
										{
											if(x)
												printf("\n");
											printf("~o\n");
											{
												struct TextNode *wn;
												for(wn=(struct TextNode *) TextList.lh_Head;wn->tn_Node.ln_Succ;wn=(struct TextNode *) wn->tn_Node.ln_Succ)
													{
														if(wn!=tnode)
															printf("%s\n",wn->tn_Buffer);
													}
											}
											printf("\n~i");
											printprompt=TRUE;
										}
									break;
								case 14: /* New */
									if(x)
										printf("\n");
									if(ask("Clear Text",FALSE))
										{
											struct TextNode *wn,*nn;
											wn=(struct TextNode *) TextList.lh_Head;
											while(nn=(struct TextNode *) wn->tn_Node.ln_Succ)
												{
													if(wn!=tnode)
														{
															Remove((struct Node *) wn);
															FreeMem(wn,sizeof(struct TextNode));
														}
													else
														strcpy(wn->tn_Buffer,"");
													wn=nn;
												}
											x=y=0;
											printf("~s\nMessage text cleared.\n\n~i");
											printprompt=TRUE;
										}
									else
										{
											printf("\n~i");
											printprompt=TRUE;
										}
									break;
								case 16: /* Help */
									if(x)
										printf("\n");
									printf("~o\n");
									printf("[CONTROL-A] Abort Message\n");
									printf("[CONTROL-L] List Message\n");
									printf("[CONTROL-N] New, Clear Text\n");
									printf("[CONTROL-P] Help\n");
									printf("[CONTROL-Z] Save Message\n");
									printf("\n~i");
									printprompt=TRUE;
									break;
								case 26: /* Save */
									if(x)
										printf("\n");
										if(!x)
											{
												Remove((struct Node *) tnode);
												FreeMem(tnode,sizeof(struct TextNode));
												tnode=NULL;
											}
										kg=FALSE;
									break;
								case '\r':
									if(tnode=AllocMem(sizeof(struct TextNode),MEMF_CLEAR))
										{
											AddTail(&TextList,(struct Node *) tnode);
											x=0;
											y++;
											printf("\n");
										}
									break;
								case '\b':
									if(tnode)
										{
											if(x)
												{
													x--;
													tnode->tn_Buffer[x]=NULL;
													printf("\b \b");
												}
											else
												{
													if(y)
														{
															struct TextNode *wn;
															if(wn=(struct TextNode *) tnode->tn_Node.ln_Pred)
																{
																	Remove((struct Node *) tnode);
																	FreeMem(tnode,sizeof(struct TextNode));
																	tnode=wn;
																	x=strlen(tnode->tn_Buffer);
																	y--;
																	printprompt=TRUE;
																}
														}
												}
										}
									break;
								case '\t':
									if(tnode)
										{
											if(x<75)
												{
													int i,t;
													t=5-(x%5);
													for(i=0;i<t;i++)
														{
															strcat(tnode->tn_Buffer," ");
															printf(" ");
															x++;
														}
												}
										}
									break;
								default:
									if(tnode)
										{
											if(isprint(c))
												{
													if(x<79)
														{
															sprintf(&tnode->tn_Buffer[strlen(tnode->tn_Buffer)],"%c",c);
															printf("%c",c);
															x++;
														}
													else
														{
															if(strchr(tnode->tn_Buffer,' '))
																{
																	char *p;
																	p=&tnode->tn_Buffer[strlen(tnode->tn_Buffer)-1];
																	while(!isspace(*p))
																		{
																			printf("\b \b");
																			p--;
																		}
																	*p=NULL;
																	p++;
																	if(tnode=AllocMem(sizeof(struct TextNode),MEMF_CLEAR))
																		{
																			AddTail(&TextList,(struct Node *) tnode);
																			strcpy(tnode->tn_Buffer,p);
																			sprintf(&tnode->tn_Buffer[strlen(tnode->tn_Buffer)],"%c",c);
																			x=strlen(tnode->tn_Buffer);
																			y++;
																			printf("\n");
																			printf(tnode->tn_Buffer);
																		}
																}
															else
																{
																	if(tnode=AllocMem(sizeof(struct TextNode),MEMF_CLEAR))
																		{
																			AddTail(&TextList,(struct Node *) tnode);
																			tnode->tn_Buffer[0]=c;
																			tnode->tn_Buffer[1]=NULL;
																			x=1;
																			y++;
																			printf("\n");
																			printf(tnode->tn_Buffer);
																		}
																}
														}
												}
										}
									break;
							}
						if(printprompt)
							{
								printf("~p%d:\n~i",y+1);
								if(x && tnode)
									printf(tnode->tn_Buffer);
							}
					}
				}
		}
		if(!abort)
			{
				BPTR fh;
				if(fh=Open(argv[1],MODE_NEWFILE))
					{
						for(tnode=(struct TextNode *) TextList.lh_Head;tnode->tn_Node.ln_Succ;tnode=(struct TextNode *) tnode->tn_Node.ln_Succ)
							{
								FPuts(fh,tnode->tn_Buffer);
								FPutC(fh,'\n');
							}
						Close(fh);
					}
			}
		{
			struct TextNode *wn,*nn;
			wn=(struct TextNode *) TextList.lh_Head;
			while(nn=(struct TextNode *) wn->tn_Node.ln_Succ)
				{
					FreeMem(wn,sizeof(struct TextNode));
					wn=nn;
				}
		}
		exit(0);
	}

BOOL ask(BYTE *prompt,BOOL def)
	{
		int c;
		BOOL ret=NULL;
		int kg=TRUE;
		if(prompt)
			printf("~p\n%s (%s,%s)? ",prompt,(def) ? "[Y]" : "Y",(!def) ? "[N]" : "N");
		while(!panic && kg)
			{
				c=getch();
				if(c==EOF)
					break;
				{
					switch(c)
						{
							case '\r':
								ret=def;
								kg=FALSE;
								break;
							case 'y':
							case 'Y':
								ret=TRUE;
								kg=FALSE;
								break;
							case 'n':
							case 'N':
								ret=FALSE;
								kg=FALSE;
								break;
						}
				}
			}
		if(!kg)
			printf((ret) ? "Yes\n" : "No\n");
		return(ret);
	}
