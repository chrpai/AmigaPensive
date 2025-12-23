/*
**  $VER: SplitLog.c 2.3 (22 Aug 1996)
**
**        © 1996 Ralf Ramge, Nicolai Zizka
**
**  PROGRAMNAME:
**      SplitLog
**
**  FUNCTION:
**      split CL5.0 Logfiles
*/

#include <exec/memory.h>
#include <mui/demo.h>
#include <cl/types.h>
#include <cl/cll.h>
#include <proto/locale.h>
#include <libraries/locale.h>
#include <utility/tagitem.h>

#define LogfilePath "CONNECTLINE:Log"
#define SublogfilePath "CONNECTLINE:Log/Sublogs"
#define LogfileName "CONNECTLINE:Log/Logfile"
#define BakLogFile "Logfile-bak."

#define MSG_MMSG_PROJECT 0
#define MSG_MMSG_PROJECT_STR "Project"
#define MSG_PROGRESS 1
#define MSG_PROGRESS_STR "Progress Display"
#define MSG_TOTAL 2
#define MSG_TOTAL_STR "Completed:"
#define MSG_WAITING 3
#define MSG_WAITING_STR "<Waiting>"
#define MSG_ENTRIES 4
#define MSG_ENTRIES_STR " Entries"
#define MSG_NOMEM 5
#define MSG_NOMEM_STR "SplitLog terminated - insufficient memory!"
#define MSG_NOOPEN 6
#define MSG_NOOPEN_STR "SplitLog terminated - failed to open '%s'!"
#define MSG_NOCREATE 7
#define MSG_NOCREATE_STR "SplitLog terminated - could not create '%s'!"
#define MSG_NOLOGLIB 8
#define MSG_NOLOGLIB_STR "Error: Could not open cllog.library.\n"
#define MSG_FAILED 9
#define MSG_FAILED_STR "SplitLog terminated - DOS-Error :"

#define MMSG_ABOUT      "Info..."
#define aboutstring     "\33b\33cCL-SplitLog 2.3\33n\n© 1996 Ralf Ramge, Nicolai Zizka\n\n\33iMUI\33n © Stefan Stuntz"

/* Variablen */

ULONG filelaenge;
APTR MyPool;
char nfilename[50]="";

/* Strukturen */

typedef struct zeilen zeilen;

struct zeilen {
    char *zeile;
    zeilen *next;
    };

typedef struct logid logid;

struct logid {
    char *name;
    ULONG anzahl;
    zeilen *inhalt;
    logid *next;
    };

enum
{
    MEN_PROJECT, MEN_ABOUT
    };

static struct NewMenu MUIMenuData[]=
{
    {   NM_TITLE,   MSG_MMSG_PROJECT_STR,   0 , 0 , 0 , (APTR)MEN_PROJECT   },
    {   NM_ITEM,    MMSG_ABOUT          ,  "?", 0 , 0 , (APTR)MEN_ABOUT     },
    };

struct AppString
{
    LONG   as_ID;
    STRPTR as_Str;
};

struct AppString AppStrings[] =
{
    {MSG_MMSG_PROJECT,MSG_MMSG_PROJECT_STR},
    {MSG_PROGRESS,MSG_PROGRESS_STR},
    {MSG_TOTAL,MSG_TOTAL_STR},
    {MSG_WAITING,MSG_WAITING_STR},
    {MSG_ENTRIES,MSG_ENTRIES_STR},
    {MSG_NOMEM,MSG_NOMEM_STR},
    {MSG_NOOPEN,MSG_NOOPEN_STR},
    {MSG_NOCREATE,MSG_NOCREATE_STR},
    {MSG_NOLOGLIB,MSG_NOLOGLIB_STR},
    {MSG_FAILED,MSG_FAILED_STR},
};


/* Zeiger */

APTR app,window,aboutwin,bytemeter,GA_Total,GA_Sublog,menustrip;
struct Library *CLLBase;


/* Prototypen */

