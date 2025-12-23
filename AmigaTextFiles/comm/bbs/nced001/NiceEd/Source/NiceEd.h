
/*
** Flags format in file:
**
** 01234567 "bits"
** X--XX--X
**
** Saves as:  1*1 + 0*2 + 0*4 + 1*8 + 1*16 + 0*32 + 1*64 + 0*128 = 89
*/

struct Flags
{
	int flag[8];
};
typedef struct Flags FlagsType;

struct User
{
	char *Name;
	char *Street;
	char *City;
	char *Province;
	char *Phone;
	char *Computer;
	char *Birthdate;	/* YYMMDD */
	char *Postalcode;
	char *Handle;
	char *Lastcall;		/* YYMMDD */
	char *Password;		/* Max 10 chars! */
	int Kbdl;
	int Filesdl;
	int Kbul;
	int Filesul;
	int Messages;
	int Calls;
	int Screenlength;
	int Fileratio;
	int Timelimit;
	int Unusedtime;
	int Commandmode;	/* 0=stacking, 1=hotkeys    */
	int Graphicsmode;	/* 0=ASCII,    1=ANSI       */
	int Editor;			/* 0=line,     1=Fullscreen */
	int Accesslevel;
	int Netmailcredit;
	FlagsType *Flags;
	int Help;			/* 0=novice,   1=expert     */
	int Usernumber;		/* Must not be changed!     */
	/* 10 blank lines for padding... */
};
typedef struct User UserType;
	
