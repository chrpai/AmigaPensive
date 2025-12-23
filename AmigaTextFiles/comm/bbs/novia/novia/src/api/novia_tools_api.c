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



struct ColorSpec colorspec[]=
{
	{0,0x0,0x0,0x0},
	{1,0xc,0x0,0x0},
	{2,0x0,0xc,0x0},
	{3,0xc,0xc,0x0},
	{4,0x0,0x0,0xc},
	{5,0xc,0x0,0xc},
	{6,0x0,0xc,0xc},
	{7,0xF,0xF,0xF},
	{~0,0,0,0}
};

char *empty_string="";



extern struct Task *MainTask;

extern struct GUIFrontend guifrontend;
extern struct Task		  *GUITask;

extern void StartGUI();


__saveds ASM void nslib_GetTime(register __a0 struct Date *date);
__saveds ASM void nslib_Datecode2Date(register __a0 struct Date *date);
__saveds ASM void nslib_DateToString(register __a0 const char *buffer, register __a1 struct Date *date, register __d0 ULONG flags);
__saveds ASM void nslib_CalcDatecode(register __a0 struct Date *date);
__saveds ASM void nslib_StringToDate(register __a0 const char *buffer, register __a1 struct Date *date, register __d0 ULONG flags);
__saveds ASM void nslib_Monitor_open();
__saveds ASM void nslib_Monitor_close();
__saveds ASM char * nslib_getstr(register __d0 UWORD group, register __d1 UWORD stringno);
__saveds ASM ULONG nslib_SearchDateKey(register __a0 char *str);
__saveds ASM ULONG nslib_opengui();
__saveds ASM ULONG nslib_closegui();


__saveds ASM void nslib_GetTime(register __a0 struct Date *date)
{
	timeval tv;
	ClockData cd;
	GetSysTime(&tv);
	Amiga2Date(tv.tv_secs,&cd);
	date->year	=cd.year;
	date->month	=(UBYTE)cd.month;
	date->day	=(UBYTE)cd.mday;
	date->hours	=(UBYTE)cd.hour;
	date->mins	=(UBYTE)cd.min;
	date->secs	=(UBYTE)cd.sec;
	date->weekday=(UBYTE)cd.wday;
	date->datecode=tv.tv_secs;
}

__saveds ASM void nslib_Datecode2Date(register __a0 struct Date *date)
{
	if (date->year && date->month && date->day)
	{
		ClockData cd;
		cd.year	=date->year;
		cd.month=(UWORD)date->month;
		cd.mday	=(UWORD)date->day;
		cd.hour	=(UWORD)date->hours;
		cd.min	=(UWORD)date->mins;
		cd.sec	=(UWORD)date->secs;
		date->datecode=Date2Amiga(&cd)
	}
}

__saveds ASM void nslib_CalcDatecode(register __a0 struct Date *date)
{
	if (date->year && date->month && date->day)
	{
		ClockData cd;
		cd.year	=date->year;
		cd.month=(UWORD)date->month;
		cd.mday	=(UWORD)date->day;
		cd.hour	=(UWORD)date->hours;
		cd.min	=(UWORD)date->mins;
		cd.sec	=(UWORD)date->secs;
		date->datecode=Date2Amiga(&cd)
	}
}

