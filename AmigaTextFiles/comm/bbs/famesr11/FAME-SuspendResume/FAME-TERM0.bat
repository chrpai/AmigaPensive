
; FAME-TERM0.bat by David 'Strider' Wettig.
;
; Script to check for FAME Node 0.
;
; If Node 0 exists suspend it via Arexx-script and start TERM instead.
; After quiting TERM resume Node 0.
;
; If Node 0 doesn't exist just start TERM.

FailAt 100

FAME:Modules/CheckForNode NODENR=0 LOCATION=FAME:

If Not Fail

	Echo "Suspending FAME Node 0."

	rx FAMEOFF.rexx

	Echo "Starting TERM instead."

	stack 16384
	TERM:TERM sync

	Echo "Resuming FAME Node 0."

	rx FAMEON.rexx

Else

	Echo "Starting TERM."

	TERM:TERM sync

Endif

Echo ""

