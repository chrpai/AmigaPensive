

/*  Trion BBS 'RIPscrip Clidoor Support' MakeRipScreen door             */
/*                                                                      */
/*  V 1.00   08 - Jan - 1998                                            */
/*                                                                      */
/*  (c) Copyright 1995-98 by Paul Spijkerman                            */





#include "ripdoor.h"
#include "adoor.h"

void MakeRIPScreen(struct DoorIO *pd, struct ADS_Data *pp, int screen);
char hoofdletter(char c);



int main(int argc, char **argv)    /* was void */
{
   struct ADS_Data      port , *pport;
   struct DoorIO        io   , *pio;
   int    node = 0, ret = 0, screen;

   pport = &port;
   pio   = &io;

   onbreak(brk);

   SetOpIO(pio);


   if (argc != 3){

      nprintf(pio, "Trion RipDoor V 1.00 Copyright 1996-98 By Paul Spijkerman\r\n\r\n");
      nprintf(pio, "Use:    RipDoor  \"doors:demodoor/demodoor ~50 screen\"\r\n");
      Delay(120L);

   }else{

      node   = atoi( argv[1] );
      screen = atoi( argv[2] );

      if (screen >= 0 && screen <= 15){

         if (SetopPort( pport , node, 0 )){

           MakeRIPScreen(pio , pport, screen);

         }
         RemovePort( pport );
      }
   }

   RemoveIO( pio );

   return ret;
}


UBYTE *name[16] = {
   "Main menu",     "Files menu",     "Mail menu",      "Bulletin menu",
   "Door menu",     "Logout menu",    "Sysop menu",     "User edit menu", 
   "Test menu",     "Protocol menu",  "Browse menu",    "List menu",
   "Download menu", "Read msgs menu", "Scan msgs menu", "Editor menu"  };



/* For instance :  "<>File<>F^M"    */
/*   "File" = button text ,  "F^M" = command when presses (F + return) */

/* For instance :  " "      <- skip this button position */
/* For instance :  "*"      <- goto next line of buttons */



