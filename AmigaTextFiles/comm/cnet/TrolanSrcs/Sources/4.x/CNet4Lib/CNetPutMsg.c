#include "cnet4lib.h"

void CNetPutMsg( char *port, UBYTE c )
{
   struct CMessage  mess;
   struct MsgPort  *msgport,
                   *rport;

   Forbid();
   msgport = FindPort(port);
   Permit();

   if(msgport)
   {
      if(rport = CreatePort(0, 0))
      {
         mess.cn_Message.mn_ReplyPort    = rport;
         mess.cn_Message.mn_Length       = sizeof(struct CMessage);
         mess.cn_Message.mn_Node.ln_Name = "task_msg";
         mess.command = c;

         PutMsg(msgport, (struct Message *)&mess);

         WaitPort  (rport);
         GetMsg    (rport);
         DeletePort(rport);
      }
   }
}
