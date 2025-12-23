/* ORC - test script      */
options results
parse arg orcport a1
a1 = trim(a1)

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/"'a1
    cnetircvar a1
    orccol '00'||'181b5b'x||'31;43m'
    say '-'result'-'
end
