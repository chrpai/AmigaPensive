/* 

script to recieve Fax



GP-FAX must always be loaded and you must have it set to FAX/DATA but
disable auto answer and in ya startup sequence after gpfax has had time to
initialise and setup the modem (should have the same DTE rate as the
tempest node u want to use as a fax)

put the lines: (for example)

run >nil: bbs:tempest 1 tempest:
wait 2
run >nil: gpfax:gpfax -s -l19200 -q
wait 4
rx "" address "REXX_GPFAX" closeserial



 */

options results

if ~show('P','REXX_GPFAX') = 1 then do
 say 'Sorry GPFax not loaded...'
 exit 1
end

address "REXX_GPFAX"
openserial
fconreceive
closeserial

exit 0

