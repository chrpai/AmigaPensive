
/*

Turns a CNet-BBS CNetToss-log into a toss-report:

      TagName     Toss |   In |  Out | In/Day | Out/Day | In/Toss | Out/Toss
 1 AMIGA_BBS         2 |    5 |    0 |      0 |       0 |       2 |        0
 2 AMIGA            15 |  230 |    0 |     28 |       0 |      15 |        0
 3 CNET             20 |   94 |    0 |     11 |       0 |       4 |        0
 4 NET2410_SYSOP    23 |   91 |    0 |     11 |       0 |       3 |        0
 5 CONSUMER_REPORT  22 |  522 |    0 |     65 |       0 |      23 |        0
 6 AMIGA_TECH       42 |  165 |    0 |     20 |       0 |       3 |        0
 7 AMIGA_PDREVIEW   10 |   32 |    0 |      4 |       0 |       3 |        0
 8 FS               12 |   40 |    0 |      5 |       0 |       3 |        0
 9 FILM             16 | 1056 |    0 |    132 |       0 |      66 |        0
10 ZYXEL            13 |  130 |    0 |     16 |       0 |      10 |        0

Shows how many times each Fidonet echo:
1) Gets tossed.
2) # of msgs inbound.
3) # of msgs outbound.
4) # of msgs in per day.
5) # of msgs out per day.
6) # of msgs in per toss run.
7) # of msgs out per toss run.

Installation:
   1) Copy TossReport.rexx TO Rexx:

   2) Modify CNet:BBSMENU adding the following line as
      "Available Everywhere":
      > TOSSReport | {#0 Rexx:TossReport.rexx "TOSS INBOUND OUTBOUND"}

   3) Edit the variables below marked as "user-settable".

========================================================

Options...      (All options can be abbreviated to just the 1st 4 chars.)
   LISTFILE=FileName : Only display this list of tagnames. (Not yet supported)
   SKIPFILE=FileName : Skip this list of tagnames.         (Not yet supported)
   TAGNAME=TagName   : Only display this one (1) tagname.

   DAYS=n            : Only display tosses made within the last n days.
                       (0=none, 1=today, 2=yesterday, etc...)

   SORT=n            : Sort the display, based on values in column #n.
                       (The default is "no sorting", "display as logged".)
   SORT=-n           : Reverse-sort the display, based on values in column #n.
                       (Not yet supported)

   TOSSES            : Display number of tosses for each sub.
   INBOUND           : Display number of inbound msgs.
   OUTBOUND          : Display number of outbound msgs.
   INDAY             : Display number of inbound msgs per day.
   INTOSS            : Display number of outbound msgs per toss.
   OUTDAY            : Display number of outbound msgs per day.
   OUTTOSS           : Display number of outbound msgs per toss.
   SHOWALL           : Display all seven (7) of the above items.

   LOG=FileName.LOG  : Name of CNetToss log for input. (Default=SysData:Log/Toss)
   PROMPT            : Prompt user for addition options.  (Not yet supported)
   HELP              : See help text.
   TEST              : Display additional debugging code. (Not yet supported)
   NONUMBER          : Don't number the list of tagnames.
   NOHEADER          : Don't display the header-line.
   NOTOTALS          : Don't display the totals-line.
   NONAME            : Don't diplay author credits or prg name.
   DATE              : Display date before each line of output.

A typical CLI usage would be:
   CLI> rx Rexx:TossReport.rexx TOSS INBOUND OUTBOUND
   CLI> Alias Report  rx Rexx:TossReport.rexx TOSS INBOUND OUTBOUND

A typical online usage would be:
   BBSMENU> TOSSReport | {#4 rx Rexx:TossReport.rexx "INBOUND OUTBOUND"}

Other possible online uses:
   BBSMENU> TOSSReport | {#4 rx Rexx:TossReport.rexx SHOWALL}
(If you wish to have the screen-cleared first, add "\f1" right after the "|".)

Similar uses from the CLI:
   CLI> Alias TossReport    rx Rexx:TossReport.rexx SHOWALL
   CLI> Alias TossCalls     rx Rexx:TossReport.rexx INBOUND OUTBOUND

You must be using CNet's own CNetToss prg (NOT TrapToss or other tosser).
You must be running "Toss VERBOSITY 2"    (level 2 or 3).

=========================================================

Bill Beogelein
Box 530441
Livonia, MI 48153
BBS 810-473-2020, Fido 1:2410/207

History:
$VER: TossReport v0.2 Saturday 11-Jun-94 21:14:21
   First release.
   Beta-copy sent to "Michael Rudolph" #432 for testing.

$VER: TossReport v0.1 Thu 12-May-94 16:10:20
   Written and tested for CNet-BBS v3.05c.

====================================================================

Future Plans:
   Please report all bugs and offer any suggestions.
   Add "?" at PROMPT for help-text.
   Tally MATRIX too.
   Show Tosses/Second in totals-line.
   Rewrite in 'C' for speed.

Ignore:
   lz u CNet3:TossReport.LHA CNet:rexx/TossReport.rexx Help:BB_CMDS a:zzzBBSad

*/