__saveds ASM void nslib_Monitor_open()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		struct TextFont *textfont;
		struct TextAttr textattr[]={"cnet21.font",8,0,FPF_DISKFONT};
		textfont=OpenDiskFont(textattr);
		if (textfont)CloseFont(textfont);
		if (!cport->win)
		{
			if (cport->clientconfig.UseWorkbench)
			{
				cport->scr=0;
			}
			else
			{
				cport->scr=OpenScreenTags(NULL,
				SA_LikeWorkbench, TRUE,
				SA_DisplayID, cport->clientconfig.ScreenModeID,
				SA_Depth, cport->clientconfig.Colors,
				SA_Colors, colorspec,
				SA_Title,	cport->Titel,
	//			SA_Font, textattr,
				TAG_DONE, TAG_DONE);
		
			}
			if (cport->scr)
			{
				if (!(cport->win = OpenWindowTags(NULL,
				WA_Width,	cport->scr->Width,
				WA_Height,	cport->scr->Height-cport->scr->BarHeight-2,
				WA_Top, 	cport->scr->BarHeight+2,
				WA_CustomScreen,cport->scr,
				WA_Activate, 	TRUE,
				WA_DragBar, 	FALSE,
				WA_DepthGadget,	FALSE,
				WA_CloseGadget,	FALSE,
				WA_SizeGadget,	FALSE,
				WA_Borderless,	TRUE,
				WA_Backdrop,	TRUE,
				WA_BlockPen,	0,
				WA_DetailPen,	0,
				WA_Colors, colorspec,
				WA_IDCMP,		IDCMP_MENUPICK,
				WA_SmartRefresh, TRUE,
				TAG_DONE)))CloseIt(20,"Can't open window");
			}
			else
			{
				if (!(cport->win = OpenWindowTags(NULL,
				WA_Width,	640,
				WA_Height,	423,
				WA_Top, 	15,
				WA_Activate, 	TRUE,
				WA_DragBar, 	TRUE,
	
				WA_Colors, colorspec,
	
				WA_DepthGadget,	TRUE,
				WA_CloseGadget,	FALSE,
				WA_SizeGadget,	TRUE,
				WA_Borderless,	FALSE,
				WA_Backdrop,	FALSE,
				WA_BlockPen,	0,
				WA_DetailPen,	0,
				WA_IDCMP,		IDCMP_MENUPICK,
				WA_SmartRefresh, TRUE,
				TAG_DONE)))CloseIt(20,"Can't open window");
	
			}
	/*			SetRGB4(&win->WScreen->ViewPort,0,0,0,0);
				SetRGB4(&win->WScreen->ViewPort,1,12,0,0);
				SetRGB4(&win->WScreen->ViewPort,2,0,12,0);
				SetRGB4(&win->WScreen->ViewPort,3,12,12,0);
				SetRGB4(&win->WScreen->ViewPort,4,0,0,12);
				SetRGB4(&win->WScreen->ViewPort,5,12,0,12);
				SetRGB4(&win->WScreen->ViewPort,6,0,12,12);
				SetRGB4(&win->WScreen->ViewPort,7,15,15,15);*/
	
			if (cport->win)
			{
				cport->MonitorOpen=TRUE;
	
			   cport->ConWrite->io_Message.mn_ReplyPort = cport->ConPort;
			   cport->ConWrite->io_Data = (APTR)cport->win;
			   cport->ConWrite->io_Length = sizeof(Window);
			   if (OpenDevice("console.device", 0, (IORequest *)cport->ConWrite, 0))
					printf("Can't open console.device. error: %d\n",cport->ConWrite->io_Error);
	
			   *cport->ConRead = *cport->ConWrite;

				cport->ConWrite->io_Command = CMD_WRITE;

				cport->ConRead->io_Command = CMD_READ;
				cport->ConRead->io_Data = (APTR)cport->ConBuf;
				cport->ConRead->io_Length = 1;
				SendIO((IORequest *)cport->ConRead);
				cport->ConReadSend=TRUE;
	
				if ((cport->winmenu = CreateMenus(cport->mymenu, TAG_DONE)))
				{
					if (!(cport->vinfo = GetVisualInfo(cport->win->WScreen, TAG_DONE)))
						CloseIt(20,"Can't get visualinfo");
					if (!(LayoutMenus(cport->winmenu,cport->vinfo,TAG_DONE, TAG_DONE)))
						CloseIt(20,"Can;t Layout menu");
					SetMenuStrip(cport->win,cport->winmenu);
					ResetMenuStrip(cport->win,cport->winmenu);
				}
				else
				{
					ioprintf("Can't create io menu.\n");
				}
	
	//			ClearIOBuffer();
			}
		}
		else
		{
			if (cport->scr)ScreenToFront(cport->scr);
			if (cport->win)
			{
				WindowToFront(cport->win);
				ActivateWindow(cport->win);
			}
		}
	}
}

	
__saveds ASM void nslib_Monitor_close()
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
	{
		if (cport->ConRead)
		{
			CloseDevice((IORequest *)cport->ConRead);
		}
		if (cport->win)
		{
			ClearMenuStrip(cport->win);
			FreeMenus(cport->winmenu);
			FreeVisualInfo(cport->vinfo);
			CloseWindow(cport->win);
			cport->win=NULL;
		}
		if (cport->scr)
		{
			CloseScreen(cport->scr);
			cport->scr=NULL;
		}
		cport->MonitorOpen=FALSE;
	}
}


