/* Prototypes for functions in configs.c */

extern int GetCfgLine(FILE *fptr,char *str);
extern int ReadCfg(char *cfgname, struct FL_Config *cfg);

extern struct Node *AddNewNode(struct List *list, ULONG size);
extern VOID FreeAllNodes(struct List *list, ULONG size);

