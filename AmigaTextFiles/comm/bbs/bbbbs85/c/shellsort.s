
;exec.library
allocmem	=	-198
freemem		=	-210
oldopenlibrary	=	-408
closelibrary	=	-414

;dos.library
open		=	-30
close		=	-36
read		=	-42
write		=	-48
output		=	-60
lock		=	-84
unlock		=	-90
examine		=	-102


	movem.l	d1-d7/a1-a6,-(a7)
;-------------
	move.l	$4.w,a6
	movem.l	d0/a0,-(a7)
	lea	doslib,a1
	jsr	oldopenlibrary(a6)
	move.l	d0,a5
	movem.l	(a7)+,d0/a0		;vrati registre
	move.l	a5,dosbase		;sacuvaj bazu

	cmp.l	#1,d0			;ako nema ulaza
	beq.w	kraj			;idi na kraj
	cmp.b	#$3f,(a0)		;da li je prvi znak '?'
	beq.w	kraj
	
	lea	ponovo,a1
	move.l	a1,case

	move.l	a0,a2			;privremeno spremi
ima_li	lea	buff,a1
	bsr.w	string			;procitaj prvi string sa ulaza
	cmp.b	#"-",(a1)		;da li je to neka opcija
	bne.b	nista
	cmp.b	#'n',buff+1
	bne.b	nije_n
	bset.b	#0,flag			;postavi bit za opciju "-n"
	move.l	a0,a2
	bra.b	ima_li
nije_n	cmp.b	#'o',buff+1
	bne.b	nije_o
	bset.b	#1,flag			;postavi bit za opciju "-o"
	move.l	a0,a2
	move.l	#buff,a1		;
	addq.l	#2,a1			;
	moveq	#0,d0			;
	move.l	d0,d1			;prevodjenje ascii niza
	cmp.b	#$30,(a1)		; u broj
	beq.b	nista			;
rac_of	move.b	(a1)+,d0		;
	sub.b	#$30,d0			;
	add.l	d0,d1			;
	tst.l	(a1)			;
	beq.b	putoff			;
	mulu	#10,d1			;
	bra.b	rac_of			;
putoff	move.b	d1,offset
	bra.b	ima_li
nije_o	cmp.b	#'c',buff+1
	bne.b	nista
	bset.b	#2,flag			;postavi bit za opciju "-c"
	move.l	a0,a2
	lea	pon1,a3
	move.l	a3,case
	bra.w	ima_li
nista	move.l	a2,a0
	lea	ulaz,a1
	bsr.w	string			;procitaj ime ulaznog filea
	lea	izlaz,a1		;procitaj ime izlaznog filea
	bsr.w	string
;***********************************************************************
	jsr	output(a5)
	move.l	d0,d1
	move.l	#reading,d2
	moveq	#$0b,d3
	jsr	write(a5)		;write text "reading..."

	move.l	#$108,d0		;264 bajta za FileInfoBlock
	moveq	#00,d1
	jsr	allocmem(a6)
	tst.l	d0
	bne.b	l1
	bsr.w	memo
	bra.w	kraj2			;ako nema memorije, zavrsi

l1	move.l	d0,a4			;sacuvaj

	move.l	#ulaz,d1		;pointer na ime filea
	move.l	#-2,d2			;shared lock
	jsr	lock(a5)		;lock

	tst.l	d0
	bne.b	l2
	bsr.w	error11
	bra.w	kraj2			;problem sa ulaznim fileom

l2	move.l	d0,-(a7)		;sacuvaj
	move.l	d0,d1			;lock pointer
	move.l	a4,d2			;adresa u memoriji
	jsr	examine(a5)		;examine file

	tst.l	d0
	bne.b	l3
	bsr.w	error11

l3	move.l	$7c(a4),size		;spremi duzinu filea

	move.l	#$108,d0
	move.l	a4,a1
	jsr	freemem(a6)		;oslobodi memoriju 264 bajta

	move.l	(a7)+,d1		;vrati lock pointer
	jsr	unlock(a5)		;unlock
;-------------
	move.l	size,d0
	moveq	#00,d1
	jsr	allocmem(a6)		;alociraj prostor za file
	tst.l	d0
	bne.b	l4

	bsr.w	memo
	bra.w	dealloc4

l4	move.l	d0,a4			;spremi pointer
	move.l	d0,source
