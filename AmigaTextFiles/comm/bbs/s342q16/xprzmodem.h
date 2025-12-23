/*
 *  xprzmodem.h: Definitions for xprzmodem.library;
 *  Version 3.0, 05 Apr 1995, by A. Preston for Citadel
 *  Version 2.0, 28 October 1989, by Rick Huebner.
 *  Released to the Public Domain; do as you like with this code.
 */

/* #define DEBUGLOG 1 */

/* Return codes */
#define OK        0
#define ZM_ERROR   (-1)
#define TIMEOUT (-2)
#define RCDO    (-3)

/* Relevant control characters */
#define CR      ('M' & 0x1F)    /* ^M */
#define ZM_XON     ('Q' & 0x1F)    /* ^Q */
#define ZM_XOFF    ('S' & 0x1F)    /* ^S */
#define ZM_CAN     ('X' & 0x1F)    /* ^X */
#define ZM_CPMEOF  ('Z' & 0x1F)    /* ^Z */

/* Misc. program constants */
#define LZMANAG           0     /* Default ZMODEM file management mode */
#define LZTRANS           0     /* Default ZMODEM file transport mode */
#define PATHLEN         256     /* What's the max legal path size? */
#define CONFIGLEN        32     /* Max length of transfer options string */
#define KSIZE          1024     /* Max allowable packet size */
#define MINBLOCK         64     /* Min allowable packet size */
#define MAXGOODNEEDED  8192     /* Max # good bytes req'd to bump packet size */

/* Provision for future 7-bit ZMODEM; for now, there's no difference */
#define sendline xsendline

/*
 * Replacement for global variables normally used, in order to make code
 * fully reentrant; each invocation allocs their own Vars, and passes the
 * struct pointer down through the entire program so they're always available.
 * Pointer to this struct is usually able to be a register variable, so access
 * is no worse than any stack variable (all register-relative).  Kinda
 * kludgey, but the original ZModem code design depended on lots of globals,
 * and I didn't want to redesign the whole damn thing.  Besides, it's more
 * efficient than constantly pushing & popping args all over the place.
 */

struct Vars
{
  struct XPR_IO io;             /* Copy of XProto IO struct passed by term prog. */
  struct XPR_UPDATE __aligned xpru;       /* Scratchpad xpr_update() control struct */
  struct timeval    __aligned Starttime;  /* Time transfer started */
  UBYTE             __aligned Rxhdr[4];   /* Received header */
  UBYTE Txhdr[4];               /* Transmitted header */
  UBYTE Msgbuf[128];            /* Scratchpad buffer for printing messages */
  UBYTE Filename[PATHLEN];      /* Name of the file being up/downloaded */
  UBYTE Pktbuf[KSIZE];          /* File data packet buffer */
  UBYTE Modembuf[KSIZE * 4 + 256];      /* Input buffer for data from modem */
  UBYTE Outbuf[KSIZE * 4 + 256];        /* Output buffer for data to modem */
  UBYTE *Modemchar;             /* Next char to get from Modembuf */
  UBYTE *Filebuf;               /* File I/O buffer address */
  UBYTE *Filebufptr;            /* Current position within Filebuf */
  ULONG Modtime;                /* Modification data. */
  ULONG Bits;                   /* Attribute bits. */
  ULONG BytesSent;              /* How many bytes sent since last check. */
  ULONG BytesReceived;          /* How many bytes received since last check. */
  LONG Receiving;               /* Receiving or sending? */
  FILE  *File;                  /* Handle of file being transferred */
  long Oldstatus;               /* Original terminal program's modem settings */
  long Baud;                    /* BPS setting of modem */
  long Strtpos;                 /* Starting byte position of transfer */
  long Fsize;                   /* Size of file being transferred */
  long Rxbytes;                 /* Number of bytes received so far */
  long Filebufpos;              /* File offset of data in Filebuf */
  long Filebufmax;              /* Size of Filebuf */
  long Filebuflen;              /* Number of bytes currently stored in Filebuf */
  long Filebufcnt;              /* Number of bytes remaining/written in Filebuf */
  long Rxpos;                   /* Received file position */
  long Txpos;                   /* Transmitted file position */
  short Filcnt;                 /* Number of files opened for transmission */
  short Errcnt;                 /* Number of files unreadable */
  short Noroom;                 /* Flags 'insufficient disk space' errors */
  short Rxbuflen;               /* Largest frame they're willing to xfer */
  short Tframlen;               /* Largest frame we're willing to xfer */
  short Rxtimeout;              /* Tenths of seconds to wait for something */
  short Tryzhdrtype;            /* Header type to send corresp to Last rx close */
  short Modemcount;             /* Number of bytes available in Modembuf */
  short Outbuflen;              /* Number of bytes currently stored in Outbuf */
  short Rxframeind;             /* ZBIN or ZHEX; type of frame received */
  short Txfcs32;                /* TRUE means send binary frame with 32 bit FCS */
  short Rxflags;                /* Temp register */
  short Wantfcs32;              /* want to send 32 bit FCS */
  short Crc32t;                 /* Display flag indicates 32 bit CRC being sent */
  short Crc32;                  /* Display flag indicates 32 bit CRC being recd */
  short Rxtype;                 /* Type of header received */
  short Rxcount;                /* Count of data bytes received */
  short Znulls;                 /* Number of nulls to send at begin of ZDATA hdr */
  short ErrorLimit;             /* How many sequential errors before abort */
  short Zctlesc;                /* Are ctl chars to be escaped? */
  short FileAttributes;         /* Bit 0: send file attributes, bit 1: honor file attributes received. */
  char SetAttributes;           /* Set file attributes and time? */
  char Rxbinary;                /* Force binary mode download? */
  char Rxascii;                 /* Force text mode download? */
  char Thisbinary;              /* Receive this file in binary mode? */
  char Lzconv;                  /* Suggested binary/text mode for uploads */
  char Eofseen;                 /* Text-mode EOF marker (^Z) rec'd on download? */
  UBYTE Zconv;                  /* ZMODEM file conversion request */
  UBYTE Zmanag;                 /* ZMODEM file management request */
  UBYTE Ztrans;                 /* ZMODEM file transport request */
  UBYTE Lastsent;               /* Last text char written by putsec() */
  UBYTE Lastzsent;              /* Last char sent by zsendline() */
  UBYTE Fileflush;              /* Flush file I/O buffer before closing? */
  UBYTE Attn[ZATTNLEN + 1];     /* Attention string rx sends to tx on err */
  LONG ksize;                   /* Max allowable packet size */
};

