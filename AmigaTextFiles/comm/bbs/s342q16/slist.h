/************************************************************************/
/*                             sList.h                                  */
/*      #include file for List.C.                                       */
/************************************************************************/
/************************************************************************/
/*              List handling structures - generic sorted lists         */
/************************************************************************/
#ifndef slist_h
#define slist_h
typedef struct Slist      SListData;
typedef struct sbasestuff SListBase;
#define GetFirst(x)     (((x)->start != NULL) ? (x)->start->data :(void *)NULL)
#define MoveAndClear(s, d)      (d)->start = (s)->start, (s)->start = NULL
/*
* This is the generic list structure normal link.  It contains a pointer
* to the next element in the list and a pointer to a chunk of data.
*/
struct Slist
  {
  void     *data;
  SListData *next;

  };
/*
* This structure contains data and functions necessary to handle some given
* instantiation of a list.  Included is a pointer to the data of the list,
* and pointers to functions which should always be used for given functions
* applied to the list.
*/
struct sbasestuff
  {
  SListData *start;
  void     *(*CheckIt)(void *d1, void *d2);
  /*        int      (*cmp)(void *d1, void *d2); */
  int      (*cmp)(void *d1, void *d2);
  /*        char    *(*CheckIt)(void *d1, *d2); */
  void     (*FreeFunc)(void *d);
  /*      void     (*FreeFunc)(void *data);  */
  /*        void     *(*EatLine)(char *line);     */
  void     *(*EatLine)(char *line);

  };
#define InitListValues(l, ci, xcmp, f, e)       (l)->start = NULL, (l)->CheckIt = ci,   (l)->cmp = xcmp, (l)->FreeFunc = f, (l)->EatLine = e
/*
* These definitions are for the listshow functions: behavior specification.
*/
#define NO_REFRESH      0x01    /* for use with the mode argument       */
#define ADVANCE         0x02    /* advance one in list                  */
#define DEL_ACTIVE      0x04    /* if DEL is active                     */
#define NO_HEADER       0x08    /* No header on list if set             */
#define NO_ACTION       0x10    /* do nothing - just display            */
#define PAGE_LABELS     0x20    /* show page labels                     */
#define NO_BAR          0x40    /* show page labels                     */
/*
* These definitions are mode definitions of ListShow.
*/
#define LS_NORMAL       0
#define LS_ONTOP        1
/*
* This structure is used by listshow.c for displaying lists of data in
* dynamic columns.
*/
typedef struct
  {
  SListBase Data;
  char *title;
  int *critical;                  /* chars which cause exit       */
  char (*DelFunc)();              /* If DEL key is touched        */
  int left, right, top, bottom;   /* window to display in         */
  int fg, bg, dfg, dbg;           /* colors                       */
  int ColWidth;                   /* how wide are columns?        */
  void (*DispFunc)();             /* displays data for you        */
  char (*SelectorId)();         /* see if the selector agrees   */
  int  (*UserInput)();

  }
DisplayList;
void *DispList(DisplayList *, void *, int  *, int);
char MakeList(SListBase *, char *, FILE *);
void AddData(SListBase *, void *, void (*)(void *),char );
void KillData(SListBase *, void *);
void KillList(SListBase *);
void *GetLast(SListBase *);
void *SearchList(SListBase *, void *);
int  RunList(SListBase *, void (*)(void *));
void FrontToEnd(SListBase *);
void NoFree(void *);
char *GetAString(char *, int , FILE *);
#endif