__saveds ASM void nslib_DateToString(register __a0 const char *buffer, register __a1 struct Date *date, register __d0 ULONG flags)
{
	if (date->year && date->year <= 99)  // CNet Year 2000 bug ??
	{
		date->year = date->year + 1900;
	}
	if ((!flags) | ((flags & DATEFLAG_USEDATE) && (flags & DATEFLAG_USETIME)))
	{
		Date actualdate;
		GetTime(&actualdate);
		if (date->day == actualdate.day && date->month == actualdate.month && date->year == actualdate.year)
		{
			sprintf((char *)buffer,"Today      %02d:%02d:%02d",
					date->hours,
					date->mins,
					date->secs);			
		}
		else
		{
			actualdate.datecode = actualdate.datecode - 86400;
			Datecode2Date(&actualdate);
			if (date->day == actualdate.day && date->month == actualdate.month && date->year == actualdate.year)
			{
				sprintf((char *)buffer,"Yesterday  %02d:%02d:%02d",
						date->hours,
						date->mins,
						date->secs);			
			}
			else
				sprintf((char *)buffer,"%02d-%02d-%04d %02d:%02d:%02d",date->day,date->month,date->year,date->hours,date->mins,date->secs);
		}
	}
	else
	{
		if (flags & DATEFLAG_USEDATE)
		{
			Date actualdate;
			GetTime(&actualdate);
			if (date->day == actualdate.day && date->month == actualdate.month && date->year == actualdate.year)
			{
				sprintf((char *)buffer,"Today     ");
			}
			else
			{
				actualdate.datecode = actualdate.datecode - 86400;
				Datecode2Date(&actualdate);
				if (date->day == actualdate.day && date->month == actualdate.month && date->year == actualdate.year)
					sprintf((char *)buffer,"Yesterday ");
				else
					sprintf((char *)buffer,"%02d-%02d-%04d", date->day, date->month, date->year);
			}
		}
		else
		{
			if (flags & DATEFLAG_USETIME)
			{
				sprintf((char *)buffer,"%02d:%02d:%02d",
						date->hours,
						date->mins,
						date->secs);
			}
		}
	}
}


__saveds ASM void nslib_StringToDate(register __a0 const char *buffer, register __a1 struct Date *date, register __d0 ULONG flags)
{
	char *pos=(char *)buffer;
	ULONG	day	= 0;
	ULONG	month	= 0;
	ULONG	year	= 0;
	ULONG	mins	= 0;
	ULONG	hours	= 0;
	ULONG	secs	= 0;
	ULONG ulong;

	#define DATEFORMAT_EUROPE		1
		// 31.12.1999
	#define DATEFORMAT_US			2
		// 12/31/999
	#define DATEFORMAT_TIMEONLY	3

	char	dateformat;

	while ((*pos<'0' | *pos>'9') && *pos)pos++;			// day
	if (*pos)StrToLong(pos,(LONG *)&day);			
	while (*pos>='0' && *pos<='9' && *pos)pos++;


	if ((ulong=SearchDateKey(pos))>=10)						// month
	{
		month=ulong-10;
	}
	else
	{
		while ((*pos<'0' | *pos>'9') && *pos)pos++;
		if (*pos)StrToLong(pos,(LONG *)&month);
		while (*pos>='0' && *pos<='9' && *pos)pos++;
	}


	while ((*pos<'0' | *pos>'9') && *pos)pos++;			// year
	if (*pos)StrToLong(pos,(LONG *)&year);
	while (*pos>='0' && *pos<='9' && *pos)pos++;

	while ((*pos<'0' | *pos>'9') && *pos)pos++;			// hours
	if (*pos)StrToLong(pos,(LONG *)&hours);
	while (*pos>='0' && *pos<='9' && *pos)pos++;

	while ((*pos<'0' | *pos>'9') && *pos)pos++;			// mins
	if (*pos)StrToLong(pos,(LONG *)&mins);
	while (*pos>='0' && *pos<='9' && *pos)pos++;

	while ((*pos<'0' | *pos>'9') && *pos)pos++;			// secs
	if (*pos)StrToLong(pos,(LONG *)&secs);

	pos=(char *)buffer;
	while (*pos>='0' && *pos<='9' && *pos)pos++;

	if (*pos=='-' | *pos=='.' | *pos==' ')					// canadian, europe, uucp... date format
		dateformat=DATEFORMAT_EUROPE;
	else
	{
		if (*pos==':')										// check for time online
		{
			if (!hours && !mins && !secs && year<60)
			{
				hours=day;
				mins=month;
				secs=year;
				day=0;month=0;year=0;
				dateformat=DATEFORMAT_TIMEONLY;
			}
		}
	}
	if (dateformat!=DATEFORMAT_TIMEONLY)
	{
		if (year<100 && month>0 && day>0)
		{
			if (year>=78)
				year=year+1900;
			else
				year=year+2000;
		}
		if (*pos=='/')
		{
			ULONG ulong=day;
			day=month;
			month=ulong;
		}
	}
	date->day	=(UBYTE)day;
	date->month	=(UBYTE)month;
	date->year	=year;
	date->hours	=(UBYTE)hours;
	date->mins	=(UBYTE)mins;
	date->secs	=(UBYTE)secs;
	CalcDatecode(date);
}


