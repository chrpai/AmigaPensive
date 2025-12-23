/*        $VER: 1.1 Palette.rexx 30 Sep 1992 (30.9.92)
    copyright 1991 Richard Lee Stockton and Gramma Software.
      FREELY DISTRIBUTABLE as long as this notice remains

USAGE: rx Palette [public_screen] [colors] [replyport]
      defaults to Workbench with 4 colors, no replyport

ASYNCH example:  ADDRESS AREXX Palette MY8COLORSCREEN 8 MYPORT

  Palette.rexx will return 5 if the user selects "USE" and if
  Palette.rexx was given a replyport name, the message 'NEW_COLORS'
  is sent to the replyport. Otherwise no message is sent and the
  function returns 0.


WARNING! There is no way to check to see if colors is the correct
          number for a particular screen.
         Colors *MUST* be either 2, 4, 8, 16, or 32.

        requires these external libraries:
                            arp.library
                    rexxsupport.library
                     rexxarplib.library
                    screenshare.library


NOTE! Palette.comments contains the same program, but heavily commented

*/

IF ~SHOW('L','rexxsupport.library') THEN
  CALL ADDLIB('rexxsupport.library',0,-30,0)
IF ~SHOW('L','rexxarplib.library') THEN
  CALL ADDLIB('rexxarplib.library',0,-30,0)
CALL getversions()

PARSE ARG pscreen' 'colors' 'replyport .
IF colors~=2 & colors~=8 & colors~=16 & colors~=32 THEN colors=4
IF pscreen='' THEN pscreen='Workbench'
IF pscreen='Workbench' & ksversion<37 THEN
  CALL ALL_DONE('Can not open Palette on pre-2.0 WorkBench!')

host='PALETTEHOST'
port='PALETTEPORT'
IF replyport='' THEN
  DO
    DO i=1 WHILE SHOW('P',host||i)
    END
    host=host||i
    port=port||i
  END
ELSE
  DO
    host=host'.'replyport
    port=port'.'replyport
    IF SHOW('P',port) THEN
      DO
        INTERPRET ADDRESS port FRONT
        EXIT(0)
      END
  END

CALL setup_variables()
CALL setup_host()
w.=''
w.idcmp='CLOSEWINDOW+MENUPICK+GADGETDOWN+GADGETUP+MOUSEBUTTONS'
w.flags='WINDOWCLOSE+WINDOWDRAG'
w.title=' ARexx Color Palette           '
IF replyport~='' THEN
  DO
    t=LENGTH(replyport)
    w.title=' 'LEFT(replyport,t-4)' Colors          '
  END
xmax=218
ymax=126
CALL OpenWindow(host,(ScreenCols(pscreen)-xmax)%2, ,
                     (ScreenRows(pscreen)-ymax)%2, ,
                     xmax,ymax,w.idcmp,w.flags,w.title)
CALL SetFont(host,'topaz.font',8)
p1=1
p2=2
IF ksversion<37 THEN
  DO
    p1=2
    p2=1
    CALL SetReqColor(host,'OKAYPEN',1)
  END
CALL ModifyHost(host,MOUSEBUTTONS,"%b %x %y")

CALL AddMenu(host,'ARexx Palette')
CALL AddItem(host,'Use     ','OK','U')
CALL AddItem(host,'Reset   ','RESET','R')
CALL AddItem(host,'About   ','ABOUT')
CALL AddItem(host,'Quit    ','CLOSEWINDOW','Q')

CALL AddGadget(host,15,26,1,'<','%l 1 -1')
CALL AddGadget(host,51,26,2,'>','%l 1 1')
CALL AddGadget(host,85,26,3,'<','%l 2 -1')
CALL AddGadget(host,121,26,4,'>','%l 2 1')
CALL AddGadget(host,155,26,5,'<','%l 3 -1')
CALL AddGadget(host,191,26,6,'>','%l 3 1')

CALL SetAPen(host,1)
CALL Move(host,27,22)
CALL Text(host,'Red')
CALL Move(host,89,22)
CALL Text(host,'Green')
CALL Move(host,163,22)
CALL Text(host,'Blue')

