#include "Aridoorstuff.h"



void dmain(int,char *[]);


void dmain(int argc,char *argv[])
{
  int a;
  char str[80];


  putdoorchar(7);
  a = showfile("BBS:Text/Esc.text");
  sprintf(str,"Result = %d\n",a);
  sendmessage(str,0);
  Delay(50);

  putdoorchar(7);
  a = chkfile("BBS:Text/Esc.text");
  sprintf(str,"Result = %d\n",a);
  sendmessage(str,0);
  Delay(50);

  putdoorchar(7);
  a = editfile("BBS:Text/Esc.text",99);
  sprintf(str,"Result = %d\n",a);
  sendmessage(str,0);
  Delay(50);

  end();
}                        
