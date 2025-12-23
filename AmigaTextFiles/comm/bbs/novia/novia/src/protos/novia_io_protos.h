#ifndef NOVIA_REGISTERS_H
#include <novia/novia_registers.h>
#endif

#ifndef NOVIA_USERDATA_H
#include <novia/novia_userdata.h>
#endif

extern __saveds ASM void nslib_ShowText( register __a0 char *path);
extern __saveds ASM void nslib_ShowPart( register __a0 char *path, register __d0 long startseek, register __d1 long size);
extern __saveds ASM void nslib_SystemMessage( register __a0 char *text);
extern __saveds ASM void nslib_SetColor( register __d0 char newcolor);
extern __saveds ASM void nslib_SetBColor( register __d0 char newcolor);
extern __saveds ASM void nslib_space( register __d0 char wert);
extern __saveds ASM void nslib_cleft( register __d0 char wert);
extern __saveds ASM void nslib_cright (register __d0 char wert);
extern __saveds ASM void nslib_LF();
extern __saveds ASM char nslib_Getstring( register __a0 char *bp, register __a1 char *src, register __d0 char maxlen, register __d1 ULONG flags, register __d2 ULONG sigs);
extern __saveds ASM char nslib_Getbyte( register __a0 UBYTE *p, register __a1 UBYTE *source, register __d0 char max, register __d1 ULONG flags, register __d2 ULONG sigs);
extern __saveds ASM char nslib_Getword( register __a0 UWORD *p, register __a1 UWORD *source, register __d0 char max, register __d1 ULONG flags, register __d2 ULONG sigs);
extern __saveds ASM char nslib_Getlong( register __a0 ULONG *p, register __a1 ULONG *source, register __d0 char max, register __d1 ULONG flags, register __d2 ULONG sigs);
extern __saveds ASM void nslib_Writeio( register __a0 char *string, register __d0 ULONG len);
extern __saveds ASM void nslib_Writelong(register __d0 ULONG tolong);
extern __saveds ASM void nslib_CSI(register __d0 char csi);
extern __saveds ASM void nslib_JumpXY( register __d0 char x,register __d1 char y);
extern __saveds ASM void nslib_CLS();
extern __saveds ASM char nslib_Getline( register __a0 char *bp, register __a1 char *src, register __d0 char maxlen, register __d1 ULONG flags, register __d2 ULONG sigs);
extern __saveds ASM char nslib_Yesno( register __d0 BOOL vorgabe, register __d1 ULONG sigs);
extern __saveds ASM char nslib_AskKey( register __d0 BOOL vorgabe, register __d1 ULONG flags, register __d2 ULONG sigs);
extern __saveds ASM char nslib_GetChar( register __a0 char *vorgabe, register __d1 ULONG sigs);
extern __saveds ASM char nslib_WaitKey( register __d0 ULONG sigs);
extern __saveds ASM void nslib_ChangeScreenStatus( register __a0 char *string);
extern __saveds ASM void nslib_ErrorMessage( register __a0 char *string);
extern __saveds ASM void nslib_GetCurserPos();
extern __saveds ASM void nslib_FreeMsg( register __a0 message *msg);
extern __saveds ASM void nslib_ReturnMsg( register __a0 message *msg);
extern __saveds ASM void nslib_ClearIOBuffer();
extern __saveds ASM void nslib_vioprintf( register __a0 char *formatstring, register __a1 char *args);
extern __saveds ASM void nslib_PrintDosError( register __d0 const LONG errorcode);
extern __saveds ASM LONG nslib_execute_nsl( register __a0 char *name);
extern __saveds ASM void nslib_Datecode2Date(register __a0 struct Date *date);
extern __saveds ASM void nslib_GetTime(register __a0 struct Date *date);
extern __saveds ASM void nslib_DateToString(register __a0 const char *buffer, register __a1 struct Date *date, register __d0 ULONG flags);
extern __saveds ASM MainPortConfig * nslib_getmainportconfig();

// novia_global.c

extern __saveds ASM void nslib_Monitor_open();
extern __saveds ASM void nslib_Monitor_close();
extern __saveds ASM ULONG nslib_opengui();
extern __saveds ASM ULONG nslib_closegui();

extern __saveds ASM void nslib_CalcDatecode(register __a0 struct Date *date);
extern __saveds ASM void nslib_StringToDate(register __a0 const char *buffer, register __a1 struct Date *date, register __d0 ULONG flags);
extern __saveds ASM char * nslib_getstr(register __d0 UWORD group, register __d1 UWORD stringno);
extern __saveds ASM ULONG nslib_SearchDateKey(register __a0 char *str);


// novia_user.c

extern ULONG __saveds ASM nslib_SearchUser(register __a0 char *username);
extern ULONG __saveds ASM nslib_FindUser(register __a0 char *username);
extern struct UserNode * __saveds ASM nslib_GetUserByIP(register __a0 ULONG IPNumber);
extern  void __saveds ASM nslib_SaveUser(register __a0 UserData *pointer);
extern void __saveds ASM nslib_AddUser(register __a0 UserData *pointer);
extern BOOL __saveds ASM nslib_LoadUser(register __a0 UserData *pointer, register __d0 ULONG Usernumber);
extern char __saveds ASM nslib_CheckPassword(register __a0 char *buffer, register __d0 ULONG UserNumber);


