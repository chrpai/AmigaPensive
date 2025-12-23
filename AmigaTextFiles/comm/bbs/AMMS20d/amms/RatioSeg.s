	opt	l-,c-,e+

	INCDIR	"Include/"
	INCLUDE	"mb/Main.i"
	INCLUDE "mb/User.i"
	INCLUDE "mb/Text.i"

*************************************************
*	V1.14
* NAME	  : UDRate_Unit
*
* FUNKTION: Berechnung der Download/Upload-Ratio
*	    - Steht im Brettkommentar ein ONLYBYTE: am Anfang, so
*	      wird nur die Byteratio beim Upload und Download benutzt
*	    - Steht im Brettkommentar ein ONLYDOWN: am Anfang, so
*	      wird nur die Downloadratio beim Upload und Download benutzt
*	    - Steht im Brettkommentar ein BYTE: am Anfang, so
*	      wird nur die Byteratio beim Upload benutzt,
*	      beim Download wird Byteratio und im Fehlerfall die
*	      Downloadratio benutzt
*	    - Steht im Brettkommentar ein DOWN: am Anfang, so
*	      wird nur die Downloadratio beim Upload benutzt,
*	      beim Download wird Downloadratio und im Fehlerfall die
*	      Byteratio benutzt
*	    - Sonst sind beide Ratios aktiv, mind. eine muss erfuellt sein
*	    Nach folgender Methode wird Upload/Download-Ratio berechnet :
*	    - Restdownloadanzahl = Restdownloadanzahl + Ratio
*	    - Restdownloadbytes = Restdownloadbytes + Ratio * Filelaenge
*	      Restdownloadbytes werden in KByte angegeben und gespeichert !
*	    - Wenn beide Aktiv, so muss mind. fuer den Download ein Fall
*	      erfuellt sein, es werden bei beiden Abzuege bzw. Zugaben
*	      vorgenommen
*
*	Update: 13-Aug-1995
*************************************************

RatioB_UpByte:	equ	0		; Bitnr: Byteratio-Upload
RatioB_DoByte:	equ	1		; Bitnr: Byteratio-Download
RatioB_UpDown:	equ	2		; Bitnr: Downratio-Upload
RatioB_DoDown:	equ	3		; Bitnr: Downratio-Download
RatioB_ErrByte:	equ	4		; Bitnr: Byteratio-Ersatzdownload
RatioB_ErrDown:	equ	5		; Bitnr: Downratio-Ersatzdownload
RatioF_UpByte:	equ	1<<RatioB_UpByte ; Byteratio-Upload
RatioF_DoByte:	equ	1<<RatioB_DoByte ; Byteratio-Download
RatioF_UpDown:	equ	1<<RatioB_UpDown ; Downratio-Upload
RatioF_DoDown:	equ	1<<RatioB_DoDown ; Downratio-Download
RatioF_ErrByte:	equ	1<<RatioB_ErrByte ; Byteratio-Ersatzdownload
RatioF_ErrDown:	equ	1<<RatioB_ErrDown ; Downratio-Ersatzdownload


	moveq	#0,d0
	rts
	jmp	TestDownload
	jmp	AddDownload
	jmp	SubDownload
	jmp	AddUpload
	dc.l	T_Version


*************************************************
*
* NAME	  : TestDownload
*
* FUNKTION: Pruefe, ob weiterer Download moeglich
*
* INPUT	  : d0.l: Laenge des Download-Files
*	    d7.w: Filebrettnummer (-1=kein Filebrett)
*
* OUTPUT  : keine
*	N-Bit: Download nicht moeglich
*************************************************
TestDownload:
 movem.l d0/d4/a0,-(sp)			; Register retten

 bsr WhichRatio				; Ratioart in d4 holen

 move.l Main_User(a5),a0		; Zeiger auf Userdaten in a0

** Restdownloads pruefen

 bclr #RatioB_DoDown,d4			; Download-Ratio erlaubt ?
 beq.s TestDownload_Down_End		; Nein, verzw.
