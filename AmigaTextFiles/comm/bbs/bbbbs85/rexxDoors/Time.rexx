/* Time.rexx         by Greg Cunningham 1990    stolen from Unix TODAY.C */

CR='0D'x
temp=date(o)
parse var temp year'/'mon'/'day .
say 'Today is' date(w)', the' nbrtxt(day%1,1)' day of' date(m)',' nbrtxt(19,0) nbrtxt(year,0)'.'CR

ctime=time()
parse var ctime hour':'min':'sec
hr=hour//12
if(hr=0) then hr=12
str='The big hand is on the' nbrtxt((((min+2+sec%30)%5+11)//12)+1,0)
say str 'and the little hand is on the' nbrtxt(hr,0)'.'CR

exit


/*
 * Integer to Readable ASCII Conversion Routine.
 *
 * Synopsis:
 *
 * string=nbrtxt(value,ordinal)
 *	value	-- The number to output
 *	ordinal -- Non-zero for ordinal number
 *
 *
 *     say nbrtxt(0,0) ==> "zero"
 *     say nbrtxt(0,1) ==> "zeroth"
 *
 * Largest number:
 *	nine hundred ninety-nine thousand, nine hundred ninety-nine
 *
 */

nbrtxt: procedure

	arg val,ordflag

	cardinal.0='zero'
	cardinal.1='one'
	cardinal.2='two'
	cardinal.3='three'
	cardinal.4='four'
	cardinal.5='five'
	cardinal.6='six'
	cardinal.7='seven'
	cardinal.8='eight'
	cardinal.9='nine'
	cardinal.10='ten'
	cardinal.11='eleven'
	cardinal.12='twelve'
	cardinal.13='thirteen'
	cardinal.14='fourteen'
	cardinal.15='fifteen'
	cardinal.16='sixteen'
	cardinal.17='seventeen'
	cardinal.18='eighteen'
	cardinal.19='nineteen'

	ordinal.0='zeroth'
	ordinal.1='first'
	ordinal.2='second'
	ordinal.3='third'
	ordinal.4='fourth'
	ordinal.5='fifth'
	ordinal.6='sixth'
	ordinal.7='seventh'
	ordinal.8='eighth'
	ordinal.9='ninth'
	ordinal.10='tenth'
	ordinal.11='eleventh'
	ordinal.12='twelfth'

	twenties.2='twen'
	twenties.3='thir'
	twenties.4='for'
	twenties.5='fif'
	twenties.6='six'
	twenties.7='seven'
	twenties.8='eigh'
	twenties.9='nine'

	op=''
	if(val<0) then do
		op = 'minus '
		val=(-val);
	end
	if(val>=1000) then do
		op=op||nbrtxt(val%1000,0);
		op=op 'thousand'
		val=val//1000
		if(val=0) then do
			if(ordflag=1) then op=op'th'
			return op
		end
		if(val>=100) then op=op', '
		else op=op 'and '
	end
	if(val>=100) then do
		num=val%100
		op=op||cardinal.num 'hundred'
		val=val//100
		if(val=0) then do
			if(ordflag=1) then op=op'th'
			return op
		end
		op=op' '
	end
	if(val>=20) then do
		if(val=90 & ordflag=1) then return op'nintieth' /* ninetieth? */
		num=val%10
		op=op||twenties.num
		val=val//10
		if(val=0) then do
			if(ordflag=1) then op=op'tieth'
			else op=op'ty'
			return op
		end
		op=op'ty-'
	end
	if(val<=12) then do
		if(ordflag=1) then op=op||ordinal.val
		else op=op||cardinal.val
		return op
	end
	op=op||cardinal.val
	if(ordflag=1) then op=op'th'
	return op
