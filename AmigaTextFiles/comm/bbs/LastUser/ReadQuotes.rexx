/* Read quotes that users have left */

options results

'transmit' "\f1 \z4\c3 Q U O T E S \q1 v1.00 by Renze de Ruiter for Excelsior!\n1               (c)1993 All Rights Reserved\n1"

ret = open(quotes,'EX.Quotes','R')
if ret = 0 then do
	'transmit' "\n1Could not open the EX.Quotes file!\n1"
	'logentry' "Could not open the EX.Quotes file!"
	exit
end
if ret = 1 then do
	do q = 1 to 50
		quote.q = readln(quotes)
	end
end

loop:
do d = 1 to 5
	qd = trunc(randu()*50)+1
	'transmit' quote.qd
end

'sendstring' "\c2Again? \c7(y/N)\q1 "
'getchar'
yesno = upper(result)
if yesno = "Y" then do
	'transmit' "Yes\n1"
	signal loop:
end
'transmit' "No\n1"
'bufferflush'
exit
