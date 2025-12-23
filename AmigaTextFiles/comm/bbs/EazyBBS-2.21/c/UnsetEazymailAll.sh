# cshell script

#resident `which unspace`
set _except "20;abortline"
cd MB_USER:

if -f UULIB:Aliases
	echo Removing EazyBBS aliases from UULIB:Aliases ...
	foreach user ( * ) {
		set username = `mb:c/unspace $user`
		set _lasterr 0
		Search >nil: UULIB:Aliases $username": " QUIET
		if -nr $_lasterr 0 >
			echo Removing alias $username
			# ignore: op:killline $username": \*\"\*\|mail2eazy "$user\*\"
			# Don't ask why, but it work's only this way.
			# Cshell has a special internal representation of usernames with spaces in them.
			MB:C/KillLine <UULIB:Aliases >UULIB:Aliases.new $username": \*\"\*\|mail2eazy "$user\*\"
			Copy UULIB:Aliases.new UULIB:Aliases QUIET
			Delete UULIB:Aliases.new QUIET
		endif
	}
else
	echo Removing EazyBBS folders from UUMAIL: ...
	foreach user ( * ) {
		set username = `mb:c/unspace $user`
		set folderlen = @filelen( uumail:$username )
		if -r $folderlen 50 <
			echo Removing uumail:$username
			rm uumail:$username
		endif
	}
endif

dswap
unset _except
unset user
unset username
unset killline
#resident -r unspace >nil:
echo Done.

