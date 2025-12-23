/*
*				tools.c
*
* Random functions for both Ctdl and it's utilities.
*/
/*
*				history
*
* 90Aug26 HAW  Created.
*/
#include "ctdl.h"
/*
*				Contents
*
*	NormStr()		Deletes leading trailing blanks etc.
*	PrintPretty()		Print numbers prettily.
*	normId()		Normalizes a node id.
*	hash()			Hashes a string to an integer.
*	CleanEnd()		Clears trailing blanks off.
*	lbyte()			Finds the 0 byte of a string.
*/
void  Do_Stack_Check(void);
/*
* NormStr()
*
* This function Deletes leading trailing blanks etc.
*/
void NormStr(char *s)
  {
  char *save,*pc;
  Do_Stack_Check();
  pc = save = s;
  /* find end of string and remove control characters   */
  while (*pc)
    {
    if (*pc < ' ')   *pc = ' ';   /* zap tabs etc... */
    pc++;
    
    };
  pc--;                              /* no trailing spaces: */
  while (pc>s  && *pc == ' ') pc--;
  pc++;
  *pc = '\0';                        /* no leading spaces: */
  pc = s;
  while( *pc == ' ') pc++;
  if( pc != s )
    {
    while(*pc)*s++ = *pc++;
    *s = '\0';
    
    };
  s = save;                          /* no double blanks */
  for (;  *s;)
    {
    if (*s == ' '   &&   *(s+1) == ' ')
      {
      for (pc=s;  *pc;  pc++)    *pc = *(pc+1);
      
      }
    else s++;
    
    };
  
  }
static long StartDiv = 1000000l;
static char FirstFlag = TRUE;
/*
* PrintPretty()
*
* This will pretty print a long with commas.
*/
char *PrintPretty(long s, char *result)
  {
  Do_Stack_Check();
  if (StartDiv == 1)
    {
    sprintf(result, FirstFlag ? "%ld" : "%03ld", s);
    FirstFlag = TRUE;
    StartDiv  = 1000000l;
    return result;
    
    }
  if (s >= StartDiv)
    {
    sprintf(result, FirstFlag ? "%ld," : "%03ld,", s / StartDiv);
    FirstFlag = FALSE;
    s %= StartDiv;
    StartDiv /= 1000;
    PrintPretty(s, result + strlen(result));
    
    }
  else
    {
    StartDiv /= 1000;
    PrintPretty(s, result);
    
    }
  return result;
  
  }
/*
* normId()
*
* This function normalizes a node id.
***duplicate of lib source
char normId(label source, label dest)
  {
  int digitcount = 0;
  while (!isalpha(*source) && *source)
  source++;
  if (!*source) return FALSE;
  *dest++ = toUpper(*source++);
  while (!isalpha(*source) && *source)
  source++;
  if (!*source) return FALSE;
  *dest++ = toUpper(*source++);
  while (*source)
    {
    if (isdigit(*source))
      {
      *dest++ = *source;
      digitcount++;
      }
      source++;
      }
      *dest = '\0';
      return (digitcount > 8);
      }
      Commented out NormId */
      /*
      * hash()
      *
      * This function hashes a string to an integer.
      */
      UNS_16 hash(char *str)
        {
        UNS_16  h, shift;
        Do_Stack_Check();
        for (h=shift=0;  *str;  shift=(shift+1)&7, str++)
          {
          h ^= (toUpper(*str)) << shift;
          
          }
        return h;
        
        }
      /*
      * CleanEnd()
      *
      * This function cleans up a message trailer for later display via Continue or
      * .EH.  Inspired by Glen Heinz (MacCitadel).
      */
      char *CleanEnd(char *text)
        {
        char *ptr;
        int  wc, lc;	/* Word Count and Letter Count */
        Do_Stack_Check();
        if (strLen(text) == 0) return text;
        ptr = lbyte(text) - 1;      /* End of text of msg */
        /*
        * Strip trailing whitespace.  We structure the loop this
        * way to avoid any chance of accidentally accessing memory outside
        * of the memory area.
        */
        while (ptr != text - 1)
          {
          if (!(*ptr == ' ' || *ptr == NEWLINE || *ptr == TAB)) break;
          ptr--;
          
          }
        ptr++;	/* point at byte following last significant character */
        *ptr = 0;	/* tie it off with a NULL */
        /* Now we want to find a "preferred place" */
        for (wc = lc = 0, ptr--; wc < 4 && ptr > text && lc < 35; ptr--, lc++)
          {
          if (*ptr == ' ') wc++;
          if (*ptr == NEWLINE) break;     /* can't go beyond embedded NEWLINE */
          
          }
        if (ptr == text) return ptr;	/* if msg is empty or < 35 chars long */
        /* else */       return (ptr + 1);	/* else return "favored" spot */
        
        }
      /*
      * lbyte()
      *
      * This function finds the 0 byte of a string, returns pointer to it...
      */
      char *lbyte(char *l)
        {
        Do_Stack_Check();
        while (*l) l++;
        return l;
        
        }
      