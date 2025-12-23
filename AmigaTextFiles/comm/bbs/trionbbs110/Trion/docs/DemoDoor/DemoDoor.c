
/*  Trion DemoDoor with additional CliDoor Support code linked in   */

/*  Start this door with the tilde code ~50 after the door name !!! */

/*  27-11-'95 By Paul Spijkerman                                    */


#include "adoor.h"        /* link code with  adoor.c !! */



void Chat           (struct DoorIO *pd, struct ADS_Data *pp);
void TestDownload   (struct DoorIO *pd, struct ADS_Data *pp);
void LogDemo        (struct DoorIO *pd, struct ADS_Data *pp);


int main(int argc, char **argv)    /* was void */
{
   struct ADS_Data      port , *pport;
   struct DoorIO        io   , *pio;
   int    node = 0, ret = 0, ready = 0, time, sysop;
   char   name[37];
   UBYTE  c;

   pport = &port;
   pio   = &io;

   onbreak(brk);

   SetOpIO(pio);


   if (argc != 2){

      nprintf(pio, "Trion DemoDoor V 1.10 Copyright 1996-97 By Paul Spijkerman\r\n\r\n");
      nprintf(pio, "Use:    CliDoor  \"doors:demodoor/demodoor ~50\"\r\n");
      Delay(120L);

   }else{

      node = atoi( argv[1] );

      if (SetopPort( pport , node, 0 )){

         GetUserStringField(pport, UDSF_NAME , name );

         while(!ready && Carrier(pio) ){

            col(pio, HYELLOW , BLUE);
            nprintf(pio, "Trion Additional (cli)Door Support Demo door\r\n\r\n");

            col(pio, HYELLOW , BLACK);
            time  = GetTimeOnline(pport);
            sysop = CheckSysopAvailable(pport);

            if (sysop){
               nprintf(pio, "Sysop is available for a chat.\r\n");
            }else{
               nprintf(pio, "Sysop is not around.\r\n");
            }
            nprintf(pio, "Hello %s , you still have %d minutes left.\r\n\r\n", name, time);

            nprintf(pio, "Press a key ..\r\n\r\n");

            nprintf(pio, "S.  Play Sample. \r\n");
            nprintf(pio, "P.  Page Sysop. \r\n");
            nprintf(pio, "D.  Download startup-sequence. \r\n");
            nprintf(pio, "C.  Simple chat in 2 colors. \r\n");
            nprintf(pio, "L.  Write a line of text to the log. \r\n");
            nprintf(pio, "+.  Increase time online. \r\n");
            nprintf(pio, "-.  Decrease time online. \r\n");
            nprintf(pio, "X.  Drop Carrier\r\n");
            nprintf(pio, "Q.  Quit\r\n\r\n");

            c = GetCharacter(pio);

            if (c == 's' || c == 'S')   PlaySample( pport , "uhoh.8sv" );
            if (c == 'p' || c == 'P'){
               PageSysop(pport);
               PlaySampleNum( pport , "startrek-alert", 3 );
            }
            if (c == 'c' || c == 'C')   Chat( pio  , pport );
            if (c == 'd' || c == 'D')   TestDownload(pio , pport);
            if (c == 'l' || c == 'L')   LogDemo(pio , pport);
            if (c == '+')               IncreaseTimeOnline(pport , 1);
            if (c == '-')               DecreaseTimeOnline(pport , 1);
            if (c == 'x' || c == 'X'){
               DropCarrier(pport);
               ready = 1;
            }

            if (c == 'q' || c == 'Q')   ready = 1;

            cls(pio);
         }
         nprintf(pio, "Bye !\r\n");


      }
      RemovePort( pport );
   }

   RemoveIO( pio );

   return ret;
}



void TestDownload(struct DoorIO *pd, struct ADS_Data *pp)
{
   nprintf(pd, "Let's mark the startup-sequence\r\n");

   /* mark as much files as needed before calling download */

   MarkFile(pp , "s:startup-sequence" );

   Delay(100);

   nprintf(pd, "Here it comes\r\n");
   Download( pp );

   Delay(100);
}




void Chat(struct DoorIO *pd, struct ADS_Data *pp)
{
   UBYTE key , ID, ready = 0, prevID = 0;

   cls ( pd );
   col ( pd, HGREEN , BLUE );

   nprintf(pd, "Type 'q' to Quit.\r\n");

   col ( pd, HGREEN , BLACK );


   SplitChatMode(pp , 1);

   while ( Carrier(pd) && !ready){

      key = GetCharacter(pd);

      ID  = (key & 128) >> 7;       /* Extract ID bit   (0-1)  */
      key = (key & 127);            /* Make 7 bit ASCII        */

      if (prevID != ID){
         prevID = ID;
         if (ID == 0)    col ( pd, HGREEN  , BLACK );
         if (ID == 1)    col ( pd, HYELLOW , BLACK );
      }

      if (key == 'q') ready = 1;

      if (key == '\r'){
         nprintf(pd, "\r\n");
      }else{
         nprintf(pd, "%c" , key);
      }

      /* printf("%c", key);  doesn't work because of missing \r\n  */
      /*                     best to use the nprintf();            */

      /* printf("char (%c) value (%d) ID (%d)\r\n" , key , key , ID); */
   }

   SplitChatMode(pp , 0);
}




void LogDemo(struct DoorIO *pd, struct ADS_Data *pp)
{
   char string[41], buf[80];

   col ( pd, HGREEN , BLUE );
   nprintf(pd, "Let's type a line to the log.\r\n\r\n");

   col ( pd, HYELLOW , BLACK );
   nprintf(pd, "----------------------------------------\r\n");

   if (GetString(pd, string, 41)){
      sprintf(buf , "DemoDoor: %s" , string);
      PrintToLog(pp, buf);
   }

}


