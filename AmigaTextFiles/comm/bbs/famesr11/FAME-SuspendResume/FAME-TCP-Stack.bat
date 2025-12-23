
; FAME-TCP-Stack.bat by David 'Strider' Wettig.
;
; Script to check for FAME Node 0.
;
; If Node 0 exists suspend it via Arexx-script and start Miami instead.
; After quiting Miami resume Node 0.
;
; If Node 0 doesn't exist just start Miami.

FailAt 100

FAME:Modules/CheckForNode NODENR=0 LOCATION=FAME:

If Not Fail

	Echo "Suspending FAME Node 0."

	rx FAMEOFF.rexx

	Echo "Starting Miami instead."

	Work:Internet/MIAMI/Miami

	Echo "Resuming FAME Node 0."

	rx FAMEON.rexx

Else

	Echo "Starting Miami."

	Work:Internet/MIAMI/Miami

Endif

Echo ""

