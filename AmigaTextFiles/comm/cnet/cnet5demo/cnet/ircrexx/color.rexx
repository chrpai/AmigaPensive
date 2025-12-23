/* ORC - color prefs script      */
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
    orccmd '/"*!* LOAD Color Prefs:  'a1
    orccmd '/""'||'1b'x||'[>6m'              /* set window bg color */
    orccmd '/col 00 '||'1b'x||'[31;44m'      /* C_STATUSLINE */
    orccmd '/col 01 '||'1b'x||'[0;31;40m'    /* C_INPUTLINE */
    orccmd '/col 02 '||'1b'x||'[33;40m'      /* C_SYSLINEOUT */
    orccmd '/col 03 '||'1b'x||'[37;40m'      /* C_USERECHOOUT */
    orccmd '/col 04 '||'1b'x||'[31m'         /* C_DEBUGIN */
    orccmd '/col 05 '||'1b'x||'[31m'         /* C_DEBUGOUT */
    orccmd '/col 06 '||'1b'x||'[34;40m'      /* C_NORMLINETXT */
    orccmd '/col 07 '||'1b'x||'[32m'         /* C_NORMLINEHDR */
    orccmd '/col 08 '||'1b'x||'[35m'         /* C_OFFCHANTXTHDR */
    orccmd '/col 09 '||'1b'x||'[36m'         /* C_PRIVMSGHDR */
    orccmd '/col 10 '||'1b'x||'[37m'         /* C_ALERT */
end



