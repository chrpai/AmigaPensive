/* Chat-Door for Excelsior! */

OPTIONS RESULTS

/* Change pstep for longer or shorter paging-time */

pstep = 30
ptime = pstep*60

call addlib("apig.library",0,-30,0)
call addlib("rexxsupport.library",0,-30,0)

call init

pname = "chatport"

p = openport(pname)

call set_apig_globals()

/* E!-part starts here... */

print ' '
do t = 1 to 6
	print text.t
end

sendstring "[4A\c3|\c7"
receive
reason = result

len = length(reason)

if (len>70) then do
		print '0d0a0d0a'x||'\c2Reason to long! (max. 70 Chars)'
		exit
		end

if reason = '' then do 
	print '0d0a0d0a'x||'\c2NO Reason NO Chat !'
	exit
	end

/* Intuition Part starts here */

TITLE = "E!-Page1.2 by ACP-Development in 02/95"
IDCMP = VANILLAKEY
FLAGS = GIMMEZEROZERO+WINDOWDEPTH+WINDOWDRAG

scr = 0

buf = allocmem(200,MEMF_CLEAR)
call getscreendata(buf,200,1,0)

xr = getvalue(buf,68,2,'n')
yr = getvalue(buf,70,2,'n')

ws = len*9+108

if ws<320 then ws = 320

xp = xr%2-ws%2
yp = yr%2-50

win = openwindow(pname,xp,yp,ws,100,0,0,IDCMP,FLAGS,TITLE,scr,1,0,0)

call writeconsole(win,'9b302070'x)

fp = maketattr(win,'pearl.font',8)
call openfont(fp)

rp = getwindowrastport(win)

call activatewindow(win)

x= screentofront(0)

getuser 1
name = result

outs.1 = name||' is paging you!'
outs.2 = 'Chat-Reason: '||reason
outs.3 = '... Press any key to answer Chat-Request ...'

sp = ws%2-9*(length(outs.3)%2)

xk.1 = 0 ; yk.1 = 20
xk.2 = 18 ; yk.2 = 20+16
xk.3 = 18+sp ; yk.3 = 20+16*2

ln = length(outs.1)
xk.1 = xk.1 + ws%2-9*ln%2
ln = length(outs.2)
xk.2 = xk.2 + ws%2-9*ln%2

do t = 1 to sp
	outs.5 = ' '||outs.5
	end

do t = 1 to 3
        pitext(rp,xk.t,yk.t,outs.t,1,0,1,fp)
end

print "[2B"

print ' '
print '\c3Press any key to quit paging'||'0d0a'x
sendstring '\c2Paging '

logentry 'Chat request by '||name||' .... Reason: '||reason

changewhere reason
changewhat 'Chat-Request'

ex = 0
vk = 0

do t = 0 to ptime
	if t//pstep = 0 then sendstring '.'
	checkio
	if result = true then ex = 1
	msg = getpkt(pname)
	if ex = 1 then leave
	if msg = '00000000'x then iterate
	class = getarg(msg,0)
	if class = VANILLAKEY then vk = 1
	x = reply(msg,0)
	if vk = 1|ex = 1 then leave
end

print '0d0a0d0a'x


if vk = 1 then do 
	print 'The SYSOP is going to enter the Chat-Mode soon!'
	openscreen
	end
else if ex = 0 then print 'The SYSOP seems not to be available...'
	else print 'Aborted ... User break'

x = closewindow(win)

bufferflush

call freemem(buf,200)


init:
text.1 = '\c3+-------------< \c1Enter a reason for Chat (max. 70 Chars) \c3>----------------+'
text.2 = '\c3+------------------------------------------------------------------------+'
text.3 = '\c3|                                                                        |'
text.4 = '\c3+------------------------------------------------------------------------+'
text.5 = '\c6  --------- \c8\c4 E!-Page1.2 - Coded by the \c6ACP-Development\c8 \c4in 2/95 \c6 -------- '
text.6 = '\c3   Amiga Crackpoint --- \c2*\c3 --- +43 2252 46519\c3 --- \c2*\c3 --- 2 Lines USR V.34'
