/*******************************************************************************/
/* maxsdoor.h WRITTEN BY ARI TSIRONIS                                          */
/*******************************************************************************/




/*******************************************************************************/
/* INCLUDES                                                                    */
/*******************************************************************************/

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <exec/exec.h>


/* THIS INCLUDES PROTOS FOR ALL  exec.h FUNCTIONS                              */

#include <clib/exec_protos.h>



/*******************************************************************************/
/* DEFINES                                                                     */
/*******************************************************************************/

#define OKAY 0
#define CARRIER_DROPPED -1000

#define DROPPED 20


/* DEFINES MAXIMUM STRING SIZE TO BE SENT TO SENDMESSAGE */

#define MAX_STR 80


/* DEFINES SCREEN WIDTH, 1 LESS BECAUSE OF CR */

#define COLS 79




/*******************************************************************************/
/* PROTOTYPES                                                                  */
/*******************************************************************************/


/* THIS LETS US WORK WITH 1.3 OR 2.X */

struct MsgPort *CreatePort(STRPTR,long);
void DeletePort(struct MsgPort *);


/* OUR FUNCTION PROTOS                                                         */
/* ALL FUNCTIONS RETURN CARRIER_DROPPED IF CARRIER LOST, OKAY IF NOT           */

int sendmessage(char *,int);
int localmessage(char *,int);
int putmodemchar(char);
int putscreenchar(char);
int putdoorchar(char);
int prompt(char *,char *,int);
int sprompt(char *,char *,int);
int hotkey(char *,char *);
int twituser(void);
int showfile(char *);
int chkfile(char *);
int editfile(char *,int);
int getsnum(int);
int getsvar(int,char *);
int newaccess(int);
void end(void);
int addtime(int);
int domenu(int,int,char *);
int changeuserint(int,long);
int getkey(char *);
void dmain(int, char **);



/*******************************************************************************/
/* STRUCTURES                                                                  */
/*******************************************************************************/

struct DoorMsg 
{
	struct Message Door_Msg;
	short command;
	short data;
	char string[MAX_STR];
	short carrier;
};

struct DoorMsg doormsg;
struct MsgPort *MyPort,*replyport;
struct Task *mytask; 



/*******************************************************************************/
/* COMMAND 1  SENDMESSAGE                                                      */
/*******************************************************************************/

