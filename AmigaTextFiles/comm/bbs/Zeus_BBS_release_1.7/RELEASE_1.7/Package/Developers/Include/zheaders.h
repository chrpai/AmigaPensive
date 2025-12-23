
#define	MAKE_ID(a,b,c,d)	\
	((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))

/* Files */

#define	FSVD	MAKE_ID ( 'F', 'S', 'V', 'D' ) /* struct FileSavedData */
#define	FRAT	MAKE_ID ( 'F', 'R', 'A', 'T' ) /* struct FileRatioData */

/** Now defunct **/
#define	FACC	MAKE_ID ( 'F', 'A', 'C', 'C' ) /* AreaAccess back2back */

#define FCHG	MAKE_ID ( 'F', 'C', 'H', 'G' ) /* ExtAreaAccess back2back */
#define FMRK	MAKE_ID ( 'F', 'M', 'R', 'K' ) /* struct Mark bak2bak  */

/* Messages */

#define MCHG	MAKE_ID ( 'M', 'C', 'H', 'G' ) /* ExtAreaAccess back2back */

/** Now defunct **/
#define	MACC	MAKE_ID ( 'M', 'A', 'C', 'C' ) /* List - struct AANode */
#define	MSVD	MAKE_ID ( 'M', 'S', 'V', 'D' ) /* struct MsgSavedData  */
#define MPTR	MAKE_ID ( 'M', 'P', 'T', 'R' ) /* struct HighPointer   */

/* QWK */

#define QWKA	MAKE_ID ( 'Q', 'W', 'K', 'A' ) /* struct QwkConfig */
#define QWKC	MAKE_ID ( 'Q', 'W', 'K', 'C' ) /* struct QwkArea(s) */

/* HEADERS */

#define FHDR	MAKE_ID ( 'F', 'H', 'D', 'R' ) /* text */
#define XHDR    MAKE_ID ( 'X', 'H', 'D', 'R' )
#define RHDR    MAKE_ID ( 'R', 'H', 'D', 'R' )

/* User banner */

#define BANN	MAKE_ID ( 'B', 'A', 'N', 'N' ) /* text */
#define NOTE    MAKE_ID ( 'N', 'O', 'T', 'E' ) /* text */

