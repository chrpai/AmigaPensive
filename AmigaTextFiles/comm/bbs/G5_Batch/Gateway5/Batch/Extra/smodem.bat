.key path
.bra {
.ket }
failat 30
Requestfile >T:Files.un DRAWER {path} TITLE "Tag files..." MULTISELECT NOICONS
if warn
	delete >NIL: T:Files QUIET
	smodem device serial.device unit 0 baud 38400 dlpath {path} ctsrts chkcarrier overwrite
else
	tr <T:Files.un >T:Files
	smodem device serial.device unit 0 baud 38400 dlpath {path} ctsrts chkcarrier overwrite ullist t:Files
endif
delete >NIL: T:Files.un QUIET


; Edit Smodem device blabla string to suit your needs!!!
