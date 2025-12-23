/*
** $VER: Fisher.rexx 1.1 (27.3.94) Keith R. Burby
** -------------------------------------------------------------------------
** ARexx front end for KingFisher database.
** Copyright © 1994 Keith R. Burby
*/

/*
  History:
  -------------------------------------------------------------------------
  v0.0 (12.1.94)

  First Edition.
  -------------------------------------------------------------------------
  v0.1 (13.1.94)

  Moved some code around, nothing anyone would notice.

  Rewrote the menu displays to accomodate a 24 line screen instead of 48
  (silly me).

  Removed search command for later work.

  Added in use flag.

  Entering a negative number will now move to the specified entry.

  Added break/error handling.
  -------------------------------------------------------------------------
  v0.2 (14.1.94)

  Added an improved and more flexible search command.

  Added online help.

  Added ANSI color.
  -------------------------------------------------------------------------
  v0.3 (15.1.94)

  Fisher will now abort if it is set to quit KingFisher upon exit and
  KingFisher is currently running.
  -------------------------------------------------------------------------
  v1.0 (4.3.94)

  First public release.

  Fisher is now able to send requests for selections and entire disks to
  the sysop, if the sysop's name is entered in the variable.

  Removed online help and created doc file for users.
  -------------------------------------------------------------------------
  v1.1 (27.3.94)

  Fixed problem with ANSI colors and command line parameters

  If CLI mode was turned off, as specified in the docs, this script was
  unable to get input from local terminals.
*/

/* ====================================================================== */

/*
** Constants
*/
FALSE = 0
TRUE = ~FALSE
clr = "0C"x
esc = "1B"x

/*
DLG command to allows ANSI colors
%a4
*/

/* ====================================================================== */

/*
** Settings that may need to be changed by the sysop.
**
** Kingfisher.Path  = Complete path to find the KingFisher program.
** Kignfisher.Prefs = Complete path to find the prefs file for KingFisher.
** KingFisher.Port  = Name of KingFisher port to conduct business on.
** KingFisher.Quit  = Whether or not KingFisher should quit on termination of
**                    this script.
** Sysop            = Sysop's name.  Note, all spaces should be changed to
**                    underscores, ala My_Name.
** RightMargin      = Right margin for word wrapping.
*/

KingFisher.Path  = "DH1:Database/KingFisher"
KingFisher.Prefs = "DOORS:Fisher/KingFisher.cfg"
KingFisher.Port  = "KINGFISHER1"
KingFisher.Quit  = TRUE

Sysop = "Keith_Burby"

RightMargin = 77

/* ====================================================================== */

options failat 20
options results
signal on Break_C
signal on Break_D
signal on Break_E
signal on Break_F
signal on Halt
signal on Syntax

if ~Show("P", KingFisher.Port) then
  do
    address "COMMAND" "Run >NIL:" KingFisher.Path "SETTINGS=" || KingFisher.Prefs "NOABOUT STAYBEHIND"
    address "COMMAND" "WaitForPort" KingFisher.Port
    if ~Show("P", KingFisher.Port) then
      do
        say clr
        say "I'm sorry, but the KingFisher program is currently not available for use."
        say "Please inform the SysOp about this problem."
        say
        call GetInput("-- Press any key to return to the BBS --", "I")
        signal Halt
      end
    call SetClip("KingFisher") /* Nobody can be using it if it's not running. */
  end
else
  if KingFisher.Quit = TRUE then
    do
      say clr
      say "I'm sorry, but KingFisher is currently being used by another person."
      say "Please try again at a later time."
      say
      call GetInput("-- Press any key to return to the BBS --", "I")
      signal Halt
    end

if GetClip("KingFisher") ~= "" then
  do
    say clr
    say "I'm sorry, but KingFisher is currently being used by another person."
    say "Please try again at a later time."
    say
    call GetInput("-- Press any key to return to the BBS --", "I")
    signal Halt
  end

call SetClip("KingFisher", "IN USE")
interpret "address" "'" || KingFisher.Port || "'"

parse Arg ANSI UserName

if (ANSI ~= "") & (ANSI ~= "TRUE") & (ANSI ~= 0) then
  do
    BR = esc || "[33m" /* brown    */
    DC = esc || "[36m" /* Dark Cyan  */
    DG = esc || "[32m" /* Dark Green */
    LG = esc || "[37m" /* Light Grey */
  end
else
  do
    BR = ""
    DC = ""
    DG = ""
    LG = ""
  end

/* Misc information */
VERSION
VerStr = Result
TELL FISHCOUNT
LastFish = Result
TELL DISKCOUNT
LastDisk = Result

/* Initialize the default search parameters */
RESET
Search.Mode = TRUE    /* TRUE = Title, FALSE = Description     */
Search.Direction = FALSE /* TRUE = Forward, FALSE = Backward      */
Search.Range = FALSE   /* TRUE = Active, FALSE = Inactive      */
Search.Case = FALSE    /* TRUE = Case Sensitive, FALSE = Insenstive */
Search.Low = 0
Search.High = 0
Search.For = ""

