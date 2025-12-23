/* ORC - TRAP ctcp requests                                           */
/* '/alias <password> trap:----004 ctcptrap.rexx :'||'01'x            */
options results
parse arg orcport nick cmd dest ctcpcmd args
parse var nick ':'nick'!'site
parse var ctcpcmd +2 ctcpcmd

if cmd == 'PRIVMSG' then do
  if show('P', orcport) then do
    address value orcport
      if upper(ctcpcmd) == 'SOUND' then do
        if args ~= '' then
          /* this script really doesn't play the sample yet */
          cnetirccmd '// notice 'nick' :Hey.  I just heard 'args
        else
          cnetirccmd '// notice 'nick' :Choose sounds from:  beep.snd  scream.snd  squish.snd'
        exit
      end
      if upper(ctcpcmd) == 'TIME' then do
        cnetirccmd '// notice 'nick' :The time is:  'time('C')' MST'
        exit
      end
      if upper(ctcpcmd) == 'PING' then do
        cnetirccmd '// notice 'nick' :hello.  8)'
        exit
      end
      if upper(ctcpcmd) == 'FINGER' then do
        cnetirccmd '// notice 'nick' :Hey.  Its me.  8)'
        exit
      end
      if upper(ctcpcmd) == 'DCC' then do
        cnetirccmd '// notice 'nick' :Sorry, no DCC support yet.  Please send me a private message.'
        exit
      end
      if upper(ctcpcmd) == 'USERINFO' then do
        cnetirccmd '// notice 'nick' :Hey, its just me.'
        exit
      end
      if upper(ctcpcmd) == 'VERSE' then do
        address command 'orc:]]].rexx 'orcport' 'nick' 'args
        exit
      end
      if upper(ctcpcmd) == 'CLIENTINFO' then do
        cnetirccmd '// notice 'nick' :'||'01'x||'CLIENTINFO VERSE SOUND TIME FINGER USERINFO'
        exit
      end
  end
end

