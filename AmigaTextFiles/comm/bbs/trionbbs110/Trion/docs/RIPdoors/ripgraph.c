

/*  Trion BBS 'RIPscrip Clidoor Support' system functions               */
/*                                                                      */
/*  V 1.00   08 - Jan - 1998                                            */
/*                                                                      */
/*  (c) Copyright 1995-98 by Paul Spijkerman                            */


#include "ripdoor.h"


void ripprint(UWORD x, UWORD y, UBYTE *text)
{
   UBYTE *str = "!|@0000";           /* RIP_TEXT_XY */

   Store2Chars( &str[3], x );
   Store2Chars( &str[5], y );

   printf( "%s%s\r\n" , str, text);
}


/*
void Color(UWORD color)
{
   UBYTE *str = "!|c00";

   str[3] = MakeMegaChar( color / 36 );
   str[4] = MakeMegaChar( color % 36 );

   printf("%s\r\n" , str);
}
*/

void Color(UWORD color)
{
   UBYTE *str = "!|c00";

   Store2Chars( &str[3], color);

   printf("%s\r\n" , str);
}

/* Choose one of the 64 EGA colors for the 16 color RIP palette */
/* cX = 0-63 */

void SetPalette(UWORD  c1, UWORD  c2, UWORD  c3, UWORD  c4, UWORD  c5,
                UWORD  c6, UWORD  c7, UWORD  c8, UWORD  c9, UWORD c10,
                UWORD c11, UWORD c12, UWORD c13, UWORD c14, UWORD c15,
                UWORD c16)
{
   UBYTE *str = "!|Q00000000000000000000000000000000";

   Store2Chars( &str[ 3],  c1 );
   Store2Chars( &str[ 5],  c2 );
   Store2Chars( &str[ 7],  c3 );
   Store2Chars( &str[ 9],  c4 );
   Store2Chars( &str[11],  c5 );
   Store2Chars( &str[13],  c6 );
   Store2Chars( &str[15],  c7 );
   Store2Chars( &str[17],  c8 );
   Store2Chars( &str[19],  c9 );
   Store2Chars( &str[21], c10 );
   Store2Chars( &str[23], c11 );
   Store2Chars( &str[25], c12 );
   Store2Chars( &str[27], c13 );
   Store2Chars( &str[29], c14 );
   Store2Chars( &str[31], c15 );
   Store2Chars( &str[33], c16 );

   printf( "%s\r\n" , str);
}

/* color = 0-15 */
/* value = 0-63 */

void OnePalette(UWORD color, UWORD value)
{
   UBYTE *str = "!|a0000";

   Store2Chars( &str[3], color);
   Store2Chars( &str[5], value);

   printf("%s\r\n" , str);
}


/* Graphics draw mode */
/* mode = 0, normal drawing mode (overwrite) */
/* mode = 1, XOR (complimentary) mode */

void  GraphicsMode  (UWORD mode)
{
   UBYTE *str = "!|W00";

   Store2Chars( &str[3], mode );

   printf("%s\r\n" , str);
}


/*
void PlotAt(UWORD x, UWORD y)
{
   UBYTE *str = "!|X0000";

   str[3] = MakeMegaChar( x / 36 );
   str[4] = MakeMegaChar( x % 36 );
   str[5] = MakeMegaChar( y / 36 );
   str[6] = MakeMegaChar( y % 36 );

   printf("%s\r\n" , str);
}
*/

void PlotAt(UWORD x, UWORD y)
{
   UBYTE *str = "!|X0000";

   Store2Chars( &str[3], x );
   Store2Chars( &str[5], y );

   printf("%s\r\n" , str);
}

/*
void LineAt(UWORD x1, UWORD y1, UWORD x2, UWORD y2)
{
   UBYTE *str = "!|L00000000";

   str[3] = MakeMegaChar( x1 / 36 );
   str[4] = MakeMegaChar( x1 % 36 );
   str[5] = MakeMegaChar( y1 / 36 );
   str[6] = MakeMegaChar( y1 % 36 );

   str[ 7] = MakeMegaChar( x2 / 36 );
   str[ 8] = MakeMegaChar( x2 % 36 );
   str[ 9] = MakeMegaChar( y2 / 36 );
   str[10] = MakeMegaChar( y2 % 36 );

   printf( "%c%s\r\n" , 1, str);      /* ^A!|......\r  , \r of \r\n */
}
*/

void LineAt(UWORD x1, UWORD y1, UWORD x2, UWORD y2)
{
   UBYTE *str = "!|L00000000";

   Store2Chars( &str[3], x1 );
   Store2Chars( &str[5], y1 );

   Store2Chars( &str[7], x2 );
   Store2Chars( &str[9], y2 );

   /* printf( "%c%s\r\n" , 1, str); */     /* ^A!|......\r  , \r of \r\n */
   printf( "%s\r\n" , str);
}

