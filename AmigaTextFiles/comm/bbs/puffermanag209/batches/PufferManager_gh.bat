.key systemname/A,dateiname/A
.bra {
.ket }
;an wem soll die info gehen wenn es mal probleme gibt
EBLcopy {dateiname} sysop
;dieses batch wird mit run gestartet, deshalb muss das batch loeschen:
c:delete {dateiname} quiet