/****************** Top of user-settable variables **********************/

logName = "SysData:Log/Toss"     /* Default name of CNetTosser log */
pri     = -1                     /* Run at this task-priority */

/****************** End of user-settable variables ***********************/

options results
parse arg opts

LF='0a'x; CR='0d'x; QU='22'x; BELL='07'x; TAB='09'x;
title= "`TossReport' by Bill Beogelein..."
call pragma('PRIORITY', pri)

if( opts="" ) then
do
   call Help1()
   exit
end

if( ParseArgs("PROMPT") ) then
do
   call writeCH(STDOUT, LF " TossReport.rexx - Enter options: ")
   opts=opts readLN(STDIN)
end

/* if( ParseArgs("NOLOGS") ) then log="" */

o_quiet    = ParseArgs("QUIET")
o_help     = ParseArgs("HELP")
o_test     = ParseArgs("TEST")
o_date     = ParseArgs("DATE")
o_days     = ParseArgs("DAYS"); if(o_days<1) then o_days=99999
o_sort     = ParseArgs("SORT")

o_log      = ParseArgs("LOG=")            /* LOG=SysData:Log/Toss */
if( o_log~="0" ) then logname=o_log       /* Use cmd line passed */

o_listFile = ParseArgs("LISTFILE")        /* LISTFILE=filename */
o_skipFile = ParseArgs("SKIPFILE")        /* SKIPFILE=filename */
o_tagName  = upper(ParseArgs("TAGNAME"))  /* TAGNAME=filename  */

o_tosses   = ParseArgs("TOSSES")
o_in       = ParseArgs("INBOUND")
o_out      = ParseArgs("OUTBOUND")
o_inDay    = ParseArgs("INDAY")
o_outDay   = ParseArgs("OUTDAY")
o_inToss   = ParseArgs("INTOSS")
o_outToss  = ParseArgs("OUTTOSS")
o_showAll  = ParseArgs("SHOWALL")

o_noNumber = ParseArgs("NONUMBER")
o_noHeader = ParseArgs("NOHEADER")
o_noTotals = ParseArgs("NOTOTALS")
o_noName   = ParseArgs("NONAME")

if( opts~="" ) then
do
   say LF "*** Rexx:TossReport.rexx illegal option(s): " opts BELL LF
   call Help1()
   exit
end

if(o_help) then
do
   call Help2()
   exit
end

call LoadIt()
call SortIt()
call ShowIt()

exit     /****************** End of main ***********************/

