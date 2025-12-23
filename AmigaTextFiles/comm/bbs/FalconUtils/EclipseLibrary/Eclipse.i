;Released as public domain 28-Jan-1996 by the author Dave Dustin

*****************************************************************************
*                                                                           *
* This is the Assembly include file for my custom library (Eclipse.Library) *
*                                                                           *
*                                                                           *
* The library itself is compatible with both Dos 1.3 and Dos 2.x, but many  *
* of my programs which use this library only work with Dos 2.x.             *
*                                                                           *
* Neither the source code, or autodocs are included with this library for   *
* several reasons.  I am still working on the library (but all versions are *
* compatible with each other (hopefully), and there are many more examples  *
* of how to program custom librarys.  This Include file is included so that *
* my programs my be assembled by other people.                              *
*                                                                           *
*****************************************************************************

	LIBINIT
	LIBDEF	_LVOChangeCase
	LIBDEF	_LVOConvertDecimal
	LIBDEF	_LVOConvertToDecimal

	LIBDEF	_LVOFindData
	LIBDEF	_LVOCopyData
	LIBDEF	_LVOCopyDataExclude

	LIBDEF	_LVOCompareStrings

	LIBDEF	_LVOConvertFalconDate
	LIBDEF	_LVOFindUser
	LIBDEF	_LVOGetConfigEntry
	LIBDEF	_LVOLogTag

	LIBDEF	_LVOOutputText

	LIBDEF	_LVODate
	LIBDEF	_LVOLongDay
	LIBDEF	_LVOShortDay
	LIBDEF	_LVOTime12
	LIBDEF	_LVOTime24

	LIBDEF	_LVODivu32Bit
	LIBDEF	_LVOMulu32Bit

	LIBDEF	_LVOReadFile
	LIBDEF	_LVOGetFile
	LIBDEF	_LVOGetFileSize

EclipseName	MACRO
	Dc.B	'eclipse.library',0
	ENDM
EclipseVer	Equ	35

CALLECLIPSE	MACRO
	Move.L	_ECLIPSEBase,A6
	Jsr	_LVO\1(A6)
	ENDM
