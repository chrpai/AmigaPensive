/* ORC - acts like .ircrc file                                    */
options results
parse arg orcport a1

address value orcport
  cnetirccmd '/"*!* INIT .ircrc'
  cnetirccmd '/alias 'orcport' trap1----001 nickused.rexx 433'
  cnetirccmd '/alias 'orcport' trap:----002 pascoetrap.rexx pascoe'
  cnetirccmd '/alias 'orcport' trap:----003 ctcptrap.rexx :'||'01'x
  cnetirccmd '/alias 'orcport' fig /e figlet.rexx '
  cnetirccmd '/alias 'orcport' hi /e hibow.rexx '
  cnetirccmd '/alias 'orcport' bow /e bow.rexx '
  cnetirccmd '/alias 'orcport' scroll /e scroll.rexx '
  cnetirccmd '/alias 'orcport' scr /e status_scroll.rexx '
  cnetirccmd '/alias 'orcport' sc /s chicago.il.us.undernet.org '
  cnetirccmd '/alias 'orcport' sp /s phoenix.az.us.undernet.org '
  cnetirccmd '/alias 'orcport' su /s saltlake.ut.us.undernet.org '
  cnetirccmd '/alias 'orcport' sn /s norman.ok.us.undernet.org '
  cnetirccmd '/alias 'orcport' sv /s vancouver.bc.ca.undernet.org '
  cnetirccmd '/alias 'orcport' sl /s lowell.ma.us.undernet.org '
  cnetirccmd '/alias 'orcport' st /s toronto.on.ca.undernet.org '
  cnetirccmd '/alias 'orcport' sa /s Atlanta.GA.us.undernet.org '
  cnetirccmd '/alias 'orcport' so /s okc.ok.us.undernet.org '
  cnetirccmd '/alias 'orcport' sm /s irc.mo.net '
  cnetirccmd '/alias 'orcport' rc /e .ircrc.rexx'
  cnetirccmd '/alias 'orcport' ali /e alias.rexx '
  cnetirccmd '/alias 'orcport' size /""'||'9b302071'x
  cnetirccmd '/alias 'orcport' aw /e away.rexx '
  cnetirccmd '/alias 'orcport' v /e modev.rexx '
  cnetirccmd '/alias 'orcport' -v /e demodev.rexx '
  cnetirccmd '/alias 'orcport' fkey3 '||'181b5b'x||'10;10R/c'||'0d'x
  cnetirccmd '/alias 'orcport' fkey4 '||'181b5b'x||'17;40R/c'||'0d'x

