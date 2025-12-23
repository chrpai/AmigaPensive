/*
** $VER: E!Mines.rexx v1.5 (Unregistered) (24.12.96) (c) Inorix scripts
**
** The Excelsior! BBS Mines Doorgame!
**
** The registered version has:
** ---------------------------
** - Easy changeing of field x and y size
** - 3 difficulty levels
** - Seperate highscores for every combination! (:
** - Better graphics
** - Free updates
*/

OPTIONS RESULTS

IF ~SHOW('L','rexxsupport.library') THEN DO
  IF ~ADDLIB('rexxsupport.library',0,-30) THEN DO
    transmit '\c1Error! Missing library!'
    sysoplog 'Missing rexxsupport.library!'
    EXIT
  END
END

DataPath='RAM:'
IF EXISTS('ENV:InorixDataPath') THEN DO
  CALL OPEN(dump,'ENV:InorixDataPath',R)
  DataPath=READLN(dump)
  IF RIGHT(DataPath,1)~=':' & RIGHT(DataPath,1)~='/' THEN DataPath=DataPath'/'
  CALL CLOSE(dump)
END
TR=TRANSMIT
GETUSER 1
UAlias=RESULT
UAlias=SPACE(UAlias,1,'_')
BBSIDENTIFY TERM
ULines=WORD(RESULT,2)
IF ULines<32 THEN SETLENGTH 32
FLASH='07'x
ESC='1b'x
Col.0=ESC'[30m'
Col.1=ESC'[31m'
Col.2=ESC'[32m'
Col.3=ESC'[33m'
Col.4=ESC'[34m'
Col.5=ESC'[35m'
Col.6=ESC'[36m'
Col.7=ESC'[37m'
CALL ReadHiScores
DO FOREVER
  BUFFERFLUSH
  CLS
  TR '\n5\c7            ษอออออออออออออออออออออออออออออออออออออออออออออออออออออป'
  TR '            บ \c3E!Mines v1.5 (Unregistered) \c1(c) 1996 \c2Inorix scripts \c7บ'
  TR '            ศอออัอออออออออออออออออออออออออออออออออออออออออออออัอออผ'
  TR '                ณ    \c6User : \c2'LEFT(UAlias,20)'              \c7ณ'
  TR '                ณ \c6Topscore: \c3'list.1' \c7ณ'
  TR '                ภฤฤฤฤฤฤฤฤฤฤฤฤฤาฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤาฤฤฤฤฤฤฤฤฤฤฤฤู'
  TR '                              บ \c61\c7) \c3Start E!Mines \c7บ'
  TR '                              บ \c62\c7) \c3Instructions  \c7บ'
  TR '                              บ \c63\c7) \c3High scores   \c7บ'
  TR '                              บ \c64\c7) \c3Quit          \c7บ'
  TR '                        ษอออออสออออออออออออออออออสออออป'
  TR '                        บ \c1Cyborg BBS - +31-33-4804065 \c7บ'
  TR '                        ศอออออออออออออออออออออออออออออผ'
  Choice=WaitKey()
  SELECT
    WHEN Choice='1' THEN DO
      CALL SetUp
      CALL Showfield
      CALL GameLoop
    END
    WHEN Choice='2' THEN DO
      CALL Instructions
    END
    WHEN Choice='3' THEN DO
      CALL ReadHiScores
      CLS
      TR '\n4\c3                        ษออออออออออออออออออออออออออออออป'
      TR '                        บ \c6The 10 fastest minesweepers: \c3บ'
      TR '                     ีออสออออออออออออออออออออออออออออออสออธ'
      DO a=1 TO 10
        TR '\c3                     ณ'RIGHT(a,2)' \c6'list.a'\c3ณ'
      END
      TR '                     ภฤฤฤฤยฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤยฤฤฤฤฤู'
      TR '                          ณ \c2Hit any key to continue!\c3ณ'
      TR '                          ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู'
      CALL WaitKey()
    END
    WHEN Choice='4' | Choice='Q' THEN DO
      LEAVE
    END
    OTHERWISE NOP
  END
