#include <exec/types.h>
//#include <exec/memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <pragma/noviasys_lib.h>
#include <novia/novia_portdata.h>

char editfield(char *bp, char *src, char maxlen, char fieldlen, ULONG flags, ULONG sigs)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	char result = 0;
	Writeio("[40;33m",-1);
	if (cport)
	{
		char cp[80];
		char bp2[80];
		char pos		= 0;
		char end		= 0;
		char start	= 0;
		BOOL Quit	= FALSE;
		BOOL habschon;
		BOOL change	= FALSE;
		int i;
		char wert;
		short counter;
		BOOL first	= FALSE;
		BOOL ende	= FALSE;
		*bp			= 0;
		Writeio("c3",-1);
		if (src!=0)
		{
			strcpy(bp,src);
			end = strlen(bp);
			if (end > fieldlen)
			{
				Writeio(bp + (end - fieldlen),fieldlen);
				start = end - fieldlen;
			}
			else
			{
				Writeio(bp, fieldlen);
			}
			pos = end;
		}
		
//		printf("pos: %d start: %d end: %d maxlen: %d fieldlen: %d source: <%s>\n",pos,start,end,maxlen,fieldlen,src);

		bp[end]=0;
		while(!Quit && cport->network.online)
		{
//			printf("pos: %d start: %d end: %d pos: <%c>\n",pos,start,end,bp[pos]);
			habschon=FALSE;
			Conread(cp,1,sigs);
			if (*cp==13)
			{
				*cp=10;
				Writeio(cp,1);
				Quit=TRUE;
			}
			if (*cp==-101)
			{
				Conread(cp,1,sigs);
				switch (*cp)
				{
					case 'D':			// CURSER LEFT
						if (pos>0)
						{
							if (pos - start > 0)
							{
								Writeio("[D",-1);
							}
							else
							{
								start--;
								if ((end - pos) > fieldlen)
								{
//									printf("CURSER LEFT: end - pos <= fieldlen, %d %d\n",end,pos);
									Writeio(bp + start,fieldlen);
									cleft(fieldlen);
								}
								else
								{
//									printf("CURSER LEFT: end - pos %d %d\n",end,pos);
									Writeio(bp + start, end - pos);
									cleft(end - pos);
								}
							}
							pos--;
					 	}
						break;
					case 'C':			// CURSER RIGHT
						if (pos<end && end<=maxlen)
						{
							if (pos - start < fieldlen)
							{
								Writeio("[C",-1);
							}
							else
							{
								start++;
								cleft(fieldlen);
								Writeio(bp + start,fieldlen);
							}
							pos++;
						}
						break;
				}
				habschon=TRUE;
				change=TRUE;
			}
			if (*cp == 8 && habschon == FALSE)		// BACKSPACE
			{
				if (pos > 0)
				{
//					printf("BACKSPACE\n");
					if (start < pos)
					{
						cleft(1);
						if (end - pos >= fieldlen)
						{
							Writeio(bp + pos, (fieldlen - (pos - start)) + 1);
							cleft((fieldlen - (pos - start)) + 1);
						}
						else
						{
//							printf("start == pos %d %d\n",start,pos);
							Writeio(bp + pos, end - pos);
							space(fieldlen - (end - pos) - (pos - start) + 1);
							cleft(fieldlen - (end - pos) - (pos - start) + 1);
							cleft(end - pos);
						}
					}
					else
					{
						start--;
					}
					pos--;
					for (i=pos;i<end;i++)
					{
						bp[i]=bp[i+1];
					}
					end--;
					change = TRUE;
				}
				habschon = TRUE;
			}
			if (*cp == 127 &&habschon == FALSE)	// DELete
			{
				if (pos < end)
				{
					for (i=pos;i<end;i++)
					{
						bp[i]=bp[i+1];
					}
					end--;
					Writeio(bp + pos, fieldlen - (pos - start));
					if (end - pos > fieldlen)
					{
						cleft((fieldlen - (pos - start)));
					}
					else
					{
						space(fieldlen - (pos - start) - (end - pos));
						cleft(fieldlen - (pos - start) - (end - pos));
						cleft(end - pos);
					}
					change = TRUE;
				}
				habschon = TRUE;
			}
			if (((*cp>=-96 && *cp<0) | (*cp>=32 && *cp<127)) && end<maxlen && pos<maxlen && habschon==FALSE)
			{
				if ((!(flags & INPUT_TYPE_NUMBER))| (*cp>47 && *cp<58))
				{
					if (pos == end)
					{
/*						if (change == FALSE && pos>0)
						{
							cleft(pos);
							space(end);
							cleft(end);
							pos		= 1;
							end		= 1;
							bp[0]		= *cp;
							bp[1]		= 0;
							Writeio(cp,1);
						}
						else*/
						{
							if ((pos - start) < fieldlen)
							{
//								printf("if 1\n");
								Writeio(cp,1);
							}
							else
							{
//								printf("if 2\n");
								cleft(fieldlen);
								Writeio(bp + start + 1,fieldlen);
								Writeio(cp,1);
								start++;
							}
							bp[pos] = *cp;
							pos++;
							end++;
							bp[end] = 0;
						}
					}
					else
					{
						if ((pos - start) == fieldlen)
						{
//							printf("if 3\n");
							cleft(fieldlen);
							Writeio(bp + start + 1,fieldlen - 1);
							Writeio(cp, 1);
							start++;
						}
						else
						{
							if (end > fieldlen)
							{
//								printf("if 4\n");
								cleft((pos - start));
								Writeio(bp + start,pos - start);
								Writeio(cp,1);
								Writeio(bp + start + (pos - start), fieldlen - (pos - start) - 1);
								cleft((fieldlen - (pos - start)) -1);
							}
							else
							{
//								printf("if 5\n");
								Writeio(cp,1);
                        if ((end - pos) > fieldlen)
								{
									Writeio(bp + pos, fieldlen - (pos - start) - 1);
									cleft((fieldlen - (pos - start)) -1);
								}
								else
								{
                           Writeio(bp + pos, end - pos - 1);
									cleft(end - pos - 1);
								}
							}
						}
						for (i = end ; i >= pos ; i--)
						{
							bp[i+1] = bp[i];
						}
						end++;
						bp[pos]	= *cp;
						bp[end]	= 0;
						pos++;
					}
					change	= TRUE;
					habschon	= TRUE;
					bp[end]	= 0;
				}
			}
			if (flags & CURSER_BREAK)
				Quit = TRUE;
//			if (pos == 0 && end == 0) Quit = TRUE;
		}
		bp[end]=0;
		bp[end+1]=0;
	}
	return result;
}
