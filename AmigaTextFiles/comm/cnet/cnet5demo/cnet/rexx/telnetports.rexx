/* one possible method of loading telnet ports  */

signal on syntax
signal on error
firstport=4
lastport=5

if show('P',MIAMI.1) then do
   do port=firstport to lastport
      address CONTROLREXX.1 'RUNPORT 'port
   end
end
