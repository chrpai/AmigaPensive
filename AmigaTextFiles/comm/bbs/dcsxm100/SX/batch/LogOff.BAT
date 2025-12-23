.key node

IF "<node>" EQ "2"
	QUIT			; Quit if node 2 (my local node)
ENDIF

SX:Utils/SAmiLog -UC"<node>" -O"SX:Screens/LastCallers.ANS"10 >NIL:
