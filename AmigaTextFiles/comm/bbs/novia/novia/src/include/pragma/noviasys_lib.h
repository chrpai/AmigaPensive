#ifndef INCLUDE_PRAGMA_NOVIASYS_LIB_H
#define INCLUDE_PRAGMA_NOVIASYS_LIB_H

#ifndef INCLUDE_CLIB_NOVIASYS_PROTOS_H
#include <clib/noviasys_protos.h>
#endif

//#pragma amicall(NoviaSysBase, 30
//#pragma amicall(NoviaSysBase, 36
//#pragma amicall(NoviaSysBase, 42
//#pragma amicall(NoviaSysBase, 48
//#pragma amicall(NoviaSysBase, 54
#pragma amicall(NoviaSysBase, 60, ShowText(a0))
#pragma amicall(NoviaSysBase, 66, ShowPart(a0,d0,d1))
#pragma amicall(NoviaSysBase, 72, SystemMessage(a0))
#pragma amicall(NoviaSysBase, 78, SetColor(d0))
#pragma amicall(NoviaSysBase, 84, SetBColor(d0))
#pragma amicall(NoviaSysBase, 90, space(d0))
#pragma amicall(NoviaSysBase, 96, cleft(d0))
#pragma amicall(NoviaSysBase, 102, cright(d0))
#pragma amicall(NoviaSysBase, 108, LF())
#pragma amicall(NoviaSysBase, 114, Getstring(a0,a1,d0,d1,d2))
#pragma amicall(NoviaSysBase, 120, Getbyte(a0,a1,d0,d1,d2))
#pragma amicall(NoviaSysBase, 126, Getword(a0,a1,d0,d1,d2))
#pragma amicall(NoviaSysBase, 132, Getlong(a0,a1,d0,d1,d2))
#pragma amicall(NoviaSysBase, 138, Writeio(a0,d0))
#pragma amicall(NoviaSysBase, 144, Writelong(d0))
#pragma amicall(NoviaSysBase, 150, CSI(d0))
#pragma amicall(NoviaSysBase, 156, JumpXY(d0,d1))
#pragma amicall(NoviaSysBase, 162, CLS())
#pragma amicall(NoviaSysBase, 168, Getline(a0,a1,d0,d1,d2))
#pragma amicall(NoviaSysBase, 174, Yesno(d0,d1))
#pragma amicall(NoviaSysBase, 180, AskKey(d0,d1,d2))
#pragma amicall(NoviaSysBase, 186, GetChar(a0,d0))
#pragma amicall(NoviaSysBase, 192, WaitKey(d0))
#pragma amicall(NoviaSysBase, 198, ChangeScreenStatus(a0))
#pragma amicall(NoviaSysBase, 204, ErrorMessage(a0))
#pragma amicall(NoviaSysBase, 210, GetCurserPos())
#pragma amicall(NoviaSysBase, 216, FreeMsg(a0))
#pragma amicall(NoviaSysBase, 222, ReturnMsg(a0))
#pragma amicall(NoviaSysBase, 228, ClearIOBuffer())
#pragma amicall(NoviaSysBase, 234, vioprintf(a0,a1))
#pragma amicall(NoviaSysBase, 240, PrintDosError(d0))
#pragma amicall(NoviaSysBase, 246, execute_nsl(a0))
#pragma amicall(NoviaSysBase, 252, Datecode2Date(a0))
#pragma amicall(NoviaSysBase, 258, GetTime(a0))
#pragma amicall(NoviaSysBase, 264, DateToString(a0,a1,d0))
#pragma amicall(NoviaSysBase, 270, CalcDatecode(a0))
#pragma amicall(NoviaSysBase, 276, StringToDate(a0,a1,d0))
#pragma amicall(NoviaSysBase, 282, SearchDateKey(a0))
#pragma amicall(NoviaSysBase, 288, getstr(d0,d1))
#pragma amicall(NoviaSysBase, 294, ndos_closeroot())
#pragma amicall(NoviaSysBase, 300, ndos_openroot())				// basic dos functions
#pragma amicall(NoviaSysBase, 306, ndos_parrentdir())
#pragma amicall(NoviaSysBase, 312, ndos_changedir(a0))
#pragma amicall(NoviaSysBase, 318, ndos_mkdir(a0))
#pragma amicall(NoviaSysBase, 324, ndos_closedir(a0))
#pragma amicall(NoviaSysBase, 330, ndos_delete(a0))
#pragma amicall(NoviaSysBase, 336, ndos_editflags(a0))
#pragma amicall(NoviaSysBase, 342, ndos_info(a0))
#pragma amicall(NoviaSysBase, 348, ndos_rename(a0,a1))
#pragma amicall(NoviaSysBase, 354, ndos_getlist())
#pragma amicall(NoviaSysBase, 360, ndos_load_ndos_object(a0,a1))
#pragma amicall(NoviaSysBase, 366, ndos_save_ndos_object(a0,a1))

