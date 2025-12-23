#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "extra.h"
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "dos.h"
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include "io.h"
extern long DOSBase;
int Adjust_Factor(m3,money)
int m3,money;
  {
  int m4;
  if( m3 == 0 ) m3 = 1;
  if( money == 0)money = 1;
  if( m3   < money )
    {
    m4 =  (money + m3 -1) / m3;
    if( m4 > 15 ) m4 = 15;       /* negative is harder on you!*/
    if( m4 <  1 ) m4 = 1;
    m4  *= -1;
    }
  else
    {
    m4 =  (money + m3 -1) / money;
    if( m4 > 5 ) m4 = 5;
    if( m4 < 1 ) m4 = 1;
    };
  return m4;
  }
int Request_Data(String,def,max)
char *String;
int  def;
  {
  int result;
  char ostring[16];
  result = -1;
  while( result == -1 )
    {
    printf("%s [default: %d, max: %d]: ",String,def,max);
    (void)fgets(ostring,15,stdin);
    strip_crlf(ostring);
    if( ostring[0] == '\0')
      {
      result = def;

      }
    else result = atoi(ostring);
    if( result > max )
      {
      printf(" %d is too much, the limit is %d\n",result,max);
      result = -1;

      };

    };
  if(result < 0)result = 0;
  return result;

  }
/*
* pprompt(mstring,ostring,len) - outputs the string "mstring" and inputs
* a string which is placed in ostring.  'len' is the maximum number of
* characters which will be accepted.
*/
void pprompt(mstring,ostring,len)
char *mstring;
char *ostring;
int len;
  {
  printf("%s",mstring);
  (void)fgets(ostring,len,stdin);
  strip_crlf(ostring);
  if( ostring[0] ==  0)ostring[0] = ' ';

  }
/*
* hotkey(mstring,ostring) - outputs the string "mstring" and waits
* for one key which it will place in element [0] of "ostring".
* "mstring" may be a null string, in which case it will only wait for
* the key, and not output any pprompt.
*/
void hotkey(mstring,ostring)
char mstring[];
char *ostring;
  {
  printf("%s",mstring);
  *ostring =    getchar();
  if( *ostring != '\n')while ( getchar() !=   '\n');
  if( ostring[0] >='a' && ostring[0] <= 'z')
    {
    ostring[0] = (char) ( (int)ostring[0] - 32 );

    }
  else if( ostring[0] == '\n') ostring[0] = ' ';

  }
/*
*/
void end()
  {
  (void)fcloseall();
  printf("Returning to Citadel!");
  exit(0);

  }
int boolean(astring,yndef)
char astring[];
int yndef;
  {
  char ostring[20];
  int retv =  0;
  reget:
  printf(astring);
  if(yndef ==  1) printf(" [Y/n] ");
  if(yndef ==  2) printf(" [y/N] ");
  hotkey("",ostring);
  if(ostring[0] != 'Y' && ostring[0] != 'N' && ostring[0] != ' ') goto reget;
  switch(yndef)
    {
    case 1: if(ostring[0] ==  'N') retv =  2; else retv =  1; break;
    case 2: if(ostring[0] ==  'Y') retv =  1; else retv =  2; break;

    }
  return retv;

  }
char *harmony_level(type,level)
int type,level;
  {
  switch (type)
    {
    case  1:    /* Morale */
    if( level >=   -20 && level <= 20 ) return "average";
    if( level >  1000) return "Elite Guards";
    if( level >   400) return "Elite Trained";
    if( level >   200) return "very high";
    if( level >    80) return "high";
    if( level >    10) return "above average";
    if( level < -1000) return "not trained";
    if( level <  -400) return "very poorly trained";
    if( level <  -200) return "very low";
    if( level <   -80) return "low";
    if( level <   -10) return "below average";
    break;
    case  2:    /* Population Harmony */
    if( level >=   -20  &&  level <= 20 ) return "Satisfied";
    if( level >  1000 ) return "Exalted";
    if( level >   400 ) return "Extremely Happy";
    if( level >   200 ) return "Very Happy";
    if( level >   100 ) return "Boisterous";
    if( level >    80 ) return "Happy";
    if( level >    40 ) return "Jolly";
    if( level >    10 ) return "Content";
    if( level < -1000 ) return "Anarchy";
    if( level < - 400 ) return "Revolting";
    if( level <  -200 ) return "Enraged";
    if( level <   -80 ) return "Restless";
    if( level <   -40 ) return "Protesting";
    if( level <   -10 ) return "Discontent";
    break;
    }

  }
