#include <exec/types.h>
#include <exec/exec.h>
#include <stdio.h>
#include <string.h>

#include <proto/exec.h>

void main()
	{
		struct MsgPort *port;
		if(port=CreatePort("AUX Debug",0))
			{
				while(1)
					{
						WaitPort(port);
						{
							struct Message *msg;
							while(msg=GetMsg(port))
								{
									printf("%s\n",msg->mn_Node.ln_Name);
									ReplyMsg(msg);
								}
						}
				}
				DeletePort(port);
			}
	}