END
CLS
SEND APos(20,4)||Esc'[33;44mษอออออออออออออออออออออออออออออป'Apos(20,5)'บ    Thank you for playing    บ'APos(20,6)'บ'Esc'[31m E!Mines v1.5 (Unregistered) 'Esc'[33mบ'APos(20,7)'ศอออออออออออออออออออออออออออออผ'
TR '\w2'
SEND APos(25,5)||Esc'[31;42mฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ'APos(25,6)'ณ'Esc'[33m An Inorix scripts doorgame'Esc'[31m ณ'Apos(25,7)'ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู'
TR '\w2'
SEND APos(15,6)||Esc'[37;44mษออออออออออออออออออออออออออออออออป'APos(15,7)'บ Ask your sysop to register it! บ'APos(15,8)'บ So you don''t have to wait...   บ'APos(15,9)'ศออออออออออออออออออออออออออออออออผ'
TR '\w3'
SEND APos(35,9)||Esc'[34;46mษอออออออออออออออออออออออออออออป'APos(35,10)'บ For all these messages! :-) บ'APos(35,11)'ศอออออออออออออออออออออออออออออผ'APos(0,12)||Esc'[0m'
TR '\w3'
SETLENGTH ULines
EXIT

SetUp:
  SEND APos(27,17)||col.2'   Creating minefield...   'APos(0,19)
  DO a=1 TO 10
    DO b=1 TO 10
      field.b.a.0='?'
      field.b.a.1='-'
    END
  END
  DO a=1 TO 18
    hit=0
    DO UNTIL hit=1
      x=QRnd(1,10)
      y=QRnd(1,10)
      IF field.x.y.1~='*' THEN DO
        hit=1
        field.x.y.1='*'
      END
    END
  END
  DO y=1 TO 10
    DO x=1 to 10
      x1=x-1
      x2=x
      x3=x+1
      y1=y-1
      y2=y
      y3=y+1
      count=0
      IF field.x1.y1.1='*' THEN count=count+1
      IF field.x2.y1.1='*' THEN count=count+1
      IF field.x3.y1.1='*' THEN count=count+1
      IF field.x1.y2.1='*' THEN count=count+1
      IF field.x3.y2.1='*' THEN count=count+1
      IF field.x1.y3.1='*' THEN count=count+1
      IF field.x2.y3.1='*' THEN count=count+1
      IF field.x3.y3.1='*' THEN count=count+1
      IF field.x2.y2.1='*' THEN count='*'
      IF count=0 THEN count=' '
      field.x.y.1=count
    END
  END
RETURN

Showfield:
  CLS
  TR '\n2'
  TR col.3'             +---+---+---+---+---+---+---+---+---+---+'
  DO a=1 TO 10
    SEND '            '
    DO b=1 TO 10
      SEND col.3' | 'col.7'?'
    END
    TR col.3' |'
    TR col.3'             +---+---+---+---+---+---+---+---+---+---+'
  END
RETURN

