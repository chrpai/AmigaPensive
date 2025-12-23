        /*
* Use these only if we're (semi) ANSI capable, with prototypes.
*/
#define ANSI_PROTOTYPING
#ifdef ANSI_PROTOTYPING

void Output_Citadel_Message(char *pcode, long arg1, long arg2, long arg3);

void CheckVI(char *where);
/* These functions are located in CTDL.C */
char doChat(char expand, char first);
char doEnter(char expand, char first);
char doForget(char expand);
char doGoto(char expand);
char doHelp(char expand);
char doKnown(char expand);
char doLogin(char expand);
char doLogout(char expand, char first);
char doMeet(char moreYet);
char doRead(char expand, char first);
char doSkip(char expand);
char doUngoto(char expand);
char OptionValidate(int mode);
char OptionCheck(char mode);
char doRegular(char x, char c);
char doSysop(void);
char getCommand(char *c, char bs);
char doAide(char moreYet, char first);
void doDirectory(char doDir, label fileName, char *phrase);
void UserAdmin(logBuffer *lBuf);
void greeting(void);
/* These functions are located in LOG.C */
char doInviteDisplay(void);
void login(char *password);
void newPW(void);
void newUser(logBuffer *lBuf);
void terminate(int discon, int save);
void slideLTab(int slot, int last);
void storeLog(void);
char GetIntMessage(void);
char CheckIntMessage(void);
int DoAllQuestion(char *posprompt, char *netprompt);
int PWSlot(char pw[NAMESIZE], char load);
int strCmpU(char s[], char t[]);
int findPerson(char *name, logBuffer *lBuf);
int GetUser(label who, logBuffer *lBuf, char Menu);
/* These functions are located in MSG.C */
void AddMail();                 /* intentionally unfinished */
void MailWork(int slot);
void LocalForwarding(label name, logBuffer *workBuf);
void NetForwarding(logBuffer *lBuf);
void AddNetMail(char *system, char CreditWho);
void SetShowLimits(char rev, int *start, int *finish, int *increment);
void aideMessage(char *name, char noteDeletedMessage);
void DelMsg(char killit, int m);
void TranslateFilename(char *realfn, char *fn);
void doFlush(FILE *whichmsg, struct mBuf *mFile);
void fakeFullCase(char *text);
void AddMail(char *DaPerson);
void flushMsgBuf(void);
void *FindIcky(char *, char *);
void *EatIcky(char *str);
void *FindIckyPeople(struct User_Id *, MessageBuffer *);
void *EatIckyPeople(char *str);
void MakeIntoRouteMail(int result, DOMAIN_FILE fn, char isdomain, char *system,
char *domain, char OriginIsMsgBase, int slot);
void mFormat(char *string);
void mPeek(void);
void msgToDisk(char *filename, char all, MSG_NUMBER id, SECTOR_ID loc,
UNS_16 size);
void noteLogMessage(logBuffer *lBuf, int logNo);
void noteMessage(logBuffer *lBuf);
void noteAMessage(theMessages *base, int slots, MSG_NUMBER id, SECTOR_ID loc);
void netMailProcess(int netPlace);
void netMailOut(char isdomain, char *system, char *domain, char MsgBase,
int slot);
void putWord(char *st);
void CheckForwarding(logBuffer *lbuf);
void prNetStyle(int i, int (*method)(int c), char GetMsg, char *GoingFor);
void NetCC(char *d);
void TellRoute(void);
void ShowReply(int i);
void undirect(void);
void dLine(char *garp);
char printMessage(int status);
char InterruptMessage(void);
char redirect(char *name);
char deleteMessage(int m);
char canRespond(void);
char dGetWord(char *dest, int lim);
char doActualWrite(FILE *whichmsg, struct mBuf *mFile, char c);
char mAbort(void);
char getRecipient(void);
char CheckForSpecial(int second, int third);
char replyMessage(MSG_NUMBER msgNo, SECTOR_ID Loc);
char HasWritePrivs(void);
char hldMessage(char IsReply);
char idiotMessage(void);
char moveMessage(char answer, int m, char *toReturn);
char findMessage(SECTOR_ID loc, MSG_NUMBER id, char ClearOthers);
char pullIt(int m);
char putMessage(logBuffer *lBuf);
int procMessage(char uploading, char IsReply);
int PersonExists(char *name);
int getWord(char *dest, char *source, int offset, int lim);
int putMsgChar(int c);
int DoRespond(void);
int makeMessage(char uploading);
int showMessages(char whichMess, char revOrder, MSG_NUMBER since,
char (*Style)(int i));
int FindNextFile(char *base);
/* These functions are located in INFO.C */
void EditInfo(void);
void ReadCitInfo(void);
void WriteOutInformation(void);
char *GetInfo(label name);
void KillInfo(char *name);
void ChangeInfoName(char *newname);
void AllInfo(void);
char doInfo(void);
/* These functions are located in MISC.C */
char ARCDir(FILE *fd, char *FileName, long *RSize, long *SSize, char *DateStr);
char ZIPDir(FILE *fd, char *FileName, long *RSize, long *SSize, char *DateStr);
char ZOODir(FILE *fd, char *FileName, long *RSize, long *SSize, char *DateStr);
char LZHDir(FILE *fd, char *FileName, long *RSize, long *SSize, char *DateStr);
char *PrintPretty(long s, char *result);
char GifDir(FILE *fd, char longexpl, char *buf);
char *Current_Time(void);
char reconfigure(void);
int  CompressType(char *name);
char configure(logBuffer *lBuf, char AllQuestions, char AllowAbort);
void DosToNormal(char *DateStr, UNS_16 DosDate);
void changeDate(void);
void civTime(int *hours, char **which);
void FileCommentUpdate(char *filename, char aideMsg);
void crashout(char *message);
void DirFree();
void *DirCheck();
void doCR(void);
void ExtraOption(char *Opts[], char *NewOpt);
void download(char whichMess, char revOrder, char whatkind, char global,
int Compression);
void doGlobal(char whichMess, char revOrder);
void getCdate(int *year, char **month, int *day, int *hours, int *minutes);
char ingestFile(char *name, MessageBuffer *msg);
void NormStr(char *st);
void SaveInterrupted(MessageBuffer *SomeMsg);
void TranFiles(int protocol, char *phrase);
void *CheckFwd();
void *EatForwarding(char *line);
void FreeForwarding(ForwardMail *data);
void UpdateForwarding(void);
void *FindSelect();
void TranSend(int protocol, void (*fn)(DirEntry *fn), char *filespec,
char *phrase, char NeedToMove);
void CompressedDir(DirEntry *fn);
void doFormatted(DirEntry *fn);
void upLoad(int WC, char*file, char NeedToMove);
void writeTutorial(FILE *fd, char noviceWarning);
char *formHeader(void);
char *formRoom(int roomNo, int showPriv, int noDiscrimination);
char *formDate(void);
char HelpIfPresent(char *filename);
char *lbyte(char *l);
char MultiBanner(char *basename);
char tutorial(char *filename, char addHelpArea);
char visible(AN_UNSIGNED c);
char CheckDLimit(long estimated);
char TranAdmin(int protocol, int NumFiles);
int DirCmp();
int AsciiHeader(long fileSize, char *filename);
int FindFunnyEntry(char *fn);
int CmdMenuList(char *Opts[], SListBase *Selects, char *HelpFile, char *List,
char moreYet, char OneMore);
int GetMenuChar(void);
int GetSecond(void);
int NextSeq(void);
int putBufChar(int c);
int putFLChar(int c);
void transmitFile(DirEntry *filename);
void SendThatDamnFile(FILE *fbuf, int (*method)(int c));
void *FindSelect();
CRC_TYPE calcrc(unsigned char *ptr, int count);
/* These functions are located in ROOMA.C */
void dumpRoom(char ShowFloor);
void fillMailRoom(void);
void retRoom(char *roomName);
void CountMsgs(int *count, int *NewCount);
void searchRooms(char *str);
void setUp(char justIn);
void systat(void);
void UngotoMaintain(int lRoom);
void SetKnown(int GenVal, int Index, int Room, logBuffer *lBuf);
void listRooms(char mode);
char initCitadel(void);
char CheckForSkippedMsgs(void);
char legalMatch(int i, label target);
char knowRoom(logBuffer *lBuf, int i);
char *DateSearch(char *fn, long *before, long *after);
void fDir(DirEntry *file);
int KnownRoom(int roomNo);
int RealGNR(char *nam, int (*func)(char *room));
int GotoNamedRoom(char *nam, char mode);
int SkippedNewRoom(int i);
int gotoRoom(char *nam, char mode);
int tableRunner(int (*func)(), char OnlyKnown);
int partialExist(label target);
int roomCheck(int (*checker)(), char *nam);
int roomExists(char *room);
void ShowVerbose(DirEntry *fn);
int wildCard(void (*fn)(), char *filename, char needToMove, char *phrase,
char AssumeDflt);
int wild2Card(SListBase *base, void (*fn)());
/* These functions are located in ROOMB.C */
char conGetYesNo(char *prompt);
char coreGetYesNo(char *prompt, int consoleOnly);
char *CleanEnd(char *msg);
char getText(int uploading);
char GetBalance(int uploading, char *buf, int size);
char getYesNo(char *prompt);
char *matchString(char *buf, char *pattern, char *bufEnd);
char renameRoom(void);
char *formatSummary(char *buffer, char NotFinal);
char
getXString(char *prompt, char *target, int targetSize, char *CR_str,
char *dft);
char getXInternal(MenuId id, char *prompt, char *target, int targetSize,
char *CR_str, char *dft);
char SepNameSystem(char *string, char *name, char *system, NetBuffer *buf);
char WhoIsModerator(char *buf);
int CCAddDelete(char *name);
int WritePrivs(char *name);
int CmnNetList(char *name, int *slot, char SBT, char *errstr);
int editText(char *buf, int lim);
int findRoom(void);
int knownHosts(char *name);
int addToList(char *name);
int makeKnown(char *user);
int ListAsShared(char *name);
int makeUnknown(char *user);
int doMakeWork(char *user, int val);
int killFromList(char *sysName);
int searchForRoom(void);
long getNumber(char *prompt, long bottom, long top);
void CCDeliver(void *dd);
int getString(char *prompt, char *buf, int lim, int Flags);
int BlindString(char *buf, int lim, int Flags,
char (*input)(void), void (*output)(char c), char Echo);
void givePrompt(void);
void indexRooms(void);
void insertParagraph(char *buf, int lim);
void makeRoom(void);
int getNormStr(char *prompt, char *s, int size, int Flags);
void noteRoom(void);
void getList(int (*fn)(char *data), char *prompt, int size, char Sysop);
void replaceString(char *buf, int lim, char Global);
void initialArchive(char *fn);
void WriteAList(SListBase *base, char *fn, void (*func)(NumToString *d));
/* These functions are located in FLOORS.C */
int CheckFloor(int i);
int UserToFloor(char *name);
int FindFloor(label name, char doPartial);
int FloorRunner(int start, int (*func)());
int NewRoom(void);
int FirstRoom(int FloorNo);
int RoomHasNew(int i);
int NSRoomHasNew(int i);
int DoFloors(void);
int Zroom(int i);
int FSroom(int i);
int FindAny(int i);
char FAide(void);
int MoveToFloor(char *name);
int DispRoom(int i);
int MaimOrKill(int i);
void DispFloorName(int FloorNo);
void FInvite(void);
void FWithdraw(void);
void FSkipped(void);
char FForget(void);
char FConfigure(void);
char FGotoSkip(int mode);
void DeleteFloors(void);
void MoveRooms(void);
void RenameFloor(void);
void RunDisplay(void);
void CreateFloor(void);
void FlModerator(void);
void putFloor(int i);
void KillFloor(void);
char FKnown(char mode);
/* These functions are located in EVENTS.C */
void ForceAnytime(void);
void InitEvents(void);
void ResolveDLStuff(void);
void InitEvTimes(void);
void setPtrs(void);
void EventShow(void);
void ActiveEvents(char *buf);
void *ChED();
void *ChRed();
void *ChkTwoNumbers();
void *EatTwoNumbers(char *line);
void WrtTwoNumbers();
char DoTimeouts(void);
char HandleQuiet(int index);
char *ChkPreempt(long estimated);
char AlreadyProcessed(EVENT *s);
char *RedirectFile(char *filename, char *systemname);
long WeekDiff(long future, long now);
int CheckAutoDoor(char *name);
int during(EVENT *x);
int passed(EVENT *x);
int eventSort(EVENT *s1, EVENT *s2);
int FigureEvent(int index);
int CmpED();
int CmpTwoLong();
TwoNumbers *MakeTwo(int First, long Second);
/* These functions are in DOMAINS.C */
void WriteDomainContents(void);
void UpdateMap(void);
void RationalizeDomains(void);
void DomainInit(char FirstTime);
void DomainFileAddResult(label DName, label system, label NodeId, char result);
char *LocalName(char *system);
char SystemInSecondary(char *Name, char *Domain, char *dup);
char DomainMailFileName(DOMAIN_FILE buffer, label DName, label NodeId,
label NodeName);
UNS_16 FindCost(char *domain);
int DomainOut(char LocalCheck);
#ifdef CONFIGURE
/* These functions are located in CONFG2.C */
int EatEvent(char *line, int offset);
int GetStoreQuote(char *line, char *target, int *rover, int *offset);
int FigureDays(char *vals);
char *getLVal(char *line, int *rover, char fin);
MULTI_NET_DATA FigureNets(char *str);
void EvIsDoor();
void EvFree();
void EventWrite();
/* These functions are located in CONFG.C */
void init(int attended);
void readString(char *source, char *destination, char doProc);
void xlatfmt( char *s );
void illegal(char *errorstring);
void msgInit(void);
void indexRooms(void);
void noteRoom(void);
void logInit(void);
void noteLog(void);
void slideLTab(int slot, int last);
void wrapup(char onlyParams);
void netInit(void);
void crashout(char *str);
void CheckFloors(void);
char isoctal( int c );
char dGetWord(char *dest, int lim);
char zapMsgFile(void);
char realZap(void);
char zapRoomFile(void);
char zapLogFile(void);
char cfindMessage(SECTOR_ID loc, MSG_NUMBER id);
int logSort();
int msgSort();
int strCmpU(char s[], char t[]);
MSG_NUMBER findHighestNative(void);
#endif
/* These functions are located in MODEM.C */
char BBSCharReady(void);
char getMod(void);
char iChar(void);
char JumpStart(int tries, int timeout, int Starter, int t1, int t2,
char (*Method)(int (*wrt)(int c)), int (*WriteFn)(int c));
char Reception(int protocol, int (*WriteFn)(int c));
char recWX(int (*WriteFn)(int c));
char recXYmodem(int (*WriteFn)(int c));
char sendWXchar(int data);
char Transmission(int protocol, char mode);
char XYBlock(int mode, int size);
void FlowControl(void);
void GenTrInit(void);
void initTransfers(void);
void interact(char ask);
void interOut(char c);
void modemInit(int KillCarr);
void ChatGrab(char Up);
void oChar(char c);
void PushBack(char c);
char ModemSetup(char ShouldBeCarrier);
void ringSysop(void);
void runHangup(void);
void SendCmnBlk(char type, TransferBlock *block, char (*SendFn)(int c),
int size);
void SummonSysop(void);
void WXResponses(void);
int ClearWX(void);
int CommonPacket(char type, int size, int (*recFn)(int t), int *Sector);
int CommonWrite(int (*WriteFn)(int c), int size);
int SurreptitiousChar(char c);
void getSize(DirEntry *fileName);
int recWXchar(int ErrorTime);
int sendWCChar(int c);
int YMHdr(long fileSize, char *filename);
int sendWXModem(int c);
int sendYMChar(int c);
int SendYBlk(void);
int XYClear(void);
AN_UNSIGNED modIn(void);
/* These functions are located in LIBROOM.C */
void getRoom(int rm);
void putRoom(int rm);
/* These functions are located in LIBTABL.C */
char readSysTab(char kill, char showMsg);
/* void *GetDynamic(unsigned size); */
int writeSysTab(void);
int common_read(void *block, int size, int elements, FILE *fd, char showMsg);
void openFile(char *filename, FILE **fd);
/* These functions are located in LIBLOG.C */
void getLog(logBuffer *lBuf, int n);
void putLog(logBuffer *lBuf, int n);
/* These functions are located in LIBCRYP.C */
void crypte(void *buf, unsigned len, unsigned seed);
UNS_16 hash(char *str);
/* These functions are located in LIBMSG.C */
char getMessage(int (*Source)(void), char FromNet, char all, char ClearOthers);
void InitMsgBase(void);
void InitBuffers(void);
void getMsgStr(int (*Source)(void), char *dest, int lim);
void startAt(FILE *whichmsg, struct mBuf *mFile, SECTOR_ID sect, int byt);
void unGetMsgChar(char c);
void ZeroMsgBuffer(MessageBuffer *msg);
int getMsgChar(void);
/* These functions are in libmsg also, but handle CC on msgs. */
void *ChkCC();
void DisplayCC();
/* void *EatCC(char *line);
void *MakeCC(char *name, char needstatic); */
void ShowCC(int where);
/* These functions are located in LIBNET.C */
void getNet(int n, NetBuffer *buf);
void putNet(int n, NetBuffer *buf);
char normId(label source, label dest);
/* These functions are located in ARCH.C */
void *EatNMapStr(char *line);
void *EatArchRec(char *line);
void *NtoStrInit(int num, char *str, int num2, char needstatic);
void *ChkNtoStr();
void *ChkStrtoN();
void WrtNtoStr();
void WrtArchRec();
void WrtCC(void *dd);
void FreeNtoStr();
char *AskForNSMap(SListBase *base, int val);
int  GetArchSize(int num);
/* These functions are located in CALLLOG.C */
void logMessage(int val, char *str, char sig);
void CallMsg(char *fn, char *str);
void fileMessage(char mode, char *fn, char IsDL, int protocol, long size);
/* These functions are located in NETMISC.C */
char called_stabilize(void);
char RecipientAvail(void);
char MakeNetted(int m);
char check_for_init(char mode);
char callOut(int i);
char DirectRoute(NetBuffer *system);
char SendPrepAsNormal(char *work, int *count);
char roomsShared(int slot);
char netInfo(char GetName);
char NetValidate(char talk);
char HasOutgoing(int system, int index);
char AnyCallsNeeded(MULTI_NET_DATA whichNets);
char ReqNodeName(char *prompt, label target, label domain, char WideSpecValid,
char Once, char Ask, char Display, char SysopMenu,
NetBuffer *nBuf);
int AckStabilize(int index);
int AddNetMsgs(char *base, void (*procFn)(), char zap, int roomNo, char AddNetArea);
int FindRouteIndex(int system);
int getNMsgChar(void);
int searchNet(char *forId, NetBuffer *buf);
int timeLeft(void);
int GetCompression(int system);
int addSendFile(char *Files);
int searchNameNet(label name, NetBuffer *buf);
int addNetMem(char *netnum);
int makeCall(char EchoErr, MenuId id);
int subNetMem(char *netnum);
int getNetChar(void);
int needToCall(int system, MULTI_NET_DATA CurrentNets);
void inRouteMail(void);
void KillTempFiles(int whichnode);
void NetPrivs(label who);
void NodeValues(MenuId id);
void sendWCFile(FILE *fd);
void DiscardMessage(char *filename);
void inMail(void);
void netController(int NetStart, int NetLength, MULTI_NET_DATA whichNets,
char mode, UNS_16 flags);
void ClearRoomSharing(void);
void ParticipatingNodes(char *target);
void initNetRooms(void);
void AreaCode(char *Id, char *Target);
void setTime(int NetStart, int NetLength);
void moPuts(char *s);
void writeNet(char idsAlso, char LocalOnly);
void NetInit(void);
void netStuff(void);
void getSendFiles(MenuId id, label sysName);
void addNetNode(void);
void editNode(void);
void fileRequest(void);
int DumpRoom(SharedRoom *room, int system, int index, int roomslot,void *d);
void netResult(char *msg);
void killConnection(void);
void setPoll(void);
void EvalNeed(int searcher, MULTI_NET_DATA whichNets);
void parseBadRes(char *c);      /* temporary until next major release */
/* These functions are located in NETCACHE.C */
char SendFastTransfer(void);
void DelFile(DirEntry *f);
char SendMapFile(void);
void ITL_Line(char *data);
char MapFileAccepted(void);
void netFastTran(struct cmd_data *cmds);
void KillCacheFiles(int which);
void ReadFastFiles(void);
void CacheMessages(MULTI_NET_DATA whichNets, char VirtOnly);
void CacheSystem(int system, char moo);
void RecoverMassTransfer(char *line);
/* These functions are located in NETCALL.C */
void caller(void);
void caller_stabilize(void);
void checkMail(void);
void Addressing(int system, int index, char *commnd, char **send1, char **send2,
char **send3, char **name, char *doit);
void EachSharedRoom(int system,
int (*func)(SharedRoom *room, int system, int index, int roomslot, void *d),
int (*virtfunc)(VirtualRoom *room, int sys, int index, int which, void *d),
void *data);
void findAndSend(int commnd, char *send1, char *send2, char *send3, int rover,
int (*MsgSender)(), label roomName, int (*MsgReceiver)(int x, char y));
void NetCC();
void SetHighValues(int rover);
void WriteCC();
char NetRoute(int mode);
void readNegMail(char talk);
void roleReversal(char reversed);
void sendStuff(char reversed, char SureDoIt);
void SendPwd(void);
void sendId(void);
void sendMail(void);
void sendSharedRooms(void);
void doSendFiles(void);
void askFiles(void);
void sendHangUp(void);
void no_good(char *str, char hup);
void netSendFile(DirEntry *fn);
void SendHostFile(char *fn);
char sendNetCommand(struct cmd_data *cmds, char *error);
char multiReceive(struct fl_req *file_data);
char SetAddressFlags(char **s1, char **s2, char **s3, int *commnd, int rover);
int RoutePath(char *rp, char *str);
int netMessage(int uploading);
int s_m_n(void);
int RoomSend(int rover, char *send1, char *send2, char *send3);
int RoomReceive(int rover, char ReplyFirst);
/* These functions are located in NETRCV.C */
void called(void);
void CheckRecipient();
void rcvStuff(char reversed);
void RecoverNetwork(void);
void netPwd(struct cmd_data *cmds);
void doResults(void);
void ReadNetRoomFile(int rover, char *fn);
void UpdateRecoveryFile(char *val);
void getId(void);
void getNextCommand(struct cmd_data *cmds);
void grabCommand(struct cmd_data *cmds, char *sect);
void reply(char state, char *reason);
void reqReversal(char reversed);
void reqCheckMail(void);
void CheckRecipient(char *d);
void targetCheck(void);
void doNetRooms(void);
void getMail(void);
void reqSendFile(struct cmd_data *cmds);
void netFileReq(struct cmd_data *cmds);
void netRRReq(struct cmd_data *cmds, char SendBack);
void netRoomReq(struct cmd_data *cmds);
void recNetMessages(int arraySlot, char *name, int RoomNo, char ReplyFirst);
void netMultiSend(DirEntry *fn);
char RoomRoutable(RoomSearch *data);
int IsRoomRoutable(SharedRoom *room, int system, int index, int roomslot,
void *d);
/* These functions are located in VIRT.C */
char VNeedCall(VirtPoint *VirtP);
int VirtRoomRoutable(VirtualRoom *room, int system, int index, int which,
void *d);
int SendVirtual(int VirtIndex, char *d1, char *d2, char *d3);
int RecVirtualRoom(int VirtIndex, char ReplyFirst);
void SetUpForVirtuals(int VirtIndex, int *VirtNo, char *fn);
void DoVirtuals(int system,
int (*virtfunc)(VirtualRoom *room, int system, int index, int which, void *d),
void *d);
int SendVirtualRoom(VirtualRoom *room, int sys, int index, int which,void *d);
int DumpVRoom(VirtualRoom *room, int system, int index, int which, void *d);
int UnCacheVirtualRoom(VirtualRoom *room, int system, int index, int which,
void *d);
int VRNeedCall(VirtualRoom *room, int system, int index, int which,
char *d);
int VirtualRoomOutgoing(VirtualRoom *room, int system, int index, int which,
char *d);
int VirtualRoomMap(VirtualRoom *room, int system, int index, int which,
void *d);
int CacheVirtualRoom(VirtualRoom *room, int sys, int index, int which, void *d);
void VirtInit(void);
void InitVNode(int slot);
void UpdVirtStuff(void);
void VirtSummary(void);
/* These functions reside in NETITL.C */
char ITL_Receive(char *FileName, int ReplyFirst, int OpenIt,
int (*W)(int c), int (*CloseFn)(FILE *f));
char ITL_Send(char mode);
int sendITLchar(int c);
char ITL_SendMessages(void);
char ITL_StartRecMsgs(char *FileNm, char ReplyFirst, char OpenIt,
int (*OverRide)(int c));
int increment(int c);
void ITL_RecCompact(struct cmd_data *cmds);
void ITL_InitCall(void);
void ITL_StopSendMessages(void);
void ITL_DeInit(void);
void ITL_optimize(char both);
void ITL_rec_optimize(struct cmd_data *cmds);
/* These functions reside in HOT_HELP.C,
courtesy Paul Gauthier. */
int printHelp(char *fileName);
char getFileChar(FILE *file, int init);
int  getHelpNames(FILE *file, char *name, char count);
/* These functions are located in VORTEX.C */
void ClearVortex(void);
void FinVortexing(void);
void InitVortexing(void);
void VortexInit(void);
char NotVortex(void);
long ReadTime(char *time);
/* These functions are located in ROUTE.C */
char RouteHere(char *Id, char *Name, char *Domain);
char AcceptRoute(char *id, char *name);
char FindTheNode(char *id, char *name);
int ReadRoutedDest(int c);
int FindRouteSlot(void);
int SendRoutedAsLocal(void);
int IdStName(char *name);
int ReadRouted(void);
void RouteOut(void);
void netRouteMail(struct cmd_data *cmds);
void NextRouteName(SYS_FILE fn);
void AdjustRoute(void);
void MakeRouted(void);
char *UseNetAlias(char *Name, char FindAlias);
char SendRouteMail(char *filename, char *domainname, char *Tid, char *Tname,
char LocalCheck);
/* Compact.C */
int StartEncode(int (*func)(int c));
int StartDecode(int (*func)(int c));
int Encode(int c);
int Decode(int c);
void StopDecode(void);
void StopEncode( void );
/* Mailfwd.c */
void OpenForwarding(void);
void AddMailForward(char *acct, char *system, char *fwdacct);
void KillLocalFwd(char *name);
char *FindLocalForward(char *name);
/* Stroll.C */
void StrollIt(void);
/* BIO.C */
void EditBio(void);
char GetBioInfo(int which);
void SaveBioInfo(int which);
void ClearBio(int which);
void BioDirectory(void);
#endif
