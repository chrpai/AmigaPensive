;:ts=8

	xref	_PipeUtilBase
	xdef	_zeroize
_zeroize:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-30(a6)
	move.l	(sp)+,a6
	rts

	xdef	_NumFindNode
_NumFindNode:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/a0
	jsr	-36(a6)
	move.l	(sp)+,a6
	rts

	xdef	_MySort
_MySort:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-42(a6)
	move.l	(sp)+,a6
	rts

	xdef	_MakeUpper
_MakeUpper:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-48(a6)
	move.l	(sp)+,a6
	rts

	xdef	_LineWrap
_LineWrap:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-54(a6)
	move.l	(sp)+,a6
	rts

	xdef	_CopySpaces
_CopySpaces:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-60(a6)
	move.l	(sp)+,a6
	rts

	xdef	_InitParse
_InitParse:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-66(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ParseNum
_ParseNum:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	move.l	16(sp),d0
	jsr	-72(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ParseChar
_ParseChar:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	move.l	16(sp),d0
	jsr	-78(a6)
	move.l	(sp)+,a6
	rts

	xdef	_PrintSpaces
_PrintSpaces:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/d1
	jsr	-84(a6)
	move.l	(sp)+,a6
	rts

	xdef	_TxtDisplay
_TxtDisplay:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	movem.l	12(sp),d0/d1
	jsr	-90(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ScanFiles
_ScanFiles:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-96(a6)
	move.l	(sp)+,a6
	rts

	xdef	_InitFileScanKey
_InitFileScanKey:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-102(a6)
	move.l	(sp)+,a6
	rts

	xdef	_CloseFileScanKey
_CloseFileScanKey:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-108(a6)
	move.l	(sp)+,a6
	rts

	xdef	_LoadFile
_LoadFile:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/d1
	jsr	-114(a6)
	move.l	(sp)+,a6
	rts

	xdef	_EditFile
_EditFile:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-120(a6)
	move.l	(sp)+,a6
	rts

	xdef	_SaveFile
_SaveFile:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-126(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetUserFileData
_GetUserFileData:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-132(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetFileAreaData
_GetFileAreaData:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-138(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetFileFlagList
_GetFileFlagList:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-144(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetFileGlobals
_GetFileGlobals:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-150(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetFileStats
_GetFileStats:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-156(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetFile
_GetFile:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/a0
	jsr	-162(a6)
	move.l	(sp)+,a6
	rts

	xdef	_AreasLoad
_AreasLoad:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-168(a6)
	move.l	(sp)+,a6
	rts

	xdef	_XprProtosLoad
_XprProtosLoad:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-174(a6)
	move.l	(sp)+,a6
	rts

	xdef	_IsPlFileThere
_IsPlFileThere:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-180(a6)
	move.l	(sp)+,a6
	rts

	xdef	_IsPlMsgThere
_IsPlMsgThere:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-186(a6)
	move.l	(sp)+,a6
	rts

	xdef	_FileNumberToName
_FileNumberToName:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/a0
	jsr	-192(a6)
	move.l	(sp)+,a6
	rts

	xdef	_FileNameToNumber
_FileNameToNumber:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-198(a6)
	move.l	(sp)+,a6
	rts

	xdef	_FileSize
_FileSize:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-204(a6)
	move.l	(sp)+,a6
	rts

	xdef	_Pvt1
_Pvt1:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-210(a6)
	move.l	(sp)+,a6
	rts

	xdef	_Pvt2
_Pvt2:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-216(a6)
	move.l	(sp)+,a6
	rts

	xdef	_Pvt3
_Pvt3:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-222(a6)
	move.l	(sp)+,a6
	rts

	xdef	_Pvt4
_Pvt4:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-228(a6)
	move.l	(sp)+,a6
	rts

	xdef	_Pvt5
_Pvt5:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-234(a6)
	move.l	(sp)+,a6
	rts

	xdef	_Pvt6
_Pvt6:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-240(a6)
	move.l	(sp)+,a6
	rts

	xdef	_DeleteCatFile
_DeleteCatFile:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-246(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetFileNoExt
_GetFileNoExt:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-252(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetProtocol
_GetProtocol:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-258(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetEditor
_GetEditor:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-264(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetBaudRate
_GetBaudRate:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-270(a6)
	move.l	(sp)+,a6
	rts

	xdef	_BaudRatesLoad
_BaudRatesLoad:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-276(a6)
	move.l	(sp)+,a6
	rts

	xdef	_EditorsLoad
_EditorsLoad:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-282(a6)
	move.l	(sp)+,a6
	rts

	xdef	_LoadMessage
_LoadMessage:
	movem.l	d2/a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	12(sp),d0/d1/d2
	jsr	-288(a6)
	movem.l	(sp)+,d2/a6
	rts

	xdef	_SaveMessage
_SaveMessage:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-294(a6)
	move.l	(sp)+,a6
	rts

	xdef	_InitMsgScanKey
_InitMsgScanKey:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-300(a6)
	move.l	(sp)+,a6
	rts

	xdef	_CloseMsgScanKey
_CloseMsgScanKey:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-306(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ScanMessages
_ScanMessages:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-312(a6)
	move.l	(sp)+,a6
	rts

	xdef	_EditMessage
_EditMessage:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-318(a6)
	move.l	(sp)+,a6
	rts

	xdef	_DeleteMessage
_DeleteMessage:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/d1
	jsr	-324(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetUserMsgData
_GetUserMsgData:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-330(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetMsgAreaData
_GetMsgAreaData:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-336(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetMsgGlobals
_GetMsgGlobals:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-342(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetEditorList
_GetEditorList:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-348(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetNumberOfMessages
_GetNumberOfMessages:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-354(a6)
	move.l	(sp)+,a6
	rts

	xdef	_FreeMessage
_FreeMessage:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-360(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetArchiverList
_GetArchiverList:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-366(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetPointers
_GetPointers:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-372(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ProgramLog
_ProgramLog:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/d1/a0
	jsr	-378(a6)
	move.l	(sp)+,a6
	rts

	xdef	_SetPointers
_SetPointers:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-384(a6)
	move.l	(sp)+,a6
	rts

	xdef	_MoveFile
_MoveFile:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	move.l	16(sp),d0
	jsr	-390(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ReadAddresses
_ReadAddresses:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	move.l	16(sp),d0
	jsr	-396(a6)
	move.l	(sp)+,a6
	rts

	xdef	_WriteAddresses
_WriteAddresses:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	movem.l	12(sp),d0/d1/a1
	jsr	-402(a6)
	move.l	(sp)+,a6
	rts

	xdef	_FreeAddresses
_FreeAddresses:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-408(a6)
	move.l	(sp)+,a6
	rts

	xdef	_CompareAddresses
_CompareAddresses:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-414(a6)
	move.l	(sp)+,a6
	rts

	xdef	_LoadSigs
_LoadSigs:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-420(a6)
	move.l	(sp)+,a6
	rts

	xdef	_FindMessageFromDate
_FindMessageFromDate:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/a0
	move.l	16(sp),d1
	jsr	-426(a6)
	move.l	(sp)+,a6
	rts

	xdef	_FindFileFromDate
_FindFileFromDate:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/a0
	move.l	16(sp),d1
	jsr	-432(a6)
	move.l	(sp)+,a6
	rts

	xdef	_InsertNodeAlpha
_InsertNodeAlpha:
	movem.l	a2/a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	12(sp),a0/a1/a2
	jsr	-438(a6)
	movem.l	(sp)+,a2/a6
	rts

	xdef	_FormMessageText
_FormMessageText:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	movem.l	12(sp),d0/a1
	jsr	-444(a6)
	move.l	(sp)+,a6
	rts

	xdef	_MatchStr
_MatchStr:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-450(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetHighPointer
_GetHighPointer:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/d1
	jsr	-456(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ReadWholeFile
_ReadWholeFile:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-462(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetSigList
_GetSigList:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-468(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetSig
_GetSig:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-474(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetConfigUpdate
_GetConfigUpdate:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-480(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetBaudList
_GetBaudList:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-486(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ScanIndex
_ScanIndex:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-492(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetFileAreaNode
_GetFileAreaNode:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-498(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetIndexFileName
_GetIndexFileName:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-504(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetFileIndex
_GetFileIndex:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	movem.l	12(sp),d0/a1
	jsr	-510(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetMsgAreaNode
_GetMsgAreaNode:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-516(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ParseFidoAddr
_ParseFidoAddr:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-522(a6)
	move.l	(sp)+,a6
	rts

	xdef	_FileAreaAccess
_FileAreaAccess:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/d1
	jsr	-528(a6)
	move.l	(sp)+,a6
	rts

	xdef	_MsgAreaAccess
_MsgAreaAccess:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/d1
	jsr	-534(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetTemplateList
_GetTemplateList:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	jsr	-540(a6)
	move.l	(sp)+,a6
	rts

	xdef	_SetFidoHeaders
_SetFidoHeaders:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-546(a6)
	move.l	(sp)+,a6
	rts

	xdef	_LoadPaths
_LoadPaths:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-552(a6)
	move.l	(sp)+,a6
	rts

	xdef	_SavePaths
_SavePaths:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-558(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetPath
_GetPath:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-564(a6)
	move.l	(sp)+,a6
	rts

	xdef	_AddMsgArea
_AddMsgArea:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-570(a6)
	move.l	(sp)+,a6
	rts

	xdef	_AddFileArea
_AddFileArea:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-576(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ScanMsgHdrs
_ScanMsgHdrs:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	jsr	-582(a6)
	move.l	(sp)+,a6
	rts

	xdef	_LoadOrigin
_LoadOrigin:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-588(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetOrigin
_GetOrigin:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-594(a6)
	move.l	(sp)+,a6
	rts

	xdef	_LoadExecutables
_LoadExecutables:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-600(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetExecutable
_GetExecutable:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-606(a6)
	move.l	(sp)+,a6
	rts

	xdef	_ReflowText
_ReflowText:
	movem.l	a2/a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	12(sp),a0/a1/a2
	movem.l	24(sp),d0/d1
	jsr	-612(a6)
	movem.l	(sp)+,a2/a6
	rts

	xdef	_Validate
_Validate:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-618(a6)
	move.l	(sp)+,a6
	rts

	xdef	_AllocIndex
_AllocIndex:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	move.l	12(sp),d0
	jsr	-624(a6)
	move.l	(sp)+,a6
	rts

	xdef	_BuildIndex
_BuildIndex:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	move.l	16(sp),d0
	jsr	-630(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetCloseFileArea
_GetCloseFileArea:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-636(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetCloseMsgArea
_GetCloseMsgArea:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),d0
	jsr	-642(a6)
	move.l	(sp)+,a6
	rts

	xdef	_AddWaitingMail
_AddWaitingMail:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	movem.l	12(sp),d0/d1
	jsr	-648(a6)
	move.l	(sp)+,a6
	rts

	xdef	_AddMarkedFile
_AddMarkedFile:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),a0/a1
	move.l	16(sp),d0
	jsr	-654(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetSigTag
_GetSigTag:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	move.l	8(sp),a0
	jsr	-660(a6)
	move.l	(sp)+,a6
	rts

	xdef	_GetSigByDirectory
_GetSigByDirectory:
	move.l	a6,-(sp)
	movea.l	_PipeUtilBase,a6
	movem.l	8(sp),d0/a0
	jsr	-666(a6)
	move.l	(sp)+,a6
	rts
