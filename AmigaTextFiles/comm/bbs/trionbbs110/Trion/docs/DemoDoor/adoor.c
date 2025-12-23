

/*  Trion BBS 'Additional Clidoor Support' system functions             */
/*                                                                      */
/*  V 1.00   25 - 11 - 1995                                             */
/*  V 1.10    7 -  6 - 1997                                             */
/*                                                                      */
/*  (c) Copyright 1995-97 by Paul Spijkerman                            */



#include  "adoor.h"


int  SendMessage    (struct ADS_Data *p);
int  SafePutToPort  (struct Message *, STRPTR);




/*===( Input / Output code )=================================*/


void SetOpIO(struct DoorIO *p)
{
   p->ConInput  = Input();
   p->ConOutput = Output();

   p->carrier = 1;

   SetMode (p->ConInput , 1);     /* RAW: mode */

   p->xmax = 80;
   p->ymax = 40;

   p->kar  = 0;

   norm(p);      /*   mfc, mbc, mhigh  */
   cls (p);      /*   mx,  my          */
}


void RemoveIO(struct DoorIO *p)
{
   SetMode (p->ConInput , 0);     /* CON: mode */
}


/* 'Normal' GetCharacter() function */

/*
UBYTE GetCharacter(struct DoorIO *p)
{
   UBYTE c = 0, getstring[3];

   while (c == 0 && p->carrier){
      if (0 != WaitForChar( p->ConInput ,10000)){
         Read( p->ConInput , getstring, 1);
         c = getstring[0];
         if (c == 3) p->carrier = 0;        /* carrier lost */
      }
   }
   return c;
}
*/

/* 'Normal' MayGetCharacter() function */

/*
UBYTE MayGetCharacter(struct DoorIO *p)
{
   UBYTE  c = 0, getstring[3];
   getstring[0] = '\0';

   if (0 != WaitForChar( p->ConInput , 0)){
      Read( p->ConInput , getstring, 1);      /* IPV getstring -> &c */
      c = getstring[0];
      if (c == 3) p->carrier = 0;             /* carrier lost */
   }
   return c;
}
*/

/*  GetCharacter() function with code for UnGetCharacter() fuction */

UBYTE GetCharacter(struct DoorIO *p)
{
   UBYTE c = 0, getstring[3];

   if (p->kar > 0){
      c = p->kar;
      p->kar = 0;
   }else{
      while (c == 0 && p->carrier){
         if (0 != WaitForChar( p->ConInput ,10000)){
            Read( p->ConInput , getstring, 1);
            c = getstring[0];
            if (c == 3) p->carrier = 0;        /* carrier lost */
         }
      }
   }
   return c;
}


/* Stores ONE character that was read to much                      */
/* Usefull in a 'splitchat' door .. when you read a character from */
/* 'The other side' when you are reading a line 'from this side'   */
/* Just unget the character and finish the line ..                 */

void UnGetCharacter(struct DoorIO *p, UBYTE kar)
{
   p->kar = kar;
}


/*  MayGetCharacter() function with code for UnGetCharacter() fuction */

UBYTE MayGetCharacter(struct DoorIO *p)
{
   UBYTE  c = 0, getstring[3];
   getstring[0] = '\0';

   if (p->kar > 0){
      c = p->kar;
      p->kar = 0;
   }else{
      if (0 != WaitForChar( p->ConInput , 0)){
         Read( p->ConInput , getstring, 1);      /* IPV getstring -> &c */
         c = getstring[0];
         if (c == 3) p->carrier = 0;             /* carrier lost */
      }
   }
   return c;
}


