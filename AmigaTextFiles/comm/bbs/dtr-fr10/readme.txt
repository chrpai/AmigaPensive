Short:    BBSCMD/FR (FileReverse) door for Fame 
Uploader: Gravedancer@PoBoxes.Com (Sebastian Bergmann)
Author:   Gravedancer@PoBoxes.Com (Sebastian Bergmann)
Type:     comm/bbs
Requires: F.inal A.miga M.ailbox E.ngine (Fame) 
Replaces: Any other BBSCMD/FR door/tool 

Another FileReverse [FR] for Fame, you might think. But wait... Why, another?
This is as far as I know the first FileReverse door explicitly written for
Fame, and not just a XIM emulated one like ByteAndi's NewScan, which is
supplied with the Fame package. (Yes, i know that Scandic/Decision once started
a FileReverse door for Fame. But I think he canceled this project long ago ;)

Let's talk about features:

	o Written in SAS/C 6.58 exclusively for Fame (Tested with Fame MP:1.29/SV:1.24/SE:1.23 registered)
	o Using fame.library and utility.library to avoid big/slow ANSI-C standard
    functions

	o Flagging using Cursor-Keys (Numbers and Filename not included in this version)
	o Displays how often each file got downloaded
	o Optimized memory usage

	o Sysop can configure:
			- Memory usage (fixed buffer-size or dynamic buffer-size)
			- Display Filenumbers on default [yes/no]
			- Display DlTimes of each file on default [yes/no]
			- Default colours

	o Each user can configure:
			- Display Filenumbers [yes/no]
			- Display DlTimes of each file [yes/no]
			- Colours

--

  COPYRIGHT
 -----------

 The copyright for this software is by Sebastian Bergmann. This archive may
 only be distributed in unmodified form. In particular the deletion of
 the documentation and this copyright-note is not allowed.
 
 
  DISTRIBUTION
 --------------

 This software is freely distributable. The distribution is permitted under
 the following conditions:
 
 - All associated files included with the distribution archive are to
   remain intact and unaltered.
 
 - The distribution over BBS's, Internet, software libraries like the ones
   from Fred Fish or Aminet CD-ROM's and similar electronic channels is
   granted.
 
 - For disc-magazines and service providers, who rise additional costs for
   the file transfer, the permission of the author is required.
  
 
  DISCLAIMER
 ------------

 This software is provided as-is, without warranty of any kind, either
 expressed or implied. In no event will the author be liable for direct,
 indirect, incidental or consequential damages or data loss resulting from
 the use or application of this software. The entire risk as to the results
 and performance of this software is assumed by the user.