ShowIt:
   ttlMsgs=0
   if( o_tagName~="0" ) then o_noNumber=1

   if( ~o_noName ) then say LF title LF

   if( ~o_noHeader ) then
   do
      header=""
      if(o_date)      then header=header || " Date  "
      if(~o_noNumber) then header=header || "   "

      header = header || "     TagName     "
      if( o_tosses | o_showall ) then header = header || " Toss |"
      if( o_in     | o_showall ) then header = header || "   In |"
      if( o_out    | o_showall ) then header = header || "  Out |"
      if( o_inDay  | o_showall ) then header = header || " In/Day |"
      if( o_outDay | o_showall ) then header = header || " Out/Day |"
      if( o_inToss | o_showall ) then header = header || " In/Toss |"
      if( o_outToss| o_showall ) then header = header || " Out/Toss"
      say header
   end

/*  tagname=0   inmsgs=1   outmsgs=2  intosses=3  outtoss=4  days=# */
   i=1
   do while( tag.i.0~="0" )
      if( o_noNumber | o_tagname~="0" ) then
         count = " "
      else
         count = right(i, 4)

      tagName =  left(tag.i.0, 16)
      toss    = right(tag.i.3 + tag.i.4, 4) "| "
      in      = right(tag.i.1, 4) "| "
      out     = right(tag.i.2, 4) "| "

      ttlMsgs = ttlMsgs + tag.i.1 + tag.i.2

      inDay   = 0;  if( days>0      ) then inDay   = tag.i.1 % days
      outDay  = 0;  if( days>0      ) then outDay  = tag.i.2 % days
      inToss  = 0;  if( tag.i.3 > 0 ) then inToss  = tag.i.1 % tag.i.3
      outToss = 0;  if( tag.i.4 > 0 ) then outToss = tag.i.2 % tag.i.4

      inDay   = right(inDay  , 6) "| "
      outDay  = right(outDay , 7) "| "
      inToss  = right(inToss , 7) "| "
      outToss = right(outToss, 8)

      if(~o_showAll) then
      do
         if(~o_tosses ) then toss   =""
         if(~o_in     ) then in     =""
         if(~o_out    ) then out    =""
         if(~o_inDay  ) then inDay  =""
         if(~o_outDay ) then outDay =""
         if(~o_inToss ) then inToss =""
         if(~o_outToss) then outToss=""
      end

      if( o_tagName="0" | o_tagName=tagName ) then
      do
         if(o_date) then call writeCH(STDOUT, MyDate())
         say count tagName || toss || in || out || inDay || outDay || inToss || outToss
      end
      i=i+1
   end
   /* say ttlMsgs ttlSecs */
   if(~o_noTotals) then say " " right(days,6) "Day Total  " right(tosses,4) "|"
                                       /* ttlMsgs/ttlSecs "msgs per sec" */
   if(~o_noHeader) then say header
return

LoadIt:
   i=0; inbound.=0; outbound.=0; tag.=0; oldInt=0; days=0; tosses=0; count=0; ttlSecs=1;
   today = date('INTERNAL')
   if( open(fp, logname, 'READ') ) then
   do
      do forever        /* Load all lines of each port */
         i=i+1
         if( eof(fp) ) then leave
         buf = readLN(fp)
         p = pos("; toss begins", buf)
         if( p>0 ) then               /* Sun  8-May-1994  5:31a; toss begins */
         do
            startSecs = date2sec(buf)
            tosses = tosses+1
            int = date2int(buf)
            if( int+o_days <= today ) then good_days=0; else good_days=1
            /* say xxx buf xxx int xxx o_days xxx good_days xxx */
            do forever
               buf = readLN(fp)
               p = pos("; toss ends", buf)
               if( p>0 | eof(fp) ) then
               do
                  ttlSecs = ttlSecs + date2sec(buf) - startSecs
                  /* say xxx buf xxx startSecs xxx date2sec(buf) xxx ttlSecs xxx */
                  leave
               end

               if( pos("; tossed ", buf)>0 & good_days ) then
               do  /*      AMIGA_SYSOP got    35 message(s); tossed     0 message(s) */
                  parse VAR buf tagName " got " in "message(s); tossed" out "message(s)"
                  tagName = strip(tagName)
                  /* say zzz tagName zzz in zzz out zzz */
                  spot       = FindSpot(tagName)
                  tag.spot.0 = tagName
                  tag.spot.1 = tag.spot.1 + in
                  tag.spot.2 = tag.spot.2 + out

                  if( in>0) then tag.spot.3 = tag.spot.3 + 1
                  if(out>0) then tag.spot.4 = tag.spot.4 + 1
               end
            end
            if( oldInt<int & good_days ) then days=days+1
            oldInt=int
            /* say xxx buf xxx LF */
         end
      end
      call close(fp)
   end
   else
   do
      say  LF "*** TossReport.rexx can't open" logname "for reading." LF
      exit
   end
