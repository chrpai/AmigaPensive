/* Complete TopTen Display */

options results
getuser 27;terwid = result
if terwid = 80 then call displayfull
else call halfsize
exit
end

displayfull:
sendfile "gfiles:topten.calls80"
transmit ""
sendfile "gfiles:topten.mess80"
transmit ""
sendfile "gfiles:topten.dloads80"
transmit ""
sendfile "gfiles:topten.uloads80"
transmit ""
sendfile "gfiles:Top 20 downloads"
sendstring "\n1\c1<hit a key>\g1\q1\n1"
exit
end

halfsize:
sendfile "gfiles:topten.calls"
transmit ""
sendfile "gfiles:topten.mess"
transmit ""
sendfile "gfiles:topten.dloads"
transmit ""
sendfile "gfiles:topten.uloads"
transmit ""
sendfile "gfiles:Top 20 downloads.40"
transmit "\n1\c1<hit a key>\g1\q1"
exit
end