DO i=1 TO 3
  CALL box(host,p1,p1,8+(i-1)*70,13,61,26)
END
CALL box(host,p1,p2,53,42,108,10)
CALL box(host,p1,p1,15,55,186,50)
CALL read_colors()

CALL AddGadget(host,11,ymax-16,98,' USE ','OK')
CALL AddGadget(host,xmax%2-24,ymax-16,99,'RESET','RESET')
CALL AddGadget(host,xmax-64,ymax-16,99,'CANCEL','CLOSEWINDOW')
CALL tofront()

keep_going=1
DO WHILE keep_going=1
  t=WAITPKT(port)
  DO ff=1
    p=GETPKT(port)
    IF p='0000 0000'x THEN LEAVE ff    /* message port empty */
    command=GETARG(p)
    t=REPLY(p,0)
    IF keep_going=0 THEN ITERATE ff
    PARSE VAR command arg1' 'arg2' 'arg3' ' 
    SELECT
      WHEN arg1='CLOSEWINDOW'  THEN keep_going=0
      WHEN arg1='RESET'        THEN CALL reset_colors()
      WHEN arg1='OK'           THEN CALL do_ok()
      WHEN arg1='FRONT'        THEN CALL tofront()
      WHEN arg1='GADGETDOWN'   THEN CALL gadgetdown(arg2 arg3)
      WHEN arg1='SELECTDOWN'   THEN CALL selectdown(arg2 arg3)
      WHEN arg1='ABOUT'        THEN CALL Request(,,copyright,,,,pscreen)
      WHEN arg1='GADGETUP'     THEN NOP
      WHEN arg1='SELECTUP'     THEN NOP
      WHEN arg1='CONTINUE'     THEN NOP
      OTHERWISE CALL REQUEST(,100,arg1 arg2 arg3,,,,pscreen)
    END
  END
END
CALL ALL_DONE('RESET')
EXIT(0)




/* Functions */

ALL_DONE:
  PARSE ARG air
  changed=0
  CALL PostMsg()
  IF air='RESET' THEN CALL reset_colors()
  ELSE IF air='NEW_COLORS' THEN changed=5
  ELSE IF air~='' THEN
    DO
      CALL usermsg(air)
      CALL waiting()
    END
  CALL clearport(port)
  IF SHOW('P',host) THEN CALL Stop(host)
  EXIT(changed)
RETURN


clearport:
PARSE ARG portname
p=1
DO FOREVER
  p=GETPKT(portname)
  IF p='0000 0000'x THEN RETURN
  t=REPLY(p,0)
END
RETURN


do_ok:
  IF replyport~='' THEN
    IF SHOWLIST('P',replyport) THEN
      INTERPRET ADDRESS replyport 'NEW_COLORS'
  CALL ALL_DONE('NEW_COLORS')
RETURN


tofront:
  CALL ActivateWindow(host)
  CALL WindowToFront(host)
  CALL ScreenToFront(pscreen)
RETURN