TestDownload_Down:
 bclr #RatioB_ErrDown,d4		; Ersatz-Download-Ratio verbieten
 tst.w UserDat_DownLeft(a0)		; Restdownloads vorhanden ?
 bne.s TestDownload_OK			; Ja, verzw.
 btst #RatioB_ErrByte,d4		; Byte-Ratio im Fehlerfall nutzen ?
 bne.s TestDownload_Byte		; Ja, verzw.
TestDownload_Down_End:

** Restdownloadbytes pruefen

 bclr #RatioB_DoByte,d4			; Byte-Ratio erlaubt ?
 beq.s TestDownload_Error		; Nein, verzw.
TestDownload_Byte:
 bclr #RatioB_ErrByte,d4		; Ersatz-Byte-Ratio verbieten
  add.l #512,d0				; Filelaenge in KByte umrechnen,
  lsr.l #8,d0				; runde dabei in der Mitte
  lsr.l #2,d0				; d0=(d0+512)/1024
 cmp.l UserDat_DownBytesLeft(a0),d0	; Genug Restdownloadbytes vorhanden ?
 bls.s TestDownload_OK			; Ja, verzw.
 btst #RatioB_ErrDown,d4		; Download-Ratio im Fehlerfall nutzen ?
 bne.s TestDownload_Down		; Ja, verzw.
 bra.s TestDownload_Error		; Nein, verzw.

** Status auf OK stellen

TestDownload_OK:
 moveq #0,d0				; Status auf OK stellen

** Ende

TestDownload_End:
 movem.l (sp)+,d0/d4/a0			; Register holen
 rts

** Status auf Fehler setzen

TestDownload_Error:
 moveq #-1,d0				; Status auf Fehler stellen
 bra.s TestDownload_End			; -> Ende



*************************************************
*
* NAME	  : AddDownload
*
* FUNKTION: Ein Download-File aus Download-Restdaten abziehen
*	    IM Buffer werden Daten fuer den eventuellen
*	    SubDownload()-Aufruf gespeichert
*
* INPUT	  : d0.l: Laenge des Download-Files
*	    d7.w: Filebrettnummer (-1=kein Filebrett)
*	    a0.l: Zeiger auf Buffer (Laenge=File_RatioSeg_Buf_Size)
*
* OUTPUT  : keine
*	N-Bit: Download nicht moeglich
*************************************************
AddDownload:
 movem.l d0-d2/d4/a6,-(sp)		; Register retten

 bsr WhichRatio				; Ratioart in d4 holen

 move.l Main_UserLib(a5),a6		; Userbase in a6
 jsr User_WaitON(a6)			; User-Zugriff sperren
 move.l Main_User(a5),a6		; Zeiger auf Userdaten in a6
 moveq #-1,d2				; Status: N-Bit (Fehler)

** Restdownloadanzahl erniedrigen

 clr.w File_RatioSeg_Buf_Downloads(a0)	; Abgezogene Download-Anzahl loeschen
 bclr #RatioB_DoDown,d4			; Download-Ratio erlaubt ?
 beq.s AddDownLoad_SubDown_End		; Nein, verzw.
AddDownload_SubDown:
 bclr #RatioB_ErrDown,d4		; Ersatz-Download-Ratio verbieten
 move.w UserDat_DownLeft(a6),d1		; Restdownloadanzahl in d1
 bne.s AddDownLoad_SubDown_Start	; Wenn Restdownloads, verzw.
 btst #RatioB_ErrByte,d4		; Byte-Ratio im Fehlerfall nutzen ?
 bne.s AddDownload_Byte			; Ja, verzw.
 bra.s AddDownLoad_SubDown_End
AddDownLoad_SubDown_Start:
 cmp.w #UserDat_DownLeft_Unlimited,d1	; Unendlich viele Restdownloads ?
 beq.s AddDownload_SubDown_OK		; Ja, verzw.
 subq.w #1,d1				; Restdownloadanzahl erniedrigen
 addq.w #1,File_RatioSeg_Buf_Downloads(a0) ; Abgezogene Download-Anzahl speichern
 move.w d1,UserDat_DownLeft(a6)		; Restdownloadanzahl speichern
AddDownload_SubDown_OK:
 moveq #0,d2				; Status: N'-Bit (OK)
AddDownLoad_SubDown_End:

