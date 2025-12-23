/* ORC - sets the user away with channel message.       */
options results
parse arg orcport a1
a1 = trim(a1)

if show('P', orcport) then do
  address value orcport
    if length(a1) > 0 then
      cnetirccmd '/me is AWAY -- 'a1
    else
      cnetirccmd '/me is BACK.'
    cnetirccmd '/away 'a1
end

