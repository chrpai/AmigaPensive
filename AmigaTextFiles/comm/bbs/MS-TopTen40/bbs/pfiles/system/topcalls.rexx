/* TopTen UD ENTRY Display */

options results
getuser 27;terwid = result
if terwid = 80 then call displayfull
else call halfsize
exit
end

displayfull:
sendfile "gfiles:topten.calls80"
exit
end

halfsize:
sendfile "gfiles:topten.calls"
exit
end
