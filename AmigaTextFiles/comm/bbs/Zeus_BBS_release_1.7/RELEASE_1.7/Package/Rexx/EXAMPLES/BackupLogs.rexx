/*
 * BackupLogs.rexx by Neil Williams (30.10.98), based on code from:
 *
 * Backup.rexx By Dave Naylor 25-1-97
 *
 */

options results
failat 21

/*    IF( ~show( 'l', "rexxtricks.library" ) ) then DO
        IF( ~addlib( "rexxtricks.library", 0, -30, 0 ) ) THEN DO
            SAY "Could not open rexxtricks.library"
            EXIT 10
        END
    END


    MAIL = GETDIR(INBOUND,'#?.(pkt|mo?|tu?|we?|th?|fr?|sa?|su?)','BACKUP','FILES','PATH')  */

/*    IF MAIL = 0 THEN DO
        SAY 'Sorry, No Mail to backup'
        EXIT
    END  */

KIDD = DATE(S)

Address Command

if exists( 'Logs:MailManager.oldlog' ) then do
    'Copy Logs:MailManager.oldlog to Mail:Back/MailManager.log.'||KIDD' CLONE'
    'lzx -3 a Mail:Back/Logs_'||KIDD' Mail:Back/MailManager.log.'||KIDD
    if RC = 0 then do
       'delete' 'Mail:Back/MailManager.log.'||KIDD
       'delete' 'Logs:MailManager.oldlog'
    end
end

if exists( 'Logs:ZMailer1.log' ) then do
    'Copy Logs:ZMailer1.log to Mail:Back/ZMailer1.log.'||KIDD' CLONE'
    'lzx -3 a Mail:Back/Logs_'||KIDD' Mail:Back/ZMailer1.log.'||KIDD
    if RC = 0 then do
       'delete' 'Mail:Back/ZMailer1.log.'||KIDD
       'delete' 'Logs:ZMailer1.log'
    end
end

if exists( 'Logs:Gotcha.log' ) then do
    'Copy Logs:Gotcha.log to Mail:Back/Gotcha.log.'||KIDD' CLONE'
    'lzx -3 a Mail:Back/Logs_'||KIDD' Mail:Back/Gotcha.log.'||KIDD
    if RC = 0 then do
       'delete' 'Mail:Back/Gotcha.log.'||KIDD
       'delete' 'Logs:Gotcha.log'
    end
end

if exists( 'Logs:ZMP.log' ) then do
    'Copy Logs:ZMP.log to Mail:Back/ZMP.log.'||KIDD' CLONE'
    'lzx -3 a Mail:Back/Logs_'||KIDD' Mail:Back/ZMP.log.'||KIDD
    if RC = 0 then do
       'delete' 'Mail:Back/ZMP.log.'||KIDD
       'delete' 'Logs:ZMP.log'
    end
end


EXIT


