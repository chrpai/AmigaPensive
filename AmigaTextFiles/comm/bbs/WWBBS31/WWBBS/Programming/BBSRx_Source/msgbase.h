struct MessageGlobalNode {
	struct Node mgn_Node;
	BYTE mgn_Name[21];
	BYTE mgn_Read[256];
};

void InitMessageGlobal(BYTE *);
void EditMessageSearchPatterns(BYTE *);
struct List *OpenMessageGlobal(BYTE *);
void CloseMessageGlobal(struct List *);
BOOL AddMessageGlobal(struct List *,BYTE *,BYTE *);
BOOL DeleteMessageGlobal(struct List *,BYTE *);
BYTE *GetMessageGlobalRead(struct List *,BYTE *);
BOOL SetMessageGlobalRead(struct List *,BYTE *,BYTE *);
int SelectMessageArea(BYTE *,UBYTE,BYTE *);
void MessageUpdateVars(BYTE *);

void JoinMessageArea(BYTE *);
void ChangeMessageArea(BYTE *);
void ParentMessageArea(BYTE *);
void NextMessageArea(BYTE *);
void PreviousMessageArea(BYTE *);
void DisplayMessageArea(BYTE *,APTR);
void SetMessageDoorVariables(BYTE *);

BOOL NextUnreadMessage(BYTE *);
ULONG GetNextThread(BYTE *);
ULONG GetPreviousThread(BYTE *);
ULONG JumpToThread(BYTE *);
void ListThreads(BYTE *);
ULONG GetNextResponse(BYTE *);
ULONG GetPreviousResponse(BYTE *);
ULONG JumpToResponse(BYTE *);
void ListResponses(BYTE *);
BOOL SearchNextMessage(BYTE *);
BOOL MessageDisplay(BYTE *);

void MessageWrite(BYTE *);
void MessageReplyTo(BYTE *,BOOL);
void MessageSend(BYTE *,BYTE *,BYTE *,BYTE *,ULONG);
BOOL MessageReadOnlyArea(void);