UBYTE *data[16][40] = {

     /* main menu      0  */

     { "<>Mail<>M",   "<>Files<>F",    "<>Bulletins<>B","<>Door<>D",
       "<>UserEd<>E", "<>Log off<>G",
       "<>Page<>P",   "<>Comment<>C",  "<>Conf<>I",     "<>Listnodes<>L",
       "<>SendMsg<>S","<>Sysop<>$",    "" },

     /* file menu      1  */

     { "<>Group<>G",       "<>Area<>O",      "<>File list<>C", "<>New files<>N",
       "<>Scan Date<>S",   "<>Scan days<>Y",    /* "*", */
       "<>Find files<>F",  "<>Download<>D", "<>Upload<>U",
       "<>List menu<>L",   "<>Browse Menu<>B",  /* "*", */
       "<>View archive<>T",
       "<>Ratio report<>R", "<>Protocol<>P", "<>Mark file<>M", "<>Allfiles<>I",
       "<>List mark.<>J",  "<>Remove marked<>V", "<>Clear marked<>*", "<>Add file<>+",
       "<>Del. file<>K",  "<>Edit file<>E", "<>Make free<>H", "<>Dos files<>#",
       "<>Main menu<>Q", "  <>Logoff<>G",  ""  },

     /* Mail menu      2  */

     { "<>Group<>G",      "<>Area<>O",   "<>Read menu<>R", "<>Scan menu<>S", "*",
       "<>Wait mail<>W",  "<>Read new<>N","<>Read forward<>F", "<>Global new<>Z", "*",
       "<>Read marked<>M","<>Unmark msgs<>U", "<>Editor<>E", "<>Edit sig<>I",
       "<>Write msg<>L",  "<>Rescan wait<>?", "*",
       "<>Main menu<>Q",  "<>Log off<>G", "" },

     /* Bulletin menu  3  */

     { "<>About system<>I",  "<>Teletext<>A",       "*",
       "<>Lastcallers<>L",   "<>Usage<>U",          "*",
       "<>Top Up/Down<>0",   "<>Top down files<>1", "<>Top down bytes<>2",
       "<>Top up files<>3",  "<>Top up bytes<>4",   "<>Top callers<>5",
       "<>Top time on<>6",   "<>Top msgs written<>7",  "*",
       "<>Main menu<>Q",     "<>Comment sysop<>C",  "<>Logout<>G", "" },

     /* Door menu      4  */

     { "<>Tetris<>T", "<>Shell<>S", "*"
       "<>PageDoor<>P", "<>SplitChat<>=", "<>LineChat<>-", "*",      
       "<>Dutch robo<>R", "<>English Robo<>R", "*",
       "<>CDrom door<>C", "<>Demodoor<>D", "*",
       "<>Main menu<>Q", "<>Files<>F", "<>Mail<>M", "<>Bulletin<>B", "<>Log off<>G", "" },

     /* Logout menu    5  */

     { "<>Main menu<>M",    "<>Msg to sysop<>C", "<>Log off<>G", "*",
       "<>Previous menu<>P","<>Msg to All<>A"  , "" },

     /* Sysop menu     6  */

     { "" },

     /* User edit menu 7  */

     { "" },

     /* test menu      8  */

     { "" },

     /* Protocol menu  9  */

     { "<>Zmodem<>Z", "*",
       "<>Ymodem<>Y", "<>Ymodem-G<>G", "*",
       "<>Xmodem<>X", "*",
       "<>Kermit<>K", "*",
       "<>Back<>Q" ,  "" },

     /* Browse menu    10 */

     { "<>Chrono<>C",   "<>Alpha<>L", "<>ChronoNew<>N", "<>AlphaNew<>M", "*",
       "<>FindFile<>Z", "<>ChronoUnval<>V", "*",
       "<>Chrono<>A",   "<>Alpha<>H", "<>ChronoNew<>G", "<>AlphaNew<>B", "*",
       "<>FindFile<>F", "<>ChronoUnval<>U", "*",
       "<>ReverseChrono<>T", "<>ReverseAlpha<>R", "" },

     /* List menu      11 */

     { "<>Chrono<>C",   "<>Alpha<>L", "<>ChronoNew<>N", "<>AlphaNew<>M", "*",
       "<>FindFile<>Z", "<>ChronoUnval<>V", "*",
       "<>Chrono<>A",   "<>Alpha<>H", "<>ChronoNew<>G", "<>AlphaNew<>B", "*",
       "<>FindFile<>F", "<>ChronoUnval<>U", "*",
       "<>ReverseChrono<>T", "<>ReverseAlpha<>R", "" },

     /* Download menu  12 */

     { "<>Download & Logoff<>G", "<>Mark a file<>M", "*",
       "<>Start Download<>D",    "<>Mark allfiles<>A", "*",
       " ", " ",                 "<>Remove marked<>V", "*",
       "<>To file menu<>Q",      "<>Clear marked files<>*",   "" },

     /* Read msgs menu 13 */

     { "<>New<>N", "<>Forward<>F", "<>Reverse<>R", "<>Global new<>Z", "*",
       "<>Wait mailing<>W", "<>Marked msgs<>M",                       "*",
       "<>To User<>1", "<>From User<>2", "<>To and From<>3", "<>Subject<>4", "*",
       "<>To,From,Subject<>5",
       "<>To you<>6", "<>From you<>7", "<>To & From you<>8", "<>Rev. to you<>9",
       "<>Rev. From you<>A", "<>Rev. To & From you<>B", ""  },

     /* Scan msgs menu 14 */

     { "<>New<>N", "<>Forward<>F", "<>Reverse<>R", "<>Global new<>Z", "*",
       "<>Wait mailing<>W", "<>Marked msgs<>M",                       "*",
       "<>To User<>1", "<>From User<>2", "<>To and From<>3", "<>Subject<>4", "*"
       "<>To,From,Subject<>5",
       "<>To you<>6", "<>From you<>7", "<>To & From you<>8", "<>Rev. to you<>9",
       "<>Rev. From you<>A", "<>Rev. To & From you<>B", ""  },

     /* Editor menu    15 */

     { "<>Line Editor<>0",         "*",
       "<>Line ED (CED local)<>1", "*",
       "<>Full screen<>2",         "*",
       "<>Back<>Q",                ""   },


};


