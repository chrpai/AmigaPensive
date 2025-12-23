/*  Mail Run with FOOZLE! run from trapdoor after mail is recieved from
    another system */

/* AFTERSESSION 'rx fooz.rexx' */

options results

address command "bbs:bin/TrAL L"
address "Foozle"
rescan all
maint
import
renum all
rescan MATRIX
export
address command 'overnight'
address command 'bbs:bin/TrAL U'

