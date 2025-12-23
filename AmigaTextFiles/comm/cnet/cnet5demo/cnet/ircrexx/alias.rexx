/* ORC - bypass password security on /alias command          */
/*  WARNING!!!  This is only a sample script to show how to bypass
                a security feature of ORC.  Not for normal use.  */
options results
parse arg orcport a1

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/"*!* ALIAS password security bypassed'
    cnetirccmd '/alias 'orcport' 'a1
end

