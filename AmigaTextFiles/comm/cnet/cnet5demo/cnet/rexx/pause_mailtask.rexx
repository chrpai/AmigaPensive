/* Script to pause mail-task mail processing */

options results

address MAILTASK_RX.1

PAUSE

say 'Mail-task status: ' RESULT
