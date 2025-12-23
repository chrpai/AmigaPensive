/*

    STR.ScrollerFB.rexx - Reads E!'s FBase files and displays them using
                          an ANSI interface.

    Version 3.51fb

*/

Options results
Signal on Syntax
Signal on Error

Call Pragma('S',64000)

Version = 'v3.5fb'

If ~Show('L','rexxsupport.library') Then Do
    If ~AddLib('rexxsupport.library',0,-30,0) Then Do
       Transmit 'RexxSupport.Library missing! Can''t continue...'
       Exit 10
    End
End

       /***********************************************************/
      /* Begining of the configuration... Adjust it as you need. */
     /***********************************************************/

/* Selected language: '1' = English, '0' = French. */
/* You could also put your own language selection routine here... */
Language = 1

/* Minimum access required for accessing the Sysop Module */
SysopLevel = 32

/* Set the following to '1' to enable FileScroller logging */
LogActive = 1

/* Here you put the path and the filename of the log */
LogFile ='BBS:TEXT/Logs/FScroller.log'

/* Where extracted files goes. */
DestDir = 'T:'

/* Most number of comment lines allowed per file. */
commentlines = 10

/* Set this only when debuging. */
debug = 0

/* If you have never run TransAmiga BBS, set this to 0. */
stripnumfidl = 1  /*  If your listings are from TransAmiga */
                  /*  and have the [0], etc., download     */
                  /*  counters, this will strip them from  */
                  /*  the final output.                    */

/* Next part is about archive handling. */
/* To disable one archive type, just tell "Com.? = 'OFF'", and leave the
   remainder of its line untouched.

Archiver Filename ; List Switches   ; Extract Switches                      */
Com.1 = 'LhA'     ; Switch.1 = 'vv' ; Extract.1 = 'e'  /* .lha & .run files */
Com.2 = 'LhA'     ; Switch.2 = 'vv' ; Extract.2 = 'e'  /* .lzh files        */
Com.3 = 'Zoo'     ; Switch.3 = 'l'  ; Extract.3 = 'x'  /* .zoo files        */
Com.4 = 'OFF'     ; Switch.4 = '-v' ; Extract.4 = '-e' /* .arc files        */ 
Com.5 = 'unzip'   ; Switch.5 = '-v' ; Extract.5 = ''   /* .zip files        */
Com.6 = 'OFF'     ; Switch.6 = 'v'  ; Extract.6 = 'x'  /* .arj files        */

                      /**************************/
                     /* End of the Config Area */
                    /**************************/

/* Initialisation */

ViewArchiveFlag = 0
NumFiDL = 1
HighArea = 0
ActualDate = Date('I')
BBSIdentify Term; Screen = Word(Result,2)
Screen = Screen - 7 ; If Screen < 14 Then Screen = 14
GetUser '11' ; Tmp = Result ; Call ConvertDate(tmp) ; LastCall = Result ; CutOff = LastCall
GetUser '3'  ; Name = Result
GetUser '23' ; Node = Result
GetUser '15' ; Acces = Result

If debug then do
  GetUser '11'; Print Result
End

/*
If NumFiDl Then LineLength = 98
Else LineLength = 93
*/

If NumFiDl Then LineLength = 110
Else LineLength = 105

GetCarrier ; Carry = Result
If Carry = 'FALSE' Then Exit

CSI = '1b'x||'[' ; BRI = CSI'1m' ; REB = CSI'41m' ; BLB= CSI'40m'
OFF = CSI'0;37m' ; RED = CSI'31m' ; GRE = CSI'32m'; YEL = CSI'33m'
BLU = CSI'34m' ; PUR = CSI'35m' ; CYA = CSI'36m' ; WHI = CSI'37m'
ITA = CSI'3m' ; UND = CSI'4m' ; CEL = CSI'K' ; CEP = CSI'J' ; UP = CSI'A'

Newline
Transmit '\@6'||CEL||BLU||'FileScroller '||CYA||version||BLU||' by '||ITA||CYA||'Merlin'||OFF||BLU||'.'
Transmit 'Modified for Excelsior! by '||ITA||CYA||'Black Fire'||OFF||BLU||'.'
Newline
SetNodeLocation 'FileScroller'

Call PrepFBase()

If LogActive then do
  If ~Exists(LogFile||Node) then tmp = 'W'
  Else tmp = 'A'
  Call Open('ScrollLog',LogFile||Node,tmp)
  Call LogThis('*',Name||' entered FileScroller.')
End

ListType = 1
If Language Then TextType = YEL||'Normal listing'||GRE||'.'
Else TextType = YEL||'Liste normale'||GRE||'.'
ListName = 'T:FileList'||node

Path = CDPath

/* Main Program */

Main:
Call CreateFileList
Call Entete
if ~Open('Input',ListName,'R') then exit
Call Affiche
Call DownPrompt
Call Main_Prompt
Call Quit(0)


/* Functions */

CreateFileList:
  AreaName=CDName1
  AreaNumber=HighArea+1
  If LogActive then Call LogThis('&','Entered '||AreaNumber||'-'||AreaName||' area.')
  ActualDate = Date('I')
  Block = 0

  Call CleanUp

  tmp = 'CreateList -sT:Files.BBS -l'||lastcall||' -c'||Right(Date('S'),6)||' -n'||node||' -a1'
  If NumFidl then tmp = tmp||' -d1'
  If debug Then Print tmp
  Address Command tmp

  Call Open('Input','T:ListInfos'||node,'R')
  MaxLines = ReadLn('Input')
  Comments = ReadLn('Input')
  CommentsLines = ReadLn('Input')
  Call Close('Input')

  If ListType =4 | ListType =5 then Do
    OldListName ='T:FileList'||node
    OldListType =1
    If Language then OldTextType = YEL||'Normal listing'||GRE||'.'
    Else OldTextType = YEL||'Liste normale'||GRE||'.'
  End

  If ListType = 2 Then Call Process_2
  If ListType = 3 Then Call Process_3
Return

Position:
   Arg x,y
   Send CSI||y||';'||x'H'
Return

LogThis:
  Parse Arg Kind,Entry
  Call Writeln('ScrollLog',' '||Kind Date()||'  '||Time()||' - '||Entry)
Return

Cursor_Convert:
  If Answer = '5b'x | Answer = '9b'x then Do
     GetChar ; Tmp = upper(result)
     If Tmp = 'A' then Return '8'
     If Tmp = 'B' then Return '2'
     If Tmp = 'C' then Return '1'
     If Tmp = 'D' then Return '7'
  End
Return Answer


Entete:
  Cls
  Areaname=CDName1
  if Length(Areaname) > 30 Then Areaname = Left(Areaname,30)
  If Language then texte = 'Area'
  Else texte = 'Librairie'
  Sendstring '\@6'GRE||texte||':'||BRI||YEL||Areaname||OFF||GRE||'.'
  Call Total_Files