GameLoop:
  BeginTime=TIME(S)
  GameTime=0
  quit=0
  lose=0
  win=0
  xpos=15
  ypos=5
  flagsset=0
  flagsleft=18
  SEND col.1||APos(xpos,ypos)'('Apos(xpos+2,ypos)')'APos(0,0)
  SEND APos(65,2)||col.7'Flags left: 'col.2||RIGHT(flagsleft,2)||APos(65,3)||col.7' Flags set: 'col.2||RIGHT(flagsset,2)||APos(0,0)
  DO WHILE quit=0
    key=Waitkey()
    IF key='1B'x THEN DO
      GETCHAR
      key=Waitkey()
      IF key='A' THEN key='8'
      IF key='B' THEN key='2'
      IF key='C' THEN key='6'
      IF key='D' THEN key='4'
    END
    oldx=xpos
    oldy=ypos
    niets=0
    show=0
    SELECT
      WHEN UPPER(key)='Q' THEN quit=1
      WHEN key='2' & ypos<23 THEN ypos=ypos+2
      WHEN key='4' & xpos>15 THEN xpos=xpos-4
      WHEN key='6' & xpos<51 THEN xpos=xpos+4
      WHEN key='8' & ypos>5 THEN ypos=ypos-2
      WHEN key=' ' | key='0d'x THEN show=1
      WHEN key='0' | key='.' | key='Z' THEN show=2
      OTHERWISE niets=1
    END
    IF niets=0 & quit=0 THEN DO
      SEND APos(oldx,oldy)||col.1' 'APos(oldx+2,oldy)' 'APos(xpos,ypos)'('Apos(xpos+2,ypos)')'APos(0,0)
    END
    IF show=1 & quit=0 THEN DO
      x=(xpos-11)/4
      y=(ypos-3)/2
      SEND APos(xpos+1,ypos)||col.6||field.x.y.1||Apos(0,0)
      IF field.x.y.0='#' THEN DO
        flagsset=flagsset-1
        flagsleft=flagsleft+1
      END
      field.x.y.0=field.x.y.1
      SELECT
        WHEN field.x.y.1='*' THEN DO
          BUFFERFLUSH
          SEND APos(26,13)'                 '
          SEND APos(26,14)||FLASH||col.1' KABOOOOOOOM!!!! '
          SEND APos(26,15)'                 'APos(0,0)
          lose=1
          quit=1
        END
        WHEN field.x.y.1=' ' THEN DO
          x1=x-1
          x2=x
          x3=x+1
          y1=y-1
          y2=y
          y3=y+1
          IF y1>0 THEN DO
            IF x1>0 THEN DO
              SEND APos(xpos-3,ypos-2)||col.6||field.x1.y1.1
              IF field.x1.y1.0='#' THEN DO
                flagsset=flagsset-1
                flagsleft=flagsleft+1
              END
              field.x1.y1.0=field.x1.y1.1
            END
            SEND APos(xpos+1,ypos-2)||col.6||field.x2.y1.1
            IF field.x2.y1.0='#' THEN DO
              flagsset=flagsset-1
              flagsleft=flagsleft+1
            END
            field.x2.y1.0=field.x2.y1.1
            IF x3<11 THEN DO
              SEND APos(xpos+5,ypos-2)||col.6||field.x3.y1.1
              IF field.x3.y1.0='#' THEN DO
                flagsset=flagsset-1
                flagsleft=flagsleft+1
              END
              field.x3.y1.0=field.x3.y1.1
            END
          END
          IF x1>0 THEN DO
            SEND APos(xpos-3,ypos)||col.6||field.x1.y2.1
            IF field.x1.y2.0='#' THEN DO
              flagsset=flagsset-1
              flagsleft=flagsleft+1
            END
            field.x1.y2.0=field.x1.y2.1
          END
          IF x3<11 THEN DO
            SEND APos(xpos+5,ypos)||col.6||field.x3.y2.1
            IF field.x3.y2.0='#' THEN DO
              flagsset=flagsset-1
              flagsleft=flagsleft+1
            END
            field.x3.y2.0=field.x3.y2.1
          END
          IF y3<11 THEN DO
            IF x1>0 THEN DO
              SEND APos(xpos-3,ypos+2)||col.6||field.x1.y3.1
              IF field.x1.y3.0='#' THEN DO
                flagsset=flagsset-1
                flagsleft=flagsleft+1
              END
              field.x1.y3.0=field.x1.y3.1
            END
            SEND APos(xpos+1,ypos+2)||col.6||field.x2.y3.1
            IF field.x2.y3.0='#' THEN DO
              flagsset=flagsset-1
              flagsleft=flagsleft+1
            END
            field.x2.y3.0=field.x2.y3.1
            IF x3<11 THEN DO
              SEND APos(xpos+5,ypos+2)||col.6||field.x3.y3.1
              IF field.x3.y3.0='#' THEN DO
                flagsset=flagsset-1
                flagsleft=flagsleft+1
              END
              field.x3.y3.0=field.x3.y3.1
            END
          END
        END
        OTHERWISE NOP
      END
      SEND APos(65,2)||col.7'Flags left: 'col.2||RIGHT(flagsleft,2)||APos(65,3)||col.7' Flags set: 'col.2||RIGHT(flagsset,2)||APos(0,0)
    END
    IF show=2 & quit=0 THEN DO
      x=(xpos-11)/4
      y=(ypos-3)/2
      SELECT
        WHEN field.x.y.0='?' & flagsset<18 THEN DO
          flagsset=flagsset+1
          flagsleft=flagsleft-1
          field.x.y.0='#'
          SEND APos(xpos+1,ypos)||col.1'#'Apos(0,0)
          IF flagsset=18 THEN CALL CheckDone
        END
        WHEN field.x.y.0='#' THEN DO
          flagsset=flagsset-1
          flagsleft=flagsleft+1
          field.x.y.0='?'
          SEND APos(xpos+1,ypos)||col.7'?'Apos(0,0)
        END
        OTHERWISE NOP
      END
      SEND APos(65,2)||col.7'Flags left: 'col.2||RIGHT(flagsleft,2)||APos(65,3)||col.7' Flags set: 'col.2||RIGHT(flagsset,2)||APos(0,0)
    END
  END
  IF lose=1 THEN DO
    BUFFERFLUSH
    CALL waitkey()
  END
  IF win=1 THEN DO
    EndTime=TIME(S)
    GameTime=EndTime-BeginTime
    SEND APos(22,13)'                         '
    SEND APos(22,14)||col.2' You''ve found all mines! '
    SEND APos(22,15)'     In 'col.7||RIGHT(GameTime,3)||col.2' seconds...    '
    SEND APos(22,16)'                         'APos(0,0)
    CALL SortBit
    CALL WriteHiScores
    BUFFERFLUSH
    CALL Waitkey()
  END
