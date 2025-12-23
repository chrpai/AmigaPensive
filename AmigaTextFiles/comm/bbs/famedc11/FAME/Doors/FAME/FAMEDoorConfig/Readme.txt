
                          FAMEDoorConfig.FIM v1.1
                         ~~~~~~~~~~~~~~~~~~~~~~~~~



FAMEDoorConfig.FIM is coded by: David 'Strider/tRSi' Wettig
                        e-mail: strider@deadline.snafu.de




FAMEDoorConfig.FIM needs FAME MainPart v1.13 to run.

FAMEDoorConfig.FIM is a FAME door to start foreign door type doors like
XIM, AIM, etc... as a FAME DoorConfig door.

DoorConfig doors are doors which are config/setup doors for a door to let
users configure their own settings for example for a newfiles door.

DoorConfig doors can be extra doors or can be a part of the main door, but
called with an argument to reach the config part.

DoorConfig doors can only be FIM doors and no other door type.

FAMEDoorConfig.FIM is for all SysOps who are needing to start foreign
DoorConfig doors and not only FIM ones.

In this case FAMEDoorConfig.FIM will help.

FAMEDoorConfig.FIM will be called instead of the foreign DoorConfig Door.
FAMEDoorConfig.FIM is a FIM door and can run as a FAME DoorConfig door.
FAMEDoorConfig.FIM will then start the foreign DoorConfig door as a
SYSCMD installed door.

What you need to do:

First take the SystemEditor and generate a SYSCMD door entry for your
foreign DoorConfig door. Name it for example 'FDC1'.

FDC1 should call your foreign DoorConfig door.

Now take the SystemEditor and generate a new DoorConfig entry
(SystemEditor -> Doors -> DoorConfig).

Fill the string gadgets and activate the other gadgets as needed, but put
into the 'Door Config Path' gadget not the foreign DoorConfig door but
FAMEDoorConfig.FIM !

Example:

 Door Config Path: 'FAME:Doors/FAME/FAMEDoorConfig/FAMEDoorConfig.FIM'

Put into 'Arguments' gadget the name of the SYSCMD you've generated before.

Example:

 Arguments: 'FDC1'

That's all... but only if the foreign DoorConfig door door doesn't need
arguments for the DoorConfig part.

If it need arguments like 'W' put them after the SYSCMD call name of it,
but put a space between.

Example:

 Arguments: 'FDC1 W'


The pack file 'FAMEDoC1.lha' includes some examples for Siegel's
'NewFiles Config' door which is a XIM door.

In this example the SYSCMD call name for 'NewFiles Config' is named
'NewFilesConfig'.
The DoorConfig data file is named 'NewFilesConfig.dat'.


'NewFiles Config' can also be started without FAMEDoorConfig.FIM, because
newfiles itself exists also in a FIM version and because of this we can
start newfiles directly as a FAME DoorConfig door, but with 'n w' as
argument.

For an example in this case copy and load the DoorConfig data file
'NewFiles.dat' into your SystemEditor.

Note that newfiles needs a BBSCMD called 'Config' installed which calls
'NewFiles Config' and will be called from newfiles.


------------------------------------------------------------------------------

FAMEDoorConfig.FIM v1.1 programmed by:

--------------------------------- ______  ____ :___  _________ -----
David 'Strider/tRSi' Wettig       \  __/_/    \|   \/    /  _/___
E-Mail: strider@trsi.de           |    _//     \   \/   /  _|   /
E-Mail: strider@deadline.snafu.de |____|________\__/   /|_______\tGø
URL: http://www.trsi.de/inno/strider/index.html   /____\ Developer
--------------------------------------------------------------------
<EOF>
