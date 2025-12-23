#ifndef MUI_TEXTFIELD_MCC_H
#define MUI_TEXTFIELD_MCC_H


#ifndef LIBRARIES_MUI_H
#include "libraries/mui.h"
#endif

#define MUIA_Window_DisableKeys    0x80424c36     /* V15 */


#define MUIA_TextField_Width             (TAG_USER | (5015<<16) | 0x0001)
#define MUIA_TextField_Height            (TAG_USER | (5015<<16) | 0x0002)
#define MUIA_TextField_Font              (TAG_USER | (5015<<16) | 0x0003)    /* set in .mcp           */
#define MUIA_TextField_Contents          (TAG_USER | (5015<<16) | 0x0004)
#define MUIA_TextField_Wrap              (TAG_USER | (5015<<16) | 0x0005)    /* ignored               */
#define MUIA_TextField_Overwrite         (TAG_USER | (5015<<16) | 0x0006)    /* ignored               */
#define MUIA_TextField_Tab               (TAG_USER | (5015<<16) | 0x0009)    /* spaces                */
#define MUIA_TextField_TextPen           (TAG_USER | (5015<<16) | 0x000B)
#define MUIA_TextField_BackgroundPen     (TAG_USER | (5015<<16) | 0x000C)
#define MUIA_TextField_CursorPen         (TAG_USER | (5015<<16) | 0x000D)
#define MUIA_TextField_Clipboard         (TAG_USER | (5015<<16) | 0x000E)  /* clipboard number     */
#define MUIA_TextField_DefaultHeight     (TAG_USER | (5015<<16) | 0x0013)  /* height in lines      */
#define MUIA_TextField_DefaultWidth      (TAG_USER | (5015<<16) | 0x0014)  /* width in characters  */     


#define MUICFG_TextField_CursorPen       (TAG_USER | (5015<<16) | 0x0100)
#define MUICFG_TextField_TextPen         (TAG_USER | (5015<<16) | 0x0101)
#define MUICFG_TextField_BackgroundPen   (TAG_USER | (5015<<16) | 0x0102)
#define MUICFG_TextField_Font            (TAG_USER | (5015<<16) | 0x0103)
#define MUICFG_TextField_Editor          (TAG_USER | (5015<<16) | 0x0104)
#define MUICFG_TextField_Tab             (TAG_USER | (5015<<16) | 0x0105)
#define MUICFG_TextField_TempDir         (TAG_USER | (5015<<16) | 0x0106)
#define MUICFG_TextField_LineLength      (TAG_USER | (5015<<16) | 0x0107)


#define MUIM_TextField_Redraw            (TAG_USER | (5015<<16) | 0x0000)  /* MUIP_TextField_Move */
#define MUIM_TextField_Move              (TAG_USER | (5015<<16) | 0x0015)
#define MUIM_TextField_Load              (TAG_USER | (5015<<16) | 0x0011)  /* MUIP_TextField_File    */        
#define MUIM_TextField_Save              (TAG_USER | (5015<<16) | 0x0012)  /* MUIP_TextField_File  */
#define MUIM_TextField_Clear             (TAG_USER | (5015<<16) | 0x000A)
#define MUIM_TextField_ExternalEditor    (TAG_USER | (5015<<16) | 0x0010)  /*                      */

struct MUIP_TextField_Redraw    { ULONG MethodID; LONG mode; };
struct MUIP_TextField_File      { ULONG MethodID; char *file; };
struct MUIP_TextField_Move      { ULONG MethodID; LONG mode; };
struct MUIP_TextField_Clear     { ULONG MethodID; LONG mode; };

#define MUIV_TextField_Move_Top          0
#define MUIV_TextField_Clear_All         0

#define DEFAULT_TEXTPEN         "m6"
#define DEFAULT_CURSORPEN       "m3"
#define DEFAULT_BACKGROUNDPEN   "m1"

#define MUIC_TextField "TextField.mcc"

#define EditObject MUI_NewObject(MUIC_TextField

#define Editor EditObject,StringFrame

#endif /* MUI_TEXTFIELD_MCC_H */


