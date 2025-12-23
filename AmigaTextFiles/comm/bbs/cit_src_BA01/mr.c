/**
  Citadel Mail Snooper Utility Version 1.00
**/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 /*
  * #define DEBUG
  */

void      StopDecode(void);
void      Process(char *file);
void GetStr(FILE * fd, char *buf);
void StartDecode( int (*func)() );
int  Decode(int c);
int moo(int c);
int Decode2(int val);
#define TRUE    1
#define FALSE   0

int main(int, char **);

int main(int argc, char **argv)
  {
  FILE *fp;
  char line[80];
  int i, j, k;
  fprintf(stderr," Citadel Mail Identifier Version 1.00\n");
  if( argc > 1 )
    {
    for( i=1; i<argc; i++)Process(argv[i]);
    }
  else
    {
  if( (fp=fopen("map.sys", "r")) == NULL )
    {
    fprintf(stderr,"unable to open map.sys\n");
    fprintf(stderr,"This utility must be run from the DOMAINAREA\n");
    }
  else
    {
    while( fgets(line,80,fp) )
      {
      i = atoi(line);
      for( j=2; line[j] != ' ' && j < 80; j++);
      for(    ; line[j] == ' ' && j < 80; j++);  /* find start of number */
      j = atoi(&line[j]);
      for(k=0; k<j; k++)
        {
        sprintf(line,"%d/%d",i,k);
        Process(line);
        };
      };
    fclose(fp);
    };
  };
  return 0;
  }
void strip(char *p)
  {
  int i = strlen(p);
  for(i--; i> 0 && (p[i] == ' '|| p[i]=='\0'); i--);
  p[i+1]='\0';
  }





char      author[50];
char        date[50];
char        node[50];
char          to[50];
char      domain[50];

#define STARTING        0
#define COLLECTING        1
#define EATING                2
int       state = STARTING;
void Process(char *file)
{
  char      id[21];
  int       c;
  char      name[21];
  FILE     *fd;

  author[0] = 0;
  date[0] = 0;
  to[0] = 0;
  node[0] = 0;
  domain[0] = 0;
  state = STARTING;
  if ((fd = fopen(file, "rb")) == NULL)return;
  GetStr(fd, id);
  GetStr(fd, name);

  StartDecode(moo);
  while ((c = getc(fd)) != EOF)
    {
      Decode(c);
    }
  StopDecode();
  fclose(fd);
  strip(author);
  strip(date);
  strip(to);
  strip(node);
  strip(domain);
  printf("%s from %s@%s_%s to %s@%s\n", date, author, node,domain, to,
	 name);
}

void GetStr(FILE * fd, char *buf)
{
  int       c,
            i = 0;

  while ((c = getc(fd)) != 0 && c != EOF)
    {
      buf[i++] = c;
    }
  buf[i] = 0;
}

int moo(c)
  int       c;
{
  static int ind;
  static char *str;

  if (state == STARTING)
    {
      ind = 0;
      switch (c)
	{
	  case 'A':
	    str = author;
	    state = COLLECTING;
	    break;
	  case 'D':
	    str = date;
	    state = COLLECTING;
	    break;
	  case 'T':
	    str = to;
	    state = COLLECTING;
	    break;
	  case 'N':
	    str = node;
	    state = COLLECTING;
	    break;
	  case 'X':
	    str = domain;
	    state = COLLECTING;
	    break;
	  default:
	    state = EATING;
	}
    }
  else if (state == EATING)
    {
      if (c == 0)
	{
	  state = STARTING;
	}
    }
  else
    {
      if (c == 0)
	{
	  state = STARTING;
	}
      str[ind++] = c;
    }
  return TRUE;
}

#define CAPS            0
#define NO_CAPS         1
#define NUMERICS        2
#define SPEC_CHARS      3
#define FINISHED        4	/*
				 * finished with encoded data
				 */
#define NO_SET          5	/*
				 * an initial value
				 */

#define COMP_SPACE      26	/*
				 * universal
				 */
#define ZERO_COMP       16
#define RET_COMP        17

#ifdef NEEDED