** Restdownloadbytes erniedrigen

 clr.l File_RatioSeg_Buf_DownBytes(a0)	; Abgezogene Downloadbytes loeschen
 bclr #RatioB_DoByte,d4			; Byte-Ratio erlaubt ?
 beq.s AddDownload_Byte_End		; Nein, verzw.
AddDownload_Byte:
 bclr #RatioF_ErrByte,d4		; Ersatz-Byte-Ratio verbieten
 move.l UserDat_DownBytesLeft(a6),d1	; Restdownloadbytes in d1
 cmp.l #UserDat_DownBytesLeft_Unlimited,d1 ; Unendlich viele Restdownloadbytes ?
 beq.s AddDownload_Byte_OK		; Ja -> Ende mit OK
  add.l #512,d0				; Filelaenge in KByte umrechnen,
  lsr.l #8,d0				; runde dabei in der Mitte
  lsr.l #2,d0				; d0=(d0+512)/1024
 cmp.l d1,d0				; Genug Bytes vorhanden ?
 bls.s AddDownload_Byte_Dec		; Ja, verzw.
 tst.w d2				; Status noch auf Fehler ?
 bpl.s AddDownload_Byte_SimulateOK	; Nein, verzw.
 btst #RatioB_ErrDown,d4		; Download-Ratio im Fehlerfall nutzen ?
 bne.s AddDownload_SubDown		; Ja, verzw.
 bra.s AddDownLoad_Byte_End
AddDownload_Byte_SimulateOK:
 move.l d1,d0				; Downloadbytes fuer Null erniedrigen
AddDownload_Byte_Dec:
 move.l d0,File_RatioSeg_Buf_DownBytes(a0) ; Abgezogene Downloadbytes speichern
 sub.l d0,d1				; Restdownloadbytes erniedrigen
 move.l d1,UserDat_DownBytesLeft(a6)	; Restdownloadbytes speichern
AddDownload_Byte_OK:
 moveq #0,d2				; Status: N'-Bit (OK)
AddDownload_Byte_End:

** Ende

 move.l Main_UserLib(a5),a6		; Userbase in a6
 jsr User_WaitOFF(a6)			; User-Zugriff erlauben
 tst.w d2				; Status-Bit setzen
 movem.l (sp)+,d0-d2/d4/a6		; Register holen
 rts



*************************************************
*
* NAME	  : SubDownload
*
* FUNKTION: Ein Download-File wegen Abbruch zu Download-Restdaten wieder
*	    addieren. Im Buffer stehen Daten vom AddDownload()-Aufruf.
*
* INPUT	  : a0.l: Zeiger auf Buffer (Laenge=File_RatioSeg_Buf_Size)
*
* OUTPUT  : keine
*
*************************************************
SubDownload:
 movem.l d1/a6,-(sp)			; Register retten

 move.l Main_UserLib(a5),a6		; Userbase in a6
 jsr User_WaitON(a6)			; User-Zugriff sperren
 move.l Main_User(a5),a6		; Zeiger auf Userdaten in a6

** Restdownloadanzahl erhoehen

 move.w UserDat_DownLeft(a6),d1		; Restdownloadanzahl in d1
 cmp.w #UserDat_DownLeft_Unlimited,d1	; Unendlich viele Restdownloads ?
 beq.s SubDownload_AddBytes		; Ja -> Ende
 cmp.w #UserDat_DownLeft_Max,d1		; Max. Anzahl erreicht ?
 beq.s SubDownload_AddBytes		; Ja, verzw.
 add.w File_RatioSeg_Buf_Downloads(a0),d1 ; Restdownloadanzahl erhoehen
 move.w d1,UserDat_DownLeft(a6)		; Restdownloadanzahl speichern

** Restdownloadbytes erhoehen

SubDownload_AddBytes:
 move.l UserDat_DownBytesLeft(a6),d1	; Restdownloadbytes in d1
 cmp.l #UserDat_DownBytesLeft_Unlimited,d1 ; Unendlich viele Restdownloadbytes ?
 beq.s SubDownload_End			; Ja -> Ende
 add.l File_RatioSeg_Buf_DownBytes(a0),d1 ; Restdownloadbytes erhoehen
 bcs.s SubDownload_SetMax		; Ueberlauf, verzw.
 cmp.l #UserDat_DownBytesLeft_Max,d1	; Maximum ueberschritten ?
 bls.s SubDownload_Set			; Nein, verzw.
