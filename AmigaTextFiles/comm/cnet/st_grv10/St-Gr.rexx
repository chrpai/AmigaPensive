/*************************************************************/
/* Stat-Graph V1.0 by Blowfly - Lego Land BBS 49(0)9227-2542 */
/*************************************************************/

options results ; ss =  sendstring ; tr = transmit
getuser 1 ; Han = result ; Han = left(Han,22)
getuser 3 ; RName = result ; RName = left(RName,24)	
getuser 4 ; City = result ; city = left(city,24)
getuser 6 ; loc = result ; loc = left(loc,23)
getuser 11 ; lc = result ; lc = left(lc,23)
getuser 12 ; Time = result ; time = left(time,24)
getuser 21 ; GP = result ; GP = left(GP,24)	
getuser 23 ; Port = result ; port = left(port,7)	
getuser 24 ; Baud = result ; getuser 28 ; TT = result ; getuser 29 ; TP = result	
getuser 41 ; SerId = result ; serid = left(serid,23)
getuser 15 ; axl = result ; axl = left(axl,23) ; getuser 16 ; axn = result ; axn = left(axn,24)
getuser 1400472 ; totcalls = result ; totcalls = left(totcalls,23)
getuser 1400484 ; upb = result ; upb = left(upb,23)
getuser 1400492 ; crd = result ; crd = left(crd,24)
BaudRate = baud * 10 ; ; baudrate = left(baudrate,11)
if tt = 0 then term = "ASCII Grafix           " ; if tt = 1 then term = "C-64 Grafix            " 
if tt = 2 then term = "IBM Grafix             " ; if tt = 3 then term = "Amiga Int`l            "
if tt = 4 then term = "SkyPix Grafix          "
if TP = 65 then proto = "ASCII                   " ; if TP = 42 then proto = "ZModem* (CRC-32 Bit)    " 
if TP = 90 then proto = "ZModem (CRC-16 Bit)     " ; if TP = 88 then proto = "XModem                  "
if TP = 89 then proto = "YModem                  " ; if TP = 71 then proto = "YModem-G (CRC-32 Bit)   " 
if TP = 83 then proto = "ZSpeed (CRC-16 Bit)     "
ss "\f1\c7    Stat-Graph V1.0 by Blowfly of PoSSeSSeD - The Lego Land ++49(0)9227-2542"
ss "\n1\c5 +----------------------------------------------------------------------------+"
ss "\n1 |\c3Handle\c1        :\c6"Han"\c3 Real Name\c1    :\c6"RName"\c5|"
ss "\n1 |\c3Address\c1       :\c6"loc"\c3City\c1         :\c6"city"\c5|"
ss "\n1 |\c3Last Call\c1     :\c6"lc"\c3Now\c1          :\c6"time"\c5|"
ss "\n1 |\c3Calls Total\c1   :\c6"totcalls"\c3Game Points\c1  :\c6"GP"\c5|"
ss "\n1 |\c3Access Level\c1  :\c6"axl"\c3Access Name\c1  :\c6"axn"\c5|"
ss "\n1 |\c3Serial ID\c1     :\c6"serid"\c3Baud Rate\c1    :\c6"baudrate"\c3Port\c1 :\c6"port"\c5|"
ss "\n1 |\c3Terminal\c1      :\c6"term"\c3Protocol\c1     :\c6"proto"\c5|" 
ss "\n1 |\c3KB Uploaded\c1   :\c6"upb"\c3KB Download\c1  :\c6"crd"\c5|"
ss "\n1 +----------------------------------------------------------------------------+"
getuser 1400484 ; ub = result ; ub = ub / 2000 ; ub = ub + 1 
getuser 1400492 ; db = result ; db = db / 2000 ; db = db + 1
getuser 1400504 ; cr = result ; cr = cr / 2000000 ; cr = cr + 1
if ub <= 26 then do
tr "\q1\n1                        \z4\c6  You Uploaded (Megabytes):  \q1\n1"
tr "\c3 0             10             20             30             40             50\q1"
tr " \c1:..:..:..:..:..:\c2..:..:..:..:..:..:..:..:..:..:..:..:..:..:..:\c3..:..:..:..:..:\q1"
do i = 1 to ub 
ss '  \r1\c2 ' ; end i ; transmit '\r0\q1'
end
if ub > 26 then do
ub = ub / 10
if ub > 26 then ub = 26
tr "\q1\n1                        \z4\c6  You Uploaded (Megabytes):  \q1\n1"
tr "\c3 0             100            200            300            400            500\q1"
tr " \c1:..:..:..:..:..:\c2..:..:..:..:..:..:..:..:..:..:..:..:..:..:..:\c3..:..:..:..:..:\q1"
do i = 1 to ub 
ss '  \r1\c2 ' ; end i ; transmit '\r0\q1'
end
if Db <= 26 then do
tr "\q1\n1                       \z4\c6  You Downloaded (Megabytes):  \q1\n1"
tr "\c3 0             10             20             30             40             50\q1"
tr " \c1:..:..:..:..:..:\c2..:..:..:..:..:..:..:..:..:..:..:..:..:..:..:\c3..:..:..:..:..:\q1"
do i = 1 to db 
ss '  \r1\c1 ' ; end i ; transmit '\r0\q1'
end
if db > 26 then do
db = db / 10
if db > 26 then db = 26
tr "\q1\n1                       \z4\c6  You Downloaded (Megabytes):  \q1\n1"
tr "\c3 0             100            200            300            400            500\q1"
tr " \c1:..:..:..:..:..:\c2..:..:..:..:..:..:..:..:..:..:..:..:..:..:..:\c3..:..:..:..:..:\q1"
do i = 1 to db 
ss '  \r1\c1 ' ; end i ; transmit '\r0\q1'
end
if cr <= 26 then do
tr "\q1\n1                        \z4\c6  Your Credits (Megabytes):  \q1\n1"
tr "\c3 0             10             20             30             40             50\q1"
tr " \c1:..:..:..:..:..:\c2..:..:..:..:..:..:..:..:..:..:..:..:..:..:..:\c3..:..:..:..:..:\q1"
do i = 1 to cr 
ss '  \r1\c3 ' ; end i ; transmit '\r0\q1'
end
if cr > 26 then do
cr = cr / 10
if cr > 26 then cr = 26
tr "\q1\n1                        \z4\c6  Your Credits (Megabytes):  \q1\n1"
tr "\c3 0             100            200            300            400            500\q1"
tr " \c1:..:..:..:..:..:\c2..:..:..:..:..:..:..:..:..:..:..:..:..:..:..:\c3..:..:..:..:..:\q1"
do i = 1 to cr 
ss '  \r1\c3 ' ; end i ; transmit '\r0\q1'
end
