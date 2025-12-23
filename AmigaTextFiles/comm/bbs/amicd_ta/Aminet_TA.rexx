/*
   ************************************* 
   *  Auto Convert                     *
   *                                   *
   *  Written by Kevin Tankard         *
   *  TA Sysop # 5015                  *
   *                                   *
   *  Copyright SAS Software 1996      *
   *                                   *
   *************************************

   Auto convert Aminet CD(s) for use
   with Transamiga. Creates shadow
   directories on hard disk, converts
   Files.BBS to TA format and creates
   File.CFG segment.

   * NOTE *
   
   Hard disk destination *MUST* be a
   daughter directory off whatever 
   root directory you choose and
   variable 'dest' *MUST* contain
   a trailing '/'.
   
   IE. dest = 'ROOT:Daughter/'

*/

OPTIONS RESULTS

/* Configuration */

src   = 'CD0:Aminet/'            /* Source directory */
dest  = 'FILE:Aminet_shadow/'    /* Hard drive shadow directory */
wkdir = 'RAM:'                   /* For temp files */

/* Variables for File.cfg */

cfgname = 'AMINET9.cfg'          /* Temp name of cfg file */
asgn    = 'AMINET:'              /* Assignment for use with File.CFG */
read    = 10                     /* Set download access */
write   = 255                    /* Set Upload access (must be high for CD) */
acc     = 5                      /* Set List files access */
flags   = 0                      /* Set access flags (1 = 'X-------') */

/* End Configuration */

/* Leave these variables alone !! */

space = '                       '
lf = '0a'x
PARSE VAR dest device':'junk
device = device||':'
num = '0'

/* Check for src and dest */

PRAGMA('W','N')

IF ~EXISTS(src) THEN DO
   SAY 'No CD in drive OR Wrong CD in drive'
   SAY 'Insert Aminet CD in drive and re-run program.'
   EXIT
END

IF ~EXISTS(dest) THEN DO
   SAY dest 'does not exist.'
   PARSE VAR dest dev':'rest
   dev = dev||':'
   IF EXISTS(dev) THEN DO
      SAY 'Create '||dest||' directory ??'
      PULL ans
      IF ans ~= 'Y' THEN DO
         SAY 'Please manually create the directory or re-configure'
         EXIT
      END
      PARSE VAR dest hdir'/'
      ADDRESS COMMAND 'makedir '||hdir
      IF rc = 0 THEN SAY dest||' created !!'
   END
END

/* Make temp control file in WKDIR */

SAY 'Examining CD path structure....'
ADDRESS COMMAND 'list >'||wkdir||'TDlist '||src||'#? lformat %S%S/ dirs all'

/* Create hard disk shadow directories */

SAY 'Creating hard disk directories and Files.BBS'

CALL OPEN(in,wkdir||'TDList','r')
DO WHILE ~EOF(in)
   str = READLN(in)
   IF str ~= '' THEN DO
      PARSE VAR str junk'/'hdir
      str1 = REVERSE(hdir)
      PARSE VAR str1 junk'/'hdir
      hdir = REVERSE(hdir)
      IF ~EXISTS(dest||hdir) THEN DO
         ADDRESS COMMAND 'makedir '||dest||hdir
         CALL FBBS
      END
      ELSE DO
         IF ~EXISTS(dest||hdir||'/Files.BBS') THEN DO
            CALL FBBS
         END
      END
   END
END
CALL CLOSE(in)
CALL FCFG
DELETE(wkdir||'TDList)

SAY 
SAY num||' File Areas in '||cfgname
SAY
SAY 'All Done !!'

EXIT

/* Convert Files.BBS */

FBBS:

IF EXISTS(str||'Files.BBS') THEN DO
   CALL OPEN(fbbs,str||'Files.BBS','r')
   CALL OPEN(out,dest||hdir||'/Files.BBS','w')
   DO WHILE ~EOF(fbbs)
      str = READLN(fbbs)
      IF str = '' THEN ITERATE
      PARSE VAR str fn jsize desc
      desc = STRIP(desc)
      CALL SIZER
      size = LEFT(space,7-l)||size
      l = LENGTH(fn)
      fn = fn||LEFT(space,23-l)
      date = COMPRESS(DATE(o),'/')
      out_str = fn||' '||size||' '||date||' '||desc
      WRITELN(out,out_str)
   END
   CALL CLOSE(fbbs)
   CALL CLOSE(out)
END
RETURN

SIZER:

size = COMPRESS(jsize,'kK')
post = ''
IF jsize = '?' THEN size = '0'
IF RIGHT(size,1) = 'M' THEN DO
   size = COMPRESS(size,'M')
   PARSE VAR size pre'.'post
   size = (pre * 1024)
   post = ((post * 1024) * 100)
END
IF post ~= '' THEN size = (size * 1024) + post
ELSE size = size * 1024
l = LENGTH(size)
IF l > 7 THEN DO
   size = RIGHT(size,7)
   desc = '>10meg '||LEFT(desc,33)
   l = 7
END
IF size = '0' THEN DO
   s_str = STATEF(str||fn)
   PARSE VAR s_str type size .
END
RETURN

FCFG:

SAY 'Creating '||cfgname

CALL OPEN(in,wkdir||'TDlist','r')
DO WHILE ~EOF(in)
   str = READLN(in)
   IF str = '' THEN ITERATE
   IF EXISTS(str||'Files.BBS') THEN DO
      IF EXISTS(device||cfgname) THEN CALL OPEN(cfg,device||cfgname,'a')
      ELSE CALL OPEN(cfg,device||cfgname,'w')
      CALL OPEN(index,str||'INDEX','r')
      na = READLN(index)
      CALL CLOSE(index)
      PARSE VAR na junk':'name
      name = STRIP(name)
      PARSE VAR str junk':'hdir
      
      /* Create and Write file area entry to File.cfg */
      
      entry = name||lf||asgn||hdir||lf||read||lf||write||lf||acc||lf||flags||lf||lf
      WRITELN(cfg,entry)
      num = num + 1
      CALL CLOSE(cfg)
   END
END
CALL CLOSE(in)
RETURN
