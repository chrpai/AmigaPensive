/* ORC - creates rainbow colored message with standard esc codes       */
options results
parse arg orcport nick mynick a1
orcport = trim(orcport)
mynick = trim(mynick)
nick = trim(nick)
a1 = trim(a1)

wb = '02'x
wv = '16'x
wu = '1f'x

outstr = '[' wb 'THWAP!' wb ']'

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/NOTICE 'mynick' Hey' 'nick': 'outstr
end