RETURN

CheckDone:
  controle=0
  DO y=1 to 10
    DO x=1 to 10
      IF field.x.y.1='*' & field.x.y.0='#' THEN controle=controle+1
    END
  END
  IF controle=18 THEN DO
    quit=1
    win=1
  END
RETURN

APos:
RETURN(ESC'['arg(2)';'arg(1)'H')

QRnd: PROCEDURE
  ARG lo,hi
  n1=TIME(S)
  d=hi-lo+1
  x=LENGTH(d)
  n1=RIGHT(n1,x)
  p=d
  IF p>999 THEN p=999
  r=n1+RANDOM(0,p)
  r=r//d+lo
RETURN(r)

Instructions:
  CLS
  TR '\c3                ษอออออออออออออออออออออออออออออออออออออออออออออป'
  TR '                บ \c1INSTRUCTIONS for E!Mines v1.5 (Unregistered)\c3บ'
  TR '                ศอออออออออออออออออออออออออออออออออออออออออออออผ'
  TR '\n1\c6This is a ''Mines'' doorgame, and playing is simple:'
  TR 'You will be presented with a \c310\c6 by \c310 \c6grid that contains \c318\c6 mines.'
  TR 'Your mission: Clear the minefield by marking the location of all mines...'
  TR '\n1You move the cursor to a field, and press \c3<space>\c6 or \c3<enter>\c6 to clear it.'
  TR 'Hopefully you''ll get either an empty field, or a number.'
  TR 'If the field contains a mine, then it''s tough luck!'
  TR 'A number gives you the number of surrounding mines. An empty field means:'
  TR 'There are no mines in the surrounding 8 fields (So I''ll clear them for you)'
  TR 'You can mark out mines by pressing \c3<0>\c6 or \c3<.>\c6 on a field.'
  TR 'If you marked all mines correctly, then you''ve won!'
  TR 'And if you''re fast enough, you can even make it to the highscore! :-)'
  TR '\n1By the way, cursor movement can be done with the numpad, or the cursor keys.'
  TR '\n1\c2Hit any key to continue!'
  CALL WaitKey()
RETURN

ReadHiScores:
  IF EXISTS(DataPath'EMines.hi') THEN DO
    CALL OPEN(file,DataPath'EMines.hi',R)
    a=0
    DO UNTIL EOF(file)
      dummy=READLN(file)
      IF dummy~='' THEN DO
        a=a+1
        list.a=dummy
      END
    END
    CALL CLOSE(file)
  END
  ELSE DO
    DO a=1 TO 10
      list.a='Nobody..............:\c7 999 \c6seconds'
    END
    CALL OPEN(file,DataPath'EMines.hi',W)
    DO a=1 TO 10
      CALL WRITELN(file,list.a)
    END
    CALL CLOSE(file)
  END
RETURN

SortBit:
  hit=1
  DO a=10 TO 1 BY -1
    IF GameTime>=WORD(list.a,2) THEN DO
      hit=a+1
      LEAVE
    END
  END
  DO b=1 TO hit
    sort.b=list.b
  END
  sort.hit=LEFT(UALias,20,'.')':\c7 'RIGHT(GameTime,3)' \c6seconds'
  DO b=hit+1 TO 10
    a=b-1
    sort.b=list.a
  END
  DO a=1 TO 10
    list.a=sort.a
  END
RETURN

WriteHiScores:
  CALL OPEN(file,DataPath'EMines.hi',W)
  DO a=1 TO 10
    CALL WRITELN(file,list.a)
  END
  CALL CLOSE(file)
RETURN

WaitKey: PROCEDURE
  GETCHAR
  dummy=UPPER(RESULT)
  IF dummy='###PANIC' THEN EXIT
RETURN(dummy)

