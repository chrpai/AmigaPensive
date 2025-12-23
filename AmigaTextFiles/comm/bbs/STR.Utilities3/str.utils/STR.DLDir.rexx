/*

    STR.DLDir.rexx - Download a *WHOLE* directory in E!  Be careful with
                     this one folks!

    Version 1.01

*/

options results

if ~show('l','rexxsupport.library') then do
  if ~addlib('rexxsupport.library',0,-30,0) then do
    print 'RexxSupport.Library missing! Can''t continue...'
    exit 10
  end
end

print
query '\c6Enter the path to the directory \c7(\c6include \c7":" \c6or \c7"/")\c6:\n2>\c7'
dir = result

if dir = '' then exit

if ~exists(dir) then do
  print
  print '\c1That directory doesn''t exist!\n1'
  exit
end

files = showdir(dir,'f','01'x)
files = translate(files,'02'x,' ')
files = translate(files,' ','01'x)

print
do i = 1 to words(files)
  file = translate(word(files,i),' ','02'x)
  sendstring '\c6'file' \c7- \c2'
  markfile dir||file
  print '\c2Marked!'
end
print

print '\c6Finished!  Marked 'words(files)' files.  Use \c7DL\c6 to download the files.\n1'

exit