return

MyDate:                 /*** Format current date as "15-Oct-93" ***/
   a2 = date('NORMAL')             /* 01 Jan 1990 */
   a2 = delstr(a2, 8, 2)           /* 01 Jan 90   */
return  compress(a2, ' ')          /* 01Jan90   translate(a2, "-", " ")  */

Help1:
   say title
   say "LISTFILE=FileName   (Not yet supported)"
   say "SKIPFILE=FileName   (Not yet supported)"
   say "TAGNAME=TagName  "

   say "DAYS=n              (0=none, 1=today, 2=yesterday, etc...)"

   say "SORT=n           "
   say "SORT=-n             (Not yet supported)"

   say "TOSSES           "
   say "INBOUND          "
   say "OUTBOUND         "
   say "INDAY            "
   say "INTOSS           "
   say "OUTDAY           "
   say "OUTTOSS          "
   say "SHOWALL          "

   say "LOG=FileName.LOG "
   say "PROMPT              (Not yet supported)"
   say "HELP             "
   say "TEST                (Not yet supported)"
   say "NONUMBER         "
   say "NOHEADER         "
   say "NOTOTALS         "
   say "NONAME           "
   say "DATE             "
   say "(All options can be abbreviated to just the 1st 4 chars.)"
return

Help2:
   say title
   say "LISTFILE=FileName : Only display this list of tagnames. (Not yet supported)"
   say "SKIPFILE=FileName : Skip this list of tagnames.         (Not yet supported)"
   say "TAGNAME=TagName   : Only display this one (1) tagname."

   say "DAYS=n            : Only display tosses made within the last n days."
   say "                    (0=none, 1=today, 2=yesterday, etc...)"

   say "SORT=n            : Sort the display, based on values in column #n."
   say "                    (The default is "no sorting", "display as logged".)"
   say "SORT=-n           : Reverse-sort the display, based on values in column #n."
   say "                    (Not yet supported)"

   say "TOSSES            : Display number of tosses for each sub."
   say "INBOUND           : Display number of inbound msgs."
   say "OUTBOUND          : Display number of outbound msgs."
   say "INDAY             : Display number of inbound msgs per day."
   say "INTOSS            : Display number of outbound msgs per toss."
   say "OUTDAY            : Display number of outbound msgs per day."
   say "OUTTOSS           : Display number of outbound msgs per toss."
   say "SHOWALL           : Display all seven (7) of the above items."

   say "LOG=FileName.LOG  : Name of CNetToss log for input. (Default=SysData:Log/Toss)"
   say "PROMPT            : Prompt user for addition options.  (Not yet supported)"
   say "HELP              : See help text."
   say "TEST              : Display additional debugging code. (Not yet supported)"
   say "NONUMBER          : Don't number the list of tagnames."
   say "NOHEADER          : Don't display the header-line."
   say "NOTOTALS          : Don't display the totals-line."
   say "NONAME            : Don't diplay author credits or prg name."
   say "DATE              : Display date before each line of output."
   say "(All options can be abbreviated to just the 1st 4 chars.)"
