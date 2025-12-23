/* wwbbs.c */
BOOL __saveds StartProcess(BYTE *,APTR,ULONG,BYTE *);
void __saveds KillProcess(BYTE *,BYTE *);

/* node.c */
LONG __saveds NodeProc(void);

/* node_frontend.c */
LONG __saveds NodeFrontEndProc(void);

/* local.c */
LONG __saveds LocalLogonProc(void);

/* console.c */
LONG __saveds ConsoleProc(void);

/* serial.c */
LONG __saveds SerialProc(void);

/* timer.c */
LONG __saveds TimerProc(void);


/***/

struct ConfirmationMessage {
	struct Message cm_Message;
	BYTE *cm_ID;
	BOOL cm_Return;
};