int createapp(void);
int gethighestsuffix(void);
logid *readlog (void);
logid *addlogidptr(char *name, char *zeile);
zeilen *addlineptr(char *zeile);
void failed2open(char *failedfile);
void refreshwin(void);
void ReadStrings(void);
void startbatch(void);
BOOL renamelog (void);
BOOL makedir(char *CreateNew);
BOOL savelogs(logid *pointer);


/* MUI-Fenster-Definition */

int createapp()
{
    app =   ApplicationObject,
            MUIA_Application_Title      , "CL-SplitLog",
            MUIA_Application_Version    , "$VER: CL-SplitLog 2.3 (22.08.96)",
            MUIA_Application_Copyright  , "©1996 Ralf Ramge/Nicolai Zizka",
            MUIA_Application_Author     , "Ralf Ramge, Nicolai Zizka",
            MUIA_Application_Description, "Splitten des Logfiles von Connectline 5.0",
            MUIA_Application_Base       , "SPLITLOG",
            MUIA_Application_Menustrip  , menustrip = MUI_MakeObject (MUIO_MenustripNM, MUIMenuData,0),

            SubWindow, window = WindowObject,

                MUIA_Window_Title   , "CL-SplitLog",
                MUIA_Window_ID      , MAKE_ID('S','P','L','T'),
                MUIA_Window_CloseGadget, FALSE,

                WindowContents, VGroup,

                    Child, bytemeter = TextObject, TextFrame, MUIA_Text_PreParse, "\33c", MUIA_Text_Contents, "0", End,

                    Child, ColGroup(2), GroupFrameT(AppStrings[MSG_PROGRESS].as_Str),
                        Child, Label(AppStrings[MSG_TOTAL].as_Str), Child, GA_Total = GaugeObject, GaugeFrame, MUIA_Gauge_Current,0, MUIA_Gauge_InfoText, AppStrings[MSG_WAITING].as_Str, MUIA_Gauge_Horiz, TRUE, End,
                        Child, Label("Sublog:"), Child, GA_Sublog = GaugeObject, GaugeFrame, MUIA_Gauge_Current, 0, MUIA_Gauge_InfoText, AppStrings[MSG_WAITING].as_Str, MUIA_Gauge_Max, 100, MUIA_Gauge_Horiz, TRUE, End,
                        Child, VSpace(0)       , Child, ScaleObject, End,
                        End,
                    End,
                End,
            End;

    if (!app)
    {
        fail(app,"Failed to create application.");
        return 0;
        }

    DoMethod(window,MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
             app,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);  //CloseRequest
    set(window,MUIA_Window_Open,TRUE);
    return 1;
    }


/* alloziiere neuen logid * */

logid *addlogidptr(char *name, char *zeile)
{
    logid *tmplptr;
    if ((tmplptr = AllocPooled(MyPool,sizeof(logid)))!=NULL) {
        if ((tmplptr->name = AllocPooled(MyPool,strlen(name)+1))!=NULL) {
            strcpy(tmplptr->name,name);
            tmplptr->inhalt = addlineptr(zeile);
            tmplptr->next = NULL;
            tmplptr->anzahl = 1;
            }
        else
            tmplptr=NULL;
        }
    return tmplptr;
}

/* alloziiere neuen zeilen * */

zeilen *addlineptr(char *zeile)
{
    zeilen *tmpzptr;
    if ((tmpzptr = AllocPooled(MyPool,sizeof(zeilen)))!=NULL) {
        if ((tmpzptr->zeile = AllocPooled(MyPool,strlen(zeile)+1))!=NULL) {
            strcpy(tmpzptr->zeile,zeile);
            tmpzptr->next = NULL;
            }
        else
            tmpzptr=NULL;
        }
    return tmpzptr;
}

/* Logfile lesen */