int  GetString(struct DoorIO *p, char *str, int strsize)
{
   int   len = 0, pos = 0, t, d;
   UBYTE kar;

   while (/*len < strsize-1 &&*/ '\r' != (kar = GetCharacter(p)) &&  Carrier(p)){

      /* cursor ->    (2+pos, y) */
      /* nprintf(p, "[%d;%dH",27,7 + pos); */

      switch (kar){

         case 8:                              /* case of backspace */
            if (pos > 0){
               nprintf(p, "[D");
               for (t = pos - 1; t < len - 1; t++)
                  nprintf(p, "%c", str[t] = str[t+1] );
               nprintf(p, "%c", str[len-1] = ' ' );
               d = len - pos + 1;
               nprintf(p, "[%dD", d);
               len--;
               pos--;
            }
         break;

         case 0x7f:                           /* case of Delete */
            if (len > 0 && pos < len){
               for (t = pos; t < len - 1; t++)
                  nprintf(p, "%c", str[t] = str[t+1] );
               nprintf(p, "%c", str[len-1] = ' ' );
               d = len - pos;
               nprintf(p, "[%dD", d);
               len--;
            }
         break;

         case    0:
         case    3:                           /* ignore Ctrl-c   */
         case '\n':                           /* ignore newlines */
         break;

         case 0x1b:                           /* escape (sequence) */
            kar = GetCharacter(p);
            if (kar == '['){
               kar = GetCharacter(p);
               if (kar == 'D'){               /* backward */
                  if (pos > 0){
                     pos--;
                     nprintf(p, "[D");
                  }
               }
               if (kar == 'C'){               /* forward  */
                  if (pos < len){
                     pos++;
                     nprintf(p, "[C");
                  }
               }
            }
         break;

         default:
            if (len < strsize-1){
/*
            str[pos++] = kar;
            if (pos > len)  len = pos;
            nprintf(p, "%c", kar);
*/
            if (pos == len){
               str[pos++] = kar;
               if (pos > len)  len = pos;
               nprintf(p, "%c", kar);
            }else{
               for (t = len; t >= pos; t--)
                  str[t+1] = str[t];
               str[pos] = kar;

               for (t = pos; t <= len; t++)
                  nprintf(p, "%c", str[t]);

               d = len - pos;
               nprintf(p, "[%dD", d);
               len++;
               pos++;
            }

            }
         break;

      }
   }
   str[len] = '\0';
   /* nprintf(p, "\r\n"); */
   return len;
}


int Carrier(struct DoorIO *p)        /* voor de compleet heid :) */
{
   return (p->carrier);
}


int nprintf(struct DoorIO *p, char *fmt, ...)
{
   int len;
   va_list var_args;
   char str[120];

   va_start(var_args, fmt);
   len = (int) vsprintf(str, fmt, var_args);    /* of eigen brouwsel */
   va_end(var_args);

   PutStr (str);
   Flush  (p->ConOutput);
   return (len);
}


int brk(void)
{
   return (0);
}


/*===( FileMark / Download code )=================================*/


int  SetopPort(struct ADS_Data *p, int node, int quiet)
{
   int ret = 0;

   p->node    = node;
   p->quiet   = quiet;
   p->BBSMsg  = NULL;
   p->BBSreplyport = NULL;
   p->IDMode  = 0;

   if (p->node < 0 || p->node > 10){
      if (!p->quiet)   printf("Got an unvalid node number '%d'\n", p->node);
   }else{

      if (NULL == (p->BBSreplyport = CreatePort(0,0))){
         if (!p->quiet)    printf("Couldn't create BBSreplyport.\n");
      }else{

         if (NULL == (p->BBSMsg = (struct ADoorMessage *) AllocMem(sizeof(struct ADoorMessage), MEMF_PUBLIC | MEMF_CLEAR))){
            if (!p->quiet)    printf("Door couldn't get memory.\n");
         }else{

            p->BBSMsg->ADS_Msg.mn_Node.ln_Type = NT_MESSAGE;   /* make up a message, */
            p->BBSMsg->ADS_Msg.mn_Length       = sizeof(struct ADoorMessage);  /* including the replyport.  */
            p->BBSMsg->ADS_Msg.mn_ReplyPort    = p->BBSreplyport;

            GetSharedData(p);

            ret = 1;    /* succes */
         }
      }
   }

   if (ret == 0)  RemovePort(p);

   return ret;
}


int  RemovePort(struct ADS_Data *p)
{
   int ret = 0;

   if (p->BBSMsg)        FreeMem    ( p->BBSMsg, sizeof(struct ADoorMessage));
   if (p->BBSreplyport)  DeletePort ( p->BBSreplyport);

   return ret;
}






int  DropCarrier(struct ADS_Data *p)
{
   p->BBSMsg->ADS_Command     =   ADSC_DROP_CARRIER;
   return SendMessage( p );
}


int  PrintToLog(struct ADS_Data *p, char *string)
{
   p->BBSMsg->ADS_Command     =   ADSC_STRING_TO_LOG;
   strcpy (p->BBSMsg->ADS_String , string);
   return SendMessage( p );
}