;-------------
	move.l	#ulaz,d1
	move.l	#1005,d2
	jsr	open(a5)		;open input file
	tst.l	d0
	bne.b	l5

	bra.w	error1			;ako je NULL pointer, zavrsi

l5	move.l	d0,-(a7)		;spremi file pointer

	move.l	d0,d1
	move.l	a4,d2
	move.l	size,d3
	jsr	read(a5)		;read input file
	tst.l	d0
	bne.b	l6

	bra.w	error1

l6	move.l	d0,length		;spremi duzinu procitanih bajta
	cmp.l	size,d0			;usporedi broj procitanih bajta sa
	bne.w	error1			;duzinom filea i po potrebi reagiraj

	move.l	(a7)+,d1		;vrati file pointer
	jsr	close(a5)		;close input file
;*************************************************************************
	clr.l	d5			;ocisti brojac linija
	move.l	length,d0		;vrati duzinu filea
	moveq	#10,d3			;LINE FEED
	btst.b	#0,flag			;koje citanje filea koristiti
	bne.b	bb

	bra.w	ba

	EVEN
ba 	subq.l	#1,d0			;smanji brojac znakova
	bmi.b	otislo			;da li smo procitali sve znakove
	cmp.b	(a4)+,d3		;da li je znak LINE FEED
	bne.b	ba			;skoci ako nije LF
	
	addq.l	#1,d5			;dodaj 1 broju linija	
	bra.b	ba
;-------------
bb
	moveq	#$20,d1			;SPACE
	bra.b	b21

b11	addq.l	#1,a4			;povecaj adresu
	subq.l	#1,d0			;smanji brojac znakova
	bmi.b	otislo			;zavrsi

b21	cmp.b	(a4),d1			;da li je SPACE ili manje
	bge.b	b11

bc 	subq.l	#1,d0			;smanji brojac znakova
	bmi.b	otislo
	cmp.b	(a4)+,d3		;da li je znak LINE FEED
	bne.b	bc			;skoci ako nije LF

	addq.l	#1,d5			;dodaj 1 broju linija
	bra.b	b21
;-------------
otislo
	moveq	#-1,d7
	cmp.b	(a4),d7			;ako zadnji red nema LF, nego samo
	bne.b	l61			; EOF, tada zabiljezi zadnji red
	addq.l	#1,d5			; kao jos jedan red vise

l61	move.l	d5,no_of_lines		;broj linija
	
	add.l	d5,d5
	add.l	d5,d5			;mnozi sa 4
	addq.l	#4,d5			;dodaj jos 4 bajta
	move.l	d5,d0
	move.l	d5,size2
	moveq	#00,d1
	jsr	allocmem(a6)		;alociraj prostor za polje pointera
	tst.l	d0
	bne.b	l7

	bsr.w	memo
	bra.w	dealloc4		;problemi sa memorijom

l7	move.l	d0,a3			;spremi pointer
	move.l	d0,pointers1
	move.l	source,a4
	
	btst.b	#0,flag
	bne.b	aa
	move.l	a4,(a3)+

aa	move.l	no_of_lines,d0
	add.l	d0,d0
	moveq	#00,d1			;alociraj prostor za duzine
	jsr	allocmem(a6)		;linija u bajtovima
	tst.l	d0
	bne.b	l8

	bsr.w	memo
	bra.w	dealloc3

l8	move.l	d0,pointers2
;-------------
	move.l	length,d0		;vrati duzinu linija
	move.l	pointers2,a2		;vrati pointer u registar
	moveq	#0,d2			;obrisi prvi brojac znakova u redu
	moveq	#10,d3			;LINE FEED
	btst.b	#0,flag			;koje citanje filea koristiti
	bne.b	read2

nije 	subq.l	#1,d0			;smanji brojac znakova
	bmi.b	nema			;ako smo procitali sve znakove
	addq.w	#1,d2			;brojac znakova + 1
	cmp.b	(a4)+,d3		;da li je znak LINE FEED
	bne.b	nije			;skoci ako nije LF

	move.w	d2,(a2)+		;spremi broj znakova u memoriju
	move.l	a4,(a3)+		;spremi slijedecu adresu kao pointer
	moveq	#0,d2			;obrisi brojac
	bra.b	nije
;-------------
read2	move.l	d0,d5
	moveq	#32,d1			;SPACE
	moveq	#01,d7
	bra.b	b2

