#ifndef NOVIA_REGISTERS_H
#include <novia/novia_registers.h>
#endif

extern Directory * __saveds ASM nslib_mkdir	(register __a0 char *name);
extern LONG __saveds ASM nslib_closeroot		();
extern LONG __saveds ASM nslib_openroot		();
extern LONG __saveds ASM nslib_parrentdir		();
extern LONG __saveds ASM nslib_list				();
extern LONG __saveds ASM nslib_changedir		(register __a0 char *name);
extern LONG __saveds ASM nslib_delete			(register __a0 char *name);
extern LONG __saveds ASM nslib_addmail			(register __a0 struct NewItem *item);
extern LONG __saveds ASM nslib_addreply		(register __a0 char *itemname,					register __a1 char *replyname);
extern LONG __saveds ASM nslib_loadselectlist(register __a0 List *select_list);
extern LONG __saveds ASM nslib_saveselectlist(register __a0 List *select_list);
extern LONG __saveds ASM nslib_getlist			();
extern LONG __saveds ASM nslib_tstselect		(register __a0 char *inputstring);
extern LONG __saveds ASM nslib_additementry	(register __a0 struct ItemEntry *itementry);
extern LONG __saveds ASM nslib_additementry2dir(register __a0 struct Directory *dir,		register __a1 struct ItemEntry *itementry);
extern LONG __saveds ASM nslib_addmail2dir	(register __a0 Directory *dir,					register __a1 struct NewItem *item);
extern LONG __saveds ASM nslib_closedir		(register __a0 Directory *dir);
extern void __saveds ASM nslib_readmessage	(register __a0 ItemEntryBlock *ieb);
extern LONG __saveds ASM nslib_sendmail		(register __a0 NewMail *newmail);
extern LONG __saveds ASM nslib_readmailbox	(register __a0 ItemEntryBlock *ieb);
extern LONG __saveds ASM nslib_rename			(register __a0 char *oldname,						register __a1 char *newname);
extern LONG __saveds ASM nslib_editflags		(register __a0 char *name);
extern LONG __saveds ASM nslib_info				(register __a0 char *name);
extern LONG __saveds ASM nslib_execute			(register __a0 char *name);
extern struct ItemEntryBlock * __saveds ASM nslib_get_ieb			(register __a0 char *name);
extern LONG __saveds ASM nslib_load_ndos_object(register __a0 char *name, register __a1 APTR ptr);
extern LONG __saveds ASM nslib_save_ndos_object(register __a0 char *name, register __a1 APTR ptr);
extern LONG __saveds ASM nslib_uucp_import_mail(register __a0 char *filename);

