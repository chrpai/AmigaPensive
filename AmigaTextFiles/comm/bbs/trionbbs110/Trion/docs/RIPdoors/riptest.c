

/*  Trion BBS 'RIPscrip Clidoor Support' test program                   */
/*                                                                      */
/*  V 1.00   08 - Jan - 1998                                            */
/*                                                                      */
/*  (c) Copyright 1995-98 by Paul Spijkerman                            */




#include "ripdoor.h"


int brk(void);
int brk(void)
{
   return(0);
}





int main(int argc, char **argv)    /* was void */
{
   int t, r, d;

   onbreak(brk);

   printf("[2J");               /* ClrScr(); */
   printf("[2!\r\n");           /* RipScrip on, return !! .. rip starts on begin of line */

   /* palette: 4 times (black, dark grey, light grey, white) */
   /* SetPalette (00,56,07,63,  00,56,07,63,  00,56,07,63,  00,56,07,63  ); */

   /* OnePalette (00, 00); */  /* background BLACK !! */
   /* OnePalette (01, 63); */  /* white */
   /* OnePalette (02, 00); */  /* black */
   /* OnePalette (03, 56); */  /* dark grey */
   /* OnePalette (04, 07); */  /* light grey */
   /* OnePalette (05, 63); */  /* white */

   Color(14);
   ripprint( 200 , 15 , "RIPscrip BBS graphics test" );


   /* Color(15); */
   for (t = 0; t < 10; t++){
      for (r = 0; r < 10; r++){
         d = (r * t) & 15;
         if (d == 0) d = 1;
         Color(d);
         PlotAt(10 + t<<2, 10 + r<<1);
      }
   }

   /* Color(15); */
   for (t = 0; t < 15; t++){
      for (r = 0; r < 15; r++){
         d = (r + t) & 15;        /*  d = (r * t) & 15; */
         Color(d);
         PlotAt(10 + t, 30 + r);
      }
   }

   GetImage(10,30, 25,45);
   for (r = 0; r < 4; r++){
      for (t = 0; t < 4; t++){
         PutImage( 10+15*t, 45+15*r, 0);
      }
   }


/*
   Color(2);
   LineAt(101,201,201,101);
   Color(3);
   LineAt(100,100,200,200);
*/

   for (t = 0; t < 16; t++){
      Color(t);
      LineAt(20,100+t*7,120,200-t*7);
   }


   Delay(50L);
   GraphicsMode( 01 );              /* 1 = XOR */
   for (t = 0; t < 16; t++){
      Color(t);
      LineAt(20,100+t*7,120,200-t*7);
   }
   GraphicsMode( 00 );


   for (t = 0; t < 7; t++){
      Color(t);
      d = t << 1;
      Rectangle( 140+d, 50+d, 200-d ,100-d);
   }
   FillStyle( 11, 3);          /* style, color */
   FloodFill( 170, 75, 6);     /* x, y, border */


   for (t = 0; t < 12; t++){
      Color(t);
      Circle(250,100,10+t*3);
      
   }


   for (t = 0; t < 12; t++){
      FillStyle(t, t);
      d = 0; /* t << 1; */
      FilledRectangle( 140+d + 40*t, 150+d, 200-d+40*t ,200-d);
   }

   CustomFillPattern( 1+16, 2+32, 4+64, 2+32, 1+16, 2+32, 4+64, 2+32 ,15);
   FilledRectangle( 140, 150, 200, 200);

   Color(3);
   for (t = 0; t < 5; t++){
      SetLineStyle(t, 255+170*256, 1);
      LineAt(320, 20+t*5, 600, 20+t*5 );
   }

   Color(14);
   for (t = 0; t < 5; t++){
      SetLineStyle(t, 255+170*256, 3);
      LineAt(320, 45+t*5, 600, 45+t*5 );
   }

   SetLineStyle(0, 0, 1);       /* reset to normal */


   BezierCurve(0,199,  350,0,  440,0,  639,199, 30 );



/* ButtonStyle( 60, 20, 02, 32+256+512+1024, 3, 15, 8, 15, 8, 7, 0, 1, 14, 7 ); */
/* RipButton  ( 400, 150, 500, 180, '=', 0, "<>Button<>b^M" ); */

   ButtonStyle( 0, 0, 02, 32+256+512+1024, 3, 15, 8, 15, 8, 7, 0, 1, 14, 7 );
   RipButton  ( 300, 80, 400, 95, '=', 0, "<>Files<>F^M" );
   RipButton  ( 410, 80, 510, 95, '=', 0, "<>Mail<>M^M" );
   RipButton  ( 520, 80, 620, 95, '=', 0, "<>Doors<>D^M" );



   /* XOR Test */

   GraphicsMode( 01 );              /* 1 = XOR */
   for (t = 0; t < 100; t++){
      Color(15);
      LineAt(0,t,600,t);
      LineAt(0,t,600,t);
   }
   GraphicsMode( 00 );


   Delay(200L);
   endrip();
   printf("[1!");               /* RipScrip off */
   Delay(50L);

   return 0;
}



/*  RIP Palette is 64 EGA kleuren     */
/*  default 16 color RIP palette :    */
/*  Value Color          EGAnr  RGB   */

/*  00    Black          0   0x0000  (00 is always background color) */
/*  01    Blue           1   0x000A  */
/*  02    Green          2   0x00A0  */
/*  03    Cyan           3   0x00AA  */
/*  04    Red            4   0x0A00  */
/*  05    Magenta        5   0x0A0A  */
/*  06    Brown          20  0x0A50  ( 7 ?)  */
/*  07    Light Gray     7   0x0AAA  (20 ?)  */
/*  08    Dark  Gray     56  0x0555  */
/*  09    Light Blue     57  0x055F  */
/*  0A    Light Green    58  0x05F5  */
/*  0B    Light Cyan     59  0x05FF  */
/*  0C    Light Red      60  0x0F55  */
/*  0D    Light Magenta  61  0x0F5F  */
/*  0E    Yellow         62  0x0FF5  */
/*  0F    White          63  0x0FFF  */

