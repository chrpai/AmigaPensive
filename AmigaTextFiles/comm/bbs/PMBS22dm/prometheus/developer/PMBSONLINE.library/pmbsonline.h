/*
                               pmbsonline.h

                      Ab PMBS Version 1.0 und hoeher
                              pmbsonline.library
*/


#ifndef PMBSONLINE_H
#define PMBSONLINE_H 1

#pragma libcall PTBase pt_OpenPrometheus  1E 001
#pragma libcall PTBase pt_ClosePrometheus 24 001
#pragma libcall PTBase pt_Send            2A 8002
#pragma libcall PTBase pt_Read            30 81003
#pragma libcall PTBase pt_BlueRead        36 81003
#pragma libcall PTBase pt_Quit            3C 001
#pragma libcall PTBase pt_AnsiStatus      42 001
#pragma libcall PTBase pt_AskUser         48 8002
#pragma libcall PTBase pt_AskLevel        4E 001
#pragma libcall PTBase pt_AskLog          54 001
#pragma libcall PTBase pt_AskBoxname      5A 8002
#pragma libcall PTBase pt_ReceiveFile     60 001
#pragma libcall PTBase pt_Sendfile        66 8002
#pragma libcall PTBase pt_CheckCarrier    6C 001
#pragma libcall PTBase pt_Typefile        72 8002
#pragma libcall PTBase pt_GetChar         78 8002
#pragma libcall PTBase pt_Command         7E 8002
#pragma libcall PTBase pt_Arexx           84 98003

long pt_OpenPrometheus(long);
long pt_ClosePrometheus(long);
void pt_Send(long, char *);
void pt_Read(long, long, char *);
void pt_BlueRead(long, long, char *);
void pt_Quit(long);
long pt_AnsiStatus(long);
void pt_AskUser(long, char *);
void pt_AskLevel(long, long);
long pt_AskLog(long);
void pt_AskBoxname(long, char *);
void pt_ReceiveFile(long);
void pt_Sendfile(long, char *);
long pt_CheckCarrier(long);
void pt_Typefile(long, char *);
void pt_GetChar(long, char *);
void pt_Command(long, char *);
void pt_Arexx(long, char *, char *);

#endif /* PMBSONLINE_H */
