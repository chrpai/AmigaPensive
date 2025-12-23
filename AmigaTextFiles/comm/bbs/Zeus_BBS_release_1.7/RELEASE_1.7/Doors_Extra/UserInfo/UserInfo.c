#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/pipeline.h>
#include <string.h>
#include <stdio.h>

struct Library *PipelineBase;
struct User *u;

UBYTE *Months[] =
{
	"Jan",	"Feb",	"Mar",	"Apr",	"May",	"Jun",
	"Jul",	"Aug",	"Sep",	"Oct",	"Nov",	"Dec",
};

UBYTE *Genders[] =
{
	"Unknown",
	"Male",
	"Female",
};

int main(int argc, char *argv[])
{
struct tm *FirstCall;
struct tm *LastCall;

	if (argc < 2 || !strcmp(argv[1], "?"))
	{
		Printf("Syntax: UserInfo <User name>");
		exit(RETURN_WARN);
	}

	if (PipelineBase = OpenLibrary("zeus.library", 0))
	{
		if (u = LoadUser(argv[1]))
		{
			Printf("User info for %s:\n", u->ur_Name);

			FirstCall = gmtime(&u->ur_FirstCall);
			LastCall = gmtime(&u->ur_LastCall);

			printf( "Alias......: %-40s  First call: %02d-%3s-19%02d\n"
				"Voice phone: %-40s  Last call.: %02d-%3s-19%02d\n"
				"Data phone.: %-40s  Last baud.: %ld\n"
				"Street addr: %-40s  Expire....: %ld\n"
				"Town.......: %-40s  Credit....: %ld\n"
				"County.....: %-40s  DOB.......: %02d-%3s-%04ld\n"
				"Post code..: %-40s\n"
				"Country....: %-40s\n\n"
				"More.......: %-40s  Valid......: %s\n"
				"Expert.....: %-40s  Want RIP...: %s\n"
				"Hot keys...: %-40s  Deleted....: %s\n"
				"No 8-bit...: %-40s  Amiga ANSI.: %s\n"
				"Validated..: %-40s  Protected..: %s\n"
				"Clear scr..: %-40s  No restrict: %s\n"
				"Log user...: %-40s  ANSI pos...: %s\n"
				"ANSI Color.: %-40s  ANSI Comp..: %s\n"
				"CRLF.......: %-40s  Anim prompt: %s\n"
				"ANSI Music.: %-40s\n\n"

				"Access.....: %-40d  Mins left..: %ld\n"
				"Num calls..: %-40ld  Max call/d.: %ld\n"
				"Max mins/ca: %-40ld  Max min/day: %ld\n"
				"Computer no: %-40d  Calls today: %d\n"
				"Scr width..: %-40d  Scr height.: %d\n"
				"Menu set...: %-40d  Language...: %d\n"
				"Gender.....: %s\n",
				u->ur_Alias,
				FirstCall->tm_mday, Months[FirstCall->tm_mon], FirstCall->tm_year,
				u->ur_VoicePhone,
				LastCall->tm_mday, Months[LastCall->tm_mon], LastCall->tm_year,
				u->ur_DataPhone,
				u->ur_LastBaudRate,
				u->ur_Street,
				u->ur_Expire,
				u->ur_Town,
				u->ur_Credit,
				u->ur_County,
				u->ur_DOB_Day, Months[u->ur_DOB_Mon], u->ur_DOB_Year,
				u->ur_PostCode,
				u->ur_Country,
				(u->ur_Flags & UF_MORE ? "Yes" : "No"), (u->ur_Flags & UF_VALID ? "Yes" : "No"),
				(u->ur_Flags & UF_EXPERT ? "Yes" : "No"), (u->ur_Flags & UF_WANTRIP ? "Yes" : "No"),
				(u->ur_Flags & UF_HOTKEYS ? "Yes" : "No"), (u->ur_Flags & UF_DELETED ? "Yes" : "No"),
				(u->ur_Flags & UF_NO8BIT ? "Yes" : "No"), (u->ur_Flags & UF_AMIGAANSI ? "Yes" : "No"),
				(u->ur_Flags & UF_VALIDATED ? "Yes" : "No"), (u->ur_Flags & UF_PROTECTED ? "Yes" : "No"),
				(u->ur_Flags & UF_CLRSCR ? "Yes" : "No"), (u->ur_Flags & UF_NORESTRICT ? "Yes" : "No"),
				(u->ur_Flags & UF_LOGUSER ? "Yes" : "No"), (u->ur_Flags & UF_ANSIPOS ? "Yes" : "No"),
				(u->ur_Flags & UF_ANSICOLR ? "Yes" : "No"), (u->ur_Flags & UF_ANSICOMP ? "Yes" : "No"),
				(u->ur_Flags & UF_CRLF ? "Yes" : "No"), (u->ur_Flags & UF_ANIMPROMPTS ? "Yes" : "No"),
				(u->ur_Flags & UF_ANSIMUSIC ? "Yes" : "No"),
				u->ur_Access,
				u->ur_MinsLeft,
				u->ur_NumCalls,
				u->ur_MaxCallsPerDay,
				u->ur_MaxMinsPerCall,
				u->ur_MaxMinsPerDay,
				u->ur_Computer,
				u->ur_CallsToday,
				u->ur_ScrWidth,
				u->ur_ScrHeight,
				u->ur_MenuSet,
				u->ur_Language,
				Genders[u->ur_Gender]);

		}
		CloseLibrary(PipelineBase);
	}
}
