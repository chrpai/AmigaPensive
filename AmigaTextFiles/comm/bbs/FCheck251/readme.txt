

BUG in FileCheck 2.50:

There was an error in the last release. If no FILE_ID was found, sometimes
a @hlp-file was created. I have fixed the problem and I hope it works
fine.


INFO to the next generation of FileCheck:

FileCheck is not longer developed in ARexx. I'm working on a fully
C-Version, because it's faster and there will be more features inside:

- outputs can 100% configured
- no parameter needed for running (env-vars will be used)
- own filetype-scanner (faster, because no libs needed)
- and other things, I think about ;)

For other bug-reports or mail-bombs send me an E-Mail to:

Cannon@outside.in-berlin.de
