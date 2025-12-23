

/*  Trion BBS 'RIPscrip Clidoor Support' system functions               */
/*                                                                      */
/*  V 1.00   08 - Jan - 1998                                            */
/*                                                                      */
/*  (c) Copyright 1995-98 by Paul Spijkerman                            */


#include "ripdoor.h"




void ButtonStyle(UWORD wid,  UWORD hgt,     UWORD orient, UWORD flags,
                 UWORD size, UWORD dfore,   UWORD dback,  UWORD bright,
                 UWORD dark, UWORD surface, UWORD grp_no, UWORD flags2,
                 UWORD uline_col, UWORD corner_col )
{
   UBYTE *str = "!|1B000000000000000000000000000000000000";

   Store2Chars( &str[ 4],  wid        );
   Store2Chars( &str[ 6],  hgt        );
   Store2Chars( &str[ 8],  orient     );
   Store4Chars( &str[10],  flags      );
   Store2Chars( &str[14],  size       );
   Store2Chars( &str[16],  dfore      );
   Store2Chars( &str[18],  dback      );
   Store2Chars( &str[20],  bright     );
   Store2Chars( &str[22],  dark       );
   Store2Chars( &str[24],  surface    );
   Store2Chars( &str[26],  grp_no     );
   Store2Chars( &str[28],  flags2     );
   Store2Chars( &str[30],  uline_col  );
   Store2Chars( &str[32],  corner_col );
/* Store6Chars( &str[34],  reserved   );   */

   printf( "%s\r\n" , str);
}

void RipButton(UWORD x1, UWORD y1, UWORD x2, UWORD y2, UBYTE hotkey,
               UWORD flags, UBYTE *text)
{
   UBYTE *str = "!|1U000000000000";

   Store2Chars( &str[ 4], x1 );
   Store2Chars( &str[ 6], y1 );

   Store2Chars( &str[ 8], x2 );
   Store2Chars( &str[10], y2 );

   Store2Chars( &str[12], hotkey   );
   Store1Chars( &str[14], flags    );
/* Store1Chars( &str[15], reserved ); */

   printf( "%s%s\r\n" , str, text);
}



/* copy image to clipboard */

void  GetImage      (UWORD x1, UWORD y1, UWORD x2, UWORD y2)
{
   UBYTE *str = "!|1C000000000";

   Store2Chars( &str[ 4], x1 );
   Store2Chars( &str[ 6], y1 );

   Store2Chars( &str[ 8], x2 );
   Store2Chars( &str[10], y2 );
/* Store1Chars( &str[12], reserved ); */

   printf( "%s\r\n" , str);
}

/* paste clipboard image to screen */
/* Mode: 0=copy, 1=XOR, 2=OR, 3=AND, 4=NOT */

void  PutImage      (UWORD  x, UWORD  y, UWORD mode)
{
   UBYTE *str = "!|1P0000000";

   Store2Chars( &str[ 4], x  );
   Store2Chars( &str[ 6], y  );

   Store2Chars( &str[ 8], mode );
/* Store1Chars( &str[10], reserved ); */

   printf( "%s\r\n" , str);
}

/* write clipboard contents to disk */

void  WriteIcon     (UBYTE *name)
{
   UBYTE *str = "!|1W0";

/* Store1Chars( &str[ 4], reserved ); */

   printf( "%s%s\r\n" , str, name);
}

/* loads and displays a disk based icon to screen  */
/* Mode: 0=copy, 1=XOR, 2=OR, 3=AND, 4=NOT         */
/* if clip=1 image is also copied to the clipboard */
/* if clip=0 image is only pasted on the screen    */

void  LoadIcon      (UWORD x, UWORD y, UWORD mode, UWORD clip, UBYTE *name)
{
   UBYTE *str = "!|1I000000010";     /* don't change the "10" part ! */

   Store2Chars( &str[ 4], x  );
   Store2Chars( &str[ 6], y  );

   Store2Chars( &str[ 8], mode );
   Store1Chars( &str[10], clip );

/* Store2Chars( &str[11], reserved ); */   /* default "10" */

   printf( "%s%s\r\n" , str, name);
}






