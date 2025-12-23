#define VERTAG "v1.0"

#define MAIL  0x01      /* For Flags */
#define RESP  0x02
#define GFILE 0x04
#define PFILE 0x08
#define ULOAD 0x10
#define DLOAD 0x20
#define HIDE  0x40
#define XXXXX 0x80

struct LT
{
   short  id;              /* Account ID#    */ /*  2 */
   short  port;            /* Port on        */ /*  4 */
   short  baud;            /* Baud rate      */ /*  6 */
   short  calls;           /* Calls to system*/ /*  8 */
   short  ctype;           /* Computer Type  */ /* 10 */
   short  timeon;          /* Time online    */ /* 12 */
   char   from [ 14 ];     /* User from      */ /* 26 */
   struct IsDate logon;    /* Date of logon  */ /* 32 */
   UBYTE  flags;           /* RPGUDM flags   */ /* 33 */
}; /* LT */