void MakeRIPScreen(struct DoorIO *pd, struct ADS_Data *pp, int screen)
{
   int    x, y, xs, ys, xt, t, number, a[8], ready, len;
   UBYTE  key , retkey , ID , str[5], *p;


   onbreak(brk);

   printf("[2J");          /* ClrScr(); */
   printf("[2!\r\n");      /* RipScrip on, return !! .. rip starts on begin of line */


   Color(15);     /* white */

   p = name[screen];

   ripprint( 250 , 15 , p );
   /* ripprint( 250 , 15 , "Main menu" ); */


/* ButtonStyle( 60, 20, 02, 32+256+512+1024, 3, 15, 8, 15, 8, 7, 0, 1, 14, 7 ); */
/* RipButton  ( 400, 150, 500, 180, '=', 0, "<>Button<>b^M" ); */

   ButtonStyle( 0, 0, 02, 32+256+512+1024, 3, 15, 8, 15, 8, 7, 0, 1, 14, 7 );

/* RipButton  ( 300, 80, 400, 95, '=', 0, "<>Files<>F^M" ); */
/* RipButton  ( 410, 80, 510, 95, '=', 0, "<>Mail<>M^M" );  */
/* RipButton  ( 520, 80, 620, 95, '=', 0, "<>Doors<>D^M" ); */


   number = 12;
   ready  =  0;

   x  = 20;         /* starting point */
   y  = 100;

   xs = 80;         /* button size */
   ys = 12;

   /* for (t = 0; t < number; t++){ */
   t = 0;

   while (!ready){
      /* RipButton  ( x, y, x+xs, y+ys, '=', 0, "<>Doors<>D" ); */

      p = data[screen][t];

      if (*p == '\0'){
         ready = 1;             /* end of buttons */
      }else{
         if (*p == '*'){
            x  = 20;            /* start new line of buttons */
            y += 24;
         }else{
            if (*p != ' '){
               len = strlen(p) - 5;       /* length of button text */
               len = len * 8;             /* in pixels ! */

               if (len < xs + 8){
                  xt = xs;
               }else{
                  xt = 2 * xs + 15;
               }

               RipButton  ( x, y, x+xt, y+ys, '=', 0, p );

               x += 15;
               x += xt;
               if (x > 550){
                  x  = 20;
                  y += 24;
               }

            }else{

               x += 15;
               x += xs;
               if (x > 550){
                  x  = 20;
                  y += 24;
               }

            }
         }
      }


      t++;
   }

   endrip();     /* RIP screen build ? */



 

   SplitChatMode( pp , 1);
   /* nprintf(pd, "[s"); */

   retkey = ' ';
   ID     = 0;

   x  = 0;         /* starting point */
   FillStyle(13, x);               /* pattern , color */

   ready = 0;
   while ( Carrier(pd) && !ready){

      /*=== Normal ===*/
/*
      key = hoofdletter ( GetCharacter(pd) );
*/
      /*=== Animated (polled I/O) ===*/

      Delay(10L);   /* wait 1/5th of a seconde, don't hog the CPU/blitter */
      key = hoofdletter ( MayGetCharacter(pd) );


      ID  = (((key & 128) >> 7) & 1);       /* Extract ID bit    (0-1)   */
      key = (key & 127);                    /* Make 7 bit ASCII          */

      if (key > 0){
         ready  = 1;
         retkey = key;
      }else{
         /* animation */

         y = 1 << x;
         for (t = 0; t < 8; t++){
            a[t] = y;
            y = y << 1;
            if (y == 256) y = 1;
         }
         /* FillStyle(13, x+7); */              /* pattern , color */
         CustomFillPattern(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7], 13);
         FilledRectangle( 20, 30, 580, 90 );
         x++;
         if (x > 7) x = 0;      /* x = 0 - 7 */


         /* Color(x & 15);  */
         /* LineAt( x, y, x, y+60); */
         /* x++; */
         /* if (x == 540) x = 0; */
      }
   }/* end while */


   /* nprintf(pd, "[u"); */
   SplitChatMode( pp , 0);

   if (key != 0){
      FlushBuffer( pp );
      str[0] =  retkey;
      str[1] = '\0';
      PushIntoBuffer( pp, str , ID);
   }



   printf("[1!");      /* RipScrip off, remove buttons, restore ANSI color */
   printf("[2J");          /* ClrScr(); */

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




/* Maak een hoofdletter van de aangeboden letter */
/* verschil met de C-lib routine is dat niet     */
/* met integers gewerkt wordt                    */

char hoofdletter(char c)
{
   if (c >= 'a' && c <= 'z')
      return (char) (c+('A'-'a'));
   else
      return (char) (c);
}


