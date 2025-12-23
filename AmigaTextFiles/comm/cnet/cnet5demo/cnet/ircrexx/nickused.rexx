/* ORC - TRAP autoselects a new nick if one is already in use      */
/* /alias <password> trap1----001 nickused.rexx 433                  */
options results
parse arg orcport a1

if show('P', orcport) then do
  address value orcport
    cnetircvar f
    parse var result conn +1 away +1 reg +1 .
    if conn == '+' & reg == '-' then do
      cnetirccmd '/"*!* TRAP Auto-Selecting new nickname'
      cnetirccmd '/nick newnick'
    end
end

