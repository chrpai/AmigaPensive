
#define PREFSID_SORTIN MAKE_ID('S','O','R','T')

#define SUBID_SORTIN_PREFS (0)

typedef struct sortinprefs {
	char spool[ 128 ];
	char trash[ 128 ];
	ULONG cache_ix;                 /* Maximum Number of Indexes to cache */
	ULONG cache_bf;                 /* Read Buffer Size (def: 16k) */
	ULONG cache_msg;                /* Maximum msg buffer size */
	ULONG batch_kb;                 /* Batchen nach soundsoviel Daten */

	UBYTE conv_concurrent;  /* Flag: Converter may run concurrently */
	UBYTE conv_quit;                /* Flag: Converter may shut down if everything is converted fine */
	UBYTE sortin_quit;
	UBYTE do_gate;                  /* Flag: Gateway-Zeile erzeugen */

	char    trashname[ 40 ];        /* Default-Unzustellbar: Brett oder User */
	char    defcensor[ 40 ];        /* Default-Zensor: Brett oder User */

	ULONG news_maxage;                      /* Maximales Alter für Newsbatches */
	ULONG mail_maxhops;
	ULONG mail_maxsendback;

	UBYTE gateid[ 4 ];                      /* obsolete */

	UBYTE domailstat;        /* Flag: Enable full mail logging */
	UBYTE donewsstat;        /* Flag: Enable full news logging */

	UBYTE prearc_everytime;  /* Flag: If active, batch routines will be called when there */
				 /* are no entries in the spool directory left, batch_kb will */
				 /* be overridden */
	UBYTE dopresortbatch;    /* Flag: CONNECTLINE:S/presortin,batch */
	UBYTE dopostsortbatch;   /* Flag: CONNECTLINE:S/postsortin.batch */
	UBYTE doprearcbatch1;    /* Flag: CONNECTLINE:S/prearc.batch1 */
	UBYTE doprearcbatch2;    /* Flag: CONNECTLINE:S/prearc.batch2 */
	UBYTE do_hybrid;         /* Flag: Changes From_ appearance to hybrid form */

} sortinprefs;