SubDownload_SetMax:			; Nein, Ueberlauf
 move.l #UserDat_DownBytesLeft_Max,d1	; Maximum in d1
SubDownload_Set:
 move.l d1,UserDat_DownBytesLeft(a6)	; Restdownloadbytes speichern

** Ende

SubDownload_End:
 move.l Main_UserLib(a5),a6		; Userbase in a6
 jsr User_WaitOFF(a6)			; User-Zugriff erlauben
 movem.l (sp)+,d1/a6			; Register holen
 rts



*************************************************
*
* NAME	  : AddUpload
*
* FUNKTION: Ein Upload-File zu Download-Restdaten addieren
*	    - Ratio wird hier benutzt
*
* INPUT	  : d0.l: Laenge des Download-Files
*	    d7.w: Filebrettnummer (-1=kein Filebrett)
*
* OUTPUT  : keine
*
*************************************************
AddUpload:
 movem.l d0-d4/a0/a6,-(sp)		; Register retten

 bsr WhichRatio				; Ratioart in d4 holen

 move.l Main_UserLib(a5),a6		; Userbase in a6
 jsr User_WaitON(a6)			; User-Zugriff sperren

 move.l Main_User(a5),a0		; Zeiger auf Userdaten in a0

** Restdownloadanzahl erhoehen

 btst #RatioB_UpDown,d4			; Download-Ratio erlaubt ?
 beq.s AddUpload_Down_End		; Nein, verzw.
 moveq #0,d2				; d2 loeschen
 move.b UserDat_DownUp(a0),d2		; Downloadanzahl-Ratio in d2
 beq.s AddUpload_Down_End		; Wenn Null (ausgestellt), verzw.
 move.w UserDat_DownLeft(a0),d1		; Restdownloadanzahl in d1
 cmp.w #UserDat_DownLeft_Unlimited,d1	; Unendlich viele Restdownloads ?
 beq.s AddUpload_Down_End		; Ja, verzw.
 add.w d2,d1				; Up-Down-Ratio addieren
 cmp.w #UserDat_DownLeft_Max,d1		; Max. Anzahl ueberschritten ?
 bls.s AddUpload_Down_Set		; Nein, verzw.
 move.w #UserDat_DownLeft_Max,d1	; Max. Anzahl in d1
AddUpload_Down_Set:
 move.w d1,UserDat_DownLeft(a0)		; Restdownloadanzahl speichern
AddUpload_Down_End:

** Restdownloadbytes erhoehen

 btst #RatioB_UpByte,d4			; Byte-Ratio erlaubt ?
 beq.s AddUpload_Byte_End		; Nein, verzw.
 moveq #0,d1				; d1 loeschen
 move.b UserDat_DownUpBytes(a0),d1	; Faktor in d1
 beq.s AddUpload_Byte_End		; Wenn Null (ausgestellt), verzw.
 move.l UserDat_DownBytesLeft(a0),d2	; Restdownloadbytes in d2
 cmp.l #UserDat_DownBytesLeft_Unlimited,d2 ; Unendlich viele Restdownloadbytes ?
 beq.s AddUpload_Byte_End		; Ja, verzw.
  add.l #512,d0				; Filelaenge in KByte umrechnen,
  lsr.l #8,d0				; runde dabei in der Mitte
  lsr.l #2,d0				; d0=(d0+512)/1024
** 16 Bit * 32 Bit-Multiplikation
 move.l d0,d3				; Bits 16-31 der Laenge in d3
 swap d3
 mulu d1,d3				; Obere 16 Bits mit Faktor multiplizieren
 swap d3				; Um 16 Bits nach oben shiften
; clr.w d3				; Unteres Wort loechen
 mulu d1,d0				; Untere 16 Bits mit Faktor multiplizieren
 add.l d3,d0				; Beide Werte addieren
 bcs.s AddUpload_Byte_SetMax		; Wenn Ueberlauf, verzw.
 add.l d2,d0				; Alten Restwert und neuen Wert summieren
 bcs.s AddUpload_Byte_SetMax		; Wenn Ueberlauf, verzw.
