
                         ----------------------
                         FAMERemoteControl v1.1
                         ----------------------

 FAMERemoteControl is a tool for FAME BBS (MainPart v1.25 +) which is a
 remote controler of Nodes from CLI.

 FAMERemoteControl is a CLI tool and can be used from CLI and scripts.

 FAMERemoteControl is able to start or quit a specific Node.

 Start: If a Node is Idle (not started) you can start the Node with the
        argument: "STARTNODE".

        If the Node is already running "STARTNODE" will let the Node
        open it's screen and/or brings it to front.

  Quit: If a Node is running you can quit it with the arguments:
        "QUITNODE" or "QUITNODEOH".

        "QUITNODEOH" will quit the Node Off Hook (Modem OH).

        Note: It's not possible to quit a Node with "QUITNODE" and
              "QUITNODEOH" if it's not awaitng a call.

 Argument template for FAMERemoteControl:

 "NODE/K/N/A,STARTNODE/S,QUITNODE/S,QUITNODEOH/S"

 Example FAMERemoteControl argument string:

 'FAME:Modules/FAMERemoteControl NODE=1 STARTNODE'

 Argument switch priority:
              
 1. STARTNODE
 2. QUITNODE
 3. QUITNODEOH

 FAMERemoteControl informs the Server about your request and sends the
 needed commands to the depending Node.


------------------------------------------------------------------------------

FAMERemoteControl v1.1 programmed by:

--------------------------------- ______  ____ :___  _________ -----
David 'Strider/tRSi' Wettig       \  __/_/    \|   \/    /  _/___
E-Mail: strider@trsi.de           |    _//     \   \/   /  _|   /
E-Mail: strider@deadline.snafu.de |____|________\__/   /|_______\tGø
URL: http://www.trsi.de/inno/strider/index.html   /____\ Developer
--------------------------------------------------------------------
<EOF>