Cmd = ""
ShowMenu = TRUE

do until Cmd = "Q"

  TELL FISH
  CurrFish = Result
  TELL DISK
  CurrDisk = Result
  SHOW TITLE
  fTitle = Result
  SHOW DESCRIPTION
  fDescription = FormatString(Result, RightMargin);
  call ShowTitle
  say DC || "Current entry: " || LG || CurrFish "(of" LastFish || ")"
  say DC || "Current disk:  " || LG || CurrDisk "(of" LastDisk || ")"
  say DC || "Entry name:    " || LG || fTitle
  say DC || Copies("-", RightMargin)

  if ShowMenu then
    do
      /* Display the main menu. */
      say DC || "Options:"
      say
      say LG || " F" || DC || Left(") First Entry", RightMargin % 2 - 2),
          LG || " L" || DC || ") Last Entry"
      say LG || " P" || DC || Left(") Previous Entry", RightMargin % 2 - 2),
          LG || " N" || DC || ") Next Entry"
      say LG || " -" || DC || Left(") Previous Disk", RightMargin % 2 - 2),
          LG || " +" || DC || ") Next Disk"
      say LG || " <" || DC || Left(") Previous Version", RightMargin % 2 - 2),
          LG || " >" || DC || ") Next Version"
      say LG || " S" || DC || Left(") Begin Search", RightMargin % 2 - 2),
          LG || " C" || DC || ") Continue Search"
      if Sysop ~= "" then
        say LG || " R" || DC || ") Make Request"
      say LG || " 1-" || LastDisk || DC || ") specific disk."
      say LG || " Q" || DC || ") Quit Fisher."
      say
      say " <" || LG || "Enter" || DC || "> toggles between the menu and entry description."
    end
  else
    say LG || fDescription /* The description of the current entry */
  say

  Cmd = GetInput(DC || "Enter your command: " || LG, "U")
  if ~DataType(Cmd, "N") then
    Cmd = Left(Cmd, 1)
  select
    when Cmd = "" then
      ShowMenu = ~ShowMenu
    when Cmd = "F" then
      MOVETO FIRST FISH
    when Cmd = "L" then
      MOVETO LAST FISH
    when Cmd = "P" then
      MOVETO PREV FISH
    when Cmd = "N" then
      MOVETO NEXT FISH
    when Cmd = "-" then
      MOVETO PREV DISK
    when Cmd = "+" then
      MOVETO NEXT DISK
    when Cmd = "<" then
      MOVETO PREV VERSION
    when Cmd = ">" then
      MOVETO NEXT VERSION
    when Cmd = "S" then
      call Search()
    when Cmd = "C" then
      do
        SEARCH AGAIN
        if RC ~= 0 then
          call GetInput("No matches found.  Press any key.", "I")
      end
    when Cmd = "R" then
      if Sysop ~= "" then
        call MakeRequest(CurrDisk, fTitle)
    otherwise
      if DataType(Cmd, "N") & ((Cmd >= 1) | (Cmd <= LastDisk)) then
        MOVETO DISK Cmd
      if DataType(Cmd, "N") & ((Cmd <= -1) | (Cmd >= -LastDisk)) then
        MOVETO FISH (-1 * Cmd)
  end

end

if KingFisher.Quit then
  QUIT

Halt:
call SetClip("KingFisher", "")
exit

/* ====================================================================== */

/*
** Break and errors handling routine.
*/
Syntax:
say FormatString("An error occured on line" SIGL || ".", RightMargin)
say FormatString("Please inform the SysOp of this.", RightMargin)
call GetInput("-- Press any key to return to the BBS --", "I")

Break_C:
Break_D:
Break_E:
Break_F:
call SetClip("KingFisher", "")
exit

/* ====================================================================== */

/*
** The Search Menu.
*/
Search:

do until (sCmd = "Q") | (sCmd = "B")

  if Search.Mode then
    sMode = "TITLE"
  else
    sMode = "DESCRIPTION"
  if Search.Direction then
    sDirection = "FORWARD"
  else
    sDirection = "BACKWARD"
  if Search.Range then
    sRange = Search.Low || "-" || Search.High
  else
    sRange = "DISABLED"
  if Search.Case then
    sCase = "SENSITIVE"
  else
    sCase = "IGNORED"

  call ShowTitle
  say DC || "Mode:      " || LG || Left(sMode, RightMargin % 2 - 11),
      DC || "Current entry: " || LG || CurrFish "(of" LastFish || ")"
  say DC || "Direction: " || LG || Left(sDirection, RightMargin % 2 - 11),
      DC || "Current disk:  " || LG || CurrDisk "(of" LastDisk || ")"
  say DC || "Range:     " || LG || sRange
  say DC || "Case:      " || LG || sCase
  say DC || "String:    " || BR || '"' || LG || Search.For || BR || '"'
  say DC || Copies("-", RightMargin)
  say DC || "Options:"
  say LG || " M" || DC || ") Change Mode      " || LG || "[TITLE | DESCRIPTION]"
  say LG || " D" || DC || ") Change Direction " || LG || "[FORWARD | BACKWARD]"
  say LG || " R" || DC || ") Change Range     " || LG || "[DISABLED | max 1-" || LastDisk || "]"
  say LG || " C" || DC || ") Change Case      " || LG || "[IGNORED | SENSITIVE]"
  say LG || " S" || DC || ") Change String"
  say LG || " B" || DC || ") Begin search"
  say LG || " Q" || DC || ") Quit search menu"
  say

  sCmd = Left(GetInput("Enter your search command: " || LG, "U"), 1)
  select
    when sCmd = "M" then
      Search.Mode = ~Search.Mode
    when sCmd = "D" then
      Search.Direction = ~Search.Direction
    when sCmd = "R" then
      do
        Search.Range = ~Search.Range
        if Search.Range then
          do
            Search.Low = GetInput("Enter the low disk: ", "N")
            if (Search.Low < 1) | (Search.Low > LastDisk) then
              Search.Low = 0
            Search.High = GetInput("Enter the high disk: ", "N")
            if (Search.High < Search.Low) | (Search.High > LastDisk) then
              Search.High = 0
            if (Search.Low = 0) | (Search.High = 0) then
              Search.Range = FALSE
          end
      end
    when sCmd = "C" then
      Search.Case = ~Search.Case
    when sCmd = "S" then
      Search.For = GetInput("Enter the search string: ", "M")
    otherwise
      nop
  end

end

if (sCmd = "B") & (Search.For ~= "") then
  do
    SEARCH MODE sMode
    SEARCH DIRECTION sDirection
    SEARCH CASE sCase
    SEARCH RANGE sRange
    if Search.Range then
      SEARCH RANGE ACTIVE
    SEARCH FOR Search.For
    if RC ~= 0 then
      call GetInput("No matches were found.  Press any key", "I")
  end

drop sMode
drop sDirection
drop sRange
drop sCase
drop sCmd

return

/* ====================================================================== */

/*
** Allow the user to make a request for a specific title or disk.
*/

MakeRequest:

parse arg DiskNum, Selection

ReqSel = ""
do until ReqSel ~= ""
  call ShowTitle
  say
  say DC || "Current entry: " || LG || CurrFish "(of" LastFish || ")"
  say DC || "Current disk:  " || LG || CurrDisk "(of" LastDisk || ")"
  say DC || "Entry name:    " || LG || fTitle
  say DC || Copies("-", RightMargin)
  say DG || FormatString("Did you want to request the current selection, or the entire disk?", RightMargin)
  say LG || " S" || DG || ") Current selection"
  say LG || " D" || DG || ") Entire disk"
  say LG || " Q" || DG || ") Quit (Don't send request)"
  say

  ReqSel = GetInput(DC || "Which would you like? " || LG, "U")

  select
    when ReqSel = "S" then
      call SendRequest("S")
    when ReqSel = "D" then
      call SendRequest("D")
    when ReqSel = "Q" then
      nop
  end
end

drop ReqSel
return

/* ====================================================================== */

/*
** Send the request to the sysop.
*/
SendRequest:

parse arg ReqType

call Open("Request", "T:FisherReq", "Write")
call WriteLn("Request", "USER:   " UserName)
if ReqType = "S" then
  do
    call WriteLn("Request", "TITLE:  " fTitle)
    call WriteLn("Request", "ON DISK:" CurrDisk)
  end
else
  call WriteLn("Request", "DISK:   " CurrDisk)
call Close("Request")

address "COMMAND" "DLG:SendMsg -f Fisher.rexx -s Fish Request -b T:FisherReq -q -r" Sysop
address "COMMAND" "Delete T:FisherReq quiet"

return

/* ====================================================================== */

/*
** Display Fisher's title bar.
*/
ShowTitle:
say clr || BR || Copies("*", RightMargin) || LG
say Trim(Center("Fisher 1.1 Copyright 1994 Keith R. Burby", RightMargin))
say Trim(Center(VerStr, RightMargin))
say BR || Copies("*", RightMargin) || DG
return

/* ====================================================================== */

/*
** Formats the string into a multi-line string where each line ends on or
** before the specified right margin.
*/
FormatString: procedure
nl = "0A"x

parse arg String, Margin
fString = ""
do while Length(String) > Margin
  bPos = Max(LastPos(" ", String, Margin), LastPos("/", String, Margin), LastPos("-", String, Margin))
  if bPos = 0 then
    bPos = Margin
  fString = fString || Strip(Left(String, bPos), "T") || nl
  String = Strip(SubStr(String, bPos + 1), "L")
end /* until */
fString = fString || String
return fString

/* ====================================================================== */

/*
** Get input from the user.
*/
GetInput: procedure

parse arg PromptStr, Mode
Mode = Upper(Left(Mode, 1))
options prompt PromptStr
if Upper(Left(Mode, 1)) = "U" then
  parse upper pull Response
else
  parse pull Response
if (Mode = "N") & ~DataType(Response, "N") then
  Response = 0
return Response
