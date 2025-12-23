/* ORC - Scrolls a message on the status line marquee style  -Mustang  
think about messages like

/scroll 5 Users in this channel
/scroll x users are online the BBS currently
/scroll You have been idle for xxx min sec

would be cool

 */
options results
parse arg orcport a1

size = 28
i = 1
address value orcport
cnetircvar d
parse var result col row
col = col-size
row = row-1
a1 = copies(' ',size)||a1||copies(' ',size)
len = length(a1)
do while i < (len-size+2)
  str = substr(a1, i, size)
  i = i+1
  cnetirccmd '/""'||'1b'x||'['row';'col'H'||'1b'x||'[31m'||'1b'x||'[44m'str
  Delay(4)
end
cnetirccmd '/c x'
