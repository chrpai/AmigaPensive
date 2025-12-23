/* Rename fido over email packets which are named exactly
 * the same
 */

options results
parse arg args

n=0
good = 0

if Open( in, 'uumail:fido' ,"R") then do
   if Open( out, 'uumail:fido_o',"W") then do

      Do Until EOF( in )
         line=ReadLn(in)
         line=Strip(line)

         if left( line, 55 ) = 'Content-Type: application/octet-stream; name="e616ab81.' then do
            call DateStamp
            call writeln( out, 'Content-Type: application/octet-stream; name="'stamp'"' )
         end
         else
            if left( line, 52 ) = 'Content-Disposition: attachment; filename="e616ab81.' then do
               call writeln( out, 'Content-Disposition: attachment; filename="'stamp'"' )
            end
            else
               call writeln( out, line )

         good = 1
      End
      call close( out )
   end
   call close( in )

   if good then do
      address command 'delete uumail:fido'
      address command 'rename uumail:fido_o uumail:fido'
   end
end

exit

DateStamp:
N=N+1
S=D2x((Time('E')*100)+N)
D=D2x(Time('S'))
Z=D2x(Date('I'))
Stamp=Right(Z||D||S,8)
ExtTod = '.'UPPER(Left(Date('W'),2))'0'
Stamp=Stamp||ExtTod
Return

