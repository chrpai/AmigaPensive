/* TopTen UD ENTRY Display */

options results
getuser 27;terwid = result
if terwid = 80 then call displayfull
else call halfsize
exit
end

displayfull:
sendstring "\f1"
sendfile "gfiles:topten.uloads80"
sendfile "gfiles:topten.dloads80"
sendstring "\n1\c4Press a key..\g1\q1"
exit
end

halfsize:
sendstring "\f1"
sendfile "gfiles:topten.uloads"
sendfile "gfiles:topten.dloads"
sendstring "\n1\c4Press a key..\g1\q1"
exit
end
