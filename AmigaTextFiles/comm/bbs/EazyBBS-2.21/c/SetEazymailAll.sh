# cshell script

#resident `which unspace`
set _except "20;abortline"
cd MB_USER:

if -f UULIB:Aliases
	echo Installing EazyBBS aliases in UULIB:Aliases ...
	foreach user ( * ) {
		set username = `mb:c/unspace $user`
		set _lasterr 0
		Search >nil: UULIB:Aliases $username": " QUIET
		if -r $_lasterr 0 >
			echo Installing alias $username
			echo $username: \"\|mail2eazy $user\" >>UULIB:Aliases
		endif
	}
else
	echo Installing EazyBBS folders in UUMAIL: ...
	foreach user ( * ) {
		set username = `mb:c/unspace $user`
		if -nf uumail:$username
			echo Installing uumail:$username
			echo forward to \|mail2eazy \"$user\" >uumail:$username
		endif
	}
endif

dswap
unset _except
unset user
unset username
#resident -r unspace >nil:
echo Done.