logid *readlog (void)
{
    ULONG tmplaenge=0,linectr=0;
    char currentid[32] = "",line[1024];
    BPTR file;
    logid *root=NULL,*pointer;
    zeilen *tmpptr;
    struct FileInfoBlock *fib;
    int prozentin;
    char statuszeile[25];

    fib=AllocDosObject(DOS_FIB,NULL);

    if ((file = Open(LogfileName, MODE_OLDFILE))) {
        ExamineFH(file,fib);
        filelaenge=fib->fib_Size;

        while (FGets(file,line,4096)) {
            refreshwin();

            pointer = root;
            linectr++;

            tmplaenge += strlen(line);
            prozentin = (int) (tmplaenge+1)*100/(filelaenge+1);
            sprintf(statuszeile,"%ld Byte (%d%%), %ld %s",tmplaenge,prozentin,linectr,AppStrings[MSG_ENTRIES].as_Str);
            set(bytemeter, MUIA_Text_Contents, statuszeile);

            sscanf(line,"%*s %*s %s %*s",currentid);

            if (pointer != NULL) {
                while ((pointer->next != NULL)&&(stricmp(pointer->name,currentid) != 0))
                    pointer = pointer->next;

                if (stricmp(pointer->name,currentid) == 0) {
                    tmpptr=pointer->inhalt;

                    while (tmpptr->next != NULL)
                        tmpptr = tmpptr->next;

                    tmpptr->next = addlineptr(line);
                    pointer->anzahl++;
                    }
                else {
                    pointer->next = addlogidptr(currentid,line);
                    pointer=pointer->next;
                    }
                }
            else {
                 pointer = addlogidptr(currentid,line);
                 root=pointer;
                 }

            if ((pointer==NULL)||(pointer->inhalt==NULL)||(pointer->name==NULL)||(pointer->inhalt->zeile==NULL)) {
                CLL_AddLogA (5,"SPLITLOG",AppStrings[MSG_NOMEM].as_Str,NULL);
                root=NULL;
                break;
                }
            }
        Close(file);
        }
    else
         failed2open(LogfileName);

    FreeDosObject(DOS_FIB,fib);
    return root;
}


/* einzelne sublogs speichern */

BOOL savelogs(logid *pointer)
{
    BPTR file;
    char out[110]="",pro[5];
    ULONG subtotal=0,templaenge=0;
    zeilen *tempptr;
    int prozentout;

    if (!makedir(SublogfilePath))
        return FALSE;

    set (GA_Total, MUIA_Gauge_Max, filelaenge);
    while(pointer != NULL) {
        sprintf(out,"%s/%s",SublogfilePath,pointer->name);

        if (!(file = Open(out,MODE_OLDFILE))) {
            if (!(file = Open(out,MODE_NEWFILE))) {
                failed2open(out);
                return FALSE;
                }
            }
        else
            Seek(file,0,OFFSET_END);

        tempptr = pointer->inhalt;

        set(GA_Sublog, MUIA_Gauge_Max, pointer->anzahl);
        set(GA_Sublog, MUIA_Gauge_InfoText, pointer->name);
        refreshwin();

        while(tempptr != NULL) {
            FPuts(file,tempptr->zeile);
            subtotal++;
            templaenge += strlen(tempptr->zeile);
            tempptr = tempptr->next;
            prozentout = (int) (templaenge+1)*100/(filelaenge+1);
            sprintf(pro,"%d%%",prozentout);
            set(GA_Total, MUIA_Gauge_Current, templaenge);
            set(GA_Total, MUIA_Gauge_InfoText, pro);
            set(GA_Sublog, MUIA_Gauge_Current, subtotal);
            }

        Close(file);

        pointer = pointer->next;
        subtotal = 0;
        }
    return TRUE;
}

/* Batchdatei aufrufen */

void startbatch()
{
    SetVar("LOGBACKUPNAME",nfilename,-1,GVF_LOCAL_ONLY);
    Execute(strcat("CONNECTLINE:s/splitlog.batch ",nfilename),NULL,NULL);
    }

/* file lies sich nicht öffnen */

