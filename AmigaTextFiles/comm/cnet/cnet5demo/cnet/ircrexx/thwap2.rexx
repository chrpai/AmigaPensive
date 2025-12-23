/* ORC - creates rainbow colored message with standard esc codes       */
options results
parse arg orcport nick mynick a1
orcport = trim(orcport)
mynick = trim(nick)
nick = trim(nick)
a1 = trim(a1)

wb = '02'x
wv = '16'x
wu = '1f'x

outstr = wb'THWAP'wb's the shit out of 'nick

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/me 'outstr
end