Small_Entete:
  Texte = BLU||Copies('Ä',79)
  Call Position(1,2)
  Send Texte
  If ListType ~=4 then Do
    Call Position(1,4)
    Send Texte
    Call Position(1,Screen+5)
    Send Texte
  End
  If Language & ListType ~=4 then do
    Call Position(1,3) ; Send CEL||WHI||'Filename'
    Call Position(24,3) ; Send CYA||'Kb  '||YEL||'Date'
    Call Position(38,3)
    If NumFidl then Send RED||'N '||GRE||'D/L '||CYA||'Description'
    Else Send RED||'N '||CYA||'Description'
  End
  If Language = 0 & ListType ~=4 then do
    Call Position(1,3) ; Send CEL||WHI||'Nom du fichier'
    Call Position(24,3) ; Send CYA||'Kb  '||YEL||'Date'
    Call Position(38,3)
    If NumFiDl then Send RED||'N '||GRE||'D/L '||CYA||'Description'
    Else Send RED||'N '||CYA||'Description'
  End
  Call Position(40,1)
  Send BRI||Left(TextType,32)||OFF
Return


Smart_Entete:
  If Language then texte = 'Area'
  Else texte = 'Librairie'
  Areaname=CDName1||':'||substr(Path,length(CDPath)+1)
  if Length(Areaname) > 30 Then Areaname = Left(Areaname,30)
  Call Position(1,1)
  Send CEL||GRE||texte||':'||BRI||YEL||Areaname||OFF||GRE||'.'
  Call Total_Files
  Call Position(40,1)
  Send BRI||Left(TextType,32)||OFF
Return

  
Affiche:
  Call Position(1,5)
  If Move ~= 'N' then Call Seek('Input',Block * Screen * LineLength,'B')
  Move = ''
  Do Count = 1 to Screen
    Data.Count = Readln('Input')
    Transmit CEL||Trim(Data.Count)
  End
  Refresh = 0
Return


ClrScrn:
  Arg start,finish
  Call Position(1,start)
  Do Count = 1 to finish
    Transmit CEL
  End
Return

ShowHelp:
  Arg tmp
  tmp = 'FileScroll_'||tmp
  If Language then tmp = tmp||'.Hlp'
  Else tmp = tmp ||'.Fra.Hlp'
  SendMenu tmp
  Call Position(1,screen+6)
  Send CEL||OFF
  Call Position(1,screen+4)
  Continue ; Call Position(1,screen+6) ; Send CEL
Return

DownPrompt:
  Call Position(1,Screen+6)
  Send CEL
  Call PageTotal
  Call Position(1,Screen+6)
  If Language Then Do
    If Max > 0 then texte = '?'||OFF||WHI||'=Help '||BLU||'-'||BRI||CYA||' #,0,8,2,7,1 '||OFF||BLU||'-'||BRI||WHI||' D,I,L,M,R,S,V,Q'
    Else texte = '?'||OFF||WHI||'=Help '||BLU||'-'||BRI||CYA||' #,0,8,2,7,1 '||OFF||BLU||'-'||BRI||WHI||' D,L,R,Q'
  End
  Else Do
    If Max > 0 then texte = '?'||OFF||WHI||'=Aide '||BLU||'-'||BRI||CYA||' 0,8,2,7,1 '||OFF||BLU||'-'||BRI||WHI||' D,I,L,M,R,C,V,Q'
    Else texte = '?'||OFF||WHI||'=Aide '||BLU||'-'||BRI||CYA||' 0,8,2,7,1 '||OFF||BLU||'-'||BRI||WHI||' D,L,R,Q'
  End
  getuser 7; timeleft = result
  Send BLU||'['||WHI||TimeLeft||BLU||'] '||BRI||CYA||texte||BLU||' >'||OFF
Return


PageTotal:
   Call Position(68,Screen+6)
   Send CEL||GRE||'Page '||BRI||YEL||Block + 1||OFF||GRE||'/'||BRI||YEL||((MaxTotal-1) % Screen) + 1||OFF||GRE||'.'
Return


