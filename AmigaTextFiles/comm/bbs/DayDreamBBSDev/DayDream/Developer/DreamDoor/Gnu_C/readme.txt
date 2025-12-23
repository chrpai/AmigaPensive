FD file for DreamDoor.library
-----------------------------

Hydra  did  write  prototype  file  only for SasC, but since the SAS Institute
won't  develop  the compiler any further, I decided to start using the one and
only,  Gnu  CC.  Of course I ran into problem that GCC's inlining system won't
work  without  proper  inline  -headerfile.   So here is my solution, handmade
dddoor_lib.fd  -file  and  inlines  for GCC created with fd2inline from fd and
little modified version of proto/dddoor.h

You  should  put  these  files to your /ade/os-include/ -directory, except the
inline  -header,  which  belongs  (in theory) to /local/include/inline/, but I
keep it in /ade/include/inline/

The  dddoor_lib.fd  is  quickly  handmade  by NAiG/aBUSE, and after quick
testing  it  seems to work.  If you encounter any bugs or typos, please report
them to me <jajoki@utu.fi> or ORiGO.

You can also remake the inline file with fd2inline -program with commandline
"fd2inline ADE:lib/fd/dddoor_lib.fd ADE:os-include/clib/dddoor_protos.h -o
ADE:include/inline/dddoor.h"

P.S. here are only the modified files, rest of Sas_C/ -dir's files work
without modifications.

/ NAiG/Abuse

From 1.26 and up:
I have updated the structures and library protos as far as I have been able.
I relly hope they work allright, but please tell me if they dont!

/ Mattias Nilsson