**
 cmp.l #UserDat_DownBytesLeft_Max,d0	; Maximum ueberschritten ?
 bls.s AddUpload_Byte_SetBytes		; Nein, verzw.
AddUpload_Byte_SetMax:			; Nein, Ueberlauf
 move.l #UserDat_DownBytesLeft_Max,d0	; Maximum in d0
AddUpload_Byte_SetBytes:
 move.l d0,UserDat_DownBytesLeft(a0)	; Restdownloadbytes speichern
AddUpload_Byte_End:

** Ende

 move.l Main_UserLib(a5),a6		; Userbase in a6
 jsr User_WaitOFF(a6)			; User-Zugriff erlauben
 movem.l (sp)+,d0-d4/a0/a6		; Register holen
 rts



*************************************************
*
* NAME	  : WhichRatio
*
* FUNKTION: Pruefe, ob durch den Brettkommentar die Ratio-Art
*	    eingeschraenkt ist
*	    - Steht im Brettkommentar ein BYTE: am Anfang, so
*	      wird nur die Byteratio benutzt
*	    - Steht im Brettkommentar ein DOWN: am Anfang, so
*	      wird nur die Downloadratio benutzt
*
* INPUT	  : d7.w: Filebrettnummer (-1=kein Filebrett)
*
* OUTPUT  : d4.w: Art der Ratio (Bit0(s)=ByteRatio / Bit1(s)=Downloadratio)
*
*************************************************
WhichRatio:
 movem.l a0/a1/a6,-(sp)			; Register retten

 tst.w d7				; Filebrett ?
 bmi.s WhichRatio_Both			; Nein -> Beide Arten erlauben

 move.l Main_TextLib(a5),a6		; Textbase in a6

 move.l Main_FileLib(a5),a1		; Filebase in a1
 move.l File_Comment(a1),a1		; Zeiger auf Kommentartabelle in a1
 move.w d7,d4				; Zeiger auf Kommentar in a1 errechnen
 mulu #File_CommentLength,d4
 add.l d4,a1

 moveq #RatioF_UpByte+RatioF_DoByte,d4	; Nur Byteratio beim Upload&Download in d4
 lea T_OnlyByteRatio(pc),a0		; Zeiger auf Wildcard-String in a0
 jsr Text_STCompare(a6)			; Vergleiche beide Strings
 beq.s WhichRatio_End			; Gleich -> Byteratio

 moveq #RatioF_UpDown+RatioF_DoDown,d4	; Nur Downloadratio beim Upload & Download in d4
 lea T_OnlyDownRatio(pc),a0		; Zeiger auf Wildcard-String in a0
 jsr Text_STCompare(a6)			; Vergleiche beide Strings
 beq.s WhichRatio_End			; Gleich -> Downratio

 moveq #RatioF_UpByte+RatioF_DoByte+RatioF_ErrDown,d4 ; Nur Byteratio beim Upload in d4
 lea T_ByteRatio(pc),a0			; Zeiger auf Wildcard-String in a0
 jsr Text_STCompare(a6)			; Vergleiche beide Strings
 beq.s WhichRatio_End			; Gleich -> Byteratio

 moveq #RatioF_UpDown+RatioF_DoDown+RatioF_ErrByte,d4 ; Nur Downloadratio beim Upload in d4
 lea T_DownRatio(pc),a0			; Zeiger auf Wildcard-String in a0
 jsr Text_STCompare(a6)			; Vergleiche beide Strings
 beq.s WhichRatio_End			; Gleich -> Downratio

WhichRatio_Both:			; Alle Ratios erlaubt in d2
 moveq #RatioF_UpByte+RatioF_UpDown+RatioF_DoByte+RatioF_DoDown,d4

WhichRatio_End:
 movem.l (sp)+,a0/a1/a6			; Register holen
 rts



***** DATENABSCHNITT


T_Version:	dc.b	"AMMS-RatioSeg V1.13",0
T_ByteRatio:	dc.b	"BYTE:*",0
T_DownRatio:	dc.b	"DOWN:*",0
T_OnlyByteRatio: dc.b	"ONLYBYTE:*",0
T_OnlyDownRatio: dc.b	"ONLYDOWN:*",0