void Rectangle(UWORD x1, UWORD y1, UWORD x2, UWORD y2)
{
   UBYTE *str = "!|R00000000";

   Store2Chars( &str[3], x1 );
   Store2Chars( &str[5], y1 );

   Store2Chars( &str[7], x2 );
   Store2Chars( &str[9], y2 );

   printf( "%s\r\n" , str);
}

void FilledRectangle(UWORD x1, UWORD y1, UWORD x2, UWORD y2)
{
   UBYTE *str = "!|B00000000";

   Store2Chars( &str[3], x1 );
   Store2Chars( &str[5], y1 );

   Store2Chars( &str[7], x2 );
   Store2Chars( &str[9], y2 );

   printf( "%s\r\n" , str);
}

/* style 00=normal, 01-03=dotted, 04 = custom (see pattern) */
/* pattern .. 16 bit line pattern */
/* line thickness  1 or 3 */

void SetLineStyle(UWORD style, UWORD pattern, UWORD thick)
{
   UBYTE *str = "!|=00000000";

   Store2Chars( &str[3], style   );
   Store4Chars( &str[5], pattern );
   Store2Chars( &str[9], thick   );

   printf("%s\r\n" , str);
}

/* pattern 0 = solid with background color */
/* pattern 1 = solid .. 2-11 = line and/or dot patterns */
/* color = 0-15 */

void FillStyle(UWORD pattern, UWORD color)
{
   UBYTE *str = "!|S0000";

   Store2Chars( &str[3], pattern );
   Store2Chars( &str[5], color   );

   printf("%s\r\n" , str);
}

/* define 8x8 pixel fill pattern */

void CustomFillPattern(UWORD  c1, UWORD  c2, UWORD  c3, UWORD  c4, UWORD  c5,
                       UWORD  c6, UWORD  c7, UWORD  c8, UWORD color)
{
   UBYTE *str = "!|s000000000000000000";

   Store2Chars( &str[ 3],  c1 );
   Store2Chars( &str[ 5],  c2 );
   Store2Chars( &str[ 7],  c3 );
   Store2Chars( &str[ 9],  c4 );
   Store2Chars( &str[11],  c5 );
   Store2Chars( &str[13],  c6 );
   Store2Chars( &str[15],  c7 );
   Store2Chars( &str[17],  c8 );
   Store2Chars( &str[19],  color );

   printf( "%s\r\n" , str);
}

/* Fills in all directions up to <border> color (don't change border) */

void FloodFill(UWORD x, UWORD y, UWORD border)
{
   UBYTE *str = "!|F000000";

   Store2Chars( &str[3], x );
   Store2Chars( &str[5], y );
   Store2Chars( &str[7], border );

   printf( "%s\r\n" , str);
}


void BezierCurve(UWORD x1, UWORD y1, UWORD x2, UWORD y2,
                 UWORD x3, UWORD y3, UWORD x4, UWORD y4, UWORD count )
{
   UBYTE *str = "!|Z000000000000000000";

   Store2Chars( &str[ 3],  x1 );
   Store2Chars( &str[ 5],  y1 );
   Store2Chars( &str[ 7],  x2 );
   Store2Chars( &str[ 9],  y2 );
   Store2Chars( &str[11],  x3 );
   Store2Chars( &str[13],  y3 );
   Store2Chars( &str[15],  x4 );
   Store2Chars( &str[17],  y4 );
   Store2Chars( &str[19],  count );

   printf( "%s\r\n" , str);
}


void Circle(UWORD x, UWORD y, UWORD radius)
{
   UBYTE *str = "!|C000000";

   Store2Chars( &str[3], x );
   Store2Chars( &str[5], y );
   Store2Chars( &str[7], radius );

   printf( "%s\r\n" , str);
}


/* ToDo and test:        */

/* RIP_TEXT_WINDOW       */
/* RIP_VIEWPORT          */
/* RIP_RESET_WINDOWS     */
/* RIP_ERASE_WINDOW      */
/* RIP_ERASE_VIEW        */
/* RIP_GOTOXY            */
/* RIP_MOVE              */
/* RIP_HOME              */
/* RIP_TEXT              */
/* RIP_FONT_STYLE        */
/* RIP_ERASE_EOL         */

/* RIP_OVAL              */
/* RIP_FILLED_OVAL       */
/* RIP_ARC               */
/* RIP_OVAL_ARC          */
/* RIP_PIE_SLICE         */
/* RIP_OVAL_PIE_SLICE    */
/* RIP_POLYGON           */
/* RIP_FILL_POLYGON      */
/* RIP_POLYLINE          */

/* RIP_MOUSE             */
/* RIP_KILL_MOUSE_FIELDS */
/* RIP_BEGIN_TEXT        */
/* RIP_REGION_TEXT       */
/* RIP_END_TEXT          */
/* RIP_DEFINE            */
/* RIP_QUERY             */
/* RIP_COPY_REGION       */
/* RIP_READ_SCENE        */
/* RIP_FILE_QUERY        */
/* RIP_ENTER_BLOCK_MODE  */