era	moveq	#0,d2			;obrisi brojac znakova
	subq.l	#1,d5			;smanji duzinu filea
	bra.b	b2
		
b1	subq.l	#1,d0			;smanji brojac znakova
	bmi.b	nema2			;da li smo procitali sve znakove
	addq.l	#1,a4			;povecaj adresu
	subq.l	#1,d5			;smanji duzinu filea za 1

b2	cmp.b	(a4),d1			;da li je SPACE
	bge.b	b1			;skace ako je d1>=(a4)

	move.l	a4,a0			;sacuvaj pocetnu adresu

nije2 	subq.l	#1,d0			;smanji brojac znakova
	bmi.b	nema2			;da li smo procitali sve znakove
	addq.w	#1,d2			;brojac znakova + 1 (pomocni)
	cmp.b	(a4)+,d3		;da li je znak LINE FEED
	bne.b	nije2			;skoci ako nije LF

	cmp.w	d7,d2			;da li je linija duzine 1,tj. samo LF
	beq.b	era			;ako je, ignoriraj
	move.w	d2,(a2)+		;spremi broj znakova u memoriju
	move.l	a0,(a3)+		;spremi tekucu adresu kao pointer
	moveq	#0,d2			;obrisi brojac
	bra.b	b2

nema2	move.l	d5,length		;spremi novu duzinu filea
;**********************************************************************
nema	moveq	#-1,d7
	cmp.b	(a4),d7
	bne.b	l81
	move.w	d2,(a2)+
	move.l	a0,(a3)+

l81	jsr	output(a5)
	move.l	d0,d1
	move.l	#sorting,d2
	moveq	#$0f,d3
	jsr	write(a5)			;write text "sorting..."

	move.l	no_of_lines,a4
	move.l	a4,d0
		
	move.l	pointers1,a0
	move.l	pointers2,a3
	move.l	case,a5
	moveq	#0,d3
	btst.b	#1,flag
	beq.b	a

	move.b	offset,d3
	bra.b	a

;d0=x, d1=i, d2=j, a4=n

zamjena	
	movem.l	a1a2,a1/a2
	move.l	(a2),d4
	move.l	(a1),(a2)		;1. pointer na mjesto drugog
	move.l	d4,(a1)			;2. pointer na mjesto prvog

	move.l	d6,a1			;vrati adresu 1. operanda
	move.l	d7,a2			;vrati adresu 2. operanda
	move.w	(a2),d4			;zamijeni brojeve znakova u redu
	move.w	(a1),(a2)		;za dva reda koje mijenjas
	move.w	d4,(a1)
	bra.b	dalje

a	lsr.l	#1,d0			;x dijeli sa 2
	beq.w	gotovo			;ako je x=0, zavrsi

	move.l	d0,d1			;i=x
	subq.l	#1,d1			

b	addq.l	#1,d1			;i=i+1
	cmp.l	a4,d1			;da li je i=n
	beq.b	a			;ako je, skoci

	move.l	d1,d2			;j=i
dalje	sub.l	d0,d2			;j=i-x
	bmi.b	b			;ako je j<0, skoci

	move.l	a3,d6			;procitaj adrese duzina linija
	add.l	d2,d6
	add.l	d2,d6
	move.l	d6,d7
	add.l	d0,d7
	add.l	d0,d7

	add.l	d2,d2
	add.l	d2,d2			;d2=d2*4; za longove
	add.l	d0,d0
	add.l	d0,d0			;d0      -||-
	move.l	a0,a1			;a1=j - ti element
	add.l	d2,a1
	move.l	a1,a2
	add.l	d0,a2			;a2=j+x - ti element
	movem.l	a1/a2,a1a2		;spremi 1. i 2. pointer
	move.l	(a1),a1			;spremi adresu 1. operanda
	move.l	(a2),a2			;spremi adresu 2. operanda
	add.l	d3,a1
	add.l	d3,a2
	asr.l	#2,d2			;d2=d2/4
	asr.l	#2,d0			;d0=d0/4
	jmp	(a5)
;------------- ignorira vrstu slova
pon0	addq.l	#1,a1
	addq.l	#1,a2

pon1	move.b	(a1),d4
	move.b	(a2),d5
	cmp.b	d4,d5
	beq.b	pon0

pon11	cmp.b	#'A',d4			;da li je uopce slovo
	blt.b	pon2
	bset.l	#5,d4			;setiraj na malo slovo
	cmp.b	#'z',d4
	bgt.b	pon2

	cmp.b	#'A',d5			;da li je uopce slovo
	blt.b	pon2
	bset.l	#5,d5			;setiraj na malo slovo

