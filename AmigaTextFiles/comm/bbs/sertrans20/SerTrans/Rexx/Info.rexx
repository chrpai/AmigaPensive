/* Shows if someone is online */

OPTIONS RESULTS

PortID = 0

ADDRESS 'rexx_SERTRANS3'


/* Let's get all LibraryBases... */

IsServer;server = result

if server = 0 then Say 'The Server is not running... but who cares'
else
  Say 'The Server is running.. So I guess SerTrans gets his datas from the Server'

PortNumber	;id = result
SerDevice	;device = result
SerDTE		;dte = result
SerConnect	;connect = result
SerUnit		;unit = result
PathUpload	;upload = result
PathDownload	;download = result

Say 'The Port ' || id || ' of SerTrans is currently running!'

Say 'SerTrans has opened the ' || device || ' on Unit ' || unit '!'

Say 'The DTE-Rate is ' || dte || ' and the connected Speed of the User is ' || connect || '!'

Say '0 Means, that no one is connected :-)'

Say 'When the Sysop thinks about to load something up or down, he will use'

Say download || ' for donwloading and ' || upload || ' for uploading.'


