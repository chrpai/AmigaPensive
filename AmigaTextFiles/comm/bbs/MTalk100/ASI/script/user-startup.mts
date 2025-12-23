/alias #            /comment
/alias /accept      /channel \$inviter
/alias /bye         /quit
/alias /c           /channel
/alias /ch          /channel
/alias /channel     /enter
/alias /ignore      /forget
/alias /inv         /invite
/alias /man         /help
/alias /monitor     /scan
/alias /mon         /monitor
/alias /print       /echo
/alias /private     /mode +p
/alias /public      /mode -p
/alias /rem         /remember
/alias /remember    /unforget
/alias /q           /quit
/alias /type        /echo
/alias /uni         /uninvite
/alias /whisper     /msg
/alias /w           /whisper
/alias /x           /quit
/alias /s           /list all
/alias /?           /help
/alias /topic       /mode +t
/alias /unmonitor   /unscan
/alias /unmon       /unmonitor
/if $level == 255 /alias ! /dos
#   ^^^^^^ this is only good if you pass the LEVEL option when you
# run the MTalk client.  Otherwise, the user can do a /set LEVEL 255
# and run DOORS that they are not suppose to run.

/echo ^JType /help for help\; /setenv to set environment\; /quit to quit^J

/exec quiet DEFAULT-ENV
/exec quiet USER-LOGIN

/setenv load

# Is MTalk used as a SysOp<->User chat program?  note: the \\\; will shrink to a single \; and then command chaining continues.
/if "$talk$ansi" == YY /setenv TalkOutWinHeight 0\;/setenv QuitAfterTalk Y\;/unsetenv DefaultChannel\;/setenv WinTitle Multi-Talk by Sam Yee -- \\\$User chatting @ \\\$time
/if "$talk$ansi" == YN /enter chat\;/unsetenv DefaultChannel
/if "$talk$ansi" == YN /if "$talkto" == "\\\$talkto" /mode +l 2
/if "$talk $talkto $ansi" == "Y \$talkto Y" /setenv OnTalk /talk \\\$sender
/if "$talk$ansi" == YY /if "$talkto" != "\$talkto" /talk \$talkto

# go to specified default channel
/if "$DefaultChannel " != "\$DefaultChannel " /enter $DefaultChannel ELSE /setenv DefaultChannel ?

# check who's in the current channel
/echo ^M
/list .
/echo ^M

/unalias #
