/* ORC - Sends random hi-lighted text to destination specified       */
options results
parse arg orcport nick a1
orcport = trim(orcport)
nick = trim(nick)
a1 = trim(a1)

wb = '02'x
wv = '16'x
wu = '1f'x

ii = 0
outstr = ''
do while a1 ~= ''
  parse var a1 wd a1
  if randu(time('s')) > 0.5 then outstr = outstr''wb
  if randu(17*time('s')) > 0.5 then outstr = outstr''wv
  if randu(3*time('s')) > 0.5 then outstr = outstr''wu
  outstr = outstr''wd' '
  ii = ii + 1
end

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/m 'nick' 'outstr
end
