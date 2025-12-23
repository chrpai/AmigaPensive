void SaveMsg(struct FMsg *, struct AreaCfg *, ULONG, UBYTE *, UBYTE *);
struct FMsg *LoadMsg(struct AreaCfg *, ULONG);
void UpdateMsg(struct FMsg *, struct AreaCfg *);
struct MemMsg *F2MMsg(struct FMsg *fm);
void main(int, char **);
void Close_All(void);
void *SendBBSMsg(UWORD, void *);
int nbrk(void);
void RexxCmd(UBYTE *);
void *DoRxFnc(UBYTE **);
__stkargs void *RexxFnc(UBYTE *, ...);
void parsebbscfg(BPTR);
void parseusercfg(BPTR);
void MakeAreaList(void);
void PackMessages(void);

