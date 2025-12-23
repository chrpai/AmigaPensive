#define FLUT_PORT		"Flut!v4"

#define MSG_INIT	1
#define MSG_QUIT	2
#define MSG_TURN	3
#define MSG_GAME	4
#define MSG_SCORES	5
#define MSG_MESS	6

struct famestruct
{
	char	news[15][105],
			hall[2][10][37];
};

struct userstruct
{
	LONG	userid;							/* welcher user */
	int		points[2];						/* wieviel points */
};

struct playerstruct
{
	LONG	userid;							/* welcher user? */
	ULONG	last_move;						/* wann war der letzte zug? */
	char	last_wall_lese;					/* lese: "x" gelesen: " " */
};

struct fluterstruct
{
	UBYTE	xpos,
			ypos,
			akt_user,
			fuellgrad;
};

struct olgstruct
{
	struct 	fluterstruct	fluter[481];	/* aufgelaufene "mess" (turn) */
	int		pointer;						/* wo simmer? */
	UBYTE	joined,							/* wieviel spieler hams selected? */
			akt_user,						/* wer is dran? */
			status[6];						/* wer hat gewonnen? */
};

struct gamestruct
{
	UBYTE	anzahl,							/* spieler (1-6) */
			x_size,							/* (4-20) */
			y_size,							/* (4-8) */
			status,							/* 4=normale 8=x-flut */
			border,							/* 0=noborder 1=border */
			farbe[20][8],					/* wem gehoerts */
			fuellgrad[20][8];				/* wie voll */
	BOOL	belegt;							/* haengt einer drauf? */
	struct playerstruct		player[6];		/* Userdaten */
};

struct allstruct
{
	struct userstruct		user[360];		/* Userdaten */
	struct gamestruct		game[60];		/* Gamedaten */
	struct famestruct		fame;			/* News und Hall */
	char 					mails[20][82];	/* allgemeine Messies */
	UBYTE					pointer;		/* ja wo sind wir? */
	struct olgstruct		olg[10];		/* onlinegamestruct */
};


struct messagestruct
{
	struct Message			msg;		/* Exec Message */
	long					com;		/* Command für Flut */
	BYTE					game_neu;	/* GamenummerNeu bei Anfrage */
	BYTE					game_alt;	/* GamenummerAlt bei Anfrage */
	BOOL					result;		/* Ergebnis der Anfrage */
	short					timeleft;	/* is die zeit wech??? */

	UBYTE					status[6],	/* wer gewinnt? */
							akt_user,	/* fuer wen? */
							punkte;		/* wieviel points beim turn */

	char					*name[6],   /* Namen der Mitspieler */
							pname[21],	/* Playername */
							*mess;		/* yes - die mess */

	UBYTE					pointer;	/* ja wo is der port? */
	int						olgpointer;	/* ja wo isser? */

	struct allstruct		*all;		/* Userdaten */
};

struct	MsgPort	*flutport;			/* Flutport */
struct	MsgPort	*flutreplyport;		/* ReplyPort */

struct	messagestruct	*ptr;
struct	Message			*ptrreply;
