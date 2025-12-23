/*  Shovel by Anthony Brice - July 10th 1995 - FreeWare   */

/* Based on Mat Bettinsons infamous PKTShovel.rexx script */

/* Modified by Neil Williams 14/11/96 */

Options Results
Parse Arg Flag

ThorBBSName= 'TKG FidoNet'
ConfigFile = 'Zeus:Config/ShovelThor.cfg'

CR = '0d'X
MadeDate = '18th Novemeber 1996'
Ver = '1.1' ; Nil = '>NIL: ' ; N = 1

If ~Show(l,'rexxsupport.library') then Call AddLib('rexxsupport.library',0,-30)

If ~show('ports','THOR.01') then Do
  Address Command
          "run >nil: `GetEnv THOR/THORPath`Thor"
          'WaitForPort THOR.01'
  End

Address 'THOR.01'


Call Time('R')

If ~Exists(ConfigFile) then do
  Temp=''||CR
  Temp=Temp||'ShovelThor 'Ver' by Neil Williams. Based on code by Anthony Brice'||CR||CR
  Temp=Temp||' Unable to find Config file!'||CR||CR  
  Exit 10
  End

Flowsrce = 0 ; Flowdest = 0 ; FlowAddr = 0 ; NumShove=1

Call Open(Cfg,Configfile,"R")
Do Until EOF(CFG)
  Crap=ReadLn(Cfg)
  Crap=Strip(Crap)
  If Crap~='' then Do
    FlowSrce.NumShove=Strip(Word(Crap,1))
    FlowDest.NumShove=Strip(Word(Crap,2))
    FlowAddr.NumShove=Strip(Word(Crap,3))
    NumShove=NumShove+1
    End
  End
Call Close(Cfg)

NumShove=NumShove-1

If Flag='REPORT' then Signal Report

/*
 * Open a ProgressBar
 */

OPENPROGRESS TITLE '"Packet Shovel"' PROGRESSTEXT '"      Setting Up     "' TOTAL 1
window = RESULT

UPDATEPROGRESS REQ window CURRENT 0 PT '"Exporting From Thor"'

/*
 * Export from Thor
 */

PACKEVENTS '"'ThorBBSName'"'

/*
 * Copy packets using the FLO/HLO/etc. files
 */

UPDATEPROGRESS REQ window CURRENT 1
UPDATEPROGRESS REQ window TOTAL NumShove CURRENT 0 PT '"Copying Packets"'

Call CopyPackets()

/*
 * Get the mail into Zeus / MM and exported to the downlinks
 */

UPDATEPROGRESS REQ window TOTAL 6 CURRENT 1 PT '"Importing Mail"'
/*Address Command 'RX <>NIL: MM:Rexx/MM_ImportPlus'*/
UPDATEPROGRESS REQ window CURRENT 2 PT '"Copying Crashes/Requests"'
Address Command 'Copy <>NIL: Mail:ThorOut/#?.CUT Outbound:'
UPDATEPROGRESS REQ window CURRENT 3
Address Command 'Copy <>NIL: Mail:ThorOut/#?.REQ Outbound:'
UPDATEPROGRESS REQ window CURRENT 4
Address Command 'Delete <>NIL: Mail:ThorOut/#?.CUT'
UPDATEPROGRESS REQ window CURRENT 5
Address Command 'Delete <>NIL: Mail:ThorOut/#?.REQ'
UPDATEPROGRESS REQ window CURRENT 6 PT '"Exporting Mail"'
/*Address Command 'RX <>NIL: MM:Rexx/MM_ExportPlus SMARTEXPORT'*/

/*
 * Copy any new mail that might have been exported into Thors Inbound Dir
 */

UPDATEPROGRESS REQ window TOTAL NumShove CURRENT 0 PT '"Copying New Packets Back"'

Call CopyPackets()

/*
 * Make sure Thor is ready to import the mail by the user
 */

RESCAN

/*Address Command 'List >T:ThorParse.temp Mail:ThorIn/#?.((MO|TU|WE|TH|FR|SA|SU)[0-9]|PKT) LFormat "Thor:bin/ParseFIDO >NIL: BBSNAME TKG GRAB Mail:ThorIn/%s ARCHIVE"'
Address Command 'Execute >NIL: T:ThorParse.temp'
Address Command 'List >T:ThorParse.temp Mail:ThorIn/#?.((MO|TU|WE|TH|FR|SA|SU)[0-9]|PKT) LFormat "Delete Mail:ThorIn/%s"'
Address Command 'Execute >NIL: T:ThorParse.temp'
Address Command 'Delete >NIL: T:ThorParse.temp'
*/

