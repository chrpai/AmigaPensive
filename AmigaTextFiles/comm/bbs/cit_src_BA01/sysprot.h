void dirExists(char *theDir);
void doAreaCommon(char *var, char *line, SYS_AREA *area, int which);
void anyArea(char *var, char *line, char *target);
void MSDOSparse(char *theDir, char *drive);
void OpenDoorFile(void);
void StoreDoor(char *line, FILE *fd);
AN_UNSIGNED interpret(int instruction);
void writeArea(char rightNow, aRoom *roomData, char *buf);
char *GetDoorData(char *line, char *field, int size);
char *NextWhite(char *line);
char *NextNonWhite(char *line);
void WriteDoors(void);
int  ScreenColor(char *line);
int  findcolor(char *str);
char *NextWhite(char *line);
void DoDomainDirectory(int i, char kill);
char *NextNonWhite(char *line);
int waitPutch(int c);
char MakeCmdLine(char *target, char *source, char *miscdata, int len);
void InitExternEditors(void);
void InitDoors(void);
int SearchFileComments(char *FileName);
int BaudCode(int bps);
void InitProtocols(void);
char realSetSpace(char *dir);
void MSDOSparse(char *theDir, char *drive);
void doSendWork(char *filename, void (*fn)(DirEntry *fn));
long getModemId(void);
char ShowDoors(FILE *fd);
void initDirList(void);
int nodie(void);
int Control_C(void);
void SysWork(void (*form)(), char *cmdLine);
int specCmpU(char *f1, char *f2);
void setup_nocccb(void);
void VideoInit(void);
int fileType(char *theDir);
int goodArea(char *prompt, char *dir);
void AddName(DirEntry *fn);
/* IBM support function prototypes */
void setInterrupts(void);
void killIBMint(void);
/* IBM video function prototypes (see CITVID.H) */
#ifndef VMODULE
char vputch(unsigned char c);
#endif
void video(char *tagline);
void statusline(char *tagline);
/* Z-100 support function prototypes */
int mGetch(void);
int mHasch(void);
int mPutch(int c);
int mHasout(void);
int mInit(int port, int baud, int parity, int stopbits, int wordlen, int xon);
void Zreset_video(void);
void Zsline(char *data);
void Zvideo(char *tagline);
void Zreset_video(void);
void mClose(void);
/* Miscellaneous prototypes */
void diskSpaceLeft(char *drive, long *sectors, long *bytes);
char  check_CR(void);
/* Timer support function prototypes */
long timeSince(struct timePacket *Slast);
long milliTimeSince(struct timePacket *Slast);
void setTimer(struct timePacket *Slast);
void SpaceBug(int x);
void CitadelBaudRate(long,char *);
/* end of this file */
