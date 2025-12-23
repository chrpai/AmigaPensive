/* CNetIRC - TRAP CTCP requests
** Install: '/alias <password> trap:----004 ctcptrap.rexx :'||'01'x
** Requires: scroll.rexx aliased as scroll
** $VER: 1.1 - 11.12.97
*/

ctcpcmds = "CLIENTINFO SOUND TIME FINGER USERINFO XDCC" /* commands */

options results;numeric digits 12               /* NEEDED! */
parse arg orcport unick cmd dest ctcpcmd args
parse var unick ':'unick'!'ident'@'site
parse var ctcpcmd +2 ctcpcmd
ctcpcmd = upper(ctcpcmd)

if cmd == 'PRIVMSG' then do
  if show('P', orcport) then do
    address value orcport

      mynick = cnetircvar nick    /* My nick  */
      cnetircvar p       /* PortData */
      pd = result
      cnetircvar m       /* MainPort */
      mp = result

      if pd ~= 0 then do
        myident = import(offset(null(),pd+546))  /* PortData->user1(68).UUCP(478) */
        maint   = import(offset(null(),pd+44))   /* PortData->SysMaint(44)        */
      end

      if ctcpcmd == 'SOUND' then do
        if args ~= '' then
          /* this script really doesn't play the sample yet */
          cnetirccmd '// notice 'unick' :Hey.  I just heard 'args
        else
          cnetirccmd '// notice 'unick' :Choose sounds from:  beep.snd  scream.snd  squish.snd'
        exit
      end
      if ctcpcmd == 'TIME' then do
        cnetirccmd '// notice 'unick' :The time is:  'time('C')' MST'
        exit
      end
      if ctcpcmd == 'PING' then do
        cnetirccmd '// notice 'unick' :hello.  8)'
        exit
      end
      if ctcpcmd == 'FINGER' then do
        cnetirccmd '// notice 'unick' :Hey.  Its me.  8)'
        exit
      end
      if ctcpcmd == 'XDCC' then do
        call XDCC(args)
        exit
      end
      if ctcpcmd == 'USERINFO' then do
        cnetirccmd '// notice 'unick' :Hey, its just me.'
        exit
      end
      if ctcpcmd == 'CLIENTINFO' then do
        cnetirccmd '// notice 'unick' :'ctcpcmds
        exit
      end
  end
end

XDCC:
parse upper arg type num .

xdccdir  = 'Mail:Users/'myident'/Files' /* user home dir  */
xdccfile = xdccdir'/.XDCC'              /* XDCC data file */

if ~exists(xdccfile) then do
    cnetirccmd '// notice 'unick' :No files offered for XDCC'
    return
end
else do
    call open(fp, xdccfile, 'r')    /* load XDCC data */

    i = 0   /* files - 1 */

    line = readln(fp)
    do while line ~= ''
        parse var line filename.i desc.i
        desc.i = strip(desc.i)
        line = readln(fp)
        i = i + 1
    end

    close(fp)
end

if type == 'LIST' then do
    cnetirccmd '/scroll Sending XDCC list to 'unick'.'

    cnetirccmd '// notice 'unick' :CNet/4 XDCC - /CTCP 'dest' XDCC SEND <num> to send!'

    y = 0
    do while y < i
        cnetirccmd '// notice 'unick' :'y+1': 'filename.y' - 'desc.y
        y = y + 1
    end
end
else if type == 'SEND' then do
    if num == '' then do
        cnetirccmd '// notice 'unick' :You need to specify a file number!'
    end
    else do
        if ((num > 0) & (num < (i+1))) then do
            num = num - 1
            if maint ~= 0 then do;
                sendfile = xdccdir'/'filename.num
            end
            else do
                sendfile = filename.num
            end
            if exists(sendfile) then do
                cnetirccmd '// notice 'unick' :Now sending you 'filename.num' - 'desc.num
                cnetirccmd '/DCC SEND 'unick' 'sendfile
            end
            else do
                cnetirccmd '// notice 'unick' :Error! Filename "'filename.num'" does not exist!'
            end
        end
        else do
            cnetirccmd '// notice 'unick' :Invalid file number 'num
        end
    end
end

return