/* an extension (.TXT .ANSI .RIP etc.) is not needed */

int  TypeFile(struct ADS_Data *p, char *string)
{
   p->BBSMsg->ADS_Command     =   ADSC_TYPE_FILE;
   strcpy (p->BBSMsg->ADS_String , string);
   return SendMessage( p );
}

int  GetTimeOnline(struct ADS_Data *p)
{
   int res = -1;

   p->BBSMsg->ADS_Command     =   ADSC_GET_TIME_ONLINE;
   res = SendMessage( p );

   if (res >= 0){
      return p->BBSMsg->ADS_Data_word;
   }else{
      return -1;
   }
}

int  IncreaseTimeOnline(struct ADS_Data *p, int time)
{
   p->BBSMsg->ADS_Command     =   ADSC_CHANGE_TIME_ONLINE;
   p->BBSMsg->ADS_SubCommand  =   1;    /* Increase */
   p->BBSMsg->ADS_Data_word   =   time;
   return SendMessage( p );
}

int  DecreaseTimeOnline(struct ADS_Data *p, int time)
{
   p->BBSMsg->ADS_Command     =   ADSC_CHANGE_TIME_ONLINE;
   p->BBSMsg->ADS_SubCommand  =   0;    /* Decrease */
   p->BBSMsg->ADS_Data_word   =   time;
   return SendMessage( p );
}





int  Download(struct ADS_Data *p)
{
   p->BBSMsg->ADS_Command     =   ADSC_DOWNLOAD_DOS_FILES;
   return SendMessage( p );
}


int  MarkFile(struct ADS_Data *p, char *str)   /* 79 letters max */
{
   p->BBSMsg->ADS_Command     =   ADSC_MARK_DOS_FILE;
   strcpy (p->BBSMsg->ADS_String , str);
   return SendMessage( p );
}


int  UnmarkAllFiles(struct ADS_Data *p)
{
   p->BBSMsg->ADS_Command     =   ADSC_UNMARK_ALL_DOSFILES;
   return SendMessage( p );
}


/* receive a file including path */

/* res == 0  ..  No file                    */
/* res == 1  ..  Error (partially received  */
/* res == 2  ..  File received !            */


int  Upload(struct ADS_Data *p, char *str)   /* 79 letters max */
{
   int res;

   p->BBSMsg->ADS_Command     =   ADSC_UPLOAD;
   res = SendMessage( p );

   strcpy (str, p->BBSMsg->ADS_String);
   return res;
}




int  PageSysop(struct ADS_Data *p)
{
   p->BBSMsg->ADS_Command     =   ADSC_PAGE_SYSOP;
   return SendMessage( p );
}



/* If you want to play a sample multiple times , increase SubCommand */


int  PlaySample(struct ADS_Data *p, char *sample)
{
   UBYTE  *path;

   p->BBSMsg->ADS_Command     =   ADSC_PLAY_SAMPLE;
   p->BBSMsg->ADS_SubCommand  =   1;

   /* Simple way to get a path to a sample (provided for example)     */
   /* sprintf (p->BBSMsg->ADS_String,  "trion:samples/%s", sample);   */

   /* Get pointer to Samples directory (see TrionRM.cfg)  */
   path = p->SharedData->Paths.Samples;

   /* Make a path .. but use the TrionRM.cfg value for path */
   /* sprintf (p->BBSMsg->ADS_String,  "%s%s", path, sample); */

   /* Same as above but uses less memory (if you don't use sprintf() */
   /* in your sources) */
   strcpy( p->BBSMsg->ADS_String , path   );
   strcat( p->BBSMsg->ADS_String , sample );

   return SendMessage( p );
}


int  PlaySampleNum  (struct ADS_Data *p, char *sample, int num)
{
   UBYTE  *path;

   p->BBSMsg->ADS_Command     =   ADSC_PLAY_SAMPLE;
   p->BBSMsg->ADS_SubCommand  =   num;

   /* Simple way to get a path to sample (provided for example)     */
   /* sprintf (p->BBSMsg->ADS_String,  "trion:samples/%s", sample); */

   /* Get pointer to Samples directory (see TrionRM.cfg)  */
   path = p->SharedData->Paths.Samples;

   /* Make a path .. but use the TrionRM.cfg value for path */
   /* sprintf (p->BBSMsg->ADS_String,  "%s%s", path, sample); */

   /* Same as above but uses less memory (if you don't use sprintf() */
   /* in your sources */
   strcpy( p->BBSMsg->ADS_String , path   );
   strcat( p->BBSMsg->ADS_String , sample );

   return SendMessage( p );
}


