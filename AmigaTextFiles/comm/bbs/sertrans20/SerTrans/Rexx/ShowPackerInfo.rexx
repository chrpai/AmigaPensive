/* Shows if someone is online */

OPTIONS RESULTS

PortID = 0

ADDRESS 'rexx_SERTRANS0'


/* Let's get all Packerstrings.. */

ArcPacker

Say 'The selected Packer is ' || result

Say 'So lets see what we can get for ' || result

ArcAdd;add = result;say 'Adding    : ' || add
ArcExt;ext = result;say 'Extracting: ' || ext
ArcLst;lst = result;say 'Listing   : ' || lst
ArcTst;tst = result;say 'Testing   : ' || tst
ArcDel;del = result;say 'Deleting  : ' || del

Archive

Say '...and the Archive itself is ' || result

