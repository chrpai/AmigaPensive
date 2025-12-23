#ifndef  CLIB_NOVIASYS_PROTOS_H
#define  CLIB_NOVIASYS_PROTOS_H

#ifndef NOVIA_MAIL_H
#include <novia/novia_mail.h>
#endif

#ifndef NOVIA_MESSAGE_H
#include <novia/novia_message.h>
#endif

#ifndef NOVIA_USERDATA_H
#include <novia/novia_userdata.h>
#endif

#ifndef NOVIA_CONFIG_H
#include <novia/novia_config.h>
#endif

void ShowText(char *path);
void ShowPart(char *path, long startseek, long size);
void SystemMessage(char *text);
void SetColor(char newcolor);
void SetBColor(char newcolor);
void space(char wert);
void cleft(char wert);
void cright(char wert);
void LF();
char Getstring(char *bp, char *src, char maxlen, ULONG flags, ULONG sigs);
char Getbyte(UBYTE *p, UBYTE *source, char max, ULONG flags, ULONG sigs);
char Getword(UWORD *p, UWORD *source, char max, ULONG flags, ULONG sigs);
char Getlong(ULONG *p, ULONG *source, char max, ULONG flags, ULONG sigs);
void Writeio(char *string, ULONG len);
void Writelong(ULONG tolong);
void CSI(char csi);
void JumpXY(char x, char y);
void CLS();
char Getline(char *bp, char *src, char maxlen, ULONG flags, ULONG sigs);
char Yesno(BOOL vorgabe,ULONG sigs);
char AskKey(BOOL vorgabe, ULONG flags, ULONG sigs);
char GetChar(char *vorgabe, ULONG sigs);
char WaitKey(ULONG sigs);
void ChangeScreenStatus(char *string);
void ErrorMessage(char *string);
void GetCurserPos();
void FreeMsg(message *msg);
void ReturnMsg(message *msg);
void ClearIOBuffer();
void vioprintf(char *formatstring, char *args);
void PrintDosError(const LONG errorcode);
LONG execute_nsl(char *name);
void Datecode2Date(struct Date *date);
void GetTime(struct Date *date);
void DateToString(const char *buffer, struct Date *date, ULONG flags);

Directory * ndos_mkdir(char *name);
LONG ndos_closeroot();
LONG ndos_openroot();
LONG ndos_parrentdir();
LONG ndos_list();
LONG ndos_changedir(char *name);
LONG ndos_delete(char *name);
LONG ndos_execute(char *name);
LONG ndos_addmail(struct NewItem *item);
LONG ndos_addreply(char *itemname, char *replyname);
LONG ndos_loadselectlist(List *select_list);
LONG ndos_saveselectlist(List *select_list);
LONG ndos_getlist();
LONG ndos_load_ndos_object(char *name, APTR ptr);
LONG ndos_save_ndos_object(char *name, APTR ptr);

LONG ndos_tstselect(char *inputstring);
LONG ndos_additementry(struct ItemEntry *itementry);
LONG ndos_additementry2dir(struct Directory *dir, struct ItemEntry *itementry);
LONG ndos_addmail2dir(Directory *dir, struct NewItem *item);
LONG ndos_closedir(Directory *dir);
void ndos_readmessage(ItemEntryBlock *ieb);
LONG ndos_sendmail(NewMail *newmail);
LONG ndos_rename(char *oldname, char *newname);
LONG ndos_uucp_import_mail(char *filename);
LONG ndos_editflags(char *name);
LONG ndos_info(char *name);
LONG ndos_readmailbox(ItemEntryBlock *ieb);
struct ItemEntryBlock *ndos_get_ieb(char *name);

void Monitor_open();
void Monitor_close();
LONG opengui();
LONG closegui();
void CalcDatecode(struct Date *date);
void StringToDate(char *buffer, struct Date *date, ULONG flags);
char *getstr(UWORD group, UWORD stringno);
ULONG SearchDateKey(char *str);

LONG uucp_sendmail(struct INet_POP3Server *server, struct INet_Mail *mail);
LONG uucp_getmails(struct INet_POP3Server *server);

ULONG SearchUser(char *username);
ULONG FindUser(char *username);
struct UserNode * GetUserByIP(ULONG IPNumber);
void SaveUser(UserData *pointer);
void AddUser(UserData *pointer);
BOOL LoadUser(UserData *pointer, ULONG Usernumber);
char CheckPassword(char *buffer, ULONG UserNumber);

struct MainPortConfig * getmainportconfig();

void  CopyUUCPName(char *string, char *RealName, char *Name);
UBYTE GetUUCPMonth(char *string);
void  ConvertUUCPDate(char *string, struct Date *date);



#endif