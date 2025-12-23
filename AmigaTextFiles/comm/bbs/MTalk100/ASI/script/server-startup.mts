/alias # /comment
# Server-Startup.mts
# -- MTalk script that the server executes when it runs.
# note: userlevel 255 is typically the SysOp level.

/set SYSOP 255

# run everything in superuser mode
/power on

/alias * /config door
# configure some doors
# door name  level  DOS command with codes
# ---------  -----  ----------------------
* banner     0      <DOORS:ASI/BIN/Banner-F1 \$1+
* cls        0      echo "*Ec"
* dos        $SYSOP \$1+
* eval       0      <rx "say \$1"
* finger     0      <DOORS:ASI/BIN/UList \$1+ EXACT NOHEAD LFORMAT "%NAME (%ALIAS) lives in %CITY, %PROVINCE.\\n  Aged at %AGE.  Computing with an %COMPUTER!\\n  Last call on %LASTCALL\\n----"
* fortune    0      <DOORS:ASI/BIN/ASI-Fortune DATA:TEXT/Fortune.data
* lookup     0      <DOORS:ASI/BIN/TeleRegShowRecord DISPLAY COLOR NAME \$1+
* motd       0      <type DOORS:ASI/TEXT/MTalk.motd
* time       0      <DOORS:ASI/BIN/Digit-Clock QUICK
* tpage      0      <DOORS:ASI/BIN/TelePage "\$REAL" \$1 \$2+
* twho       0      <DOORS:ASI/BIN/TeleWho \$PORT
* version    0      <version DOORS:ASI/BIN/MTalkD full
* _about     0      <echo help: please type /help \$0
* _codes     0      <echo help: please type /help \$0
* _doors     0      <echo help: please type /help \$0
* _editing   0      <echo help: please type /help \$0
* _flags     0      <echo help: please type /help \$0
* _intro     0      <echo help: please type /help \$0
* _vars      0      <echo help: please type /help \$0

# execute debugging script. don't worry if it's not found.
/exec script/debug.mts

/config awaytimeout 5
/config arexxlevel  $SYSOP
# arexx clients will enter mtalk with level $SYSOP

/config setlevel config $SYSOP
/config setlevel power  $SYSOP

/alias * /config script
# configure some scripts
# script alias      actual filename (* = substitute for user name)
# ------------      ---------------
* SYS-LOGIN         SCRIPT/user-startup.mts
* SYS-LOGOUT        SCRIPT/user-exit.mts
* USER-LOGIN        USER/*/login.mts
* USER-LOGOUT       USER/*/logout.mts
* USER-ENV          USER/*/env.mts
* DEFAULT-ENV       CONFIG/default.env.mts

# let people /TALK over the network
# /config TalkOverNet Y

# create the default immortal channel
/enter pub
/mode +i
/exit pub

/exec data/actions.mts

/unalias #
/unalias *