int sendmessage(char *mstring,int nl)
{
  int i,len;
  
  
  /* CHECK TO SEE IF STRING IS LONGER THAN 80 CHARS, IF SO THEN SPLIT IT UP    */
  /* INTO 80 CHAR STRINGS, WHICH ARE OUTPUT SEPERATELY.                        */
  
  i=0;
  len=strlen(mstring);
  
  if(len>COLS)
    for(i=0;i<len/COLS;i++)
      {  
        doormsg.data = 0;
	      doormsg.command = 1;
	      strncpy(doormsg.string,&mstring[i*COLS],COLS);
        doormsg.string[COLS]=0;
	
        PutMsg(MyPort,(struct Message *)&doormsg);
	      WaitPort(replyport);
	      GetMsg(replyport);
      }

  doormsg.data = nl;
	doormsg.command = 1;
	strncpy(doormsg.string,&mstring[i*COLS],len-i*COLS);
  doormsg.string[len-i*COLS]=0;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 2  LOCALMESSAGE                                                     */
/*******************************************************************************/

int localmessage(char *mstring,int nl)
{
  int i,len;
  
  
  /* CHECK TO SEE IF STRING IS LONGER THAN 80 CHARS, IF SO THEN SPLIT IT UP    */
  /* INTO 80 CHAR STRINGS, WHICH ARE OUTPUT SEPERATELY.                        */
  
  i=0;
  len=strlen(mstring);
  
  if(len>COLS)
    for(i=0;i<len/COLS;i++)
      {  
        doormsg.data = 0;
        doormsg.command = 2;
	      strncpy(doormsg.string,&mstring[i*COLS],COLS);
        doormsg.string[COLS]=0;
	
        PutMsg(MyPort,(struct Message *)&doormsg);
	      WaitPort(replyport);
	      GetMsg(replyport);
      }
  
  doormsg.data = nl;
	doormsg.command = 2;
	strncpy(doormsg.string,&mstring[i*COLS],len-i*COLS);
	doormsg.string[len-i*COLS]=0;
  
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 3  PUTMODEMCHAR                                                     */
/*******************************************************************************/

int putmodemchar(char outd)
{
  doormsg.data = (short)outd;
	doormsg.command = 3;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 4  PUTSCREENCHAR                                                    */
/*******************************************************************************/

int putscreenchar(char outd)
{
  doormsg.data = (short)outd;
	doormsg.command = 4;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 5  PUTDOORCHAR                                                      */
/*******************************************************************************/

int putdoorchar(char outd)
{
  doormsg.data = (short)outd;
	doormsg.command = 5;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 6  PROMPT                                                           */
/*******************************************************************************/

int prompt(char *mstring,char *ostring,int len)
{
	strcpy(doormsg.string,mstring);
	doormsg.data=len;
	doormsg.command = 6;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);
	
  strcpy(ostring,doormsg.string);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 7  SPROMPT                                                          */
/*******************************************************************************/

int sprompt(char *mstring,char *ostring,int len)
{
	strcpy(doormsg.string,mstring);
	doormsg.data=len;
	doormsg.command = 7;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);
	
  strcpy(ostring,doormsg.string);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 8  HOTKEY                                                           */
/*******************************************************************************/

int hotkey(char *mstring,char *ostring)
{
	strcpy(doormsg.string,mstring);
	doormsg.command = 8;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);
	
  strcpy(ostring,doormsg.string);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 9  TWITUSER                                                         */
/*******************************************************************************/

int twituser(void)
{
	doormsg.command = 9;

	PutMsg(MyPort,(struct Message *)&doormsg);
  WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 10  SHOWFILE                                                        */
/*******************************************************************************/

int showfile(char *mstring)
{
	strcpy(doormsg.string,mstring);
	doormsg.command = 10;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 11  CHKFILE                                                         */
/*******************************************************************************/

int chkfile(char *filename)
{
	strcpy(doormsg.string,filename);
	doormsg.command = 11;
  doormsg.data = 1;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);
  
  return(doormsg.data);
}



/*******************************************************************************/
/* COMMAND 12  EDITFILE                                                        */
/*******************************************************************************/

int editfile(char *mstring,int maxlines)
{
	strcpy(doormsg.string,mstring);
  doormsg.data=maxlines;
	doormsg.command = 12;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 13  GETSNUM                                                         */
/*******************************************************************************/

int getsnum(int typ)
{
  doormsg.data=typ;
	doormsg.command = 13;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(doormsg.data);
}



/*******************************************************************************/
/* COMMAND 14  GETSVAR                                                         */
/*******************************************************************************/

int getsvar(int typ,char *mstring)
{
	doormsg.data = typ;
	doormsg.command = 14;

	PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  strcpy(mstring,doormsg.string);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 15  NEWACCESS                                                       */
/*******************************************************************************/

int newaccess(int now)
{
  doormsg.data = now;
	doormsg.command = 15;

	PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 20  END                                                             */
/*******************************************************************************/

void end(void)
{
	doormsg.command = 20;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);
	
	DeletePort(replyport);
	
  exit(0);
}



/*******************************************************************************/
/* COMMAND 21  ADDTIME                                                         */
/*******************************************************************************/

int addtime(int howmuch)
{
  doormsg.data = howmuch;
	doormsg.command = 21;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* MAXS SPECIFIC COMMANDS                                                      */
/*******************************************************************************/


/*******************************************************************************/
/* COMMAND MENU+100  ADDTIME                                                   */
/*******************************************************************************/

int domenu(int menu,int extra,char *filename)
{
	strcpy(doormsg.string,filename);
	doormsg.command = menu+100;
	doormsg.data	 = extra;
	
  PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);
 
  return(OKAY);
}




/*******************************************************************************/
/* COMMAND 200  CHANGE USER DATA                                               */
/*******************************************************************************/

int changeuserint(int number,long val)
{
	long *ptr;


	ptr = (long *)doormsg.string;	
	*ptr = val;
	
	doormsg.command = 200;
	doormsg.data    = number;
	PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* COMMAND 201  GETKEY                                                         */
/*******************************************************************************/

int getkey(char *key)
{
	doormsg.command = 201;
	PutMsg(MyPort,(struct Message *)&doormsg);
	WaitPort(replyport);
	GetMsg(replyport);

  strcpy(key,doormsg.string);

  if(doormsg.command==DROPPED)
    return(CARRIER_DROPPED);

  return(OKAY);
}



/*******************************************************************************/
/* MAIN                                                                        */
/*******************************************************************************/

void main(int argc,char *argv[])
{
	char buffer[81],line_number;
	char cportname[12];	/* ReplyPort and Control Port names */


	/* PULL OUT THE LINE NUMBER FROM THE ARGV FIELD TO FIND WHERE WE ARE */
	
	line_number=argv[argc-1][0];

	
	mytask=(struct Task *)FindTask(0L);
	mytask->tc_Node.ln_Name = "DoorExample";

	
	/* CREATE THE REPLY PORT, WITH THE NAME BASED ON THE NUMBER... */
	
	sprintf(buffer,"DoorReply%c",line_number);
	replyport=(struct MsgPort *)CreatePort(buffer,0L);

	
	doormsg.Door_Msg.mn_Node.ln_Type = NT_MESSAGE;
	doormsg.Door_Msg.mn_ReplyPort = replyport;
	doormsg.Door_Msg.mn_Length = (UWORD)sizeof(doormsg);

	
	/* LOCATE THE CORRECT DOORCONTROL PORT BASED ON THE LINE NUMBER */
	
	sprintf(cportname,"DoorControl%c",line_number);

  
  MyPort=(struct MsgPort *)FindPort(cportname);
  if(MyPort==0L) 
	  {
	    puts("DoorControl port not located!");
      puts("This is not an AmigaDOS executable!");
      puts("It is a MAXS BBS door, that needs MAXS to be running!");
		
      DeletePort(replyport);
      
      exit(10);
	  }
	
	
  /* THIS IS WHERE YOUR DOOR STARTS! SO JUST START YOU C DOOR WITH dmain() */
  /* INSTEAD OF main(), AND FINISH IT WITH end()!                          */

  dmain(argc, argv);
  

  /* THIS IS WHERE YOUR DOOR ENDS! */

	end();
}

 