struct
{
  char      Set,		  /*
				   * code set
				   */
            CompValue;		  /*
				   * position in code set
				   */
}
compress[] =
{
  { 0, 0 }, { 2, 10 }, { 2, 11 }, { 2, 12 }, { 2, 13 },	/*	 * ' ' - '$' 	 */
  { 2, 14 }, { 2, 15 }, { 2, 16 }, { 2, 17 }, { 2, 18 },	/*	 * '%' - ')' 	 */
  { 2, 19 }, { 2, 20 }, { 2, 21 }, { 2, 22 }, { 2, 23 },	/*	 * '*' - '.' 	 */
  { 2, 24 }, { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 },	/*	 * '/' - '3' 	 */
  { 2, 4 }, { 2, 5 }, { 2, 6 }, { 2, 7 }, { 2, 8 },	/*	 * '4' - '8' 	 */
  { 2, 9 }, { 2, 25 }, { 3, 0 }, { 3, 1 }, { 3, 2 },	/*	 * '9' - '=' 	 */
  { 3, 3 }, { 3, 4 }, { 3, 5 }, { 0, 0 }, { 0, 1 },	/*	 * '>' - 'B' 	 */
  { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 },	/*	 * 'C' - 'G' 	 */
  { 0, 7 }, { 0, 8 }, { 0, 9 }, { 0, 10 }, { 0, 11 },	/*	 * 'H' - 'L' 	 */
  { 0, 12 }, { 0, 13 }, { 0, 14 }, { 0, 15 }, { 0, 16 },	/*	 * 'M' - 'Q' 	 */
  { 0, 17 }, { 0, 18 }, { 0, 19 }, { 0, 20 }, { 0, 21 },	/*	 * 'R' - 'V' 	 */
  { 0, 22 }, { 0, 23 }, { 0, 24 }, { 0, 25 }, { 3, 6 },	/*	 * 'W' - '[' 	 */
  { 3, 7 }, { 3, 8 }, { 3, 9 }, { 3, 10 }, { 3, 11 },	/*	 * '\' - '`' 	 */
  { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 },	/*	 * 'a' - 'e' 	 */
  { 1, 5 }, { 1, 6 }, { 1, 7 }, { 1, 8 }, { 1, 9 },	/*	 * 'f' - 'i' 	 */
  { 1, 10 }, { 1, 11 }, { 1, 12 }, { 1, 13 }, { 1, 14 },	/*	 * 'j' - 'o' 	 */
  { 1, 15 }, { 1, 16 }, { 1, 17 }, { 1, 18 }, { 1, 19 },	/*	 * 'p' - 't' 	 */
  { 1, 20 }, { 1, 21 }, { 1, 22 }, { 1, 23 }, { 1, 24 },	/*	 * 'u' - 'y' 	 */
  { 1, 25 }, { 3, 12 }, { 3, 13 }, { 3, 14 }, { 3, 15 }	/** 'z' - '' 	 */};

#endif

static char Numerics[] =
{
  '0', '1', '2', '3', '4',
  '5', '6', '7', '8', '9',
  '!', '\"', '#', '$', '%',
  '&', '\'', '(', ')', '*',
  '+', ',', '-', '.', '/',
  ':'
};

static char SpecChars[] =
{
  ';', '<', '=', '>', '?',
  '@', '[', '\\', ']', 0x5e,
  '_', '`', '{', '|', '}',
  ' ', 0, '\r'
};

static char CurSet;
static int OB,
          ThisVal,
          NB;
int       (*OutFunc) ();


void StartDecode(func)
  int       (*func) ();
{
  ThisVal = 0;
  CurSet = NO_SET;
  OutFunc = func;
  NB = 5;
}

void
StopDecode()
{
}

int
Decode(c)
  int       c;
{
  static int mask[] =
  {
    0, 1, 3, 7, 0xf, 0x1f
  };
  int       AB;

  if (CurSet == FINISHED)
    return TRUE;

#ifdef DEBUG
  fprintf(stderr, "decoding %x, ThisVal is currently %x\n", c, ThisVal);
#endif

  if (!Decode2(c & mask[NB]))
    return FALSE;

  c >>= NB;

#ifdef DEBUG
  fprintf(stderr, "c becomes %x\n", c);
#endif
  AB = 8 - NB;

  if (AB >= 5)
    {
      ThisVal = 0;
      AB -= 5;
      NB = 5;
      if (!Decode2(c & mask[NB]))
	return FALSE;
      c >>= 5;
#ifdef DEBUG
      fprintf(stderr, "c becomes %x\n", c);
#endif
    }
  NB = 5 - AB;
  ThisVal = c;
  return TRUE;
}

int Decode2(val)
  int       val;
{
  int       toReturn;
  char      Used;

  if (CurSet == FINISHED)
    return TRUE;
  ThisVal += (val << (5 - NB));
#ifdef DEBUG
  fprintf(stderr, "ThisVal calculates to be %x, val is %x\n", ThisVal, val);
#endif
  switch (ThisVal - 27)
    {
      case FINISHED:
#ifdef DEBUG
	fprintf(stderr, "Finished found.\n");
#endif
      case CAPS:
      case NO_CAPS:
      case NUMERICS:
      case SPEC_CHARS:
	CurSet = ThisVal - 27;
	Used = TRUE;
	toReturn = TRUE;
	break;
      default:
	Used = FALSE;
    }
/*
 * Discard if no set defined.
 */
  if (!Used)
    {
      if (CurSet == NO_SET)
	toReturn = TRUE;

 /*
  * True for any valid code set
  */
      else if (ThisVal == COMP_SPACE)
	toReturn = (*OutFunc) (' ');
      else
	switch (CurSet)
	  {
	    case CAPS:
	      toReturn = (*OutFunc) ('A' + ThisVal);
	      break;
	    case NO_CAPS:
	      toReturn = (*OutFunc) ('a' + ThisVal);
	      break;
	    case NUMERICS:
	      toReturn = (*OutFunc) (Numerics[ThisVal]);
	      break;
	    case SPEC_CHARS:
	      toReturn = (*OutFunc) (SpecChars[ThisVal]);
	      break;
	  }
    }
  return toReturn;
}
