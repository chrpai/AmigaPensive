
#ifdef __SASC
#include <proto/exec.h>
#endif
#ifdef DICE
#include <clib/exec_protos.h>
#endif

#include <proto/exec.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/io.h>
#include <exec/devices.h>
#include <devices/timer.h>
#include <clib/alib_protos.h>



#if 0
Prototype BOOL OpenTimer4MsgId(void);
Prototype void GetTimer4MsgId(struct timeval *);
Prototype void CloseTimer4MsgId(void);
#endif


static struct MsgPort *msgid_tport = NULL;
static struct timerequest *msgid_tio = NULL;
static BOOL msgid_topen = FALSE;



BOOL OpenTimer4MsgId(void)
{
	if (!msgid_topen) {
		if (msgid_tport = CreatePort(NULL, 0L)) {
			if (msgid_tio = (struct timerequest *) CreateExtIO(msgid_tport, sizeof(struct timerequest))) {
				if (!OpenDevice("timer.device", UNIT_MICROHZ, (struct IORequest *) msgid_tio, 0L)) {
					msgid_topen = TRUE;
					return (TRUE);
				}
				else {
					DeleteExtIO((struct IORequest *) msgid_tio);
					msgid_tio = NULL;
					DeletePort(msgid_tport);
					msgid_tport = NULL;
				}
			}
			else {
				DeletePort(msgid_tport);
				msgid_tport = NULL;
			}
		}
	}
	return (FALSE);
}



void GetTimer4MsgId(struct timeval *tv)
{
	/* if (!msgid_topen) return; */

	msgid_tio->tr_node.io_Command = TR_GETSYSTIME;
	DoIO((struct IORequest *) msgid_tio);

	*tv = msgid_tio->tr_time;
}



void CloseTimer4MsgId(void)
{
	if (msgid_topen) {
		CloseDevice((struct IORequest *) msgid_tio);
		msgid_topen = FALSE;
		DeleteExtIO((struct IORequest *) msgid_tio);
		msgid_tio = NULL;
		DeletePort(msgid_tport);
		msgid_tport = NULL;
	}
}
