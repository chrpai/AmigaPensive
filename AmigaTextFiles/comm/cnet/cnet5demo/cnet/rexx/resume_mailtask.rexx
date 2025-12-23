/* Script to pause mail-task mail processing */

options results

address MAILTASK_RX.1

RESUME

say 'Mail-task status: ' RESULT
