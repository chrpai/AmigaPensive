CC = -v -ccsmuq -rr -j121 $(GO)

xprd: initio.o intuition.o main.o doio.o pn.o 
    blink lib:c.o,main.o,initio.o,doio.o,pn.o,intuition.o to xprd lib lib:lcr.lib nd sc sd define @_main=@_tinymain

initio.o: initio.c
    lc $(CC) $(30) -mt initio

intuition.o: intuition.c
    lc $(CC) $(30) -mt intuition

main.o: main.c
    lc $(CC) $(30) -mt main

doio.o: doio.c
    lc $(CC) $(30) -mt doio

pn.o: pn.c
    lc $(CC) $(30) -ms pn
