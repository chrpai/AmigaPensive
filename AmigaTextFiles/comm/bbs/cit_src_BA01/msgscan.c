/*  Citadel Message Base Scanner */
#include <stdio.h>
#include <string.h>
#include "ctdl.h"

typedef enum { NO=0, YES=1 }  BOOLEAN_TYPE;

char netDebug      = '\0';
char logNetResults = '\0';
FILE  *netLog = stdout;
char verbose_flag  = FALSE;

extern CONFIG      cfg;            /* A buncha variables           */
extern FILE            *netfl, *roomfl;
extern MessageBuffer  msgBuf; /* The -sole- message buffer      */
extern FILE *msgfl, *msgfl2;

void Print_Items(struct headlist *ptr);
void Match_Message(MessageBuffer *msg);
void Do_Statistics(MessageBuffer *msg);
void Print_Statistics(void);
void Increment( struct headlist **cptr, char *name);
void Scan_Messages(void);
void crashout(char *);
int main(int argc, char **argv);
int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */
void A_Menu(void);
void Terminal(char *string,int  size);
void strip_crlf(char *string);
BOOLEAN_TYPE match(char *str1, int idx);

void crashout(str)
char *str;
  {
  printf(str);
  exit(10);

  }

int  main(argc,argv)
int  argc;
char **argv;
  {
  /* Process CONFIG  */
  cfg.weAre = UTILITY;
  printf("Citadel Message Base Scanner Version %s\n", VERSION_NAME);
  if (!readSysTab(FALSE, TRUE)) exit(100);
  A_Menu();
  Scan_Messages();    /* process the messages */
  Print_Statistics();
  return 0;
  }

void Scan_Messages()
  {
  MSG_NUMBER msg, firstMessage;
  int total=1;     /* For stat keeping. */
  extern struct mBuf mFile1;
  fprintf(stderr, "Mulching...\n");
  InitMsgBase();
  startAt(msgfl, &mFile1, 0, 0);
  getMessage(getMsgChar, FALSE, TRUE, TRUE);
  firstMessage = atol(msgBuf.mbId);
  msg = firstMessage -1;
  while (msg != firstMessage)
    {
    total++;
    getMessage(getMsgChar, FALSE, TRUE, TRUE);
    msg = atol(msgBuf.mbId);
    Match_Message(&msgBuf);
    };
  }


#define MAX_OPTIONS  (12)
#define MAX_FIELD    (150)

struct select
  {
  BOOLEAN_TYPE  option_enabled;  /* Is this option enabled */
  char          option_name[20]; /* name of the option     */
  int           size;            /* size of object */
  };

struct select options[MAX_OPTIONS] =
  {
  NO, "Author             ", 129,             NO, "Creation Date      ", sizeof(label),
  NO, "Creation Time      ", sizeof(label),   NO, "Id Number          ", sizeof(label),
  NO, "Node Name          ", sizeof(label),   NO, "Origin Name        ", sizeof(label),
  NO, "Room Name          ", sizeof(label),   NO, "To User            ", 129,
  NO, "Net Routing Address", (NAMESIZE*2)+10, NO, "OtherNet Address   ", O_NET_PATH_SIZE,
  NO, "Reply              ", sizeof(label),   NO, "Domain             ", sizeof(label),
  };

char *option_data[MAX_OPTIONS];

void A_Menu()
  {
  int idx;
  char ans;
  char line[MAX_FIELD];
  ans = ' ';
  for(idx=0; idx < MAX_OPTIONS; idx++)
    {
    options[idx].option_enabled = NO;
    option_data[idx] = NULL;
    };
  while( ans != 'Q' && ans != 'q'
      && ans != 'X' && ans != 'x')
    {
    printf("  Message Selection Options\n");
    for( idx=0; idx < MAX_OPTIONS; idx++ )
      {
      if( ( idx % 2 ) == 0 )printf("\n");
      printf( " %c - %20s(%8s)",( 'A'+idx ), options[idx].option_name,
      ( options[idx].option_enabled == YES ? "enabled" : "disabled"));
      };
    printf("\n Verbose is %s", (verbose_flag== YES) ? "on." : "off.");
    printf("\n Q - to quit,  X - to exit and do scan:");
    Terminal(line,1);
    ans = line[0];
    ans = toupper(ans);
    idx = ans - 'A';
    if( ans == 'V' || ans == 'v')
      {
      verbose_flag = ( verbose_flag == YES ) ? NO : YES ;
      };
    if( ans != 'Q' && ans != 'q'
     && ans != 'x' && ans != 'X' )
      {
      if( idx < 0 || idx >= MAX_OPTIONS )
        {
        ans = ' ';
        printf("Error: Must enter A thru L\n");
        }
      else
        {
        options[idx].option_enabled =  options[idx].option_enabled == YES ? NO : YES;
        if( options[idx].option_enabled == YES )
          {
          printf("Enter %s(up to %d long):",options[idx].option_name, options[idx].size);
          Terminal(line,options[idx].size);
          option_data[idx] = strdup(line);
          }
        else
          {
          free(option_data[idx]);
          }
        };
      };
    };
  if( ans == 'q' || ans == 'Q' ) exit(0);
  }

void strip_crlf(char *string)
  {
  int x;
  for(x =  strlen(string); x>=0; x--)
    {
    if((string[x] ==  '\r')||(string[x] ==  '\n')) string[x] =  0;

    }

  }

void Terminal(char *string,int  size)
  {
  char  c;
  short i;
  if( size > 8 )
    {
    for( i = 0; i < size; ) string[i++] = '\0';
    for (i = 0; i < size && ( c = fgetchar() ) != '\n' && c != EOF;i++)
      {
      string[i] = c;
      };
    while ( c != '\n' && c != EOF  && c != 0x03 )c = fgetchar();
    }
  else
    {
    char tmp[8];
    for (i = 0; i < 8 && ( c = fgetchar() ) != '\n' && c != EOF;i++)
      {
      tmp[i] = c;
      };
    while ( c != '\n' && c != EOF && c != 0x03 )c = fgetchar();
    strncpy(string,tmp,size);
    };
  strip_crlf(string);
  if( string[0] == '\0' )
    {
    string[0] = '\n';
    string[1] = '\0';
    };
  }

void Match_Message(MessageBuffer *msg)
  {
  int idx;
  BOOLEAN_TYPE a_match;

  for( a_match = YES, idx=0; idx < MAX_OPTIONS && a_match == YES; idx++ )
    {
    if( options[idx].option_enabled == YES)
      {
      switch (idx)
        {
        case  0: if( match(msg->mbauth,  idx) == NO) a_match = NO; break;
        case  1: if( match(msg->mbdate,  idx) == NO) a_match = NO; break;
        case  2: if( match(msg->mbtime,  idx) == NO) a_match = NO; break;
        case  3: if( match(msg->mbId,    idx) == NO) a_match = NO; break;
        case  4: if( match(msg->mboname, idx) == NO) a_match = NO; break;
        case  5: if( match(msg->mborig,  idx) == NO) a_match = NO; break;
        case  6: if( match(msg->mbroom,  idx) == NO) a_match = NO; break;
        case  7: if( match(msg->mbsrcId, idx) == NO) a_match = NO; break;
        case  8: if( match(msg->mbaddr,  idx) == NO) a_match = NO; break;
        case  9: if( match(msg->mbOther, idx) == NO) a_match = NO; break;
        case 10: if( match(msg->mbreply, idx) == NO) a_match = NO; break;
        case 11: if( match(msg->mbdomain,idx) == NO) a_match = NO; break;
        };
      };
    };
  if( a_match == YES )
    {
    if( verbose_flag )
      {
      printf( "Author: %20s  ",msg->mbauth  );
      printf( "Node Id:%20s\n",msg->mbId    );
      printf( "Date:   %20s  ",msg->mbdate  );
      printf( "Time:   %20s\n",msg->mbtime  );
      printf( "Node:   %20s  ",msg->mboname );
      printf( "Origin: %20s\n",msg->mborig  );
      printf( "Room:   %20s  ",msg->mbroom  );
      printf( "Src Id: %20s\n",msg->mbsrcId );
      printf( "Net Adr:%s\n",msg->mbaddr  );
      printf( "Other:%s\n",msg->mbOther );
      printf( "Reply:  %20s\n",msg->mbreply );
      printf( "Domain:%s\n",msg->mbdomain);
      };
    Do_Statistics(msg);
    };
  }

BOOLEAN_TYPE match(char *str1, int idx)
  {
  char *c1;
  char *c2;
  int  siz;
  int  dif;
  siz = options[idx].size;
  c1  = str1;
  c2  = option_data[idx];
  dif = 0;
  while ( dif == 0 && *c1 != '\0' && *c1 != '\0' && siz >0 )
    {
    siz--;
    if( *c2 == '*')break ;
    if( *c2 != '?')dif = *c1 - *c2;
    c1++;
    c2++;
    };
  return ( dif == 0 ) ? YES : NO;
  }

struct headlist
  {
  struct headlist *next; /* next item in the list */
  char *name;            /* name of primary item */
  long  count;           /* occurances */
  };

struct headlist *msg_per_room= NULL;
struct headlist *msg_per_node= NULL;
struct headlist *msg_per_domain= NULL;
struct headlist *msg_per_author=NULL;
long msg_per_hour[24] = { 0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0 };

void Do_Statistics(MessageBuffer *msg)
  {
  char *tptr;
  int hour;
  Increment(&msg_per_room, msg->mbroom);
  Increment(&msg_per_node, msg->mborig);
  Increment(&msg_per_author, msg->mbauth);
  Increment(&msg_per_domain, msg->mbdomain);
  tptr = &msg->mbtime[0];
  while( *tptr == ' ')tptr++;
  if( *tptr >= '0' && *tptr <= '9' )
    {
    hour = tptr[0] - '0';
    tptr++;
    if( *tptr >= '0' && *tptr <= '9')
      {
      hour =  ( hour * 10 );
      hour += ( *tptr - '0');
      };
    if( hour == 12 ) hour=0;
    if( tptr[4] == 'p')hour +=12;
    if( hour < 0 || hour > 23 )
      {
      printf("computed %d for %s, invalid time\n", hour, msg->mbtime);
      }
    else msg_per_hour[hour]++;
    };
  }

void Increment( struct headlist **cptr, char *name)
  {
  struct headlist *pptr;
  pptr = *cptr;
  if( *name == '\0' || *name == ' ' )
    {
    name = "*** local ***";
    };
  while ( pptr != NULL )
    {
    if( strcmp(name, pptr->name) == 0 )
      {
      pptr->count++;
      return;
      };
    pptr = pptr->next;
    };
  pptr = (struct headlist *)malloc(sizeof(struct headlist));
  if( pptr == NULL )
    {
    printf("Error: No memory available\n");
    exit(100);
    };
  pptr->name = strdup(name);
  pptr->count= 1;
  pptr->next = *cptr;
  *cptr = pptr;
  }

void Print_Items(struct headlist *ptr)
  {
  int count=0;
  while( ptr )
    {
    printf("%20s  %10ld    ", ptr->name, ptr->count);
    count = ( count + 1 ) % 2;
    if( count == 0 ) printf("\n");
    ptr= ptr->next;
    };
  }

void Print_Statistics(void)
  {
  char *h1 = "Room Name";
  char *h2 = "Count";
  char *h3 = "Node";
  char *h4 = "Author";
  char *h5 = "Domain";
  char *h6 = "Hour  ";
  char *h7 =  "am";
  char *h8 =  "pm";
  int hour;
  printf("\n\n            Messages Per Room  Report\n");
  printf("%20s  %10s    %20s  %10s\n", h1, h2, h1, h2);
  Print_Items( msg_per_room );
  printf("\n\n            Messages Per Node  Report\n");
  printf("%20s  %10s    %20s  %10s\n", h3, h2, h3);
  Print_Items( msg_per_node );
  printf("\n\n            Messages Per Author  Report\n");
  printf("%20s  %10s    %20s  %10s\n", h4, h2, h4);
  Print_Items( msg_per_author );
  printf("\n\n            Messages Per Domain  Report\n");
  printf("%20s  %10s    %20s  %10s\n", h5, h2, h5);
  Print_Items( msg_per_domain );
  printf("\n\n            Messages Per Hour  Report\n");
  printf("%20s  %10s    %20s  %10s\n", h6, h2, h6);
  for(hour=0; hour < 12; hour +=2 )
    {
    printf("%17d %s  %10ld",( hour == 0 ) ? 12 : hour, h7, msg_per_hour[hour]);
    printf("    %17d %s  %10ld\n",hour+1, h7, msg_per_hour[hour+1]);
    };
  for(hour=12; hour < 24; hour +=2 )
    {
    printf("%17d %s  %10ld",(hour == 12) ? 12: hour-12, h8, msg_per_hour[hour]);
    printf("    %17d %s  %10ld\n", hour-11, h8, msg_per_hour[hour+1]);
    };
  }
