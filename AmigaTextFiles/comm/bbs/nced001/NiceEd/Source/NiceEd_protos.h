/* Prototypes for functions defined in
NiceEd.c
 */

extern char const version[];

extern struct ReqToolsBase *ReqToolsBase;

extern struct Screen *screen;

extern struct Window *window;

extern struct Gadget *glist;

extern struct Gadget *name;

extern struct Gadget *usernumber;

extern struct Gadget *kbdl;

extern struct Gadget *kbul;

extern struct Gadget *handle;

extern struct Gadget *fidl;

extern struct Gadget *fiul;

extern struct Gadget *street;

extern struct Gadget *birth;

extern struct Gadget *lastcall;

extern struct Gadget *computer;

extern struct Gadget *msgs;

extern struct Gadget *calls;

extern struct Gadget *postalcode;

extern struct Gadget *scrlength;

extern struct Gadget *netmail;

extern struct Gadget *city;

extern struct Gadget *command;

extern struct Gadget *graphics;

extern struct Gadget *province;

extern struct Gadget *editor;

extern struct Gadget *help;

extern struct Gadget *phone;

extern struct Gadget *password;

extern struct Gadget *flags[];

extern struct Gadget *accesslevel;

extern struct Gadget *ratio;

extern struct Gadget *timelimit;

extern struct Gadget *unused;

extern struct Gadget *next;

extern struct Gadget *prev;

extern struct Gadget *save;

extern struct Gadget *choose;

extern struct Gadget *text;

extern struct Gadget *add;

extern struct Gadget *new;

extern struct Gadget *normal;

extern APTR vi;

extern int allusers;

void main(int , char **);

struct List * scanusers(char *);

BOOL addnode(struct List *, char *);

void freelist(struct List *);

void freenode(struct Node *);

struct Window * getwindow(void);

void shutdown(void);

UserType * readuser(struct Node *, char *);

char * getstring(FILE *);

int getint(FILE *);

FlagsType * getflags(FILE *);

FlagsType * unpackflags(int );

int packflags(FlagsType *);

void displayuser(UserType *);

void writeuser(UserType *, char *);

void setwintitle(char *);

void showusers(struct List *, char *, char *);

struct Node * choosewin(struct List *);

struct Node * getnodenumber(struct List *, UWORD );

char * getnewname(char *);

struct Node * findnode(struct List *, char *);

unsigned long really(char *, char *, char *, APTR );

BOOL deleteuser(struct Node *, UserType *, struct List *, char *);

int __stdargs rawsprintf(char *, char *, ...);

