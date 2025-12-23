/********************************************************
 * TIS Startup Script for DLG BB/OS - by Mark Thurgood. *
 *      - Chaotic Knights BBS - +1 416 272 1860 -       *
 ********************************************************/

parse arg UNAME MSGENTER FILESUP SCLENGTH ANSI

BBSName = 'Chaotic Knights BBS - DLG BB/OS - 14.4kbps DS'
TISProg = 'DOORS:TIS/TIS'

say ''
say '  [-- The Immortal Struggle --]'
say '  [--   is now loading...   --]'
say ''

address command TISProg UNAME' 'MSGENTER' 'FILESUP' 'SCLENGTH' 'ANSI

say ''
say 'Returning to: 'BBSName
say ''

exit
