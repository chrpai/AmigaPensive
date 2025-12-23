; Character set conversion routine

; void ConvString(__A0 UBYTE *src, __A1 UBYTE *dst, __A2 UBYTE *conv);

		xdef	@ConvString
		xdef	@CRtoLF
		xdef	@LFtoCR

		section	text,code

@ConvString	moveq	#0,d0
cnvloop		move.b	(a0)+,d0
		beq	nomore
		move.b	0(a2,d0.w),d0
		beq	cnvloop
		move.b	d0,(a1)+
		bra	cnvloop
nomore		clr.b	(a1)
cnomore		rts

@CRtoLF		move.b	(a0)+,d0
		beq	cnomore
		cmpi.b	#13,d0
		bne	@CRtoLF
		move.b	#10,-1(a0)
		bra	@CRtoLF

@LFtoCR		move.b	(a0)+,d0
		beq	cnomore
		cmpi.b	#10,d0
		bne	@LFtoCR
		move.b	#13,-1(a0)
		bra	@LFtoCR

		end
