DEFSNG a-z
PRINT :PRINT :PRINT 
PRINT "Nim Games File Generator"

loop:

PRINT:PRINT
PRINT "This prog is highly destructive."
PRINT "It wipes out all your nim games completely."
PRINT "Okay, so are you sure you want to run it?"
PRINT "You must type in 'YES' in full!!"
PRINT
INPUT sure$
IF UCASE$(sure$)<>"YES" THEN END

PRINT :PRINT 
PRINT "What's the pathname of where the door is kept?"
PRINT "       (EG 'DH1:Doorfiles1/Door21/' ) "
PRINT 
PRINT " << Don't forget the trailing slash! >> "
PRINT 
INPUT path$
PRINT :PRINT 
PRINT path$; "  ?"
PRINT 
INPUT "Is that correct (y/N) ";check$
IF UCASE$(LEFT$(check$,1))<>"Y" THEN loop:

aa=0
flnm$=path$+"nim.games"

OPEN flnm$ AS #2 LEN=83
FIELD #2, 4 AS a$(0), 5 AS a$(1), 25 AS a$(2), 5 AS a$(3),25 AS a$(4), 10 AS a$(5), 5 AS a$(6), 3 AS a$(7), 1 AS a$(8)


FOR k=1 TO 99

  LSET a$(0) = "0"
  LSET a$(1) = "0"
  LSET a$(2) = " "
  LSET a$(3) = "0"
  LSET a$(4) = " "
  LSET a$(5) = "Never"
  LSET a$(6) = "0"
  LSET a$(7) = "0"
  LSET a$(8) = "0"

  PUT #2,k
NEXT k
CLOSE #2

PRINT :PRINT :PRINT 
PRINT  "The blank Games file has now been generated."
PRINT :PRINT 
INPUT "Press Return to exit"; x$
END

