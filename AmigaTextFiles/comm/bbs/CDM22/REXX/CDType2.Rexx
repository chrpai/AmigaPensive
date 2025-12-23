/* CDType2 1.0 - Type TEXT files, with Pauses (XX Rows) */

OPTIONS RESULTS

ARG scr flist
datafile=strip(flist)
call open('reader',datafile,'R')
yes=1
b=0
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
   b=b+1
   numlist=numlist+1
   dat=readln('reader')
   num=upper(numlist)
   pathnum=right(num,3,' ')
   pathdat=strip(dat)
   if pathdat ~= '' then do
      say '['pathnum']: 'dat
   end
end
call close('reader')

Done:
Exit

/* End of CDType Script */