pon2	cmp.b	d4,d5
	bmi.w	zamjena
	beq.b	pon0
	bra.w	b
;------------- NE ignorira vrstu slova
ponovo	cmpm.b	(a1)+,(a2)+		;uspredjuje dva znaka
	bmi.w	zamjena			;ako je 2<1, zamijeni ih
	beq.b	ponovo			;ako je 1=2, ponovo usporedi
	bra.w	b			;ako je 1<2, usporedi slijedece redove

;***********************************************************************
gotovo	move.l	dosbase,a5
	jsr	output(a5)

	move.l	d0,d1
	move.l	#writing,d2
	moveq	#$0f,d3
	jsr	write(a5)		;write text "writing..."

	move.l	size,d0
	moveq	#00,d1
	jsr	allocmem(a6)		;alociraj prostor za sortirani file
	tst.l	d0
	bne.w	l9

	bsr.w	memo
	bra.w	dealloc2

l9	move.l	d0,dest			;spremi pointer odredisne memorije

	move.l	pointers1,a3		;pointeri na linije
	move.l	d0,a2			;adresa prema kud kopiramo
	move.l	pointers2,a0		;adresa sa duzinama linija
	move.l	no_of_lines,d2		;adresa sa brojem linija

	move.w	#$fffc,d1		;maska za izvlacenje prvih 6 bita, tj.
					;za odredjivanje broja longova
	moveq	#3,d3			;3 bajta podataka i maska %00000011
	moveq	#10,d4			;LINE FEED	

jos	tst.l	d2			;da li je brojac linija = 0
	beq.b	next			;ako je, zavrsi
	move.l	(a3)+,a1		;stavi pointer aktualnog reda u a1
	move.w	(a0)+,d0		;procitaj duzinu linije
	subq.l	#1,d2			;smanji brojac linija za 1
	cmp.w	d3,d0			;uspredi sa 3
	ble.b	x			;ako je <= 3, skoci

long	move.w	d0,d6			;privremeno spremi
	and.w	d1,d0			;ocisti broj znakova sa maskom %11111100
	lsr.w	#2,d0			;i eliminiraj 2 zadnja desna bita

jos2	move.l	(a1)+,(a2)+		;kopiraj potrebni broj longova
	subq.w	#1,d0			;smanji broj longova za 1
	bne.b	jos2			;vrti petlju
	
	and.w	d3,d6			;sacuvaj dva desna bita
	beq.b	jos			;ako nema znakova, idi dalje

x	move.b	(a1),(a2)+		;iskopiraj znak
	move.b	(a1)+,d6		;buffer za aktualni bajt podatka
	cmp.b	d4,d6			;da li je znak=LINE FEED,
	bne.b	x			;nije, jos kopiraj
	bra.b	jos			;je, idi dalje

;***********************************************************************
next	move.l	#izlaz,d1
	move.l	#1006,d2
	jsr	open(a5)		;open output file
	tst.l	d0
	beq.w	error2			;output file error
;-------------
	move.l	d0,d1			;file pointer
	move.l	d0,-(a7)		;privremeno spremi
	move.l	dest,d2			;adresa
	move.l	length,d3		;duzina filea
	jsr	write(a5)		;write output file
;-------------
	move.l	(a7)+,d1
	jsr	close(a5)		;close output file
;-------------
	jsr	output(a5)
	move.l	d0,d1
	move.l	#done,d2		;obrisi poruke
	moveq	#$10,d3
	jsr	write(a5)
	
dealloc1
;-------------
	move.l	size,d0
	move.l	dest,a1
	jsr	freemem(a6)		;oslobodi dest memoriju
dealloc2
;-------------
	move.l	no_of_lines,d0
	move.l	pointers2,a1		;oslobodi prostor za duzine
	add.l	d0,d0
	jsr	freemem(a6)		;linija u bajtovima
dealloc3
;-------------
	move.l	size2,d0
	move.l	pointers1,a1
	jsr	freemem(a6)		;oslobodi prostor polja pointera
dealloc4
;-------------
	move.l	size,d0
	move.l	source,a1
	jsr	freemem(a6)		;oslobodi source memoriju
