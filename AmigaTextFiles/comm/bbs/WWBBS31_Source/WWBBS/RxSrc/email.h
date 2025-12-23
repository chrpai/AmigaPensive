BOOL EmailNext(BYTE *);
BOOL EmailPrevious(BYTE *);
BOOL EmailJumpTo(BYTE *);
void EmailList(BYTE *);
void EmailWrite(BYTE *);
void EmailReplyTo(BYTE *);
void EmailDelete(BYTE *);
void EmailFeedback(BYTE *);
void EmailShowNew(BYTE *);

BOOL EmailIsAddressee(BYTE *,APTR,ULONG);
void EmailDisplay(BYTE *,APTR);
void EmailSend(BYTE *,BYTE *,BYTE *,BYTE *);
void EmailUpdateVars(BYTE *);
