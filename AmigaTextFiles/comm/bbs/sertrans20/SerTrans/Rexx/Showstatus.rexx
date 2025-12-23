/* Shows if someone is online */

OPTIONS RESULTS

PortID = 0

ADDRESS 'rexx_SERTRANS0'


/* See if SerTrans is online */

STATUS

if RESULT = 'ONLINE' then 
  say 'Port ' || PortID || ' is online!'
else
  say 'Port ' || PortID || ' is offline!'


/* But is there a Carrier? */

CHECKCARRIER

if RESULT = 'YES' then 
  say 'A Carrier was on Port ' || PortID || ' detected!'
else
  say 'No Carrier detected on Port ' || PortID || '!'


/* Let's flash the SerTrans' Screen */

Beep