int   CheckSysopAvailable(struct ADS_Data *p)
{
   int res = -1;

   p->BBSMsg->ADS_Command     =   ADSC_CHECK_SYSOP_AVAILABLE;
   res = SendMessage( p );

   if (res >= 0){
      return (p->BBSMsg->ADS_Data_word);
   }else{
      return -1;
   }
}


/* mode == 0   ->   Normal mode   8 bit ASCII  */
/* mode == 1   ->   Chat mode     7 bit ASCII , MSB is the source of character */

int  SplitChatMode(struct ADS_Data *p, int mode)
{
   p->IDMode                  =   mode & 1;
   p->BBSMsg->ADS_Command     =   ADSC_SPLIT_CHAT_MODE;
   p->BBSMsg->ADS_SubCommand  =   mode & 1;
   return SendMessage( p );
}



int  GetUserStringField(struct ADS_Data *p, int field, char *string)
{
   int res = -1;

   if (field >= 64 && field <= 127){
      p->BBSMsg->ADS_Command     =   ADSC_GET_USER_DATA;
      p->BBSMsg->ADS_SubCommand  =   field;
      res =  SendMessage( p );

      if (res >= 0){
         strcpy(string, p->BBSMsg->ADS_String);
      }
   }
   return res;
}

int  GetUserNumericalField(struct ADS_Data *p, int field, ULONG *var)
{
   int res = -1;

   if (field > 128){
      p->BBSMsg->ADS_Command     =   ADSC_GET_USER_DATA;
      p->BBSMsg->ADS_SubCommand  =   field;
      res =  SendMessage( p );

      if (res >= 0){
         *var = p->BBSMsg->ADS_Data_long;
      }
   }
   return res;
}

int  ChangeUserStringField(struct ADS_Data *p, int field, char *string)
{
   int res = -1;

   if (field >= 64 && field <= 127){
      p->BBSMsg->ADS_Command     =   ADSC_CHANGE_USER_DATA;
      p->BBSMsg->ADS_SubCommand  =   field;
      strcpy(p->BBSMsg->ADS_String, string);
      res =  SendMessage( p );
   }
   return res;
}


int  GetBaudRate(struct ADS_Data *p, UWORD *local, ULONG *baudrate)
{
   int res = 0;

   p->BBSMsg->ADS_Command     =   ADSC_GET_SYSTEM_DATA;
   p->BBSMsg->ADS_SubCommand  =   1;
   res =  SendMessage( p );

   *local    = p->BBSMsg->ADS_Data_word;
   *baudrate = p->BBSMsg->ADS_Data_long;

   return res;
}


int  GetSharedData(struct ADS_Data *p)
{
   int res = 0;

   p->BBSMsg->ADS_Command     =   ADSC_GET_SYSTEM_DATA;
   p->BBSMsg->ADS_SubCommand  =   2;
   res =  SendMessage( p );

   p->SharedData  = p->BBSMsg->ADS_P;

   return res;
}





int  ChangeUserNumericalField(struct ADS_Data *p, int field, ULONG var)
{
   int res = -1;

   if (field > 128){
      p->BBSMsg->ADS_Command     =   ADSC_CHANGE_USER_DATA;
      p->BBSMsg->ADS_SubCommand  =   field;
      p->BBSMsg->ADS_Data_long   =   var;
      res =  SendMessage( p );

   }
   return res;
}


int  FlushBuffer(struct ADS_Data *p)
{
   p->BBSMsg->ADS_Command     =   ADSC_FLUSH_BUFFER;
   return SendMessage( p );
}


int  PushIntoBuffer(struct ADS_Data *p, char *str, int source)   /* 79 letters max */
{
   p->BBSMsg->ADS_Command     =   ADSC_PUSHINTOBUFFER;
   p->BBSMsg->ADS_SubCommand  =   source;
   strcpy (p->BBSMsg->ADS_String , str);
   return SendMessage( p );
}



