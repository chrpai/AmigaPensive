/*************************************************/
/*                                               */
/* Hier das REXX-Script des SETFREE Befehl...... */
/*                                               */
/* Dieser Befehl ist extra in REXX damit ihn sich*/
/*    jeder selber ggf. anpassen kann            */
/*                                               */
/*************************************************/

if ~show('L','cl_rexx.library') then do
    if ~addlib('cl_rexx.library',0,-30,0) then do
        address command 'ASSIGN LIBS: CONNECTLINE:Libs ADD'
        if ~addlib('connectline:libs/cl_rexx.library',0,-30,0) then exit 10
        end
    end


anzahl=CLGET_UserNumberOf()
do x=1 to anzahl
    user.x  = CLGET_Username(x)
    uploads = clget_useruploads(user.x)
    down    = clget_userdownloads(user.x)

free = uploads * 3                                /*   Rate ( 3 fuer 1:3 )   */

free = free - down

if free < 512 then free = 512                     /* mindest Free der eingetragen wird ( in KB ) */
say ' '
say ' '
say '-------------------------------------------'
say 'Username........ : 'user.x
say ' '
say 'Uploads......... : 'left(uploads,9)||' ByTES'
say 'Downloads....... : 'left(down,9)||' ByTES'
say 'Neuer Freiraum.. : 'left(free,9)||' ByTES'
dummy   = clset_userfreekbyte(user.x,free )
say ' '
end
call ende


Ende:
say ' '
say ' '
say '-------------------------------------------'
EXIT(0)
