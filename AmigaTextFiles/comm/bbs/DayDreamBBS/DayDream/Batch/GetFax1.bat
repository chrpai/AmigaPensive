;DD launches this script when it detects a fax call (Connect string is either
;FAX or +FCON).
;
;run gpfax:gpfax -np -q -b38400 -S -L
;
;is a command that should be used to start GPFax. I guess you have to
;start GPFax *BEFORE* DD.
;
;I have never tested this fax thingie so this is quite experimental,
;but if someone finds this working, please tell me..

  rx "Address 'REXX_GPFAX' 'openserial'"
  rx "Address 'REXX_GPFAX' 'fconreceive'"
  rx "Address 'REXX_GPFAX' 'closeserial'"
