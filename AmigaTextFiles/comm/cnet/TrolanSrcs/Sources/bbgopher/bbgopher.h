/*
 * $VER: bbgopher.h 1.0 (20.4.95)
 *
 * BBGopher/amiga header file.
 */

/* Version Information */
char *VersTag = "\0$VER: bbgopher/CNet 1.0 (4.11.96)";

/* Defaults */
#define CONFIGPATH      "SysData:bbgopher.conf"
#define DEFAULTTITLE    "BBGopher/CNet 1.0 (%s)"
#define DEFAULTHOST     "gopher"
#define DEFAULTPORT     70
#define DEFAULTPAGER    ""
#define DEFAULTTELNET   "telnet"
#define DEFAULTTN3270   "tn3270"
#define DEFAULTGIF      ""
#define DEFAULTIMG      ""
#define DEFAULTLINES    19
#define DEFAULTDIR      ""
#define MAXSTACKLEN     250
#define MAXMENULEN      2500

#define G_OK      0
#define G_HOST    1
#define G_SOCKET  2
#define G_CONNECT 3
#define G_STACK   4
#define G_MENU    5
#define G_MEMORY  6
#define G_CREATE  7

typedef struct {
   char  type;
   char  *display;
   char  *selector;
   char  *hostname;
   short port;
   } GO4ENTRY;

typedef struct {
   char type;
   char *display;
   char acf [ 128 ];
   char supported;
   } TYPEENTRY;

TYPEENTRY KnownTypes[] = {
   { '0', "TXT", "", 1 },
   { '1', "DIR", "", 1 },
   { '2', "CSO", "", 0 },
   { '3', "ERR", "", 0 },
   { '4', "HQX", "", 1 },
   { '5', "DOS", "", 1 },
   { '6', "UUE", "", 1 },
   { '7', "IDX", "", 1 },
   { '8', "TEL", "", 1 },
   { '9', "BIN", "", 1 },
   { '+', "DUP", "", 0 },
   { 'g', "GIF", "", 0 },
   { 'I', "PIC", "", 0 },
   { 'T', "TN3", "", 1 },
   { 'i', "INF", "", 1 },
   { 0, 0 }
   };

char  Title     [ 256 ];
char  DefHost   [ 256 ];
char  HelpFile  [ 256 ];
char  Pager     [ 256 ];
char  Telnet    [ 256 ];
char  TN3270    [ 256 ];
char  GIFViewer [ 256 ];
char  IMGViewer [ 256 ];
char  Directory [ 256 ];
char  BookMark  [ 256 ];
char  BadPorts  [ 256 ];
short DefPort;
short Lines;
short Twirly;
short SkipACF;
short SkipType;
short ClrScreen;

GO4ENTRY Stack [ MAXSTACKLEN ];

short StackLen;

GO4ENTRY Menu [ MAXMENULEN ];

short MenuLen;

struct sockaddr_in sain;
struct hostent *host;

int sock;
FILE *sd;
int BM;

#define NUMTWIRLYCHARS 4
char TwirlyChars [ NUMTWIRLYCHARS + 1 ] = "-\\|/";

/* Function prototypes */
int ValidNumber(int number, char *string);
int CheckUser(char type);
int SupportedType(char type);
int newEntry(GO4ENTRY *ent, char type, char *display, char *selector, char *hostname, short port);
int delEntry(GO4ENTRY *ent);
char *TypeLookup(char type);
int DisplayLookup(char *display);
int Connect(char *selector, char *hostname, short port);
void Disconnect(void);
int Page(char *file);
int GetData(char *file, int binary);
int GetFile(char *selector, char *hostname, short port, char *type, int binary);
int MailFile(char *filename);
int SaveFile(char *filename);
int bb_ReadText(char *selector, char *hostname, short port);
int DoSearch(char *selector, char *hostname, short port);
int ReadMenu(char *display, char *selector, char *hostname, short port, int connect, FILE *fd);
void SaveBookmark(int n);
void DelBookmark(int n);
int CompareEntries(GO4ENTRY *ent1, GO4ENTRY *ent2);
int ReadBookmarks(void);
int ProcessMenu(void);
int Setup(void);
