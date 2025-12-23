/* Top Files downloaded Display */

options results
getuser 27;terwid = result
if terwid = 80 then call displayfull
else call halfsize
exit
end

displayfull:
sendfile "gfiles:Top 20 downloads"
exit
end

halfsize:
sendfile "gfiles:Top 20 downloads.40"
exit
end
