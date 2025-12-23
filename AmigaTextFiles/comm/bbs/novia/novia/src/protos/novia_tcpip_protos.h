#ifndef NOVIA_REGISTERS_H
#include <novia/novia_registers.h>
#endif

LONG __saveds ASM nslib_uucp_sendmail(register __a0 INet_POP3Server *server, register __a1 INet_Mail *mail);
LONG __saveds ASM nslib_uucp_getmails(register __a0 INet_POP3Server *server);
void __saveds ASM nslib_CopyUUCPName(register __a0 char *string, register __a1 char *RealName, register __a2 char *Name);
UBYTE __saveds ASM nslib_GetUUCPMonth(register __a0 char *string);
void __saveds ASM nslib_ConvertUUCPDate(register __a0 char *string, register __a1 struct Date *date);
