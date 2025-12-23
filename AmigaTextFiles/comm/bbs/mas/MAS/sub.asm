; SH-Ware assembly routines for C by S.Klemola

Lib MACRO * function[,basereg]
         xref _LVO\1
         ifnc '\2',''
         movea.l \2,a6
         endc
         jsr _LVO\1(a6)
         ENDM

         section SH-Funcs,code
         near a4

         xref _DOSBase     ; External reference to the base of dos.library

         xdef _rawprint    ; prints an unformatted string
         xdef _print       ; formats a string and calls _rawprint
         xdef _sprint      ; formats a string and puts it into a buffer
         xdef _strcmp
         xdef _strcpy
         xdef _strlen
         xdef _clear

_rawprint         ; ULONG rawprint(string)

         movea.l  4(sp),a1

_raw1    movem.l  d2/d3/a6,-(sp)    ; registered entry point (A1=string)
         move.l   a1,d2
         moveq    #-1,d3
_raw2    addq.l   #1,d3
         tst.b    (a1)+
         bne      _raw2
         Lib      Output,_DOSBase(a4)
         move.l   d0,d1
         Lib      Write
_raw3    movem.l  (sp)+,d2/d3/a6
         rts

_print            ; ULONG print(string[,arg][,arg][...])

         movem.l  d4/a2-a4/a6,-(sp)
         clr.l    d0
         bset.l   #9,d0
         clr.l    d1
         Lib      AllocMem,4
         movea.l  d0,a3
         tst.l    d0
         beq      _pri1
         movea.l  24(sp),a0
         lea      28(sp),a1
         lea      _pri2(pc),a2
         Lib      RawDoFmt
         movea.l  a3,a1
         bsr      _raw1
         move.l   d0,d4
         movea.l  a3,a1
         clr.l    d0
         bset.l   #9,d0
         Lib      FreeMem,4
         move.l   d4,d0
_pri1    movem.l  (sp)+,d4/a2-a4/a6
         rts
_pri2    move.b   d0,(a3)+
_rts     rts

_sprint           ; ULONG sprint(buffer,string[,arg][,arg][...])

         movem.l  d4/a2-a4/a6,-(sp)
         tst.l    24(sp)
         beq      _pri1
         movea.l  24(sp),a3
         movea.l  28(sp),a0
         lea      32(sp),a1
         lea      _pri2(pc),a2
         Lib      RawDoFmt,4
         movea.l  a3,a0
         bsr      _strlen1
         bra      _pri1

_strcmp           ; ULONG strcmp(string,string)

         movea.l  4(sp),a0
         movea.l  8(sp),a1
_strcmp1 clr.l    d0
compb    move.b   (a0)+,d1
         bne      compc
         tst.b    (a1)
         beq      compa
         bne      compd
compc    cmp.b    (a1)+,d1
         beq      compb
compd    moveq    #1,d0
compa    rts

_strcpy           ; LONG strcpy(buffer,string)

         movea.l  4(sp),a1
         movea.l  8(sp),a0
_strcpy1 clr.l    d0
_strcpy2 move.b   (a0)+,(a1)+
         beq      _rts
         addq.l   #1,d0
         bra      _strcpy2

_strlen           ; LONG strlen(string)

         movea.l  4(sp),a0
_strlen1 clr.l    d0
_strlen2 tst.b    (a0)+
         beq      _rts
         addq.l   #1,d0
         bra      _strlen2

_clear            ; void clear(buffer,length)

         movea.l  4(sp),a0
         move.l   8(sp),d0
         beq      _rts
         subq.l   #1,d0
         beq      _rts
_clear1  clr.b    (a0)+
         dbf      d0,_clear1
         rts

