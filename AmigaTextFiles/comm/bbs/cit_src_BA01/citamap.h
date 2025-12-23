/*
 *				CitaMap.h
 *
 * Header file for RoutMail.C, the C86Net map eater program.
 */

/*
 *				History
 *
 * 89Jan?? HAW  Initial creation finished, V1.
 */

#define R_TITLE "Routmail V2.2\n%s\n", COPYRIGHT

typedef struct peon PEONS;
typedef struct o_hub LINKS;
typedef struct hub HUB;

struct peon {
    label Name, Id;
    int   baud;
    PEONS *next;
};

struct o_hub {
    HUB *Hub;
    struct o_hub *next;
} ;

struct hub {
    label	Name, Id;
    int		baud;
    char	Checked;
    struct hub	*next;
    PEONS	*list;
    LINKS	*Hubs;
};

typedef struct {
    char *Area;
    SListBase Systems;
} Hop;

typedef struct {
    char *Id;
    char *RouteId;
    int  hopcount;
} HopNode;

extern char *READ_ANY, *WRITE_ANY;
void *CheckArea();

#ifdef ANSI_PROTOTYPING

HUB *FindHub(HUB *base, label id);
HUB *MakeMap(char *fn);

char ChangeRouting(int target);
char DirectRoute(NetBuffer *n);
char *GetLine(char *line, int s, FILE *fd);
char ParseCh(label OldId, label NewId, label Name, int *baud, char *line);
char Parse(label Id, label Name, int *baud, char *line);
char ProcChanges(FILE *fd);
char ProcessDead(FILE *fd);
char ReadArguments(int argc, char **argv, char *filename);
char IsLowerHopCount(char *TargetId, int HopCount);
char ReadHubs(HUB *base, FILE *fd);
char ReDoRouted(int target);
char ReRoute(int target);
char SetUpHub(HUB *Hr);

PEONS *FindPeon(PEONS *base, label id);

void *HopSearch();
void SetHopCount(char *Id, int count, char *RouteId);
void AddNewNode(label Name, label Id, int baud);
void BackToDirect(int i, NetBuffer *n);
void AreaCode(char *Id, char *Target);
void AddNewHop(char *Id);
void CheckAccessible(HUB *HomeHub, HUB *CurHub, PEONS *Kr, int hopcount);
void CoverRouteHole(int system, int start);
void DoReRoute(int target);
void HubHub(HUB *Us, HUB *Hub);
void InitSystem(void);
void KillMapNodes();
void NormStr(label Id);
void PrintRouteIds(void);
void ReDoAuxFiles(label newid);
void KillAuxFiles(void);
void HandleDomains(void);
void RouteLocalMail(int target);
void SetHub(HUB *CurHub, HUB *HomeHub, int HopCount);
void SetPeon(PEONS *Peon, HUB *HomeHub, int HopCount);
void SetNode(label TargetName, label TargetId, int TargetBaud, label RouteId,
						int HopCount, char CheckHops);
void ShowArguments(void);
void DoEdit(void);
void WeAreHub(HUB *Us, HUB *Systems);
void WeArePeon(HUB *Systems);

int  FindRouteName(int slot, SYS_FILE newfn);
int  GetNewSlot(NetBuffer *n);
int  IdRouteFile(SYS_FILE fn, label Id, label Name);
int  putRouteChar(int c);

PEONS *FindGlobalPeon(HUB *base, char *id);

HopNode *GetHopNode(char *Id);

#endif