/* Address Command 'Copy Mail:ThorIn/#? Mail:MMIn/' */

Address Command 'Copy >NIL: Mail:ThorIn/#? Mail:CycIn/'
/*Address Command 'Move >NIL: Mail:CycOut/#? Mail:ThorOut/'*/


/*
 * Close up
 */

CLOSEPROGRESS REQ window

Exit 0

FlowfileExtract:
Call Open(Flow,FlowFile,'R')
Do Until EOF(flow)
  Flowline = READLN(Flow)
  If Length(FlowLine) < 3 then Break
  FlowLine = Translate(flowline,'-','#')
  FlowLine = Translate(flowline,'-','^')
  If Left(FlowLine,1) = '-' then Do
    FlowLine = DelStr(Strip(FlowLine),1,1)
    Delete = 'YES'
    End
  Else Delete = 'NO'
  Address Command
  TestFile = FlowLine
  Call GetFileType
  If TestFile = 'PKT' then Do
    MailType = 2
    FileEXT = Right(FlowLine,4)
    Call DateStamp
    Address Command 'Copy 'flowline' To 'FlowDest||Stamp
    End
   If testfile = 'BIN' then DO
     Address Command 'Copy 'flowline' To 'Flowdest
     End
   If delete = 'YES' then DO
     Address Command 'Delete 'NIL||FlowLine
     End
   End
Eh=Close(Flow)
Address Command 'Delete 'NIL||FlowFile
Return

GetFileType:
Temp=Translate(TestFile,' ',':/')
Temp=Word(Temp,Words(Temp))
Temp=Delstr(Temp,Length(Temp)-2,2)
Temp=Compress(temp,'.')
If DataType(Temp)='NUM' then Do
  TestFile = 'PKT'
  End
Else TestFile='BIN'
Return

DateStamp:
N=N+1 
S=D2x((Time('E')*100)+N)
D=D2x(Time('S'))
Z=D2x(Date('I'))
Stamp=Right(Z||D||S,8)
ExtTod = '.'UPPER(Left(Date('W'),2))'0'
If MailType=1 then Stamp=Stamp'.PKT'
Else Stamp=Stamp||ExtTod
Return

CopyNetmail:
MailType = 1
Call DateStamp
Address Command 'Copy 'NIL||Netmailfile' TO 'FlowDest.I||Stamp
Address Command 'Delete 'NIL||NetmailFile
Return

Report:
Say ; Say 'ShovelThor 'Ver' by Neil Williams. Based on code by Anthony Brice.'
Say MadeDate ; Say
Say 'Reporting Config file:-' ; Say
Do I=1 to NumShove
  Say 'Packets: 'FlowSrce.I||FlowAddr.I' to Directory: 'FlowDest.I
  End
Say ; Say 'Report complete. Exiting Shovel...' ; Say
Exit 0


/*
 * CopyPackets()
 *
 * Searches for the relevant FLO/HLO/etc. files and acts on them accordingly
 * (AFAIK, this was written by Anthony Brice)
 */

CopyPackets:

Do I = 1 to NumShove
  NetMtemp = Flowsrce.i||Flowaddr.i

  UPDATEPROGRESS REQ window CURRENT I

  If Exists(NetMtemp'.DUT') then DO
    NetmailFile = netMtemp'.DUT'
    Call CopyNetMail
    End
  If Exists(NetMtemp'.HUT') then DO
    NetmailFile = netMtemp'.HUT'
    Call CopyNetMail
    End
  If Exists(NetMtemp'.CUT') then DO
    NetmailFile = netMtemp'.CUT'
    call CopyNetMail
    End
  If Exists(NetMtemp'.OUT') then DO
    NetmailFile = netMtemp'.OUT'
    call CopyNetMail
    End
  FlowDest = FlowDest.I
  If exists(NetMtemp'.FLO') then DO
    Flowfile = NetMtemp'.FLO'
    Call FlowfileExtract
    END
  If exists(NetMtemp'.HLO') then DO
    Flowfile = NetMtemp'.HLO'
    Call FlowfileExtract
    END
  If exists(NetMtemp'.CLO') then DO
    Flowfile = NetMtemp'.CLO'
    Call FlowfileExtract
    END
  END

Return

