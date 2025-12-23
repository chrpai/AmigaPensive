/***********************************************************
 *
 * ConnectLine: Allgemeine GUI- und System-Includes (MUI)
 *
 * Verwalter: Michael Balzer
 * 
 * $RCSfile: GUI.h $
 * $Revision: 1.1 $
 * $Date: 1994/02/19 17:05:14 $
 *
 * $Author: balzer $
 * $Locker: balzer $
 * $State: Exp $
 *
 * $Log: GUI.h $
 * Revision 1.1  1994/02/19  17:05:14  balzer
 * Initial revision
 *
 */

#ifndef _CL_GUI_H
#define _CL_GUI_H


/*
 * System
 */

#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <graphics/gfxmacros.h>
#include <workbench/workbench.h>
#include <devices/timer.h>
#include <libraries/locale.h>
#include <libraries/mui.h>


/*
 * Prototypes
 */

#include <clib/alib_protos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/locale.h>
#include <proto/icon.h>
#include <proto/muimaster.h>


/*
 * ANSI C
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <dos.h>


/*
 * Compiler specific stuff
 */

#define REG(x) register __ ## x
#define ASM    __asm
#define SAVEDS __saveds


/*
 * Prefs- und MUI-ID-Generator
 */

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif


/*
 * Lokalisierung
 */

#define CATCOMP_NUMBERS

#ifdef __cplusplus
extern "C"
{
#endif

STRPTR __asm GetString( register __a0 struct LocaleInfo *li, register __d0 LONG stringNum );
extern struct LocaleInfo __cllocaleinfo;

#ifdef __cplusplus
}
#endif

#define GS(id) GetString( &__cllocaleinfo, id )
#define MSG(x) GetString( &__cllocaleinfo, MSG_ ## x )
#define KEY(x) *GetString( &__cllocaleinfo, MSG_ ## x ## _Key )


/*
 * Konstanten fuer MUIA_Disabled
 */

#define DISABLED	TRUE
#define ENABLED		FALSE


/*
 * Abkürzung zum Anlegen eines Hooks
 */

#define HOOK( fn ) cl_allochook( (HOOKFUNC) fn, NULL )


/*
 * Makros für MUI_MakeObject
 */

#define MkString( label, maxlen ) \
	MUI_MakeObject( MUIO_String, label, maxlen )

#define MkCycle( label, entries ) \
	MUI_MakeObject( MUIO_Cycle, label, entries )

#define MkCheckMark( label ) \
	MUI_MakeObject( MUIO_Checkmark, label )


/*
 * Menu-Erzeugung
 */

#define Menu( title ) \
	MenuObject, \
		MUIA_Menu_Title, title

#define MenuChild MUIA_Family_Child

#define MenuItem( title, shortcut, userdata ) \
	MenuitemObject, \
		MUIA_Menuitem_Title, title, \
		MUIA_Menuitem_Shortcut, shortcut, \
		MUIA_UserData, userdata, \
		End

#define MenuToggleItem( title, shortcut, userdata, state ) \
	MenuitemObject, \
		MUIA_Menuitem_Title, title, \
		MUIA_Menuitem_Shortcut, shortcut, \
		MUIA_Menuitem_Toggle, TRUE, \
		MUIA_Menuitem_Checkit, TRUE, \
		MUIA_Menuitem_Checked, state, \
		MUIA_UserData, userdata, \
		End

#define MenuItemSeparator \
	MenuitemObject, \
		MUIA_Menuitem_Title, NM_BARLABEL, \
		End


/*
 * andere nützliche Makros
 */

#define export( obj, id ) \
	set( obj, MUIA_ExportID, id );


#endif
