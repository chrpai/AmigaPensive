

/*  Trion BBS 'RIPscrip Clidoor Support' system functions               */
/*                                                                      */
/*  V 1.00   08 - Jan - 1998                                            */
/*                                                                      */
/*  (c) Copyright 1995-98 by Paul Spijkerman                            */


#include "ripdoor.h"


UBYTE MakeMegaChar(int n)
{
   UBYTE c = '0';

   if (n >= 0 && n <= 9)  c = n + '0';
   if (n >= 10)           c = n + 'A' - 10;

   return c;
}

void Store1Chars(UBYTE *p, UWORD value)
{
   *p = MakeMegaChar( value );
}

void Store2Chars(UBYTE *p, UWORD value)
{
   *p++ = MakeMegaChar( value / 36 );
   *p   = MakeMegaChar( value % 36 );
}

void Store3Chars(UBYTE *p, UWORD value)
{
   UWORD hi, mi, lo;

   hi = (value / 1296);                /*  1296 = 36*36  */
   mi = (value - (hi * 1296)) / 36;
   lo = (value % 36);                  /* lo = value - (hi * 1296) - (mi * 36);  */

   *p++ = MakeMegaChar( hi );
   *p++ = MakeMegaChar( mi );
   *p   = MakeMegaChar( lo );
}

void Store4Chars(UBYTE *p, UWORD value)
{
   UWORD hi, lo;

   hi = value / 1296;
   lo = value - hi * 1296;

   Store2Chars(p,   hi);
   Store2Chars(p+2, lo);
}




void endrip(void)
{
   UBYTE *str = "!|#|#|#";         /* RIP_NO_MORE */

   printf( "%s%s\r\n" , str);
}