/*
 * Option settings and other variables needed outside of XProtocolSend/Receive;
 * separated from rest of Vars so that huge Vars struct doesn't have to be
 * allocated except during transfers.  Pointer to this struct kept in xpr_data.
 */
struct SetupVars
{
  UBYTE *matchptr, *bufpos;
  short buflen;
  UBYTE option_t[2], option_o[2], option_b[8], option_f[8], option_e[8],
    option_s[4];
  UBYTE option_c[4], option_r[4], option_a[4], option_d[4], option_k[4],
    option_p[256], option_m[8];
  UBYTE option_i[2];
};

/* Function prototypes */

long XProtocolSend( struct XPR_IO *xio);
short getzrxinit(struct Vars *v);
short sendzsinit(struct Vars *v);
void sendbatch(struct Vars *v);
short sendone(struct Vars *v);
short sendname(struct Vars *v);
short zsendfile(struct Vars *v, short blen);
short zsendfdata(struct Vars *v);
short getinsync(struct Vars *v);
void saybibi(struct Vars *v);

long  XProtocolReceive( struct XPR_IO *xio);
short rcvbatch(struct Vars *v);
short tryz(struct Vars *v);
short rzfiles(struct Vars *v);
short rzfile(struct Vars *v);
short procheader(struct Vars *v);
short putsec(struct Vars *v);
void ackbibi(struct Vars *v);

long XProtocolSetup(struct XPR_IO *xio);
long XProtocolCleanup(struct XPR_IO *xio);
long XProtocolHostMon(struct XPR_IO *xio, char *serbuff, long actual, long maxsize);
long XProtocolUserMon(struct XPR_IO *xio, char *serbuff, long actual, long maxsize);
struct Vars *setup(struct XPR_IO *io);
UBYTE *find_option(UBYTE * buf, UBYTE option);
void set_textmode(struct Vars *v);
void canit(struct Vars *v);
void zmputs(struct Vars *v, UBYTE * s);
void xsendline(struct Vars *v, UBYTE c);
void sendbuf(struct Vars *v);
short readock(struct Vars *v, short tenths);
char char_avail(struct Vars *v);
void update_rate(struct Vars *v);
FILE * bfopen(struct Vars *v, UBYTE * mode);
void bfclose(struct Vars *v);
void bfseek(struct Vars *v, long pos);
long bfread(struct Vars *v, UBYTE * buf, long length);
long bfwrite(struct Vars *v, UBYTE * buf, long length);
void ioerr(struct XPR_IO *io, char *msg);
void upderr(struct Vars *v, char *msg);
void updmsg(struct Vars *v, char *msg);
long getfree(struct Vars *v);
char exist(struct Vars *v);

void mysprintf(STRPTR, STRPTR,...);
LONG Atol(STRPTR buffer);
void kprintf(STRPTR,...);

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;
extern struct Library *UtilityBase;
extern struct Library *TimerBase;
extern LONG GMT_Offset;

void zsbhdr(struct Vars *v, USHORT type);
void zshhdr(struct Vars *v, USHORT type);
void zsdata(struct Vars *v, short length, USHORT frameend);
short zrdata(struct Vars *v, UBYTE * buf, short length);
short zrdat32(struct Vars *v, UBYTE * buf, short length);
short zrdatr32(struct Vars *v, UBYTE * buf, short length);
short zgethdr(struct Vars *v);
short zrbhdr(struct Vars *v);
short zrbhdr32(struct Vars *v);
short zrhhdr(struct Vars *v);
void zputhex(struct Vars *v, UBYTE c);
void zsendline(struct Vars *v, UBYTE c);
short zgethex(struct Vars *v);
short zdlread(struct Vars *v);
short noxrd7(struct Vars *v);
void stohdr(struct Vars *v, long pos);
long rclhdr(struct Vars *v);
void xpr_fwrite(char *, long, long, FILE *);

/*
 * # seconds between 1-1-70 (Unix time base) and 1-1-78 (Amiga time base).
 * Add this value to the returned seconds count to convert Amiga system time
 * to normal Unix system time.
 */

#define UTC_OFFSET 252482400

ULONG getsystime(struct timeval *tv);

/* End of XprZmodem.h source */
