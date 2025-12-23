;
; FastScan - AMMS-Batch V2.1  by cANNoN/iDP!
;
; $VER: 2.1 / 02.07.1997
;

;
; FastScan: It is useful to execute this batch within a event.
;           Access for the event-user to the command 'extern' is needed
;           if you want to start this batch.
;
;           A file called 'FileCheck.fast' will be created in the
;           directory of FileCheck. This file will be used to speed up
;           the Duplicate-Checker. To disable this function do you can
;           delete 'FileCheck.fast' or you can use the parameter
;           'NOTURBO'.
;

_extern/quiet mbeigene:Cannon/FileChecker/FileCheck TURBOSCAN
