#ifndef CLIB_SERTRANS_PROTOS_H
#define CLIB_SERTRANS_PROTOS_H
/*
**	$Filename: clib/sertrans_protos.h $
**	$Version : 14.0 $
**
**	(C) Copyright 1995-1996 Dirk Tietke
**	    All Rights Reserved
*/

/* private functions */

VOID  Private1(VOID);
VOID  Private2(VOID);
VOID  Private3(VOID);
VOID  Private4(VOID);
VOID  Private5(VOID);
VOID  Private6(VOID);

/* public functions */

APTR  ST_OpenSerTrans (ULONG);
VOID  ST_CloseSerTrans (APTR);
VOID  ST_Send(APTR, STRPTR);

VOID ST_Read(APTR, WORD, BOOL, BOOL, BOOL, APTR, STRPTR);
char ST_CursorInput(APTR, WORD, STRPTR);
VOID ST_ReceiveFile(APTR, STRPTR);
VOID ST_SendFile(APTR, APTR);
VOID ST_Logoff(APTR);
VOID ST_OutKick(APTR, BYTE);

/* private functions */

VOID  Private7(VOID);
VOID  Private8(VOID);

/* public functions */

BOOL  ST_CheckCarrier(APTR);
VOID ST_ConPutString(APTR, STRPTR);
VOID ST_SerPutString(APTR, STRPTR);
VOID ST_WriteProtocol(APTR,STRPTR);
char ST_GetChar(APTR);
BYTE ST_HorizCursorInput(APTR,STRPTR);

#endif /* CLIB_SERTRANS_PROTOS_H */

