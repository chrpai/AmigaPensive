#include "wwbbs.h"

__asm
void LogEntry(register __a0 BYTE *id,register __a1 BYTE *log,register __a2 BYTE *text)
	{
		BPTR fh;
		BYTE file[256];
		sprintf(file,"WWBBS:Logs/%s",log);
		if(fh=Open(file,MODE_READWRITE))
			{
				Seek(fh,0,OFFSET_END);
				{
					char date[9],time[9];
					{
						struct DateTime dt;
						DateStamp(&dt.dat_Stamp);
						dt.dat_Format=FORMAT_USA;
						dt.dat_Flags=NULL;
						dt.dat_StrDay=NULL;
						dt.dat_StrDate=date;
						dt.dat_StrTime=time;
						DateToStr(&dt);
					}
					{
						char s[256];
						sprintf(s,"%s %s %s %s\n",date,time,(id) ? id : "<None>",text);
						FPuts(fh,s);
					}
				}
				Close(fh);
			}
	}

__asm
BOOL IsRange(register __a0 BYTE *range,register __d0 ULONG num)
	{
		BOOL ret=FALSE;
		char rangebuff[256],*token;
		if(strlen(range)>255)
			{
				strncpy(rangebuff,range,255);
				range[255]=NULL;
			}
		else
			strcpy(rangebuff,range);
		token=strtok(rangebuff,", ");
		while(token)
			{
				if(strchr(token,'-'))
					{
						ULONG buff_low=0,buff_high=0;
						if(*token=='-')
							buff_high=atol(&token[1]);
						else
							sscanf(token,"%ld-%ld",&buff_low,&buff_high);
						if(!buff_high)
							buff_high=(ULONG) ~0;
						if(buff_low<=num && buff_high>=num)
							ret=TRUE;
					}
				else
					{
						ULONG buff;
						buff=atol(token);
						if(buff==num)
							ret=TRUE;
					}
				token=strtok(NULL,", ");
			}
		return(ret);
	}