read_colors:
  colors.=''
  box_x=92
  box_y=48
  IF colors>8 THEN
    DO
      box_y=12
      box_x=46
      IF colors=32 THEN box_x=23
    END
  ELSE IF colors>2 THEN
    DO
      box_y=24
      IF colors=8 THEN box_x=46
    END
  box_cols=184%box_x
  box_rows=48%box_y
  DO i=0 TO colors-1
    colors.i=ScreenColor(pscreen,i)
    CALL SetAPen(host,i)
    CALL RectFill(host,16+(i//box_cols)*box_x,56+(i%box_cols)*box_y,16+box_x+(i//box_cols)*box_x,56+box_y+(i%box_cols)*box_y)
  END
  CALL SetAPen(host,1)


reset_colors:
  DO i=0 TO colors-1
    DO j=1 TO 3
      colors.i.j=WORD(colors.i,j)%1
    END
  END


set_colors:
  DO i=0 TO colors-1
    CALL ScreenColor(pscreen,i,colors.i.1,colors.i.2,colors.i.3)
  END


update_colors:
  register=register%1
  CALL Move(host,22,50)
  CALL Text(host,RIGHT(register,2))
  CALL Move(host,xmax-44,50)
  CALL Text(host,d2x(colors.register.1)||d2x(colors.register.2)||d2x(colors.register.3))
  DO i=1 TO 3
    CALL Move(host,31+(i-1)*70,33)
    CALL Text(host,right(colors.register.i%1,2))
  END
  CALL SetAPen(host,register)
  CALL RectFill(host,54,43,160,51)
  CALL SetAPen(host,1)
RETURN


gadgetdown:
  PARSE ARG rgb updown .
  DO icount=1
    colors.register.rgb=colors.register.rgb+updown
    IF colors.register.rgb<0 THEN colors.register.rgb=15
    IF colors.register.rgb>15 THEN colors.register.rgb=0
    CALL ScreenColor(pscreen,register,colors.register.1,colors.register.2,colors.register.3)
    CALL update_colors()
    IF cpu>68000 THEN CALL DELAY(2)
    p=GETPKT(port)
    IF p~='0000 0000'x THEN
      DO
        arg1=GETARG(p)
        t=REPLY(p,0)
        PARSE VAR arg1 arg1 .
        IF arg1="GADGETUP" | arg1="SELECTUP" | arg1='MOUSEBUTTONS' THEN
          LEAVE icount
      END
  END
RETURN


selectdown:
  IF arg2<14 | arg2>198 | arg3<55 | arg3>103 THEN RETURN
  mx=(arg2-14)%box_x
  my=(arg3-55)%box_y
  IF mx>=box_cols THEN mx=box_cols-1
  IF my>=box_rows THEN my=box_rows-1
  register=mx+my*box_cols
  CALL update_colors()
RETURN


box:
  ARG boxhost,pen1,pen2,upleft,uptop,width,height
  CALL SetAPen(boxhost,pen2)
  CALL Move(boxhost,upleft+width+1,uptop)
  CALL Draw(boxhost,upleft+width+1,uptop+height)
  CALL Draw(boxhost,upleft-1,uptop+height)
  CALL Move(boxhost,upleft+width,uptop+1)
  CALL Draw(boxhost,upleft+width,uptop+height)
  CALL SetAPen(boxhost,pen1)
  CALL Move(boxhost,upleft,uptop)
  CALL Draw(boxhost,upleft+width,uptop)
  CALL Move(boxhost,upleft,uptop+height-1)
  CALL Draw(boxhost,upleft,uptop)
  CALL Move(boxhost,upleft-1,uptop)
  CALL Draw(boxhost,upleft-1,uptop+height)
RETURN


setup_host:
  CALL OPENPORT(port)
  ADDRESS AREXX "'x=CreateHost("host","port","pscreen")'"
  DO 200 WHILE ~SHOW('Ports',host)
    CALL DELAY 10  /* 200 ms */
  END
  IF ~SHOW('Ports',host) THEN
    CALL ALL_DONE('Could not open host 'host'.')
  IF ~SHOW('Ports',port) THEN
    CALL ALL_DONE('Could not open port 'port'.')
RETURN


usermsg:
  PARSE ARG umsg
  CALL PostMsg()
  CALL PostMsg(0,160,umsg,pscreen)
RETURN


waiting:
  CALL DELAY(200)
  CALL PostMsg()
RETURN


setup_variables:
  register=0
  x=SOURCELINE(1)
  copyright=''
  DO i=3 TO 7
    copyright=copyright WORD(x,i)
  END
  copyright=CENTER(STRIP(copyright),32)'\\
  © 1991 Richard Lee Stockton\'CENTER('and',32)'\
    Gramma Software Systems\
17730-15th Avenue NE, Suite 223\
     Seattle WA 98155-3804\
    Office: (206) 363-6417\
       FAX:       361-0429\
       BBS:       744-1254\
      Tech:       776-1253\\
      FREELY DISTRIBUTABLE'
RETURN


getversions:
  ADDRESS COMMAND 'version >RAM:VERSION'
  x=OPEN(f,'RAM:VERSION','R')
  line=READLN(f)
  CALL CLOSE(f)
  CALL DELETE('RAM:VERSION')
  ksversion=STRIP(WORD(line,3))
  PARSE VERSION . . cpu .
RETURN
  

/* Palette.rexx */