return

Date2Int:   PROCEDURE      /* Turn "Sun  8-May-1994  5:31a" into "5955" */
   parse arg dat           /*      "123456789 123456789 */

   b = word(dat, 2)
   parse VAR b d "-" m "-" y

   sorted = y || Mon2Num(m) || right(d,2,'0')
return date('INTERNAL', sorted, 'SORTED')

Date2Sec:   PROCEDURE      /* Turn "Sun  8-May-1994  5:31a;" into "39925955" secs */
   parse arg dat           /*      "123456789 123456789 */

   b = word(dat, 2)
   parse VAR b d "-" m "-" y

   sorted = y || Mon2Num(m) || right(d,2,'0')
   days = date('INTERNAL', sorted, 'SORTED')

   b = word(dat, 3)     /* 5:31a;     */
   parse VAR b h ":" m  /* 5 and 31a; */
   c = substr(m, 3, 1)  /* Get a or p */
   m = left(m, 2)       /* 31a;       */

   secs = (h*60*60) + (m*60)
   if( c='p' ) then secs = secs+(13*60*60)      /* pm */
return (days*24*60*60) + secs

FindSpot:               /* Find next open hole */
   parse arg str
   do i=1 to 999
      if( tag.i.0="0" | str=tag.i.0 ) then leave
   end
   if( i>count ) then count=i
return i

ParseArgs:  PROCEDURE EXPOSE opts    /* Remove options already handled */
   parse arg keywrd
   len    = 4                              /* Max letters to match (3) */
   keywrd = left(keywrd, len)

   i=0
   do forever
      i=i+1
      w = word(opts, i)                               /* Get 1 word */
      if( w="" ) then leave                           /* All done   */
      if( abbrev(upper(w), upper(keywrd), len) ) then /* First LEN chars only */
      do
         ans = 1                                      /* Assume "FOO" only   */
         p   = pos("=", w)                            /* Look for "FOO=123"  */
         if(p>0) then ans=word(substr(w, p+1), 1)     /* Get just "123" part */
         opts = delword(opts, i, 1)                   /* Remove this keyword */
         return ans
      end
   end
return 0               /****** End of ParseArgs() ******/

SortWhich:   /* 1         2      3      4       5        6         7          8    */
        /* TagName      Toss |   In |  Out | In/Day | Out/Day | In/Toss | Out/Toss */
   parse arg which
   if(o_sort=1) then return tag.which.0
   if(o_sort=2) then return tag.which.3 + tag.which.4
   if(o_sort=3) then return tag.which.1
   if(o_sort=4) then return tag.which.2
   if(o_sort=5) then return tag.which.1 / days
   if(o_sort=6) then return tag.which.2 / days
   if(o_sort=7) then return tag.which.1 / (tag.which.3 + tag.which.4)
   if(o_sort=8) then return tag.which.2 / (tag.which.3 + tag.which.4)
return

Swap:
   parse arg a, b
   do iii=0 to 4
      tmp       = tag.a.iii
      tag.a.iii = tag.b.iii
      tag.b.iii = tmp
   end
return

Mon2Num: PROCEDURE
   parse arg mon
   p = pos(mon,  "JanFebMarAprMayJunJulAugSepOctNovDec")
   p = (p%3)+1 /* 1  4  7  10 13 16 19 22 25 28 31 34 */
return right(p, 2, '0')

SortIt:
   if(o_sort<1) then return
   switch = 1
   gap    = count

   do until( switch=0 & gap=1 )
      gap    = max(trunc(gap/1.3),1)
      switch = 0

      do i=1 TO count-gap
         j = i+gap
         if( SortWhich(i) > SortWhich(j) ) then
         do
            call Swap(i, j)
            switch = switch + 1
         end
      end
   end
return

/*** CNet:rexx/TossReport.rexx 810-473-2020 ***/
