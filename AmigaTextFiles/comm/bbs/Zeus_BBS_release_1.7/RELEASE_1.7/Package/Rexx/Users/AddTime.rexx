/*
** Add 15 minutes to User's time
*/

Options Results
Parse Arg Line

Address 'ZEUS'

GetUserData 'MinsLeft'
if RC = 0 then do
    Mins = RESULT
    Mins = Mins + 15
    SetUserData 'MinsLeft' Mins
    end
