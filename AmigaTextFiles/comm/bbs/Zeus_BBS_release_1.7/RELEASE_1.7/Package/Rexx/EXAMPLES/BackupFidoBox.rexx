/*
 * BackupFidoBox.rexx by Neil Williams (10.6.2000), based on code from:
 * BackupLogs.rexx by Neil Williams (30.10.98), based on code from:
 *
 * Backup.rexx By Dave Naylor 25-1-97
 *
 */

options results

KIDD = DATE(S)

Address Command

if exists( 'uumail:fido' ) then do
    ft = 'Mail:Back/fido.'||KIDD
		fn = ft
    n=0
    do while exists( fn )
      fn = ft || '.' || n
      n=n+1
    end

    'Copy uumail:fido to '|| fn ||' CLONE'
    if RC = 0 then do
       'delete' 'uumail:fido'
    end
end

if exists( 'uumail:neil' ) then do
    ft = 'Mail:Back/neil.'||KIDD
		fn = ft
    n=0
    do while exists( fn )
      fn = ft || '.' || n
      n=n+1
    end

    'Copy uumail:neil to '|| fn ||' CLONE'
end


EXIT

