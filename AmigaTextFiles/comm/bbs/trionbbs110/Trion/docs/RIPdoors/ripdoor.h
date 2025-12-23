

/*  Trion BBS 'RIPscrip Clidoor Support' header file                    */
/*                                                                      */
/*  V 1.00   08 - Jan - 1998                                            */
/*                                                                      */
/*  (c) Copyright 1995-98 by Paul Spijkerman                            */




#include <exec/types.h>
#include <exec/ports.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#include <clib/dos_protos.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <stdarg.h>




/* ripsupport.c */


UBYTE MakeMegaChar  (int n);
void  Store1Chars   (UBYTE *p, UWORD value);
void  Store2Chars   (UBYTE *p, UWORD value);
void  Store3Chars   (UBYTE *p, UWORD value);
void  Store4Chars   (UBYTE *p, UWORD value);

void  endrip        (void);


/* ripbuttons.c */

void ButtonStyle(UWORD wid,  UWORD hgt,     UWORD orient, UWORD flags,
                 UWORD size, UWORD dfore,   UWORD dback,  UWORD bright,
                 UWORD dark, UWORD surface, UWORD grp_no, UWORD flags2,
                 UWORD uline_col, UWORD corner_col );

void RipButton(UWORD x1, UWORD y1, UWORD x2, UWORD y2, UBYTE hotkey,
               UWORD flags, UBYTE *text);

/* copy image to clipboard */
void  GetImage      (UWORD x1, UWORD y1, UWORD x2, UWORD y2);
/* paste clipboard image to screen */
void  PutImage      (UWORD  x, UWORD  y, UWORD mode);
/* write clipboard contents to disk */
void  WriteIcon     (UBYTE *name);
/* loads and displays a disk based icon to screen */
void  LoadIcon      (UWORD x, UWORD y, UWORD mode, UWORD clip, UBYTE *name);


/* ripgraph.c */

void SetPalette(UWORD  c1, UWORD  c2, UWORD  c3, UWORD  c4, UWORD  c5,
                UWORD  c6, UWORD  c7, UWORD  c8, UWORD  c9, UWORD c10,
                UWORD c11, UWORD c12, UWORD c13, UWORD c14, UWORD c15,
                UWORD c16);

void CustomFillPattern(UWORD  c1, UWORD  c2, UWORD  c3, UWORD  c4, UWORD  c5,
                       UWORD  c6, UWORD  c7, UWORD  c8, UWORD color);

void BezierCurve(UWORD x1, UWORD y1, UWORD x2, UWORD y2,
                 UWORD x3, UWORD y3, UWORD x4, UWORD y4, UWORD count );

void  ripprint       (UWORD x, UWORD y, UBYTE *text);
void  Color          (UWORD color);
void  OnePalette     (UWORD color, UWORD value);
void  GraphicsMode   (UWORD mode);
void  PlotAt         (UWORD  x, UWORD y );
void  LineAt         (UWORD x1, UWORD y1, UWORD x2, UWORD y2);
void  Rectangle      (UWORD x1, UWORD y1, UWORD x2, UWORD y2);
void  FilledRectangle(UWORD x1, UWORD y1, UWORD x2, UWORD y2);
void  FillStyle      (UWORD pattern, UWORD color);
void  SetLineStyle   (UWORD style, UWORD pattern, UWORD thick);
void  Circle         (UWORD x, UWORD y, UWORD radius);
void  FloodFill      (UWORD x, UWORD y, UWORD border);