#pragma amicall(NoviaSysBase, 372, ndos_list())
#pragma amicall(NoviaSysBase, 378, ndos_execute(a0))
#pragma amicall(NoviaSysBase, 384, ndos_get_ieb(a0))
#pragma amicall(NoviaSysBase, 390, Monitor_open())
#pragma amicall(NoviaSysBase, 396, Monitor_close())
#pragma amicall(NoviaSysBase, 402, opengui())
#pragma amicall(NoviaSysBase, 408, closegui())
#pragma amicall(NoviaSysBase, 414, getmainportconfig())

#pragma amicall(NoviaSysBase, 420, ndos_addmail(a0))
#pragma amicall(NoviaSysBase, 426, ndos_addreply(a0,a1))
#pragma amicall(NoviaSysBase, 432, ndos_additementry(a0))
#pragma amicall(NoviaSysBase, 438, ndos_additementry2dir(a0,a1))
#pragma amicall(NoviaSysBase, 444, ndos_addmail2dir(a0,a1))
#pragma amicall(NoviaSysBase, 450, ndos_readmessage(a0))
#pragma amicall(NoviaSysBase, 456, uucp_sendmail(a0,a1))
#pragma amicall(NoviaSysBase, 462, uucp_getmails(a0))


//#pragma amicall(NoviaSysBase, 468, ndos_DoNFSPacket(d0,d1,d2,d3,d4,d5,d6))
//#pragma amicall(NoviaSysBase, 474, ndos_SendNFSPacket(d0,a0))
//#pragma amicall(NoviaSysBase, 480, ndos_WaitNFSPacket(d0,a0))
//#pragma amicall(NoviaSysBase, 486, ndos_AbortNFSPacket(d0,a0))
//#pragma amicall(NoviaSysBase, 492, ndos_ReplyNDSPacket(d0,a0))
//#pragma amicall(NoviaSysBase, 498,
#pragma amicall(NoviaSysBase, 504, ndos_readmailbox(a0))

#pragma amicall(NoviaSysBase, 510, ndos_loadselectlist(a0))
#pragma amicall(NoviaSysBase, 516, ndos_saveselectlist(a0))
#pragma amicall(NoviaSysBase, 522, ndos_tstselect(a0))

#pragma amicall(NoviaSysBase, 528, SearchUser(a0))
#pragma amicall(NoviaSysBase, 534, FindUser(a0))
#pragma amicall(NoviaSysBase, 540, GetUserByIP(d0))
#pragma amicall(NoviaSysBase, 546, SaveUser(a0))
#pragma amicall(NoviaSysBase, 552, AddUser(a0))
#pragma amicall(NoviaSysBase, 558, LoadUser(a0,d0))
#pragma amicall(NoviaSysBase, 564, CheckPassword(a0,d0))

#pragma amicall(NoviaSysBase, 570, CopyUUCPName(a0,a1,a2))
#pragma amicall(NoviaSysBase, 576, GetUUCPMonth(a0))
#pragma amicall(NoviaSysBase, 582, ConvertUUCPDate(a0,a1))
#pragma amicall(NoviaSysBase, 588, ndos_sendmail(a0))
#pragma amicall(NoviaSysBase, 594, ndos_uucp_import_mail(a0))
//#pragma amicall(NoviaSysBase, 600, 

#endif