__saveds ASM char *nslib_getstr(register __d0 UWORD group, register __d1 UWORD stringno)
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	if (cport)
	{
		if (group<cport->strgroups)
		{
			if (stringno<cport->strgroup[group].entrys)
				if (cport->strgroup[group].table[stringno])
					return cport->strgroup[group].table[stringno];
				else
					return empty_string;
			else
				return empty_string;
		}
		else
			return empty_string;
	}
}


__saveds ASM ULONG nslib_SearchDateKey(register __a0 char *str)
{
	if (searchstr(str,"mon")) return 1;
	if (searchstr(str,"tue")) return 2;
	if (searchstr(str,"wed")) return 3;
	if (searchstr(str,"thu")) return 4;
	if (searchstr(str,"fri")) return 5;
	if (searchstr(str,"sat")) return 6;
	if (searchstr(str,"sun")) return 7;

	if (searchstr(str,"jan")) return 11;
	if (searchstr(str,"feb")) return 12;
	if (searchstr(str,"mar")) return 13;
	if (searchstr(str,"apr")) return 14;
	if (searchstr(str,"may")) return 15;
	if (searchstr(str,"jun")) return 16;
	if (searchstr(str,"jul")) return 17;
	if (searchstr(str,"aug")) return 18;
	if (searchstr(str,"sep")) return 19;
	if (searchstr(str,"oct")) return 20;
	if (searchstr(str,"nov")) return 21;
	if (searchstr(str,"dec")) return 22;
	return(NULL);
}

__saveds ASM ULONG nslib_opengui()
{
	LONG result = NULL;
	struct Task *mytask = FindTask(NULL);
	struct PortData *cport=(PortData *)mytask->tc_UserData;
	if (cport->systemservice)
	{
//		if (mytask == MainTask)
		{
			if (!guifrontend.GUIOpen)
			{
				printf("start gui\n");	
				StartGUI();
			}
			else
			{
				printf("ERROR_GUI_ALREADY_OPENED\n");
				result = ERROR_GUI_ALREADY_OPENED;
			}
		}
//		else
//		{
//			printf("ERROR_CANT_START_FROM_MAINTASK_ONLY\n");
//			result = ERROR_CANT_START_FROM_MAINTASK_ONLY;
//		}
	}
	else
	{
		printf("ERROR_ACCESS_DENIED\n");
		result = ERROR_ACCESS_DENIED;
	}
	return(result);
}

__saveds ASM ULONG nslib_closegui()
{
	LONG result = NULL;
	struct Task *mytask = FindTask(NULL);
	struct PortData *cport=(PortData *)mytask->tc_UserData;
	if (cport->systemservice)
	{
		if (mytask == MainTask)
		{
			if (guifrontend.GUIOpen && GUITask)
			{
				guifrontend.ProgramClose=TRUE;
				Signal(GUITask,GUITask->tc_SigWait);
				while (guifrontend.GUIOpen)
				{
					Delay(5);
				}
			}
			else
				result = ERROR_GUI_NOT_OPENED;
		}
		else
			result = ERROR_CANT_START_FROM_MAINTASK_ONLY;			
	}
	else
		result = ERROR_ACCESS_DENIED;
	return(result);
}

