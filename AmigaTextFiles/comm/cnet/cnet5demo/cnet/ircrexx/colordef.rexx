/* ORC - default color prefs script      */
options results
parse arg orcport a1
a1 = trim(a1)

/*  attributes:  0 = attrib off
                 1 = bold
                 3 = italic (Amiga only)
                 4 = underscore
                 5 = blink (PC only)
                 7 = reverse vid
                 8 = invisible

  3X = foreground   4X = background

  X = 0  black        4  blue
      1  red          5  magenta
      2  green        6  cyan
      3  yellow       7  white
*/

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/"*!* LOAD Color Prefs:  'a1
    cnetirccmd '/""'||'1b'x||'[>0m'              /* set window bg color */
    cnetirccmd '/col 00 '||'1b'x||'[37;44m'      /* C_STATUSLINE */
    cnetirccmd '/col 01 '||'1b'x||'[0;32;40m'    /* C_INPUTLINE */
    cnetirccmd '/col 02 '||'1b'x||'[37;40m'      /* C_SYSLINEOUT */
    cnetirccmd '/col 03 '||'1b'x||'[33;40m'      /* C_USERECHOOUT */
    cnetirccmd '/col 04 '||'1b'x||'[34m'         /* C_DEBUGIN */
    cnetirccmd '/col 05 '||'1b'x||'[35m'         /* C_DEBUGOUT */
    cnetirccmd '/col 06 '||'1b'x||'[36;40m'      /* C_NORMLINETXT */
    cnetirccmd '/col 07 '||'1b'x||'[32m'         /* C_NORMLINEHDR */
    cnetirccmd '/col 08 '||'1b'x||'[34m'         /* C_OFFCHANTXTHDR */
    cnetirccmd '/col 09 '||'1b'x||'[31m'         /* C_PRIVMSGHDR */
    cnetirccmd '/col 10 '||'1b'x||'[31m'         /* C_ALERT */
end