;-------------
kraj2	move.l	a5,a1
	jsr	closelibrary(a6)	;close dos.library
	movem.l	(a7)+,d1-d7/a1-a6
	moveq	#0,d0
	rts
;***********************************************************************
kraj	bsr.b	ekran			;pointer je u d1
	move.l	#logo,d2
	move.l	#$160,d3
	jsr	write(a5)		;write text poruka

	bra.b	kraj2

error1	bsr.b	ekran
	move.l	#greska1,d2
	moveq	#22,d3
	jsr	write(a5)		;write text input error

	bra.b	dealloc4

error2	bsr.b	ekran
	move.l	#greska2,d2
	moveq	#22,d3
	jsr	write(a5)		;write text output error

	bra.w	dealloc1

;-------------
ekran	jsr	output(a5)
	tst.l	d0
	beq.b	.ekr2
	move.l	d0,d1
	rts

.ekr2	move.l	(a7)+,d0
	jmp	kraj2

;-------------
error11	move	#$108,d0		;kod rada sa lock-om
	move.l	a4,a1
	jsr	freemem(a6)

memo	bsr.b	ekran
	move.l	#greska3,d2
	moveq	#20,d3
	jsr	write(a5)
	rts

string	movem.l	d1-d3/a1,-(a7)		;pocetnu adresu i pomocne registre
					;privremeno spremi na stack
	moveq	#10,d1			;LINE FEED
	moveq	#32,d2			;SPACE
	clr.b	d0			;obrisi brojac
.a	move.b	(a0),d3
	cmp.b	d2,d3			;da li je znak SPACE
	beq.b	.b
	cmp.b	d1,d3			;da li je znak LINE FEED
	beq.b	.c
	tst.l	d3			;da li je znak NULL
	beq.b	.c
	move.b	(a0)+,(a1)+		;kopiraj znak
	addq.b	#1,d0			;dodaj 1 broju procitanih znakova
	bra.b	.a
.b	addq.l	#1,a0			;povecaj adresu
	cmp.b	(a0),d2			;da li je jos SPACE
	beq.b	.b			;ako nije, stani
.c	clr.b	(a1)			;na kraj stavi NULL
	addq.l	#1,d0			;dodaj 1 broju procitanih znakova
	movem.l	(a7)+,d1-d3/a1		;vrati pocetnu adresu sa stacka
					;zajedno sa koristenim registrima
	rts

;------------------------------------------------------------------------------	
doslib	dc.b	"dos.library",0
ulaz	ds.b	100
izlaz	ds.b	100
buff	ds.b	100
logo	dc.b	10,"Shellsort V1.14 ",10					 ;18
	dc.b	"   Made by ONE MAN GANG ",10,10				 ;26
	dc.b	"E-mail: drazen.herendic@etf.hr",10,10				 ;32
	dc.b	"USAGE: Shellsort [-n] [-oX] [-c] input_file output_file ",10,10 ;58
	dc.b	"  -n leading spaces and control characters are ignored ",10	 ;56
	dc.b	"     (tabs, line feeds, form feeds, etc.) ",10,0		 ;44
	dc.b	"  -oX X is offset from beginning of line (new reference) ",10	 ;58
	dc.b	"  -c case insensitive sorting (default is case sensitive) ",10,0 ;60
;		 1---5----0----5----0----5----0----5----0----5----0
reading	dc.b	"Reading...",0,0
sorting	dc.b	$9b,$31,$30,$44,"Sorting...",0,0
writing	dc.b	$9b,$31,$30,$44,"Writing...",0,0
done	dc.b	$9b,$31,$30,$44,"Done !    ",10,0
greska1	dc.b	10,"Input file error ! ",10,0
greska2	dc.b	10,"Output file error !",10,0
greska3	dc.b	10,"Memory problems !",10,0
version	dc.b	"$VER: shellsort 1.14 (14.7.94)"
	even
a1a2	ds.b	8
case	dc.l	0
dosbase	dc.l	0			;samo ime kaze sve
length	dc.l	0			;duzina ucitanog filea
source	dc.l	0			;adresa gdje se ucitava file
dest	dc.l	0			;adresa gdje se sprema sortirani file
pointers1	dc.l	0		;polje pointera za adrese linija
pointers2	dc.l	0		;pointer za duzine linija u bajtima
no_of_lines	dc.l	0		;broj linija, tj. pointera
flag	dc.b	0
	even
size	dc.l	0
size2	dc.l	0
offset	dc.b	0
	even