int  SendMessage(struct ADS_Data *p)  /* int node, struct ADoorMessage *Msg */
{
   struct ADoorMessage  *reply;
   int    result = 0;
   char   portname[20];

   sprintf(portname , "tronadoor%d", p->node);

   if (SafePutToPort((struct Message *) p->BBSMsg,  portname )){
      WaitPort(p->BBSreplyport);
      if (reply = (struct ADoorMessage *) GetMsg(p->BBSreplyport)){
         result = p->BBSMsg->ADS_Result;
      }else{
         /* print("Error.\n"); */
         result = -3;
      }
   }else{
      if (!p->quiet) printf("Can't find BBS node %d.\n", p->node);
      result = -2;
   }

   return result;
}



int  SafePutToPort(struct Message *message, STRPTR portname)
{
   struct MsgPort *port;

   Forbid();
   port = FindPort (portname);
   if (port) PutMsg(port, message);
   Permit();
   return(port ? TRUE : FALSE);    /* FALSE if the port was not found */
}





/*===(  ANSI code  )=================================*/


#define  apri2buf  printf          /* Buffer code removed    */
                                   /* color buffer inplace   */
                                   /* cursor buffer removed  */


void cursxy(struct DoorIO *p, int x,int y)
{
   if (y == 1 && x == 1){
      printf("\x1b[H"); 
   }else{
      printf("\x1b[%d;%dH", p->my = y, p->mx = x); 
   }
}



/*
void cursxy(struct DoorIO *p, int x,int y)
{
   if (y == 1 && x == 1){
      apri2buf("\x1b[H"); 
      my = y; mx = x;
   }else{
      if (y != my || x != mx){
         if (y == my){
            if (x > mx)
               forward(x - mx,0);
            else
               backward(mx - x);
         }else{
            if (x == mx){
               if (y > my)
                  down(y - my);
               else
                  up(my - y);
            }else{
               if (x != mx || y != my){
                  apri2buf("\x1b[%d;%dH", my=y, mx=x); 
               }
            }
         }
      }
   }
}
*/


/*
void forward(int n,int spaties)   /* checken: gaat niet voorbij EOL */
{                                 /* als geen spaties: n < x-mx     */
   register int tel;

/* if (n < 5 || spaties != 0)  */

   if (n < 0 || spaties != 0)      /* problems bij achtergrond kleur */
      for (tel=0; tel<n; tel++)
         apri2buf(" ");
   else
      if (n == 1) apri2buf("\x1b[C");
      else        apri2buf("\x1b[%dC",n);

   mx += n;
   if (mx > xmax)   mx = xmax;
}
*/

/*
void backward(int n)
{
   if (n == 1) apri2buf("\x1b[D");
   else        apri2buf("\x1b[%dD",n);
   mx -= n;
   if (mx < 1)   mx = 1;
}
*/

/*
void down(int n)
{
   if (n == 1) apri2buf("\x1b[B");
   else        apri2buf("\x1b[%dB",n);
   my += n;
   if (my > ymax)   my = ymax;
}
*/

/*
void up(int n)
{
   if (n == 1) apri2buf("\x1b[A");
   else        apri2buf("\x1b[%dA",n);
   my -= n;
   if (my < 1)   my = 1;
}
*/



void col(struct DoorIO *p, int fc,int bc)
{
   int high;       /* is de voorgrond color high intensity ?  */
   int h = 0, f = 0, b = 0;

   if (fc > 7){           /* gewone of High intensity colors (Bold) */
      high = 1;
      fc = fc + 22;       /* waarde van 30-37 */
   }else{
      high = 0;
      fc = fc + 30;       /* waarde van 30-37 */
   }

   if (bc > 7) bc += 32;  /* waarde van 40-47 */
          else bc += 40;

   if (p->mhigh != high)   h = 1;
   if (p->mfc   != fc  )   f = 1;
   if (p->mbc   != bc  )   b = 1;

   /* stel mfc=fc en mbc=bc en mhigh!=high dan kleurverandering !! */

       /* als 2 kleuren veranderen of de 0m code moet worden   */
       /* gegeven (intensiteit verandering met low intensity)  */
       /* dan beide kleuren veranderen                         */

   if ( f || b || h ){         /* iets veranderen  ?*/
      apri2buf("\x1b[");
      if (( f && b ) || ( b && h ) || ( high == 0 && h )){  /* beide kleuren  ?*/
         if ( h ){
            apri2buf("%d;", p->mhigh = high); 
         }
         apri2buf("%d;%dm", p->mfc = fc, p->mbc = bc);
      }else{                           /* anders 1 kleur veranderen */
         if ( f || h ){                /* voorgrond kleur   */
            if ( h ){                  /* indien voorgrond en intensiteit    */
               apri2buf("1;");         /* verandering geef de 1m (low int.)  */
               p->mhigh = 1;
            }
            apri2buf("%dm", p->mfc = fc);
         }else{
            apri2buf("%dm", p->mbc = bc);        /* achtergrond kleur  */
         }
      }
   }
}

