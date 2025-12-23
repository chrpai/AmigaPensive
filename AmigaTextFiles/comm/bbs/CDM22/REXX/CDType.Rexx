/* CDType 1.0 - Type TEXT files, with Pauses (25 Rows) */

OPTIONS RESULTS

ARG scr flist
datafile=strip(flist)
call open('reader',datafile,'R')
yes=1
b=2
numlist=0
do while ~eof('reader')
   if b >= scr then do
      b=0
      say ' '
      options prompt '[RETURN] to Continue, [Q] to Quit: '
      pull dude
      dude=upper(dude)
      if dude = 'Q' then do
         call close('reader')
         call Done
      end
   end
   dat=readln('reader')
   newdat=substr(dat,1,78)
   say newdat
   b=b+1
end
call close('reader')

Done:
Exit

/* End of CDType Script */

