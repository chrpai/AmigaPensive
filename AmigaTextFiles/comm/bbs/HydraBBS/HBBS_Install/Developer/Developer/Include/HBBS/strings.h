
// only the node and control source includes this file.  cut and paste if you want to use these strings
// in your own programs

#ifdef MAIN


  // Common strings for all doors and programs and whatever...
  UBYTE *str_NODE="Node",
        *str_OK="Ok!", // used for error messages...
        *str_CtrlScrnName = "CtrlScrn",
        *CtrlMainPortName="HBBS_Ctrl_Main",
        *str_CONSOLE="CON:0/400/700/150/HBBS Output Window/SCREEN @S@/AUTO/CLOSE";


  #ifdef HBBS_CONTROL

    // CONTROL only..
    UBYTE *str_STAT_CLOSED="Closed",
          *str_STAT_LOADING="Loading..",
          *str_STAT_INITIALIZING="Initializing..",
          *str_STAT_READY="Ready!",
          *str_STAT_ONLINE="OnLine",
          *str_STAT_CLOSING="Closing..";

  #endif


  #ifdef HBBS_NODE

    // need to be accessable by NODE only
    UBYTE *str_CRLF="\n\r";
    UBYTE *str_CLS="\014\033[2J\033[1;1H\0";

    UBYTE str_CSI_CURSORUP[]={'A',0x00};
    UBYTE str_CSI_CURSORDOWN[]={'B',0x00};
    UBYTE str_CSI_CURSORLEFT[]={'D',0x00};
    UBYTE str_CSI_CURSORRIGHT[]={'C',0x00};

    UBYTE str_CSI_F1[]={'0','~',0x00};       // F1
    UBYTE str_CSI_F2[]={'1','~',0x00};
    UBYTE str_CSI_F3[]={'2','~',0x00};
    UBYTE str_CSI_F4[]={'3','~',0x00};       // ...
    UBYTE str_CSI_F5[]={'4','~',0x00};
    UBYTE str_CSI_F6[]={'5','~',0x00};
    UBYTE str_CSI_F7[]={'6','~',0x00};
    UBYTE str_CSI_F8[]={'7','~',0x00};
    UBYTE str_CSI_F9[]={'8','~',0x00};
    UBYTE str_CSI_F10[]={'9','~',0x00};      // F10
    UBYTE str_CSI_SF1[]={'1','0','~',0x00};  // SHIFT + F1
    UBYTE str_CSI_SF2[]={'1','1','~',0x00};
    UBYTE str_CSI_SF3[]={'1','2','~',0x00};  //  ...
    UBYTE str_CSI_SF4[]={'1','3','~',0x00};
    UBYTE str_CSI_SF5[]={'1','4','~',0x00};
    UBYTE str_CSI_SF6[]={'1','5','~',0x00};
    UBYTE str_CSI_SF7[]={'1','6','~',0x00};
    UBYTE str_CSI_SF8[]={'1','7','~',0x00};
    UBYTE str_CSI_SF9[]={'1','8','~',0x00};
    UBYTE str_CSI_SF10[]={'1','9','~',0x00}; // SHIFT + F10

  #endif

#else

  extern UBYTE
        *str_NODE,
        *str_OK,
        *str_CtrlScrnName,
        *str_CONSOLE,
        *CtrlMainPortName;

  #ifdef HBBS_CONTROL

  extern UBYTE
        *str_STAT_CLOSED,
        *str_STAT_LOADING,
        *str_STAT_INITIALIZING,
        *str_STAT_READY,
        *str_STAT_ONLINE,
        *str_STAT_CLOSING;

  #endif

  #ifdef HBBS_NODE

  extern UBYTE
        *str_CRLF,
        *str_CLS,
        str_CSI_CURSORUP[],
        str_CSI_CURSORDOWN[],
        str_CSI_CURSORLEFT[],
        str_CSI_CURSORRIGHT[],
        str_CSI_F1[],
        str_CSI_F2[],
        str_CSI_F3[],
        str_CSI_F4[],
        str_CSI_F5[],
        str_CSI_F6[],
        str_CSI_F7[],
        str_CSI_F8[],
        str_CSI_F9[],
        str_CSI_F10[],
        str_CSI_SF1[],
        str_CSI_SF2[],
        str_CSI_SF3[],
        str_CSI_SF4[],
        str_CSI_SF5[],
        str_CSI_SF6[],
        str_CSI_SF7[],
        str_CSI_SF8[],
        str_CSI_SF9[],
        str_CSI_SF10[];

  #endif

#endif