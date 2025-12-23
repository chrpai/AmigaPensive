/* ORC - color cycle backfield      */
options results
parse arg orcport

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/"*!* ATTENTION!!!!  8)'
    cnetirccmd '/""'||'181b5b'x||'>1m'
    cnetirccmd '/""'||'181b5b'x||'>2m'
    cnetirccmd '/""'||'181b5b'x||'>3m'
    cnetirccmd '/""'||'181b5b'x||'>4m'
    cnetirccmd '/""'||'181b5b'x||'>5m'
    cnetirccmd '/""'||'181b5b'x||'>6m'
    cnetirccmd '/""'||'181b5b'x||'>7m'
    cnetirccmd '/""'||'181b5b'x||'>0m'
end