void fcol(struct DoorIO *p, int fc)
{
   int high;       /* is de voorgrond color high intensity ?  */
   /* int oc; */   /* De output color (30-37) */

   if (fc > 7){         /* gewone of High intensity colors (Bold) */
      high = 1;
      fc = fc + 22;     /* waarde van 30-37 */
   }else{
      high = 0;
      fc = fc + 30;     /* waarde van 30-37 */
   }

   /* stel mfc=fc en mbc=bc en mhigh!=high dan kleurverandering !! */

   if (p->mfc != fc || p->mhigh != high){     /* iets veranderen ?*/
      apri2buf("\x1b[");
      if (high == 0){
         if (p->mhigh != high){      /* als int. verandering en lage int. */
            apri2buf("0;%d;",p->mbc);  /* dan beide kleuren uitgeven  */
            p->mhigh = 0;            /* maar eerst intensiteit code */
         }
         apri2buf("%dm", p->mfc = fc);
      }else{
         if (p->mhigh != high){      /* anders alleen voorgrond kleur    */
            apri2buf("1;");         /* en indien nodig bold code geven  */
            p->mhigh = 1;
         }
         apri2buf("%dm", p->mfc = fc);
      }
   }
}

void bcol(struct DoorIO *p, int bc)
{
   if (bc > 7)           /*  if high. int color         */
      bc = bc + 32;      /*  nie goed met 8 verlagen    */
   else
      bc = bc + 40;      /*  background color is 40-47  */
   
   if (p->mbc != bc){       /* moet Bkleur verandert ?     */
      apri2buf("\x1b[%dm", p->mbc = bc);
   }
}

void atr(struct DoorIO *p, int atr)       /* attribute veranderen, geen invloed op kleur */
{
   apri2buf("\x1b[%d",atr); 
   if (atr == 0){              /* als geen atribuut dan verandert kleur */
      if (p->mhigh == 1)          /* esc[0;1;[mfc];[mbc]m  */
         apri2buf(";1");
      apri2buf(";%d;%d" ,p->mfc ,p->mbc );
   }
   apri2buf("m");
}

void norm(struct DoorIO *p)        /* alle atributes uit, kleuren normaal */
{                      /* kleuren: grijs op zwart              */
   apri2buf("\x1b[0m");
   p->mfc   = 37;
   p->mbc   = 40;
   p->mhigh =  0;
}

void cls(struct DoorIO *p)     /* cls en cursor home (home mandatory!!) */
{
   apri2buf("\x1b[H\x1b[J");         /* official ansi , clear screen en */
   p->my = p->mx = 1;                 /* cursor op home positie */
}




/*===( Simple ANSI code )=================================*/

/*
void col(int fc,int bc)
{
   if (fc > 7){
      printf("\x1b[1;%d;%dm", fc + 22 , bc + 40);
   }else{
      printf("\x1b[0;%d;%dm", fc + 30 , bc + 40);
   }
}

void cursxy(int x,int y)
{
   if (y == 1 && x == 1){
      printf("\x1b[H"); 
   }else{
      printf("\x1b[%d;%dH", y, x); 
   }
}

void fcol(int fc)
{
   if (fc > 7){
      printf("\x1b[1;%dm", fc + 22);
   }else{
      printf("\x1b[0;%dm", fc + 30);
   }
}

void bcol(int bc)
{
   printf("\x1b[%dm", bc + 40);
}

void atr(int atr)
{
   printf("\x1b[%dm", atr);
}

void norm(void)
{
   printf("\x1b[0m");
}

void cls(void)
{
   printf("\x1b[H\x1b[J"); 
}

*/
