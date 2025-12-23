/*************************************************************************/
/*                   SysopAvail.rexx v1.0  by Morpheus                   */
/*                                                                       */
/*          Utility to set/reset the sysop-avail flag for PagE!r         */
/*                                                                       */
/*          Usage: "rx SysopAvail.rexx 20 40" to open the window         */
/*               at position (20,40), default is (448,112)               */
/********************************* Script ********************************/
options results
parse arg x y .
def_x=448
def_y=112
avail_flag=1
call setclip('SysopAvail','TRUE')
host='AVAILHOST'
port='AVAILPORT'
if ~show(l,'rexxarplib.library') then;do
if ~addlib("rexxarplib.library",0,-30,0) then;do
say "Can't open rexxarplib.library"
exit
end
end
if ~show(l,'rexxsupport.library') then;do
if ~addlib("rexxsupport.library",0,-30,0) then;do
say "Can't open rexxsupport.library"
exit
end
end
MaxCols=ScreenCols()-192
MaxRows=ScreenRows()-52
if ~datatype(x,W) then x=def_x
if ~datatype(y,W) then y=def_y
if x<0 then x=0
if y<0 then y=0
if x>MaxCols then x=MaxCols
if y>MaxRows then y=MaxRows
address AREXX "'call CreateHost("host","port")'"
do 50 while ~show('P',host)
call delay(10)
end
if ~show('P',host) then;do
call Request(112,100,'Error: Could not create controlport for SysopAvail',,' OKAY ',,)
exit
end
idcmp='GADGETUP+CLOSEWINDOW'
flags='WINDOWDRAG+WINDOWDEPTH+WINDOWCLOSE'
call OpenWindow(host,x,y,192,52,idcmp,flags,"SysopAvail v1.0")
call AddGadget(host,12,36,1,' Avail ',1)
call AddGadget(host,87,36,2,' Not Avail ',2)
call Show_Text
quitflag=0
call openport(port)
do until quitflag=1
call waitpkt(port)
packet=getpkt(port)
if packet ~=null() then;do
cmd=getarg(packet)
select
when cmd=CLOSEWINDOW then quitflag=1
when cmd='1' then;do
avail_flag=1
call setclip('SysopAvail','TRUE')
call Show_Text
end
when cmd='2' then;do
avail_flag=0
call setclip('SysopAvail','FALSE')
call Show_Text
end
otherwise nop
end
call reply(packet,0)
end
end
call setclip('SysopAvail')
call CloseWindow(host)
call ClosePort(port)
exit
Show_Text:
if avail_flag=1 then call WindowText(host,"Sysop is available!")
else call WindowText(host,"Sysop not available")
return