void failed2open(char *failedfile)
{
    char *failedstr="";

    sprintf(failedstr,AppStrings[MSG_NOOPEN].as_Str,failedfile);
    CLL_AddLogA (5,"SPLITLOG",failedstr,NULL);
}

/* Directory anlegen */

BOOL makedir(char *CreateNew)
{
    BPTR lock;
    char *failedstr="";

    if ((lock = Lock(CreateNew,ACCESS_WRITE))==0)
        if ((lock = CreateDir(CreateNew))==NULL)
        {
              sprintf(failedstr,AppStrings[MSG_NOCREATE].as_Str,CreateNew);
              CLL_AddLogA (5,"SPLITLOG",failedstr,NULL);
              return FALSE;
              }

    UnLock(lock);
    return TRUE;
}

void refreshwin(void)
{
    long id;
    ULONG sigs;

    id = DoMethod(app,MUIM_Application_NewInput, &sigs);
    switch (id)
    {
        case MEN_ABOUT:
            MUI_Request(app,aboutwin,0,NULL,"OK",aboutstring);
            break;
        }
}

/* Lokalisierung */

void ReadStrings()
{
    long ctr;
    APTR catalog;

    catalog = OpenCatalog(NULL,"CL-SplitLog.catalog",OC_BuiltInLanguage,"english",TAG_DONE);

    for (ctr=0; ctr<10; ctr++)
        AppStrings[ctr].as_Str = GetCatalogStr(catalog,ctr,AppStrings[ctr].as_Str);

    CloseCatalog(catalog);

    strcpy(MSG_MMSG_PROJECT_STR, AppStrings[MSG_MMSG_PROJECT].as_Str);
}

/* aktuellen suffix ermitteln */

int gethighestsuffix(void)
{
    BPTR lock;
    struct FileInfoBlock *fib;
    BOOL cont=TRUE;
    char *ppattern="",pattern[15]="Logfile-bak.#?",*buffer="";
    int len=0,highest=0;

    fib=AllocDosObject(DOS_FIB,NULL);
    lock=Lock(LogfilePath,ACCESS_READ);

    ParsePatternNoCase(pattern,ppattern,2*(strlen(pattern))+2);
    Examine (lock,fib);

    while (cont) {
        cont=ExNext (lock,fib);
        if ( MatchPatternNoCase (ppattern,fib->fib_FileName))
            if (highest<atoi(&fib->fib_FileName[12]))
                highest = atoi (&fib->fib_FileName[12]);
        }

    Fault(IoErr(),AppStrings[MSG_FAILED].as_Str,buffer,len);

    if (len!=0) {
        CLL_AddLogA (5,"SPLITLOG",buffer,NULL);
        highest=-1;
        }

    UnLock(lock);
    FreeDosObject(DOS_FIB,fib);
    return highest;
}

/* logfile umbennen */

BOOL renamelog (void)
{
    int high;

    if ((high=gethighestsuffix())!=-1) {
        sprintf(nfilename,"%s/%s%04d",LogfilePath,BakLogFile,++high);
        CLL_RenameLog (nfilename);
        return TRUE;
        }
    return FALSE;
}

/* Hauptprogramm */

int main (void)
{
    logid *root;

    if (CLLBase=OpenLibrary("connectline:libs/cllog.library",0)) {

        ReadStrings();

        if ((MyPool = CreatePool(MEMF_ANY,10000,10000))!=NULL) {
            init();
            if (createapp()) {
                if((root = readlog())!=NULL) {
                     if (renamelog()) {
                         if (savelogs(root)) {

                             startbatch();

                             refreshwin();
                             }
                         }
                     }
                set (window,MUIA_Window_Open,FALSE);
                MUI_DisposeObject(app);
                fail(NULL,NULL);
                DeletePool(MyPool);
                }
            }
        else
            CLL_AddLogA (5,"SPLITLOG",AppStrings[MSG_NOMEM].as_Str,NULL);

        CloseLibrary (CLLBase);
        }
    else
        printf("%s\n", AppStrings[MSG_NOLOGLIB].as_Str);

}