Main_Prompt:
  Do Forever
    GetChar ; Answer = upper(result)
    If Answer = '###PANIC' then Call Quit(1)
    Call Cursor_Convert ; Answer = result

    Select

       When Answer = '8' & Block > 0 then do
          Block = Block - 1
          If Language then Send PUR||'Page up.'
          Else send PUR||'Monte.'
          Call Affiche
          Call DownPrompt
       End

       When Answer = '7' & Block ~=0 then do
          Block = 0
          if Language then Send PUR||'Top.'
          else Send PUR||'Debut.'
          Call Affiche
          Call DownPrompt
       End

       When (Answer = '2' | Answer = '0d'x) & Block < ((MaxTotal-1)% Screen) Then Do
          Move = 'N'
          Block = Block+1
          if Language then Send PUR||'Page down.'
          Else Send PUR||'Descend.'
          Call Affiche
          Call DownPrompt
       End

       When Answer = '1' & Block ~=((MaxTotal-1) % Screen) then Do
          Block = (MaxTotal-1) % Screen
          If Language then Send PUR||'End.'
          else Send PUR||'Fin.'
          Call Affiche
          Call DownPrompt
       End

       When Answer = '0' & (Block+5) <= ((MaxTotal-1)% Screen) Then Do
          Block = Block+5
          if Language then Send PUR||'Jump 5 pages.'
          Else Send PUR||'Avance 5 pages.'
          Call Affiche
          Call DownPrompt
       End

       When Answer = 'R' then do
          Call Entete
          Call Affiche
          Call DownPrompt
       End

       When Answer = 'L' then do
          Call Close('Input')
          Call ListingType
          Call Affiche
          Call DownPrompt
       End

       When Answer = '?' then do
          Call ClrScrn(5,Screen)
          Call ShowHelp('main')
          Call ClrScrn(5,Screen)
          Call ShowHelp('main2')
          Call Affiche
          Call DownPrompt
       End

       When Answer = 'D' then do
         Transmit YEL||'Download Marked Files'
         If ~exists('T:MarkedFiles') Then do
           If Language then texte = 'No Marked Files!'
           Else texte = 'Aucun fichier!'
           Call Position(1,Screen+6)
           Call WarnPrompt(Texte,'P')
         End
         Else Do
           Cls
           Download
         End
         Call Delay(50)
         GetCarrier
         If result = 'FALSE' then do
           Call Close_IO
           Call Quit(1)
         End
         Call Entete
         Call Affiche
         Call DownPrompt
       End

       When Answer = 'M' & Max > 0 then do
         If Language then action = 'Filename to mark'
         Else action = 'Fichier a marquer'
         Do Forever
           Call AFR ; File = result
           If File = '###NOTHING' then Leave
           Call Mark_File
         End
         Call DownPrompt
       End

       When Answer = 'G' & Max > 0 then do
         If Language then action = 'Filename to View'
         Else action = 'Fichier a marquer'
         Do Forever
           Call AFR ; File = result
           If File = '###NOTHING' then Leave
           Call View_File()
         End
         Call DownPrompt
       End

       When Answer = 'V' & Max > 0 Then Do
         If Language Then action = 'File or archive to view'
         Else action = 'Texte ou archive a voir'
         Call AFR ; File = result
         If File = '###PANIC' then Call Quit(1)
         If File ~= '###NOTHING' then do
           Call Read_File
           Call Entete
           Call Affiche
         End
         Call DownPrompt
       End

       When Answer = 'I' & Max > 0 then do
         If Language then action = 'File to look at'
         Else action = 'Fichier a examiner'
         Call AFR ; File = result
         If File ~= '###NOTHING' then do
           Call Affiche_Browse_Entete
           If Language then Send YEL||'Retrieving informations...'
           Else Send YEL||'Rassemblons les informations...'
           If LogActive then Call LogThis('.','Examined '''||File||'''.')
           Call GetBrowseInfo
           Call Affiche_Browse_Data
           Call Position(1,Screen+6)
           Send CEL ; Call Position(1,Screen+4) ; Continue
           Call Entete
           Call Affiche
         End
         Call DownPrompt
       End

       When (Answer = 'S' | Answer = 'C') & Max > 0 then do
         OldListName = ListName
         OldListType = ListType
         OldTextType = TextType
         ListName = 'T:FileList'||node
         Call Close('Input')
         Call Open('Input',ListName,'R')
         ListType = 4
         If Language then TextType = YEL||'Search File'||GRE||'.'
         Else TextType = YEL||'Recherche'||GRE||'.'
         Call Entete
         Call Affiche_Browse_Entete
         Call BrowseSearch
         ListType = OldListType
         ListName = OldListName
         TextType = OldTextType
         Call Total_Files
         Call Small_Entete
         Call Close('Input')
         Call Open('Input',ListName,'R')
         Call Affiche
         Call DownPrompt
       End

       When Answer = '#' then do
         Call Position(1,Screen+6)
         if Language then Query CEL||CYA||'Jump at page number'||BLU||' ('||WHI||'1'||BLU||'-'||WHI||((MaxTotal-1) % Screen) + 1||BLU||') :'||WHI
         Else Query CEL||CYA||'Sauter a la page numero'||BLU||' ('||WHI||'1'||BLU||'-'||WHI||((MaxTotal-1) % Screen) + 1||BLU||') :'||WHI
         Tmp = result
         If Tmp = '###PANIC' then Call Quit(1)
         Send UP
         If Upper(Tmp) ~= 'RESULT' then do
           If DataType(Tmp) = 'NUM' & Tmp >0 & Tmp ~= Block & Tmp <= ((MaxTotal-1) % Screen) + 1 then Do
             If Language then Send CEL||BLU||'Jumping at page '||CYA||Tmp||BLU||'.'
             Else Send CEL||BLU||'Saut a la page '||CYA||Tmp||BLU||'.'
             Block = Tmp-1
             Call Affiche
           End
         End
         Else Do
             If Upper(Tmp) ~= 'RESULT' & Tmp ~= Block Then Do
               Call Position(1,Screen+6)
               If Language then Texte ='Invalid page number!'
               Else Texte ='Numero de page invalide!'
               Call WarnPrompt(Texte)
             End
         End
         Call DownPrompt
       End

       When Answer = 'Q' then do
         If Language then Transmit BRI||YEL||'Quit!'
         Else Transmit BRI||YEL||'Quitter!'
         Call Close('Input')
         Leave
       End
       Otherwise NOP
     End
  End
Return



ListingType:
  Call Position(1,Screen+6)
  If Language then Do
     texte = '('||BRI||CYA||'1'||OFF||WHI||')Normal  ('||BRI||CYA||'2'||OFF||WHI||')New Files  ('||BRI||CYA||'3'||OFF||WHI||')Sorted'
     If Max > 0 then Texte = Texte||' ('||BRI||CYA||'4'||OFF||WHI||')Browse'
  End
  Else Do
     Texte = '('||BRI||CYA||'1'||OFF||WHI||')Normal  ('||BRI||CYA||'2'||OFF||WHI||')Nouveaux fichiers  ('||BRI||CYA||'3'||OFF||WHI||')Classes'
     If Max > 0 then Texte = Texte||' ('||BRI||CYA||'4'||OFF||WHI||')Detaille'
  End
  Send CEL||WHI||Texte||BLU||' >'
  GetChar ; Answer = upper(Result)
  If Answer = '###PANIC' then Call Quit(1)

  Select
    When Answer = '1' then Do
      ListType = 1
      ListName = 'T:FileList'||node
      If Language then TextType = YEL||'Normal listing'||GRE||'.'
      Else TextType = YEL||'Liste normale'||GRE||'.'
    End

    When Answer = '2' then Do
      Call Position(1,Screen+6)
      If Language then Texte = CEL||CYA||'Files uploaded since (ie '940125')'||BLU||':'
      Else Texte = CEL||CYA||'Fichiers uploades depuis le'||BLU||':'
      Query Texte||WHI
      CutOff = result
      If CutOff = '###PANIC' then Call Quit(1)
      If Length(CutOff) ~= 6 | ~DataType(CutOff,Numeric) then CutOff = '780101' 
      If Language then Send YEL||'Processing partial listing...'
      Else Send YEL||'Creation de la liste partielle...'
      ListType = 2
      If Language then TextType = YEL||'Since '||WHI||Date('N','19'||CutOff,'S')||GRE||'.'
      Else TextType = YEL||'Depuis le '||WHI||Date('N','19'||CutOff,'S')||GRE||'.'
      ListName = 'T:CutOffList'||node
      Call Process_2
    End

    When Answer = '3' then Do
      Call Position(1,Screen+6)
      If Language then Texte ='Sorted by '||WHI||'N'||CYA||'ame, '||WHI||'D'||CYA||'ate, '||WHI||'S'||CYA||'ize '
      Else Texte = 'Classer par '||WHI||'N'||CYA||'om, '||WHI||'D'||CYA||'ate, '||WHI||'L'||CYA||'ongueur ?'
      Answer = ''
      Send CEL||CYA||Texte||BLU||'>'||YEL
      Do Until Answer = '###PANIC' | Answer = 'D' | Answer = 'N' | Answer = 'S' | Answer = 'L'
        GetChar ; Answer = upper(result)
      End

      If Answer = '###PANIC' then Call Quit(1)

      If Answer = 'D' then Do
        SortType = 'd'
        If Language then TextType = 'Sorted by Date'
        Else TextType = 'Classes par Date'
      End

      If Answer = 'N' then Do
        SortType = 'a'
        If Language Then TextType = 'Sorted By Filename'
        Else TextType = 'Classes par Nom'
      End

      If Answer = 'S' | Answer = 'L' then Do
        SortType = 's'
        If Language Then TextType = 'Sorted by Size'
        Else TextType = 'Classes par Longueur'
      End

      TextType = YEL||TextType||GRE||'.'

      Call Position(1,Screen+6)
      If Language then Texte ='Perform a reversed sort ?'
      Else Texte = 'Effectuer un classement inverse ?'
      QueryN CEL||CYA||Texte||YEL
      Answer = Upper(left(Result,1))
      If Answer = 'Y' then SortType = SortType||' r'

      ListType = 3
      Call Position(1,Screen+6)
      If Language then Send CEL||YEL||'Processing...'
      Else Send CEL||YEL||'Classement...'
      ListName = 'T:FileSorted'||Node
      Call Process_3
    End

    When Answer = '4' & Max > 0 Then Do
      OldListName = ListName
      OldListType = ListType
      OldTextType = TextType
      ListName = 'T:FileList'||node
      Call Open('Input',ListName,'R')
      ListType = 4
      If Language then TextType = YEL||'Browse mode'||GRE||'.'
      Else TextType = YEL||'Liste detaillee'||GRE||'.'
      Call Browse
      ListType = OldListType
      ListName = OldListName
      TextType = OldTextType
      Call Small_Entete
    End

    Otherwise NOP
  End

  Block = 0
  Call Open('Input',ListName,'R')
  Call Position(40,1)
  Send BRI||Left(TextType,32)||OFF
  Call Total_Files

Return


Process_2:
  dfm = 0
  Call Open('Input','T:FileList'||node,'R')
  Call Seek('Input',0,'B')
  Call Delete(ListName)
  Call Open('Output',ListName,'W')
  MaxNew = 0
  Do while eof('Input') = 0
    Data = Readln('Input')
    If Data = '' then Iterate
    MaxNew = MaxNew + 1
    If Left(Data,1) = ' ' & dfm then Do
      Call Writeln('Output',Data)
      Iterate
    End
    tmp = ConvertDate('aaa '||Translate(Strip(SubStr(Data,43,9)),'-',' '))
    If tmp >= CutOff then do 
      Call Writeln('Output',Data)
      dfm = 1
    End
    Else do
      MaxNew = MaxNew - 1
      dfm = 0
    End
  End
  Call Close('Output')
  Call Close('Input')
Return


Process_3:
  If Exists('T:Files.BBS') & (Word(StateF('T:Files.BBS'),2) > 0) Then Do 
     Tmp = 'FileSort T:Files.BBS T:Files.BBS'||Node||' '||SortType
     Address command Tmp
     tmp = 'CreateList -sT:Files.BBS'||node||' -l'||lastcall||' -c'||Right(Date('S'),6)||' -n'||(node+10)||' -a1'
  End
  Else Do
     tmp = 'CreateList -s'path||'Files.BBS -l'||lastcall||' -c'||Right(Date('S'),6)||' -n'||(node+10)||' -a1'
  End
  If NumFidl then tmp = tmp||' -d1'
  Address Command tmp

  Call Delete(Listname)
  Call Rename('T:FileList'||(node+10),ListName)
  Call Delete('T:#?'||(Node+10))
  Call Delete('T:Files.BBS'||node)
Return



Read_File:
  Cls
  Call ArchiveList(File)
  If result ~= 'Unsupported' then Do
    Do Until ArchFile = 'RESULT' | ArchFile = ''
      Newline
      If Language then texte = CYA||'Filename to extract, '''||GRE||'?'||CYA||''',or '
      Else texte = CYA||'Fichier a extraire, '''||GRE||'?'||CYA||''', ou '
      Query texte||GRE||'RETURN'||BLU||':'||YEL
      ArchFile = result
      If ArchFile = '###PANIC' then Call Quit(1)
      If ArchFile = '?' then do
        Cls
        Call ArchiveList(File)
        Iterate
      End
      If ArchFile ~= 'RESULT' & ArchFile ~= '' then Do
        If LogActive then Call LogThis('.','Unpacked '''||File||''' archive.')
        If Language then Transmit GRE||'Extracting file...'
        Else Transmit GRE||'Extraction du fichier...'
        OldPath = Pragma('D')
        Call Pragma('Directory',DestDir)
        Address Command Com.tmp '>NIL:' Extract.tmp path||File ArchFile
        If Exists(ArchFile) then do
          Address command 'Protect '||ArchFile||' +d'
          Cls
          Sendfile DestDir||ArchFile
          Call Delete(ArchFile)
        End
        Else Do
          NewLine
          If Language then Transmit RED||'No file extracted!'
          Else Transmit RED||'Fichier non trouve dans l''archive!'
          NewLine
        End
        Call Pragma('Directory',OldPath)
        Continue
      End
    End
  End
  Else Do
    If Upper(Right(File,4)) = '.GIF' | Upper(Right(File,4)) = '.JPG' then do
      Address Command 'VT >T:PicSize Info 'Path||File
      Call Open('DFM','T:PicSize','r')
      Do i = 1 to 4
        texte = readln('DFM')
      End
      Call Close('DFM')
      Call Delete('T:PicSize')
      Newline; Call WarnPrompt(Texte,'L')
      i = -1
    End
    Else Do
      tmp = 'LHALZHZOOARCDMSZIPZOMIFFWRPGIFRUNEXEPICDEXHAMARJAPESFXJPGOWS'
      Do i = 1 to 60 by 3
         If Upper(Right(File,4)) = '.'||SubStr(tmp,I,3) Then Do
            i = -1
            if Language then texte = 'Cannot process this file!'
            else texte = 'Ne peux visionner ce fichier!'
            Newline ; Call WarnPrompt(Texte)
            Leave i
         End
      End
    End
    If i ~=-1 Then Do
      If LogActive then Call LogThis('.','Viewed '''||File||'''.')
      Sendfile path||File
      Continue
    End
  End
Return


Mark_File:
   Call Position(1,Screen+6)
   Address Command 'Echo >>T:MarkedFiles' Path||File; result = 'OK'
   MarkFile Path||File
   If result = 'FAIL' then do
     If Language then texte = 'Can''t mark this file!'
     Else texte = 'Ne peut marquer ce fichier!'
     Call WarnPrompt(Texte)
   End
   Else do
     If LogActive then Call LogThis('.','Marked '''||File||'''')
     if Language then texte ='File '''||CYA||File||WHI||''' is now marked. '
     Else texte = 'Le fichier '''||CYA||File||WHI||''' est marque. '
     Send CEL||WHI||Texte
     Call Delay(150)
   End
Return


WriteCenter:
  Parse Arg Textebis
  i = Length(Textebis)
  Texte = Centre(Textebis,77)
  Call WriteLn('Dest',Texte)
Return


Total_Files:
  Call Position(62,1)
  If Language then do
    If ListType =4 then Texte = '   File: '||GRE||'/'
    Else Texte = ' Total Files:'
  End
  Else Do
    If ListType =4 then Texte = 'Fichier: '||GRE||'/'
    Else Texte = '    Fichiers:'
  End

  If ListType =2 then Do
    Max = MaxNew - Comments
    MaxTotal = MaxNew
  End
  Else Do
    Max = MaxLines - Comments
    MaxTotal = MaxLines
  End

  Transmit CEL||GRE||Texte||YEL||BRI||Max||OFF||GRE||'.'
Return



AFR:
  Call AFR_Prompt
  Pointer = 1
  If Left(Data.Pointer,1) = ' ' then Do 
    Do Until Left(Data.Pointer,1) ~= ' '
       Pointer = Pointer + 1
    End
  End
  Call Affiche_Red

  Do Forever
    GetChar ; Answer = upper(result)
    Call Cursor_Convert ; Answer = Result

    Select
      When Answer = '8' then do
        Call Affiche_black
        Pointer = Pointer - 1
        If Pointer <1 then Pointer = Screen
        If Data.Pointer = '' | Left(Data.Pointer,1) = ' ' then do while (Data.Pointer = '' | Left(Data.Pointer,1) = ' ')
          Pointer = Pointer - 1
          If Pointer <1 then Pointer = Screen
        End
        Call Affiche_Red
      End

      When Answer = '7' then do
        Call Affiche_black
        Pointer = 1
        If Left(Data.Pointer,1) = ' ' then do While Left(Data.Pointer,1) = ' '
          Pointer = Pointer + 1
        End
        Call Affiche_red
      End

      When Answer = '2' then do
        Call Affiche_black
        Pointer = Pointer + 1
        If Pointer > Screen | Data.Pointer = '' then Pointer = 1
        If Left(Data.Pointer,1) = ' ' then do while Left(Data.Pointer,1) = ' '
          Pointer = Pointer + 1
          If Pointer > Screen | Data.Pointer = '' then Pointer = 1
        End
        Call Affiche_Red
      End

      When Answer = '0' then do
        Call Affiche_black
        Pointer = Pointer + 5
        If Pointer > Screen | Data.Pointer = '' then Pointer = 1
        If Left(Data.Pointer,1) = ' ' then do while Left(Data.Pointer,1) = ' '
          Pointer = Pointer + 1
          If Pointer > Screen | Data.Pointer = '' then Pointer = 1
        End
        Call Affiche_Red
      End

      When Answer = '1' then do
        Call Affiche_black
        Pointer = Screen
        If Data.Pointer = '' | Left(Data.Pointer,1) = ' ' then do while (Data.Pointer = '' | Left(Data.Pointer,1) = ' ')
          Pointer = Pointer - 1
        End
        Call Affiche_red
      End

      When Answer = '0d'x then do
        TheFile = DelStr(word(data.pointer,1),1,5)
        Call Affiche_Black
        Leave
      End

      When Answer = '?' then do
        Call ClrScrn(5,Screen)
        Call ShowHelp('Afr')
        Call Affiche
        Call Affiche_red
        Call AFR_Prompt
      End

      When Answer = 'E' then do
        Call Affiche_Black
        Call Position(1,Screen+6)
        Query CEL||CYA||Action||BLU||':'||WHI
        TheFile = result
        If TheFile = '###PANIC' then Call Quit(1)
        If Exists(path||TheFile) then Leave
        Else do
          If TheFile ~= 'RESULT' then do
            Call Position(1,Screen+6)
            If Language then Texte ='File not found! '
            Else Texte ='Fichier introuvable! '
            Call WarnPrompt(Texte)
          End
          Call Affiche_Red
          Call AFR_Prompt
        End
      End

      When Answer = 'Q' then Do
        Call Affiche_Black
        TheFile = '###NOTHING'
        Leave
      End
      Otherwise NOP
    End

    If Answer = '###PANIC' then Call Quit(1)
  End
  Answer = ''
  Data = Data.Pointer
Return TheFile

Affiche_black:
  Call Position(1,Pointer+4)
  Send word(data.pointer,1)
Return

Affiche_red:
   Call Position(1,Pointer+4)
   Send REB||word(data.pointer,1)||BLB
Return

AFR_Prompt:
  Call Position(1,Screen+6)
  If Language Then texte = '?'||OFF||WHI||'=Help '||BLU||'-'||BRI||CYA||' 0,8,2,7,1 '||OFF||BLU||'-'||BRI||WHI||' E,Q  '||OFF||CYA||'Return'||WHI||'=Select'
  Else texte = '?'||OFF||WHI||'=Aide '||BLU||'-'||BRI||CYA||' 0,8,2,7,1 '||OFF||BLU||'-'||BRI||WHI||' E,Q  '||OFF||CYA||'Return'||WHI||'=Accepter'
  getuser 7; timeleft = result
  Send CEL||BLU||'['||WHI||Timeleft||BLU||'] '||BRI||CYA||texte||BLU||' :'
Return



ConvertDate: Procedure Expose ActualDate
  Parse Arg tmp
  If Left(Tmp,5) = 'Today' then Do
    Tmp = Date('N')
    Tmp = DelStr(Tmp,8,2)
  End

  if pos('/',tmp) > 0 | pos('.',tmp) > 0 then do
    tmp = word(tmp,2)
    if pos('/',tmp) > 0 then parse var tmp day'/'month'/'year
    else parse var tmp day'.'month'.'year
    year = right(year,2)
    if length(day) = 1 then day = '0'day
    if length(month) = 1 then month = '0'month    
  end
  else do
    tmp = word(tmp,2)
    parse var tmp day'-'month'-'year
    year = right(year,2)
    if length(day) = 1 then day = '0'day
    month = pos(month,'   JanFebMarAprMayJunJulAugSepOctNovDec')%3
    if length(month) = 1 then month = '0'month
  end

Return Year||Month||Day

/* This is European Date format in E!:  Mon 28/3/1994 6:02am */


ArchiveList:
  Arg File
  Transmit Path||File
  If Open('Archive',Path||File,'R') then do
    I = Readch('Archive',48)
    Call Close('Archive')
    Select
      When SubStr(I,45,3) = "SFX" Then Tmp = 1
      When Substr(I,3,4) = "-lh0" Then Tmp = 1
      When Substr(I,3,4) = "-lh1" Then Tmp = 2
      When Substr(I,3,4) = "-lh2" Then Tmp = 1
      When Substr(I,3,4) = "-lh3" Then Tmp = 1
      When Substr(I,3,4) = "-lh4" Then Tmp = 1
      When Substr(I,3,4) = "-lh5" Then Tmp = 1
      When Substr(I,3,4) = "-lhd" Then Tmp = 1
      When Left(I,3) = "ZOO" Then Tmp = 3
      When Left(I,1) = "1a"x Then Tmp = 4
      When Left(I,2) = "PK" Then Tmp = 5
      When Left(I,2) = "60ea"x Then Tmp = 6
      Otherwise Tmp = 'Unsupported'
    End
  End
  Else Tmp = 'Unsupported'
  If Com.Tmp = 'OFF' Then Tmp = 'Unsupported'
  If Tmp ~= 'Unsupported' Then do
    If ViewArchiveFlag then ViewArchive File
    Else do
      Address Command Com.Tmp' >T:Listing'||Node Switch.Tmp Path||File
      Sendfile 'T:listing'||node
      Call Delete('T:listing'||node)
      NewLine
      Continue
    End
  End
Return Tmp


Browse:
  Pointer = 0
  FilePointer = 1
  Move = 'F'
  Call Entete
  Call Affiche_Browse_Entete
  If LogActive then Call LogThis('-','Starting browse mode.')
  Do Until Answer = 'Q'
    Call Browse_Read_Data(Move) ; File = Result
    Call GetBrowseInfo
    Call Browse_File_Total
    Call Affiche_Browse_Data
    Call Browse_Prompt
    Call Get_Browse_Answer
  End
  If Language then Send PUR||'Quit...'
  Else Send PUR||'Quitter...'
  Answer = ''
  If LogActive then Call LogThis('-','Ending browse mode.')
Return


Browse_File_Total:
  Call Position(70,1)
  If Language then Transmit CEL||BRI||YEL||FilePointer||OFF||GRE||'/'||BRI||YEL||Max||OFF||GRE||'.'
  Else Transmit CEL||BRI||YEL||FilePointer||OFF||GRE||'/'||BRI||YEL||Max||OFF||GRE||'.'
Return

Browse_Prompt:
  Call Position(1,Screen+6)
  If Language Then texte = '?'||OFF||WHI||'=Help '||BLU||'-'||BRI||CYA||' #,8,2,7,1 '||OFF||BLU||'-'||BRI||WHI||' <,>,A,M,R,S,V,Q'
  Else texte = '?'||OFF||WHI||'=Aide '||BLU||'-'||BRI||CYA||' #,8,2,7,1 '||OFF||BLU||'-'||BRI||WHI||' <,>,A,C,M,R,V,Q'
  getuser 7; timeleft = result
  Send CEL||BLU||'['||WHI||Timeleft||BLU||'] '||BRI||CYA||texte||BLU||' >'||OFF
Return


Get_Browse_Answer:
   Do Forever
      GetChar ; Answer = upper(result)
      If Answer = '###PANIC' then Call Quit(1)
      Call Cursor_Convert ; Answer = Result

      Select
        When Answer = '2' then Do
          If Language then Send PUR||'Next.'
          Else Send PUR||'Suivant.'
          Pointer = Pointer + 1
          FilePointer = FilePointer + 1 
          If Pointer >= MaxTotal then Do
            Pointer = 0
            FilePointer = 1
          End
          Move = 'F'
          Leave
        End

        When Answer = '8' then Do
          If Language then Send PUR||'Previous.'
          Else Send PUR||'Precedant.'
          Pointer = Pointer - 1
          FilePointer = FilePointer - 1
          If Pointer < 0 then Do
            Pointer = MaxTotal-1
            FilePointer = Max
          End
          Move = 'B'
          Leave
        End

        When Answer = '7' then Do
          If Language then Send PUR||'First.'
          Else Send PUR||'Premier.'
          Pointer = 0
          FilePointer = 1
          Move = 'F'
          Leave
        End

        When Answer = '1' then Do
          If Language then Send PUR||'Last.'
          Else Send PUR||'Dernier.'
          Pointer = MaxTotal-1
          FilePointer = Max
          Move = 'B'
          Leave
        End

        When Answer = 'R' then Do
          Call Entete
          Call Affiche_Browse_Entete
          Move = 'N'
          Leave
        End

        When Answer = 'M' then do
          Call Mark_File
          Call Browse_Prompt
        End

        When Answer = 'A' then do
          Call Area_Change
          Pointer = 0
          FilePointer = 1
          Call Entete
          Call Affiche_Browse_Entete
          If Max = 0 Then do
            If Language then texte = 'No Files! '||WHI||'Going back to normal mode... '
            Else texte = 'Aucun fichier! '||WHI||'Retournons au mode normal... '
            Call Position(1,Screen+6)
            Send CEL||RED||texte
            Call Position(1,Screen+4) ; Continue
            Answer = 'Q'
          End
          Move = 'F'
          Leave
        End

        When Answer = '>' | Answer = '<' then do
          NewLine
          Call Area_Change_1
          Pointer = 0
          FilePointer = 1
          Call Smart_Entete
          If Max = 0 Then do
            If Language then texte = 'No Files! '||WHI||'Going back to normal mode... '
            Else texte = 'Aucun fichier! '||WHI||'Retournons au mode normal... '
            Call Position(1,Screen+6)
            Send CEL||RED||texte
            Call Position(1,Screen+4) ; Continue
            Answer = 'Q'
          End
          Move = 'F'
          Leave
        End

        When Answer = '#' then do
          Call Position(1,Screen+6)
          if Language then Query CEL||CYA||'Jump at file number'||BLU||' ('||WHI||'1'||BLU||'-'||WHI||Max||BLU||') :'||WHI
          Else Query CEL||CYA||'Sauter au fichier numero'||BLU||' ('||WHI||'1'||BLU||'-'||WHI||Max||BLU||') :'||WHI
          Tmp = result
          If Tmp = '###PANIC' then Call Quit(1)
          If Upper(Tmp) ~= 'RESULT' then do
            If DataType(Tmp) = 'NUM' & Tmp >0 & Tmp <= Max then Do
              FilePointer = Tmp
              Pointer = Tmp - 1
              I = 0 ; Tmp = 0
              If (Comments >0) & (Word(CommentsLines,1) < Pointer + 1) then do
                Do I = I + 1 Until Tmp >= FilePointer
                  Tmp = Word(CommentsLines,I)
                End
                Pointer = Pointer + I-1
              End
              Leave
            End
            Else Do
              Call Position(1,Screen+6)
              If Language then Texte ='Invalid file number!'
              Else Texte ='Numero de fichier invalide!'
              Call WarnPrompt(Texte)
            End
          End
          Call Browse_Prompt
        End

        When Answer = '?' then do
          Call ClrScrn(3,Screen+2)
          Call ShowHelp('Browse')
          Call ClrScrn(3,Screen+2)
          Call ShowHelp('Browse2')
          Call Affiche_Browse_Entete
          Move = 'N'
          Leave
        End

        When Answer = 'S' | Answer = 'C' then do
          Move = 'N'
          Call BrowseSearch
          If upper(TheFile) ~= 'RESULT' then Leave
          Call Browse_Prompt
        End

        When Answer = 'V' Then Do
          Call Read_File
          Call Entete
          Call Affiche_Browse_Entete
          Move = 'N'
          Leave
        End

        When Answer = 'Q' then Leave

        Otherwise NOP
     End
   End
Return


Browse_Read_Data:
  Arg Move
  Do Until Left(Data,1) ~= ' ' | Move = 'N'
    Call Seek('Input',Pointer * LineLength,'B')
    Data = Readln('Input')
    If Left(Data,1) = ' ' & Move ~= 'N' then do
      Select
        When Move = 'F' then Do 
          Pointer = Pointer + 1 
          If Pointer > MaxTotal-1 then Do
            Pointer = 0
            FilePointer = 1
          End
        End
        When Move = 'B' then Do
          Pointer = Pointer - 1
          If Pointer < 0 then Do
            Pointer = MaxTotal - 1
            FilePointer = Max
          End
        End
      End
    End
  End
  Move = ''
Return DelStr(Word(Data,1),1,5)


GetBrowseInfo:
   If Exists(Path||File||'.Desc') then Do
     Call Open('Info',Path||File||'.Desc','R')
     Uploader = Readln('Info')
   End
   Else do
     If Language then Uploader = 'Someone'
     Else Uploader = 'Un Inconnu'
   End
   If Word(Data,1) ~= WHI||File then do
      Call Open('SearchData',Listname,'R')
      Do until Upper(word(Data,1)) = Upper(WHI||File) | Eof('SearchData')=1
         Data = Readln('SearchData')
      End
      Call Close('SearchData')
   End
   Desc = SubStr(Data,60)
   If NumFiDl then Desc = DelStr(Desc,Pos(']',Desc)+1,5)
   Else Desc = DelStr(Desc,1,5)
   Date = Strip(SubStr(Data,43,9))
   FileSize = Strip(SubStr(Data,32,5))
   Call Close('Info')
Return


Affiche_Browse_Entete:
   Call Position(1,3)
   if Language then do
      Transmit CEP||CYA||'Filename'||BLU||':'
      If NumFiDL then do
        Call Position(57,3)
        Transmit CYA||'Downloaded'||BLU||':'
      End
      Transmit CYA||'File Size'||BLU||':'
      NewLine
      Transmit CYA||'Date Uploaded'||BLU||':'
      NewLine
      Transmit CYA||'Description'||BLU||':'
      NewLine
      Transmit UND||CYA||'Long description'||OFF||BLU||':'
   End
   Else Do
      Transmit CEP||CYA||'Fichier'||BLU||':'
      If NumFidl Then Do
        Call Position(59,3)
        Transmit CYA||'Downloads'||BLU||':'
      End
      Transmit CYA||'Longueur'||BLU||':'
      NewLine
      Transmit CYA||'Date Uploade'||BLU||':'
      NewLine
      Transmit CYA||'Description'||BLU||':'
      NewLine
      Transmit UND||CYA||'Longue description'||OFF||BLU||':'
   End
   Call Position(1,Screen+5)
   Transmit BLU||Copies('Ä',79)
Return


Affiche_Browse_Data:
  If Language then Do
     Call Position(10,3)
     Send WHI||Left(File||BLU||'.',29)
     If NumFiDl then do
       Call Position(68,3)
       Transmit WHI||Compress(SubStr(Desc,6,4),'[] ')||CYA||' times'||BLU||'. '
     End
     Call Position(11,4)
     Transmit CEL||WHI||FileSize||BLU||'.'
     Call Position(15,6)
     Transmit CEL||WHI||Date||CYA||', by '||WHI||Uploader||BLU||'.'
     If SubStr(Data,58,1)= '*' then Do
        Call Position(57,6)
        Send BRI||RED||UND||'New Upload'||OFF
     End
  End
  Else Do
     Call Position(9,3)
     Send WHI||Left(File||BLU||'.',29)
     If NumFiDl then do
       Call Position(69,3)
       Transmit WHI||Compress(SubStr(Desc,6,4),'[] ')||CYA||' fois'||BLU||'. '
     End
     Call Position(10,4)
     Send CEL||WHI||FileSize||BLU||'.'
     Call Position(14,6)
     Transmit CEL||WHI||Date||CYA||', par '||WHI||Uploader||BLU||'.'
     If SubStr(Data,58,1)= '*' then Do
        Call Position(59,6)
        Send BRI||RED||UND||'Nouveau Upload'||OFF
     End
  End
  Call Position(13,8)
  If NumFiDl then Transmit WHI||SubStr(Desc,Pos(']',Desc)+2)||CEL
  Else Transmit WHI||Desc||CEL
  Transmit CEL
  NewLine
  I = 0
  Tmp = 7
  If exists(path||File||'.desc') then do
    Call Open('Info',path||File||'.desc','R')
    Texte = Readln('Info') /* Dummy */
    Texte = Readln('Info')
    If Texte = '' then do
      If Language then Send CEL||RED||'No long description available.'
      Else Send CEL||RED||'Aucune description detaillee.'
    End
    Else Transmit CEL||GRE||Texte
    Do Tmp = Tmp + 1
       Texte = Readln('Info')
       If Eof('Info') then leave
       Transmit CEL||GRE||texte
       If Tmp = Screen then Do
         Call Position(1,Screen+6)
         If Language then Send CEL||YEL||'More description - Press any key to view...'
         Else Send CEL||YEL||'Appuyez sur une touche pour la suite de la description...' 
         GetChar
         Call Position(1,Screen+6) ; Send CEL
         Tmp = 6
         Call Position(1,11)
       End
     End
     Call Close('Info')
  End
  Else Do
    If Language then Send RED||'No long description available.'
    Else Send RED||'Aucune description detaillee.'
  End
  Call ClrScrn(5+tmp,Screen-tmp)
Return


BrowseSearch:
  Call Position(1,Screen+6)
  If Language then texte = 'Enter pattern to search'
  Else texte = 'Partie du nom ou de sa description'
  Query CEL||CYA||texte||BLU||':'||WHI
  TheFile = Upper(result)
  If TheFile = 'RESULT' | TheFile = '' then Return
  Call Position(1,Screen+6)
  If Language then Action = 'Searching for '
  Else action = 'Recherchons '
  Send CEL||YEL||Action||WHI||TheFile
  If LogActive then Call LogThis('.','Search for '''TheFile''' pattern.')
  OldPointer = Pointer
  OldFilePointer = FilePointer
  FilePointer = 0
  Do Pointer = 0 to MaxTotal
    Call Browse_Read_Data('N') ; File = Result
    If Left(Data,1) = ' ' then Iterate
    FilePointer = FilePointer + 1
    Desc = SubStr(Data,60)
    If NumFiDl then Desc = DelStr(Desc,Pos(']',Desc)+1,5)
    Else Desc = DelStr(Desc,1,5)
    If Pos(TheFile,Upper(File)) >0 | Pos(TheFile,Upper(Desc)) >0 then Do
       Call Browse_File_Total
       Call GetBrowseInfo
       Call Affiche_Browse_Data
       If Language then texte = 'M'||OFF||WHI||'ark file, '||BRI||CYA||'C'||OFF||WHI||'ontinue scan, '||BRI||CYA||'S'||OFF||WHI||'top scan'
       Else texte = 'M'||OFF||WHI||'arquer ce fichier, '||BRI||CYA||'C'||OFF||WHI||'ontinuer recherche, '||BRI||CYA||'S'||OFF||WHI||'topper recherche'
       Call Position(1,Screen+6)
       Send CEL||BRI||CYA||texte||BLU||' >'
       Do Until Tmp = 'C' | Tmp = 'M' | Tmp = 'S'
         GetChar ; Tmp = upper(result)
         If Tmp = '###PANIC' then Call Quit(1)
         If Tmp = 'M' then Call Mark_File
         If Tmp = 'S' then Leave Pointer
       End
       Call Position(1,Screen+6)
       Transmit CEL||YEL||Action||WHI||TheFile
    End
  End
  Call Position(1,Screen+6)
  If Language then Send CEL||CYA||'Search finished!'||WHI
  Else Send CEL||CYA||'Recherche terminee!'||WHI
  Pointer = OldPointer
  FilePointer = OldFilePointer
  Call Delay(150)
Return


WarnPrompt:
  Parse Arg Texte
  Send CEL||RED||Texte||WHI||
  Call Delay(250)
Return

Open_IO:
  Call Open('Liste1','T:Files.BBS','R')
  Call Open('Output2','T:FileList.New'||node,'W')
  Call Open('Output1','T:Files.BBS'||node,'W')
  Call Open('Liste2','T:FileList'||node,'R')
Return

Close_IO:
  Call Close('Liste1')
  Call Close('Output1')
  Call Close('Input')
  Call Close('Liste2')
  Call Close('Output2')
Return



ASCII_Rtn:
  If LogActive then Do
    If ~Exists(LogFile||Node) then tmp = 'W'
    Else tmp = 'A'
    Call Open('ScrollLog',LogFile||Node,tmp)
    Call LogThis('*',Name||' entered FileScroller.')
    Call LogThis('*','ASCII user. Fallback to normal listing.')
  End
  Cls
  ListFiles
  Newline
  If LogActive then Do
    Call LogThis('*','Exiting FileScroller.')
    Call Close('ScrollLog')
  End
Exit



CleanUp:
  Call Delete('T:ListInfos'||node)
  Call Delete('T:FileSorted'||node)
  Call Delete('T:CutOffList'||node)
  Call Delete('T:Files.BBS'||node)
  Call Delete('T:FileList'||node||'.lha')
  Call Delete('T:FileList'||node||'.txt')
  Call Delete('T:FileList'||(node+10))
  Call Delete('T:ListInfos'||(node+10))
Return

Quit:
Arg tmp
Call CleanUp
If LogActive then Do
  If Tmp then Call LogThis('!','Carrier lost...')
  Else Call LogThis('*','Leaving FileScroller.')
  Call Close('ScrollLog')
End
If Tmp then LogEntry 'Carrier lost in FileScroller.'
Transmit CEL
Exit



SYNTAX:
   Error = rc
   NewLine
   Transmit CEL||RED||'---CRASH!!! Line' SIGL
   Texte = RED||'FileScroller crashed at line '||SIGL||'.'||WHI
   LogEntry Texte
   If LogActive then do
     Call LogThis('!',Texte)
     Call LogThis('!',RED||'Error '||Error||' - '||ErrorText(Error)||WHI)
   End
   Send CEL ; Continue
   Cls
Exit

ERROR:
   Error = rc
   NewLine
   Transmit CEL||RED||'---CRASH!!! Line' SIGL
   Texte = RED||'FileScroller crashed at line '||SIGL||'.'||WHI
   LogEntry Texte
   If LogActive then do
     Call LogThis('!',Texte)
     Call LogThis('!',RED||'Error '||Error||' - '||ErrorText(Error)||WHI)
   End
   Send CEL ; Continue
   Cls
Exit

/* That's it! */

View_File:
  DFM = Upper(Right(File,4))
  If DFM ~= '.GIF' & DFM ~= '.JPG' then return
  Address Command 'VT >NIL: 'Path||File' DELAY=15 HIRES LACE NOBUSY'
Return


PrepFBase:

inp  = 'inp'
inp2 = 'inp2'
out  = 'out'
file. = ''

path 4; fbase = result
path 0; dpath = result
path 9; mpath = result

getuser '28'; ansi  = result

if ansi = 1 then do
  print
  print 'Sorry, only ANSI callers can use the FileScroller.'
  exit 10
end

if ~open(out,'T:Files.BBS','w') then do
  print
  print '\c1Couldn''t create Temp file.'
  exit 10
end

print BLU||'Preparing FBASE Directory.['||CYA||'0   '||BLU||']'||CYA

MaxLines = 0
Comments = 0
first = 0
do i = 1
  setitem i
  iteminfo 1; file.i.1 = left(result,20)
  j = i-1
  if file.i.1 = file.j.1 then leave
  iteminfo 4; type = result
  if type = 'POST' then iterate
  if first = 0 then first = i
  iteminfo 5; file.i.2 = right(result,10)
/*iteminfo 6; file.i.2 = right(result,5)*/
  iteminfo 7; file.i.4 = '['result']'
  iteminfo 10; date = word(result,2)
  if pos('/',date) > 0 then do
    parse var date day'/'month'/'year
    year = right(year,2)
    if length(day) = 1 then day = '0'day
    if length(month) = 1 then month = '0'month
  end
  else do
    parse var date day'-'month'-19'year
    if length(day) = 1 then day = '0'day
    month = pos(month,'   JanFebMarAprMayJunJulAugSepOctNovDec')%3
    if length(month) = 1 then month = '0'month
  end
  file.i.3 = year||month||day
/*file.i.3 = day' 'month' 'year
  if upday = lastcall then do
    file.i.3 = 'Today!   '
    file.i.0 = '*'
  end
  else file.i.0 = ' '*/
  iteminfo 11; file.i.5 = compress(result,'0d'x)
  if pos('\',file.i.5) > 0 then file.i.5 = replace(file.i.5,'\','\\')
  if left(file.i.5,1) = '[' & stripnumfidl then file.i.5 = substr(file.i.5,pos(']',file.i.5)+2)
  if strip(file.i.5) = '' then file.i.5 = '\c7\z4 No Comment Available! \z0'
  If i//5 = 0 then print UP||CSI||'27C'||i
  call smoothcomments()
  writeln(out,file.i.1' 'file.i.2' 'file.i.3' 'file.i.4' 'file.i.5)
/*writeln(out,'\c7'file.i.1' \c6'file.i.2' \c3'file.i.3' \c1'file.i.0' \c2'file.i.4' \c6'left(file.i.5,36))
  MaxLines = MaxLines+1*/
  do j = 6 to commentlines+4
    if file.i.j = '' then iterate
    writeln(out,'\'copies(' ',42)'\c6'file.i.j)
/*  writeln(out,copies(' ',43)'\c6'left(file.i.j,51))
    MaxLines = Maxlines+1
    Comments = Comments+1
    CommentsLines = CommentsLines||MaxLines' '*/
  end
end
close(out)

if first = 0 then do
  print
  print BLU'Sorry, there are no files in this area.'
  exit
end

setitem first
iteminfo 12; path = result
CDPath  = left(path,lastpos('/',path))
start = pos(':',CDPath)+1
len   = length(CDPath)-start
CDName1 = substr(CDPath,start,len)

return


smoothcomments:

do j = 5 to commentlines+4
  if file.i.j = '' then return
  if length(file.i.j) > 35 then do
    k = j+1
    do while length(file.i.j) > 35
      l = lastpos(' ',file.i.j)
      if l = 0 then l = 35
      file.i.k = substr(file.i.j,l+1)' 'file.i.k
      file.i.j = left(file.i.j,l-1)
    end
  end
end

return


replace: procedure

parse arg text, srch, repl

slen = length(srch)
tlen = length(text)

do until tlen = 0
  tlen = lastpos(srch,text,tlen)
  if tlen ~= 0 then do
    text = insert(repl,delstr(text,tlen,slen),tlen-1)
    tlen = tlen-1
  end
end

return text
